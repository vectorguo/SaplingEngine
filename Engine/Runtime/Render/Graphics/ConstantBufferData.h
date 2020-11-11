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
	 * \brief ǰ������
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
		 * \brief �������
		 * \param size �������ݴ�С
		 * \param pMaterial ����Ĳ���ָ��
		 * \return ���õ�����ָ��
		 */
		virtual void* FillOcbData(size_t& size, Material* pMaterial)
		{
			return nullptr;
		}
	};

	/**
	 * \brief ���������ĳ������������ݣ����ܳ���512���ֽ�
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
		 * \brief �������
		 * \param size �������ݴ�С
		 * \param pMaterial ����Ĳ���ָ��
		 * \return ���õ�����ָ��
		 */
		void* FillOcbData(size_t& size, Material* pMaterial) override;

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};
	
	/**
	 * \brief �����ͨ�ó������������ݣ����ܳ���256���ֽ�
	 */
	class CommonOcbData final
	{
		struct OcbData
		{
			Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
		};
		
	public:
		/**
		 * \brief �������
		 * \param size �������ݴ�С
		 * \param pTransform �����Transform�����ָ��
		 * \return ���õ�����ָ��
		 */
		static void* FillOcbData(size_t& size, Transform* pTransform);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	

	/**
	 * \brief ÿ��Pass��ͨ�õĳ������������ݣ����ܳ���512���ֽ�
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
		 * \brief �������
		 * \param size �������ݴ�С
		 * \param pCamera ���ָ��
		 * \return ���õ�����ָ��
		 */
		static void* FillPcbData(size_t& size, Camera* pCamera);

	private:
		static const size_t DataSize = sizeof(PcbData);
		static PcbData m_Data;
	};
}