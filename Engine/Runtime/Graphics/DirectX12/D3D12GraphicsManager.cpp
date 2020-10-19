#include "D3D12GraphicsManager.h"

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
	
	D3D12GraphicsManager::D3D12GraphicsManager() : m_Viewport(), m_ScissorRect()
	{
	}

	/**
	 * \brief 绘制
	 */
	void D3D12GraphicsManager::Render()
	{
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get()));

		//切换渲染状态
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//设置ViewPort和ScissorRect
		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		//清除后台缓冲区和深度模板缓冲区
		const auto rtv = CurrentBackBufferView();
		const auto dsv = DepthStencilBufferView();
		m_CommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		m_CommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		//指定要渲染的缓冲区
		m_CommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
		ID3D12DescriptorHeap* descriptorHeaps[] = { m_CbvDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		m_CommandList->SetGraphicsRootDescriptorTable(1, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get(), m_PassCbvOffset, m_CbvDescriptorSize));

		//渲染物体
		//TODO
		m_CommandList->IASetVertexBuffers(0, 1, g_pMesh->GetVertexBufferView());
		m_CommandList->IASetIndexBuffer(g_pMesh->GetIndexBufferView());
		m_CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->SetGraphicsRootDescriptorTable(0, GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeap.Get()));
		m_CommandList->DrawIndexedInstanced(g_pMesh->GetIndexCount(), 1, 0, 0, 0);

		//将当前的后台缓冲区从渲染目标状态设置为呈现状态
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//完成并执行渲染命令
		ExecuteCommandList();

		//交换后台缓冲和前台缓冲
		ThrowIfFailed(m_SwapChain->Present(0, 0));
		m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;

		FlushCommandQueue();
	}

	/**
	 * \brief 窗口变化的回调函数
	 */
	void D3D12GraphicsManager::OnResize()
	{
		FlushCommandQueue();

		//Reset Command List
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		//Resize SwapChain Buffer
		const auto width = GameSetting::Instance()->ScreenWidth();
		const auto height = GameSetting::Instance()->ScreenHeight();
		CreateRenderTargetViews(width, height);
		CreateDepthStencilView(width, height);

		//TODO TEST Create Box Mesh
		if (g_pMesh == nullptr)
		{
			g_pMesh = MeshHelper::CreateBoxMesh();
		}

		ExecuteCommandList();
		FlushCommandQueue();

		//Reset ViewPort
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(width);
		m_Viewport.Height = static_cast<float>(height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		//Reset ScissorRect
		m_ScissorRect = { 0, 0, static_cast<long>(width), static_cast<long>(height) };
	}

	/**
	 * \brief 创建默认缓冲区
	 * \param initData 初始化数据
	 * \param byteSize 数据大小
	 * \param uploadBuffer uploadBuffer
	 * \return 默认缓冲区
	 */
	ComPtr<ID3D12Resource> D3D12GraphicsManager::CreateDefaultBuffer(const void* initData, uint64_t byteSize, ComPtr<ID3D12Resource>& uploadBuffer) const
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

		//创建上传堆
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

		//描述我们希望复制到默认缓冲区的数据
		D3D12_SUBRESOURCE_DATA subResourceData =
		{
			initData,
			static_cast<LONG_PTR>(byteSize),
			static_cast<LONG_PTR>(byteSize),
		};

		//将数据复制到默认缓冲区
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
		UpdateSubresources<1>(m_CommandList.Get(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		return defaultBuffer;
	}

	/**
	 * \brief 初始化DirectX12
	 * \return 是否初始化成功
	 */
	void D3D12GraphicsManager::BeginInitialize(HWND hWnd)
	{
		ShaderManager::Instance()->Initialize();
		
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

		//创建围栏
		ThrowIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//创建命令队列/命令分配器和命令列表
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();

		//创建交换链
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = GameSetting::Instance()->ScreenWidth();
		sd.BufferDesc.Height = GameSetting::Instance()->ScreenHeight();
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

	void D3D12GraphicsManager::EndInitialize()
	{
		//初始化
		InitializeRootSignature();
		InitializePso();

		//创建常量缓冲区
		CreateConstantBufferViews();

		//Resize
		OnResize();
	}

	/**
	 * \brief 销毁
	 */
	void D3D12GraphicsManager::Destroy()
	{
		
	}

	/**
	 * \brief 初始化根签名
	 */
	void D3D12GraphicsManager::InitializeRootSignature()
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
	 * \brief 初始化PSO
	 */
	void D3D12GraphicsManager::InitializePso()
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
	 * \brief 创建渲染缓冲视图
	 */
	void D3D12GraphicsManager::CreateRenderTargetViews(uint32_t width, uint32_t height)
	{
		m_BackBufferIndex = 0;

		//释放之前的缓存
		for (auto& buffer : m_SwapChainBuffer)
		{
			buffer.Reset();
		}

		//重新设置交换链Buffer大小
		ThrowIfFailed(m_SwapChain->ResizeBuffers(SwapChainBufferCount, width, height, m_SwapChainBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		for (auto i = 0; i < SwapChainBufferCount; ++i)
		{
			//首先从已经创建好的交换连SwapChain中获取对应索引的渲染缓冲buffer，填充到交换链缓冲区数组中
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));

			//为渲染缓冲创建渲染目标视图，并存储在渲染视图描述符堆中
			m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

			//获取渲染视图描述符堆中的下一个渲染对象视图的句柄
			rtvHeapHandle.ptr += m_RtvDescriptorSize;
		}
	}

	/**
	 * \brief 创建深度模板缓冲视图
	 */
	void D3D12GraphicsManager::CreateDepthStencilView(uint32_t width, uint32_t height)
	{
		//释放之前的缓存
		m_DepthStencilBuffer.Reset();

		//创建深度/模板缓冲区对应的资源描述结构
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		//设置资源清除选项
		D3D12_CLEAR_VALUE optClear;
		optClear.Format = m_DepthStencilViewFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		//创建深度/模板缓冲对应的纹理资源和存储资源的堆，并把创建的资源提交到堆中
		D3D12_HEAP_PROPERTIES heapProperties
		{
			D3D12_HEAP_TYPE_DEFAULT,
			D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			D3D12_MEMORY_POOL_UNKNOWN,
			1,1
		};

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

		// Transition the resource from its initial state to be used as a depth buffer.
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);
	}

	/**
	 * \brief 创建常量缓冲区描述符
	 */
	void D3D12GraphicsManager::CreateConstantBufferViews()
	{
		m_ObjConstantBuffer = std::make_unique<D3D12UploadBuffer<ObjectConstantData>>(m_D3D12Device.Get(), m_CbvBufferViewCount, true);
		m_PassConstantBuffer = std::make_unique<D3D12UploadBuffer<PassConstantData>>(m_D3D12Device.Get(), 1, true);
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

	/**
	 * \brief 执行命令
	 */
	void D3D12GraphicsManager::ExecuteCommandList() const
	{
		//完成并执行渲染命令
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}

	void D3D12GraphicsManager::FlushCommandQueue()
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
}
