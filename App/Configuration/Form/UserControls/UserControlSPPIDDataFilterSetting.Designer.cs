namespace Configuration
{
    partial class UserControlSPPIDDataFilterSetting
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
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.radLabelSelectedDataFilterCount = new Telerik.WinControls.UI.RadLabel();
            this.comboTreeBoxItemType = new Configuration.UserControlComboTreeBox();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonDown = new Telerik.WinControls.UI.RadButton();
            this.radButtonUp = new Telerik.WinControls.UI.RadButton();
            this.fpSpreadDataFilter = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadDataFilter_Sheet1 = new FarPoint.Win.Spread.SheetView();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelSelectedDataFilterCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDataFilter)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDataFilter_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radButtonDelete);
            this.radPanel1.Controls.Add(this.radLabelSelectedDataFilterCount);
            this.radPanel1.Controls.Add(this.comboTreeBoxItemType);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.radButtonDown);
            this.radPanel1.Controls.Add(this.radButtonUp);
            this.radPanel1.Controls.Add(this.fpSpreadDataFilter);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 807);
            this.radPanel1.TabIndex = 1;
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(987, 5);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 63;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Visible = false;
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radLabelSelectedDataFilterCount
            // 
            this.radLabelSelectedDataFilterCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.radLabelSelectedDataFilterCount.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabelSelectedDataFilterCount.Location = new System.Drawing.Point(3, 787);
            this.radLabelSelectedDataFilterCount.Name = "radLabelSelectedDataFilterCount";
            this.radLabelSelectedDataFilterCount.Size = new System.Drawing.Size(68, 17);
            this.radLabelSelectedDataFilterCount.TabIndex = 62;
            this.radLabelSelectedDataFilterCount.Text = "Row Count :";
            // 
            // comboTreeBoxItemType
            // 
            this.comboTreeBoxItemType.DropDownHeight = 106;
            this.comboTreeBoxItemType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.comboTreeBoxItemType.Location = new System.Drawing.Point(80, 8);
            this.comboTreeBoxItemType.Name = "comboTreeBoxItemType";
            this.comboTreeBoxItemType.Size = new System.Drawing.Size(189, 21);
            this.comboTreeBoxItemType.TabIndex = 60;
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(12, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(65, 17);
            this.radLabel2.TabIndex = 59;
            this.radLabel2.Text = "Item Type :";
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = global::Configuration.Properties.Resources.SmallDown;
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(1048, 5);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 24);
            this.radButtonDown.TabIndex = 58;
            this.radButtonDown.ThemeName = "Office2010Silver";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = global::Configuration.Properties.Resources.SmallUp;
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(1018, 5);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 24);
            this.radButtonUp.TabIndex = 57;
            this.radButtonUp.ThemeName = "Office2010Silver";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // fpSpreadDataFilter
            // 
            this.fpSpreadDataFilter.AccessibleDescription = "fpSpreadDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadDataFilter.AllowDragDrop = true;
            this.fpSpreadDataFilter.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadDataFilter.ClipboardPasteToFill = true;
            this.fpSpreadDataFilter.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadDataFilter.Location = new System.Drawing.Point(3, 35);
            this.fpSpreadDataFilter.Name = "fpSpreadDataFilter";
            this.fpSpreadDataFilter.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadDataFilter_Sheet1});
            this.fpSpreadDataFilter.Size = new System.Drawing.Size(1069, 751);
            this.fpSpreadDataFilter.TabIndex = 49;
            this.fpSpreadDataFilter.Change += new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadDataFilter_Change);
            this.fpSpreadDataFilter.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadDataFilter_CellClick);
            // 
            // fpSpreadDataFilter_Sheet1
            // 
            this.fpSpreadDataFilter_Sheet1.Reset();
            this.fpSpreadDataFilter_Sheet1.SheetName = "Sheet1";
            // Formulas and custom names must be loaded with R1C1 reference style
            this.fpSpreadDataFilter_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.R1C1;
            this.fpSpreadDataFilter_Sheet1.ColumnFooter.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ColumnFooter.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.ColumnFooter.DefaultStyle.Parent = "ColumnHeaderDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.ColumnFooter.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ColumnFooterSheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ColumnFooterSheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.ColumnFooterSheetCornerStyle.Parent = "CornerFooterDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.ColumnFooterSheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ColumnHeader.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ColumnHeader.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.ColumnHeader.DefaultStyle.Parent = "ColumnHeaderDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.ColumnHeader.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.DefaultStyle.Parent = "DataAreaDefault";
            this.fpSpreadDataFilter_Sheet1.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.FilterBar.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.FilterBar.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.FilterBar.DefaultStyle.Parent = "FilterBarDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.FilterBar.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.FilterBarHeaderStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.FilterBarHeaderStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.FilterBarHeaderStyle.Parent = "RowHeaderDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.FilterBarHeaderStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.RowHeader.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.RowHeader.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.RowHeader.DefaultStyle.Parent = "RowHeaderDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.RowHeader.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.SheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.SheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadDataFilter_Sheet1.SheetCornerStyle.Parent = "CornerDefaultEnhanced";
            this.fpSpreadDataFilter_Sheet1.SheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadDataFilter_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.A1;
            // 
            // UserControlSPPIDDataFilterSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlSPPIDDataFilterSetting";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelSelectedDataFilterCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDataFilter)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDataFilter_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadButton radButtonDown;
        private Telerik.WinControls.UI.RadButton radButtonUp;
        private FarPoint.Win.Spread.FpSpread fpSpreadDataFilter;
        private FarPoint.Win.Spread.SheetView fpSpreadDataFilter_Sheet1;
        private UserControlComboTreeBox comboTreeBoxItemType;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadLabel radLabelSelectedDataFilterCount;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
    }
}
