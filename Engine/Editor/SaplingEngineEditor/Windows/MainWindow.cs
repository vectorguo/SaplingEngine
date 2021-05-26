using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public partial class MainWindow : Form
    {
        /// <summary>
        /// 单例
        /// </summary>
        public static MainWindow Instance { get; private set; }

        public MessageHandler MessageHandler { get; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public MainWindow()
        {
            Instance = this;

            //初始化组件
            InitializeComponent();

            try
            {
                //初始化编辑器配置
                EditorSetting.Initialize();

                //初始化编辑器
                IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                if (NativeMethod.Editor_Initialize(hInstance, DirectXPanel.Handle, (uint)DirectXPanel.Width, (uint)DirectXPanel.Height))
                {
                    //打开起始场景
                    EditorSceneManager.OpenScene(EditorSetting.StartSceneName, EditorSetting.StartScenePath);
                }
                else
                {
                    MessageBox.Show("编辑器初始化失败");
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }

            MessageHandler = new MessageHandler(DirectXPanel, this);
        }

        protected override void OnClosed(EventArgs e)
        {
            NativeMethod.Editor_Destroy();
        }

        #region Hierarchy

        /// <summary>
        /// 初始化Hierarchy
        /// </summary>
        public void InitializeHierarchy(List<EditorGameObjectData> gameObjectDatas)
        {
            //清除之前的Node
            SceneTreeView.Nodes.Clear();

            //获取GameObject数据
            foreach (var gameObjectData in gameObjectDatas)
            {
                SceneTreeView.Nodes.Add(CreateTreeNode(gameObjectData));
            }
        }

        /// <summary>
        /// 创建TreeNode
        /// </summary>
        private TreeNode CreateTreeNode(EditorGameObjectData gameObjectData)
        {
            var treeNode = new TreeNode
            {
                Name = gameObjectData.ID.ToString(),
                Text = gameObjectData.Name,
            };

            if (gameObjectData.Children != null && gameObjectData.Children.Count > 0)
            {
                foreach (var child in gameObjectData.Children)
                {
                    treeNode.Nodes.Add(CreateTreeNode(child));
                }
            }

            return treeNode;
        }

        private void SceneTreeView_AfterSelect(object sender, TreeViewEventArgs e)
        {
            var selectedNode = SceneTreeView.SelectedNode;
            if (selectedNode == null)
            {
                return;
            }

            ShowGameObjectInspector(EditorSceneManager.GameObjectDatas[selectedNode.Index]);
        }
        #endregion

        #region Inspector
        private void ShowGameObjectInspector(EditorGameObjectData gameObjectData)
        {
            foreach (var componentData in gameObjectData.ComponentDatas)
            {
                var inspector = componentData.Inspector;
                inspector.ComponentData = componentData;
                inspector.Control = Inspector;
                inspector.OnGUI();
            }
        }
        #endregion
    }
}