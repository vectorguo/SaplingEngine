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
	 * \brief 填充数据
	 * \param size 返回数据大小
	 * \param pTransform 物体的Transform组件的指针
	 * \return 填充好的数据指针
	 */
	void* CommonOcbData::FillOcbData(size_t& size, Transform* pTransform)
	{
		//填充数据
		m_Data.SAPLING_MATRIX_M = pTransform->GetLocalToWorldMatrix().Transpose();

		//设置数据大小
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief 填充数据
	 * \param size 返回数据大小
	 * \param pMaterial 物体的材质指针
	 * \return 填充好的数据指针
	 */
	void* SpecialOcbData::FillOcbData(size_t& size, Material* pMaterial)
	{
		//填充数据
		m_Data.BaseColor.Set(0.788f, 0.65f, 0.5f, 1);
		
		//设置数据大小
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief 填充数据
	 * \param size 返回数据大小
	 * \param pCamera 相机指针
	 * \return 填充好的数据指针
	 */
	void* CommonPcbData::FillPcbData(size_t& size, Camera* pCamera)
	{
		//填充数据
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

		//设置数据大小
		size = DataSize;
		
		return static_cast<void*>(&m_Data);
	}
}
