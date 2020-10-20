#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	constexpr uint32_t ComponentType_Transform = 1;
	
	constexpr uint32_t ComponentType_Camera = 2;
	constexpr uint32_t ComponentType_Renderer = 3;
	constexpr uint32_t ComponentType_InternalLimit = ComponentType_Renderer;
	
	class Component : public std::enable_shared_from_this<Component>
	{
		friend class GameObject;
		
	public:
		Component();
		virtual ~Component();

		/*
		 * ��ֹ�������ƶ�
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return 0;
		}

		/**
		 * \brief �Ƿ��ǻ״̬
		 */
		bool IsEnabled() const
		{
			return m_IsEnabled;
		}
		
		/**
		 * \brief ���û״̬
		 * \param enabled �Ƿ�ʹ�״̬
		 */
		void SetEnabled(bool enabled);

		/**
		 * \brief ���л�
		 */
		virtual void Serialize();

		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		virtual bool Deserialize(const XmlNode* pNode);
		
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
			m_pOwner.swap(pOwner);
		}
		
	protected:
		std::shared_ptr<GameObject> m_pOwner;

		/**
		 * \brief �Ƿ��ڻ״̬
		 */
		bool m_IsEnabled = true;
	};

	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentMap = std::map<uint32_t, ComponentPtr>;
}