#pragma once

#include "SaplingEnginePch.h"
#include "Component.h"
#include "Transform.h"

namespace SaplingEngine
{
	class GameObject;
	using GameObjectPtr = std::shared_ptr<GameObject>;
	using GameObjectList = std::vector<GameObjectPtr>;
	
	/*
	 * 销毁GameObject
	 */
	void DestroyGameObject(const GameObjectPtr& gameObject);
	
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
		friend void DestroyGameObject(const GameObjectPtr&);
		
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
		
		bool Initialize();
		void Update();
		void Destroy();

		/*
		 * Get ID
		 */
		uint32_t GetId() const { return m_Id; }

		/*
		 * Get And Set Name
		 */
		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
		void SetName(std::string&& name) { m_Name = std::move(name); }

		/*
		 * Get And Set Active
		 */
		bool IsActive() const { return m_IsActive; }
		void SetActive(bool active);

		/*
		 * Get Destroyed
		 */
		bool IsDestroyed() const { return m_IsDestroyed; }

		/*
		 * 添加和获取Component
		 */
		template<typename T>
		std::shared_ptr<T> AddComponent();

		template<typename T>
		std::shared_ptr<T> GetComponent();

	private:
		/**
		 * \brief 销毁
		 */
		void DestroyInternal();

	private:
		/**
		 * \brief GameObject ID
		 */
		uint32_t m_Id = 0;

		/**
		 * \brief GameObject Name
		 */
		std::string m_Name;

		/**
		 * \brief 是否处于活动状态
		 */
		bool m_IsActive = true;

		/**
		 * \brief 是否被标记销毁
		 */
		bool m_IsDestroyed = false;
		
		/*
		 * 所有组件
		 */	
		ComponentMap m_Components;
		ComponentMap m_NewComponents;
		
		/**
		 * \brief transform组件
		 */
		TransformPtr m_Transform;

		/**
		 * \brief 子节点
		 */
		GameObjectList m_Children;

		/**
		 * \brief 父节点
		 */
		GameObjectPtr m_Parent;
	};

	/*
	 * inline functions
	 */
	template <typename T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		constexpr auto componentType = T::GetComponentType();
		if (m_NewComponents.find(componentType) == m_NewComponents.end() && m_Components.find(componentType) == m_Components.end())
		{
			//没有添加相同类型的组件
			auto componentPtr = std::make_shared<T>();
			componentPtr->SetOwner(shared_from_this());
			m_NewComponents.insert_or_assign(componentType, componentPtr);
			m_NewComponents[componentType]->Awake();
			return componentPtr;
		}
		else
		{
			//已经添加相同类型的组件
			return nullptr;
		}
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		auto iter = m_Components.find(T::GetComponentType());
		if (iter == m_Components.end())
		{
			return nullptr;
		}

		return std::static_pointer_cast<T>(iter->second);
	}
}
