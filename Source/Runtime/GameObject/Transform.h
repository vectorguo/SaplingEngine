#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return 1;
		}

	protected:
		void Awake() override
		{
		
		}

		void Update() override
		{
			
		}
	};

	using TransformStrongPtr = std::shared_ptr<Transform>;
	using TransformWeakPtr = std::weak_ptr<Transform>;
}