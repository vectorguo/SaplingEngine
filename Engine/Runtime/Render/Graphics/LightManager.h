#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class LightManager
	{
		friend class Light;
	public:
		/**
		 * \brief	获取平行光
		 * \return	平行光指针
		 */
		static Light* GetDirectionalLight()
		{
			return pDirectionalLight;
		}

	private:
		/**
		 * \brief	设置平行光
		 * \param	pLight		平行光指针
		 */
		static void SetDirectionalLight(Light* pLight)
		{
			pDirectionalLight = pLight;
		}

	private:
		static Light* pDirectionalLight;
	};
}