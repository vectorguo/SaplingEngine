#pragma once

#include "Runtime/SaplingEnginePch.h"
#include "Runtime/Render/Graphics/DirectX12/Dx12UploadBuffer.h"

namespace SaplingEngine
{
	class Dx12DescriptorManager
	{
		friend class GraphicsManager;

	public:
		/**
		 * \brief	��ʼ��
		 */
		static void Initialize();

		/**
		 * \brief	����
		 */
		static void Destroy();

		static void CreateDescriptorHeap(ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static void CreateDescriptorHeap(ID3D12Device* pDevice, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t descriptorCount, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		static void CreateRenderTargetView(ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateRenderTargetView(ID3D12Device* pDevice, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateDepthStencilView(ID3D12Device* pDevice, DXGI_FORMAT format, ID3D12DescriptorHeap* pHeap, ID3D12Resource* pResource, uint32_t offset);
		static void CreateShaderResourceView(ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension);
		static void CreateShaderResourceView(ID3D12Device* pDevice, ID3D12Resource* pResource, uint32_t index, uint32_t shaderMapping, DXGI_FORMAT format, D3D12_SRV_DIMENSION viewDimension);

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

		/**
		 * \brief	��ȡSRV�������ѵ�ָ������
		 * \return	SRV�������ѵ�ָ������
		 */
		static ID3D12DescriptorHeap** GetSrvDescriptorHeaps()
		{
			return defaultSrvDescriptorHeap.GetAddressOf();
		}

		/**
		 * \brief	��ȡSRV������
		 * \param	index	SRV���������е�����
		 * \return	CPU������
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvCpuDescriptor(uint32_t index)
		{
			return GetCPUHandleFromDescriptorHeap(defaultSrvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	��ȡSRV������
		 * \param	index	SRV���������е�����
		 * \return	GPU������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGpuDescriptor(uint32_t index)
		{
			return GetGPUHandleFromDescriptorHeap(defaultSrvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	��ȡ���õ�SRV����
		 */
		static uint32_t GetSrvIndex()
		{
			if (availableSrvIndices.empty())
			{
				throw Exception("��ͼ����̫�࣬�޿���Srv�������ѿռ�");
			}

			auto index = *availableSrvIndices.rbegin();
			availableSrvIndices.pop_back();
			return index;
		}

		/**
		 * \brief	�黹���õ�SRV����
		 */
		static void ReturnSrvIndex(uint32_t index)
		{
			availableSrvIndices.push_back(index);
		}

	private:
		/**
		 * \brief	����Pass������������������
		 */
		static void CreatePassCbvDescriptorHeap();

		/**
		 * \brief	����Srv��������������
		 */
		static void CreateSrvDescriptorHeap();

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

		/**
		 * \brief	SRV����������
		 */
		static constexpr uint32_t SrvCount = 500;

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

		/**
		 * \brief	SRV��������
		 */
		static ComPtr<ID3D12DescriptorHeap> defaultSrvDescriptorHeap;

		/**
		 * \brief	���õ�SRV�����б�
		 */
		static std::vector<uint32_t> availableSrvIndices;
	};

	using DescriptorManager = Dx12DescriptorManager;
}