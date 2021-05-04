#pragma once

#include "MaterialProperty.h"
#include "Shader.h"
#include "ShaderManager.h"

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
		 * \brief	�����л�
		 * \param	pNode		���ýڵ�ָ��
		 * \return	�����л��Ƿ�ɹ�
		 */
		bool Deserialize(const XmlNode* pNode);

		/**
		 * \brief	��ȡ����ʹ�õ�Shader������
		 * \return	Shader����
		 */
		inline const std::string& GetShaderName() const
		{
			return m_pShader->GetName();
		}

		/**
		 * \brief	��ȡ����ʹ�õ�Shader��HashValue
		 * \return	Shader��HashValue
		 */
		inline size_t GetShaderHashValue() const
		{
			return m_pShader->GetHashValue();
		}

		/**
		 * \brief	��ȡ����ʹ�õ�Shader������
		 * \return	Shader������
		 */
		inline uint8_t GetShaderType() const
		{
			return m_pShader->GetType();
		}

		/**
		 * \brief	����Shader
		 * \param	shaderName		Shader����
		 */
		inline void SetShader(const std::string& shaderName)
		{
			m_pShader = ShaderManager::GetShader(shaderName);
		}

		/**
		 * \brief	��ȡint���͵�����ֵ
		 * \param	propertyName	��������
		 * \return	int���͵�����ֵ
		 */
		inline int32_t GetInt(const std::string& propertyName) const
		{
			return GetInt(StringToHash(propertyName));
		}

		/**
		 * \brief	��ȡint���͵�����ֵ
		 * \param	propertyID		����ID
		 * \return	int���͵�����ֵ
		 */
		inline int32_t GetInt(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? 0 : iter->second.GetInt();
		}

		/**
		 * \brief	��ȡfloat���͵�����ֵ
		 * \param	propertyName	��������
		 * \return	float���͵�����ֵ
		 */
		inline float GetFloat(const std::string& propertyName) const
		{
			return GetFloat(StringToHash(propertyName));
		}

		/**
		 * \brief	��ȡfloat���͵�����ֵ
		 * \param	propertyID		����ID
		 * \return	float���͵�����ֵ
		 */
		inline float GetFloat(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? 0 : iter->second.GetFloat();
		}

		/**
		 * \brief	��ȡColor���͵�����ֵ
		 * \param	propertyName	��������
		 * \return	Color���͵�����ֵ
		 */
		inline const Color& GetColor(const std::string& propertyName) const
		{
			return GetColor(StringToHash(propertyName));
		}

		/**
		 * \brief	��ȡColor���͵�����ֵ
		 * \param	propertyID		����ID
		 * \return	Color���͵�����ֵ
		 */
		inline const Color& GetColor(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? Color::White : iter->second.GetColor();
		}

		/**
		 * \brief	��ȡVector4���͵�����ֵ
		 * \param	propertyName	��������
		 * \return	Vector4���͵�����ֵ
		 */
		inline const Vector4& GetVector4(const std::string& propertyName) const
		{
			return GetVector4(StringToHash(propertyName));
		}

		/**
		 * \brief	��ȡVector4���͵�����ֵ
		 * \param	propertyID		����ID
		 * \return	Vector4���͵�����ֵ
		 */
		inline const Vector4& GetVector4(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? Vector4::Zero : iter->second.GetVector4();
		}

		/**
		 * \brief	����int���͵�����ֵ
		 * \param	propertyName	��������
		 * \param	value			����ֵ
		 */
		inline void SetInt(const std::string& propertyName, int32_t value)
		{
			return SetInt(StringToHash(propertyName), value);
		}

		/**
		 * \brief	����int���͵�����ֵ
		 * \param	propertyID		����ID
		 * \param	value			����ֵ
		 */
		inline void SetInt(size_t propertyID, int32_t value)
		{
			m_Properties[propertyID].SetInt(value);
		}

		/**
		 * \brief	����float���͵�����ֵ
		 * \param	propertyName	��������
		 * \param	value			����ֵ
		 */
		inline void SetFloat(const std::string& propertyName, float value)
		{
			return SetFloat(StringToHash(propertyName), value);
		}

		/**
		 * \brief	����float���͵�����ֵ
		 * \param	propertyID		����ID
		 * \param	value			����ֵ
		 */
		inline void SetFloat(size_t propertyID, float value)
		{
			m_Properties[propertyID].SetFloat(value);
		}

		/**
		 * \brief	����Color���͵�����ֵ
		 * \param	propertyName	��������
		 * \param	value			����ֵ
		 */
		inline void SetColor(const std::string& propertyName, const Color& value)
		{
			return SetColor(StringToHash(propertyName), value);
		}

		/**
		 * \brief	����Color���͵�����ֵ
		 * \param	propertyID		����ID
		 * \param	value			����ֵ
		 */
		inline void SetColor(size_t propertyID, const Color& value)
		{
			m_Properties[propertyID].SetColor(value);
		}

		/**
		 * \brief	����Color���͵�����ֵ
		 * \param	propertyName	��������
		 */
		inline void SetColor(const std::string& propertyName, float r, float g, float b, float a)
		{
			return SetColor(StringToHash(propertyName), r, g, b, a);
		}

		/**
		 * \brief	����Color���͵�����ֵ
		 * \param	propertyID		����ID
		 */
		inline void SetColor(size_t propertyID, float r, float g, float b, float a)
		{
			m_Properties[propertyID].SetColor(r, g, b, a);
		}

		/**
		 * \brief	����Vector4���͵�����ֵ
		 * \param	propertyName	��������
		 * \param	value			����ֵ
		 */
		inline void SetVector4(const std::string& propertyName, const Vector4& value)
		{
			return SetVector4(StringToHash(propertyName), value);
		}

		/**
		 * \brief	����Vector4���͵�����ֵ
		 * \param	propertyID		����ID
		 * \param	value			����ֵ
		 */
		inline void SetVector4(size_t propertyID, const Vector4& value)
		{
			m_Properties[propertyID].SetVector4(value);
		}

		/**
		 * \brief	����Vector4���͵�����ֵ
		 * \param	propertyName	��������
		 */
		inline void SetVector4(const std::string& propertyName, float x, float y, float z, float w)
		{
			return SetVector4(StringToHash(propertyName), x, y, z, w);
		}

		/**
		 * \brief	����Vector4���͵�����ֵ
		 * \param	propertyID		����ID
		 */
		inline void SetVector4(size_t propertyID, float x, float y, float z, float w)
		{
			m_Properties[propertyID].SetVector4(x, y, z, w);
		}
		
	private:
		/**
		 * \brief	����ʹ�õ�shader
		 */
		const Shader* m_pShader = nullptr;

		/**
		 * \brief	���ʵ������б�
		 */
		std::map<size_t, MaterialProperty> m_Properties;
	};
}