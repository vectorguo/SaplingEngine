using System.Collections.Generic;
using System.Xml;

namespace SaplingEngineEditor
{
    public static class EditorSceneManager
    {
        /// <summary>
        /// 当前的场景名称
        /// </summary>
        public static string CurrentSceneName { get; private set; }

        /// <summary>
        /// 挡墙场景路径
        /// </summary>
        public static string CurrentScenePath { get; private set; }

        /// <summary>
        /// 场景XML文档
        /// </summary>
        private static XmlDocument sceneXmlDocument;

        /// <summary>
        /// 场景XML文档的根节点
        /// </summary>
        private static XmlNode sceneXmlRoot;

        /// <summary>
        /// GameObject数据列表
        /// </summary>
        private static List<EditorGameObjectData> gameObjectDatas = new List<EditorGameObjectData>();
        public static List<EditorGameObjectData> GameObjectDatas => gameObjectDatas;

        /// <summary>
        /// 打开场景
        /// </summary>
        /// <param name="sceneName">场景名称</param>
        /// <param name="scenePath">场景路径</param>
        /// <returns></returns>
        public static bool OpenScene(string sceneName, string scenePath)
        {
            if (CurrentScenePath == scenePath)
            {
                return false;
            }

            CurrentSceneName = sceneName;
            CurrentScenePath = scenePath;

            //打开场景
            NativeMethod.Scene_OpenScene(CurrentSceneName, CurrentScenePath, new Vector3(0, 20, -130), new Vector3(45, 0, 0));

            //初始化场景数据
            InitializeSceneData();

            //初始化Hierarchy界面
            MainWindow.Instance.InitializeHierarchy(gameObjectDatas);

            return true;
        }

        /// <summary>
        /// 初始化场景数据
        /// </summary>
        private static void InitializeSceneData()
        {
            if (sceneXmlDocument == null)
            {
                sceneXmlDocument = new XmlDocument();
                sceneXmlDocument.Load(CurrentScenePath);
                sceneXmlRoot = sceneXmlDocument.SelectSingleNode("root");
            }

            gameObjectDatas.Clear();
            foreach (XmlNode gameObjectNode in sceneXmlRoot.ChildNodes)
            {
                var gameObjectName = gameObjectNode.Attributes["name"].Value;
                gameObjectDatas.Add(new EditorGameObjectData(0, gameObjectName));

            }
        }
    }
}
