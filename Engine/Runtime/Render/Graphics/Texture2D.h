#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	class Texture2D
	{
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

		inline D3D12_GPU_DESCRIPTOR_HANDLE GetSrvDescriptor() const
		{
			return m_SrvDescriptor;
		}

	private:
		/**
		 * \brief	加载贴图数据
		 */
		void LoadTextureData();

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
		uint32_t m_SrvIndex = 0;

		/**
		 * \brief	描述符
		 */
		D3D12_GPU_DESCRIPTOR_HANDLE m_SrvDescriptor{};
	};
}