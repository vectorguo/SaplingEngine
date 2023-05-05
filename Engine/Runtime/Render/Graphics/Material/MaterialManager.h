#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	class MaterialManager
	{
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
		 * \brief	��������
		 * \param	shaderID		�������õ�ShaderID
		 * \return	���ʵ�����ָ��
		 */
		static MaterialSptr CreateMaterial(size_t shaderID);

		/**
		 * \brief	��������
		 * \param	shaderID		�������õ�ShaderID
		 * \return	���ʵ�����ָ��
		 */
		static MaterialSptr CreateMaterial(const std::string& shaderName);

		/**
		 * \brief	��ȡ�������
		 * \param	materialName	��������
		 * \return	������ʵ�����ָ��
		 */
		static MaterialSptr GetSharedMaterial(const std::string& materialName);

		/**
		 * \brief	��ȡ�򴴽��������
		 * \param	materialName	��������
		 * \param	shaderID		�������õ�ShaderID
		 * \return	������ʵ�����ָ��
		 */
		static MaterialSptr GetOrCreateSharedMaterial(const std::string& materialName, size_t shaderID);

		/**
		 * \brief	��ȡ�򴴽��������
		 * \param	materialName	��������
		 * \param	shaderName		�������õ�Shader����
		 */
		static MaterialSptr GetOrCreateSharedMaterial(const std::string& materialName, const std::string& shaderName);

	private:
		/**
		 * \brief	��������б�
		 */
		static std::map<std::string, MaterialSptr> sharedMaterials;
	};
}