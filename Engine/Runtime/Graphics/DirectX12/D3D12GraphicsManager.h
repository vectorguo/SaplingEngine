#pragma once

#include "D3D12UploadBuffer.h"
#include "SaplingEnginePch.h"
#include "Graphics/ConstantData.h"

namespace SaplingEngine
{
	using ObjectConstantBufferPtr = std::unique_ptr<D3D12UploadBuffer<ObjectConstantData>>;
	using PassConstantBufferPtr = std::unique_ptr<D3D12UploadBuffer<PassConstantData>>;
	
	class D3D12GraphicsManager final
	{
	public:
		D3D12GraphicsManager();
		~D3D12GraphicsManager() = default;

		SINGLETON(D3D12GraphicsManager)

		/**
		 * \brief 开始初始化DirectX12
		 */
		void BeginInitialize(HWND hWnd);

		/**
		 * \brief 结束初始化
		 */
		void EndInitialize();
		
		/**
		 * \brief 销毁
		 */
		void Destroy();
		
		/**
		 * \brief 绘制
		 */
		void Render();

		/**
		 * \brief 窗口变化回调
		 */
		void OnResize();

		/**
		 * \brief 创建默认缓冲区
		 * \param initData 初始化数据
		 * \param byteSize 数据大小
		 * \param uploadBuffer uploadBuffer
		 * \return 默认缓冲区
		 */
		ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* initData, uint64_t byteSize, ComPtr<ID3D12Resource>& uploadBuffer) const;

	private:
		/**
		 * \brief 初始化根签名
		 */
		void InitializeRootSignature();

		/**
		 * \brief 初始化PSO
		 */
		void InitializePso();

		/**
		 * \brief 创建渲染缓冲视图
		 */
		void CreateRenderTargetViews(uint32_t width, uint32_t height);

		/**
		 * \brief 创建深度模板缓冲视图
		 */
		void CreateDepthStencilView(uint32_t width, uint32_t height);

		/**
		 * \brief 创建常量缓冲区描述符
		 */
		void CreateConstantBufferViews();

		/**
		 * \brief 执行命令
		 */
		void ExecuteCommandList() const;

		/**
		 * \brief 刷新渲染队列
		 */
		void FlushCommandQueue();

		/**
		 * \brief 获取当前的后台缓冲
		 * \return 后台缓冲
		 */
		ID3D12Resource* CurrentBackBuffer() const
		{
			return m_SwapChainBuffer[m_BackBufferIndex].Get();
		}

		/**
		 * \brief 获取当前后台缓冲区视图
		 * \return 后台缓冲区视图
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
		{
			auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHeapHandle.ptr += static_cast<int64_t>(m_BackBufferIndex) * static_cast<int64_t>(m_RtvDescriptorSize);
			return rtvHeapHandle;
		}

		/**
		 * \brief 获取深度模板缓冲区试图
		 * \return 深度模板缓冲区试图
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilBufferView() const
		{
			return m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

	private:
		static constexpr  int						SwapChainBufferCount = 2;

		ComPtr<IDXGIFactory4>						m_DXGIFactory;
		ComPtr<ID3D12Device>						m_D3D12Device;

		ComPtr<ID3D12Fence>							m_Fence;
		uint64_t									m_CurrentFence = 0;

		ComPtr<ID3D12CommandQueue>					m_CommandQueue;
		ComPtr<ID3D12CommandAllocator>				m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList>			m_CommandList;

		uint32_t									m_RtvDescriptorSize = 0;				//渲染目标视图大小
		uint32_t									m_DsvDescriptorSize = 0;				//深度/模板视图大小
		uint32_t									m_CbvDescriptorSize = 0;				//常量缓冲区视图大小

		ComPtr<ID3D12DescriptorHeap>				m_RtvDescriptorHeap;					//渲染对象描述符堆
		ComPtr<ID3D12DescriptorHeap>				m_DsvDescriptorHeap;					//深度/模板描述符堆
		ComPtr<ID3D12DescriptorHeap>				m_CbvDescriptorHeap;					//常量缓冲区描述符堆
		uint32_t									m_CbvBufferViewCount = 1;				//常量缓冲区描述符数量

		ComPtr<IDXGISwapChain>						m_SwapChain;							//交换链
		ComPtr<ID3D12Resource>						m_SwapChainBuffer[SwapChainBufferCount];//交换链缓冲区
		int32_t										m_BackBufferIndex = 0;					//交换链中前台缓冲索引
		ComPtr<ID3D12Resource>						m_DepthStencilBuffer;					//深度/模板缓冲区

		DXGI_FORMAT									m_SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT									m_DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ObjectConstantBufferPtr						m_ObjConstantBuffer = nullptr;
		PassConstantBufferPtr						m_PassConstantBuffer = nullptr;
		uint32_t									m_PassCbvOffset = 0;

		ComPtr<ID3D12RootSignature>					m_RootSignature = nullptr;				//跟签名和描述符
		ComPtr<ID3D12PipelineState>					m_PipelineState = nullptr;				//流水线状态

		D3D12_VIEWPORT								m_Viewport;								//视图窗口
		D3D12_RECT									m_ScissorRect;							//裁剪矩形

		D3D_DRIVER_TYPE								m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	};
}