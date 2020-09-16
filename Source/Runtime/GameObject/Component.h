#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{	
	/*
	 * 前向声明
	 */
	class GameObject;
	
	class Component
	{
		friend class GameObject;
		
	public:
		Component();
		virtual ~Component();

		/*
		 * 禁止拷贝和移动
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return 0;
		}
		
	protected:
		virtual void Awake()
		{
			
		}

		virtual void Start()
		{
			
		}

		virtual void Update()
		{
			
		}

		virtual void LateUpdate()
		{
			
		}

		virtual void OnEnable()
		{
			
		}

		virtual void OnDisable()
		{
			
		}

		virtual void OnDestroy()
		{
			
		}

	private:
		void SetOwner(std::shared_ptr<GameObject> pOwner)
		{
			m_pOwner = std::move(pOwner);
		}
		
	protected:
		std::shared_ptr<GameObject> m_pOwner;
	};

	using ComponentStrongPtr = std::shared_ptr<Component>;
	using ComponentWeakPtr = std::weak_ptr<Component>;
}