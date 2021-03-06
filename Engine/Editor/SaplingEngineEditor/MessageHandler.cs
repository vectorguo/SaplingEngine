﻿using System;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public class MessageHandler : IMessageFilter
    {
        #region 消息ID
        const int WM_DESTROY        = 0x0002;
        const int WM_SIZE           = 0x0005;
        const int WM_ACTIVATE       = 0x0006;
        const int WM_ENTERSIZEMOVE  = 0x0231;
        const int WM_EXITSIZEMOVE   = 0x0232;
        const int WM_MOUSEWHEEL     = 0x020A;
        const int WM_MOUSEMOVE      = 0x0200;
        const int WM_LBUTTONDOWN    = 0x0201;
        const int WM_LBUTTONUP      = 0x0202;
        const int WM_LBUTTONDBLCLK  = 0x0203;
        const int WM_RBUTTONDOWN    = 0x0204;
        const int WM_RBUTTONUP      = 0x0205;
        const int WM_RBUTTONDBLCLK  = 0x0206;
        const int WM_MBUTTONDOWN    = 0x0207;
        const int WM_MBUTTONUP      = 0x0208;
        const int WM_MBUTTONDBLCLK  = 0x0209;
        const int WM_KEYDOWN        = 0x0100;
        const int WM_KEYUP          = 0x0101;
        const int WM_SYSKEYDOWN     = 0x0104;
        const int WM_SYSKEYUP       = 0x0105;
        const int WM_CLOSE          = 0x0010;
        #endregion

        private readonly Panel directxPanel;
        private readonly MainWindow parent;

        private bool m_FocusOnDirectxPanel = false;

        public MessageHandler(Panel directxPanel, MainWindow parent)
        {
            this.directxPanel = directxPanel;
            this.parent = parent;
        }

        public bool PreFilterMessage(ref Message m)
        {
            if (m.Msg == WM_LBUTTONDOWN || m.Msg == WM_RBUTTONDOWN || m.Msg == WM_MBUTTONDOWN)
            {
                CheckFocusOnDirectxPanel();
            }

            if (m.HWnd == directxPanel.Handle || (m_FocusOnDirectxPanel && (m.Msg == WM_KEYDOWN || m.Msg == WM_KEYUP)))
            {
                switch (m.Msg)
                {
                    case WM_DESTROY:
                    case WM_SIZE:
                    case WM_ACTIVATE:
                    case WM_ENTERSIZEMOVE:
                    case WM_EXITSIZEMOVE:
                    case WM_MOUSEMOVE:
                    case WM_LBUTTONDOWN:
                    case WM_LBUTTONUP:
                    case WM_LBUTTONDBLCLK:
                    case WM_RBUTTONDOWN:
                    case WM_RBUTTONUP:
                    case WM_RBUTTONDBLCLK:
                    case WM_MBUTTONDOWN:
                    case WM_MBUTTONUP:
                    case WM_MBUTTONDBLCLK:
                    case WM_KEYDOWN:
                    case WM_KEYUP:
                    case WM_SYSKEYDOWN:
                    case WM_SYSKEYUP:
                    case WM_CLOSE:
                        {
                            NativeMethod.Editor_MessageProcess(directxPanel.Handle, m.Msg, m.WParam.ToInt64(), m.LParam.ToInt64());
                            return true;
                        }
                }
            }
            return false;
        }

        private void CheckFocusOnDirectxPanel()
        {
            var position = Cursor.Position;
            var relativeToForm = directxPanel.PointToClient(position);
            m_FocusOnDirectxPanel = (relativeToForm.X >= 0 && relativeToForm.Y >= 0 && relativeToForm.X < directxPanel.Width && relativeToForm.Y < directxPanel.Width);
        }

        public void ApplicationIdle(object sender, EventArgs e)
        {
            try
            {
                NativeMethod.Editor_Run();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            parent.Invalidate();
        }
    }
}
