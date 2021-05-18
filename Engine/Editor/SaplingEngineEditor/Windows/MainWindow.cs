using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public partial class MainWindow : Form
    {
        public MessageHandler MessageHandler { get; }

        public MainWindow()
        {
            InitializeComponent();

            try
            {
                //初始化配置信息
                if (NativeMethods.Setting_Initialize((uint)DirectXPanel.Width, (uint)DirectXPanel.Height))
                {
                    //初始化DirectX窗口
                    IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                    if (NativeMethods.Application_Initialize(hInstance, DirectXPanel.Handle))
                    {
                    }
                    else
                    {
                        MessageBox.Show("DirectX窗口初始化失败");
                    }
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

            MessageHandler = new MessageHandler(DirectXPanel, this);
        }
    }
}