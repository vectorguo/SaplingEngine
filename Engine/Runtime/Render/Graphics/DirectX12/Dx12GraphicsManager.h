#pragma once

#include "Dx12UploadBuffer.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;

	class Dx12GraphicsManager final
	{
		friend class Dx12CommandManager;
		friend class Dx12CBufferManager;

		using PipelineStateMap = std::map<std::string, ComPtr<ID3D12PipelineState>>;
		
	public:
		/**
		 * \brief	开始初始化
		 * \param	hWnd		窗口句柄
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void BeginInitialize(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	结束初始化
		 * \param	hWnd		窗口句柄
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void EndInitialize(HWND hWnd, uint32_t width, uint32_t height);
		
		/**
		 * \brief	重置大小
		 */
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	获取Dx12设备
		 * \return	Dx12设备
		 */
		static ID3D12Device* GetDx12Device()
		{
			return m_D3D12Device.Get();
		}

		/**
		 * \brief	获取RootSignature
		 * \return	RootSignature指针
		 */
		static ID3D12RootSignature* GetRootSignature()
		{
			return m_RootSignature.Get();
		}

		/**
		 * \brief	获取PipelineState
		 * \param	name		PipelineState名称
		 * \return	PipelineState指针
		 */
		static ID3D12PipelineState* GetPipelineState(const std::string& name)
		{
			const auto iter = m_PipelineStates.find(name);
			return iter == m_PipelineStates.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	获取后台缓存
		 * \return	后台缓存
		 */
		static ID3D12Resource* GetCurrentRt()
		{
			return m_SwapChainBuffer[m_BackBufferIndex].Get();
		}

		/**
		 * \brief	获取当前后台缓冲区视图
		 * \return	后台缓冲区视图
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRtv()
		{
			auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHeapHandle.ptr += static_cast<int64_t>(m_BackBufferIndex) * static_cast<int64_t>(m_RtvDescriptorSize);
			return rtvHeapHandle;
		}

		/**
		 * \brief	获取深度模板缓冲区试图
		 * \return	深度模板缓冲区试图
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDsv()
		{
			return m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}
		
		/**
		 * \brief	创建默认缓冲区并上传数据
		 * \param	initData		初始化数据
		 * \param	byteSize		数据大小
		 * \return	默认缓冲区
		 */
		static ComPtr<ID3D12Resource> CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize);

		/**
		 * \brief	获取上传缓存
		 * \param	bufferSize		缓冲区大小
		 * \return	上传缓存
		 */
		static ID3D12Resource* GetUploadBuffer(uint64_t bufferSize);

		/**
		 * \brief	释放上传缓存
		 * \param	uploadBuffer	上传缓存
		 */
		static void ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer);

		/**
		 * \brief	释放所有上传缓存
		 */
		static void ReleaseAllUploadBuffers();
		
	private:
		/**
		 * \brief	创建DX12 Device
		 */
		static void CreateDevice();

		/**
		 * \brief	创建交换链
		 * \param	hWnd		窗口句柄
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void CreateSwapChain(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	创建描述符堆
		 */
		static void CreateDescriptorHeaps();

		/**
		 * \brief	创建根签名
		 */
		static void CreateRootSignature();
		
		/**
		 * \brief	创建PSO
		 */
		static void CreatePipelineState();

		/**
		 * \brief	创建Rtv
		 */
		static void CreateRtv();

		/**
		 * \brief	创建Dsv
		 */
		static void CreateDsv(uint32_t width, uint32_t height);

		/**
		 * \brief	呈现当前这一帧的绘制内容
		 */
		static void Present()
		{
			ThrowIfFailed(m_SwapChain->Present(0, 0));
			m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;
		}

	private:
		static constexpr int SwapChainBufferCount = 2;

		static ComPtr<IDXGIFactory4> m_DXGIFactory;
		static ComPtr<ID3D12Device> m_D3D12Device;

		/**
		 * \brief	渲染目标视图大小
		 */
		static uint32_t m_RtvDescriptorSize;

		/**
		 * \brief	深度/模板视图大小
		 */
		static uint32_t m_DsvDescriptorSize;

		/**
		 * \brief	渲染对象描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> m_RtvDescriptorHeap;

		/**
		 * \brief	深度/模板描述符堆
		 */
		static ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;

		/**
		 * \brief	交换链
		 */
		static ComPtr<IDXGISwapChain> m_SwapChain;

		/**
		 * \brief	交换链缓冲区
		 */
		static ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];

		/**
		 * \brief	交换链中前台缓冲索引
		 */
		static int32_t m_BackBufferIndex;

		/**
		 * \brief	深度/模板缓冲区
		 */
		static ComPtr<ID3D12Resource> m_DepthStencilBuffer;

		static DXGI_FORMAT m_SwapChainBufferFormat;
		static DXGI_FORMAT m_DepthStencilViewFormat;

		/**
		 * \brief	跟签名和描述符
		 */
		static ComPtr<ID3D12RootSignature> m_RootSignature;

		/**
		 * \brief	流水线状态
		 */
		static PipelineStateMap m_PipelineStates;

		/**
		 * \brief	视图窗口
		 */
		static D3D12_VIEWPORT m_Viewport;

		/**
		 * \brief	裁剪矩形
		 */
		static D3D12_RECT m_ScissorRect;

		/**
		 * \brief	未使用的上传缓存堆
		 */
		static std::map<ComPtr<ID3D12Resource>, uint64_t> m_UnusedUploadBuffers;

		/**
		 * \brief	已经使用的上传缓存堆
		 */
		static std::map<ComPtr<ID3D12Resource>, uint64_t> m_UsedUploadBuffers;
	};

	using GraphicsManager = Dx12GraphicsManager;
}
