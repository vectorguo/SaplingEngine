#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Camera;
	class Scene;
	
	class RenderPass
	{
	protected:
		explicit RenderPass(const std::string& name);
		explicit RenderPass(std::string&& name);

	public:
		virtual ~RenderPass();

		RenderPass(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = delete;
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass& operator=(RenderPass&&) = delete;

		/**
		 * \brief	渲染
		 */
		virtual void Render() = 0;
		
		/**
		 * \brief	获取RenderPass名称
		 * \return	RenderPass名称
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief	获取优先级
		 * \return	优先级
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief	设置优先级
		 * \param	priority	优先级
		 */
		void SetPriority(int32_t priority)
		{
			m_Priority = priority;
		}

		/**
		 * \brief	设置宽度和高度
		 * \param	width		屏幕宽度
		 * \param	height		屏幕高度
		 */
		virtual void OnSceneResize(uint32_t width, uint32_t height)
		{

		}
		
	private:
		/**
		 * \brief	RenderPass名称
		 */
		std::string m_Name;

		/**
		 * \brief	RenderPass的渲染优先级
		 */
		int32_t m_Priority = 0;
	};
}
