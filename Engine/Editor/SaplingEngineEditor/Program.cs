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
                var messageHandler = mainWindow.MessageHandler;
                if (messageHandler != null)
                {
                    Application.AddMessageFilter(messageHandler);
                    Application.Idle += new EventHandler(messageHandler.ApplicationIdle);
                }
                Application.Run(mainWindow);
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}
