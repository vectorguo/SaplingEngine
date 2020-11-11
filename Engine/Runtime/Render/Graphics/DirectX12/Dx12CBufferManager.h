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
		Dx12CBufferManager()
		{
			m_Instance = this;
		}
		~Dx12CBufferManager() = default;

		Dx12CBufferManager(const Dx12CBufferManager&) = delete;
		Dx12CBufferManager(Dx12CBufferManager&&) = delete;
		Dx12CBufferManager& operator=(const Dx12CBufferManager&) = delete;
		Dx12CBufferManager& operator=(Dx12CBufferManager&&) = delete;
		
		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static Dx12CBufferManager* Instance()
		{
			return m_Instance;
		}
		
		/**
		 * \brief ��ʼ��
		 */
		void Initialize();

		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ����Object��������������
		 */
		void UpdateOcbData(Scene* pActiveScene);

		/**
		 * \brief ����Pass��������������
		 */
		void UpdatePcbData(Camera* pCamera);

		/**
		 * \brief ��ȡObject��������������
		 * \param ocbIndex Object�ĳ�������������������
		 * \return Object��������������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetObjectCbvDescriptor(uint32_t ocbIndex) const;

		/**
		 * \brief ��ȡPass��������������
		 * \return ����Pass����������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE GetPassCbvDescriptor() const;
		
		/**
		 * \brief ��ȡ�����������������ѵ�ָ������
		 * \return �����������������ѵ�ָ������
		 */
		ID3D12DescriptorHeap** GetCbvDescriptorHeaps() const
		{
			return &m_CbvDescriptorHeapPointers[0];
		}

		/**
		 * \brief ��ȡ�����������������ѵ�����
		 * \return �����������������ѵ�����
		 */
		uint32_t GetCbvDescriptorHeapCount() const
		{
			return 1;
		}
		
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
		
	private:
		/**
		 * \brief ����������������������
		 * \param pDevice Dx12�豸ָ��
		 */
		void CreateCbvDescriptorHeap(ID3D12Device* pDevice);
		
		/**
		 * \brief �����ϴ�������
		 * \param descriptorHeap ��������
		 * \param uploadBuffer �ϴ�������ָ��
		 * \param mappedData ����ָ��
		 * \param elementCount Ԫ������
		 * \param elementSize Ԫ�ش�С
		 * \param descriptorHeapOffset �ڳ������������������е�ƫ��λ��
		 */
		void CreateUploadBuffer(ID3D12DescriptorHeap* descriptorHeap, ComPtr<ID3D12Resource>& uploadBuffer, uint8_t*& mappedData, uint32_t elementCount, uint32_t elementSize, uint32_t descriptorHeapOffset) const;
		
	public:
		/**
		 * \brief ������������Ԫ������
		 */
		static constexpr uint32_t ConstantBufferElementCount = 100;
		static constexpr uint32_t DoubleConstantBufferElementCount = ConstantBufferElementCount * 2;
		static constexpr uint32_t ConstantBufferDescriptorHeapSize = DoubleConstantBufferElementCount * 10 + 1;
		
		static constexpr uint32_t ObjectCommonCbSize = 256;
		static constexpr uint32_t ObjectSpecialCbSize = 512;
		static constexpr uint32_t PassCommonCbSize = 512;

		/**
		 * \brief ������������������С
		 */
		static uint32_t CbvDescriptorSize;

	private:
		/**
		 * \brief ����
		 */
		static Dx12CBufferManager* m_Instance;

		/**
		 * \brief ������������������ָ��
		 */
		ComPtr<ID3D12DescriptorHeap> m_CbvDescriptorHeap;
		ID3D12DescriptorHeap** m_CbvDescriptorHeapPointers = nullptr;
		
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

		/**
		 * \brief Pass����������������ƫ��
		 */
		uint32_t m_PassCbvDescriptorOffset = 0;
	};

	using CBufferManager = Dx12CBufferManager;
}