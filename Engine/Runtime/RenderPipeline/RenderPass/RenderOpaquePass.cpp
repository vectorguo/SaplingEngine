#include "RenderOpaquePass.h"


#include "Graphics/Material.h"
#include "RenderPipeline/CommandManager.h"
#include "RenderPipeline/Renderer/Renderer.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Ⱦ
	 * \param pCamera ��Ⱦʹ�õ����
	 * \param pActiveScene ����Ⱦ����
	 */
	void RenderOpaquePass::Render(const Camera* pCamera, Scene* pActiveScene)
	{
		//��ȡ������ȾĿ��
		auto& renderItems = pActiveScene->GetRenderItems();
		
		//����Materialʹ�õ�Shader��RenderItem��������
		std::sort(renderItems.begin(), renderItems.end(), [](const Renderer* pr1, const Renderer* pr2)
			{
				return pr1->GetMaterial()->GetShaderName() < pr2->GetMaterial()->GetShaderName();
			});

		//�����Ⱦ����
		auto* pCommandManager = CommandManager::Instance();
		pCommandManager->PreDraw(true, true, m_BackgroundColor);
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			pCommandManager->DrawIndexedInstanced(*iter);
		}
		pCommandManager->PostDraw();
	}
}
