#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	/**
	 * \brief	�����ͨ�ó������������ݣ����ܳ���256���ֽ�
	 */
	class CommonOcbData final
	{
		struct OcbData
		{
			Matrix4x4 SAPLING_MATRIX_M = Matrix4x4::Identity;
		};

	public:
		/**
		 * \brief	�������
		 * \param	pTransform �����Transform�����ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillOcbData(Transform* pTransform);

		/**
		 * \brief	ͨ�ó������������ݴ�С
		 */
		static constexpr size_t DataSize = sizeof(OcbData);

	private:
		static OcbData m_Data;
	};

	/**
	 * \brief	ÿ��Pass��ͨ�õĳ������������ݣ����ܳ���512���ֽ�
	 */
	class CommonPcbData final
	{
		struct PcbData
		{
			Matrix4x4 SAPLING_MATRIX_V = Matrix4x4::Identity;
			Matrix4x4 SAPLING_MATRIX_VP = Matrix4x4::Identity;

			Color AMBIENT_LIGHT_COLOR{ 0.0f, 0.2f, 0.2f, 1.0f };

			Color MAIN_LIGHT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
			Vector3 MAIN_LIGHT_DIRECTION{ 0, 0, 0 };
			Vector3 MAIN_LIGHT_POSITION{ 0, 0, 0 };
		};

	public:
		/**
		 * \brief	�������
		 * \param	pCamera		���ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillPcbData(Camera* pCamera);

		/**
		 * \brief	Pass��ͨ�õĳ������������ݴ�С
		 */
		static const size_t DataSize = sizeof(PcbData);

	private:
		static PcbData m_Data;
	};

	/**
	 * \brief ���������ĳ������������ݣ����ܳ���512���ֽ�
	 */
	class SpecialOcbData_Default final
	{
		struct OcbData
		{
			Color BaseColor;
		};

	public:

		/**
		 * \brief	�������
		 * \param	size		�������ݴ�С
		 * \param	pMaterial	����Ĳ���ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	/**
	 * \brief ���������ĳ������������ݣ����ܳ���512���ֽ�
	 */
	class SpecialOcbData_Standard final
	{
		struct OcbData
		{
			/**
			 * \brief	������ɫ
			 */
			Color BaseColor;

			/**
			 * \brief	������ϵ��
			 */
			Vector3 Fresnel;

			/**
			 * \brief	����ֲڶ�
			 */
			float Roughness;
		};

	public:
		/**
		 * \brief	�������
		 * \param	size		�������ݴ�С
		 * \param	pMaterial	����Ĳ���ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	/**
	 * \brief ���������ĳ������������ݣ����ܳ���512���ֽ�
	 */
	class SpecialOcbData1 final
	{
		struct OcbData
		{
			Color BaseColor;
		};

	public:
		/**
		 * \brief	�������
		 * \param	size		�������ݴ�С
		 * \param	pMaterial	����Ĳ���ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillOcbData(size_t& size, Material* pMaterial);

	private:
		static const size_t DataSize = sizeof(OcbData);
		static OcbData m_Data;
	};

	std::function<void* (size_t&, Material*)> GetFillSpecialOcbDataHandler(uint8_t shaderType);
}