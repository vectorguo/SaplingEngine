#pragma once

#include "SaplingEnginePch.h"
#include "Component.h"
#include "Transform.h"

namespace SaplingEngine
{
	class GameObject;
	class Scene;
	
	using GameObjectSptr = std::shared_ptr<GameObject>;
	using GameObjectList = std::vector<GameObjectSptr>;
	
	/*
	 * ����GameObject
	 */
	void DestroyGameObject(const GameObjectSptr& gameObject);
	
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
		friend void DestroyGameObject(const GameObjectSptr&);
		
	public:
		explicit GameObject(uint32_t id);
		GameObject(uint32_t id, const std::string& name);
		GameObject(uint32_t id, std::string&& name);
		~GameObject();

		/*
		 * ��ֹ�������ƶ�
		 */
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator= (const GameObject&) = delete;
		GameObject& operator= (GameObject&&) = delete;

		/**
		 * \brief	�����´��������
		 */
		static void HandleNewComponents();

		/**
		 * \brief	����Ҫ���ٵ����
		 */
		static void HandleDestroyedComponents();
		
		/**
		 * \brief ��ʼ��
		 * \param pScene ��������
		 * \param isDeserialized �Ƿ�ʱ�����л���GameObject��ʼ��
		 * \return ��ʼ���Ƿ�ɹ�
		 */
		bool Initialize(Scene* pScene, bool isDeserialized = false);
		
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
		 * \brief �Ƿ�������״̬
		 */
		bool IsDestroyed() const { return m_IsDestroyed; }

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
		const TransformSptr& GetTransformSptr() const
		{
			return m_Transform;
		}

		/**
		 * \brief ��ȡTransform
		 * \return Transform
		 */
		TransformSptr& GetTransformSptr()
		{
			return m_Transform;
		}

		/**
		 * \brief ��ȡTransform
		 * \return Transform
		 */
		Transform* GetTransform() const
		{
			return m_Transform.get();
		}

		/**
		 * \brief ��ȡparent����ָ��
		 * \return parent����ָ��
		 */
		const GameObjectSptr& GetParentSptr() const
		{
			return m_Parent;
		}

		/**
		 * \brief ��ȡparentָ��
		 * \return parentָ��
		 */
		GameObject* GetParent() const
		{
			return m_Parent.get();
		}

		/**
		 * \brief �Ƿ��и��ڵ�
		 * \return �Ƿ��и��ڵ�
		 */
		bool HasParent() const
		{
			return m_Parent != nullptr;
		}

		/**
		 * \brief ����parent
		 * \param parent parent
		 */
		void SetParent(const GameObjectSptr& parent);

		/**
		 * \brief ��ȡ���ڳ���ָ��
		 * \return ����ָ��
		 */
		Scene* GetScene() const
		{
			return m_pScene;
		}
		
		/**
		 * \brief	������
		 */
		template<typename T>
		std::shared_ptr<T> AddComponent();

		/**
		 * \brief	��ȡ���
		 */
		template<typename T>
		std::shared_ptr<T> GetComponent();

		/**
		 * \brief	�Ƿ��Ѿ����ĳ�����͵����
		 */
		template<typename T>
		bool HasComponent();

		/**
		 * \brief	�������
		 */
		template<typename T>
		void DestroyComponent();
		
		/**
		 * \brief ���л�
		 */
		void Serialize();

		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode);

	private:
		/**
		 * \brief ��������ֻ�����ͨ��ComponentFactory���������
		 * \param componentType �������
		 * \param pComponent Ҫ����ӵ����ָ��
		 */
		void AddComponent(uint32_t componentType, Component* pComponent);
		
		/**
		 * \brief ����
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
		 * \brief LayerMask
		 */
		int32_t m_LayerMask = 1;

		/**
		 * \brief �Ƿ��ڻ״̬
		 */
		bool m_IsActive = true;

		/**
		 * \brief �Ƿ񱻱������
		 */
		bool m_IsDestroyed = false;
		
		/*
		 * �������
		 */	
		ComponentList m_Components;
		
		/**
		 * \brief transform���
		 */
		TransformSptr m_Transform;

		/**
		 * \brief ���ڵ�
		 */
		GameObjectSptr m_Parent;
		
		/**
		 * \brief �ӽڵ�
		 */
		GameObjectList m_Children;

		/**
		 * \brief ���ڳ�����ָ��
		 */
		Scene* m_pScene = nullptr;

	private:
		/**
		 * \brief	�´���������б�
		 */
		static ComponentList newComponents;

		/**
		 * \brief	Ҫɾ��������б�
		 */
		static ComponentList destroyedComponents;
	};

	/*
	 * inline functions
	 */
	template <typename T>
	std::shared_ptr<T> GameObject::AddComponent()
	{
		if (!HasComponent<T>())
		{
			auto componentPtr = std::make_shared<T>();
			newComponents.emplace_back(componentPtr);
			componentPtr->SetGameObject(shared_from_this());
			componentPtr->Awake();
			return componentPtr;
		}
		else
		{
			return nullptr;
		}
	}

	template <typename T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		for (auto iter = newComponents.begin(); iter != newComponents.end(); ++iter)
		{
			auto& componentSptr = *iter;
			if (componentSptr->GetGameObject() == this && componentSptr->GetComponentType() == T::GetStaticComponentType())
			{
				return std::static_pointer_cast<T>(componentSptr);
			}
		}

		auto iter = std::find_if(m_Components.begin(), m_Components.end(), [](const ComponentSptr& componentSptr)
			{
				return componentSptr->GetComponentType() == T::GetStaticComponentType();
			});
		return iter == m_Components.end() ? nullptr : std::static_pointer_cast<T>(*iter);
	}

	template <typename T>
	bool GameObject::HasComponent()
	{
		for (auto iter = newComponents.begin(); iter != newComponents.end(); ++iter)
		{
			auto pComponent = iter->get();
			if (pComponent->GetGameObject() == this && pComponent->GetComponentType() == T::GetStaticComponentType())
			{
				return true;
			}
		}

		auto iter = std::find_if(m_Components.begin(), m_Components.end(), [](const ComponentSptr& componentSptr)
			{
				return componentSptr->GetComponentType() == T::GetStaticComponentType();
			});
		return iter != m_Components.end();
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
}
