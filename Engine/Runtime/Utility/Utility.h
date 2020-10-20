#pragma once

#include <string>

/*
 * XML
 */
#include "Utility/RapidXml/rapidxml.hpp"
#include "Utility/RapidXml/rapidxml_utils.hpp"
#include "Utility/RapidXml/rapidxml_print.hpp"

using XmlDocumentFile	= rapidxml::file<>;
using XmlDocument		= rapidxml::xml_document<>;
using XmlNode			= rapidxml::xml_node<>;
using XmlAttribute		= rapidxml::xml_attribute<>;

namespace SaplingEngine
{
	/**
	 * \brief ��ȡXML�ڵ�����ֵ��ģ�庯������
	 * \tparam T ����ֵ����
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<typename T>
	T XmlGetAttributeValue(const XmlNode* pNode, const char* attributeName);

	/**
	 * \brief ��ȡXML�ڵ�����ֵ��ģ�庯������
	 * \tparam T ����ֵ����
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<typename T>
	T XmlGetAttributeValue(const XmlNode* pNode, const std::string& attributeName);
	
	/**
	 * \brief ��ȡXML�ڵ����Ե�int32_tֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline int32_t XmlGetAttributeValue<int32_t>(const XmlNode* pNode, const char* attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName);
		return pAttribute ? std::stoi(pAttribute->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�uint32_tֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline uint32_t XmlGetAttributeValue<uint32_t>(const XmlNode* pNode, const char* attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName);
		return pAttribute ? static_cast<uint32_t>(std::stoi(pAttribute->value())) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�boolֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline bool XmlGetAttributeValue<bool>(const XmlNode* pNode, const char* attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName);
		return pAttribute ? std::stoi(pAttribute->value()) : false;
	}
	
	/**
	 * \brief ��ȡXML�ڵ����Ե�floatֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline float XmlGetAttributeValue<float>(const XmlNode* pNode, const char* attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName);
		return pAttribute ? std::stof(pAttribute->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե��ַ���ֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline const char* XmlGetAttributeValue<const char*>(const XmlNode* pNode, const char* attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName);
		return pAttribute ? pAttribute->value() : nullptr;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�int32_tֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline int32_t XmlGetAttributeValue<int32_t>(const XmlNode* pNode, const std::string& attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName.data());
		return pAttribute ? std::stoi(pAttribute->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�uint32_tֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline uint32_t XmlGetAttributeValue<uint32_t>(const XmlNode* pNode, const std::string& attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName.data());
		return pAttribute ? static_cast<uint32_t>(std::stoi(pAttribute->value())) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�boolֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline bool XmlGetAttributeValue<bool>(const XmlNode* pNode, const std::string& attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName.data());
		return pAttribute ? std::stoi(pAttribute->value()) : false;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե�floatֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline float XmlGetAttributeValue<float>(const XmlNode* pNode, const std::string& attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName.data());
		return pAttribute ? std::stof(pAttribute->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ڵ����Ե��ַ���ֵ��ģ�庯������
	 * \param pNode XML�ڵ�
	 * \param attributeName ��������
	 * \return ����ֵ
	 */
	template<>
	inline const char* XmlGetAttributeValue<const char*>(const XmlNode* pNode, const std::string& attributeName)
	{
		const auto* pAttribute = pNode->first_attribute(attributeName.data());
		return pAttribute ? pAttribute->value() : nullptr;
	}

	/**
	 * \brief ��ȡXML�ڵ�ֵ��ģ�庯������
	 * \tparam T �ڵ�ֵ����
	 * \param pParentNode XML�ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<typename T>
	T XmlGetNodeValue(const XmlNode* pParentNode, const char* childNodeName);

	/**
	 * \brief ��ȡXML�ڵ�ֵ��ģ�庯������
	 * \tparam T �ڵ�ֵ����
	 * \param pParentNode XML�ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<typename T>
	T XmlGetNodeValue(const XmlNode* pParentNode, const std::string& childNodeName);

	/**
	 * \brief ��ȡXML�ӽڵ��int32_tֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline int32_t XmlGetNodeValue<int32_t>(const XmlNode* pParentNode, const char* childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName);
		return pChildNode ? std::stoi(pChildNode->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��uint32_tֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline uint32_t XmlGetNodeValue<uint32_t>(const XmlNode* pParentNode, const char* childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName);
		return pChildNode ? static_cast<uint32_t>(std::stoi(pChildNode->value())) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��boolֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline bool XmlGetNodeValue<bool>(const XmlNode* pParentNode, const char* childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName);
		return pChildNode ? std::stoi(pChildNode->value()) : false;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��floatֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline float XmlGetNodeValue<float>(const XmlNode* pParentNode, const char* childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName);
		return pChildNode ? std::stof(pChildNode->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ���ַ���ֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline char* XmlGetNodeValue<char*>(const XmlNode* pParentNode, const char* childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName);
		return pChildNode ? pChildNode->value() : nullptr;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��int32_tֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline int32_t XmlGetNodeValue<int32_t>(const XmlNode* pParentNode, const std::string& childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName.data());
		return pChildNode ? std::stoi(pChildNode->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��uint32_tֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline uint32_t XmlGetNodeValue<uint32_t>(const XmlNode* pParentNode, const std::string& childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName.data());
		return pChildNode ? static_cast<uint32_t>(std::stoi(pChildNode->value())) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��boolֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline bool XmlGetNodeValue<bool>(const XmlNode* pParentNode, const std::string& childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName.data());
		return pChildNode ? std::stoi(pChildNode->value()) : false;
	}

	/**
	 * \brief ��ȡXML�ӽڵ��floatֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline float XmlGetNodeValue<float>(const XmlNode* pParentNode, const std::string& childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName.data());
		return pChildNode ? std::stof(pChildNode->value()) : 0;
	}

	/**
	 * \brief ��ȡXML�ӽڵ���ַ���ֵ��ģ�庯������
	 * \param pParentNode XML���ڵ�
	 * \param childNodeName �ӽڵ�����
	 * \return �ڵ�ֵ
	 */
	template<>
	inline char* XmlGetNodeValue<char*>(const XmlNode* pParentNode, const std::string& childNodeName)
	{
		const auto* pChildNode = pParentNode->first_node(childNodeName.data());
		return pChildNode ? pChildNode->value() : nullptr;
	}
	
	/*
	 * Char-WChar����ת��
	 */
	inline std::wstring CharToWChar(const char* c, size_t encode = CP_ACP)
	{
		const int len = MultiByteToWideChar(static_cast<uint32_t>(encode), 0, c, static_cast<int32_t>(strlen(c)), nullptr, 0);
		auto* bytes = new wchar_t[len + 1];
		MultiByteToWideChar(static_cast<uint32_t>(encode), 0, c, static_cast<int32_t>(strlen(c)), bytes, len);
		bytes[len] = '\0';
		std::wstring str = bytes;
		delete[] bytes;
		return str;
	}

	inline std::string WCharToChar(const wchar_t* wc, size_t encode = CP_ACP)
	{
		const int len = WideCharToMultiByte(static_cast<uint32_t>(encode), 0, wc, static_cast<int32_t>(wcslen(wc)), nullptr, 0, nullptr, nullptr);
		char* bytes = new char[len + 1];
		WideCharToMultiByte(static_cast<uint32_t>(encode), 0, wc, static_cast<int32_t>(wcslen(wc)), bytes, len, nullptr, nullptr);
		bytes[len] = '\0';
		std::string str = bytes;
		delete[] bytes;
		return str;
	}
}
