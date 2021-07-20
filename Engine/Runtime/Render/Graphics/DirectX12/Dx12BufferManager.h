#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12BufferManager final
	{
		friend class RenderPipeline;

		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
		};

		struct PassUploadBufferData
		{
			ComPtr<ID3D12Resource> PassUploadBuffer;
			uint8_t* PassMappedData = nullptr;
		};

	public:
		/**
			 * \brief	��ʼ��
			 */
		static void Initialize();

		/**
		 * \brief	����
		 */
		static void Destroy();

		/**
		 * \brief	��ȡPass������������ַ
		 * \return	����Pass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetPassCbAddress()
		{
			return passUploadBuffer.PassUploadBuffer->GetGPUVirtualAddress();
		}

		/**
		 * \brief	��ȡShadowPass������������ַ
		 * \return	����ShadowPass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetShadowPassCbAddress()
		{
			return passUploadBuffer.PassUploadBuffer->GetGPUVirtualAddress() + PassCommonCbSize;
		}

		/**
		 * \brief	��ȡSRV�������ѵ�ָ������
		 * \return	SRV�������ѵ�ָ������
		 */
		static ID3D12DescriptorHeap** GetSrvDescriptorHeaps()
		{
			return srvDescriptorHeap.GetAddressOf();
		}

		/**
		 * \brief	��ȡSRV������
		 * \param	index	SRV���������е�����
		 * \return	CPU������
		 */
		static D3D12_CPU_DESCRIPTOR_HANDLE GetSrvCpuDescriptor(uint32_t index)
		{
			return GetCPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	��ȡSRV������
		 * \param	index	SRV���������е�����
		 * \return	GPU������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvGpuDescriptor(uint32_t index)
		{
			return GetGPUHandleFromDescriptorHeap(srvDescriptorHeap.Get(), index, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	ѹ����õ�Object��������������
		 */
		static void PushCbvIndex(uint32_t index);

		/**
		 * \brief	�������õ�Object��������������
		 */
		static uint32_t PopCbvIndex(D3D12_GPU_VIRTUAL_ADDRESS& commonCbAddress, D3D12_GPU_VIRTUAL_ADDRESS& specialCbAddress);

		/**
		 * \brief	ѹ����õ�SRV����
		 */
		static void PushSrvIndex(uint32_t index);

		/**
		 * \brief	�������õ�SRV����
		 */
		static uint32_t PopSrvIndex();

		/**
		 * \brief	������峣������������
		 * \param	index			���õ����峣������������
		 * \param	pCommonData		ͨ������
		 * \param	commonDataSize	ͨ�����ݴ�С
		 * \param	pSpecialData	��������
		 * \param	specialDataSize	�������ݴ�С
		 */
		static void FillOcbData(uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize);

		/**
		 * \brief	���Pass��������������
		 * \param	pData			ͨ������
		 * \param	dataSize		ͨ�����ݴ�С
		 */
		static void FillPcbData(const void* pData, size_t dataSize);

		/**
		 * \brief	���ShadowPass��������������
		 * \param	pData			ͨ������
		 * \param	dataSize		ͨ�����ݴ�С
		 */
		static void FillShadowPcbData(const void* pData, size_t dataSize);

	private:
		/**
		 * \brief	����Object������������������
		 */
		static void CreateObjectCbvDescriptorHeap();

		/**
		 * \brief	����Pass������������������
		 */
		static void CreatePassCbvDescriptorHeap();

		/**
		 * \brief	����SRV��������
		 */
		static void CreateSrvDescriptorHeap();

		/**
		 * \brief	�����ϴ�������
		 * \param	descriptorHeap	��������
		 * \param	uploadBuffer	�ϴ�������ָ��
		 * \param	mappedData		����ָ��
		 * \param	elementCount	Ԫ������
		 * \param	elementSize		Ԫ�ش�С
		 * \param	descriptorHeapOffset �ڳ������������������е�ƫ��λ��
		 */
		static void CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset);

	public:
		/**
		 * \brief	������������������С
		 */
		static uint32_t cbvSrvDescriptorSize;

		/**
		 * \brief	������������Ԫ������
		 */
		static constexpr uint32_t ConstantBufferElementCount = 500;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;

		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief	SRV
		 */
		static constexpr uint32_t SrvCount = 100;

	private:
		/**
		 * \brief	Object������������������
		 */
		static std::vector<ComPtr<ID3D12DescriptorHeap>> objectCbvDescriptorHeaps;

		/**
		 * \brief	Object�����ݵ��ϴ��������б�
		 */
		static std::vector<ObjectUploadBufferData> objectUploadBuffers;

		/**
		 * \brief	���õ������ϴ������������б�
		 */
		static std::vector<uint32_t> availableObjectCbvIndices;

		/**
		 * \brief	Pass������������������ָ��
		 */
		static ComPtr<ID3D12DescriptorHeap> passCbvDescriptorHeap;

		/**
		 * \brief	Pass�����ݵ��ϴ�������
		 */
		static PassUploadBufferData passUploadBuffer;

		/**
		 * \brief	SRV��������
		 */
		static ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;

		/**
		 * \brief	���õ�SRV�����б�
		 */
		static std::vector<uint32_t> availableSrvIndices;
	};

	using BufferManager = Dx12BufferManager;
}