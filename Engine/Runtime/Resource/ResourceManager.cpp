#include "Runtime/Resource/ResourceManager.h"
#include "Runtime/Resource/ResourceLoader.h"

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