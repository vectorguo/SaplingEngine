
namespace SaplingEngineEditor
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.DirectXPanel = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // DirectXPanel
            // 
            this.DirectXPanel.Location = new System.Drawing.Point(300, 15);
            this.DirectXPanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.DirectXPanel.Name = "DirectXPanel";
            this.DirectXPanel.Size = new System.Drawing.Size(1390, 988);
            this.DirectXPanel.TabIndex = 0;
            this.DirectXPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.DirectXPanel_Paint);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1896, 1022);
            this.Controls.Add(this.DirectXPanel);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "MainWindow";
            this.Text = "MainWindow";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel DirectXPanel;
    }
}