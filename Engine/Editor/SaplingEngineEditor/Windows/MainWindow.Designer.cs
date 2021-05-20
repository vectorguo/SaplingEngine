
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
            this.components = new System.ComponentModel.Container();
            this.MainMenu = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSceneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openSceneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.TabControl1 = new System.Windows.Forms.TabControl();
            this.Hierarchy = new System.Windows.Forms.TabPage();
            this.SceneTreeView = new System.Windows.Forms.TreeView();
            this.TabControl2 = new System.Windows.Forms.TabControl();
            this.Inspector = new System.Windows.Forms.TabPage();
            this.DirectXPanel = new System.Windows.Forms.Panel();
            this.MainMenu.SuspendLayout();
            this.TabControl1.SuspendLayout();
            this.Hierarchy.SuspendLayout();
            this.TabControl2.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainMenu
            // 
            this.MainMenu.BackColor = System.Drawing.SystemColors.ControlDark;
            this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.MainMenu.Location = new System.Drawing.Point(0, 0);
            this.MainMenu.Name = "MainMenu";
            this.MainMenu.Size = new System.Drawing.Size(1563, 25);
            this.MainMenu.TabIndex = 1;
            this.MainMenu.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newSceneToolStripMenuItem,
            this.openSceneToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newSceneToolStripMenuItem
            // 
            this.newSceneToolStripMenuItem.Name = "newSceneToolStripMenuItem";
            this.newSceneToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.newSceneToolStripMenuItem.Text = "NewScene";
            // 
            // openSceneToolStripMenuItem
            // 
            this.openSceneToolStripMenuItem.Name = "openSceneToolStripMenuItem";
            this.openSceneToolStripMenuItem.Size = new System.Drawing.Size(142, 22);
            this.openSceneToolStripMenuItem.Text = "OpenScene";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(61, 4);
            // 
            // TabControl1
            // 
            this.TabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.TabControl1.Controls.Add(this.Hierarchy);
            this.TabControl1.Location = new System.Drawing.Point(0, 26);
            this.TabControl1.Name = "TabControl1";
            this.TabControl1.SelectedIndex = 0;
            this.TabControl1.Size = new System.Drawing.Size(299, 963);
            this.TabControl1.TabIndex = 2;
            // 
            // Hierarchy
            // 
            this.Hierarchy.Controls.Add(this.SceneTreeView);
            this.Hierarchy.Location = new System.Drawing.Point(4, 22);
            this.Hierarchy.Name = "Hierarchy";
            this.Hierarchy.Padding = new System.Windows.Forms.Padding(3);
            this.Hierarchy.Size = new System.Drawing.Size(291, 937);
            this.Hierarchy.TabIndex = 0;
            this.Hierarchy.Text = "Hierarchy";
            this.Hierarchy.UseVisualStyleBackColor = true;
            // 
            // SceneTreeView
            // 
            this.SceneTreeView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.SceneTreeView.Font = new System.Drawing.Font("Consolas", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SceneTreeView.Indent = 12;
            this.SceneTreeView.Location = new System.Drawing.Point(0, 0);
            this.SceneTreeView.Margin = new System.Windows.Forms.Padding(0);
            this.SceneTreeView.Name = "SceneTreeView";
            this.SceneTreeView.Size = new System.Drawing.Size(291, 937);
            this.SceneTreeView.TabIndex = 0;
            // 
            // TabControl2
            // 
            this.TabControl2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TabControl2.Controls.Add(this.Inspector);
            this.TabControl2.Location = new System.Drawing.Point(1257, 26);
            this.TabControl2.Name = "TabControl2";
            this.TabControl2.SelectedIndex = 0;
            this.TabControl2.Size = new System.Drawing.Size(308, 963);
            this.TabControl2.TabIndex = 3;
            // 
            // Inspector
            // 
            this.Inspector.Location = new System.Drawing.Point(4, 22);
            this.Inspector.Name = "Inspector";
            this.Inspector.Padding = new System.Windows.Forms.Padding(3);
            this.Inspector.Size = new System.Drawing.Size(300, 937);
            this.Inspector.TabIndex = 0;
            this.Inspector.Text = "Inspector";
            this.Inspector.UseVisualStyleBackColor = true;
            // 
            // DirectXPanel
            // 
            this.DirectXPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.DirectXPanel.Location = new System.Drawing.Point(296, 26);
            this.DirectXPanel.Name = "DirectXPanel";
            this.DirectXPanel.Size = new System.Drawing.Size(960, 959);
            this.DirectXPanel.TabIndex = 4;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.ClientSize = new System.Drawing.Size(1563, 987);
            this.Controls.Add(this.DirectXPanel);
            this.Controls.Add(this.TabControl2);
            this.Controls.Add(this.TabControl1);
            this.Controls.Add(this.MainMenu);
            this.MainMenuStrip = this.MainMenu;
            this.MaximumSize = new System.Drawing.Size(1579, 1026);
            this.MinimumSize = new System.Drawing.Size(1579, 1026);
            this.Name = "MainWindow";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MainWindow";
            this.MainMenu.ResumeLayout(false);
            this.MainMenu.PerformLayout();
            this.TabControl1.ResumeLayout(false);
            this.Hierarchy.ResumeLayout(false);
            this.TabControl2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip MainMenu;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem newSceneToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openSceneToolStripMenuItem;
        private System.Windows.Forms.TabControl TabControl1;
        private System.Windows.Forms.TabPage Hierarchy;
        private System.Windows.Forms.TabControl TabControl2;
        private System.Windows.Forms.TabPage Inspector;
        private System.Windows.Forms.TreeView SceneTreeView;
        private System.Windows.Forms.Panel DirectXPanel;
    }
}