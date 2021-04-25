#include "RenderOpaquePass.h"

#include "Render/Graphics/Material.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderPipeline/RenderPipeline.h"
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
		auto* pCommandList = CommandManager::GetCommandList();

		//清理颜色缓冲和深度缓冲
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
		pCommandList->SetDescriptorHeaps(CBufferManager::GetCbvDescriptorHeapCount(), CBufferManager::GetCbvDescriptorHeaps());
		pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature());
		pCommandList->SetGraphicsRootDescriptorTable(2, CBufferManager::GetPassCbvDescriptor());

		//绘制物体
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			CommandManager::DrawIndexedInstanced(*iter);
		}
	}
}
