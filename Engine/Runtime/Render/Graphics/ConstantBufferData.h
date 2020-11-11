#pragma once

#include "Math/Color.h"
#include "Math/Matrix4x4.h"

namespace SaplingEngine
{
	using Math::Color;
	using Math::Matrix4x4;
	using Math::Vector3;
	using Math::Vector4;

	/**
	 * \brief 前置声明
	 */
	class Camera;
	class Material;
	class Transform;
	
	class ISpecialOcbData
	{
	public:
		ISpecialOcbData() = default;
		virtual ~ISpecialOcbData() = default;
		ISpecialOcbData(const ISpecialOcbData&) = delete;
		ISpecialOcbData(ISpecialOcbData&&) = delete;
		ISpecialOcbData& operator=(const ISpecialOcbData&) = delete;
		ISpecialOcbData& operator=(ISpecialOcbData&&) = delete;
		
		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pMaterial 物体的材质指针
		 * \return 填充好的数据指针
		 */
		virtual void* FillOcbData(size_t& size, Material* pMaterial)
		{
			return nullptr;
		}
	};

	/**
	 * \brief 物体的特殊的常量缓冲区数据，不能超过512个字节
	 */
	class SpecialOcbData final : public ISpecialOcbData
	{
		struct OcbData
		{
			Color BaseColor;
		};

	public:
		SpecialOcbData() = default;
		~SpecialOcbData() override = default;
		SpecialOcbData(const SpecialOcbData&) = delete;
		SpecialOcbData(SpecialOcbData&&) = delete;
		SpecialOcbData& operator=(const SpecialOcbData&) = delete;
		SpecialOcbData& operator=(SpecialOcbData&&) = delete;

		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pMaterial 物体的材质指针
		 * \return 填充好的数据指针
		 */
		void* FillOcbData(size_t& size, Material* pMaterial) override;

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};
	
	/**
	 * \brief 物体的通用常量缓冲区数据，不能超过256个字节
	 */
	class CommonOcbData final
	{
		struct OcbData
		{
			Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
		};
		
	public:
		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pTransform 物体的Transform组件的指针
		 * \return 填充好的数据指针
		 */
		static void* FillOcbData(size_t& size, Transform* pTransform);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	

	/**
	 * \brief 每个Pass的通用的常量缓冲区数据，不能超过512个字节
	 */
	class CommonPcbData final
	{
		struct PcbData
		{
			Matrix4x4 SAPLING_MATRIX_V = Matrix4x4::Identity;
			Matrix4x4 SAPLING_MATRIX_VP = Matrix4x4::Identity;

			Vector3 MAIN_LIGHT_POSITION { 0, 0, 0 };
			Vector3 MAIN_LIGHT_DIRECTION { 0, 0, 0 };
		};

	public:
		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pCamera 相机指针
		 * \return 填充好的数据指针
		 */
		static void* FillPcbData(size_t& size, Camera* pCamera);

	private:
		static const size_t DataSize = sizeof(PcbData);
		static PcbData m_Data;
	};
}