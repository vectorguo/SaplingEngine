namespace SaplingEngineEditor
{
    class EditorTransformData : EditorComponentData
    {
        /// <summary>
        /// Inspector
        /// </summary>
        public override EditorComponentInspector Inspector => EditorTransformInspector.Instance;

        /// <summary>
        /// 位置
        /// </summary>
        public Vector3 Position;

        /// <summary>
        /// 欧拉角度
        /// </summary>
        public Vector3 EularAngles;

        /// <summary>
        /// 缩放
        /// </summary>
        public Vector3 Scale;
    }
}
