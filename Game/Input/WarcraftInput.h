#pragma once

#include "SaplingEngineHeader.h"
#include "WarcraftEnum.h"

namespace Warcraft
{
	class WarcraftInput final : public SaplingEngine::Component
	{
	public:
		WarcraftInput() = default;
		~WarcraftInput() override = default;

		/*
		 * ��ֹ�������ƶ�
		 */
		WarcraftInput(const WarcraftInput&) = delete;
		WarcraftInput(WarcraftInput&&) = delete;
		WarcraftInput& operator= (const WarcraftInput&) = delete;
		WarcraftInput& operator= (WarcraftInput&&) = delete;
		
		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_WarcraftInput;
		}
		
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

	protected:
		/**
		 * \brief Start
		 */
		void Start() override;

		/**
		 * \brief ����
		 */
		void Update() override;
		
		/**
		 * \brief �������
		 */
		void UpdateCamera();

	private:
		/**
		 * \brief ���
		 */
		SaplingEngine::CameraSptr m_MainCamera;

		/**
		 * \brief ���λ��
		 */
		Vector2 m_MousePosition;

		/**
		 * \brief �������ֵ 0���޲����� 1���ƶ���� 2����ת��� 3��������Զ
		 */
		int32_t m_CameraOperation = 0;
	};
}
