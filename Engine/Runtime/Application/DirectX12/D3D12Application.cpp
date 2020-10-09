#include "D3D12Application.h"

namespace SaplingEngine
{
	D3D12Application::D3D12Application() : GameApplication(), m_Viewport(), m_ScissorRect()
	{
	}

	D3D12Application::~D3D12Application() = default;

	/**
	 * \brief ��ʼ������
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool D3D12Application::InitializeConfig()
	{
		m_Width = 1280;
		m_Height = 720;
		return true;
	}

	/**
	 * \brief ����
	 */
	void D3D12Application::Update()
	{
	}

	/**
	 * \brief ����
	 */
	void D3D12Application::Render()
	{
		ThrowIfFailed(m_CommandAllocator->Reset());
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
		
		//�л���Ⱦ״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//����ViewPort��ScissorRect
		m_CommandList->RSSetViewports(1, &m_Viewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

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
		
		//����ǰ�ĺ�̨����������ȾĿ��״̬����Ϊ����״̬
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		//��ɲ�ִ����Ⱦ����
		ExecuteCommandList();

		//������̨�����ǰ̨����
		ThrowIfFailed(m_SwapChain->Present(0, 0));
		m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;

		FlushCommandQueue();
	}

	/**
	 * \brief ���ڱ仯�Ļص�����
	 */
	void D3D12Application::OnResize()
	{
		FlushCommandQueue();

		//Reset Command List
		ThrowIfFailed(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));

		//Resize SwapChain Buffer
		CreateRenderTargetViews();
		CreateDepthStencilView();

		ExecuteCommandList();
		FlushCommandQueue();

		//Reset ViewPort
		m_Viewport.TopLeftX = 0;
		m_Viewport.TopLeftY = 0;
		m_Viewport.Width = static_cast<float>(m_Width);
		m_Viewport.Height = static_cast<float>(m_Height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		//Reset ScissorRect
		m_ScissorRect = { 0, 0, static_cast<long>(m_Width), static_cast<long>(m_Height) };
	}

	/**
	 * \brief ����
	 */
	void D3D12Application::Destroy()
	{
		GameApplication::Destroy();
		
		if (m_D3D12Device != nullptr)
		{
			FlushCommandQueue();
		}
	}

	/**
	 * \brief ����Ĭ�ϻ�����
	 * \param initData ��ʼ������
	 * \param byteSize ���ݴ�С
	 * \param uploadBuffer uploadBuffer
	 * \return Ĭ�ϻ�����
	 */
	ComPtr<ID3D12Resource> D3D12Application::CreateDefaultBuffer(const void* initData, uint64_t byteSize, ComPtr<ID3D12Resource>& uploadBuffer) const
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

		//�����ϴ���
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		ThrowIfFailed(m_D3D12Device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

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
		UpdateSubresources<1>(m_CommandList.Get(), defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
		m_CommandList->ResourceBarrier(1, &resourceBarrier);

		return defaultBuffer;
	}

	/**
	 * \brief ��ʼ��DirectX12
	 * \return �Ƿ��ʼ���ɹ�
	 */
	bool D3D12Application::InitializeGraphics()
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

		//����Χ��
		ThrowIfFailed(m_D3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

		//�����������/����������������б�
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		ThrowIfFailed(m_D3D12Device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_CommandQueue)));
		ThrowIfFailed(m_D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
		ThrowIfFailed(m_D3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf())));
		m_CommandList->Close();

		//����������
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_Width;
		sd.BufferDesc.Height = m_Height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = m_SwapChainBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = SwapChainBufferCount;
		sd.OutputWindow = m_MainWindow;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		m_DXGIFactory->CreateSwapChain(m_CommandQueue.Get(), &sd, m_SwapChain.GetAddressOf());

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

		//��ʼ��
		InitializeRootSignature();
		InitializeShaders();
		InitializePso();
		
		//��������������
		CreateConstantBufferViews();
		
		return true;
	}

	/**
	 * \brief ��ʼ��PSO
	 */
	void D3D12Application::InitializePso()
	{
		
	}

	/**
	 * \brief ��ʼ����ǩ��
	 */
	void D3D12Application::InitializeRootSignature()
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
	 * \brief ��ʼ��shader�Լ����Ӧ�����벼��
	 */
	void D3D12Application::InitializeShaders()
	{
		//TODO: �����޸ĳɶ�ȡ���������Shader�Ѿ���Ӧ��InputLayout������֧�ֲ�ͬShader

		//����Shader

		//��ʼ��InputLayout
		m_InputLayout.reserve(5);
		m_InputLayout.push_back({ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
		m_InputLayout.push_back({ "TEXCOORD",	1, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });
	}
	
	/**
	 * \brief ������Ⱦ������ͼ
	 */
	void D3D12Application::CreateRenderTargetViews()
	{
		m_BackBufferIndex = 0;
		
		//�ͷ�֮ǰ�Ļ���
		for (auto& buffer : m_SwapChainBuffer)
		{
			buffer.Reset();
		}

		//�������ý�����Buffer��С
		ThrowIfFailed(m_SwapChain->ResizeBuffers(SwapChainBufferCount, m_Width, m_Height, m_SwapChainBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		for (auto i = 0; i < SwapChainBufferCount; ++i)
		{
			//���ȴ��Ѿ������õĽ�����SwapChain�л�ȡ��Ӧ��������Ⱦ����buffer����䵽������������������
			ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i])));

			//Ϊ��Ⱦ���崴����ȾĿ����ͼ�����洢����Ⱦ��ͼ����������
			m_D3D12Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

			//��ȡ��Ⱦ��ͼ���������е���һ����Ⱦ������ͼ�ľ��
			rtvHeapHandle.ptr += m_RtvDescriptorSize;
		}
	}

	/**
	 * \brief �������ģ�建����ͼ
	 */
	void D3D12Application::CreateDepthStencilView()
	{
		//�ͷ�֮ǰ�Ļ���
		m_DepthStencilBuffer.Reset();

		//�������/ģ�建������Ӧ����Դ�����ṹ
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = m_Width;
		depthStencilDesc.Height = m_Height;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		//������Դ���ѡ��
		D3D12_CLEAR_VALUE optClear;
		optClear.Format = m_DepthStencilViewFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		//�������/ģ�建���Ӧ��������Դ�ʹ洢��Դ�Ķѣ����Ѵ�������Դ�ύ������
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

		//�������/ģ����ͼ
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
	 * \brief ��������������������
	 */
	void D3D12Application::CreateConstantBufferViews()
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
	}

	/**
	 * \brief ִ������
	 */
	void D3D12Application::ExecuteCommandList() const
	{
		//��ɲ�ִ����Ⱦ����
		ThrowIfFailed(m_CommandList->Close());
		ID3D12CommandList* commands[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(commands), commands);
	}

	void D3D12Application::FlushCommandQueue()
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