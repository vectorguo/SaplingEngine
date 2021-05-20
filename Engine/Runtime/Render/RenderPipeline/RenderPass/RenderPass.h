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
		 * \brief	��Ⱦ
		 */
		virtual void Render() = 0;
		
		/**
		 * \brief	��ȡRenderPass����
		 * \return	RenderPass����
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}

		/**
		 * \brief	��ȡ���ȼ�
		 * \return	���ȼ�
		 */
		int32_t GetPriority() const
		{
			return m_Priority;
		}

		/**
		 * \brief	�������ȼ�
		 * \param	priority	���ȼ�
		 */
		void SetPriority(int32_t priority)
		{
			m_Priority = priority;
		}

		/**
		 * \brief	���ÿ�Ⱥ͸߶�
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 */
		virtual void OnSceneResize(uint32_t width, uint32_t height)
		{

		}
		
	private:
		/**
		 * \brief	RenderPass����
		 */
		std::string m_Name;

		/**
		 * \brief	RenderPass����Ⱦ���ȼ�
		 */
		int32_t m_Priority = 0;
	};
}
