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
        public static extern bool Setting_Initialize();

        [DllImport(NativeDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Application_Initialize(IntPtr instancePtrAddress, IntPtr hWndPtrAddress, int windowWidth, int windowHeight);
    }
}
