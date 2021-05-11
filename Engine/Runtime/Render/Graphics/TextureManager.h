#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class TextureManager
	{
	public:
		static void Initialize();

		/**
		 * \brief	创建Texture2D
		 * \param	path		Texture2D的资源路径
		 * \return	指向创建好的Texture2D的智能指针
		 */
		static Texture2DSptr CreateTexture2D(const std::string& path);

		/**
		 * \brief	删除Texture2D
		 * \param	textureSptr	要被删除的Texture2D的智能指针的引用
		 */
		static void DestroyTexture2D(Texture2DSptr&& textureSptr);

		/**
		 * \brief	上传Mesh数据到GPU
		 */
		static void UploadTextureDatas();

	public:
		static Texture2DSptr White;

	private:
		/**
		 * \brief	数据没有准备好的Texture2D
		 */
		static std::vector<Texture2D*> unreadyTexture2Ds;
	};
}