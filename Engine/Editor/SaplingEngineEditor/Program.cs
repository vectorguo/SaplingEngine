using System;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    static class Program
    {
        public static MainWindow MainWindow { get; private set; }

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
                //创建主窗口
                MainWindow = new MainWindow();
                var messageHandler = MainWindow.MessageHandler;
                if (messageHandler != null)
                {
                    Application.AddMessageFilter(messageHandler);
                    Application.Idle += new EventHandler(messageHandler.ApplicationIdle);
                }
                Application.Run(MainWindow);
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}
