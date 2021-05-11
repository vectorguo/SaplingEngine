#include "ResourceManager.h"
#include "ResourceLoader.h"

namespace SaplingEngine
{
	std::vector<ResourceAsyncRequestSptr> ResourceManager::asyncRequests;

	/**
	 * \brief 初始化
	 * \return 初始化是否成功
	 */
	bool ResourceManager::Initialize()
	{
		return true;
	}
	
	/**
	 * \brief	更新
	 */
	void ResourceManager::Update()
	{
	}
}