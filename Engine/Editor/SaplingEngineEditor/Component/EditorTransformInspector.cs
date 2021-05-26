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
            Control.Controls.Clear();

            var transformData = ComponentData as EditorTransformData;
            if (transformData == null)
            {
                return;
            }

            var lineNum = 0;
            ShowElementLabel("Transform", 100, ref lineNum);
            ShowVector3("Position", transformData.Position, ref lineNum);
            ShowVector3("Rotation", transformData.EularAngles, ref lineNum);
            ShowVector3("Scale", transformData.Scale, ref lineNum);
        }
    }
}
