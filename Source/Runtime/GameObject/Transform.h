#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Transform final : public Component
	{
	public:
		Transform();

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
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