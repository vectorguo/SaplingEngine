using System.Collections.Generic;
using System.Xml;

namespace SaplingEngineEditor
{
    public class EditorGameObjectData
    {
        /// <summary>
        /// ID
        /// </summary>
        public int ID { get; }

        /// <summary>
        /// 名称
        /// </summary>
        private string name;
        public string Name
        {
            get => name;
            set => name = value;
        }

        /// <summary>
        /// 组件数据
        /// </summary>
        private readonly List<EditorComponentData> componentDatas;
        public List<EditorComponentData> ComponentDatas => componentDatas;

        /// <summary>
        /// 子节点
        /// </summary>
        public List<EditorGameObjectData> Children { get; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public EditorGameObjectData(XmlNode dataNode)
        {
            name = dataNode.Attributes["name"].Value;

            //初始化组件数据
            componentDatas = new List<EditorComponentData>();
            componentDatas.Add(new EditorTransformData());
        }
    }
}
