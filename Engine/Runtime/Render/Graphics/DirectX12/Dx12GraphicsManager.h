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
		 * \brief	����DX12 Device
		 */
		static void CreateDevice();

		/**
		 * \brief	��ʼ��ʼ��
		 * \param	hWnd		���ھ��
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void Initialize(HWND hWnd, uint32_t width, uint32_t height);
		
		/**
		 * \brief	���ô�С
		 */
		static void OnWindowResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief	����
		 */
		static void Destroy();

		/**
		 * \brief	��ȡDx12�豸
		 * \return	Dx12�豸
		 */
		static ID3D12Device* GetDx12Device()
		{
			return m_D3D12Device.Get();
		}

		/**
		 * \brief	��ȡRootSignature
		 * \param	shaderHashValue		Shader��Ӧ��HashValue
		 * \return	RootSignatureָ��
		 */
		static ID3D12RootSignature* GetRootSignature(size_t shaderHashValue)
		{
			const auto iter = m_RootSignatures.find(shaderHashValue);
			return iter == m_RootSignatures.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	��ȡPipelineState
		 * \param	shaderHashValue		Shader��Ӧ��HashValue
		 * \return	PipelineStateָ��
		 */
		static ID3D12PipelineState* GetPipelineState(size_t shaderHashValue)
		{
			const auto iter = m_PipelineStates.find(shaderHashValue);
			return iter == m_PipelineStates.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	��ȡ��̨����
		 * \return	��̨����
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
		 * \brief	����Ĭ�ϻ��������ϴ�����
		 * \param	initData		��ʼ������
		 * \param	byteSize		���ݴ�С
		 * \return	Ĭ�ϻ�����
		 */
		static ComPtr<ID3D12Resource> CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize);

		/**
		 * \brief	��ȡ�ϴ�����
		 * \param	bufferSize		��������С
		 * \return	�ϴ�����
		 */
		static ID3D12Resource* GetUploadBuffer(uint64_t bufferSize);

		/**
		 * \brief	�ͷ��ϴ�����
		 * \param	uploadBuffer	�ϴ�����
		 */
		static void ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer);

		/**
		 * \brief	�ͷ������ϴ�����
		 */
		static void ReleaseAllUploadBuffers();
		
	private:

		/**
		 * \brief	���������������ģ�建����
		 * \param	hWnd		���ھ��
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void CreateSwapChainAndDepthStencilBuffer(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	���ý�����
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void ResetSwapChainAndDepthStencilBuffer(uint32_t width, uint32_t height);

		/**
		 * \brief	������ǩ��
		 */
		static void CreateRootSignature();
		
		/**
		 * \brief	����PSO
		 */
		static void CreatePipelineState();

		/**
		 * \brief	���ֵ�ǰ��һ֡�Ļ�������
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
		 * \brief	��ȾĿ����ͼ��С
		 */
		static uint32_t m_RtvDescriptorSize;

		/**
		 * \brief	���/ģ����ͼ��С
		 */
		static uint32_t m_DsvDescriptorSize;

		/**
		 * \brief	��Ⱦ������������
		 */
		static ComPtr<ID3D12DescriptorHeap> m_RtvDescriptorHeap;

		/**
		 * \brief	���/ģ����������
		 */
		static ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;

		/**
		 * \brief	������
		 */
		static ComPtr<IDXGISwapChain> swapChain;

		/**
		 * \brief	������������
		 */
		static ComPtr<ID3D12Resource> swapChainBuffer[swapChainBufferCount];

		/**
		 * \brief	���/ģ�建����
		 */
		static ComPtr<ID3D12Resource> depthStencilBuffer;

		/**
		 * \brief	��������ǰ̨��������
		 */
		static int32_t currentSwapChainIndex;

		static DXGI_FORMAT swapChainBufferFormat;
		static DXGI_FORMAT depthStencilViewFormat;

		/**
		 * \brief	��ˮ��״̬
		 */
		static PipelineStateMap m_PipelineStates;

		/**
		 * \brief	��ǩ����������
		 */
		static RootSignatureMap m_RootSignatures;

		/**
		 * \brief	��ͼ����
		 */
		static D3D12_VIEWPORT m_Viewport;

		/**
		 * \brief	�ü�����
		 */
		static D3D12_RECT m_ScissorRect;

		/**
		 * \brief	δʹ�õ��ϴ������
		 */
		static std::map<ComPtr<ID3D12Resource>, uint64_t> m_UnusedUploadBuffers;

		/**
		 * \brief	�Ѿ�ʹ�õ��ϴ������
		 */
		static std::map<ComPtr<ID3D12Resource>, uint64_t> m_UsedUploadBuffers;
	};

	using GraphicsManager = Dx12GraphicsManager;
}
