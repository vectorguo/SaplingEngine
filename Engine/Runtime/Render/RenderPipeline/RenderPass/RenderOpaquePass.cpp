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
		auto* pCommandList = CommandManager::GetCommandList();

		//������ɫ�������Ȼ���
		const auto rtv = GraphicsManager::GetCurrentRtv();
		const auto dsv = GraphicsManager::GetCurrentDsv();
		pCommandList->ClearRenderTargetView(rtv, Color::LightBlue, 0, nullptr);
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(1, &rtv, true, &dsv);

		//���ø���������ͳ������������������������󶨵���Ⱦ��ˮ����
		pCommandList->SetDescriptorHeaps(CBufferManager::GetCbvDescriptorHeapCount(), CBufferManager::GetCbvDescriptorHeaps());
		pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature());
		pCommandList->SetGraphicsRootDescriptorTable(2, CBufferManager::GetPassCbvDescriptor());

		//��������
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			CommandManager::DrawIndexedInstanced(*iter);
		}
	}
}
