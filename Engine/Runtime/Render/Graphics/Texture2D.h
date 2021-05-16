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
		 * 禁止拷贝和移动
		 */
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (Texture2D&&) = delete;

		/**
		 * \brief	顶点和索引数据是否已经上传到默认缓冲区
		 * \return	顶点和索引数据是否已经上传到默认缓冲区
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
		 * \brief	贴图路径
		 */
		std::string m_Path;

		/**
		 * \brief	顶点和索引数据是否已经上传到默认缓冲区
		 */
		bool m_IsReady = false;

		ComPtr<ID3D12Resource> m_ResourcePtr = nullptr;
		ComPtr<ID3D12Resource> m_UploadHeap = nullptr;

		/**
		 * \brief	常量缓冲区的索引
		 */
		uint32_t m_SrvIndex;

		/**
		 * \brief	描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_SrvDescriptor{};
	};
}