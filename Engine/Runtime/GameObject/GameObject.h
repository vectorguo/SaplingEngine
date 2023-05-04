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
		 * ��ֹ�������ƶ�
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;
		
		/**
		 * \brief ����
		 */
		void Update();
		
		/**
		 * \brief ����
		 */
		void Destroy();

		/**
		 * \brief ��ȡGameObject ID
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
		 * \brief ��ȡLayerMask
		 * \return LayerMask
		 */
		int32_t GetLayerMask() const
		{
			return m_LayerMask;
		}

		/**
		 * \brief ��ȡTransform
		 * \return Transform
		 */
		TransformSptr GetTransformSptr() const
 		{
			return SharedFromThis(m_TransformPtr);
		}

		/**
		 * \brief ��ȡTransform
		 * \return Transform
		 */
		Transform* GetTransform() const
		{
			return m_TransformPtr;
		}

		/**
		 * \brief	������
		 */
		template<typename T>
		SharedPtr<T> AddComponent();

		/**
		 * \brief	�������
		 */
		template<typename T>
		void DestroyComponent();

		/**
		 * \brief	��ȡ���
		 */
		template<typename T>
		SharedPtr<T> GetComponent();

		/**
		 * \brief	�Ƿ��Ѿ����ĳ�����͵����
		 */
		template<typename T>
		bool HasComponent();

	private:
		/**
		 * \brief	��������GameObjects
		 */
		static void UpdateAll();

		/**
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode);

		/**
		 * \brief	���л�
		 */
		void Serialize();

		/**
		 * \brief	��ʼ��
		 */
		void Initialize();

		/**
		 * \brief	���ö�������
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
		 * \brief	�Ƿ��ڻ״̬
		 */
		bool m_IsActive = true;

		/**
		 * \brief	�Ƿ񱻱������
		 */
		bool m_IsDestroyed = false;

		/**
		 * \brief	LayerMask
		 */
		int32_t m_LayerMask = 1;
		
		/**
		 * \brief	�������
		 */
		ComponentList m_Components;
		
		/**
		 * \brief	transform���
		 */
		Scene* m_ScenePtr = nullptr;

		/**
		 * \brief	transform���
		 */
		Transform* m_TransformPtr = nullptr;

	private:
		/**
		 * \brief	�´���������б�
		 */
		static ComponentList newComponents;

		/**
		 * \brief	Ҫɾ��������б�
		 */
		static ComponentList destroyedComponents;

		/**
		 * \brief	�´�����GameObject�б�
		 */
		static GameObjectList newGameObjects;

		/**
		 * \brief	�����Ѿ�������GameObject�б�
		 */
		static GameObjectList allGameObjects;
	};

	/**
	 * \brief	����GameObject
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject();

	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject(const std::string& name);

	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
	 */
	GameObjectSptr CreateGameObject(std::string&& name);

	/**
	 * \brief	����GameObject
	 * \param	pGameObject		Ҫ��ɾ�������ָ��
	 */
	inline void DestroyGameObject(GameObject* pGameObject)
	{
		pGameObject->SetDestroyed();
	}

	/**
	 * \brief	����GameObject
	 * \param	gameObject		Ҫ��ɾ�����������ָ��
	 */
	inline void DestroyGameObject(const GameObjectSptr& gameObject)
	{
		gameObject->SetDestroyed();
	}

	/**
	 * \brief	����GameObject
	 * \param	name			GameObject����
	 * \return	GameObject����ָ��
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
