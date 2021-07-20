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
	 * \brief	创建Texture2D
	 * \param	path		Texture2D的资源路径
	 * \return	指向创建好的Texture2D的智能指针
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
	 * \brief	删除Texture2D
	 * \param	textureSptr	要被删除的Texture2D的智能指针的引用
	 */
	void TextureManager::DestroyTexture2D(Texture2DSptr&& textureSptr)
	{
		if (!textureSptr->m_IsReady)
		{
			unreadyTexture2Ds.erase(std::find(unreadyTexture2Ds.begin(), unreadyTexture2Ds.end(), textureSptr.Get()));
		}
	}
	
	/**
	 * \brief	上传Mesh数据到GPU
	 */
	void TextureManager::UploadTextureDatas()
	{
		if (!unreadyTexture2Ds.empty())
		{
			for (auto iter = unreadyTexture2Ds.begin(); iter != unreadyTexture2Ds.end(); ++iter)
			{
				(*iter)->LoadTextureData();
			}
			unreadyTexture2Ds.clear();
		}
	}
}