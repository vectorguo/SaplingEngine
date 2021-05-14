#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	
	class Dx12CBufferManager final
	{
		friend class RenderPipeline;

		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
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
		 * \brief	��ȡPass��������������
		 * \return	����Pass����������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor(size_t shaderHashValue)
		{
			return GetGPUHandleFromDescriptorHeap(cbvDescriptorHeapPointers[shaderHashValue], DoubleConstantBufferElementCount, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	��ȡPass������������ַ
		 * \return	����Pass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetPassCbAddress(size_t shaderHashValue)
		{
			const auto& uploadBuffer = objectUploadBuffers[shaderHashValue];
			return uploadBuffer.PassUploadBuffer->GetGPUVirtualAddress();
		}

		/**
		 * \brief	��ȡShadowPass��������������
		 * \return	����ShadowPass����������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetShadowPassCbvDescriptor(size_t shaderHashValue)
		{
			return GetGPUHandleFromDescriptorHeap(cbvDescriptorHeapPointers[shaderHashValue], DoubleConstantBufferElementCount + 1, cbvSrvDescriptorSize);
		}

		/**
		 * \brief	��ȡShadowPass������������ַ
		 * \return	����ShadowPass��������ַ
		 */
		static D3D12_GPU_VIRTUAL_ADDRESS GetShadowPassCbAddress(size_t shaderHashValue)
		{
			const auto& uploadBuffer = objectUploadBuffers[shaderHashValue];
			return uploadBuffer.PassUploadBuffer->GetGPUVirtualAddress() + PassCommonCbSize;
		}

		/**
		 * \brief	��ȡ�����������������ѵ�ָ������
		 * \return	�����������������ѵ�ָ������
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps(size_t shaderHashValue)
		{
			return &cbvDescriptorHeapPointers[shaderHashValue];
		}

		static ID3D12DescriptorHeap** GetSrvDescriptorHeaps()
		{
			return &srvDescriptorHeapPointer;
		}

		/**
		 * \brief	ѹ����õ����峣������������
		 */
		static void PushCbvIndex(size_t shaderHashValue, uint32_t index);
		
		/**
		 * \brief	�������õ����峣������������
		 */
		static uint32_t PopCbvIndex(size_t shaderHashValue, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor, D3D12_GPU_VIRTUAL_ADDRESS& commonCbAddress, D3D12_GPU_VIRTUAL_ADDRESS& specialCbAddress);

		static void PushSrvIndex(uint32_t index);

		static void PopSrvIndex(Texture2D* pTexture2D);

		static int32_t PopSrvIndex(D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle);

		/**
		 * \brief	������峣������������
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 * \param	index			���õ����峣������������
		 * \param	pCommonData		ͨ������
		 * \param	commonDataSize	ͨ�����ݴ�С
		 * \param	pSpecialData	��������
		 * \param	specialDataSize	�������ݴ�С
		 */
		static void FillOcbData(size_t shaderHashValue, uint32_t index, const void* pCommonData, size_t commonDataSize, const void* pSpecialData, size_t specialDataSize);

		/**
		 * \brief	���Pass��������������
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 * \param	pData			ͨ������
		 * \param	dataSize		ͨ�����ݴ�С
		 */
		static void FillPcbData(size_t shaderHashValue, const void* pData, size_t dataSize);

		/**
		 * \brief	���ShadowPass��������������
		 * \param	shaderHashValue	Shader��Ӧ��HashValue
		 * \param	pData			ͨ������
		 * \param	dataSize		ͨ�����ݴ�С
		 */
		static void FillShadowPcbData(size_t shaderHashValue, const void* pData, size_t dataSize);
		
	private:
		/**
		 * \brief	����������������������
		 */
		static void CreateCbvDescriptorHeap(size_t shaderHashValue);
		
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
		static constexpr uint32_t ConstantBufferElementCount = 100;
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
		 * \brief	������������������ָ��
		 */
		static std::map<size_t, ComPtr<ID3D12DescriptorHeap>> cbvDescriptorHeaps;
		static std::map<size_t, ID3D12DescriptorHeap*> cbvDescriptorHeapPointers;

		/**
		 * \brief	SRV��������
		 */
		static ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;

		static ID3D12DescriptorHeap* srvDescriptorHeapPointer;
		
		/**
		 * \brief	Object�����ݵ��ϴ��������б�
		 */
		static std::map<size_t, ObjectUploadBufferData> objectUploadBuffers;

		/**
		 * \brief	���õ������ϴ������������б�
		 */
		static std::map<size_t, std::vector<uint32_t>> availableCbvIndices;

		/**
		 * \brief	���õ�SRV�����б�
		 */
		static std::vector<uint32_t> availableSrvIndices;
	};

	using CBufferManager = Dx12CBufferManager;
}