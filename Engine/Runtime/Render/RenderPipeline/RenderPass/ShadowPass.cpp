#include "Application/GameSetting.h"
#include "Render/Graphics/DirectX12/Dx12CBufferManager.h"
#include "Render/Graphics/Light.h"
#include "Render/Graphics/LightManager.h"
#include "Render/RenderPipeline/RenderPipeline.h"
#include "Render/RenderPipeline/RenderPass/ShadowPass.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

namespace SaplingEngine
{
	const size_t ShadowCasterHashValue = StringToHash("ShadowCaster");

	Matrix4x4 ShadowPass::m_ShadowTransform = Matrix4x4::Identity;

	uint32_t ShadowPass::m_SrvIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE ShadowPass::m_CpuDescriptorHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE ShadowPass::m_GpuDescriptorHandle;

	ShadowPass::ShadowPass(const std::string& name) :
		RenderPass(name),
		m_ShadowMapWidth(GameSetting::ScreenWidth()),
		m_ShadowMapHeight(GameSetting::ScreenHeight())
	{
		//������Ӱ����Dsv��������
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDesc.NodeMask = 0;
		ThrowIfFailed(GraphicsManager::GetDx12Device()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));

		m_SrvIndex = Dx12CBufferManager::PopSrvIndex(m_CpuDescriptorHandle, m_GpuDescriptorHandle);

		CreateDescriptors();
	}
	
	/**
	 * \brief ��Ⱦ
	 * \param pCamera ��Ⱦʹ�õ����
	 */
	void ShadowPass::Render(const Camera* pCamera)
	{
		if (!UpdateShadowTransform())
		{
			return;
		}

		auto& renderItems = RenderPipeline::GetRenderItems();
		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			const auto& shaderHashValue = iter->first;
			Dx12CBufferManager::FillShadowPcbData(shaderHashValue, CommonPcbData::FillShadowPcbData(m_WorldToLightViewMatrix, m_LightViewToProjMatrix), CommonPcbData::DataSize);
		}

		//�����Ⱦ����
		auto* pCommandList = CommandManager::GetCommandList();

		//����ViewPort��ScissorRect
		pCommandList->RSSetViewports(1, &m_Viewport);
		pCommandList->RSSetScissorRects(1, &m_ScissorRect);

		// Change to DEPTH_WRITE.
		auto resourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_ShadowMap.Get(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
		pCommandList->ResourceBarrier(1, &resourceBarrier);

		auto dsv = m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

		// Clear the back buffer and depth buffer.
		pCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		// Set null render target because we are only going to draw to
		// depth buffer.  Setting a null render target will disable color writes.
		// Note the active PSO also must specify a render target count of 0.
		pCommandList->OMSetRenderTargets(0, nullptr, false, &dsv);

		//��Ҫ�л���Ⱦ����״̬
		pCommandList->SetPipelineState(GraphicsManager::GetPipelineState(ShadowCasterHashValue));

		pCommandList->SetGraphicsRootSignature(GraphicsManager::GetRootSignature(ShadowCasterHashValue));

		for (auto iter = renderItems.begin(); iter != renderItems.end(); ++iter)
		{
			//���ø���������ͳ������������������������󶨵���Ⱦ��ˮ����
			pCommandList->SetGraphicsRootConstantBufferView(2, CBufferManager::GetShadowPassCbAddress(iter->first));

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
		pDevice->CreateShaderResourceView(m_ShadowMap.Get(), &srvDesc, m_CpuDescriptorHandle);

		// Create DSV to resource so we can render to the shadow map.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.Texture2D.MipSlice = 0;
		pDevice->CreateDepthStencilView(m_ShadowMap.Get(), &dsvDesc, m_DsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
	
	/**
	 * \brief	����λ����Ϣ
	 */
	bool ShadowPass::UpdateShadowTransform()
	{
		//��ȡ����Դ��ƽ�й⣩
		const auto* pMainLight = LightManager::GetDirectionalLight();
		if (pMainLight == nullptr)
		{
			return false;
		}

		//��ȡ��Χ��
		const auto* pCamera = CameraManager::GetMainCamera();
		const auto* pActiveScene = SceneManager::GetActiveScene();
		const auto& sceneBounds = pActiveScene->GetSceneBounds();

		//���¹�Դ��View��ProjͶӰ����
		XMVECTOR lightPos = XMVectorSet(0, 30, -120, 0);
		XMVECTOR targetPos = XMVectorSet(0, 0, -120, 0);
		XMVECTOR lightUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX lightView = XMMatrixLookAtLH(lightPos, targetPos, lightUp);

		// Transform bounding sphere to light space.
		XMFLOAT3 sphereCenterLS;
		XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, lightView));

		// Ortho frustum in light space encloses scene.
		float l = sphereCenterLS.x - sceneBounds.Radius;
		float b = sphereCenterLS.y - sceneBounds.Radius;
		float n = sphereCenterLS.z - sceneBounds.Radius;
		float r = sphereCenterLS.x + sceneBounds.Radius;
		float t = sphereCenterLS.y + sceneBounds.Radius;
		float f = sphereCenterLS.z + sceneBounds.Radius;

		XMMATRIX lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

		XMMATRIX T(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f);

		m_WorldToLightViewMatrix = lightView;
		m_LightViewToProjMatrix = lightProj;
		m_ShadowTransform = lightView * lightProj * T;

		auto data = m_ShadowTransform.MultiplyPoint(Vector3(20, 10, -120, 1));
		return true;
	}
}