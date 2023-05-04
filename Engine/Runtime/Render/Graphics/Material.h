#pragma once

#include "Runtime/Render/Graphics/MaterialProperty.h"
#include "Runtime/Render/Graphics/Shader.h"
#include "Runtime/Render/Graphics/ShaderManager.h"
#include "Runtime/Render/Graphics/Texture2D.h"

namespace SaplingEngine
{
	class Material final
	{
		friend class MaterialManager;

	public:
		Material();
		~Material() {}

		Material(const Material&) = delete;
		Material(Material&&) = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) = delete;

		/**
		 * \brief	获取材质使用的Shader的名称
		 * \return	Shader名称
		 */
		inline const std::string& GetShaderName() const
		{
			return m_ShaderPtr->GetName();
		}

		/**
		 * \brief	获取材质使用的Shader的HashValue
		 * \return	Shader的HashValue
		 */
		inline size_t GetShaderHashValue() const
		{
			return m_ShaderPtr->GetHashValue();
		}

		/**
		 * \brief	获取材质使用的Shader的类型
		 * \return	Shader的类型
		 */
		inline uint8_t GetShaderOcbType() const
		{
			return m_ShaderPtr->GetOcbType();
		}

		/**
		 * \brief	设置Shader
		 * \param	shaderID		ShaderID
		 */
		void SetShader(size_t shaderID);

		/**
		 * \brief	设置Shader
		 * \param	shaderName		Shader名称
		 */
		inline void SetShader(const std::string& shaderName)
		{
			SetShader(StringToHash(shaderName));
		}

		/**
		 * \brief	获取int类型的属性值
		 * \param	propertyName	属性名称
		 * \return	int类型的属性值
		 */
		inline int32_t GetInt(const std::string& propertyName) const
		{
			return GetInt(StringToHash(propertyName));
		}

		/**
		 * \brief	获取int类型的属性值
		 * \param	propertyID		属性ID
		 * \return	int类型的属性值
		 */
		inline int32_t GetInt(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? 0 : iter->second.GetInt();
		}

		/**
		 * \brief	获取float类型的属性值
		 * \param	propertyName	属性名称
		 * \return	float类型的属性值
		 */
		inline float GetFloat(const std::string& propertyName) const
		{
			return GetFloat(StringToHash(propertyName));
		}

		/**
		 * \brief	获取float类型的属性值
		 * \param	propertyID		属性ID
		 * \return	float类型的属性值
		 */
		inline float GetFloat(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? 0 : iter->second.GetFloat();
		}

		/**
		 * \brief	获取Color类型的属性值
		 * \param	propertyName	属性名称
		 * \return	Color类型的属性值
		 */
		inline const Color& GetColor(const std::string& propertyName) const
		{
			return GetColor(StringToHash(propertyName));
		}

		/**
		 * \brief	获取Color类型的属性值
		 * \param	propertyID		属性ID
		 * \return	Color类型的属性值
		 */
		inline const Color& GetColor(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? Color::White : iter->second.GetColor();
		}

		/**
		 * \brief	获取Vector3类型的属性值
		 * \param	propertyName	属性名称
		 * \return	Vector3类型的属性值
		 */
		inline const Vector3& GetVector3(const std::string& propertyName) const
		{
			return GetVector3(StringToHash(propertyName));
		}

		/**
		 * \brief	获取Vector3类型的属性值
		 * \param	propertyID		属性ID
		 * \return	Vector3类型的属性值
		 */
		inline const Vector3& GetVector3(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? Vector3::Zero : iter->second.GetVector3();
		}

		/**
		 * \brief	获取Vector4类型的属性值
		 * \param	propertyName	属性名称
		 * \return	Vector4类型的属性值
		 */
		inline const Vector4& GetVector4(const std::string& propertyName) const
		{
			return GetVector4(StringToHash(propertyName));
		}

		/**
		 * \brief	获取Vector4类型的属性值
		 * \param	propertyID		属性ID
		 * \return	Vector4类型的属性值
		 */
		inline const Vector4& GetVector4(size_t propertyID) const
		{
			auto iter = m_Properties.find(propertyID);
			return iter == m_Properties.cend() ? Vector4::Zero : iter->second.GetVector4();
		}

		/**
		 * \brief	获取贴图
		 * \param	index			贴图索引
		 * \return	对应索引位置上的贴图
		 */
		inline const Texture2DSptr& GetTexture(int32_t index) const
		{
			return m_Textures[index];
		}

		/**
		 * \brief	获取材质的所有贴图
		 */
		inline const std::vector<Texture2DSptr> GetTextures() const
		{
			return m_Textures;
		}

		/**
		 * \brief	设置int类型的属性值
		 * \param	propertyName	属性名称
		 * \param	value			属性值
		 */
		inline void SetInt(const std::string& propertyName, int32_t value)
		{
			return SetInt(StringToHash(propertyName), value);
		}

