#include "Scene.h"

namespace SaplingEngine
{
	
	/**
	 * \brief GameObjectȫ��ID
	 */
	static uint32_t GameObjectId = 0;
	
	/**
	 * \brief ���캯��
	 * \param sceneName ��������
	 */
	Scene::Scene(const std::string& sceneName) : m_SceneName(sceneName), m_IsActive(false)
	{
		 
	}

	Scene::~Scene() = default;

	/**
	 * \brief ��ʼ������
	 * \param pNode ���ýڵ�ָ��
	 */
	void Scene::Initialize(const XmlNode* pNode)
	{
		for (auto* pChild = pNode->first_node(); pChild != nullptr; pChild = pChild->next_sibling())
		{
			auto gameObject = CreateGameObject();
			gameObject->Deserialize(pChild);
		}
	}

	/**
	 * \brief ����
	 */
	void Scene::Update()
	{
	}

	/**
	 * \brief ���ٳ���
	 */
	void Scene::Destroy()
	{
		//���ٳ����еĶ���
		for (auto iter = m_GameObjects.begin(); iter != m_GameObjects.end(); ++iter)
		{
			(*iter)->Destroy();
		}
	}

	/**
	 * \brief ���û״̬
	 * \param active �Ƿ�ʱ�״̬
	 */
	void Scene::SetActive(bool active)
	{
		m_IsActive = active;
		if (m_IsActive)
		{
			//TODO
		}
		else
		{
			//TODO
		}
	}
}
