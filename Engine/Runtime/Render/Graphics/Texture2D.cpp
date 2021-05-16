#include "Render/Graphics/DirectX12/Dx12BufferManager.h"
#include "Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	Texture2D::Texture2D(std::string path) :
		m_Path(path),
		m_SrvIndex(BufferManager::SrvCount + 10000)
	{
	}
	
	Texture2D::~Texture2D()
	{
		if (m_SrvIndex > BufferManager::SrvCount)
		{
			BufferManager::PushSrvIndex(m_SrvIndex);
		}
	}
}