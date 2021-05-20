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
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentRt(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		//������ɫ�������Ȼ���
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//��ȡ��ӰPass
		auto* pShadowPass = RenderPipeline::GetShadowPass();

		//��ȡ������ȾĿ��
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			const auto shaderHashValue = iter->first;

			//��Ҫ�л���Ⱦ����״̬
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderHashValue));

			//���ø���������ͳ������������������������󶨵���Ⱦ��ˮ����
			pCommandList->SetDescriptorHeaps(1, BufferManager::GetSrvDescriptorHeaps());
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(shaderHashValue));
			pCommandList->SetGraphicsRootConstantBufferView(2, BufferManager::GetPassCbAddress());
			pCommandList->SetGraphicsRootDescriptorTable(3, pShadowPass->GetGpuDescriptor());

			//��������
			auto items = iter->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				CommandManager::DrawIndexedInstanced(*iter2);
			}
		}

		//��Ⱦ�����RT״̬�л�������״̬
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(GraphicsManager::GetCurrentRt(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		pCommandList->ResourceBarrier(1, &resourceBarrier);
	}
}
