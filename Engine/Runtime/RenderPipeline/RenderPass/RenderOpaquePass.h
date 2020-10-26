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
		 * \brief 渲染
		 * \param pCamera 渲染使用的相机
		 * \param pScene 被渲染场景
		 */
		void Render(const Camera* pCamera, const Scene* pScene) override;

		/**
		 * \brief 设置背景色
		 * \param color 背景色
		 */
		void SetBackgroundColor(const Color& color)
		{
			m_BackgroundColor = color;
		}

	private:
		/**
		 * \brief 背景色
		 */
		Color m_BackgroundColor;
	};
}