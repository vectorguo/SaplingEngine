#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class LightManager
	{
		friend class Light;
	public:
		/**
		 * \brief	��ȡƽ�й�
		 * \return	ƽ�й�ָ��
		 */
		static Light* GetDirectionalLight()
		{
			return pDirectionalLight;
		}

	private:
		/**
		 * \brief	����ƽ�й�
		 * \param	pLight		ƽ�й�ָ��
		 */
		static void SetDirectionalLight(Light* pLight)
		{
			pDirectionalLight = pLight;
		}

	private:
		static Light* pDirectionalLight;
	};
}