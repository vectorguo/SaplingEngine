#include "Resource/ResourceManager.h"
#include "Resource/ResourceLoader.h"

namespace SaplingEngine
{
	std::vector<ResourceAsyncRequestSptr> ResourceManager::asyncRequests;

	/**
	 * \brief ��ʼ��
	 * \return ��ʼ���Ƿ�ɹ�
	 */
	bool ResourceManager::Initialize()
	{
		return true;
	}
	
	/**
	 * \brief	����
	 */
	void ResourceManager::Update()
	{
	}
}