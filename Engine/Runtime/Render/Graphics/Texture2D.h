#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Texture2D
	{
		friend class Dx12BufferManager;
		friend class TextureManager;

	public:
		Texture2D(std::string path);
		~Texture2D();

		/*
		 * ��ֹ�������ƶ�
		 */
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (Texture2D&&) = delete;

		/**
		 * \brief	��������������Ƿ��Ѿ��ϴ���Ĭ�ϻ�����
		 * \return	��������������Ƿ��Ѿ��ϴ���Ĭ�ϻ�����
		 */
		inline bool IsReady() const
		{
			return m_IsReady;
		}

		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSrvDescriptor() const
		{
			return m_SrvDescriptor;
		}

	private:
		/**
		 * \brief	��ͼ·��
		 */
		std::string m_Path;

		/**
		 * \brief	��������������Ƿ��Ѿ��ϴ���Ĭ�ϻ�����
		 */
		bool m_IsReady = false;

		ComPtr<ID3D12Resource> m_ResourcePtr = nullptr;
		ComPtr<ID3D12Resource> m_UploadHeap = nullptr;

		/**
		 * \brief	����������������
		 */
		uint32_t m_SrvIndex;

		/**
		 * \brief	������
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_SrvDescriptor{};
	};
}