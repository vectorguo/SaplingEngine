#pragma once

#include "GameObject/Component.h"

namespace SaplingEngine
{
	class Light final : public Component
	{
	public:
		Light();
		~Light() override;

		/*
		 * ��ֹ�������ƶ�
		 */
		Light(const Light&) = delete;
		Light(Light&&) = delete;
		Light& operator= (const Light&) = delete;
		Light& operator= (Light&&) = delete;

		/*
		 * ��ȡ�������
		 * ÿ����������붨��˺���
		 * GameObject����ÿ��ComponentTypeֻ��ӵ��һ��Component
		 */
		static constexpr uint32_t GetComponentType()
		{
			return ComponentType_Light;
		}

		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode) override;

	public:
		/**
		 * \brief ��ȡ����
		 * \return ����ָ��
		 */
		static Light* Instance()
		{
			return m_pInstance;
		}

	private:
		/**
		 * \brief ����
		 */
		static Light* m_pInstance;
	};
}