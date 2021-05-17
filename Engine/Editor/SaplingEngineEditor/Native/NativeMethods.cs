using System;
using System.Runtime.InteropServices;

namespace SaplingEngineEditor
{
    static class NativeMethods
    {
        const string NativeDllName = "SaplingEngineEditorDll.dll";

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Test();

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Setting_Initialize(uint windowWidth, uint windowHeight);

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Application_Initialize(IntPtr instancePtrAddress, IntPtr hWndPtrAddress);

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Application_Run();

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Application_Destroy();
    }
}
