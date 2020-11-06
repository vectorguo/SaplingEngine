#pragma once

#include "SaplingEnginePch.h"

namespace SaplingEngine
{
	//ǰ������
	class Shader;
	
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
			return m_ShaderName;
		}
		
		/**
		 * \brief �����л�
		 * \param pNode ���ýڵ�ָ��
		 * \return �����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode);
		
	private:
		/**
		 * \brief �ò���ʹ�õ�Shader����
		 */
		std::string m_ShaderName;
	};
}