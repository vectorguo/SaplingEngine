using System;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            var i = NativeMethods.Test();

            var mainWindow = new SaplingEngineMainWindow();
            Application.Run(mainWindow);
        }
    }
}
