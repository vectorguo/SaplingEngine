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
		
		/*
		 * ��ʼ��
		 */
		bool Initialize();

		/*
		 * ����
		 */
		void Update();

		/*
		 * ����
		 */
		void Destroy();

		/*
		 * ��ȡID
		 */
		uint32_t GetId() const
		{
			return m_Id;
		}
		
		/*
		 * ��ȡ����������
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
		TransformStrongPtr GetTransform() const
		{
			return m_pTransform;
		}

		/*
		 * ��ȡ�����û״̬
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}

		void SetActive(bool active);

		/*
		 * ��Ӻͻ�ȡComponent
		 */
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			constexpr auto componentType = T::GetComponentType();
			if (m_NewComponents.find(componentType) == m_NewComponents.end() && m_Components.find(componentType) == m_Components.end())
			{
				//û�������ͬ���͵����
				auto componentPtr = std::make_shared<T>();
				componentPtr->SetOwner(shared_from_this());
				componentPtr->Awake();
				m_NewComponents.insert_or_assign(componentType, componentPtr);
				return componentPtr;
			}
			else
			{
				//�Ѿ������ͬ���͵����
				return nullptr;
			}
		}

		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			auto iter = m_Components.find(T::GetComponentType());
			if (iter == m_Components.end())
			{
				return nullptr;
			}

			return std::dynamic_pointer_cast<T>(iter->second);
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
		 * �Ƿ��ڻ״̬
		 */
		bool m_IsActive = true;

		/*
		 * �������
		 */
		ComponentMap m_Components;
		ComponentMap m_NewComponents;

		/*
		 * Transform
		 */
		TransformStrongPtr m_pTransform;
	};

	using GameObjectStrongPtr = std::shared_ptr<GameObject>;
	using GameObjectWeakPtr = std::weak_ptr<GameObject>;
}