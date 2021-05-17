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

            try
            {
                //配置信息初始化
                if (NativeMethods.Setting_Initialize())
                {
                    var mainWindow = new MainWindow();
                    Application.Run(mainWindow);
                }
                else
                {
                    MessageBox.Show("配置信息初始化失败");
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}
