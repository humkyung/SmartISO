namespace Configuration
{
    partial class FrmMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.radCommandBar1 = new Telerik.WinControls.UI.RadCommandBar();
            this.commandBarRowElement1 = new Telerik.WinControls.UI.CommandBarRowElement();
            this.commandBarStripElement1 = new Telerik.WinControls.UI.CommandBarStripElement();
            this.commandBarButtonSaveData = new Telerik.WinControls.UI.CommandBarButton();
            this.commandBarButtonExportExcel = new Telerik.WinControls.UI.CommandBarButton();
            this.radSplitContainer1 = new Telerik.WinControls.UI.RadSplitContainer();
            this.splitPanel1 = new Telerik.WinControls.UI.SplitPanel();
            this.radTreeViewMenu = new Telerik.WinControls.UI.RadTreeView();
            this.splitPanel2 = new Telerik.WinControls.UI.SplitPanel();
            this.radPanelUserControl = new Telerik.WinControls.UI.RadPanel();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.windows7Theme1 = new Telerik.WinControls.Themes.Windows7Theme();
            this.radMenuItem1 = new Telerik.WinControls.UI.RadMenuItem();
            this.radMenuItemSA = new Telerik.WinControls.UI.RadMenuItem();
            this.radMenuItemExit = new Telerik.WinControls.UI.RadMenuItem();
            this.radMenuMain = new Telerik.WinControls.UI.RadMenu();
            ((System.ComponentModel.ISupportInitialize)(this.radCommandBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).BeginInit();
            this.radSplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).BeginInit();
            this.splitPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radTreeViewMenu)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).BeginInit();
            this.splitPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelUserControl)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radMenuMain)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radCommandBar1
            // 
            this.radCommandBar1.AutoSize = true;
            this.radCommandBar1.Dock = System.Windows.Forms.DockStyle.Top;
            this.radCommandBar1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radCommandBar1.Location = new System.Drawing.Point(0, 19);
            this.radCommandBar1.Name = "radCommandBar1";
            this.radCommandBar1.Rows.AddRange(new Telerik.WinControls.UI.CommandBarRowElement[] {
            this.commandBarRowElement1});
            this.radCommandBar1.Size = new System.Drawing.Size(1341, 30);
            this.radCommandBar1.TabIndex = 0;
            this.radCommandBar1.ThemeName = "Windows8";
            // 
            // commandBarRowElement1
            // 
            this.commandBarRowElement1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(246)))), ((int)(((byte)(247)))), ((int)(((byte)(248)))));
            this.commandBarRowElement1.DisplayName = null;
            this.commandBarRowElement1.MinSize = new System.Drawing.Size(25, 25);
            this.commandBarRowElement1.Strips.AddRange(new Telerik.WinControls.UI.CommandBarStripElement[] {
            this.commandBarStripElement1});
            // 
            // commandBarStripElement1
            // 
            this.commandBarStripElement1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(246)))), ((int)(((byte)(247)))), ((int)(((byte)(248)))));
            this.commandBarStripElement1.DisplayName = "commandBarStripElement1";
            this.commandBarStripElement1.FloatingForm = null;
            this.commandBarStripElement1.Items.AddRange(new Telerik.WinControls.UI.RadCommandBarBaseItem[] {
            this.commandBarButtonSaveData,
            this.commandBarButtonExportExcel});
            this.commandBarStripElement1.Name = "commandBarStripElement1";
            this.commandBarStripElement1.StretchHorizontally = false;
            this.commandBarStripElement1.Text = "";
            // 
            // commandBarButtonSaveData
            // 
            this.commandBarButtonSaveData.AccessibleDescription = "Save";
            this.commandBarButtonSaveData.AccessibleName = "Save";
            this.commandBarButtonSaveData.DisplayName = "commandBarButton1";
            this.commandBarButtonSaveData.Image = global::Configuration.Properties.Resources.disk;
            this.commandBarButtonSaveData.Name = "commandBarButtonSaveData";
            this.commandBarButtonSaveData.StretchHorizontally = false;
            this.commandBarButtonSaveData.Text = "Save";
            this.commandBarButtonSaveData.Visibility = Telerik.WinControls.ElementVisibility.Visible;
            this.commandBarButtonSaveData.Click += new System.EventHandler(this.commandBarButtonSaveData_Click);
            // 
            // commandBarButtonExportExcel
            // 
            this.commandBarButtonExportExcel.AccessibleDescription = "ExportExcel";
            this.commandBarButtonExportExcel.AccessibleName = "ExportExcel";
            this.commandBarButtonExportExcel.DisplayName = "commandBarButton2";
            this.commandBarButtonExportExcel.Image = global::Configuration.Properties.Resources.excel;
            this.commandBarButtonExportExcel.Margin = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.commandBarButtonExportExcel.Name = "commandBarButtonExportExcel";
            this.commandBarButtonExportExcel.StretchHorizontally = false;
            this.commandBarButtonExportExcel.Text = "Export Excel";
            this.commandBarButtonExportExcel.Visibility = Telerik.WinControls.ElementVisibility.Visible;
            this.commandBarButtonExportExcel.Click += new System.EventHandler(this.commandBarButtonExportExcel_Click);
            // 
            // radSplitContainer1
            // 
            this.radSplitContainer1.Controls.Add(this.splitPanel1);
            this.radSplitContainer1.Controls.Add(this.splitPanel2);
            this.radSplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radSplitContainer1.Location = new System.Drawing.Point(0, 49);
            this.radSplitContainer1.Name = "radSplitContainer1";
            // 
            // 
            // 
            this.radSplitContainer1.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.radSplitContainer1.Size = new System.Drawing.Size(1341, 801);
            this.radSplitContainer1.TabIndex = 1;
            this.radSplitContainer1.TabStop = false;
            this.radSplitContainer1.Text = "radSplitContainer1";
            this.radSplitContainer1.ThemeName = "Office2010Silver";
            // 
            // splitPanel1
            // 
            this.splitPanel1.Controls.Add(this.radTreeViewMenu);
            this.splitPanel1.Location = new System.Drawing.Point(0, 0);
            this.splitPanel1.Name = "splitPanel1";
            // 
            // 
            // 
            this.splitPanel1.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.splitPanel1.Size = new System.Drawing.Size(253, 801);
            this.splitPanel1.SizeInfo.AbsoluteSize = new System.Drawing.Size(253, 200);
            this.splitPanel1.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(-0.1666667F, 0F);
            this.splitPanel1.SizeInfo.SizeMode = Telerik.WinControls.UI.Docking.SplitPanelSizeMode.Absolute;
            this.splitPanel1.SizeInfo.SplitterCorrection = new System.Drawing.Size(-155, 0);
            this.splitPanel1.TabIndex = 0;
            this.splitPanel1.TabStop = false;
            this.splitPanel1.Text = "splitPanel1";
            this.splitPanel1.ThemeName = "Office2010Silver";
            // 
            // radTreeViewMenu
            // 
            this.radTreeViewMenu.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radTreeViewMenu.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTreeViewMenu.Location = new System.Drawing.Point(0, 0);
            this.radTreeViewMenu.Name = "radTreeViewMenu";
            this.radTreeViewMenu.ShowLines = true;
            this.radTreeViewMenu.Size = new System.Drawing.Size(253, 801);
            this.radTreeViewMenu.SpacingBetweenNodes = -1;
            this.radTreeViewMenu.TabIndex = 0;
            this.radTreeViewMenu.Text = "radTreeView1";
            this.radTreeViewMenu.ThemeName = "Windows8";
            this.radTreeViewMenu.SelectedNodeChanged += new Telerik.WinControls.UI.RadTreeView.RadTreeViewEventHandler(this.radTreeViewMenu_SelectedNodeChanged);
            // 
            // splitPanel2
            // 
            this.splitPanel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(241)))), ((int)(((byte)(242)))), ((int)(((byte)(243)))));
            this.splitPanel2.Controls.Add(this.radPanelUserControl);
            this.splitPanel2.Location = new System.Drawing.Point(256, 0);
            this.splitPanel2.Name = "splitPanel2";
            // 
            // 
            // 
            this.splitPanel2.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.splitPanel2.Size = new System.Drawing.Size(1085, 801);
            this.splitPanel2.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(0.02167761F, 0F);
            this.splitPanel2.SizeInfo.SplitterCorrection = new System.Drawing.Size(155, 0);
            this.splitPanel2.TabIndex = 1;
            this.splitPanel2.TabStop = false;
            this.splitPanel2.Text = "splitPanel2";
            this.splitPanel2.ThemeName = "Office2010Silver";
            // 
            // radPanelUserControl
            // 
            this.radPanelUserControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanelUserControl.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanelUserControl.Location = new System.Drawing.Point(0, 0);
            this.radPanelUserControl.Name = "radPanelUserControl";
            this.radPanelUserControl.Size = new System.Drawing.Size(1085, 801);
            this.radPanelUserControl.TabIndex = 0;
            this.radPanelUserControl.ThemeName = "Windows8";
            // 
            // radMenuItem1
            // 
            this.radMenuItem1.AccessibleDescription = "File";
            this.radMenuItem1.AccessibleName = "File";
            this.radMenuItem1.Items.AddRange(new Telerik.WinControls.RadItem[] {
            this.radMenuItemSA,
            this.radMenuItemExit});
            this.radMenuItem1.Name = "radMenuItem1";
            this.radMenuItem1.Text = "File";
            this.radMenuItem1.Visibility = Telerik.WinControls.ElementVisibility.Visible;
            // 
            // radMenuItemSA
            // 
            this.radMenuItemSA.AccessibleDescription = "SA";
            this.radMenuItemSA.AccessibleName = "SA";
            this.radMenuItemSA.DescriptionFont = new System.Drawing.Font("Tahoma", 8.25F);
            this.radMenuItemSA.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radMenuItemSA.Name = "radMenuItemSA";
            this.radMenuItemSA.Text = "SA";
            this.radMenuItemSA.Visibility = Telerik.WinControls.ElementVisibility.Visible;
            this.radMenuItemSA.Click += new System.EventHandler(this.radMenuItemSA_Click);
            // 
            // radMenuItemExit
            // 
            this.radMenuItemExit.AccessibleDescription = "Exit";
            this.radMenuItemExit.AccessibleName = "Exit";
            this.radMenuItemExit.DescriptionFont = new System.Drawing.Font("Tahoma", 8.25F);
            this.radMenuItemExit.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radMenuItemExit.Name = "radMenuItemExit";
            this.radMenuItemExit.Text = "Exit";
            this.radMenuItemExit.Visibility = Telerik.WinControls.ElementVisibility.Visible;
            this.radMenuItemExit.Click += new System.EventHandler(this.radMenuItemExit_Click);
            // 
            // radMenuMain
            // 
            this.radMenuMain.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radMenuMain.Items.AddRange(new Telerik.WinControls.RadItem[] {
            this.radMenuItem1});
            this.radMenuMain.Location = new System.Drawing.Point(0, 0);
            this.radMenuMain.Name = "radMenuMain";
            this.radMenuMain.Size = new System.Drawing.Size(1341, 19);
            this.radMenuMain.TabIndex = 2;
            this.radMenuMain.ThemeName = "Windows8";
            // 
            // FrmMain
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(1341, 850);
            this.Controls.Add(this.radSplitContainer1);
            this.Controls.Add(this.radCommandBar1);
            this.Controls.Add(this.radMenuMain);
            this.Font = new System.Drawing.Font("Tahoma", 9F);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmMain";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "   Configuration";
            this.ThemeName = "Windows8";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FrmMain_FormClosed);
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.Shown += new System.EventHandler(this.FrmMain_Shown);
            ((System.ComponentModel.ISupportInitialize)(this.radCommandBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).EndInit();
            this.radSplitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).EndInit();
            this.splitPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radTreeViewMenu)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).EndInit();
            this.splitPanel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radPanelUserControl)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radMenuMain)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadCommandBar radCommandBar1;
        private Telerik.WinControls.UI.CommandBarRowElement commandBarRowElement1;
        private Telerik.WinControls.UI.CommandBarStripElement commandBarStripElement1;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainer1;
        private Telerik.WinControls.UI.SplitPanel splitPanel1;
        public Telerik.WinControls.UI.RadTreeView radTreeViewMenu;
        private Telerik.WinControls.UI.SplitPanel splitPanel2;
        private Telerik.WinControls.UI.CommandBarButton commandBarButtonSaveData;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
        private Telerik.WinControls.UI.CommandBarButton commandBarButtonExportExcel;
        private Telerik.WinControls.UI.RadPanel radPanelUserControl;
        private Telerik.WinControls.UI.RadMenuItem radMenuItem1;
        private Telerik.WinControls.UI.RadMenuItem radMenuItemSA;
        private Telerik.WinControls.UI.RadMenuItem radMenuItemExit;
        private Telerik.WinControls.UI.RadMenu radMenuMain;

    }
}
