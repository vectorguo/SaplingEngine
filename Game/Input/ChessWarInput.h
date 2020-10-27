#pragma once

#include "ChessWarEnum.h"
#include "GameObject/Component.h"

namespace ChessWar
{
	class ChessWarInput final : public SaplingEngine::Component
	{
	public:
		ChessWarInput() = default;
		~ChessWarInput() override = default;

		/*
		 * ��ֹ�������ƶ�
		 */
		ChessWarInput(const ChessWarInput&) = delete;
		ChessWarInput(ChessWarInput&&) = delete;
		ChessWarInput& operator= (const ChessWarInput&) = delete;
		ChessWarInput& operator= (ChessWarInput&&) = delete;
		
		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_ChessWarInput;
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
		SaplingEngine::CameraPtr m_MainCamera;
		
		float m_Theta = 1.5f * XM_PI;
		float m_Phi = XM_PIDIV4;
		float m_Radius = 10.0f;
	};
}
