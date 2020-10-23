#pragma once

#include "Dx12GraphicsPch.h"
#include "Dx12UploadBuffer.h"
#include "Graphics/ConstantData.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	using ObjectConstantBufferPtr = std::unique_ptr<Dx12UploadBuffer<ObjectConstantData>>;
	using PassConstantBufferPtr = std::unique_ptr<Dx12UploadBuffer<PassConstantData>>;
	
	class Dx12GraphicsManager final
	{
		friend class RenderPipeline;
		
	public:
		Dx12GraphicsManager();
		~Dx12GraphicsManager();

		Dx12GraphicsManager(const Dx12GraphicsManager&) = delete;
		Dx12GraphicsManager(Dx12GraphicsManager&&) = delete;
		Dx12GraphicsManager& operator=(const Dx12GraphicsManager&) = delete;
		Dx12GraphicsManager& operator=(Dx12GraphicsManager&&) = delete;

		/**
		 * \brief 创建默认缓冲区并上传数据
		 * \param initData 初始化数据
		 * \param byteSize 数据大小
		 * \return 默认缓冲区
		 */
		ComPtr<ID3D12Resource> CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize);

		/**
		 * \brief 获取上传缓存
		 * \param bufferSize 缓冲区大小
		 * \return 上传缓存
		 */
		ID3D12Resource* GetUploadBuffer(uint64_t bufferSize);

		/**
		 * \brief 释放上传缓存
		 * \param uploadBuffer 上传缓存
		 */
		void ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer);

		/**
		 * \brief 释放所有上传缓存
		 */
		void ReleaseAllUploadBuffers();
		
	private:
		/**
		 * \brief 创建DX12 Device
		 */
		void CreateDevice();

		/**
		 * \brief 创建交换链
		 * \param hWnd 窗口句柄
		 * \param width 窗口宽度
		 * \param height 窗口高度
		 */
		void CreateSwapChain(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief 创建描述符堆
		 */
		void CreateDescriptorHeaps();

		/**
		 * \brief 创建根签名
		 */
		void CreateRootSignature();
		
		/**
		 * \brief 创建PSO
		 */
		void CreatePipelineState();

		/**
		 * \brief 创建Rtv
		 */
		void CreateRtv();

		/**
		 * \brief 创建Dsv
		 */
		void CreateDsv(uint32_t width, uint32_t height);

		/**
		 * \brief 创建Cbv
		 */
		void CreateCbv();

		/**
		 * \brief 绘制
		 */
		void Render();

		/**
		 * \brief 销毁
		 */
		void Destroy();
		
		/**
		 * \brief 重置大小
		 */
		void Resize(uint32_t width, uint32_t height);

		/**
		 * \brief 获取后台缓存
		 * \return 后台缓存
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

		/**
		 * \brief 呈现当前这一帧的绘制内容
		 */
		void Present()
		{
			ThrowIfFailed(m_SwapChain->Present(0, 0));
			m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;
		}

	private:
		static constexpr  int						SwapChainBufferCount = 2;

		ComPtr<IDXGIFactory4>						m_DXGIFactory;
		ComPtr<ID3D12Device>						m_D3D12Device;

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

		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UnusedUploadBuffers;					//未使用的上传缓存堆
		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UsedUploadBuffers;					//已经使用的上传缓存堆
	};
}