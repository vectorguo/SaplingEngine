#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class TextureManager
	{
	public:
		static void Initialize();

		/**
		 * \brief	����Texture2D
		 * \param	path		Texture2D����Դ·��
		 * \return	ָ�򴴽��õ�Texture2D������ָ��
		 */
		static Texture2DSptr CreateTexture2D(const std::string& path);

		/**
		 * \brief	ɾ��Texture2D
		 * \param	textureSptr	Ҫ��ɾ����Texture2D������ָ�������
		 */
		static void DestroyTexture2D(Texture2DSptr&& textureSptr);

		/**
		 * \brief	�ϴ�Mesh���ݵ�GPU
		 */
		static void UploadTextureDatas();

	public:
		static Texture2DSptr White;

	private:
		/**
		 * \brief	����û��׼���õ�Texture2D
		 */
		static std::vector<Texture2D*> unreadyTexture2Ds;
	};
}