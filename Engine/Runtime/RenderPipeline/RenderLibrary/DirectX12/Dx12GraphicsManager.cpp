#include "Dx12CommandManager.h"
#include "Dx12GraphicsManager.h"

#include "Application/GameSetting.h"
#include "Graphics/MeshHelper.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderManager.h"
#include "RenderPipeline/Renderer/MeshRenderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	Dx12GraphicsManager::Dx12GraphicsManager()
		: GraphicsManager(), m_Viewport(), m_ScissorRect()
	{
	}

	Dx12GraphicsManager::~Dx12GraphicsManager() = default;

	/**
	 * \brief 开始初始化
	 * \param hWnd 窗口句柄
	 * \param width 窗口宽度
	 * \param height 窗口高度
	 */
	void Dx12GraphicsManager::BeginInitialize(HWND hWnd, uint32_t width, uint32_t height)
	{
		CreateDevice();
		CreateDescriptorHeaps();
	}

	/**
	 * \brief 结束初始化
	 * \param hWnd 窗口句柄
	 * \param width 窗口宽度
	 * \param height 窗口高度
	 */
	void Dx12GraphicsManager::EndInitialize(HWND hWnd, uint32_t width, uint32_t height)
	{
		CreateSwapChain(hWnd, width, height);
		CreateRootSignature();
		CreatePipelineState();
		CreateRtv();
		CreateDsv(width, height);
		CreateCbv();
	}

	/**
	 * \brief 绘制
	 */
	void Dx12GraphicsManager::Render()
	{
		auto* pCommandList = m_pCommandManager->m_CommandList.Get();

		//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvDescriptorHeap.Get() };
		pCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		pCommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		pCommandList->SetGraphicsRootDescriptorTable(1, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvOffset, m_CbvDescriptorSize));

		//渲染物体
		//TODO
		auto* pActiveScene = SceneManager::Instance()->GetActiveScene();
		auto pObject = pActiveScene->GetGameObject("cube");
		auto* pMesh = pObject->GetComponent<MeshRenderer>()->GetMesh();
		pCommandList->IASetVertexBuffers(0, 1, pMesh->GetVertexBufferView());
		pCommandList->IASetIndexBuffer(pMesh->GetIndexBufferView());
		pCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pCommandList->SetGraphicsRootDescriptorTable(0, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get()));
		pCommandList->DrawIndexedInstanced(pMesh->GetIndexCount(), 1, 0, 0, 0);
	}
	
	/**
	 * \brief 重置大小
	 */
	void Dx12GraphicsManager::Resize(uint32_t width, uint32_t height)
	{
		//释放之前的缓存
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

		//重置命令
		ThrowIfFailed(m_pCommandManager->m_CommandAllocator->Reset());
		ThrowIfFailed(m_pCommandManager->m_CommandList->Reset(m_pCommandManager->m_CommandAllocator.Get(), nullptr));
		
		//重新创建Rtv和Dsv
		CreateRtv();
		CreateDsv(width, height);

		//执行并等待命令结束
		m_pCommandManager->ExecuteCommandList();
		m_pCommandManager->CompleteCommand();
	}

	/**
	 * \brief 销毁
	 */
	void Dx12GraphicsManager::Destroy()
	{

	}
	
	/**
	 * \brief 创建默认缓冲区
	 * \param initData 初始化数据
	 * \param byteSize 数据大小
	 * \return 默认缓冲区
	 */
	ComPtr<ID3D12Resource> Dx12GraphicsManager::CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize)
	{
		ComPtr<ID3D12Resource> defaultBuffer;

		//设置存储资源的堆（GPU显存）的属性
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		//创建资源描述
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

		//获取上传缓冲区
		auto* uploadBuffer = GetUploadBuffer(byteSize);
		
		//描述我们希望复制到默认缓冲区的数据
		D3D12_SUBRESOURCE_DATA subResourceData =
		{
			initData,
			static_cast<LONG_PTR>(byteSize),
			static_cast<LONG_PTR>(byteSize),
		};

		//将数据复制到默认缓冲区
		m_pCommandManager->ResourceBarrierTransition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		UpdateSubresources<1>(m_pCommandManager->GetCommandList(), defaultBuffer.Get(), uploadBuffer, 0, 0, 1, &subResourceData);
		m_pCommandManager->ResourceBarrierTransition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

		return defaultBuffer;
	}

	/**
	 * \brief 获取上传缓存
	 * \param bufferSize 缓冲区大小
	 * \return 上传缓存
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

		//创建新的UploadBuffer
		//设置存储资源的堆（GPU显存）的属性
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_UPLOAD,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		//创建资源描述
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
	 * \brief 释放上传缓存
	 * \param uploadBuffer 上传缓存
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
	 * \brief 释放所有上传缓存
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
	 * \brief 创建DX12 Device
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

		//创建DXGI，用来枚举显卡，创建SwapChain
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DXGIFactory)));

		//创建显示设备(该设备对应一个显示适配器，比如显卡。下面nullptr代表使用默认的显示适配器)
		const auto result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_D3D12Device));
		if (FAILED(result))
		{//默认显示适配器对应的设备创建失败
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
	 * \brief 创建交换链
	 * \param hWnd 窗口句柄
	 * \param width 窗口宽度
	 * \param height 窗口高度
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
		m_DXGIFactory->CreateSwapChain(m_pCommandManager->GetCommandQueue(), &sd, m_SwapChain.GetAddressOf());
	}

	/**
	 * \brief 创建描述符堆
	 */
	void Dx12GraphicsManager::CreateDescriptorHeaps()
	{
		//创建RTV描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = SwapChainBufferCount;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));

		//创建深度/裁剪描述符的描述符堆
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

		//常量缓冲区描述符/着色器资源描述符/无需访问描述符堆
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NumDescriptors = m_CbvBufferViewCount + 1;		//Object常量缓冲区和Pass常量缓冲区
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(m_D3D12Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_CbvDescriptorHeap.GetAddressOf())));

		//查询描述符大小
		m_RtvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvDescriptorSize = m_D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	/**
	 * \brief 创建根签名
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
	 * \brief 创建PSO
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
	 * \brief 创建Rtv
	 */
	void Dx12GraphicsManager::CreateRtv()
	{
		//创建RTV
		auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		for (auto i = 0; i < SwapChainBufferCount; ++i)
		{
			//首先从已经创建好的交换连SwapChain中获取对应索引的渲染缓冲buffer，填充到交换链缓冲区数组中
			//为渲染缓冲创建渲染目标视图，并存储在渲染视图描述符堆中
			//获取渲染视图描述符堆中的下一个渲染对象视图的句柄
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));
			m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.ptr += m_RtvDescriptorSize;
		}
	}

	/**
	 * \brief 创建Dsv
	 */
	void Dx12GraphicsManager::CreateDsv(uint32_t width, uint32_t height)
	{
		//创建DSV
		//创建深度/模板缓冲对应的纹理资源和存储资源的堆，并把创建的资源提交到堆中
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

		//描述符参数
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

		//清除选项
		D3D12_CLEAR_VALUE optClear
		{
			m_DepthStencilViewFormat,
			1.0f,
			0
		};

		//创建深度/模板缓冲
		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));

		//创建深度/模板视图
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = m_DepthStencilViewFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		m_D3D12Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		//资源转换
		m_pCommandManager->ResourceBarrierTransition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}

	/**
	 * \brief 创建Cbv
	 */
	void Dx12GraphicsManager::CreateCbv()
	{
		//创建CBV
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
