#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class GameObject
	{
		friend void DestroyGameObject(const GameObjectSptr&);
		
	public:
		explicit GameObject(uint32_t id);
		GameObject(uint32_t id, const std::string& name);
		GameObject(uint32_t id, std::string&& name);
		~GameObject();

		/*
		 * 禁止拷贝和移动
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;

		/**
		 * \brief	处理新创建的组件
		 */
		static void HandleNewComponents();

		/**
		 * \brief	处理要销毁的组件
		 */
		static void HandleDestroyedComponents();
		
		/**
		 * \brief 初始化
		 * \param pScene 所属场景
		 * \param isDeserialized 是否时反序列化的GameObject初始化
		 * \return 初始化是否成功
		 */
		bool Initialize(Scene* pScene, bool isDeserialized = false);
		
		/**
		 * \brief 更新
		 */
		void Update();
		
		/**
		 * \brief 销毁
		 */
		void Destroy();

		/**
		 * \brief 获取GameObject ID
		 * \return GameObject ID
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

		/**
		 * \brief 是否处于销毁状态
		 */
		bool IsDestroyed() const { return m_IsDestroyed; }

		/**
		 * \brief 获取LayerMask
		 * \return LayerMask
		 */
		int32_t GetLayerMask() const
		{
			return m_LayerMask;
		}

		/**
		 * \brief 获取Transform
		 * \return Transform
		 */
		const TransformSptr& GetTransformSptr() const
		{
			return m_TransformSptr;
		}

		/**
		 * \brief 获取Transform
		 * \return Transform
		 */
		Transform* GetTransform() const
		{
			return m_TransformSptr.Get();
		}

		/**
		 * \brief 获取parent智能指针
		 * \return parent智能指针
		 */
		const GameObjectSptr& GetParentSptr() const
		{
			return m_Parent;
		}

		/**
		 * \brief 获取parent指针
		 * \return parent指针
		 */
		GameObject* GetParent() const
		{
			return m_Parent.Get();
		}

		/**
		 * \brief 是否有父节点
		 * \return 是否有父节点
		 */
		bool HasParent() const
		{
			return m_Parent != nullptr;
		}

		/**
		 * \brief 设置parent
		 * \param parent parent
		 */
		void SetParent(const GameObjectSptr& parent);

		/**
		 * \brief 获取所在场景指针
		 * \return 场景指针
		 */
		Scene* GetScene() const
		{
			return m_pScene;
		}
		
		/**
		 * \brief	添加组件
		 */
		template<typename T>
		SharedPtr<T> AddComponent();

		/**
		 * \brief	销毁组件
		 */
		template<typename T>
		void DestroyComponent();

		/**
		 * \brief	获取组件
		 */
		template<typename T>
		SharedPtr<T> GetComponent();

		/**
		 * \brief	是否已经添加某组类型的组件
		 */
		template<typename T>
		bool HasComponent();
		
		/**
		 * \brief 序列化
		 */
		void Serialize();

		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode);

	private:
		/**
		 * \brief 添加组件，只能添加通过ComponentFactory创建的组件
		 * \param componentType 组件类型
		 * \param pComponent 要被添加的组件指针
		 */
		void AddComponent(uint32_t componentType, Component* pComponent);
		
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

		/**
		 * \brief LayerMask
		 */
		int32_t m_LayerMask = 1;
		
		/*
		 * 所有组件
		 */	
		ComponentList m_Components;
		
		/**
		 * \brief transform组件
		 */
		TransformSptr m_TransformSptr;

		/**
		 * \brief 父节点
		 */
		GameObjectSptr m_Parent;
		
		/**
		 * \brief 子节点
		 */
		GameObjectList m_Children;

		/**
		 * \brief 所在场景的指针
		 */
		Scene* m_pScene = nullptr;

	private:
		/**
		 * \brief	新创建的组件列表
		 */
		static ComponentList newComponents;

		/**
		 * \brief	要删除的组件列表
		 */
		static ComponentList destroyedComponents;
	};

	/*
	 * 销毁GameObject
	 */
	void DestroyGameObject(const GameObjectSptr& gameObject);

	/*
	 * inline functions
	 */
	template <typename T>
	SharedPtr<T> GameObject::AddComponent()
	{
		if (!HasComponent<T>())
		{
			auto componentPtr = MakeShared<T>();
			newComponents.emplace_back(componentPtr);
			componentPtr->m_GameObjectPtr = this;
			componentPtr->Awake();
			return componentPtr;
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	void GameObject::DestroyComponent()
	{
		auto componentSptr = GetComponent<T>();
		if (componentSptr != nullptr)
		{
			destroyedComponents.emplace_back(std::move(componentSptr));
		}
	}

	template <typename T>
	SharedPtr<T> GameObject::GetComponent()
	{
		if (!newComponents.empty())
		{
			for (auto iter = newComponents.begin(); iter != newComponents.end(); ++iter)
			{
				auto& componentSptr = *iter;
				if (componentSptr->GetGameObject() == this && componentSptr->GetComponentType() == T::GetStaticComponentType())
				{
					return StaticPointerCast<T>(componentSptr);
				}
			}
		}

		auto iter = std::find_if(m_Components.begin(), m_Components.end(), [](const ComponentSptr& componentSptr)
			{
				return componentSptr->GetComponentType() == T::GetStaticComponentType();
			});
		return iter == m_Components.end() ? nullptr : StaticPointerCast<T>(*iter);
	}

	template <typename T>
	bool GameObject::HasComponent()
	{
		if (!newComponents.empty())
		{
			for (auto iter = newComponents.begin(); iter != newComponents.end(); ++iter)
			{
				auto pComponent = iter->get();
				if (pComponent->GetGameObject() == this && pComponent->GetComponentType() == T::GetStaticComponentType())
				{
					return true;
				}
			}
		}

		auto iter = std::find_if(m_Components.begin(), m_Components.end(), [](const ComponentSptr& componentSptr)
			{
				return componentSptr->GetComponentType() == T::GetStaticComponentType();
			});
		return iter != m_Components.end();
	}

	template<typename T>
	inline SharedPtr<T> Component::AddComponent()
	{
		return m_GameObjectPtr->AddComponent<T>();
	}

	template<typename T>
	inline void Component::DestroyComponent()
	{
		m_GameObjectPtr->DestroyComponent<T>();
	}
	
	template<typename T>
	inline SharedPtr<T> Component::GetComponent()
	{
		return m_GameObjectPtr->GetComponent<T>();
	}
	
	template<typename T>
	inline bool Component::HasComponent()
	{
		return m_GameObjectPtr->HasComponent<T>();
	}
}
