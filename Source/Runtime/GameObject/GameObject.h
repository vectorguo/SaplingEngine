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
		 * ��ֹ�������ƶ�
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
		 * Get Transform
		 */
		TransformWeakPtr GetTransform() const { return m_pTransform; }

		/*
		 * ��Ӻͻ�ȡComponent
		 */
		template<typename T>
		std::weak_ptr<T> AddComponent();

		template<typename T>
		std::weak_ptr<T> GetComponent();

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
		 * �Ƿ��ڻ״̬
		 */
		bool m_IsActive = true;

		/*
		 * �Ƿ񱻱������
		 */
		bool m_IsDestroyed = false;
		
		/*
		 * �������
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

	/*
	 * inline functions
	 */
	template <typename T>
	std::weak_ptr<T> GameObject::AddComponent()
	{
		constexpr auto componentType = T::GetComponentType();
		if (m_NewComponents.find(componentType) == m_NewComponents.end() && m_Components.find(componentType) ==
			m_Components.end())
		{
			//û�������ͬ���͵����
			auto componentPtr = std::make_shared<T>();
			componentPtr->SetOwner(shared_from_this());
			m_NewComponents.insert_or_assign(componentType, componentPtr);
			m_NewComponents[componentType]->Awake();
			return std::weak_ptr<T>(componentPtr);
		}
		else
		{
			//�Ѿ������ͬ���͵����
			return std::weak_ptr<T>();
		}
	}

	template <typename T>
	std::weak_ptr<T> GameObject::GetComponent()
	{
		auto iter = m_Components.find(T::GetComponentType());
		if (iter == m_Components.end())
		{
			return std::weak_ptr<T>();
		}

		return std::weak_ptr<T>(std::static_pointer_cast<T>(iter->second));
	}
}
