#pragma once

#include "Runtime/SaplingEnginePch.h"
#include "Runtime/GameObject/Transform.h"

namespace SaplingEngine
{
	class GameObject
	{
		friend class Application;
		friend class Scene;
		friend GameObjectSptr CreateGameObject();
		friend GameObjectSptr CreateGameObject(const std::string&);
		friend GameObjectSptr CreateGameObject(std::string&&);
		friend void DestroyGameObject(GameObject*);
		friend void DestroyGameObject(const GameObjectSptr&);
		friend GameObjectSptr FindGameObject(const std::string&);
		
	public:
		explicit GameObject();
		GameObject(const std::string& name);
		GameObject(std::string&& name);
		~GameObject();

		/*
		 * 禁止拷贝和移动
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;
		
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
		TransformSptr GetTransformSptr() const
 		{
			return SharedFromThis(m_TransformPtr);
		}

		/**
		 * \brief 获取Transform
		 * \return Transform
		 */
		Transform* GetTransform() const
		{
			return m_TransformPtr;
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

	private:
		/**
		 * \brief	更新所有GameObjects
		 */
		static void UpdateAll();

		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode);

		/**
		 * \brief	序列化
		 */
		void Serialize();

		/**
		 * \brief	初始化
		 */
		void Initialize();

		/**
		 * \brief	设置对象被销毁
		 */
		void SetDestroyed();

	private:
		/**
		 * \brief	GameObject ID
		 */
		uint32_t m_Id = 0;

		/**
		 * \brief	GameObject Name
		 */
		std::string m_Name;

		/**
		 * \brief	是否处于活动状态
		 */
		bool m_IsActive = true;

		/**
		 * \brief	是否被标记销毁
		 */
		bool m_IsDestroyed = false;

		/**
		 * \brief	LayerMask
		 */
		int32_t m_LayerMask = 1;
		
		/**
		 * \brief	所有组件
		 */
		ComponentList m_Components;
		
		/**
		 * \brief	transform组件
		 */
		Scene* m_ScenePtr = nullptr;

		/**
		 * \brief	transform组件
		 */
		Transform* m_TransformPtr = nullptr;

	private:
		/**
		 * \brief	新创建的组件列表
		 */
		static ComponentList newComponents;

		/**
		 * \brief	要删除的组件列表
		 */
		static ComponentList destroyedComponents;

		/**
		 * \brief	新创建的GameObject列表
		 */
		static GameObjectList newGameObjects;

		/**
		 * \brief	所有已经创建的GameObject列表
		 */
		static GameObjectList allGameObjects;
	};

	/**
	 * \brief	创建GameObject
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject();

	/**
	 * \brief	创建GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject(const std::string& name);

	/**
	 * \brief	创建GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr CreateGameObject(std::string&& name);

	/**
	 * \brief	销毁GameObject
	 * \param	pGameObject		要被删除对象的指针
	 */
	inline void DestroyGameObject(GameObject* pGameObject)
	{
		pGameObject->SetDestroyed();
	}

	/**
	 * \brief	销毁GameObject
	 * \param	gameObject		要被删除对象的智能指针
	 */
	inline void DestroyGameObject(const GameObjectSptr& gameObject)
	{
		gameObject->SetDestroyed();
	}

	/**
	 * \brief	查找GameObject
	 * \param	name			GameObject名称
	 * \return	GameObject智能指针
	 */
	GameObjectSptr FindGameObject(const std::string& name);

	/*
	 * inline functions
	 */
	template <typename T>
	SharedPtr<T> GameObject::AddComponent()
	{
		if (!HasComponent<T>())
		{
			auto componentSptr = MakeShared<T>();
			newComponents.emplace_back(componentSptr);
			componentSptr->m_GameObjectPtr = this;
			componentSptr->Awake();
			return componentSptr;
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
				auto pComponent = iter->Get();
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
