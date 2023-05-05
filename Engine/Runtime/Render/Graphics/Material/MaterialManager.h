#pragma once

#include "Runtime/SaplingEnginePch.h"

namespace SaplingEngine
{
	class MaterialManager
	{
	public:
		/**
		 * \brief	初始化
		 */
		static void Initialize();

		/**
		 * \brief	销毁
		 */
		static void Destroy();

		/**
		 * \brief	创建材质
		 * \param	shaderID		材质所用的ShaderID
		 * \return	材质的智能指针
		 */
		static MaterialSptr CreateMaterial(size_t shaderID);

		/**
		 * \brief	创建材质
		 * \param	shaderID		材质所用的ShaderID
		 * \return	材质的智能指针
		 */
		static MaterialSptr CreateMaterial(const std::string& shaderName);

		/**
		 * \brief	获取共享材质
		 * \param	materialName	材质名称
		 * \return	共享材质的智能指针
		 */
		static MaterialSptr GetSharedMaterial(const std::string& materialName);

		/**
		 * \brief	获取或创建共享材质
		 * \param	materialName	材质名称
		 * \param	shaderID		材质所用的ShaderID
		 * \return	共享材质的智能指针
		 */
		static MaterialSptr GetOrCreateSharedMaterial(const std::string& materialName, size_t shaderID);

		/**
		 * \brief	获取或创建共享材质
		 * \param	materialName	材质名称
		 * \param	shaderName		材质所用的Shader名称
		 */
		static MaterialSptr GetOrCreateSharedMaterial(const std::string& materialName, const std::string& shaderName);

	private:
		/**
		 * \brief	共享材质列表
		 */
		static std::map<std::string, MaterialSptr> sharedMaterials;
	};
}