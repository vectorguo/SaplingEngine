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
		 * \brief	��ʼ��ʼ��
		 * \param	hWnd		���ھ��
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void BeginInitialize(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	������ʼ��
		 * \param	hWnd		���ھ��
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void EndInitialize(HWND hWnd, uint32_t width, uint32_t height);
		
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
		 * \return	RootSignatureָ��
		 */
		static ID3D12RootSignature* GetRootSignature()
		{
			return m_RootSignature.Get();
		}

		/**
		 * \brief	��ȡPipelineState
		 * \param	name		PipelineState����
		 * \return	PipelineStateָ��
		 */
		static ID3D12PipelineState* GetPipelineState(const std::string& name)
		{
			const auto iter = m_PipelineStates.find(name);
			return iter == m_PipelineStates.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief	��ȡ��̨����
		 * \return	��̨����
		 */
		static ID3D12Resource* GetCurrentRt()
		{
			return m_SwapChainBuffer[m_BackBufferIndex].Get();
		}

		/**
		 * \brief	��ȡ��ǰ��̨��������ͼ
		 * \return	��̨��������ͼ
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRtv()
		{
			auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHeapHandle.ptr += static_cast<int64_t>(m_BackBufferIndex) * static_cast<int64_t>(m_RtvDescriptorSize);
			return rtvHeapHandle;
		}

		/**
		 * \brief	��ȡ���ģ�建������ͼ
		 * \return	���ģ�建������ͼ
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDsv()
		{
			return m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
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
		 * \brief	����DX12 Device
		 */
		static void CreateDevice();

		/**
		 * \brief	����������
		 * \param	hWnd		���ھ��
		 * \param	width		���ڿ��
		 * \param	height		���ڸ߶�
		 */
		static void CreateSwapChain(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief	������������
		 */
		static void CreateDescriptorHeaps();

		/**
		 * \brief	������ǩ��
		 */
		static void CreateRootSignature();
		
		/**
		 * \brief	����PSO
		 */
		static void CreatePipelineState();

		/**
		 * \brief	����Rtv
		 */
		static void CreateRtv();

		/**
		 * \brief	����Dsv
		 */
		static void CreateDsv(uint32_t width, uint32_t height);

		/**
		 * \brief	���ֵ�ǰ��һ֡�Ļ�������
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
		static ComPtr<IDXGISwapChain> m_SwapChain;

		/**
		 * \brief	������������
		 */
		static ComPtr<ID3D12Resource> m_SwapChainBuffer[SwapChainBufferCount];

		/**
		 * \brief	��������ǰ̨��������
		 */
		static int32_t m_BackBufferIndex;

		/**
		 * \brief	���/ģ�建����
		 */
		static ComPtr<ID3D12Resource> m_DepthStencilBuffer;

		static DXGI_FORMAT m_SwapChainBufferFormat;
		static DXGI_FORMAT m_DepthStencilViewFormat;

		/**
		 * \brief	��ǩ����������
		 */
		static ComPtr<ID3D12RootSignature> m_RootSignature;

		/**
		 * \brief	��ˮ��״̬
		 */
		static PipelineStateMap m_PipelineStates;

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
