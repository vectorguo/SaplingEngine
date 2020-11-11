#pragma once

#include "Renderer.h"

namespace SaplingEngine
{	
	class MeshRenderer final : public Renderer
	{
	public:
		MeshRenderer();
		~MeshRenderer() override;

		/*
		 * 禁止拷贝和移动
		 */
		MeshRenderer(const MeshRenderer&) = delete;
		MeshRenderer(MeshRenderer&&) = delete;
		MeshRenderer& operator= (const MeshRenderer&) = delete;
		MeshRenderer& operator= (MeshRenderer&&) = delete;
		
		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Renderer;
		}
		
		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode) override;
	};
}
