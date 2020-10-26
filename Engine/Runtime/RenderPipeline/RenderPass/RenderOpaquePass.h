#pragma once

#include "RenderPass.h"

namespace SaplingEngine
{
	class RenderOpaquePass final : public RenderPass
	{
	public:
		explicit RenderOpaquePass(const std::string& name)
			: RenderPass(name)
		{
			
		}

		explicit RenderOpaquePass(std::string&& name)
			: RenderPass(name)
		{
			
		}

		/**
		 * \brief ��Ⱦ
		 * \param pCamera ��Ⱦʹ�õ����
		 * \param pScene ����Ⱦ����
		 */
		void Render(const Camera* pCamera, const Scene* pScene) override;

		/**
		 * \brief ���ñ���ɫ
		 * \param color ����ɫ
		 */
		void SetBackgroundColor(const Color& color)
		{
			m_BackgroundColor = color;
		}

	private:
		/**
		 * \brief ����ɫ
		 */
		Color m_BackgroundColor;
	};
}