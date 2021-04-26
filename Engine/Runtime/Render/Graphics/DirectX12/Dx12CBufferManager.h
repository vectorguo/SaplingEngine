#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	
	class Dx12CBufferManager final
	{
		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
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
		 * \brief	����Object��������������
		 */
		static void UpdateOcbData();

		/**
		 * \brief	����Pass��������������
		 */
		static void UpdatePcbData(Camera* pCamera);

		/**
		 * \brief	��ȡObject��������������
		 * \param	ocbIndex	Object�ĳ�������������������
		 * \return	Object��������������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetObjectCbvDescriptor(uint32_t ocbIndex);

		/**
		 * \brief	��ȡPass��������������
		 * \return	����Pass����������
		 */
		static D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor();
		
		/**
		 * \brief	��ȡ�����������������ѵ�ָ������
		 * \return	�����������������ѵ�ָ������
		 */
		static ID3D12DescriptorHeap** GetCbvDescriptorHeaps()
		{
			return &m_CbvDescriptorHeapPointers[0];
		}

		/**
		 * \brief	��ȡ�����������������ѵ�����
		 * \return	�����������������ѵ�����
		 */
		static uint32_t GetCbvDescriptorHeapCount()
		{
			return 1;
		}
		
		/**
		 * \brief	ѹ����õ����峣������������
		 * \param	index		���õ����峣������������
		 */
		static void PushObjectCbIndex(uint32_t index);
		
		/**
		 * \brief	�������õ����峣������������
		 * \return	���õ����峣������������
		 */
		static uint32_t PopObjectCbIndex();
		
	private:
		/**
		 * \brief	����������������������
		 * \param	pDevice		Dx12�豸ָ��
		 */
		static void CreateCbvDescriptorHeap(ID3D12Device* pDevice);
		
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
		static constexpr uint32_t ConstantBufferDescriptorHeapSize = DoubleConstantBufferElementCount + 1;
		
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
		static ComPtr<ID3D12DescriptorHeap> m_CbvDescriptorHeap;
		static ID3D12DescriptorHeap** m_CbvDescriptorHeapPointers;
		
		/**
		 * \brief	Object�����ݵ��ϴ��������б�
		 */
		static std::vector<ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief	���õ������ϴ������������б�
		 */
		static std::vector<uint32_t> m_AvailableOubIndices;
		
		/**
		 * \brief	Passͨ�������ϴ�������
		 */
		static ComPtr<ID3D12Resource> m_PassCommonUploadBuffer;

		/**
		 * \brief	Passͨ���ϴ�������ָ��
		 */
		static uint8_t* m_PassCommonMappedData;

		/**
		 * \brief	Pass����������������ƫ��
		 */
		static uint32_t m_PassCbvDescriptorOffset;
	};

	using CBufferManager = Dx12CBufferManager;
}