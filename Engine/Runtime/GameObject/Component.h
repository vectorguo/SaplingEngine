#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	constexpr uint32_t ComponentType_Transform = 1;
	
	constexpr uint32_t ComponentType_Camera = 10;
	constexpr uint32_t ComponentType_CinemachineBrain = 11;
	constexpr uint32_t ComponentType_CinemachineVirtualCamera = 12;

	constexpr uint32_t ComponentType_Light = 20;

	constexpr uint32_t ComponentType_Renderer = 30;

	constexpr uint32_t ComponentType_InternalLimit = 10000;

	class GameObject;
	class Transform;
	using TransformSptr = std::shared_ptr<Transform>;
	
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
		 * \brief ��ȡGameObjectָ��
		 * \return GameObjectָ��
		 */
		GameObject* GetGameObject() const
		{
			return m_GameObjectSptr.get();
		}

		/**
		 * \brief ��ȡGameObject����ָ��
		 * \return GameObject����ָ��
		 */
		const std::shared_ptr<GameObject>& GetGameObjectSptr() const
		{
			return m_GameObjectSptr;
		}

		/**
		 * \brief ��ȡTransformָ��
		 * \return Transformָ��
		 */
		Transform* GetTransform() const;

		/**
		 * \brief ��ȡTransform����ָ��
		 * \return Transform����ָ��
		 */
		TransformSptr& GetTransformSptr() const;

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
		void SetGameObject(std::shared_ptr<GameObject> pOwner)
		{
			m_GameObjectSptr.swap(pOwner);
		}
		
	protected:
		std::shared_ptr<GameObject> m_GameObjectSptr;

		/**
		 * \brief �Ƿ��ڻ״̬
		 */
		bool m_IsEnabled = true;
	};

	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentMap = std::map<uint32_t, ComponentPtr>;
}