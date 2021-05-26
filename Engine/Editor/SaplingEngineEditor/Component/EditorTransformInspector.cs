using System.Drawing;

namespace SaplingEngineEditor
{
    public class EditorTransformInspector : EditorComponentInspector
    {
        /// <summary>
        /// 单例
        /// </summary>
        private static EditorTransformInspector instance;
        public static EditorTransformInspector Instance
        {
            get
            {
                if (instance == null)
                {
                    instance = new EditorTransformInspector();
                }
                return instance;
            }
        }

        public override void OnGUI()
        {
            var transformData = ComponentData as EditorTransformData;
            if (transformData == null)
            {
                return;
            }

            Height = 90;

            ShowGroupBox("Transform");

            var lineNum = 0;
            ShowVector3("Position", transformData.Position,     20, lineNum++ * LineSpacing + VertSpacing + 10);
            ShowVector3("Rotation", transformData.EularAngles,  20, lineNum++ * LineSpacing + VertSpacing + 10);
            ShowVector3("Scale",    transformData.Scale,        20, lineNum++ * LineSpacing + VertSpacing + 10);
        }
    }
}