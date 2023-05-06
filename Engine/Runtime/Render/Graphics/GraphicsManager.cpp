#include "CommandManager.h"
#include "GraphicsManager.h"
#include "Runtime/Application/Setting.h"
#include "Runtime/Render/Graphics/Shader/Shader.h"
#include "Runtime/Render/Graphics/Shader/ShaderManager.h"

namespace SaplingEngine
{
	ComPtr<IDXGIFactory4>					GraphicsManager::m_DXGIFactory;
	ComPtr<ID3D12Device>					GraphicsManager::m_D3D12Device;
	uint32_t								GraphicsManager::m_RtvDescriptorSize = 0;
	uint32_t								GraphicsManager::m_DsvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap>			GraphicsManager::m_RtvDescriptorHeap;
	ComPtr<ID3D12DescriptorHeap>			GraphicsManager::m_DsvDescriptorHeap;
	ComPtr<IDXGISwapChain>					GraphicsManager::swapChain;
	ComPtr<ID3D12Resource>					GraphicsManager::swapChainBuffer[swapChainBufferCount];
	ComPtr<ID3D12Resource>					GraphicsManager::depthStencilBuffer;
	int32_t									GraphicsManager::currentSwapChainIndex = 0;
	DXGI_FORMAT								GraphicsManager::swapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT								GraphicsManager::depthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	GraphicsManager::PipelineStateMap	GraphicsManager::m_PipelineStates;
	GraphicsManager::RootSignatureMap	GraphicsManager::m_RootSignatures;
	D3D12_VIEWPORT							GraphicsManager::m_Viewport;
	D3D12_RECT								GraphicsManager::m_ScissorRect;
	std::map<ComPtr<ID3D12Resource>, uint64_t> GraphicsManager::m_UnusedUploadBuffers;
	std::map<ComPtr<ID3D12Resource>, uint64_t> GraphicsManager::m_UsedUploadBuffers;

