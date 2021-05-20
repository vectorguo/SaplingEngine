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
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentRt(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		//清理颜色缓冲和深度缓冲
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//获取阴影Pass
		auto* pShadowPass = RenderPipeline::GetShadowPass();

		//获取所有渲染目标
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			const auto shaderHashValue = iter->first;

			//需要切换渲染管线状态
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderHashValue));

			//设置跟描述符表和常量缓冲区，将常量缓冲区绑定到渲染流水线上
			pCommandList->SetDescriptorHeaps(1, BufferManager::GetSrvDescriptorHeaps());
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(shaderHashValue));
			pCommandList->SetGraphicsRootConstantBufferView(2, BufferManager::GetPassCbAddress());
			pCommandList->SetGraphicsRootDescriptorTable(3, pShadowPass->GetGpuDescriptor());

			//绘制物体
			auto items = iter->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				CommandManager::DrawIndexedInstanced(*iter2);
			}
		}

		//渲染缓存从RT状态切换到呈现状态
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentRt(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		pCommandList->ResourceBarrier(1, &resourceBarrier);
	}
}
