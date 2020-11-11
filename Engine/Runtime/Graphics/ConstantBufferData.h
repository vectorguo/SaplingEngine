#pragma once

#include "Math/Matrix4x4.h"

namespace SaplingEngine
{
	using Math::Matrix4x4;
	using Math::Vector3;

	/**
	 * \brief 前置声明
	 */
	class Camera;
	class Transform;
	
	class IConstantBufferData
	{
	public:
		IConstantBufferData() = default;
		virtual ~IConstantBufferData() = default;
		IConstantBufferData(const IConstantBufferData&) = delete;
		IConstantBufferData(IConstantBufferData&&) = delete;
		IConstantBufferData& operator=(const IConstantBufferData&) = delete;
		IConstantBufferData& operator=(IConstantBufferData&&) = delete;
		
		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pTransform 物体的Transform组件的指针
		 * \return 填充好的数据指针
		 */
		virtual void* FillOcbData(size_t& size, Transform* pTransform)
		{
			return nullptr;
		}
	};
	
	/**
	 * \brief 物体的通用常量缓冲区数据，不能超过256个字节
	 */
	class CommonOcbData final : public IConstantBufferData
	{
		struct OcbData
		{
			Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
		};
		
	public:
		CommonOcbData() = default;
		~CommonOcbData() override = default;
		CommonOcbData(const CommonOcbData&) = delete;
		CommonOcbData(CommonOcbData&&) = delete;
		CommonOcbData& operator=(const CommonOcbData&) = delete;
		CommonOcbData& operator=(CommonOcbData&&) = delete;

		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pTransform 物体的Transform组件的指针
		 * \return 填充好的数据指针
		 */
		void* FillOcbData(size_t& size, Transform* pTransform) override;

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	/**
	 * \brief 物体的特殊的常量缓冲区数据，不能超过512个字节
	 */
	class SpecialOcbData final : public IConstantBufferData
	{
		struct OcbData
		{
			
		};
		
	public:
		SpecialOcbData() = default;
		~SpecialOcbData() override  = default;
		SpecialOcbData(const SpecialOcbData&) = delete;
		SpecialOcbData(SpecialOcbData&&) = delete;
		SpecialOcbData& operator=(const SpecialOcbData&) = delete;
		SpecialOcbData& operator=(SpecialOcbData&&) = delete;

		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pTransform 物体的Transform组件的指针
		 * \return 填充好的数据指针
		 */
		void* FillOcbData(size_t& size, Transform* pTransform) override;
		
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
		CommonPcbData() = default;
		~CommonPcbData() = default;
		CommonPcbData(const CommonPcbData&) = delete;
		CommonPcbData(CommonPcbData&&) = delete;
		CommonPcbData& operator=(const CommonPcbData&) = delete;
		CommonPcbData& operator=(CommonPcbData&&) = delete;

		/**
		 * \brief 填充数据
		 * \param size 返回数据大小
		 * \param pCamera 相机指针
		 * \return 填充好的数据指针
		 */
		void* FillPcbData(size_t& size, Camera* pCamera);

	private:
		static const size_t DataSize = sizeof(PcbData);
		static PcbData m_Data;
	};
}