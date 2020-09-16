#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{	
	/*
	 * ǰ������
	 */
	class GameObject;
	
	class Component
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
			m_pOwner = std::move(pOwner);
		}
		
	protected:
		std::shared_ptr<GameObject> m_pOwner;
	};

	using ComponentStrongPtr = std::shared_ptr<Component>;
	using ComponentWeakPtr = std::weak_ptr<Component>;
}