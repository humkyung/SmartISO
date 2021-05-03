namespace Configuration
{
    partial class UserControlCommonProjectSetting_EDB
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
            this.radButtonAttributeSave = new Telerik.WinControls.UI.RadButton();
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadData_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.labelTools = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAttributeSave)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radButtonAttributeSave);
            this.radPanel1.Controls.Add(this.radButtonDelete);
            this.radPanel1.Controls.Add(this.radButtonCreate);
            this.radPanel1.Controls.Add(this.fpSpreadData);
            this.radPanel1.Controls.Add(this.labelTools);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(799, 247);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radButtonAttributeSave
            // 
            this.radButtonAttributeSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAttributeSave.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAttributeSave.Image = global::Configuration.Properties.Resources.SmallSave;
            this.radButtonAttributeSave.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonAttributeSave.Location = new System.Drawing.Point(764, 4);
            this.radButtonAttributeSave.Name = "radButtonAttributeSave";
            this.radButtonAttributeSave.Size = new System.Drawing.Size(24, 24);
            this.radButtonAttributeSave.TabIndex = 44;
            this.radButtonAttributeSave.ThemeName = "Office2010Silver";
            this.radButtonAttributeSave.Click += new System.EventHandler(this.radButtonAttributeSave_Click);
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(737, 4);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 45;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Visible = false;
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(709, 4);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 24);
            this.radButtonCreate.TabIndex = 43;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Visible = false;
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData.Location = new System.Drawing.Point(9, 32);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadData_Sheet1});
            this.fpSpreadData.Size = new System.Drawing.Size(780, 206);
            this.fpSpreadData.TabIndex = 18;
            this.fpSpreadData.Change += new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadData_Change);
            // 
            // fpSpreadData_Sheet1
            // 
            this.fpSpreadData_Sheet1.Reset();
            this.fpSpreadData_Sheet1.SheetName = "Sheet1";
            // 
            // labelTools
            // 
            this.labelTools.AutoSize = true;
            this.labelTools.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.labelTools.Location = new System.Drawing.Point(11, 10);
            this.labelTools.Name = "labelTools";
            this.labelTools.Size = new System.Drawing.Size(54, 13);
            this.labelTools.TabIndex = 17;
            this.labelTools.Text = "SPTools : ";
            // 
            // UserControlCommonProjectSetting_EDB
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonProjectSetting_EDB";
            this.Size = new System.Drawing.Size(799, 247);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAttributeSave)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private System.Windows.Forms.Label labelTools;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private FarPoint.Win.Spread.SheetView fpSpreadData_Sheet1;
        private Telerik.WinControls.UI.RadButton radButtonAttributeSave;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
    }
}
