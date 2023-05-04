#pragma once

#include "Runtime/GameObject/GameObject.h"

namespace SaplingEngine
{
	class Renderer;
	
	class Scene
	{
		friend class GameObject;
		friend GameObjectSptr CreateGameObject();
		friend GameObjectSptr CreateGameObject(const std::string&);
		friend GameObjectSptr CreateGameObject(std::string&&);
		friend void DestroyGameObject(GameObject*);
		friend void DestroyGameObject(const GameObjectSptr&);
		
	public:
		explicit Scene(const std::string& sceneName);
		~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;
		
		/**
		 * \brief	初始化场景
		 * \param	pNode		配置节点指针
		 */
		void Initialize(const XmlNode* pNode);

		/**
		 * \brief	更新
		 */
		void Update();

		/**
		 * \brief	销毁场景
		 */
		void Destroy();
		
		/**
		 * \brief	是否是活动状态
		 * \return	活动状态
		 */
		bool IsActive() const
		{
			return m_IsActive;
		}
		
		/**
		 * \brief	设置活动状态
		 * \param	active		是否时活动状态
		 */
		void SetActive(bool active);

		/**
		 * \brief	获取所有游戏对象
		 * \return	游戏对象
		 */
		const std::vector<GameObject*>& GetGameObjects() const
		{
			return m_GameObjects;
		}

		/**
		 * \brief	获取所有游戏对象
		 * \return	游戏对象
		 */
		std::vector<GameObject*>& GetGameObjects()
		{
			return m_GameObjects;
		}

		/**
		 * \brief	获取环境光颜色
		 * \return	环境光颜色
		 */
		const Color& GetAmbientLightColor() const
		{
			return m_AmbientLightColor;
		}

		/**
		 * \brief	设置环境光颜色
		 * \param	color		环境光颜色
		 */
		void SetAmbientLightColor(const Color& color)
		{
			m_AmbientLightColor = color;
		}

		/**
		 * \brief	设置环境光颜色
		 * \param	r			环境光颜色r通道
		 * \param	g			环境光颜色g通道
		 * \param	b			环境光颜色b通道
		 * \param	a			环境光颜色a通道
		 */
		void SetAmbientLightColor(float r, float g, float b, float a)
		{
			m_AmbientLightColor.Set(r, g, b, a);
		}

		/**
		 * \brief	获取场景包围盒
		 */
		const DirectX::BoundingSphere& GetSceneBounds() const
		{
			return m_SceneBounds;
		}

		/**
		 * \brief	设置场景包围盒的中心
		 * \param	center		新的场景中心
		 */
		void SetCenterOfSceneBounds(const Vector3& center)
		{
			m_SceneBounds.Center = center;
		}
		
	private:
		/**
		 * \brief	添加游戏对象
		 * \param	pGameObject	游戏对象指针
		 */
		void AddGameObject(GameObject* pGameObject);

		/**
		 * \brief	删除游戏对象
		 * \param	pGameObject	游戏对象指针
		 */
		void RemoveGameObject(GameObject* pGameObject);

	private:
		/**
		 * \brief	场景名称
		 */
		std::string m_SceneName;

		/**
		 * \brief	是否活动状态
		 */
		bool m_IsActive;

		/**
		 * \brief	此场景中的GameObject
		 */
		std::vector<GameObject*> m_GameObjects;

		/**
		 * \brief	此场景中的环境光颜色
		 */
		Color m_AmbientLightColor;

		/**
		 * \brief	场景包围盒
		 */
		DirectX::BoundingSphere m_SceneBounds;
	};
}