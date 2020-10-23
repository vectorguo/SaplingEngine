#include "Dx12GraphicsManager.h"

#include "Application/GameSetting.h"
#include "Graphics/MeshHelper.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderManager.h"

namespace SaplingEngine
{
	std::unique_ptr<Mesh> g_pMesh;

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap)
	{
		return pHeap->GetCPUDescriptorHandleForHeapStart();
	}

	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap, int32_t offset, int32_t descriptorSize)
	{
		auto cbvHeapHandle = pHeap->GetCPUDescriptorHandleForHeapStart();
		const auto cbvHeapHandleOffset = offset * descriptorSize;
		cbvHeapHandle.ptr += static_cast<uint64_t>(cbvHeapHandleOffset);
		return cbvHeapHandle;
	}

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap)
	{
		return pHeap->GetGPUDescriptorHandleForHeapStart();
	}

	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleFromDescriptorHeap(ID3D12DescriptorHeap* pHeap, int32_t offset, int32_t descriptorSize)
	{
		auto cbvHeapHandle = pHeap->GetGPUDescriptorHandleForHeapStart();
		const auto cbvHeapHandleOffset = offset * descriptorSize;
		cbvHeapHandle.ptr += static_cast<uint64_t>(cbvHeapHandleOffset);
		return cbvHeapHandle;
	}
	
	Dx12GraphicsManager::Dx12GraphicsManager() : m_Viewport(), m_ScissorRect()
	{
	}

	Dx12GraphicsManager::~Dx12GraphicsManager() = default;

	/**
	 * \brief ִ������
	 */
	// ReSharper disable once CppMemberFunctionMayBeConst
	void Dx12GraphicsManager::ExecuteCommandList()
	{
		//��ɲ�ִ����Ⱦ����
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}
	
	/**
	 * \brief �ȴ��������
	 */
	void Dx12GraphicsManager::CompleteCommand()
	{
		m_CurrentFence++;
		ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence));

		if (m_Fence->GetCompletedValue() < m_CurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
			ThrowIfFailed(m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle));
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	/**
	 * \brief ����Ĭ�ϻ�����
	 * \param initData ��ʼ������
	 * \param byteSize ���ݴ�С
	 * \return Ĭ�ϻ�����
	 */
	ComPtr<ID3D12Resource> Dx12GraphicsManager::CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize)
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
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
		UpdateSubresources<1>(m_CommandList.Get(), defaultBuffer.Get(), uploadBuffer, 0, 0, 1, &subResourceData);
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		return defaultBuffer;
	}

	/**
	 * \brief ��ȡ�ϴ�����
	 * \param bufferSize ��������С
	 * \return �ϴ�����
	 */
	ID3D12Resource* Dx12GraphicsManager::GetUploadBuffer(uint64_t bufferSize)
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
		m_UsedUploadBuffers.insert_or_assign(uploadBuffer, bufferSize);
		return uploadBuffer.Get();
	}

	/**
	 * \brief �ͷ��ϴ�����
	 * \param uploadBuffer �ϴ�����
	 */
	void Dx12GraphicsManager::ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer)
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
	 * \brief �ͷ������ϴ�����
	 */
	void Dx12GraphicsManager::ReleaseAllUploadBuffers()
	{
		for (auto iter = m_UsedUploadBuffers.begin(); iter != m_UsedUploadBuffers.end(); ++iter)
		{
			m_UnusedUploadBuffers.insert(*iter);
		}
		m_UsedUploadBuffers.clear();
	}

	/**
	 * \brief ��ʼ��DX����
	 * \param hWnd ���ھ��
	 * \param width ���ڿ���
	 * \param height ���ڸ߶�
	 */
	void Dx12GraphicsManager::BeginInitializeDxEnvironment(HWND hWnd, uint32_t width, uint32_t height)
	{
		CreateDevice();
		CreateFence();
		CreateCommandSystem();
		CreateSwapChain(hWnd, width, height);
		CreateDescriptorHeaps();
	}

	/**
	 * \brief ������ʼ��
	 */
	void Dx12GraphicsManager::EndInitializeDxEnvironment(uint32_t width, uint32_t height)
	{
		//��ʼ��
		CreateRootSignature();
		CreatePipelineState();
		CreateRtv();
		CreateDsv(width, height);
		CreateCbv();

		//ִ�г�ʼ���������
		//���������б�
		ResetCommandList();

		//�л����/ģ�建�����Դ״̬���ӳ�ʼ״̬�л������д��״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//TODO TEST Create Box Mesh
		if (g_pMesh == nullptr)
		{
			g_pMesh = MeshHelper::CreateBoxMesh();
		}

		//ִ�в��ȴ��������
		ExecuteCommandList();
		CompleteCommand();
	}

	/**
	 * \brief ����
	 */
	void Dx12GraphicsManager::Render()
	{
		//�����̨�����������ģ�建����
		const auto rtv = CurrentBackBufferView();
		const auto dsv = DepthStencilBufferView();
		m_CommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		//ָ��Ҫ��Ⱦ�Ļ�����
		m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//���ø����������ͳ������������������������󶨵���Ⱦ��ˮ����
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		m_CommandList->SetGraphicsRootDescriptorTable(1, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvOffset, m_CbvDescriptorSize));

		//��Ⱦ����
		//TODO
		m_CommandList->IASetVertexBuffers(0, 1, g_pMesh->GetVertexBufferView());
		m_CommandList->IASetIndexBuffer(g_pMesh->GetIndexBufferView());
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->SetGraphicsRootDescriptorTable(0, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get()));
		m_CommandList->DrawIndexedInstanced(g_pMesh->GetIndexCount(), 1, 0, 0, 0);
	}

	/**
	 * \brief ִ����Ⱦǰ��׼������
	 */
	void Dx12GraphicsManager::PreRender()
	{
		//��������
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get()));

		//����ViewPort��ScissorRect
		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		//����ǰ�ĺ�̨�������ӳ���״̬����Ϊ��ȾĿ��״̬
		auto* currentBackBuffer = m_SwapChainBuffer[m_BackBufferIndex].Get();
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
	}

	/**
	 * \brief ִ����Ⱦ�����������
	 */
	void Dx12GraphicsManager::PostRender()
	{
		//����ǰ�ĺ�̨����������ȾĿ��״̬����Ϊ����״̬
		auto* currentBackBuffer = m_SwapChainBuffer[m_BackBufferIndex].Get();
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(currentBackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//��ɲ�ִ����Ⱦ����
		ExecuteCommandList();

		//������̨�����ǰ̨����
		ThrowIfFailed(m_SwapChain->Present(0, 0));
		m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;
		
		//�ȴ��������
		CompleteCommand();

		//�ͷ��ϴ�������
		//ReleaseAllUploadBuffers();
	}

	/**
	 * \brief ����
	 */
	void Dx12GraphicsManager::Destroy()
	{
		CompleteCommand();
	}
	
	/**
	 * \brief ���ô�С
	 */
	void Dx12GraphicsManager::Resize(uint32_t width, uint32_t height)
	{
		//�ͷ�֮ǰ�Ļ���
		for (auto& buffer : m_SwapChainBuffer)
		{
			buffer.Reset();
		}
		m_DepthStencilBuffer.Reset();

		//Resize
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_ScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };

		//Resize SwapChain
		m_BackBufferIndex = 0;
		ThrowIfFailed(m_SwapChain->ResizeBuffers(SwapChainBufferCount, width, height, m_SwapChainBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		//���´���Rtv��Dsv
		CreateRtv();
		CreateDsv(width, height);

		//���������б�
		ResetCommandList();

		//�л����/ģ�建�����Դ״̬���ӳ�ʼ״̬�л������д��״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//ִ�в��ȴ��������
		ExecuteCommandList();
		CompleteCommand();
	}
	
	/**
	 * \brief ����DX12 Device
	 */
	void Dx12GraphicsManager::CreateDevice()
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
		const auto width = GameSetting::Instance()->ScreenWidth();
		const auto height = GameSetting::Instance()->ScreenHeight();
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_ScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };
	}

	/**
	 * \brief ����Fence
	 */
	void Dx12GraphicsManager::CreateFence()
	{
		//����Χ��
		ThrowIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
	}

	/**
	 * \brief ��������ϵͳ
	 */
	void Dx12GraphicsManager::CreateCommandSystem()
	{
		//�����������/����������������б�
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();
	}

	/**
	 * \brief ����������
	 * \param hWnd ���ھ��
	 * \param width ���ڿ���
	 * \param height ���ڸ߶�
	 */
	void Dx12GraphicsManager::CreateSwapChain(HWND hWnd, uint32_t width, uint32_t height)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = m_SwapChainBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SwapChainBufferCount;
		sd.OutputWindow = hWnd;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		m_DXGIFactory->CreateSwapChain(m_CommandQueue.Get(), &sd, m_SwapChain.GetAddressOf());
	}

	/**
	 * \brief ������������
	 */
	void Dx12GraphicsManager::CreateDescriptorHeaps()
	{
		//����RTV��������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = SwapChainBufferCount;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

		//�������/�ü�����������������
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

		//����������������/��ɫ����Դ������/���������������
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = m_CbvBufferViewCount + 1;		//Object������������Pass����������
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_CbvDescriptorHeap.GetAddressOf())));

		//��ѯ��������С
		m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	/**
	 * \brief ������ǩ��
	 */
	void Dx12GraphicsManager::CreateRootSignature()
	{
		D3D12_DESCRIPTOR_RANGE cbvTable0
		{
			D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
			1,
			0,
			0,
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		D3D12_DESCRIPTOR_RANGE cbvTable1
		{
			D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
			1,
			1,
			0,
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
		};

		CD3DX12_ROOT_PARAMETER slotRootParameter[2];
		slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable0);
		slotRootParameter[1].InitAsDescriptorTable(1, &cbvTable1);
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(2, slotRootParameter, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		const auto hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());
		if (errorBlob != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
		}
		ThrowIfFailed(hr);
		ThrowIfFailed(m_D3D12Device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
	}

	/**
	 * \brief ����PSO
	 */
	void Dx12GraphicsManager::CreatePipelineState()
	{
		const auto* pShader = ShaderManager::Instance()->GetShader("Color");
		const auto* pShaderInputLayout = pShader->GetInputLayout();

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psoDesc.InputLayout = { pShaderInputLayout->data(), static_cast<uint32_t>(pShaderInputLayout->size()) };
		psoDesc.pRootSignature = m_RootSignature.Get();
		psoDesc.VS =
		{
			static_cast<BYTE*>(pShader->GetVsShader()->GetBufferPointer()),
			pShader->GetVsShader()->GetBufferSize()
		};
		psoDesc.PS =
		{
			static_cast<BYTE*>(pShader->GetPsShader()->GetBufferPointer()),
			pShader->GetPsShader()->GetBufferSize()
		};
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = m_SwapChainBufferFormat;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;
		psoDesc.DSVFormat = m_DepthStencilViewFormat;
		ThrowIfFailed(m_D3D12Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState)));
	}

	/**
	 * \brief ����Rtv
	 */
	void Dx12GraphicsManager::CreateRtv()
	{
		//����RTV
		auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		for (auto i = 0; i < SwapChainBufferCount; ++i)
		{
			//���ȴ��Ѿ������õĽ�����SwapChain�л�ȡ��Ӧ��������Ⱦ����buffer����䵽������������������
			//Ϊ��Ⱦ���崴����ȾĿ����ͼ�����洢����Ⱦ��ͼ����������
			//��ȡ��Ⱦ��ͼ���������е���һ����Ⱦ������ͼ�ľ��
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));
			m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.ptr += m_RtvDescriptorSize;
		}
	}

	/**
	 * \brief ����Dsv
	 */
	void Dx12GraphicsManager::CreateDsv(uint32_t width, uint32_t height)
	{
		//����DSV
		//�������/ģ�建���Ӧ��������Դ�ʹ洢��Դ�Ķѣ����Ѵ�������Դ�ύ������
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
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
			m_DepthStencilViewFormat,
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
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));

		//�������/ģ����ͼ
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = m_DepthStencilViewFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		m_D3D12Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	/**
	 * \brief ����Cbv
	 */
	void Dx12GraphicsManager::CreateCbv()
	{
		//����CBV
		m_ObjConstantBuffer = std::make_unique<Dx12UploadBuffer<ObjectConstantData>>(m_D3D12Device.Get(), m_CbvBufferViewCount, true);
		m_PassConstantBuffer = std::make_unique<Dx12UploadBuffer<PassConstantData>>(m_D3D12Device.Get(), 1, true);
		m_PassCbvOffset = m_CbvBufferViewCount;

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

		const auto elementSize = m_ObjConstantBuffer->GetElementSize();
		for (uint32_t i = 0; i < m_CbvBufferViewCount; ++i)
		{
			cbvDesc.BufferLocation = m_ObjConstantBuffer->GetGpuVirtualAddress(i);
			cbvDesc.SizeInBytes = elementSize;
			m_D3D12Device->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), i, m_CbvDescriptorSize));
		}

		cbvDesc.BufferLocation = m_PassConstantBuffer->GetGpuVirtualAddress();
		cbvDesc.SizeInBytes = m_PassConstantBuffer->GetElementSize();
		m_D3D12Device->CreateConstantBufferView(&cbvDesc, GetCPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvOffset, m_CbvDescriptorSize));

		ObjectConstantData data;
		data.ModelViewProj = Matrix4x4::Translate(0, 0, 1.0f);// Matrix4x4::Scale(0.5f, 0.5f, 0.5f);
		data.ModelViewProj = data.ModelViewProj.Transpose();
		m_ObjConstantBuffer->CopyData(0, data);
	}
}