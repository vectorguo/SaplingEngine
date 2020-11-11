#include "ConstantBufferData.h"

#include "Light.h"
#include "Camera/Camera.h"
#include "GameObject/Transform.h"

namespace SaplingEngine
{
	CommonOcbData::OcbData CommonOcbData::m_Data;
	SpecialOcbData::OcbData SpecialOcbData::m_Data;
	CommonPcbData::PcbData CommonPcbData::m_Data;
	
	/**
	 * \brief �������
	 * \param size �������ݴ�С
	 * \param pTransform �����Transform�����ָ��
	 * \return ���õ�����ָ��
	 */
	void* CommonOcbData::FillOcbData(size_t& size, Transform* pTransform)
	{
		//�������
		m_Data.SAPLING_MATRIX_M = pTransform->GetLocalToWorldMatrix().Transpose();

		//�������ݴ�С
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief �������
	 * \param size �������ݴ�С
	 * \param pMaterial ����Ĳ���ָ��
	 * \return ���õ�����ָ��
	 */
	void* SpecialOcbData::FillOcbData(size_t& size, Material* pMaterial)
	{
		//�������
		m_Data.BaseColor.Set(0.788f, 0.65f, 0.5f, 1);
		
		//�������ݴ�С
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief �������
	 * \param size �������ݴ�С
	 * \param pCamera ���ָ��
	 * \return ���õ�����ָ��
	 */
	void* CommonPcbData::FillPcbData(size_t& size, Camera* pCamera)
	{
		//�������
		const auto& worldToViewMatrix = pCamera->GetWorldToViewMatrix();
		const auto& worldToProjMatrix = worldToViewMatrix * pCamera->GetViewToProjMatrix();
		const auto* light = Light::Instance();
		if (light)
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();
			m_Data.MAIN_LIGHT_POSITION.Set(0, 0, 0);
			m_Data.MAIN_LIGHT_DIRECTION.Set(0.8f, 0.6f, -0.2f);
		}
		else
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();
			m_Data.MAIN_LIGHT_POSITION.Set(0, 0, 0);
			m_Data.MAIN_LIGHT_DIRECTION.Set(0.8f, 0.6f, -0.2f);
		}

		//�������ݴ�С
		size = DataSize;
		
		return static_cast<void*>(&m_Data);
	}
}
