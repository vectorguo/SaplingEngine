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
		 * \brief	ִ����Ⱦǰ��׼������
		 */
		void PreRender() override;

		/**
		 * \brief	��Ⱦ
		 */
		void Render() override;

		/**
		 * \brief	���ÿ�Ⱥ͸߶�
		 * \param	width		��Ļ���
		 * \param	height		��Ļ�߶�
		 */
		void OnSceneResize(uint32_t width, uint32_t height) override;

		/**
		 * \brief	�Ƿ��ڻ״̬
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
		 * \brief	��ȡ����ռ䵽��Ӱ�ռ������任����
		 */
		const Matrix4x4& GetWorldToShadowMatrix() const
		{
			return m_WorldToShadowMatrix;
		}

	private:
		void CreateDescriptors();

	private:
		/**
		 * \brief	��Ӱ��ͼ��ʽ
		 */
		DXGI_FORMAT m_Format = DXGI_FORMAT_R24G8_TYPELESS;

		/**
		 * \brief	��Ӱ��ͼ��С
		 */
		uint32_t m_ShadowMapSize;

		/**
		 * \brief	��ͼ����
		 */
		D3D12_VIEWPORT m_Viewport{ 0, 0, static_cast<float>(m_ShadowMapSize), static_cast<float>(m_ShadowMapSize), 0, 1 };

		/**
		 * \brief	�ü�����
		 */
		D3D12_RECT m_ScissorRect{ 0, 0, (int64_t)m_ShadowMapSize, (int64_t)m_ShadowMapSize };

		/**
		 * \brief	��Ӱ��ͼ��Դ
		 */
		ComPtr<ID3D12Resource> m_ShadowMap = nullptr;

		/**
		 * \brief	DSV��������
		 */
		ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;

	public:
		/**
		 * \brief	��Ӱ�任����
		 */
		Matrix4x4 m_WorldToShadowMatrix;
		Matrix4x4 m_WorldToLightMatrix;
		Matrix4x4 m_LightToProjMatrix;

		uint32_t m_SrvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE m_CpuDescriptor;
		D3D12_GPU_DESCRIPTOR_HANDLE m_GpuDescriptor;
	};
} 