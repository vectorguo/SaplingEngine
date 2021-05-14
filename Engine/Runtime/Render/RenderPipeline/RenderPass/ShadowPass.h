#pragma once

#include "Render/RenderPipeline/RenderPass/RenderPass.h"

namespace SaplingEngine
{
	class ShadowPass final : public RenderPass
	{
	public:
		explicit ShadowPass(const std::string& name);

		/**
		 * \brief	��Ⱦ
		 * \param	pCamera		��Ⱦʹ�õ����
		 */
		void Render(const Camera* pCamera) override;

		/**
		 * \brief	���ÿ�Ⱥ͸߶�
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 */
		void OnSceneResize(uint32_t width, uint32_t height) override;

	private:
		void CreateDescriptors();

		/**
		 * \brief	����λ����Ϣ
		 */
		bool UpdateShadowTransform();

	private:
		/**
		 * \brief	��Ӱ��ͼ��ʽ
		 */
		DXGI_FORMAT m_Format = DXGI_FORMAT_R24G8_TYPELESS;

		/**
		 * \brief	��Ӱ��ͼ���
		 */
		uint32_t m_ShadowMapWidth;

		/**
		 * \brief	��Ӱ��ͼ�߶�
		 */
		uint32_t m_ShadowMapHeight;

		/**
		 * \brief	��ͼ����
		 */
		D3D12_VIEWPORT m_Viewport{ 0, 0, static_cast<float>(m_ShadowMapWidth), static_cast<float>(m_ShadowMapHeight), 0, 1 };

		/**
		 * \brief	�ü�����
		 */
		D3D12_RECT m_ScissorRect{ 0, 0, (int64_t)m_ShadowMapWidth, (int64_t)m_ShadowMapHeight };

		/**
		 * \brief	�������굽��Դ�ռ�ı任����
		 */
		Matrix4x4 m_WorldToLightViewMatrix = Matrix4x4::Identity;

		/**
		 * \brief	��Դ�ռ䵽ͶӰ�ռ�ı任����
		 */
		Matrix4x4 m_LightViewToProjMatrix = Matrix4x4::Identity;

		ComPtr<ID3D12Resource> m_ShadowMap = nullptr;

		/**
		 * \brief	DSV��������
		 */
		ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;

	public:
		/**
		 * \brief	��Ӱ�任����
		 */
		static Matrix4x4 m_ShadowTransform;

		static uint32_t m_SrvIndex;
		static D3D12_CPU_DESCRIPTOR_HANDLE m_CpuDescriptorHandle;
		static D3D12_GPU_DESCRIPTOR_HANDLE m_GpuDescriptorHandle;
	};
} 