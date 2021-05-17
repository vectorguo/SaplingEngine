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
                var mainWindow = new MainWindow();
                Application.Run(mainWindow);
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}
