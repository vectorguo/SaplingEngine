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
			return GetGPUHandleFromDescriptorHeap(m_CbvDescriptorHeapPointers[shaderHashValue], DoubleConstantBufferElementCount, CbvDescriptorSize);
		}
		
		/**
		 * \brief	��ȡ�����������������ѵ�ָ������
		 * \return	�����������������ѵ�ָ������
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps(size_t shaderHashValue)
		{
			return &m_CbvDescriptorHeapPointers[shaderHashValue];
		}

		/**
		 * \brief	��ȡ�����������������ѵ�����
		 * \return	�����������������ѵ�����
		 */
		static uint32_t GetCbvDescriptorHeapCount()
		{
			return static_cast<uint32_t>(m_CbvDescriptorHeapPointers.size());
		}

		/**
		 * \brief	ѹ����õ����峣������������
		 */
		static void PushCbvIndex(size_t shaderHashValue, uint32_t index);
		
		/**
		 * \brief	�������õ����峣������������
		 */
		static uint32_t PopCbvIndex(size_t shaderHashValue, D3D12_GPU_DESCRIPTOR_HANDLE& commonCbvDescriptor, D3D12_GPU_DESCRIPTOR_HANDLE& specialCbvDescriptor);

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
		 * \brief	������������Ԫ������
		 */
		static constexpr uint32_t ConstantBufferElementCount = 100;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;
		
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief	������������������С
		 */
		static uint32_t CbvDescriptorSize;

	private:
		/**
		 * \brief	������������������ָ��
		 */
		static std::map<size_t, ComPtr<ID3D12DescriptorHeap>> m_CbvDescriptorHeaps;
		static std::map<size_t, ID3D12DescriptorHeap*> m_CbvDescriptorHeapPointers;
		
		/**
		 * \brief	Object�����ݵ��ϴ��������б�
		 */
		static std::map<size_t, ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief	���õ������ϴ������������б�
		 */
		static std::map<size_t, std::vector<uint32_t>> availableCbvIndices;
	};

	using CBufferManager = Dx12CBufferManager;
}