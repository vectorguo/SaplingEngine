#include "Application/Setting.h"
#include "Render/Graphics/DirectX12/Dx12BufferManager.h"
#include "Render/Graphics/Light.h"
#include "Render/Graphics/LightManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	const size_t ShadowCasterHashValue = StringToHash("ShadowCaster");

	ShadowPass::ShadowPass(const std::string& name) :
		RenderPass(name),
		m_ShadowMapWidth(2048),
		m_ShadowMapHeight(2048)
	{
		//创建阴影所需Dsv描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

		m_SrvIndex = BufferManager::PopSrvIndex();
		m_CpuDescriptor = BufferManager::GetSrvCpuDescriptor(m_SrvIndex);
		m_GpuDescriptor = BufferManager::GetSrvGpuDescriptor(m_SrvIndex);

		CreateDescriptors();
	}
	
	/**
	 * \brief 渲染
	 */
	void ShadowPass::Render()
	{
		if (!UpdateShadowTransform())
		{
			return;
		}

		//添加渲染命令
		auto* pCommandList = CommandManager::GetCommandList();

		//设置ViewPort和ScissorRect
		pCommandList->RSSetViewports(1, &m_Viewport);
		pCommandList->RSSetScissorRects(1, &m_ScissorRect);

		// Change to DEPTH_WRITE.
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_ShadowMap.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		auto dsv = m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		pCommandList->OMSetRenderTargets(0, nullptr, false, &dsv);

		//需要切换渲染管线状态
		pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(ShadowCasterHashValue));
		pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(ShadowCasterHashValue));
		pCommandList->SetGraphicsRootConstantBufferView(2, BufferManager::GetShadowPassCbAddress());

		//绘制阴影
		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			auto items = iter->second;
			for (auto iter2 = items.begin(); iter2 != items.end(); ++iter2)
			{
				CommandManager::DrawIndexedInstancedForShadowMap(*iter2);
			}
		}

		// Change back to GENERIC_READ so we can read the texture in a shader.
		resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_ShadowMap.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
		pCommandList->ResourceBarrier(1, &resourceBarrier);
	}

	void ShadowPass::OnSceneResize(uint32_t width, uint32_t height)
	{
		m_ShadowMapWidth = width;
		m_ShadowMapHeight = height;

		CreateDescriptors();
	}

	void ShadowPass::CreateDescriptors()
	{
		auto* pDevice = GraphicsManager::GetDx12Device();

		D3D12_RESOURCE_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texDesc.Alignment = 0;
		texDesc.Width = m_ShadowMapWidth;
		texDesc.Height = m_ShadowMapHeight;
		texDesc.DepthOrArraySize = 1;
		texDesc.MipLevels = 1;
		texDesc.Format = m_Format;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		CD3DX12_HEAP_PROPERTIES properties(D3D12_HEAP_TYPE_DEFAULT);
		ThrowIfFailed(pDevice->CreateCommittedResource(
			&properties,
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			&optClear,
			IID_PPV_ARGS(&m_ShadowMap)));

		// Create SRV to resource so we can sample the shadow map in a shader program.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		srvDesc.Texture2D.PlaneSlice = 0;
		pDevice->CreateShaderResourceView(m_ShadowMap.Get(), &srvDesc, m_CpuDescriptor);

		// Create DSV to resource so we can render to the shadow map.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Texture2D.MipSlice = 0;
		pDevice->CreateDepthStencilView(m_ShadowMap.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
	
	/**
	 * \brief	更新位置信息
	 */
	bool ShadowPass::UpdateShadowTransform()
	{
		//获取主光源（平行光）
		const auto* pMainLight = LightManager::GetDirectionalLight();
		if (pMainLight == nullptr)
		{
			return false;
		}
		const auto shadowDistance = pMainLight->GetShadowDistance();

		//获取包围盒
		const auto* pActiveScene = SceneManager::GetActiveScene();
		const auto& sceneBounds = pActiveScene->GetSceneBounds();

		//计算光源的View矩阵
		auto targetPosition = Vector3(sceneBounds.Center);
		auto lightPosition = targetPosition - pMainLight->GetLightDirection() * shadowDistance;
		auto worldToLightViewMatrix = Matrix4x4::LookAt(lightPosition, targetPosition, Vector3::Up);

		//计算光源的Proj矩阵
		auto targetPositionInLightSpace = worldToLightViewMatrix.MultiplyPoint(targetPosition);
		auto lightViewToProjMatrix = Matrix4x4::Orthographic(
			targetPositionInLightSpace.x - shadowDistance,
			targetPositionInLightSpace.x + shadowDistance,
			targetPositionInLightSpace.y - shadowDistance,
			targetPositionInLightSpace.y + shadowDistance,
			targetPositionInLightSpace.z - shadowDistance,
			targetPositionInLightSpace.z + shadowDistance);

		static Matrix4x4 t(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f);

		m_WorldToShadowMatrix = worldToLightViewMatrix * lightViewToProjMatrix * t;

		//填充阴影Pass缓冲区数据
		BufferManager::FillShadowPcbData(CommonPcbData::FillShadowPcbData(worldToLightViewMatrix, lightViewToProjMatrix), CommonPcbData::ShadowDataSize);
		return true;
	}
}