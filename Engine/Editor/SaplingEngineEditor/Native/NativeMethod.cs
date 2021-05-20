using System;
using System.Runtime.InteropServices;

namespace SaplingEngineEditor
{
    public static class NativeMethod
    {
        const string NativeDllName = "SaplingEngineEditorDll.dll";

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Editor_Initialize(IntPtr instancePtrAddress, IntPtr hWndPtrAddress, uint windowWidth, uint windowHeight);

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Editor_Run();

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Editor_Destroy();

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Editor_MessageProcess(IntPtr hWndPtrAddress, int msg, int wParam, int lParam);

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Scene_OpenScene(string sceneName, string scenePath);

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Scene_CloseScene();
    }
}
