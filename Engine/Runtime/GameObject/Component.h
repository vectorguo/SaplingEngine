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
		 * 禁止拷贝和移动
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

		/*
		 * 获取组件类型
		 * 每个组件都必须定义此函数
		 * GameObject对于每个ComponentType只能拥有一个Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return 0;
		}

		/**
		 * \brief 获取GameObject指针
		 * \return GameObject指针
		 */
		GameObject* GetGameObject() const
		{
			return m_GameObjectSptr.get();
		}

		/**
		 * \brief 获取GameObject智能指针
		 * \return GameObject智能指针
		 */
		const std::shared_ptr<GameObject>& GetGameObjectSptr() const
		{
			return m_GameObjectSptr;
		}

		/**
		 * \brief 获取Transform指针
		 * \return Transform指针
		 */
		Transform* GetTransform() const;

		/**
		 * \brief 获取Transform智能指针
		 * \return Transform智能指针
		 */
		TransformSptr& GetTransformSptr() const;

		/**
		 * \brief 是否是活动状态
		 */
		bool IsEnabled() const
		{
			return m_IsEnabled;
		}
		
		/**
		 * \brief 设置活动状态
		 * \param enabled 是否使活动状态
		 */
		void SetEnabled(bool enabled);

		/**
		 * \brief 序列化
		 */
		virtual void Serialize();

		/**
		 * \brief 反序列化
		 * \param pNode 配置节点指针
		 * \return 反序列化是否成功
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
		 * \brief 是否处于活动状态
		 */
		bool m_IsEnabled = true;
	};

	using ComponentPtr = std::shared_ptr<Component>;
	using ComponentMap = std::map<uint32_t, ComponentPtr>;
}