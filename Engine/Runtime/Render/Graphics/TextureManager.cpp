#include "Render/Graphics/DirectX12/DDSTextureLoader.h"
#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/DirectX12/Dx12CommandManager.h"
#include "Render/Graphics/DirectX12/Dx12GraphicsManager.h"
#include "Render/Graphics/Texture2D.h"
#include "Render/Graphics/TextureManager.h"
#include "Resource/ResourceManager.h"

namespace SaplingEngine
{
	Texture2DSptr					TextureManager::White;
	std::map<size_t, Texture2DSptr> TextureManager::texture2Ds;
	std::vector<Texture2D*>			TextureManager::unreadyTexture2Ds;

	void TextureManager::Initialize()
	{
		White = CreateTexture2D("Resources/Internal/Textures/white.dds");
	}

	/**
	 * \brief	����Texture2D
	 * \param	path		Texture2D����Դ·��
	 * \return	ָ�򴴽��õ�Texture2D������ָ��
	 */
	Texture2DSptr TextureManager::CreateTexture2D(const std::string& path)
	{
		const auto hashValue = StringToHash(path);
		auto iter = texture2Ds.find(hashValue);
		if (iter == texture2Ds.end())
		{
			auto texture2DSptr = ResourceManager::Load<Texture2D>(path);
			texture2Ds.emplace(hashValue, texture2DSptr);
			unreadyTexture2Ds.emplace_back(texture2DSptr.Get());
			return texture2DSptr;
		}
		else
		{
			return iter->second;
		}
	}

	/**
	 * \brief	ɾ��Texture2D
	 * \param	textureSptr	Ҫ��ɾ����Texture2D������ָ�������
	 */
	void TextureManager::DestroyTexture2D(Texture2DSptr&& textureSptr)
	{
		if (!textureSptr->IsReady())
		{
			unreadyTexture2Ds.erase(std::find(unreadyTexture2Ds.begin(), unreadyTexture2Ds.end(), textureSptr.Get()));
		}
	}
	
	/**
	 * \brief	�ϴ�Mesh���ݵ�GPU
	 */
	void TextureManager::UploadTextureDatas()
	{
		if (!unreadyTexture2Ds.empty())
		{
			for (auto iter = unreadyTexture2Ds.begin(); iter != unreadyTexture2Ds.end(); ++iter)
			{
				auto* pTexture2D = *iter;
				pTexture2D->m_IsReady = true;

				auto wPath = CharToWChar(pTexture2D->m_Path.c_str());
				ThrowIfFailed(
					DirectX::CreateDDSTextureFromFile12(
						GraphicsManager::GetDx12Device(),
						CommandManager::GetCommandList(),
						wPath.c_str(),
						pTexture2D->m_ResourcePtr,
						pTexture2D->m_UploadHeap));

				CBufferManager::PopSrvIndex(pTexture2D);
			}
			unreadyTexture2Ds.clear();
		}
	}
}