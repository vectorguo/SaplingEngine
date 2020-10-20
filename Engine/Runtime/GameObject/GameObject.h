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
	 * ����GameObject
	 */
	void DestroyGameObject(const GameObjectPtr& gameObject);
	
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
		friend void DestroyGameObject(const GameObjectPtr&);
		
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
		 * \brief ��ʼ��
		 * \param isDeserialized �Ƿ�ʱ�����л���GameObject��ʼ��
		 * \return ��ʼ���Ƿ�ɹ�
		 */
		bool Initialize(bool isDeserialized = false);
		
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
		 * \brief ������
		 */
		template<typename T>
		std::shared_ptr<T> AddComponent();

		/**
		 * \brief ��ȡ���
		 */
		template<typename T>
		std::shared_ptr<T> GetComponent();

		/**
		 * \brief �������
		 */
		template<typename T>
		void DestroyComponent();

		/**
		 * \brief ����parent
		 * \param parent parent
		 */
		void SetParent(const GameObjectPtr& parent);

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
		ComponentMap m_Components;
		ComponentMap m_NewComponents;
		std::vector<uint32_t> m_DestroyedComponents;
		
		/**
		 * \brief transform���
		 */
		TransformPtr m_Transform;

		/**
		 * \brief �ӽڵ�
		 */
		GameObjectList m_Children;

		/**
		 * \brief ���ڵ�
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
			//û�������ͬ���͵����
			auto componentPtr = std::make_shared<T>();
			componentPtr->SetOwner(shared_from_this());
			m_NewComponents.insert_or_assign(componentType, componentPtr);
			m_NewComponents[componentType]->Awake();
			return componentPtr;
		}
		else
		{
			//�Ѿ������ͬ���͵����
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

	template <typename T>
	void GameObject::DestroyComponent()
	{
		m_DestroyedComponents.push_back(T::GetComponentType());
	}
}