	/**
	 * \brief	����DX12 Device
	 */
	void GraphicsManager::CreateDevice()
	{
#if defined(DEBUG) || defined(_DEBUG) 
		{// Enable the D3D12 debug layer.
			ComPtr<ID3D12Debug> debugController;
			if (!FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
			}
		}
#endif

		//����DXGI������ö���Կ�������SwapChain
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory)));

		//������ʾ�豸(���豸��Ӧһ����ʾ�������������Կ�������nullptr����ʹ��Ĭ�ϵ���ʾ������)
		const auto result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device));
		if (FAILED(result))
		{//Ĭ����ʾ��������Ӧ���豸����ʧ��
			ComPtr<IDXGIAdapter> pWarpAdapter;
			ThrowIfFailed(m_DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
			ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device)));
		}

		//Set ViewPort And ScissorRect
		const auto width = Setting::ScreenWidth();
		const auto height = Setting::ScreenHeight();
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_ScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };
	}

	/**
	 * \brief	��ʼ��
	 * \param	hWnd		���ھ��
	 * \param	width		���ڿ���
	 * \param	height		���ڸ߶�
	 */
	void GraphicsManager::Initialize(HWND hWnd, uint32_t width, uint32_t height)
	{
		CreateRootSignature();
		CreatePipelineState();

		CreateSwapChainAndDepthStencilBuffer(hWnd, width, height);
	}
	
	/**
	 * \brief	���ô�С
	 */
	void GraphicsManager::OnWindowResize(uint32_t width, uint32_t height)
	{
		////�ͷ�֮ǰ�Ļ���
		//ResetSwapChainAndDepthStencilBuffer(width, height);

		////Resize
		//m_Viewport.TopLeftX = 0;
		//m_Viewport.TopLeftY = 0;
		//m_Viewport.Width = static_cast<float>(width);
		//m_Viewport.Height = static_cast<float>(height);
		//m_Viewport.MinDepth = 0.0f;
		//m_Viewport.MaxDepth = 1.0f;
		//m_ScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };

		////Resize SwapChain
		//m_BackBufferIndex = 0;
		//ThrowIfFailed(swapChain->ResizeBuffers(swapChainBufferCount, width, height, swapChainBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		////��������
		//ThrowIfFailed(CommandManager::m_CommandAllocator->Reset());
		//ThrowIfFailed(CommandManager::m_CommandList->Reset(CommandManager::m_CommandAllocator.Get(), nullptr));
		//
		////���´���Rtv��Dsv
		//CreateRtv();
		//CreateDsv(width, height);

		////ִ�в��ȴ��������
		//CommandManager::ExecuteCommandList();
		//CommandManager::CompleteCommand();
	}
	
	/**
	 * \brief ����
	 */
	void GraphicsManager::Destroy()
	{
	}
	
	/**
	 * \brief	����Ĭ�ϻ��������ϴ�����
	 * \param	initData		��ʼ������
	 * \param	byteSize		���ݴ�С
	 * \return	Ĭ�ϻ�����
	 */
	ComPtr<ID3D12Resource> GraphicsManager::CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize)
	{
		ComPtr<ID3D12Resource> defaultBuffer;

		//���ô洢��Դ�Ķѣ�GPU�Դ棩������
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		//������Դ����
		D3D12_RESOURCE_DESC bufferDesc
		{
			D3D12_RESOURCE_DIMENSION_BUFFER,
			0,
			byteSize,
			1,
			1,
			1,
			DXGI_FORMAT_UNKNOWN,
			{1, 0},
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			D3D12_RESOURCE_FLAG_NONE
		};

		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

		//��ȡ�ϴ�������
		auto* uploadBuffer = GetUploadBuffer(byteSize);
		
		//��������ϣ�����Ƶ�Ĭ�ϻ�����������
		D3D12_SUBRESOURCE_DATA subResourceData =
		{
			initData,
			static_cast<LONG_PTR>(byteSize),
			static_cast<LONG_PTR>(byteSize),
		};

		//�����ݸ��Ƶ�Ĭ�ϻ�����
		CommandManager::ResourceBarrierTransition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		UpdateSubresources<1>(CommandManager::GetCommandList(), defaultBuffer.Get(), uploadBuffer, 0, 0, 1, &subResourceData);
		CommandManager::ResourceBarrierTransition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

		return defaultBuffer;
	}

	/**
	 * \brief	��ȡ�ϴ�����
	 * \param	bufferSize		��������С
	 * \return	�ϴ�����
	 */
	ID3D12Resource* GraphicsManager::GetUploadBuffer(uint64_t bufferSize)
	{
		for (auto iter = m_UnusedUploadBuffers.begin(); iter != m_UnusedUploadBuffers.end(); ++iter)
		{
			if (iter->second >= bufferSize)
			{
				m_UsedUploadBuffers.insert(*iter);
				m_UnusedUploadBuffers.erase(iter);
				return iter->first.Get();
			}
		}

		//�����µ�UploadBuffer
		//���ô洢��Դ�Ķѣ�GPU�Դ棩������
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		//������Դ����
		D3D12_RESOURCE_DESC bufferDesc
		{
			D3D12_RESOURCE_DIMENSION_BUFFER,
			0,
			bufferSize,
			1,
			1,
			1,
			DXGI_FORMAT_UNKNOWN,
			{1, 0},
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			D3D12_RESOURCE_FLAG_NONE
		};

		ComPtr<ID3D12Resource> uploadBuffer;
		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));
		m_UsedUploadBuffers.emplace(uploadBuffer, bufferSize);
		return uploadBuffer.Get();
	}

	/**
	 * \brief	�ͷ��ϴ�����
	 * \param	uploadBuffer	�ϴ�����
	 */
	void GraphicsManager::ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer)
	{
		for (auto iter = m_UsedUploadBuffers.begin(); iter != m_UsedUploadBuffers.end(); ++iter)
		{
			if (iter->first.Get() == uploadBuffer)
			{
				m_UnusedUploadBuffers.insert(*iter);
				m_UsedUploadBuffers.erase(iter);
				break;
			}
		}
	}

	/**
	 * \brief	�ͷ������ϴ�����
	 */
	void GraphicsManager::ReleaseAllUploadBuffers()
	{
		for (auto iter = m_UsedUploadBuffers.begin(); iter != m_UsedUploadBuffers.end(); ++iter)
		{
			m_UnusedUploadBuffers.insert(*iter);
		}
		m_UsedUploadBuffers.clear();
	}

	/**
	 * \brief ����������
	 * \param hWnd ���ھ��
	 * \param width ���ڿ���
	 * \param height ���ڸ߶�
	 */
	void GraphicsManager::CreateSwapChainAndDepthStencilBuffer(HWND hWnd, uint32_t width, uint32_t height)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = swapChainBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = swapChainBufferCount;
		sd.OutputWindow = hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		ThrowIfFailed(m_DXGIFactory->CreateSwapChain(CommandManager::GetCommandQueue(), &sd, swapChain.GetAddressOf()));

		//�Ѿ������õĽ�����SwapChain�л�ȡ��Ӧ��������Ⱦ����buffer����䵽������������������
		for (auto i = 0; i < swapChainBufferCount; ++i)
		{
			ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainBuffer[i])));
		}

		//�������/ģ�建���Ӧ��������Դ�ʹ洢��Դ�Ķѣ����Ѵ�������Դ�ύ������
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,
			1
		};

		//����������
		D3D12_RESOURCE_DESC depthStencilDesc
		{
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			width,
			height,
			1,
			1,
			DXGI_FORMAT_R24G8_TYPELESS,
			1,
			0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		};

		//���ѡ��
		D3D12_CLEAR_VALUE optClear
		{
			depthStencilViewFormat,
			1.0f,
			0
		};

		//�������/ģ�建��
		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(depthStencilBuffer.GetAddressOf())));

		//��Դת��
		CommandManager::ResourceBarrierTransition(depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}

	/**
	 * \brief	���ý�����
	 * \param	width		���ڿ���
	 * \param	height		���ڸ߶�
	 */
	void GraphicsManager::ResetSwapChainAndDepthStencilBuffer(uint32_t width, uint32_t height)
	{
		for (auto& buffer : swapChainBuffer)
		{
			buffer.Reset();
		}
		depthStencilBuffer.Reset();
	}

	///**
	// * \brief ������������
	// */
	//void GraphicsManager::CreateDescriptorHeaps()
	//{
	//	//����RTV��������
	//	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	//	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	//	heapDesc.NumDescriptors = swapChainBufferCount;
	//	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//	heapDesc.NodeMask = 0;
	//	ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

	//	//�������/�ü�����������������
	//	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//	heapDesc.NumDescriptors = 1;
	//	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//	heapDesc.NodeMask = 0;
	//	ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

	//	//��ѯ��������С
	//	m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//	m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	//}

	/**
	 * \brief ������ǩ��
	 */
	void GraphicsManager::CreateRootSignature()
	{
		CD3DX12_STATIC_SAMPLER_DESC samplerDescriptors[5] =
		{
			CD3DX12_STATIC_SAMPLER_DESC
			(
				0,									// shaderRegister
				D3D12_FILTER_MIN_MAG_MIP_POINT,		// filter
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// addressU
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// addressV
				D3D12_TEXTURE_ADDRESS_MODE_WRAP		// addressW
			),

			CD3DX12_STATIC_SAMPLER_DESC
			(
				1,									// shaderRegister
				D3D12_FILTER_MIN_MAG_MIP_POINT,		// filter
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,	// addressU
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,	// addressV
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP	// addressW
			),

			CD3DX12_STATIC_SAMPLER_DESC(
				2,									// shaderRegister
				D3D12_FILTER_MIN_MAG_MIP_LINEAR,	// filter
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// addressU
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// addressV
				D3D12_TEXTURE_ADDRESS_MODE_WRAP		// addressW
			),

			CD3DX12_STATIC_SAMPLER_DESC(
				3,									// shaderRegister
				D3D12_FILTER_MIN_MAG_MIP_LINEAR,	// filter
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,	// addressU
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,	// addressV
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP	// addressW
			),

			CD3DX12_STATIC_SAMPLER_DESC(
				4,									// shaderRegister
				D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // filter
				D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressU
				D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressV
				D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressW
				0,									// mipLODBias
				16,                                 // maxAnisotropy
				D3D12_COMPARISON_FUNC_LESS_EQUAL,
				D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK
			),
		};

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		//Ϊÿһ��shader������һ����ǩ��
		const auto& shaders = ShaderManager::GetAllShaders();
		for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		{
			const auto* pShader = iter->second;
			const auto textureCount = pShader->GetTextureCount();

			CD3DX12_ROOT_PARAMETER* pSlotRootParameter;
			
			uint32_t num = 3;
			if (iter->first == StringToHash("ShadowCaster"))
			{
				pSlotRootParameter = new CD3DX12_ROOT_PARAMETER[3];
			}
			else
			{
				num += textureCount + 1;
				pSlotRootParameter = new CD3DX12_ROOT_PARAMETER[num];

				D3D12_DESCRIPTOR_RANGE srvTable{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
				pSlotRootParameter[3].InitAsDescriptorTable(1, &srvTable, D3D12_SHADER_VISIBILITY_PIXEL);

				if (textureCount == 1)
				{
					D3D12_DESCRIPTOR_RANGE srvTable1{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
					pSlotRootParameter[4].InitAsDescriptorTable(1, &srvTable1, D3D12_SHADER_VISIBILITY_PIXEL);
				}
				else if (textureCount == 2)
				{
					D3D12_DESCRIPTOR_RANGE srvTable1{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
					D3D12_DESCRIPTOR_RANGE srvTable2{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND };
					pSlotRootParameter[4].InitAsDescriptorTable(1, &srvTable1, D3D12_SHADER_VISIBILITY_PIXEL);
					pSlotRootParameter[5].InitAsDescriptorTable(1, &srvTable2, D3D12_SHADER_VISIBILITY_PIXEL);
				}
			}

			pSlotRootParameter[0].InitAsConstantBufferView(0);
			pSlotRootParameter[1].InitAsConstantBufferView(1);
			pSlotRootParameter[2].InitAsConstantBufferView(2);

			CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(num, pSlotRootParameter, 5, samplerDescriptors, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			ComPtr<ID3D12RootSignature> rootSignature;
			const auto hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			}
			ThrowIfFailed(hr);
			ThrowIfFailed(m_D3D12Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

			m_RootSignatures.emplace(pShader->GetHashValue(), rootSignature);

			delete[] pSlotRootParameter;
		}
	}

	/**
	 * \brief ����PSO
	 */
	void GraphicsManager::CreatePipelineState()
	{
		//����������PipelineState������
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = swapChainBufferFormat;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.DSVFormat = depthStencilViewFormat;
		
		
		//Ϊÿһ��shader������һ��pso
		const auto& shaders = ShaderManager::GetAllShaders();
		for (auto iter = shaders.begin(); iter != shaders.end(); ++iter)
		{
			const auto* pShader = iter->second;
			const auto* pShaderInputLayout = pShader->GetInputLayout();
			psoDesc.InputLayout = { pShaderInputLayout->data(), static_cast<uint32_t>(pShaderInputLayout->size()) };
			psoDesc.VS = { pShader->GetVsBufferPoint(), pShader->GetVsBufferSize() };
			psoDesc.PS = { pShader->GetPsBufferPoint(), pShader->GetPsBufferSize() };
			psoDesc.pRootSignature = m_RootSignatures[pShader->GetHashValue()].Get();

			if (iter->first == StringToHash("ShadowCaster"))
			{
				psoDesc.RasterizerState.DepthBias = 100000;
				psoDesc.RasterizerState.DepthBiasClamp = 0.0f;
				psoDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;
				psoDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
				psoDesc.NumRenderTargets = 0;
			}

			ComPtr<ID3D12PipelineState> pipelineState;
			ThrowIfFailed(m_D3D12Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
			m_PipelineStates.emplace(pShader->GetHashValue(), pipelineState);
		}
	}

	///**
	// * \brief ����Rtv
	// */
	//void GraphicsManager::CreateRtv()
	//{
	//	//����RTV
	//	auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	//	for (auto i = 0; i < swapChainBufferCount; ++i)
	//	{
	//		//���ȴ��Ѿ������õĽ�����SwapChain�л�ȡ��Ӧ��������Ⱦ����buffer����䵽������������������
	//		//Ϊ��Ⱦ���崴����ȾĿ����ͼ�����洢����Ⱦ��ͼ����������
	//		//��ȡ��Ⱦ��ͼ���������е���һ����Ⱦ������ͼ�ľ��
	//		ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&swapChainBuffer[i])));
	//		m_D3D12Device->CreateRenderTargetView(swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
	//		rtvHeapHandle.ptr += m_RtvDescriptorSize;
	//	}
	//}

	///**
	// * \brief ����Dsv
	// */
	//void GraphicsManager::CreateDsv(uint32_t width, uint32_t height)
	//{
	//	//����DSV
	//	//�������/ģ�建���Ӧ��������Դ�ʹ洢��Դ�Ķѣ����Ѵ�������Դ�ύ������
	//	D3D12_HEAP_PROPERTIES heapProperties
	//	{
	//		D3D12_HEAP_TYPE_DEFAULT,
	//		D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
	//		D3D12_MEMORY_POOL_UNKNOWN,
	//		1,1
	//	};

	//	//����������
	//	D3D12_RESOURCE_DESC depthStencilDesc
	//	{
	//		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
	//		0,
	//		width,
	//		height,
	//		1,
	//		1,
	//		DXGI_FORMAT_R24G8_TYPELESS,
	//		1,
	//		0,
	//		D3D12_TEXTURE_LAYOUT_UNKNOWN,
	//		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	//	};

	//	//���ѡ��
	//	D3D12_CLEAR_VALUE optClear
	//	{
	//		depthStencilViewFormat,
	//		1.0f,
	//		0
	//	};

	//	//�������/ģ�建��
	//	ThrowIfFailed(m_D3D12Device->CreateCommittedResource(
	//		&heapProperties,
	//		D3D12_HEAP_FLAG_NONE,
	//		&depthStencilDesc,
	//		D3D12_RESOURCE_STATE_COMMON,
	//		&optClear,
	//		IID_PPV_ARGS(depthStencilBuffer.GetAddressOf())));

	//	//�������/ģ����ͼ
	//	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	//	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	//	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//	dsvDesc.Format = depthStencilViewFormat;
	//	dsvDesc.Texture2D.MipSlice = 0;
	//	m_D3D12Device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	//	//��Դת��
	//	CommandManager::ResourceBarrierTransition(depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	//}
}