#pragma once

#include "Render/RenderPipeline/RenderPass/RenderPass.h"

namespace SaplingEngine
{
	class ShadowPass final : public RenderPass
	{
	public:
		explicit ShadowPass(const std::string& name);

		/**
		 * \brief	渲染
		 * \param	pCamera		渲染使用的相机
		 */
		void Render(const Camera* pCamera) override;

		/**
		 * \brief	设置宽度和高度
		 * \param	width		屏幕宽度
		 * \param	height		屏幕高度
		 */
		void OnSceneResize(uint32_t width, uint32_t height) override;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptor() const
		{
			return m_CpuDescriptor;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptor() const
		{
			return m_GpuDescriptor;
		}

		/**
		 * \brief	获取世界空间到阴影空间的坐标变换矩阵
		 */
		const Matrix4x4& GetWorldToShadowMatrix() const
		{
			return m_WorldToShadowMatrix;
		}

	private:
		void CreateDescriptors();

		/**
		 * \brief	更新位置信息
		 */
		bool UpdateShadowTransform();

	private:
		/**
		 * \brief	阴影贴图格式
		 */
		DXGI_FORMAT m_Format = DXGI_FORMAT_R24G8_TYPELESS;

		/**
		 * \brief	阴影贴图宽度
		 */
		uint32_t m_ShadowMapWidth;

		/**
		 * \brief	阴影贴图高度
		 */
		uint32_t m_ShadowMapHeight;

		/**
		 * \brief	视图窗口
		 */
		D3D12_VIEWPORT m_Viewport{ 0, 0, static_cast<float>(m_ShadowMapWidth), static_cast<float>(m_ShadowMapHeight), 0, 1 };

		/**
		 * \brief	裁剪矩形
		 */
		D3D12_RECT m_ScissorRect{ 0, 0, (int64_t)m_ShadowMapWidth, (int64_t)m_ShadowMapHeight };

		ComPtr<ID3D12Resource> m_ShadowMap = nullptr;

		/**
		 * \brief	DSV描述符堆
		 */
		ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;

	public:
		/**
		 * \brief	阴影变换矩阵
		 */
		Matrix4x4 m_WorldToShadowMatrix;

		uint32_t m_SrvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE m_CpuDescriptor;
		D3D12_GPU_DESCRIPTOR_HANDLE m_GpuDescriptor;
	};
} 