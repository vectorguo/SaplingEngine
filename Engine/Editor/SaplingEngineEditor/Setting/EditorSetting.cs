using System.Xml;

namespace SaplingEngineEditor
{
    public static class EditorSetting
    {
        /// <summary>
        /// 起始场景名称
        /// </summary>
        public static string StartSceneName { get; private set; }

        /// <summary>
        /// 起始场景路径
        /// </summary>
        public static string StartScenePath { get; private set; }

        /// <summary>
        /// 初始化
        /// </summary>
        public static void Initialize()
        {
            LoadEditorSetting();
        }

        #region LoadXML & SaveXML
        /// <summary>
        /// 从XML中读取编辑器配置
        /// </summary>
        private static void LoadEditorSetting()
        {
            XmlDocument document = new XmlDocument();
            document.Load("Resources/Configs/EditorConfig.xml");

            //根节点
            XmlNode root = document.SelectSingleNode("root");

            //起始场景
            var startSceneNode = root.SelectSingleNode("startScene");
            StartSceneName = startSceneNode.Attributes["sceneName"].Value;
            StartScenePath = startSceneNode.Attributes["scenePath"].Value;
        }

        /// <summary>
        /// 保存编辑器配置到XML
        /// </summary>
        private static void SaveEditorSetting()
        {

        }
        #endregion
    }
}
