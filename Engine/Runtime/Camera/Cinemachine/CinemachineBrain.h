#pragma once

#include "Camera/Camera.h"
#include "GameObject/Component.h"

namespace SaplingEngine
{
	/**
	 * \brief	ǰ������
	 */
	class CinemachineVirtualCamera;

	class CinemachineBrain final : public Component
	{
	public:
		CinemachineBrain();
		~CinemachineBrain() override = default;

		/**
		 * \brief	��ֹ�������ƶ�
		 */
		CinemachineBrain(const CinemachineBrain&) = delete;
		CinemachineBrain(CinemachineBrain&&) = delete;
		CinemachineBrain& operator= (const CinemachineBrain&) = delete;
		CinemachineBrain& operator= (CinemachineBrain&&) = delete;

		/**
		 * \brief	��ȡ������͡�
		 *			ÿ����������붨��˺�����
		 *			GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component��
		 */
		static constexpr uint32_t GetStaticComponentType()
		{
			return ComponentType_CinemachineBrain;
		}

		/**
		 * \brief	����������
		 * \param	pVirtualCamera	�������ָ��
		 */
		static void AddVirtualCamera(CinemachineVirtualCamera* pVirtualCamera);

		/**
		 * \brief	ɾ���������
		 * \param	pVirtualCamera	�������ָ��
		 */
		static void RemoveVirtualCamera(CinemachineVirtualCamera* pVirtualCamera);

		/**
		 * \brief	�����������
		 */
		static void SortVirtualCameras();

	protected:
		/**
		 * \brief	Start
		 */
		void Start() override;

		/**
		 * \brief	����
		 */
		void Update() override;

	private:
		/**
		 * \brief	����
		 */
		static CinemachineBrain* pInstance;

		/**
		 * \brief	��������б�
		 */
		static std::vector<CinemachineVirtualCamera*> virtualCameras;

		/**
		 * \brief	���
		 */
		CameraSptr m_CameraSptr;

		/**
		 * \brief	��ǰ�������
		 */
		CinemachineVirtualCamera* m_pCurrentVirtualCamera = nullptr;
	};
}