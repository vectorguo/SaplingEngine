#include "RenderOpaquePass.h"

#include "Render/Graphics/Material.h"
#include "Render/Renderer/Renderer.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Scene/Scene.h"

namespace SaplingEngine
{
	/**
	 * \brief ��Ⱦ
	 * \param pCamera ��Ⱦʹ�õ����
	 */
	void RenderOpaquePass::Render(const Camera* pCamera)
	{
		//�����Ⱦ����
		auto* pCommandList = CommandManager::GetCommandList();

		//������ɫ�������Ȼ���
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//��ȡ������ȾĿ��
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			const auto shaderName = iter->first;

			//��Ҫ�л���Ⱦ����״̬
			pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(shaderName));

			//���ø���������ͳ������������������������󶨵���Ⱦ��ˮ����
			pCommandList->SetDescriptorHeaps(1, CBufferManager::GetCbvDescriptorHeaps(shaderName));
			pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature());
			pCommandList->SetGraphicsRootDescriptorTable(2, CBufferManager::GetPassCbvDescriptor(shaderName));

			//��������
			auto items = iter->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				CommandManager::DrawIndexedInstanced(*iter2);
			}
		}
	}
}
