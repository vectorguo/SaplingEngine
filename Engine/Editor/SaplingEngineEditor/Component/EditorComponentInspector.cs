using System.Drawing;
using System.Windows.Forms;

namespace SaplingEngineEditor
{
    public class EditorComponentInspector
    {
        protected const int LineSpacing = 22;
        protected const int ControlHeight = 21;

        /// <summary>
        /// 组件数据
        /// </summary>
        public EditorComponentData ComponentData { protected get; set; }

        /// <summary>
        /// Control
        /// </summary>
        public TabPage Control { protected get; set; }

        /// <summary>
        /// 垂直方向的起始位置
        /// </summary>
        public int VertSpacing { protected get; set; }

        /// <summary>
        /// 高度
        /// </summary>
        public int Height { get; protected set; }

        /// <summary>
        /// GroupBox
        /// </summary>
        protected GroupBox groupBox;

        public virtual void OnGUI()
        {
            
        }

        protected void ShowGroupBox(string text)
        {
            groupBox = new GroupBox();
            groupBox.Location = new Point(0, VertSpacing);
            groupBox.Text = text;
            groupBox.Width = 297;
            groupBox.Height = Height;
            Control.Controls.Add(groupBox);
        }

        protected void ShowLabel(string text, ContentAlignment alignment, int width, int horizSpacing, int vertSpacing)
        {
            Label label = new Label();
            label.Location = new Point(horizSpacing, vertSpacing);
            label.Text = text;
            label.TextAlign = alignment;
            label.Width = width;
            label.Height = ControlHeight;
            groupBox.Controls.Add(label);
        }

        protected void ShowTextBox(string text, HorizontalAlignment alignment, int width, int horizSpacing, int vertSpacing)
        {
            TextBox textBox = new TextBox();
            textBox.Location = new Point(horizSpacing, vertSpacing);
            textBox.Text = text;
            textBox.TextAlign = alignment;
            textBox.Width = width;
            textBox.Height = ControlHeight;
            //textBox.Leave += new EventHandler(NumElementChanged);

            groupBox.Controls.Add(textBox);
        }

        protected void ShowVector3(string propertyName, Vector3 v3, int horizSpacing, int vertSpacing)
        {
            ShowLabel(propertyName, ContentAlignment.MiddleLeft, 60, horizSpacing, vertSpacing);

            ShowLabel("X", ContentAlignment.MiddleLeft, 10, horizSpacing + 70, vertSpacing);
            ShowTextBox(string.Format("{0:0.###}", v3.x), HorizontalAlignment.Right, 57, horizSpacing + 80, vertSpacing);

            ShowLabel("Y", ContentAlignment.MiddleLeft, 10, horizSpacing + 137, vertSpacing);
            ShowTextBox(string.Format("{0:0.###}", v3.y), HorizontalAlignment.Right, 57, horizSpacing + 147, vertSpacing);

            ShowLabel("Z", ContentAlignment.MiddleLeft, 10, horizSpacing + 204, vertSpacing);
            ShowTextBox(string.Format("{0:0.###}", v3.z), HorizontalAlignment.Right, 57, horizSpacing + 214, vertSpacing);
        }
    }
}