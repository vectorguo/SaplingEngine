using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();

            try
            {
                //初始化DirectX窗口
                IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                IntPtr hwnd = DirectXPanel.Handle;
                if (NativeMethods.Application_Initialize(hInstance, hwnd, DirectXPanel.Width, DirectXPanel.Height))
                {

                }
                else
                {
                    MessageBox.Show("DirectX窗口初始化失败");
                }
            }
            catch(Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }
        }
    }
}