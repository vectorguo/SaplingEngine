#include "Application/Setting.h"
#include "Editor/Application/EditorSetting.h"

using namespace SaplingEngine;

namespace SaplingEditor
{
	std::string EditorSetting::startSceneName;
	std::string EditorSetting::startScenePath;

	/**
	 * \brief	初始化
	 * \param	width		屏幕宽度
	 * \param	height		屏幕高度
	 * \return	是否初始化成功
	 */
	void EditorSetting::Initialize(uint32_t width, uint32_t height)
	{
		Setting::Initialize();

		//设置编辑器模式
		Setting::isEditorMode = true;

		//重新设置窗口宽度和高度
		Setting::screenWidth = width;
		Setting::screenHeight = height;

		//加载XML配置
		auto* pDocumentFile = new XmlDocumentFile("Resources/Configs/EditorConfig.xml");
		auto* pDocument = new XmlDocument();
		pDocument->parse<0>(pDocumentFile->data());

		//读取root节点
		const auto* pRootNode = pDocument->first_node();

		//读取启动场景配置
		const auto* pSceneNode = pRootNode->first_node("startScene");
		startSceneName = SaplingEngine::XmlGetAttributeValue<const char*>(pSceneNode, "sceneName");
		startScenePath = SaplingEngine::XmlGetAttributeValue<const char*>(pSceneNode, "scenePath");

		//卸载XML
		delete pDocument;
		delete pDocumentFile;
	}
}