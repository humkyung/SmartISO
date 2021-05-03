namespace Configuration
{
    partial class UserControlCommonRole
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserControlCommonRole));
            this.radSplitContainer1 = new Telerik.WinControls.UI.RadSplitContainer();
            this.splitPanel1 = new Telerik.WinControls.UI.SplitPanel();
            this.radButtonDelete = new TechSun.Theme.KRadCloseImageButton();
            this.radButtonCreate = new TechSun.Theme.KRadPlusImageButton();
            this.radButtonDown = new TechSun.Theme.KRadDownImageButton();
            this.radButtonUp = new TechSun.Theme.KRadUpImageButton();
            this.fpSpreadRole = new FarPoint.Win.Spread.FpSpread();
            this.splitPanel2 = new Telerik.WinControls.UI.SplitPanel();
            this.radButtonDeleteDetail = new TechSun.Theme.KRadCloseImageButton();
            this.radButtonCreateDetail = new TechSun.Theme.KRadPlusImageButton();
            this.kRadButtonSaveRoleDetail = new TechSun.Theme.KRadButton();
            this.fpSpreadRoleDetail = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadRole_Sheet1 = new FarPoint.Win.Spread.SheetView();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).BeginInit();
            this.radSplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).BeginInit();
            this.splitPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).BeginInit();
            this.splitPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDeleteDetail)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreateDetail)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonSaveRoleDetail)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRoleDetail)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole_Sheet1)).BeginInit();
            this.SuspendLayout();
            // 
            // radSplitContainer1
            // 
            this.radSplitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radSplitContainer1.Controls.Add(this.splitPanel1);
            this.radSplitContainer1.Controls.Add(this.splitPanel2);
            this.radSplitContainer1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radSplitContainer1.Location = new System.Drawing.Point(0, 0);
            this.radSplitContainer1.Name = "radSplitContainer1";
            // 
            // 
            // 
            this.radSplitContainer1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.radSplitContainer1.Size = new System.Drawing.Size(1075, 807);
            this.radSplitContainer1.SplitterWidth = 3;
            this.radSplitContainer1.TabIndex = 1;
            this.radSplitContainer1.TabStop = false;
            this.radSplitContainer1.Text = "radSplitContainer1";
            this.radSplitContainer1.ThemeName = "Office2010Silver";
            // 
            // splitPanel1
            // 
            this.splitPanel1.Controls.Add(this.radButtonDelete);
            this.splitPanel1.Controls.Add(this.radButtonCreate);
            this.splitPanel1.Controls.Add(this.radButtonDown);
            this.splitPanel1.Controls.Add(this.radButtonUp);
            this.splitPanel1.Controls.Add(this.fpSpreadRole);
            this.splitPanel1.Location = new System.Drawing.Point(0, 0);
            this.splitPanel1.Name = "splitPanel1";
            // 
            // 
            // 
            this.splitPanel1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel1.Size = new System.Drawing.Size(481, 807);
            this.splitPanel1.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(-0.05130598F, 0F);
            this.splitPanel1.SizeInfo.SplitterCorrection = new System.Drawing.Size(-55, 0);
            this.splitPanel1.TabIndex = 0;
            this.splitPanel1.TabStop = false;
            this.splitPanel1.Text = "splitPanel1";
            this.splitPanel1.ThemeName = "Office2010Silver";
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDelete.Image")));
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(394, 4);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 26);
            this.radButtonDelete.TabIndex = 65;
            this.radButtonDelete.ThemeName = "Office2013Light";
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = ((System.Drawing.Image)(resources.GetObject("radButtonCreate.Image")));
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(364, 4);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 26);
            this.radButtonCreate.TabIndex = 64;
            this.radButtonCreate.ThemeName = "Office2013Light";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDown.Image")));
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(454, 4);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 26);
            this.radButtonDown.TabIndex = 63;
            this.radButtonDown.ThemeName = "Office2013Light";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = ((System.Drawing.Image)(resources.GetObject("radButtonUp.Image")));
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(424, 4);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 26);
            this.radButtonUp.TabIndex = 62;
            this.radButtonUp.ThemeName = "Office2013Light";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // fpSpreadRole
            // 
            this.fpSpreadRole.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadRole.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadRole.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadRole.Location = new System.Drawing.Point(3, 35);
            this.fpSpreadRole.Name = "fpSpreadRole";
            this.fpSpreadRole.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadRole_Sheet1});
            this.fpSpreadRole.Size = new System.Drawing.Size(475, 769);
            this.fpSpreadRole.TabIndex = 59;
            this.fpSpreadRole.Change += new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadRole_Change);
            this.fpSpreadRole.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadRole_CellClick);
            // 
            // splitPanel2
            // 
            this.splitPanel2.Controls.Add(this.radButtonDeleteDetail);
            this.splitPanel2.Controls.Add(this.radButtonCreateDetail);
            this.splitPanel2.Controls.Add(this.kRadButtonSaveRoleDetail);
            this.splitPanel2.Controls.Add(this.fpSpreadRoleDetail);
            this.splitPanel2.Location = new System.Drawing.Point(484, 0);
            this.splitPanel2.Name = "splitPanel2";
            // 
            // 
            // 
            this.splitPanel2.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel2.Size = new System.Drawing.Size(591, 807);
            this.splitPanel2.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(0.05130595F, 0F);
            this.splitPanel2.SizeInfo.SplitterCorrection = new System.Drawing.Size(55, 0);
            this.splitPanel2.TabIndex = 1;
            this.splitPanel2.TabStop = false;
            this.splitPanel2.Text = "splitPanel2";
            this.splitPanel2.ThemeName = "Office2010Silver";
            // 
            // radButtonDeleteDetail
            // 
            this.radButtonDeleteDetail.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDeleteDetail.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDeleteDetail.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDeleteDetail.Image")));
            this.radButtonDeleteDetail.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDeleteDetail.Location = new System.Drawing.Point(478, 4);
            this.radButtonDeleteDetail.Name = "radButtonDeleteDetail";
            this.radButtonDeleteDetail.Size = new System.Drawing.Size(24, 26);
            this.radButtonDeleteDetail.TabIndex = 67;
            this.radButtonDeleteDetail.ThemeName = "Office2013Light";
            this.radButtonDeleteDetail.Click += new System.EventHandler(this.radButtonDeleteDetail_Click);
            // 
            // radButtonCreateDetail
            // 
            this.radButtonCreateDetail.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreateDetail.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreateDetail.Image = ((System.Drawing.Image)(resources.GetObject("radButtonCreateDetail.Image")));
            this.radButtonCreateDetail.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreateDetail.Location = new System.Drawing.Point(448, 4);
            this.radButtonCreateDetail.Name = "radButtonCreateDetail";
            this.radButtonCreateDetail.Size = new System.Drawing.Size(24, 26);
            this.radButtonCreateDetail.TabIndex = 66;
            this.radButtonCreateDetail.ThemeName = "Office2013Light";
            this.radButtonCreateDetail.Click += new System.EventHandler(this.radButtonCreateDetail_Click);
            // 
            // kRadButtonSaveRoleDetail
            // 
            this.kRadButtonSaveRoleDetail.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.kRadButtonSaveRoleDetail.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(239)))), ((int)(((byte)(239)))), ((int)(((byte)(239)))));
            this.kRadButtonSaveRoleDetail.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonSaveRoleDetail.ForeColor = System.Drawing.Color.Black;
            this.kRadButtonSaveRoleDetail.Location = new System.Drawing.Point(508, 4);
            this.kRadButtonSaveRoleDetail.Name = "kRadButtonSaveRoleDetail";
            this.kRadButtonSaveRoleDetail.Size = new System.Drawing.Size(80, 26);
            this.kRadButtonSaveRoleDetail.TabIndex = 57;
            this.kRadButtonSaveRoleDetail.Text = "Save";
            this.kRadButtonSaveRoleDetail.ThemeName = "Office2013Light";
            this.kRadButtonSaveRoleDetail.Click += new System.EventHandler(this.kRadButtonSaveRoleDetail_Click);
            // 
            // fpSpreadRoleDetail
            // 
            this.fpSpreadRoleDetail.AccessibleDescription = "";
            this.fpSpreadRoleDetail.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadRoleDetail.Location = new System.Drawing.Point(3, 35);
            this.fpSpreadRoleDetail.Name = "fpSpreadRoleDetail";
            this.fpSpreadRoleDetail.Size = new System.Drawing.Size(585, 769);
            this.fpSpreadRoleDetail.TabIndex = 4;
            this.fpSpreadRoleDetail.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadRoleDetail_CellClick);
            // 
            // fpSpreadRole_Sheet1
            // 
            this.fpSpreadRole_Sheet1.Reset();
            this.fpSpreadRole_Sheet1.SheetName = "Sheet1";
            // 
            // UserControlCommonRole
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radSplitContainer1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlCommonRole";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).EndInit();
            this.radSplitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).EndInit();
            this.splitPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).EndInit();
            this.splitPanel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDeleteDetail)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreateDetail)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonSaveRoleDetail)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRoleDetail)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadRole_Sheet1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadSplitContainer radSplitContainer1;
        private Telerik.WinControls.UI.SplitPanel splitPanel1;
        private Telerik.WinControls.UI.SplitPanel splitPanel2;
        private TechSun.Theme.KRadDownImageButton radButtonDown;
        private TechSun.Theme.KRadUpImageButton radButtonUp;
        private FarPoint.Win.Spread.FpSpread fpSpreadRole;
        private FarPoint.Win.Spread.FpSpread fpSpreadRoleDetail;
        private TechSun.Theme.KRadButton kRadButtonSaveRoleDetail;
        private TechSun.Theme.KRadPlusImageButton radButtonCreate;
        private TechSun.Theme.KRadCloseImageButton radButtonDelete;
        private TechSun.Theme.KRadCloseImageButton radButtonDeleteDetail;
        private TechSun.Theme.KRadPlusImageButton radButtonCreateDetail;
        private FarPoint.Win.Spread.SheetView fpSpreadRole_Sheet1;
    }
}
