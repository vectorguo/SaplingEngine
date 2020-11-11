#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Dx12ConstantBufferManager final
	{
		struct ObjectUploadBufferData
		{
			ComPtr<ID3D12Resource> CommonUploadBuffer;
			uint8_t* CommonMappedData = nullptr;
			ComPtr<ID3D12Resource> SpecialUploadBuffer;
			uint8_t* SpecialMappedData = nullptr;
		};
		
	public:
		Dx12ConstantBufferManager(ID3D12Device* pDevice, ID3D12DescriptorHeap* pCbvDescriptorHeap, uint32_t cbvDescriptorSize);
		~Dx12ConstantBufferManager() = default;

		Dx12ConstantBufferManager(const Dx12ConstantBufferManager&) = delete;
		Dx12ConstantBufferManager(Dx12ConstantBufferManager&&) = delete;
		Dx12ConstantBufferManager& operator=(const Dx12ConstantBufferManager&) = delete;
		Dx12ConstantBufferManager& operator=(Dx12ConstantBufferManager&&) = delete;

		/**
		 * \brief ѹ����õ����峣������������
		 * \param index ���õ����峣������������
		 */
		void PushObjectCbIndex(uint32_t index);
		
		/**
		 * \brief �������õ����峣������������
		 * \return ���õ����峣������������
		 */
		uint32_t PopObjectCbIndex();

		/**
		 * \brief �������ݵ�����������
		 * \param index ����ĳ�������������
		 * \param pData ����ָ��
		 * \param dataSize ���ݴ�С
		 * \param isSpecial �Ƿ������ⳣ������������
		 */
		void CopyObjectCbData(uint32_t index, const void* pData, size_t dataSize, bool isSpecial);

		void CopyPassCbData(const void* pData, size_t dataSize);
		
	private:
		/**
		 * \brief �����ϴ�������
		 * \param uploadBuffer �ϴ�������ָ��
		 * \param mappedData ����ָ��
		 * \param elementCount Ԫ������
		 * \param elementSize Ԫ�ش�С
		 * \param descriptorHeapOffset �ڳ������������������е�ƫ��λ��
		 */
		void CreateUploadBuffer(ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset) const;
		
	public:
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

	private:
		/**
		 * \brief Dx12��Deviceָ��
		 */
		ID3D12Device* m_pD3DDevice;

		/**
		 * \brief ������������������ָ��
		 */
		ID3D12DescriptorHeap* m_pCbvDescriptorHeap;

		/**
		 * \brief ������������������С
		 */
		uint32_t m_CbvDescriptorSize;
		
		/**
		 * \brief Object�����ݵ��ϴ��������б�
		 */
		std::vector<ObjectUploadBufferData> m_ObjectUploadBuffers;

		/**
		 * \brief ���õ������ϴ������������б�
		 */
		std::vector<uint32_t> m_AvailableOubIndices;
		
		/**
		 * \brief Passͨ�������ϴ�������
		 */
		ComPtr<ID3D12Resource> m_PassCommonUploadBuffer;

		/**
		 * \brief Passͨ���ϴ�������ָ��
		 */
		uint8_t* m_PassCommonMappedData = nullptr;
	};
}