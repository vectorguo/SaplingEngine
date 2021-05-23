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

			Matrix4x4 SAPLING_MATRIX_SHADOW = Matrix4x4::Identity;

			/**
			 * \brief	��������ɫ
			 */
			Color AmbientLightColor{ 0.0f, 0.2f, 0.2f, 1.0f };

			/**
			 * \brief	��Ӱǿ��
			 */
			float ShadowStrength;

			/**
			 * \brief	���λ��
			 */
			Vector3 WorldSpaceCameraPosition{ 0, 0, 0 };

			LightData MainLight;
		};

		struct ShadowPcbData
		{
			Matrix4x4 SAPLING_MATRIX_V = Matrix4x4::Identity;
			Matrix4x4 SAPLING_MATRIX_VP = Matrix4x4::Identity;
		};

	public:
		/**
		 * \brief	�������
		 * \param	pCamera		���ָ��
		 * \return	���õ�����ָ��
		 */
		static void* FillPcbData(Camera* pCamera);

		/**
		 * \brief	�����ӰPass����
		 * \param	pShadowPass	ShadowPass
		 * \return	���õ�����ָ��
		 */
		static void* FillShadowPcbData(const ShadowPass* pShadowPass);

		/**
		 * \brief	Pass��ͨ�õĳ������������ݴ�С
		 */
		static const size_t DataSize = sizeof(PcbData);

		/**
		 * \brief	Pass��ͨ�õĳ������������ݴ�С
		 */
		static const size_t ShadowDataSize = sizeof(ShadowPcbData);

	private:
		static PcbData m_Data;
		static ShadowPcbData m_ShadowData;
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
			 * \brief	������
			 */
			float Metallic;

			/**
			 * \brief	����ֲڶ�
			 */
			float Roughness;

			/**
			 * \brief	ռλ��
			 */
			float Placeholder1;

			/**
			 * \brief	ռλ��
			 */
			float Placeholder2;

			/**
			 * \brief	ռλ��
			 */
			float Placeholder3;
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