using System;
using System.Drawing;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public class EditorComponentInspector
    {
        protected const int LineSpacing = 25;

        /// <summary>
        /// 组件数据
        /// </summary>
        public EditorComponentData ComponentData { protected get; set; }

        /// <summary>
        /// Control
        /// </summary>
        public TabPage Control { protected get; set; }

        public virtual void OnGUI()
        {
            
        }

        protected void ShowElementLabel(string labelText, int labelWidth, ref int lineNum, int horizSpacing = 3, int vertSpacing = 0)
        {
            Label label = new Label();
            Point location = new Point(horizSpacing, lineNum * LineSpacing + vertSpacing);
            label.Location = location;
            label.Width = labelWidth;
            label.Text = labelText;
            Control.Controls.Add(label);

            lineNum += 1;
        }

        protected void ShowVector3(string propertyName, Vector3 v3, ref int lineNum)
        {
            ShowElementLabel(propertyName, 60, ref lineNum, 30, 3);
            --lineNum;

            var vs = new float[3] {v3.x, v3.y, v3.z};
            for (int i = 0; i < 3; ++i)
            {
                TextBox textBox = new TextBox();
                Point location = new Point(100 + (i * 65), lineNum * LineSpacing);
                textBox.Text = string.Format("{0:0.###}", vs[i]);
                textBox.TextAlign = HorizontalAlignment.Right;
                textBox.Location = location;
                textBox.Width = 60;
                //textBox.Leave += new EventHandler(NumElementChanged);

                Control.Controls.Add(textBox);
            }

            lineNum += 1;
        }
    }
}