#include "ConstantBufferData.h"

#include "Runtime/Camera/Camera.h"
#include "Runtime/Component/Transform.h"
#include "Runtime/Component/Light/Light.h"
#include "Runtime/Component/Light/LightManager.h"
#include "Runtime/Render/Graphics/Material/Material.h"
#include "Runtime/Render/RenderPipeline/RenderPipeline.h"
#include "Runtime/Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Runtime/Scene/Scene.h"
#include "Runtime/Scene/SceneManager.h"

namespace SaplingEngine
{
	static size_t BaseColorPropertyID	= StringToHash("baseColor");
	static size_t FresnelPropertyID		= StringToHash("fresnel");
	static size_t MetallicPropertyID	= StringToHash("metallic");
	static size_t RoughnessPropertyID	= StringToHash("roughness");

	CommonOcbData::OcbData				CommonOcbData::m_Data;
	CommonPcbData::PcbData				CommonPcbData::m_Data;
	CommonPcbData::ShadowPcbData		CommonPcbData::m_ShadowData;
	SpecialOcbData_Default::OcbData		SpecialOcbData_Default::m_Data;
	SpecialOcbData_Standard::OcbData	SpecialOcbData_Standard::m_Data;
	SpecialOcbData1::OcbData			SpecialOcbData1::m_Data;

	/**
	 * \brief	填充数据
	 * \param	pTransform	物体的Transform组件的指针
	 * \return	填充好的数据指针
	 */
	void* CommonOcbData::FillOcbData(Transform* pTransform)
	{
		//填充数据
		m_Data.SAPLING_MATRIX_M = pTransform->GetLocalToWorldMatrix().Transpose();
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	填充数据
	 * \param	pCamera		相机指针
	 * \return	填充好的数据指针
	 */
	void* CommonPcbData::FillPcbData(Camera* pCamera)
	{
		//填充数据
		const auto& worldToViewMatrix = pCamera->GetWorldToViewMatrix();
		const auto& worldToProjMatrix = worldToViewMatrix * pCamera->GetViewToProjMatrix();
		const auto* pDirectionalLight = LightManager::GetDirectionalLight();
		const auto* pShadowPass = RenderPipeline::GetShadowPass();
		if (pDirectionalLight)
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();
			m_Data.SAPLING_MATRIX_SHADOW = pShadowPass->GetWorldToShadowMatrix().Transpose();

			//设置主光源参数
			m_Data.MainLight.LightColor = pDirectionalLight->GetLightColor();
			m_Data.MainLight.Direction = pDirectionalLight->GetLightDirection();
			m_Data.MainLight.Position = pDirectionalLight->GetLightPosition();

			//设置阴影强度
			m_Data.ShadowStrength = pDirectionalLight->GetShadowStrength();
		}
		else
		{
			m_Data.SAPLING_MATRIX_V = worldToViewMatrix.Transpose();
			m_Data.SAPLING_MATRIX_VP = worldToProjMatrix.Transpose();
			m_Data.SAPLING_MATRIX_SHADOW = pShadowPass->GetWorldToShadowMatrix().Transpose();

			//设置主光源参数
			m_Data.MainLight.LightColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
			m_Data.MainLight.Direction.Set(-0.8f, -0.6f, 0.2f);
			m_Data.MainLight.Position.Set(0, 0, 0);

			//设置阴影强度
			m_Data.ShadowStrength = 1.0f;
		}

		//设置场景的环境光
		auto* pActiveScene = SceneManager::GetActiveScene();
		m_Data.AmbientLightColor = pActiveScene->GetAmbientLightColor();

		//设置相机参数
		m_Data.WorldSpaceCameraPosition = pCamera->GetTransform()->GetPosition();
		
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	填充阴影Pass数据
	 * \param	pShadowPass	ShadowPass
	 * \return	填充好的数据指针
	 */
	void* CommonPcbData::FillShadowPcbData(const ShadowPass* pShadowPass)
	{
		auto vp = pShadowPass->GetWorldToLightMatrix() * pShadowPass->GetLightToProjMatrix();
		m_ShadowData.SAPLING_MATRIX_V = pShadowPass->GetWorldToLightMatrix().Transpose();
		m_ShadowData.SAPLING_MATRIX_VP = vp.Transpose();

		return static_cast<void*>(&m_ShadowData);
	}
	
	/**
	 * \brief	填充数据
	 * \param	size		返回数据大小
	 * \param	pMaterial	物体的材质指针
	 * \return	填充好的数据指针
	 */
	void* SpecialOcbData_Default::FillOcbData(size_t& size, Material* pMaterial)
	{
		//填充数据
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);

		//设置数据大小
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	填充数据
	 * \param	size		返回数据大小
	 * \param	pMaterial	物体的材质指针
	 * \return	填充好的数据指针
	 */
	void* SpecialOcbData_Standard::FillOcbData(size_t& size, Material* pMaterial)
	{
		//填充数据
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);
		m_Data.Fresnel = pMaterial->GetVector3(FresnelPropertyID);
		m_Data.Metallic = pMaterial->GetFloat(MetallicPropertyID);
		m_Data.Roughness = pMaterial->GetFloat(RoughnessPropertyID);

		//设置数据大小
		size = DataSize;
		return static_cast<void*>(&m_Data);
	}

	/**
	 * \brief	填充数据
	 * \param	size		返回数据大小
	 * \param	pMaterial	物体的材质指针
	 * \return	填充好的数据指针
	 */
	void* SpecialOcbData1::FillOcbData(size_t& size, Material* pMaterial)
	{
		//填充数据
		m_Data.BaseColor = pMaterial->GetColor(BaseColorPropertyID);

		//设置数据大小
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
