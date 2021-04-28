#pragma once

#include "SaplingEnginePch.h"
#include "Shader.h"

namespace SaplingEngine
{
	class Material final
	{
	public:
		Material();
		~Material();

		Material(const Material&) = delete;
		Material(Material&&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

		/**
		 * \brief ��ȡ����ʹ�õ�Shader������
		 * \return Shader����
		 */
		const std::string& GetShaderName() const
		{
			return m_pShader->GetName();
		}

		/**
		 * \brief ��ȡ����ʹ�õ�Shader��HashValue
		 * \return Shader��HashValue
		 */
		const size_t GetShaderHashValue() const
		{
			return m_pShader->GetHashValue();
		}
		
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode);
		
	private:
		const Shader* m_pShader = nullptr;
	};
}