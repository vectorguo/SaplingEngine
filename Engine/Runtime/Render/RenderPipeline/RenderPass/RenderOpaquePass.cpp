#include "Render/Graphics/Material.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Render/RenderPipeline/RenderPass/RenderOpaquePass.h"
#include "Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Ⱦ
	 */
	void RenderOpaquePass::Render()
	{
		auto* pCommandList = CommandManager::GetCommandList();

		//����ViewPort��ScissorRect
		pCommandList->RSSetViewports(1, &GraphicsManager::GetViewport());
		pCommandList->RSSetScissorRects(1, &GraphicsManager::GetScissorRect());

		//��Ⱦ����ӳ���״̬�л���RT״̬
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentSwapChainBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		//������ɫ�������Ȼ���
		const auto rtv = DescriptorManager::GetRenderTargetView();
		const auto dsv = DescriptorManager::GetDepthStencilView();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//��ȡ��ӰPass
		auto* pShadowPass = RenderPipeline::GetShadowPass();

		//��ȡ������ȾĿ��
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter1 = renderItems.begin(); iter1 != renderItems.end(); ++iter1)
		{
			const auto shaderHashValue = iter1->first;

			//��Ҫ�л���Ⱦ����״̬
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderHashValue));

			//���ø���������ͳ������������������������󶨵���Ⱦ��ˮ����
			pCommandList->SetDescriptorHeaps(1, DescriptorManager::GetSrvDescriptorHeaps());
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(shaderHashValue));
			pCommandList->SetGraphicsRootConstantBufferView(2, Dx12DescriptorManager::GetPassCbAddress());
			pCommandList->SetGraphicsRootDescriptorTable(3, pShadowPass->GetGpuDescriptor());

			//��������
			for (auto iter2 = iter1->second.elements.begin(); iter2 != iter1->second.elements.end(); ++iter2)
			{
				for (auto iter3 = iter2->Renderers.begin(); iter3 != iter2->Renderers.end(); ++iter3)
				{
					CommandManager::DrawIndexedInstanced(*iter3);
				}
			}
		}

		//��Ⱦ�����RT״̬�л�������״̬
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentSwapChainBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		pCommandList->ResourceBarrier(1, &resourceBarrier);
	}
}
