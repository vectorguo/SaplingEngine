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
	 */
	void RenderOpaquePass::Render(const Camera* pCamera)
	{
		//添加渲染命令
		auto* pCommandList = CommandManager::GetCommandList();

		//清理颜色缓冲和深度缓冲
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//获取所有渲染目标
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			const auto shaderName = iter->first;

			//需要切换渲染管线状态
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderName));

			//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
			pCommandList->SetDescriptorHeaps(1, CBufferManager::GetCbvDescriptorHeaps(shaderName));
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature());
			pCommandList->SetGraphicsRootDescriptorTable(2, CBufferManager::GetPassCbvDescriptor(shaderName));

			//绘制物体
			auto items = iter->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				CommandManager::DrawIndexedInstanced(*iter2);
			}
		}
	}
}
