#pragma once

#include "GameObject/GameObject.h"

namespace SaplingEngine
{
	class Renderer;
	
	class Scene
	{
		friend class GameObject;
		
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;
		
		/**
		 * \brief 初始化场景
		 * \param pNode 配置节点指针
		 */
		void Initialize(const XmlNode* pNode);

		/**
		 * \brief 更新
		 */
		void Update();

		/**
		 * \brief 销毁场景
		 */
		void Destroy();
		
		/**
		 * \brief 是否是活动状态
		 * \return 活动状态
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}
		
		/**
		 * \brief 设置活动状态
		 * \param active 是否时活动状态
		 */
		void SetActive(bool active);

		/**
		 * \brief 创建GameObject
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject();
		
		/**
		 * \brief 创建GameObject
		 * \param name GameObject名称
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject(const std::string& name);

		/**
		 * \brief 创建GameObject
		 * \param name GameObject名称
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObject(std::string&& name);

		/**
		 * \brief 获取对象
		 * \param name 对象名称
		 * \return 对象只能指针
		 */
		GameObjectSptr GetGameObject(const std::string& name);

		/**
		 * \brief 获取所有游戏对象
		 * \return 游戏对象
		 */
		const std::vector<GameObjectSptr>& GetGameObjects() const
		{
			return m_GameObjects;
		}

		/**
		 * \brief 获取所有游戏对象
		 * \return 游戏对象
		 */
		std::vector<GameObjectSptr>& GetGameObjects()
		{
			return m_GameObjects;
		}

		/**
		 * \brief 添加渲染项
		 * \param renderItem renderer指针
		 */
		void AddRenderItem(Renderer* renderItem)
		{
			m_RenderItems.push_back(renderItem);
		}

		/**
		 * \brief 获取所有渲染项
		 * \return 所有渲染项
		 */
		const std::vector<Renderer*>& GetRenderItems() const
		{
			return m_RenderItems;
		}
		
		/**
		 * \brief 删除渲染项
		 * \param renderItem renderer指针
		 */
		void RemoveRenderItem(Renderer* renderItem)
		{
			m_RenderItems.erase(std::find(m_RenderItems.begin(), m_RenderItems.end(), renderItem));
		}

	private:
		/**
		 * \brief 创建GameObject
		 * \return GameObject
		 */
		GameObjectSptr CreateGameObjectInternal();
		
	private:
		/**
		 * \brief 场景名称
		 */
		std::string m_SceneName;

		/**
		 * \brief 是否活动状态
		 */
		bool m_IsActive;

		/**
		 * \brief 此场景中的GameObject
		 */
		std::vector<GameObjectSptr> m_GameObjects;

		/**
		 * \brief 渲染项列表
		 */
		std::vector<Renderer*> m_RenderItems;
	};
}