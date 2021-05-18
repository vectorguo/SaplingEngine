using System;
using System.ComponentModel;
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
                //初始化编辑器
                IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                if (NativeMethods.Editor_Initialize(hInstance, DirectXPanel.Handle, (uint)DirectXPanel.Width, (uint)DirectXPanel.Height))
                {
                }
                else
                {
                    MessageBox.Show("编辑器初始化失败");
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Error: " + e.ToString());
            }

            MessageHandler = new MessageHandler(DirectXPanel, this);
        }

        protected override void OnClosed(EventArgs e)
        {
            NativeMethods.Editor_Destroy();
        }
    }
}