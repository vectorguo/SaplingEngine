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
		Dx12GraphicsManager();
		~Dx12GraphicsManager();

		Dx12GraphicsManager(const Dx12GraphicsManager&) = delete;
		Dx12GraphicsManager(Dx12GraphicsManager&&) = delete;
		Dx12GraphicsManager& operator=(const Dx12GraphicsManager&) = delete;
		Dx12GraphicsManager& operator=(Dx12GraphicsManager&&) = delete;
		
		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static Dx12GraphicsManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief ��ʼ��ʼ��
		 * \param hWnd ���ھ��
		 * \param width ���ڿ���
		 * \param height ���ڸ߶�
		 */
		void BeginInitialize(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief ������ʼ��
		 * \param hWnd ���ھ��
		 * \param width ���ڿ���
		 * \param height ���ڸ߶�
		 */
		void EndInitialize(HWND hWnd, uint32_t width, uint32_t height);
		
		/**
		 * \brief ���ô�С
		 */
		void OnWindowResize(uint32_t width, uint32_t height);
		
		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ��ȡDx12�豸
		 * \return Dx12�豸
		 */
		ID3D12Device* GetDx12Device() const
		{
			return m_D3D12Device.Get();
		}

		/**
		 * \brief ��ȡRootSignature
		 * \return RootSignatureָ��
		 */
		ID3D12RootSignature* GetRootSignature() const
		{
			return m_RootSignature.Get();
		}

		/**
		 * \brief ��ȡPipelineState
		 * \param name PipelineState����
		 * \return PipelineStateָ��
		 */
		ID3D12PipelineState* GetPipelineState(const std::string& name) const
		{
			const auto iter = m_PipelineStates.find(name);
			return iter == m_PipelineStates.end() ? nullptr : iter->second.Get();
		}

		/**
		 * \brief ��ȡ��̨����
		 * \return ��̨����
		 */
		ID3D12Resource* GetCurrentRt() const
		{
			return m_SwapChainBuffer[m_BackBufferIndex].Get();
		}

		/**
		 * \brief ��ȡ��ǰ��̨��������ͼ
		 * \return ��̨��������ͼ
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRtv() const
		{
			auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHeapHandle.ptr += static_cast<int64_t>(m_BackBufferIndex) * static_cast<int64_t>(m_RtvDescriptorSize);
			return rtvHeapHandle;
		}

		/**
		 * \brief ��ȡ���ģ�建������ͼ
		 * \return ���ģ�建������ͼ
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDsv() const
		{
			return m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}
		
		/**
		 * \brief ����Ĭ�ϻ��������ϴ�����
		 * \param initData ��ʼ������
		 * \param byteSize ���ݴ�С
		 * \return Ĭ�ϻ�����
		 */
		ComPtr<ID3D12Resource> CreateDefaultBufferAndUploadData(const void* initData, uint64_t byteSize);

		/**
		 * \brief ��ȡ�ϴ�����
		 * \param bufferSize ��������С
		 * \return �ϴ�����
		 */
		ID3D12Resource* GetUploadBuffer(uint64_t bufferSize);

		/**
		 * \brief �ͷ��ϴ�����
		 * \param uploadBuffer �ϴ�����
		 */
		void ReleaseUploadBuffer(const ID3D12Resource* uploadBuffer);

		/**
		 * \brief �ͷ������ϴ�����
		 */
		void ReleaseAllUploadBuffers();
		
	private:
		/**
		 * \brief ����DX12 Device
		 */
		void CreateDevice();

		/**
		 * \brief ����������
		 * \param hWnd ���ھ��
		 * \param width ���ڿ���
		 * \param height ���ڸ߶�
		 */
		void CreateSwapChain(HWND hWnd, uint32_t width, uint32_t height);

		/**
		 * \brief ������������
		 */
		void CreateDescriptorHeaps();

		/**
		 * \brief ������ǩ��
		 */
		void CreateRootSignature();
		
		/**
		 * \brief ����PSO
		 */
		void CreatePipelineState();

		/**
		 * \brief ����Rtv
		 */
		void CreateRtv();

		/**
		 * \brief ����Dsv
		 */
		void CreateDsv(uint32_t width, uint32_t height);

		/**
		 * \brief ���ֵ�ǰ��һ֡�Ļ�������
		 */
		void Present()
		{
			ThrowIfFailed(m_SwapChain->Present(0, 0));
			m_BackBufferIndex = (m_BackBufferIndex + 1) % SwapChainBufferCount;
		}

	private:
		/**
		 * \brief ����
		 */
		static Dx12GraphicsManager* m_Instance;
		
		static constexpr int						SwapChainBufferCount = 2;

		ComPtr<IDXGIFactory4>						m_DXGIFactory;
		ComPtr<ID3D12Device>						m_D3D12Device;

		uint32_t									m_RtvDescriptorSize = 0;				//��ȾĿ����ͼ��С
		uint32_t									m_DsvDescriptorSize = 0;				//���/ģ����ͼ��С

		ComPtr<ID3D12DescriptorHeap>				m_RtvDescriptorHeap;					//��Ⱦ������������
		ComPtr<ID3D12DescriptorHeap>				m_DsvDescriptorHeap;					//���/ģ����������

		ComPtr<IDXGISwapChain>						m_SwapChain;							//������
		ComPtr<ID3D12Resource>						m_SwapChainBuffer[SwapChainBufferCount];//������������
		int32_t										m_BackBufferIndex = 0;					//��������ǰ̨��������
		ComPtr<ID3D12Resource>						m_DepthStencilBuffer;					//���/ģ�建����

		DXGI_FORMAT									m_SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT									m_DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ComPtr<ID3D12RootSignature>					m_RootSignature = nullptr;				//��ǩ����������
		PipelineStateMap							m_PipelineStates;						//��ˮ��״̬

		D3D12_VIEWPORT								m_Viewport;								//��ͼ����
		D3D12_RECT									m_ScissorRect;							//�ü�����

		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UnusedUploadBuffers;					//δʹ�õ��ϴ������
		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UsedUploadBuffers;					//�Ѿ�ʹ�õ��ϴ������

		Dx12CommandManager*							m_pCommandManager = nullptr;			//DX12���������
	};

	using GraphicsManager = Dx12GraphicsManager;
}