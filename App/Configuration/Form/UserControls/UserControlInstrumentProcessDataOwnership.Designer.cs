namespace Configuration
{
    partial class UserControlInstrumentProcessDataOwnership
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
            this.fpSpreadOwnership = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadRole_Sheet1 = new FarPoint.Win.Spread.SheetView();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadOwnership)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.fpSpreadOwnership);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 807);
            this.radPanel1.TabIndex = 0;
            // 
            // fpSpreadRole
            // 
            this.fpSpreadOwnership.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadOwnership.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadOwnership.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadOwnership.Location = new System.Drawing.Point(3, 33);
            this.fpSpreadOwnership.Name = "fpSpreadRole";
            this.fpSpreadOwnership.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadRole_Sheet1});
            this.fpSpreadOwnership.Size = new System.Drawing.Size(1069, 771);
            this.fpSpreadOwnership.TabIndex = 49;
            // 
            // fpSpreadRole_Sheet1
            // 
            this.fpSpreadRole_Sheet1.Reset();
            this.fpSpreadRole_Sheet1.SheetName = "Sheet1";
            // Formulas and custom names must be loaded with R1C1 reference style
            this.fpSpreadRole_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.R1C1;
            this.fpSpreadRole_Sheet1.ColumnFooterSheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadRole_Sheet1.ColumnFooterSheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadRole_Sheet1.ColumnFooterSheetCornerStyle.Parent = "CornerFooterDefaultEnhanced";
            this.fpSpreadRole_Sheet1.ColumnFooterSheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadRole_Sheet1.FilterBarHeaderStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadRole_Sheet1.FilterBarHeaderStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadRole_Sheet1.FilterBarHeaderStyle.Parent = "RowHeaderDefaultEnhanced";
            this.fpSpreadRole_Sheet1.FilterBarHeaderStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadRole_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.A1;
            // 
            // UserControlInstrumentProcessDataOwnership
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlInstrumentProcessDataOwnership";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadOwnership)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private FarPoint.Win.Spread.FpSpread fpSpreadOwnership;
        private FarPoint.Win.Spread.SheetView fpSpreadRole_Sheet1;
    }
}
