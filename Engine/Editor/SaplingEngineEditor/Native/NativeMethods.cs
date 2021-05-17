using System.Runtime.InteropServices;

namespace SaplingEngineEditor
{
    static class NativeMethods
    {
        const string NativeDllName = "SaplingEngineEditorDll.dll";

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Test();
    }
}