		/**
		 * \brief	设置int类型的属性值
		 * \param	propertyID		属性ID
		 * \param	value			属性值
		 */
		inline void SetInt(size_t propertyID, int32_t value)
		{
			m_Properties[propertyID].SetInt(value);
		}

		/**
		 * \brief	设置float类型的属性值
		 * \param	propertyName	属性名称
		 * \param	value			属性值
		 */
		inline void SetFloat(const std::string& propertyName, float value)
		{
			return SetFloat(StringToHash(propertyName), value);
		}

		/**
		 * \brief	设置float类型的属性值
		 * \param	propertyID		属性ID
		 * \param	value			属性值
		 */
		inline void SetFloat(size_t propertyID, float value)
		{
			m_Properties[propertyID].SetFloat(value);
		}

		/**
		 * \brief	设置Color类型的属性值
		 * \param	propertyName	属性名称
		 * \param	value			属性值
		 */
		inline void SetColor(const std::string& propertyName, const Color& value)
		{
			return SetColor(StringToHash(propertyName), value);
		}

		/**
		 * \brief	设置Color类型的属性值
		 * \param	propertyID		属性ID
		 * \param	value			属性值
		 */
		inline void SetColor(size_t propertyID, const Color& value)
		{
			m_Properties[propertyID].SetColor(value);
		}

		/**
		 * \brief	设置Color类型的属性值
		 * \param	propertyName	属性名称
		 */
		inline void SetColor(const std::string& propertyName, float r, float g, float b, float a)
		{
			return SetColor(StringToHash(propertyName), r, g, b, a);
		}

		/**
		 * \brief	设置Color类型的属性值
		 * \param	propertyID		属性ID
		 */
		inline void SetColor(size_t propertyID, float r, float g, float b, float a)
		{
			m_Properties[propertyID].SetColor(r, g, b, a);
		}

		/**
		 * \brief	设置Vector3类型的属性值
		 * \param	propertyName	属性名称
		 * \param	value			属性值
		 */
		inline void SetVector3(const std::string& propertyName, const Vector3& value)
		{
			return SetVector3(StringToHash(propertyName), value);
		}

		/**
		 * \brief	设置Vector3类型的属性值
		 * \param	propertyID		属性ID
		 * \param	value			属性值
		 */
		inline void SetVector3(size_t propertyID, const Vector3& value)
		{
			m_Properties[propertyID].SetVector3(value);
		}

		/**
		 * \brief	设置Vector3类型的属性值
		 * \param	propertyName	属性名称
		 */
		inline void SetVector3(const std::string& propertyName, float x, float y, float z)
		{
			return SetVector3(StringToHash(propertyName), x, y, z);
		}

		/**
		 * \brief	设置Vector3类型的属性值
		 * \param	propertyID		属性ID
		 */
		inline void SetVector3(size_t propertyID, float x, float y, float z)
		{
			m_Properties[propertyID].SetVector3(x, y, z);
		}

		/**
		 * \brief	设置Vector4类型的属性值
		 * \param	propertyName	属性名称
		 * \param	value			属性值
		 */
		inline void SetVector4(const std::string& propertyName, const Vector4& value)
		{
			return SetVector4(StringToHash(propertyName), value);
		}

		/**
		 * \brief	设置Vector4类型的属性值
		 * \param	propertyID		属性ID
		 * \param	value			属性值
		 */
		inline void SetVector4(size_t propertyID, const Vector4& value)
		{
			m_Properties[propertyID].SetVector4(value);
		}

		/**
		 * \brief	设置Vector4类型的属性值
		 * \param	propertyName	属性名称
		 */
		inline void SetVector4(const std::string& propertyName, float x, float y, float z, float w)
		{
			return SetVector4(StringToHash(propertyName), x, y, z, w);
		}

		/**
		 * \brief	设置Vector4类型的属性值
		 * \param	propertyID		属性ID
		 */
		inline void SetVector4(size_t propertyID, float x, float y, float z, float w)
		{
			m_Properties[propertyID].SetVector4(x, y, z, w);
		}

		/**
		 * \brief	设置贴图
		 * \param	texture			贴图
		 * \param	index			贴图索引
		 */
		inline void SetTexture(const Texture2DSptr& texture, int32_t index)
		{
			m_Textures[index] = texture;
		}
		
	private:
		/**
		 * \brief	反序列化
		 * \param	pNode		配置节点指针
		 * \return	反序列化是否成功
		 */
		bool Deserialize(const XmlNode* pNode);

	private:
		/**
		 * \brief	材质使用的shader
		 */
		const Shader* m_ShaderPtr = nullptr;

		/**
		 * \brief	材质的属性列表
		 */
		std::map<size_t, MaterialProperty> m_Properties;

		/**
		 * \brief	主帖图
		 */
		std::vector<Texture2DSptr> m_Textures;
	};
}