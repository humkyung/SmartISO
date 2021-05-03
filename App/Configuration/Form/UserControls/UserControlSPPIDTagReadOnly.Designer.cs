namespace Configuration
{
    partial class UserControlSPPIDTagReadOnly : UserControlBase
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.kRadButtonGenerate = new TechSun.Theme.KRadButton();
            this.comboTreeBoxItemType = new Configuration.UserControlComboTreeBox();
            this.radSplitContainer1 = new Telerik.WinControls.UI.RadSplitContainer();
            this.splitPanel1 = new Telerik.WinControls.UI.SplitPanel();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.fpSpreadTagReadOnlySetting = new FarPoint.Win.Spread.FpSpread();
            this.radButtonDown = new Telerik.WinControls.UI.RadButton();
            this.radButtonUp = new Telerik.WinControls.UI.RadButton();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.splitPanel2 = new Telerik.WinControls.UI.SplitPanel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.textBoxXMLPath = new System.Windows.Forms.TextBox();
            this.radSplitContainerTagRule = new Telerik.WinControls.UI.RadSplitContainer();
            this.radDropDownListProjectList = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonGenerate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).BeginInit();
            this.radSplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).BeginInit();
            this.splitPanel1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadTagReadOnlySetting)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).BeginInit();
            this.splitPanel2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.kRadButtonGenerate);
            this.radPanel1.Controls.Add(this.comboTreeBoxItemType);
            this.radPanel1.Controls.Add(this.radSplitContainer1);
            this.radPanel1.Controls.Add(this.radDropDownListProjectList);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1237, 715);
            this.radPanel1.TabIndex = 55;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // kRadButtonGenerate
            // 
            this.kRadButtonGenerate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.kRadButtonGenerate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonGenerate.Location = new System.Drawing.Point(1148, 9);
            this.kRadButtonGenerate.Name = "kRadButtonGenerate";
            this.kRadButtonGenerate.Size = new System.Drawing.Size(80, 26);
            this.kRadButtonGenerate.TabIndex = 68;
            this.kRadButtonGenerate.Text = "Generate";
            this.kRadButtonGenerate.ThemeName = "Office2013Light";
            this.kRadButtonGenerate.Click += new System.EventHandler(this.kRadButtonGenerate_Click);
            // 
            // comboTreeBoxItemType
            // 
            this.comboTreeBoxItemType.DropDownHeight = 80;
            this.comboTreeBoxItemType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.comboTreeBoxItemType.Location = new System.Drawing.Point(415, 8);
            this.comboTreeBoxItemType.Name = "comboTreeBoxItemType";
            this.comboTreeBoxItemType.Size = new System.Drawing.Size(246, 21);
            this.comboTreeBoxItemType.TabIndex = 51;
            // 
            // radSplitContainer1
            // 
            this.radSplitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radSplitContainer1.Controls.Add(this.splitPanel1);
            this.radSplitContainer1.Controls.Add(this.splitPanel2);
            this.radSplitContainer1.Location = new System.Drawing.Point(12, 47);
            this.radSplitContainer1.Name = "radSplitContainer1";
            this.radSplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // 
            // 
            this.radSplitContainer1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.radSplitContainer1.Size = new System.Drawing.Size(1216, 668);
            this.radSplitContainer1.SplitterWidth = 3;
            this.radSplitContainer1.TabIndex = 67;
            this.radSplitContainer1.TabStop = false;
            this.radSplitContainer1.Text = "radSplitContainer1";
            this.radSplitContainer1.ThemeName = "Office2010Silver";
            // 
            // splitPanel1
            // 
            this.splitPanel1.Controls.Add(this.groupBox2);
            this.splitPanel1.Location = new System.Drawing.Point(0, 0);
            this.splitPanel1.Name = "splitPanel1";
            // 
            // 
            // 
            this.splitPanel1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel1.Size = new System.Drawing.Size(1216, 309);
            this.splitPanel1.SizeInfo.AbsoluteSize = new System.Drawing.Size(200, 309);
            this.splitPanel1.SizeInfo.SizeMode = Telerik.WinControls.UI.Docking.SplitPanelSizeMode.Absolute;
            this.splitPanel1.SizeInfo.SplitterCorrection = new System.Drawing.Size(0, 109);
            this.splitPanel1.TabIndex = 0;
            this.splitPanel1.TabStop = false;
            this.splitPanel1.Text = "splitPanel1";
            this.splitPanel1.ThemeName = "Office2010Silver";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.fpSpreadTagReadOnlySetting);
            this.groupBox2.Controls.Add(this.radButtonDown);
            this.groupBox2.Controls.Add(this.radButtonUp);
            this.groupBox2.Controls.Add(this.radButtonCreate);
            this.groupBox2.Controls.Add(this.radButtonDelete);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox2.Font = new System.Drawing.Font("Segoe UI", 8.25F);
            this.groupBox2.Location = new System.Drawing.Point(0, 0);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(1216, 309);
            this.groupBox2.TabIndex = 54;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Attribute ReadOnly Setting";
            // 
            // fpSpreadTagReadOnlySetting
            // 
            this.fpSpreadTagReadOnlySetting.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadTagReadOnlySetting.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadTagReadOnlySetting.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadTagReadOnlySetting.Location = new System.Drawing.Point(6, 43);
            this.fpSpreadTagReadOnlySetting.Name = "fpSpreadTagReadOnlySetting";
            this.fpSpreadTagReadOnlySetting.Size = new System.Drawing.Size(1204, 260);
            this.fpSpreadTagReadOnlySetting.TabIndex = 70;
            this.fpSpreadTagReadOnlySetting.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadTagReadOnlySetting_CellClick);
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = global::Configuration.Properties.Resources.SmallDown;
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(1186, 13);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 24);
            this.radButtonDown.TabIndex = 66;
            this.radButtonDown.ThemeName = "Office2010Silver";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = global::Configuration.Properties.Resources.SmallUp;
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(1156, 13);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 24);
            this.radButtonUp.TabIndex = 65;
            this.radButtonUp.ThemeName = "Office2010Silver";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(1096, 13);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 24);
            this.radButtonCreate.TabIndex = 63;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(1126, 13);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 64;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // splitPanel2
            // 
            this.splitPanel2.Controls.Add(this.groupBox1);
            this.splitPanel2.Location = new System.Drawing.Point(0, 312);
            this.splitPanel2.Name = "splitPanel2";
            // 
            // 
            // 
            this.splitPanel2.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel2.Size = new System.Drawing.Size(1216, 356);
            this.splitPanel2.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(0F, -0.03269755F);
            this.splitPanel2.SizeInfo.SplitterCorrection = new System.Drawing.Size(0, -109);
            this.splitPanel2.TabIndex = 1;
            this.splitPanel2.TabStop = false;
            this.splitPanel2.Text = "splitPanel2";
            this.splitPanel2.ThemeName = "Office2010Silver";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.textBoxXMLPath);
            this.groupBox1.Controls.Add(this.radSplitContainerTagRule);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Font = new System.Drawing.Font("Segoe UI", 8.25F);
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1216, 356);
            this.groupBox1.TabIndex = 53;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "ItemTagFormat.xml";
            // 
            // textBoxXMLPath
            // 
            this.textBoxXMLPath.BackColor = System.Drawing.SystemColors.Control;
            this.textBoxXMLPath.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBoxXMLPath.Location = new System.Drawing.Point(6, 21);
            this.textBoxXMLPath.Name = "textBoxXMLPath";
            this.textBoxXMLPath.ReadOnly = true;
            this.textBoxXMLPath.Size = new System.Drawing.Size(877, 15);
            this.textBoxXMLPath.TabIndex = 52;
            this.textBoxXMLPath.Text = "File Path : -";
            // 
            // radSplitContainerTagRule
            // 
            this.radSplitContainerTagRule.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radSplitContainerTagRule.BackColor = System.Drawing.SystemColors.Control;
            this.radSplitContainerTagRule.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.radSplitContainerTagRule.Location = new System.Drawing.Point(6, 49);
            this.radSplitContainerTagRule.Name = "radSplitContainerTagRule";
            // 
            // 
            // 
            this.radSplitContainerTagRule.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.radSplitContainerTagRule.Size = new System.Drawing.Size(1204, 304);
            this.radSplitContainerTagRule.SplitterWidth = 3;
            this.radSplitContainerTagRule.TabIndex = 51;
            this.radSplitContainerTagRule.TabStop = false;
            this.radSplitContainerTagRule.Text = "radSplitContainer1";
            this.radSplitContainerTagRule.ThemeName = "Office2010Silver";
            // 
            // radDropDownListProjectList
            // 
            this.radDropDownListProjectList.DropDownHeight = 80;
            this.radDropDownListProjectList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectList.Location = new System.Drawing.Point(69, 8);
            this.radDropDownListProjectList.Name = "radDropDownListProjectList";
            this.radDropDownListProjectList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectList.TabIndex = 48;
            this.radDropDownListProjectList.ThemeName = "Office2010Silver";
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(48, 17);
            this.radLabel1.TabIndex = 49;
            this.radLabel1.Text = "Project :";
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(344, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(65, 17);
            this.radLabel2.TabIndex = 50;
            this.radLabel2.Text = "Item Type :";
            // 
            // UserControlSPPIDTagReadOnly
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlSPPIDTagReadOnly";
            this.Size = new System.Drawing.Size(1237, 715);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonGenerate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).EndInit();
            this.radSplitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).EndInit();
            this.splitPanel1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadTagReadOnlySetting)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).EndInit();
            this.splitPanel2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectList;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private UserControlComboTreeBox comboTreeBoxItemType;
        private Telerik.WinControls.UI.RadButton radButtonDown;
        private Telerik.WinControls.UI.RadButton radButtonUp;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainer1;
        private Telerik.WinControls.UI.SplitPanel splitPanel1;
        private System.Windows.Forms.GroupBox groupBox2;
        private FarPoint.Win.Spread.FpSpread fpSpreadTagReadOnlySetting;
        private Telerik.WinControls.UI.SplitPanel splitPanel2;
        private System.Windows.Forms.GroupBox groupBox1;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainerTagRule;
        private System.Windows.Forms.TextBox textBoxXMLPath;
        private TechSun.Theme.KRadButton kRadButtonGenerate;
    }
}
