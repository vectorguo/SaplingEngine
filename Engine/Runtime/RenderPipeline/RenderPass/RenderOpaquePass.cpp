#include "RenderOpaquePass.h"


#include "Graphics/Material.h"
#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/Renderer/Renderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief 渲染
	 * \param pCamera 渲染使用的相机
	 * \param pActiveScene 被渲染场景
	 */
	void RenderOpaquePass::Render(const Camera* pCamera, Scene* pActiveScene)
	{
		//获取所有渲染目标
		auto& renderItems = pActiveScene->GetRenderItems();
		
		//根据Material使用的Shader对RenderItem进行排序
		std::sort(renderItems.begin(), renderItems.end(), [](const Renderer* pr1, const Renderer* pr2)
			{
				return pr1->GetMaterial()->GetShaderName() < pr2->GetMaterial()->GetShaderName();
			});

		//添加渲染命令
		auto* pCommandManager = CommandManager::Instance();
		pCommandManager->PreDraw(true, true, m_BackgroundColor);
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			pCommandManager->DrawIndexedInstanced(*iter);
		}
		pCommandManager->PostDraw();
	}
}
