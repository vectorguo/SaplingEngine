#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	class Material;
	class Mesh;
	
	class CommandManager
	{
	public:
		CommandManager();
		virtual ~CommandManager() = default;

		CommandManager(const CommandManager&) = delete;
		CommandManager(CommandManager&&) = delete;
		CommandManager& operator=(const CommandManager&) = delete;
		CommandManager& operator=(CommandManager&&) = delete;

		/**
		 * \brief ��ȡ����
		 * \return ����
		 */
		static CommandManager* Instance()
		{
			return m_Instance;
		}

		/**
		 * \brief ��ʼ��ʼ��
		 */
		virtual void BeginInitialize() {}

		/**
		 * \brief ������ʼ��
		 */
		virtual void EndInitialize() {}

		/**
		 * \brief ִ����Ⱦǰ��׼������
		 */
		virtual void PreRender() {}

		/**
		 * \brief ִ����Ⱦ���������
		 */
		virtual void PostRender() {}
		
		/**
		 * \brief ����
		 */
		virtual void Destroy() {}

		/**
		 * \brief ������
		 * \param clearColor �Ƿ�������ɫ����
		 * \param clearDepth �Ƿ�������Ȼ���
		 * \param color Ĭ����ɫ
		 */
		virtual void ClearRenderTargets(bool clearColor, bool clearDepth, const Color& color) = 0;

		/**
		 * \brief ���ø���������
		 */
		virtual void SetRootSignature() = 0;

		/**
		 * \brief ��������
		 * \param pMesh Mesh
		 * \param pMaterial Material
		 */
		virtual void DrawIndexedInstanced(const Mesh* pMesh, const Material* pMaterial) = 0;

	private:
		/**
		 * \brief ����
		 */
		static CommandManager* m_Instance;
	};
}