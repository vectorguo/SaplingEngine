#include "ConstantBufferData.h"

#include "Camera/Camera.h"
#include "GameObject/Transform.h"
#include "Render/Graphics/Light.h"
#include "Render/Graphics/LightManager.h"
#include "Render/Graphics/Material.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	static size_t BaseColorPropertyID	= StringToHash("baseColor");
	static size_t FresnelPropertyID		= StringToHash("fresnel");
	static size_t RoughnessPropertyID	= StringToHash("roughness");

	CommonOcbData::OcbData				CommonOcbData::m_Data;
	CommonPcbData::PcbData				CommonPcbData::m_Data;
	SpecialOcbData_Default::OcbData		SpecialOcbData_Default::m_Data;
	SpecialOcbData_Standard::OcbData	SpecialOcbData_Standard::m_Data;
	SpecialOcbData1::OcbData			SpecialOcbData1::m_Data;

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
		const auto* pDirectionalLight = LightManager::GetDirectionalLight();
		if (pDirectionalLight)
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();

			//��������Դ����
			m_Data.MAIN_LIGHT_COLOR = pDirectionalLight->GetLightColor();
			m_Data.MAIN_LIGHT_DIRECTION = pDirectionalLight->GetLightDirection();
			m_Data.MAIN_LIGHT_POSITION = pDirectionalLight->GetLightPosition();
		}
		else
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();

			//��������Դ����
			m_Data.MAIN_LIGHT_COLOR.Set(1.0f, 1.0f, 1.0f, 1.0f);
			m_Data.MAIN_LIGHT_DIRECTION.Set(-0.8f, -0.6f, 0.2f);
			m_Data.MAIN_LIGHT_POSITION.Set(0, 0, 0);
		}

		//���ó����Ļ�����
		auto* pActiveScene = SceneManager::GetActiveScene();
		m_Data.AMBIENT_LIGHT_COLOR = pActiveScene->GetAmbientLightColor();
		
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
	void* SpecialOcbData_Standard::FillOcbData(size_t& size, Material* pMaterial)
	{
		//�������
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);
		m_Data.Fresnel = pMaterial->GetVector3(FresnelPropertyID);
		m_Data.Roughness = pMaterial->GetFloat(RoughnessPropertyID);

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
				return SpecialOcbData_Standard::FillOcbData;
			case 1:
				return SpecialOcbData1::FillOcbData;
			default:
				return SpecialOcbData_Default::FillOcbData;
		}
	}
}
