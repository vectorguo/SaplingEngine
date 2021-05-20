#pragma once

#include "Render/RenderPipeline/RenderPass/RenderPass.h"

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
		 * \brief äÖÈ¾
		 */
		void Render() override;

		/**
		 * \brief ÉèÖÃ±³¾°É«
		 * \param color ±³¾°É«
		 */
		void SetBackgroundColor(const Color& color)
		{
			m_BackgroundColor = color;
		}

	private:
		/**
		 * \brief ±³¾°É«
		 */
		Color m_BackgroundColor;
	};
}