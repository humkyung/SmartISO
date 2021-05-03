namespace Configuration
{
    partial class UserControlInstrumentDDPInhouseData
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
            this.radDropDownList3DModel = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonImport = new Telerik.WinControls.UI.RadButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadData_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.fpSpreadExcel = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadExcel_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.windows8Theme1 = new Telerik.WinControls.Themes.Windows8Theme();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownList3DModel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonImport)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadExcel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadExcel_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radDropDownList3DModel);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radButtonImport);
            this.radPanel1.Controls.Add(this.groupBox1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1009, 807);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radDropDownList3DModel
            // 
            this.radDropDownList3DModel.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownList3DModel.Location = new System.Drawing.Point(80, 8);
            this.radDropDownList3DModel.Name = "radDropDownList3DModel";
            this.radDropDownList3DModel.Size = new System.Drawing.Size(125, 20);
            this.radDropDownList3DModel.TabIndex = 53;
            this.radDropDownList3DModel.Text = "radDropDownList1";
            this.radDropDownList3DModel.ThemeName = "Windows8";
            this.radDropDownList3DModel.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListType3D_SelectedIndexChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(47, 18);
            this.radLabel1.TabIndex = 52;
            this.radLabel1.Text = "Type3D:";
            // 
            // radButtonImport
            // 
            this.radButtonImport.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonImport.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonImport.Location = new System.Drawing.Point(232, 6);
            this.radButtonImport.Name = "radButtonImport";
            this.radButtonImport.Size = new System.Drawing.Size(78, 24);
            this.radButtonImport.TabIndex = 51;
            this.radButtonImport.Text = "Import";
            this.radButtonImport.ThemeName = "Office2010Silver";
            this.radButtonImport.Click += new System.EventHandler(this.radButtonImport_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.fpSpreadData);
            this.groupBox1.Controls.Add(this.fpSpreadExcel);
            this.groupBox1.Location = new System.Drawing.Point(12, 33);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(983, 763);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "In-house DDP";
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fpSpreadData.Location = new System.Drawing.Point(3, 18);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadData_Sheet1});
            this.fpSpreadData.Size = new System.Drawing.Size(977, 742);
            this.fpSpreadData.TabIndex = 0;
            this.fpSpreadData.TabStripInsertTab = false;
            // 
            // fpSpreadData_Sheet1
            // 
            this.fpSpreadData_Sheet1.Reset();
            this.fpSpreadData_Sheet1.SheetName = "Sheet1";
            // 
            // fpSpreadExcel
            // 
            this.fpSpreadExcel.AccessibleDescription = "";
            this.fpSpreadExcel.Location = new System.Drawing.Point(321, 199);
            this.fpSpreadExcel.Name = "fpSpreadExcel";
            this.fpSpreadExcel.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadExcel_Sheet1});
            this.fpSpreadExcel.Size = new System.Drawing.Size(200, 100);
            this.fpSpreadExcel.TabIndex = 1;
            this.fpSpreadExcel.Visible = false;
            // 
            // fpSpreadExcel_Sheet1
            // 
            this.fpSpreadExcel_Sheet1.Reset();
            this.fpSpreadExcel_Sheet1.SheetName = "Sheet1";
            // 
            // UserControlInstrumentDDPInhouseData
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlInstrumentDDPInhouseData";
            this.Size = new System.Drawing.Size(1009, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownList3DModel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonImport)).EndInit();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadExcel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadExcel_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private FarPoint.Win.Spread.SheetView fpSpreadData_Sheet1;
        private Telerik.WinControls.UI.RadButton radButtonImport;
        private FarPoint.Win.Spread.FpSpread fpSpreadExcel;
        private FarPoint.Win.Spread.SheetView fpSpreadExcel_Sheet1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownList3DModel;
        private Telerik.WinControls.Themes.Windows8Theme windows8Theme1;
    }
}
