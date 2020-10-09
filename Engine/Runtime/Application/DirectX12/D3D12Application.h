#pragma once

#include "Application/GameApplication.h"
#include "Graphics/ConstantData.h"
#include "D3D12Header.h"
#include "D3D12UploadBuffer.h"

namespace SaplingEngine
{
	using ObjectConstantBufferPtr = std::unique_ptr<D3D12UploadBuffer<ObjectConstantData>>;
	using PassConstantBufferPtr = std::unique_ptr<D3D12UploadBuffer<PassConstantData>>;
	
	class D3D12Application final : public GameApplication
	{
	public:
		D3D12Application();
		~D3D12Application() override;

		D3D12Application(const D3D12Application&) = delete;
		D3D12Application(D3D12Application&&) = delete;
		D3D12Application& operator=(const D3D12Application&) = delete;
		D3D12Application& operator=(D3D12Application&&) = delete;

		/**
		 * \brief ��ʼ������
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool InitializeConfig() override;
		
		/**
		 * \brief ����
		 */
		void Update() override;

		/**
		 * \brief ����
		 */
		void Render() override;

		/**
		 * \brief ���ڱ仯�ص�
		 */
		void OnResize() override;

		/**
		 * \brief ����
		 */
		void Destroy() override;

		/**
		 * \brief ����Ĭ�ϻ�����
		 * \param initData ��ʼ������
		 * \param byteSize ���ݴ�С
		 * \param uploadBuffer uploadBuffer
		 * \return Ĭ�ϻ�����
		 */
		ComPtr<ID3D12Resource> CreateDefaultBuffer(const void* initData, uint64_t byteSize, ComPtr<ID3D12Resource>& uploadBuffer) const;

	private:
		
		/**
		 * \brief ��ʼ��DirectX12
		 * \return �Ƿ��ʼ���ɹ�
		 */
		bool InitializeGraphics() override;

		/**
		 * \brief ��ʼ��PSO
		 */
		void InitializePso();
		
		/**
		 * \brief ��ʼ����ǩ��
		 */
		void InitializeRootSignature();
		
		/**
		 * \brief ��ʼ��shader�Լ����Ӧ�����벼��
		 */
		void InitializeShaders();
		
		/**
		 * \brief ������Ⱦ������ͼ
		 */
		void CreateRenderTargetViews();
		
		/**
		 * \brief �������ģ�建����ͼ
		 */
		void CreateDepthStencilView();

		/**
		 * \brief ��������������������
		 */
		void CreateConstantBufferViews();

		/**
		 * \brief ִ������
		 */
		void ExecuteCommandList() const;
		
		/**
		 * \brief ˢ����Ⱦ����
		 */
		void FlushCommandQueue();
		
		/**
		 * \brief ��ȡ��ǰ�ĺ�̨����
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

	private:
		static constexpr  int						SwapChainBufferCount = 2;
		
		ComPtr<IDXGIFactory4>						m_DXGIFactory;
		ComPtr<ID3D12Device>						m_D3D12Device;

		ComPtr<ID3D12Fence>							m_Fence;
		uint64_t									m_CurrentFence = 0;

		ComPtr<ID3D12CommandQueue>					m_CommandQueue;
		ComPtr<ID3D12CommandAllocator>				m_CommandAllocator;
		ComPtr<ID3D12GraphicsCommandList>			m_CommandList;

		uint32_t									m_RtvDescriptorSize = 0;				//��ȾĿ����ͼ��С
		uint32_t									m_DsvDescriptorSize = 0;				//���/ģ����ͼ��С
		uint32_t									m_CbvDescriptorSize = 0;				//������������ͼ��С

		ComPtr<ID3D12DescriptorHeap>				m_RtvDescriptorHeap;					//��Ⱦ������������
		ComPtr<ID3D12DescriptorHeap>				m_DsvDescriptorHeap;					//���/ģ����������
		ComPtr<ID3D12DescriptorHeap>				m_CbvDescriptorHeap;					//������������������
		uint32_t									m_CbvBufferViewCount = 100;				//��������������������
		
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

		D3D12_VIEWPORT								m_Viewport;								//��ͼ����
		D3D12_RECT									m_ScissorRect;							//�ü�����

		D3D_DRIVER_TYPE								m_DriverType = D3D_DRIVER_TYPE_HARDWARE;

		std::vector<D3D12_INPUT_ELEMENT_DESC>		m_InputLayout;							//Shader���벼��
	};
}