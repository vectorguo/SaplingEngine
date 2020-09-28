#pragma once

#include "Application/GameApplication.h"
#include "D3D12Header.h"

namespace SaplingEngine
{
	class D3D12Application : public GameApplication
	{
	public:
		D3D12Application();
		~D3D12Application() override;

		D3D12Application(const D3D12Application&) = delete;
		D3D12Application(D3D12Application&&) = delete;
		D3D12Application& operator=(const D3D12Application&) = delete;
		D3D12Application& operator=(D3D12Application&&) = delete;
		
		/*
		 * 初始化
		 */
		bool InitializeConfig() override;
		bool InitializeApplication(HINSTANCE hInstance) override;
		
		/*
		 * 更新
		 */
		void Update() override;

		/*
		 * 绘制
		 */
		void Render() override;

	private:
		/*
		 * 消息处理回调
		 */
		static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		/*
		 * 初始化窗口
		 */
		bool InitializeWindow();

		/*
		 * 初始化DirectX12
		 */
		bool InitializeDirectX12();

		/**
		 * \brief 创建渲染缓冲视图
		 */
		void CreateRenderTargetViews();
		
		/**
		 * \brief 创建深度模板缓冲视图
		 */
		void CreateDepthStencilView();

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

		/*
		 * 消息处理
		 */
		LRESULT MessageProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/*
		 * 窗口变化回调
		 */
		void OnResize();

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

		DXGI_FORMAT									m_SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT									m_DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ComPtr<ID3D12DescriptorHeap>				m_RtvDescriptorHeap;					//渲染对象描述符堆
		ComPtr<ID3D12DescriptorHeap>				m_DsvDescriptorHeap;					//深度/模板描述符堆
		ComPtr<ID3D12DescriptorHeap>				m_CbvDescriptorHeap;					//常量缓冲区描述符堆

		ComPtr<IDXGISwapChain>						m_SwapChain;							//交换链
		ComPtr<ID3D12Resource>						m_SwapChainBuffer[SwapChainBufferCount];//交换链缓冲区
		int32_t										m_BackBufferIndex = 0;					//交换链中前台缓冲索引
		ComPtr<ID3D12Resource>						m_DepthStencilBuffer;					//深度/模板缓冲区

		D3D12_VIEWPORT								m_Viewport;								//视图窗口
		D3D12_RECT									m_ScissorRect;							//裁剪矩形

		D3D_DRIVER_TYPE								m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	};
}