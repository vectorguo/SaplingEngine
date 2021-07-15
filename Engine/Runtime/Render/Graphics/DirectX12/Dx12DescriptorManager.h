#pragma once

#include "Render/Graphics/DirectX12/Dx12UploadBuffer.h"
#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12DescriptorManager
	{
		friend class Dx12GraphicsManager;

	public:
		struct Dx12DescriptorHeap
		{
			Dx12DescriptorHeap(uint32_t uploadBufferSize) : UploadBuffer(uploadBufferSize)
			{
				CreateDescriptorHeap(DescriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, ObjectCbvDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
			}

			ComPtr<ID3D12DescriptorHeap> DescriptorHeap;
			Dx12UploadBuffer UploadBuffer;
		};

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
		static Dx12DescriptorHeap* GetObjectCbvDescriptorHeap();

	public:
		/**
		 * \brief	���峣������������������������������
		 */
		static constexpr uint32_t ObjectCbvDescriptorCount = 100;

		/**
		 * \brief	����ͨ�ó�����������С
		 */
		static constexpr uint32_t ObjectCommonCbSize = 256;

		/**
		 * \brief	�������ⳣ����������С
		 */
		static constexpr uint32_t ObjectSpecialCbSize = 512;

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
		static std::vector<Dx12DescriptorHeap*> objectCbvDescriptorHeaps;
	};

	using DescriptorManager = Dx12DescriptorManager;
}