#pragma once

#include "SaplingEnginePch.h"
#include "GameObject/Component.h"
#include "GameObject/Transform.h"

namespace SaplingEngine
{
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
		using ComponentMap = std::map<uint32_t, ComponentStrongPtr>;
		
	public:
		explicit GameObject(uint32_t id);
		GameObject(uint32_t id, const std::string& name);
		GameObject(uint32_t id, std::string&& name);
		~GameObject() = default;

		/*
		 * 禁止拷贝和移动
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;
		
		/*
		 * 初始化
		 */
		bool Initialize();

		/*
		 * 更新
		 */
		void Update();

		/*
		 * 销毁
		 */
		void Destroy();

		/*
		 * 获取ID
		 */
		uint32_t GetId() const
		{
			return m_Id;
		}
		
		/*
		 * 获取和设置名称
		 */
		const std::string& GetName() const
		{
			return m_Name;
		}
		
		void SetName(const std::string& name)
		{
			m_Name = name;
		}

		void SetName(std::string&& name)
		{
			m_Name = std::move(name);
		}

		/*
		 * Transform
		 */
		TransformWeakPtr GetTransform() const
		{
			return m_pTransform;
		}

		/*
		 * 获取和设置活动状态
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}

		void SetActive(bool active);

		/*
		 * 添加和获取Component
		 */
		template<typename T>
		std::weak_ptr<T> AddComponent()
		{
			constexpr auto componentType = T::GetComponentType();
			if (m_NewComponents.find(componentType) == m_NewComponents.end() && m_Components.find(componentType) == m_Components.end())
			{
				//没有添加相同类型的组件
				auto componentPtr = std::make_shared<T>();
				componentPtr->SetOwner(shared_from_this());
				m_NewComponents.insert_or_assign(componentType, componentPtr);
				m_NewComponents[componentType]->Awake();
				return std::weak_ptr<T>(componentPtr);
			}
			else
			{
				//已经添加相同类型的组件
				return std::weak_ptr<T>();
			}
		}

		template<typename T>
		std::weak_ptr<T> GetComponent()
		{
			auto iter = m_Components.find(T::GetComponentType());
			if (iter == m_Components.end())
			{
				return std::weak_ptr<T>();
			}

			return std::weak_ptr<T>(std::static_pointer_cast<T>(iter->second));
		}

	private:
		/**
		 * \brief GameObject ID
		 */
		uint32_t m_Id = 0;

		/*
		 * GameObject Name
		 */
		std::string m_Name;

		/*
		 * 是否处于活动状态
		 */
		bool m_IsActive = true;

		/*
		 * 所有组件
		 */
		ComponentMap m_Components;
		ComponentMap m_NewComponents;

		/*
		 * Transform
		 */
		TransformWeakPtr m_pTransform;
	};

	using GameObjectStrongPtr = std::shared_ptr<GameObject>;
	using GameObjectWeakPtr = std::weak_ptr<GameObject>;
}