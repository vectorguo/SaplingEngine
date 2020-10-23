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
		 * \param width ���ڿ��
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
		 * \brief ����Cbv
		 */
		void CreateCbv();

		/**
		 * \brief ����
		 */
		void Render();

		/**
		 * \brief ����
		 */
		void Destroy();
		
		/**
		 * \brief ���ô�С
		 */
		void Resize(uint32_t width, uint32_t height);

		/**
		 * \brief ��ȡ��̨����
		 * \return ��̨����
		 */
		ID3D12Resource* CurrentBackBuffer() const
		{
			return m_SwapChainBuffer[m_BackBufferIndex].Get();
		}
		
		/**
		 * \brief ��ȡ��ǰ��̨��������ͼ
		 * \return ��̨��������ͼ
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
		{
			auto rtvHeapHandle = m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHeapHandle.ptr += static_cast<int64_t>(m_BackBufferIndex) * static_cast<int64_t>(m_RtvDescriptorSize);
			return rtvHeapHandle;
		}
		
		/**
		 * \brief ��ȡ���ģ�建������ͼ
		 * \return ���ģ�建������ͼ
		 */
		D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilBufferView() const
		{
			return m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		}

		/**
		 * \brief ���ֵ�ǰ��һ֡�Ļ�������
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

		uint32_t									m_RtvDescriptorSize = 0;				//��ȾĿ����ͼ��С
		uint32_t									m_DsvDescriptorSize = 0;				//���/ģ����ͼ��С
		uint32_t									m_CbvDescriptorSize = 0;				//������������ͼ��С

		ComPtr<ID3D12DescriptorHeap>				m_RtvDescriptorHeap;					//��Ⱦ������������
		ComPtr<ID3D12DescriptorHeap>				m_DsvDescriptorHeap;					//���/ģ����������
		ComPtr<ID3D12DescriptorHeap>				m_CbvDescriptorHeap;					//������������������
		uint32_t									m_CbvBufferViewCount = 1;				//��������������������

		ComPtr<IDXGISwapChain>						m_SwapChain;							//������
		ComPtr<ID3D12Resource>						m_SwapChainBuffer[SwapChainBufferCount];//������������
		int32_t										m_BackBufferIndex = 0;					//��������ǰ̨��������
		ComPtr<ID3D12Resource>						m_DepthStencilBuffer;					//���/ģ�建����

		DXGI_FORMAT									m_SwapChainBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT									m_DepthStencilViewFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		ObjectConstantBufferPtr						m_ObjConstantBuffer = nullptr;
		PassConstantBufferPtr						m_PassConstantBuffer = nullptr;
		uint32_t									m_PassCbvOffset = 0;

		ComPtr<ID3D12RootSignature>					m_RootSignature = nullptr;				//��ǩ����������
		ComPtr<ID3D12PipelineState>					m_PipelineState = nullptr;				//��ˮ��״̬

		D3D12_VIEWPORT								m_Viewport;								//��ͼ����
		D3D12_RECT									m_ScissorRect;							//�ü�����

		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UnusedUploadBuffers;					//δʹ�õ��ϴ������
		std::map<ComPtr<ID3D12Resource>, uint64_t>	m_UsedUploadBuffers;					//�Ѿ�ʹ�õ��ϴ������
	};
}