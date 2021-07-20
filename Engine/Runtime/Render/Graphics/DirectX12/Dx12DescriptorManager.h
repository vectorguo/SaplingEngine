#pragma once

#include "Render/Graphics/DirectX12/Dx12UploadBuffer.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12DescriptorManager
	{
		friend class Dx12GraphicsManager;

	public:
		/**
		 * \brief	��ʼ��
		 */
		static void Initialize();

		static void CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static void CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		static void CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);

		static D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView();
		static D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView();

		/**
		 * \brief	��ȡ��������������������
		 */
		static void GetObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer*& pUploadBuffer);

		/**
		 * \brief	�黹��������������������
		 */
		static void ReturnObjectCbvDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, Dx12UploadBuffer* pUploadBuffer);

		/**
		 * \brief	��ȡPass�ϴ�������ָ��(����Ҫ�黹)
		 */
		static Dx12UploadBuffer* GetPassCbUploadBuffer()
		{
			return passCbUploadBuffer;
		}

		/**
		 * \brief	��ȡPass������������ַ
		 * \return	����Pass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetPassCbAddress()
		{
			return passCbUploadBuffer->GetGpuVirtualAddress();
		}

		/**
		 * \brief	��ȡShadowPass������������ַ
		 * \return	����ShadowPass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetShadowPassCbAddress()
		{
			return passCbUploadBuffer->GetGpuVirtualAddress(PassCommonCbSize);
		}

	private:
		/**
		 * \brief	����Pass������������������
		 */
		static void CreatePassCbvDescriptorHeap();

	public:
		/**
		 * \brief	���峣������������������������������
		 */
		static constexpr uint32_t ObjectCbvDescriptorCount = 100;

		/**
		 * \brief	Pass��������������������������������
		 */
		static constexpr uint32_t PassCbvDescriptorCount = 2;

		/**
		 * \brief	����ͨ�ó�����������С
		 */
		static constexpr uint32_t ObjectCommonCbSize = 256;

		/**
		 * \brief	����ͨ�ó����������ܴ�С
		 */
		static constexpr uint32_t TotalObjectCommonCbSize = ObjectCbvDescriptorCount * ObjectCommonCbSize;

		/**
		 * \brief	�������ⳣ����������С
		 */
		static constexpr uint32_t ObjectSpecialCbSize = 512;

		/**
		 * \brief	�������ⳣ���������ܴ�С
		 */
		static constexpr uint32_t TotalObjectSpecialCbSize = ObjectCbvDescriptorCount * ObjectSpecialCbSize;

		/**
		 * \brief	Pass������������С
		 */
		static constexpr uint32_t PassCommonCbSize = 512;

	private:
		/**
		 * \brief	��ȾĿ����ͼ��С
		 */
		static uint32_t rtvDescriptorSize;

		/**
		 * \brief	���/ģ����ͼ��С
		 */
		static uint32_t dsvDescriptorSize;

		/**
		 * \brief	������������������С
		 */
		static uint32_t cbvSrvDescriptorSize;

		/**
		 * \brief	Ĭ����Ⱦ������������
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultRtvDescriptorHeap;

		/**
		 * \brief	Ĭ�����/ģ����������
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultDsvDescriptorHeap;

		/**
		 * \brief	���峣�����������������б�
		 */
		static std::vector<ComPtr<ID3D12DescriptorHeap>> objectCbvDescriptorHeaps;

		/**
		 * \brief	���峣����������Ӧ���ϴ��������б�
		 */
		static std::vector<Dx12UploadBuffer*> objectCbUploadBuffers;

		/**
		 * \brief	Pass�������������������б�
		 */
		static ComPtr<ID3D12DescriptorHeap> passCbvDescriptorHeap;

		/**
		 * \brief	Pass������������Ӧ���ϴ�������
		 */
		static Dx12UploadBuffer* passCbUploadBuffer;
	};

	using DescriptorManager = Dx12DescriptorManager;
}