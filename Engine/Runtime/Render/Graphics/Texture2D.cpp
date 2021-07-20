#include "Render/Graphics/DirectX12/DDSTextureLoader.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12DescriptorManager.h"
#include "Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	Texture2D::Texture2D(std::string path) : m_Path(path)
	{
	}
	
	Texture2D::~Texture2D()
	{
		if (m_IsReady)
		{
			DescriptorManager::ReturnSrvIndex(m_SrvIndex);
		}
	}
	
	/**
	 * \brief	加载贴图数据
	 */
	void Texture2D::LoadTextureData()
	{
		m_IsReady = true;

		auto wPath = CharToWChar(m_Path.c_str());
		ThrowIfFailed(
			DirectX::CreateDDSTextureFromFile12(
				GraphicsManager::GetDx12Device(),
				CommandManager::GetCommandList(),
				wPath.c_str(),
				m_ResourcePtr,
				m_UploadHeap));

		//申请Srv索引
		m_SrvIndex = DescriptorManager::GetSrvIndex();

		//创建描述符
		DescriptorManager::CreateShaderResourceView(
			m_ResourcePtr.Get(),
			m_SrvIndex,
			D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING,
			m_ResourcePtr->GetDesc().Format,
			D3D12_SRV_DIMENSION_TEXTURE2D);
		m_SrvDescriptor = DescriptorManager::GetSrvGpuDescriptor(m_SrvIndex);
	}
}