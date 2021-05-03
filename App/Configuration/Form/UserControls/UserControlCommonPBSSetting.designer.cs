namespace Configuration
{
    partial class UserControlCommonPBSSetting
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProject = new Telerik.WinControls.UI.RadDropDownList();
            this.radButtonDown = new Telerik.WinControls.UI.RadButton();
            this.radButtonUp = new Telerik.WinControls.UI.RadButton();
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.fpSpreadPBSSetting = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadPBSSetting_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.windows8Theme1 = new Telerik.WinControls.Themes.Windows8Theme();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadPBSSetting)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadPBSSetting_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.radDropDownListProject);
            this.radPanel1.Controls.Add(this.radButtonDown);
            this.radPanel1.Controls.Add(this.radButtonUp);
            this.radPanel1.Controls.Add(this.radButtonDelete);
            this.radPanel1.Controls.Add(this.radButtonCreate);
            this.radPanel1.Controls.Add(this.fpSpreadPBSSetting);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 807);
            this.radPanel1.TabIndex = 1;
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(12, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(41, 17);
            this.radLabel2.TabIndex = 71;
            this.radLabel2.Text = "Project";
            // 
            // radDropDownListProject
            // 
            this.radDropDownListProject.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProject.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProject.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListProject.Name = "radDropDownListProject";
            this.radDropDownListProject.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProject.TabIndex = 70;
            this.radDropDownListProject.Text = "radDropDownList1";
            this.radDropDownListProject.ThemeName = "Windows8";
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = global::Configuration.Properties.Resources.SmallDown;
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(1048, 3);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 24);
            this.radButtonDown.TabIndex = 62;
            this.radButtonDown.ThemeName = "Office2010Silver";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = global::Configuration.Properties.Resources.SmallUp;
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(1018, 3);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 24);
            this.radButtonUp.TabIndex = 61;
            this.radButtonUp.ThemeName = "Office2010Silver";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(988, 3);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 60;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(958, 3);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 24);
            this.radButtonCreate.TabIndex = 59;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // fpSpreadPBSSetting
            // 
            this.fpSpreadPBSSetting.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadPBSSetting.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadPBSSetting.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadPBSSetting.Location = new System.Drawing.Point(3, 32);
            this.fpSpreadPBSSetting.Name = "fpSpreadPBSSetting";
            this.fpSpreadPBSSetting.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadPBSSetting_Sheet1});
            this.fpSpreadPBSSetting.Size = new System.Drawing.Size(1069, 772);
            this.fpSpreadPBSSetting.TabIndex = 69;
            // 
            // fpSpreadPBSSetting_Sheet1
            // 
            this.fpSpreadPBSSetting_Sheet1.Reset();
            this.fpSpreadPBSSetting_Sheet1.SheetName = "Sheet1";
            // Formulas and custom names must be loaded with R1C1 reference style
            this.fpSpreadPBSSetting_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.R1C1;
            this.fpSpreadPBSSetting_Sheet1.ColumnFooterSheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadPBSSetting_Sheet1.ColumnFooterSheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadPBSSetting_Sheet1.ColumnFooterSheetCornerStyle.Parent = "CornerFooterDefaultEnhanced";
            this.fpSpreadPBSSetting_Sheet1.ColumnFooterSheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadPBSSetting_Sheet1.FilterBarHeaderStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadPBSSetting_Sheet1.FilterBarHeaderStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadPBSSetting_Sheet1.FilterBarHeaderStyle.Parent = "RowHeaderDefaultEnhanced";
            this.fpSpreadPBSSetting_Sheet1.FilterBarHeaderStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadPBSSetting_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.A1;
            // 
            // UserControlCommonPBSSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlCommonPBSSetting";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadPBSSetting)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadPBSSetting_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private FarPoint.Win.Spread.FpSpread fpSpreadPBSSetting;
        private FarPoint.Win.Spread.SheetView fpSpreadPBSSetting_Sheet1;
        private Telerik.WinControls.UI.RadButton radButtonDown;
        private Telerik.WinControls.UI.RadButton radButtonUp;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProject;
        private Telerik.WinControls.Themes.Windows8Theme windows8Theme1;
    }
}
