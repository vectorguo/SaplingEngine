#pragma once

#include "Render/Graphics/Mesh.h"
#include "Resource/ResourceAsyncRequest.h"
#include "Resource/ResourceLoader.h"

namespace SaplingEngine
{
	class ResourceManager final
	{
	public:
		/**
		 * \brief	��ʼ��
		 * \return	��ʼ���Ƿ�ɹ�
		 */
		static bool Initialize();

		/**
		 * \brief	����
		 */
		static void Update();

		/**
		 * \brief	������Դ
		 * \param	path		��Դ·��
		 */
		template <typename T>
		static SharedPtr<T> Load(const std::string& path);

		/**
		 * \brief	����Mesh
		 * \param	path		Mesh·��
		 */
		template <>
		static SharedPtr<Mesh> Load<Mesh>(const std::string& path)
		{
			auto* pAsset = LoadMeshAsset(path);
			return MakeShared<Mesh>(pAsset);
		}

		/**
		 * \brief	�첽������Դ
		 * \param	path		��Դ·��
		 * \return	�첽��������
		 */
		template <typename T>
		static ResourceAsyncRequestSptr LoadAsync(const std::string& path);

		/**
		 * \brief	�첽����Mesh
		 * \param	path		Mesh·��
		 * \return	�첽��������
		 */
		template <>
		static ResourceAsyncRequestSptr LoadAsync<Mesh>(const std::string& path)
		{

		}
		
	private:
		/**
		 * \brief	�����첽���������б�
		 */
		static std::vector<ResourceAsyncRequestSptr> asyncRequests;
	};
}
