#include "ConstantBufferData.h"

#include "Camera/Camera.h"
#include "GameObject/Transform.h"
#include "Light.h"
#include "Material.h"

namespace SaplingEngine
{
	static size_t BaseColorPropertyID = StringToHash("baseColor");

	CommonOcbData::OcbData			CommonOcbData::m_Data;
	CommonPcbData::PcbData			CommonPcbData::m_Data;
	SpecialOcbData_Default::OcbData SpecialOcbData_Default::m_Data;
	SpecialOcbData0::OcbData		SpecialOcbData0::m_Data;
	SpecialOcbData1::OcbData		SpecialOcbData1::m_Data;

	/**
	 * \brief	�������
	 * \param	pTransform	�����Transform�����ָ��
	 * \return	���õ�����ָ��
	 */
	void* CommonOcbData::FillOcbData(Transform* pTransform)
	{
		//�������
		m_Data.SAPLING_MATRIX_M = pTransform->GetLocalToWorldMatrix().Transpose();
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	�������
	 * \param	pCamera		���ָ��
	 * \return	���õ�����ָ��
	 */
	void* CommonPcbData::FillPcbData(Camera* pCamera)
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
			m_Data.MAIN_LIGHT_DIRECTION.Set(-0.8f, -0.6f, 0.2f);
		}
		else
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();
			m_Data.MAIN_LIGHT_POSITION.Set(0, 0, 0);
			m_Data.MAIN_LIGHT_DIRECTION.Set(-0.8f, -0.6f, 0.2f);
		}
		
		return static_cast<void*>(&m_Data);
	}
	
	/**
	 * \brief	�������
	 * \param	size		�������ݴ�С
	 * \param	pMaterial	����Ĳ���ָ��
	 * \return	���õ�����ָ��
	 */
	void* SpecialOcbData_Default::FillOcbData(size_t& size, Material* pMaterial)
	{
		//�������
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);

		//�������ݴ�С
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	�������
	 * \param	size		�������ݴ�С
	 * \param	pMaterial	����Ĳ���ָ��
	 * \return	���õ�����ָ��
	 */
	void* SpecialOcbData0::FillOcbData(size_t& size, Material* pMaterial)
	{
		//�������
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);

		//�������ݴ�С
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	�������
	 * \param	size		�������ݴ�С
	 * \param	pMaterial	����Ĳ���ָ��
	 * \return	���õ�����ָ��
	 */
	void* SpecialOcbData1::FillOcbData(size_t& size, Material* pMaterial)
	{
		//�������
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);

		//�������ݴ�С
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	std::function<void* (size_t&, Material*)> GetFillSpecialOcbDataHandler(uint8_t shaderType)
	{
		switch (shaderType)
		{
			case 0:
				return SpecialOcbData0::FillOcbData;
			case 1:
				return SpecialOcbData1::FillOcbData;
			default:
				return SpecialOcbData_Default::FillOcbData;
		}
	}
}
