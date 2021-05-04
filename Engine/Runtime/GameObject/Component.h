#pragma once

#include "ComponentType.h"

namespace SaplingEngine
{
	class GameObject;
	class Transform;
	using TransformSptr = std::shared_ptr<Transform>;
	
	class Component : public std::enable_shared_from_this<Component>
	{
		friend class GameObject;

	public:
		Component(uint32_t componentType);
		virtual ~Component();

		/*
		 * ��ֹ�������ƶ�
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return 0;
		}

		/**
		 * \brief	��ȡ�������
		 * \return	�������
		 */
		uint32_t GetComponentType() const
		{
			return m_ComponentType;
		}

		/**
		 * \brief	��ȡGameObjectָ��
		 * \return	GameObjectָ��
		 */
		GameObject* GetGameObject() const
		{
			return m_GameObjectSptr.get();
		}

		/**
		 * \brief	��ȡGameObject����ָ��
		 * \return	GameObject����ָ��
		 */
		const std::shared_ptr<GameObject>& GetGameObjectSptr() const
		{
			return m_GameObjectSptr;
		}

		/**
		 * \brief	��ȡTransformָ��
		 * \return	Transformָ��
		 */
		Transform* GetTransform() const;

		/**
		 * \brief	��ȡTransform����ָ��
		 * \return	Transform����ָ��
		 */
		TransformSptr& GetTransformSptr() const;

		/**
		 * \brief	�Ƿ��ǻ״̬
		 */
		bool IsEnabled() const
		{
			return m_IsEnabled;
		}

		/**
		 * \brief	���û״̬
		 * \param	enabled		�Ƿ�ʹ�״̬
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

		/**
		 * \brief �Ƿ�����
		 */
		bool IsDestroyed() const
		{
			return m_IsDestroyed;
		}
		
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
		void SetGameObject(std::shared_ptr<GameObject> pOwner)
		{
			m_GameObjectSptr.swap(pOwner);
		}
		
	private:
		/**
		 * \brief	 �������
		 */
		uint32_t m_ComponentType;

	protected:
		/**
		 * \brief	 ������ڶ���
		 */
		std::shared_ptr<GameObject> m_GameObjectSptr;

		/**
		 * \brief	�Ƿ��ڻ״̬
		 */
		bool m_IsEnabled = true;

		/**
		 * \brief	�Ƿ�����
		 */
		bool m_IsDestroyed = false;
	};

	using ComponentSptr = std::shared_ptr<Component>;
	using ComponentList = std::vector<ComponentSptr>;
	using ComponentMap = std::map<uint32_t, ComponentSptr>;
}