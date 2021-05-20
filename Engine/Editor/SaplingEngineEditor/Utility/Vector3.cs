namespace SaplingEngineEditor
{
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3 Zero = new Vector3(0, 0, 0);
        public static Vector3 One = new Vector3(1,1 , 1);
        public static Vector3 Forward = new Vector3(0, 0, 1);
        public static Vector3 Up = new Vector3(0, 1, 0);
        public static Vector3 Right = new Vector3(1, 0, 0);
    }
}
