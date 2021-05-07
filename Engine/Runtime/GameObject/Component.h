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
		 * 禁止拷贝和移动
		 */
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator= (const Component&) = delete;
		Component& operator= (Component&&) = delete;

		/**
		 * \brief	获取组件类型。
		 *			每个组件都必须定义此函数。
		 *			GameObject对于每个ComponentType只能拥有一个Component。
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return 0;
		}

		/**
		 * \brief	获取组件类型
		 * \return	组件类型
		 */
		uint32_t GetComponentType() const
		{
			return m_ComponentType;
		}

		/**
		 * \brief	添加组件
		 */
		template<typename T>
		SharedPtr<T> AddComponent();

		/**
		 * \brief	销毁组件
		 */
		template<typename T>
		void DestroyComponent();

		/**
		 * \brief	获取组件
		 */
		template<typename T>
		SharedPtr<T> GetComponent();

		/**
		 * \brief	是否已经添加某组类型的组件
		 */
		template<typename T>
		bool HasComponent();

		/**
		 * \brief	获取GameObject指针
		 * \return	GameObject指针
		 */
		GameObject* GetGameObject() const
		{
			return m_GameObjectPtr;
		}

		/**
		 * \brief	获取Transform指针
		 * \return	Transform指针
		 */
		Transform* GetTransform() const;

		/**
		 * \brief	获取Transform智能指针
		 * \return	Transform智能指针
		 */
		TransformSptr& GetTransformSptr() const;

		/**
		 * \brief	是否是活动状态
		 */
		bool IsEnabled() const
		{
			return m_IsEnabled;
		}

		/**
		 * \brief	设置活动状态
		 * \param	enabled		是否使活动状态
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

		/**
		 * \brief 是否被销毁
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
		 * \brief	 组件类型
		 */
		uint32_t m_ComponentType;

	protected:
		/**
		 * \brief	 组件所在对象
		 */
		GameObject* m_GameObjectPtr = nullptr;

		/**
		 * \brief	是否处于活动状态
		 */
		bool m_IsEnabled = true;

		/**
		 * \brief	是否被销毁
		 */
		bool m_IsDestroyed = false;
	};
}