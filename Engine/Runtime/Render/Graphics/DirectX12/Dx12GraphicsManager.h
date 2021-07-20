#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;

	class Dx12GraphicsManager final
	{
		friend class Dx12DescriptorManager;
		friend class Dx12CommandManager;

		using RootSignatureMap = std::map<size_t, ComPtr<ID3D12RootSignature>>;
		using PipelineStateMap = std::map<size_t, ComPtr<ID3D12PipelineState>>;
		
	public:
		/**
		 * \brief	创建DX12 Device
		 */
		static void CreateDevice();

		/**
		 * \brief	开始初始化
		 * \param	hWnd		窗口句柄
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void Initialize(HWND hWnd, uint32_t width, uint32_t height);
		
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
		 * \param	shaderHashValue		Shader对应的HashValue
		 * \return	RootSignature指针
		 */
		static ID3D12RootSignature* GetRootSignature(size_t shaderHashValue)
		{
			const auto iter = m_RootSignatures.find(shaderHashValue);
			return iter == m_RootSignatures.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	获取PipelineState
		 * \param	shaderHashValue		Shader对应的HashValue
		 * \return	PipelineState指针
		 */
		static ID3D12PipelineState* GetPipelineState(size_t shaderHashValue)
		{
			const auto iter = m_PipelineStates.find(shaderHashValue);
			return iter == m_PipelineStates.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	获取后台缓存
		 * \return	后台缓存
		 */
		static ID3D12Resource* GetCurrentSwapChainBuffer()
		{
			return swapChainBuffer[currentSwapChainIndex].Get();
		}
		
		static const D3D12_VIEWPORT& GetViewport()
		{
			return m_Viewport;
		}

		static const D3D12_RECT& GetScissorRect()
		{
			return m_ScissorRect;
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
		 * \brief	创建交换链和深度模板缓冲区
		 * \param	hWnd		窗口句柄
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void CreateSwapChainAndDepthStencilBuffer(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	重置交换链
		 * \param	width		窗口宽度
		 * \param	height		窗口高度
		 */
		static void ResetSwapChainAndDepthStencilBuffer(uint32_t width, uint32_t height);

		/**
		 * \brief	创建根签名
		 */
		static void CreateRootSignature();
		
		/**
		 * \brief	创建PSO
		 */
		static void CreatePipelineState();

		/**
		 * \brief	呈现当前这一帧的绘制内容
		 */
		static void Present()
		{
			ThrowIfFailed(swapChain->Present(0, 0));
			currentSwapChainIndex = (currentSwapChainIndex + 1) % swapChainBufferCount;
		}

	private:
		static constexpr int swapChainBufferCount = 2;

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
		static ComPtr<IDXGISwapChain> swapChain;

		/**
		 * \brief	交换链缓冲区
		 */
		static ComPtr<ID3D12Resource> swapChainBuffer[swapChainBufferCount];

		/**
		 * \brief	深度/模板缓冲区
		 */
		static ComPtr<ID3D12Resource> depthStencilBuffer;

		/**
		 * \brief	交换链中前台缓冲索引
		 */
		static int32_t currentSwapChainIndex;

		static DXGI_FORMAT swapChainBufferFormat;
		static DXGI_FORMAT depthStencilViewFormat;

		/**
		 * \brief	流水线状态
		 */
		static PipelineStateMap m_PipelineStates;

		/**
		 * \brief	跟签名和描述符
		 */
		static RootSignatureMap m_RootSignatures;

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
