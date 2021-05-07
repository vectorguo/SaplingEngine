#pragma once

#include "ComponentType.h"

namespace SaplingEngine
{
	class Component
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

		/**
		 * \brief	��ȡGameObjectָ��
		 * \return	GameObjectָ��
		 */
		GameObject* GetGameObject() const
		{
			return m_GameObjectPtr;
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
		/**
		 * \brief	 �������
		 */
		uint32_t m_ComponentType;

	protected:
		/**
		 * \brief	 ������ڶ���
		 */
		GameObject* m_GameObjectPtr = nullptr;

		/**
		 * \brief	�Ƿ��ڻ״̬
		 */
		bool m_IsEnabled = true;

		/**
		 * \brief	�Ƿ�����
		 */
		bool m_IsDestroyed = false;
	};
}