namespace Configuration
{
    partial class UserControlSPPIDInstrumentCompareItemSetting
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
            this.radLabel3 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonCheck = new Telerik.WinControls.UI.RadButton();
            this.radBrowseEditorBackupPath = new Telerik.WinControls.UI.RadBrowseEditor();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadData_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProjectList = new Telerik.WinControls.UI.RadDropDownList();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCheck)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radBrowseEditorBackupPath)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radLabel3);
            this.radPanel1.Controls.Add(this.radButtonCheck);
            this.radPanel1.Controls.Add(this.radBrowseEditorBackupPath);
            this.radPanel1.Controls.Add(this.fpSpreadData);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radDropDownListProjectList);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1008, 685);
            this.radPanel1.TabIndex = 1;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radLabel3
            // 
            this.radLabel3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radLabel3.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel3.Location = new System.Drawing.Point(818, 9);
            this.radLabel3.Name = "radLabel3";
            this.radLabel3.Size = new System.Drawing.Size(88, 17);
            this.radLabel3.TabIndex = 64;
            this.radLabel3.Text = "Default Setting :";
            // 
            // radButtonCheck
            // 
            this.radButtonCheck.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCheck.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCheck.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCheck.Location = new System.Drawing.Point(912, 6);
            this.radButtonCheck.Name = "radButtonCheck";
            this.radButtonCheck.Size = new System.Drawing.Size(90, 23);
            this.radButtonCheck.TabIndex = 63;
            this.radButtonCheck.Text = "Setting";
            this.radButtonCheck.ThemeName = "Office2010Silver";
            this.radButtonCheck.Click += new System.EventHandler(this.radButtonCheck_Click);
            // 
            // radBrowseEditorBackupPath
            // 
            this.radBrowseEditorBackupPath.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radBrowseEditorBackupPath.DialogType = Telerik.WinControls.UI.BrowseEditorDialogType.FolderBrowseDialog;
            this.radBrowseEditorBackupPath.Enabled = false;
            this.radBrowseEditorBackupPath.Location = new System.Drawing.Point(12, 38);
            this.radBrowseEditorBackupPath.Name = "radBrowseEditorBackupPath";
            this.radBrowseEditorBackupPath.Size = new System.Drawing.Size(990, 20);
            this.radBrowseEditorBackupPath.TabIndex = 52;
            this.radBrowseEditorBackupPath.ThemeName = "Office2010Silver";
            ((Telerik.WinControls.UI.RadBrowseEditorElement)(this.radBrowseEditorBackupPath.GetChildAt(0))).Text = "(none)";
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radBrowseEditorBackupPath.GetChildAt(0).GetChildAt(1))).Shape = null;
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadData.Location = new System.Drawing.Point(12, 64);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadData_Sheet1});
            this.fpSpreadData.Size = new System.Drawing.Size(990, 612);
            this.fpSpreadData.TabIndex = 48;
            // 
            // fpSpreadData_Sheet1
            // 
            this.fpSpreadData_Sheet1.Reset();
            this.fpSpreadData_Sheet1.SheetName = "Sheet1";
            // Formulas and custom names must be loaded with R1C1 reference style
            this.fpSpreadData_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.R1C1;
            this.fpSpreadData_Sheet1.ColumnFooterSheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadData_Sheet1.ColumnFooterSheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadData_Sheet1.ColumnFooterSheetCornerStyle.Parent = "CornerFooterDefaultEnhanced";
            this.fpSpreadData_Sheet1.ColumnFooterSheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadData_Sheet1.FilterBarHeaderStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadData_Sheet1.FilterBarHeaderStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadData_Sheet1.FilterBarHeaderStyle.Parent = "RowHeaderDefaultEnhanced";
            this.fpSpreadData_Sheet1.FilterBarHeaderStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadData_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.A1;
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(48, 17);
            this.radLabel1.TabIndex = 47;
            this.radLabel1.Text = "Project :";
            // 
            // radDropDownListProjectList
            // 
            this.radDropDownListProjectList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectList.Location = new System.Drawing.Point(97, 8);
            this.radDropDownListProjectList.Name = "radDropDownListProjectList";
            this.radDropDownListProjectList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectList.TabIndex = 46;
            this.radDropDownListProjectList.ThemeName = "Office2010Silver";
            // 
            // UserControlSPPIDInstrumentCompareItemSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlSPPIDInstrumentCompareItemSetting";
            this.Size = new System.Drawing.Size(1008, 685);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCheck)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radBrowseEditorBackupPath)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabel3;
        private Telerik.WinControls.UI.RadButton radButtonCheck;
        private Telerik.WinControls.UI.RadBrowseEditor radBrowseEditorBackupPath;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private FarPoint.Win.Spread.SheetView fpSpreadData_Sheet1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectList;

    }
}
