using System.Collections.Generic;

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
        /// 子节点
        /// </summary>
        public List<EditorGameObjectData> Children { get; }

        /// <summary>
        /// 构造函数
        /// </summary>
        public EditorGameObjectData(int id, string name)
        {
            ID = id;
            Name = name;
        }
    }
}
