#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	/**
	 * \brief	物体的通用常量缓冲区数据，不能超过256个字节
	 */
	class CommonOcbData final
	{
		struct OcbData
		{
			Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
		};

	public:
		/**
		 * \brief	填充数据
		 * \param	pTransform 物体的Transform组件的指针
		 * \return	填充好的数据指针
		 */
		static void* FillOcbData(Transform* pTransform);

		/**
		 * \brief	通用常量缓冲区数据大小
		 */
		static constexpr size_t DataSize = sizeof(OcbData);

	private:
		static OcbData m_Data;
	};

	/**
	 * \brief	每个Pass的通用的常量缓冲区数据，不能超过512个字节
	 */
	class CommonPcbData final
	{
		struct LightData
		{
			Color LightColor;
			Vector3 Direction;
			float Placeholder1;
			Vector3 Position;
			float Placeholder2;
		};

		struct PcbData
		{
			Matrix4x4 SAPLING_MATRIX_V = Matrix4x4::Identity;
			Matrix4x4 SAPLING_MATRIX_VP = Matrix4x4::Identity;

			Color AmbientLightColor{ 0.0f, 0.2f, 0.2f, 1.0f };

			Vector3 WorldSpaceCameraPosition{ 0, 0, 0 };

			/**
			 * \brief	占位符
			 */
			float Placeholder1;

			LightData MainLight;
		};

	public:
		/**
		 * \brief	填充数据
		 * \param	pCamera		相机指针
		 * \return	填充好的数据指针
		 */
		static void* FillPcbData(Camera* pCamera);

		/**
		 * \brief	Pass的通用的常量缓冲区数据大小
		 */
		static const size_t DataSize = sizeof(PcbData);

	private:
		static PcbData m_Data;
	};

	/**
	 * \brief 物体的特殊的常量缓冲区数据，不能超过512个字节
	 */
	class SpecialOcbData_Default final
	{
		struct OcbData
		{
			Color BaseColor;
		};

	public:

		/**
		 * \brief	填充数据
		 * \param	size		返回数据大小
		 * \param	pMaterial	物体的材质指针
		 * \return	填充好的数据指针
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	/**
	 * \brief 物体的特殊的常量缓冲区数据，不能超过512个字节
	 */
	class SpecialOcbData_Standard final
	{
		struct OcbData
		{
			/**
			 * \brief	基础颜色
			 */
			Color BaseColor;

			/**
			 * \brief	菲涅尔系数
			 */
			Vector3 Fresnel;

			/**
			 * \brief	表面粗糙度
			 */
			float Roughness;
		};

	public:
		/**
		 * \brief	填充数据
		 * \param	size		返回数据大小
		 * \param	pMaterial	物体的材质指针
		 * \return	填充好的数据指针
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	/**
	 * \brief 物体的特殊的常量缓冲区数据，不能超过512个字节
	 */
	class SpecialOcbData1 final
	{
		struct OcbData
		{
			Color BaseColor;
		};

	public:
		/**
		 * \brief	填充数据
		 * \param	size		返回数据大小
		 * \param	pMaterial	物体的材质指针
		 * \return	填充好的数据指针
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	std::function<void* (size_t&, Material*)> GetFillSpecialOcbDataHandler(uint8_t shaderType);
}