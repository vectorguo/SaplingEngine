#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	Texture2D::Texture2D(std::string path) :
		m_Path(path),
		m_SrvIndex(Dx12CBufferManager::SrvCount + 10000)
	{
	}
	
	Texture2D::~Texture2D()
	{
		if (m_SrvIndex > Dx12CBufferManager::SrvCount)
		{
			Dx12CBufferManager::PushSrvIndex(m_SrvIndex);
		}
	}
}