#pragma once

#include "Runtime/Render/RenderPipeline/RenderPass/RenderPass.h"

namespace SaplingEngine
{
	class ShadowPass final : public RenderPass
	{
		friend class RenderPipeline;
	public:
		explicit ShadowPass(const std::string& name);

		/**
		 * \brief	执行渲染前的准备工作
		 */
		void PreRender() override;

		/**
		 * \brief	渲染
		 */
		void Render() override;

		/**
		 * \brief	设置宽度和高度
		 * \param	width		屏幕宽度
		 * \param	height		屏幕高度
		 */
		void OnSceneResize(uint32_t width, uint32_t height) override;

		/**
		 * \brief	是否处于活动状态
		 */
		bool IsActive() const override;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptor() const
		{
			return m_CpuDescriptor;
		}

		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptor() const
		{
			return m_GpuDescriptor;
		}

		const Matrix4x4& GetWorldToLightMatrix() const
		{
			return m_WorldToLightMatrix;
		}

		const Matrix4x4& GetLightToProjMatrix() const
		{
			return m_LightToProjMatrix;
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

	private:
		/**
		 * \brief	阴影贴图格式
		 */
		DXGI_FORMAT m_Format = DXGI_FORMAT_R24G8_TYPELESS;

		/**
		 * \brief	阴影贴图大小
		 */
		uint32_t m_ShadowMapSize;

		/**
		 * \brief	视图窗口
		 */
		D3D12_VIEWPORT m_Viewport{ 0, 0, static_cast<float>(m_ShadowMapSize), static_cast<float>(m_ShadowMapSize), 0, 1 };

		/**
		 * \brief	裁剪矩形
		 */
		D3D12_RECT m_ScissorRect{ 0, 0, (int64_t)m_ShadowMapSize, (int64_t)m_ShadowMapSize };

		/**
		 * \brief	阴影贴图资源
		 */
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
		Matrix4x4 m_WorldToLightMatrix;
		Matrix4x4 m_LightToProjMatrix;

		uint32_t m_SrvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE m_CpuDescriptor;
		D3D12_GPU_DESCRIPTOR_HANDLE m_GpuDescriptor;
	};
} 