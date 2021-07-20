#include "Render/Graphics/Material.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Render/RenderPipeline/RenderPass/RenderOpaquePass.h"
#include "Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief 渲染
	 */
	void RenderOpaquePass::Render()
	{
		auto* pCommandList = CommandManager::GetCommandList();

		//设置ViewPort和ScissorRect
		pCommandList->RSSetViewports(1, &GraphicsManager::GetViewport());
		pCommandList->RSSetScissorRects(1, &GraphicsManager::GetScissorRect());

		//渲染缓存从呈现状态切换到RT状态
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentSwapChainBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		//清理颜色缓冲和深度缓冲
		const auto rtv = DescriptorManager::GetRenderTargetView();
		const auto dsv = DescriptorManager::GetDepthStencilView();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//获取阴影Pass
		auto* pShadowPass = RenderPipeline::GetShadowPass();

		//获取所有渲染目标
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter1 = renderItems.begin(); iter1 != renderItems.end(); ++iter1)
		{
			const auto shaderHashValue = iter1->first;

			//需要切换渲染管线状态
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderHashValue));

			//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
			pCommandList->SetDescriptorHeaps(1, DescriptorManager::GetSrvDescriptorHeaps());
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(shaderHashValue));
			pCommandList->SetGraphicsRootConstantBufferView(2, Dx12DescriptorManager::GetPassCbAddress());
			pCommandList->SetGraphicsRootDescriptorTable(3, pShadowPass->GetGpuDescriptor());

			//绘制物体
			for (auto iter2 = iter1->second.elements.begin(); iter2 != iter1->second.elements.end(); ++iter2)
			{
				for (auto iter3 = iter2->Renderers.begin(); iter3 != iter2->Renderers.end(); ++iter3)
				{
					CommandManager::DrawIndexedInstanced(*iter3);
				}
			}
		}

		//渲染缓存从RT状态切换到呈现状态
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentSwapChainBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		pCommandList->ResourceBarrier(1, &resourceBarrier);
	}
}
