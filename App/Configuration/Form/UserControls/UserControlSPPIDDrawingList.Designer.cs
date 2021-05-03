namespace Configuration
{
    partial class UserControlSPPIDDrawingList
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
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProjectList = new Telerik.WinControls.UI.RadDropDownList();
            this.radDropDownListPlant = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.fpErrorDrawingListSetting = new FarPoint.Win.Spread.FpSpread();
            this.radButtonDeleteAll = new Telerik.WinControls.UI.RadButton();
            this.radButtonAdd = new Telerik.WinControls.UI.RadButton();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListPlant)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpErrorDrawingListSetting)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDeleteAll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(48, 17);
            this.radLabel1.TabIndex = 11;
            this.radLabel1.Text = "Project :";
            // 
            // radDropDownListProjectList
            // 
            this.radDropDownListProjectList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectList.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListProjectList.Name = "radDropDownListProjectList";
            this.radDropDownListProjectList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectList.TabIndex = 10;
            this.radDropDownListProjectList.ThemeName = "Office2010Silver";
            // 
            // radDropDownListPlant
            // 
            this.radDropDownListPlant.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListPlant.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListPlant.Location = new System.Drawing.Point(80, 34);
            this.radDropDownListPlant.Name = "radDropDownListPlant";
            this.radDropDownListPlant.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListPlant.TabIndex = 10;
            this.radDropDownListPlant.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(12, 35);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(39, 17);
            this.radLabel2.TabIndex = 11;
            this.radLabel2.Text = "Plant :";
            // 
            // fpErrorDrawingListSetting
            // 
            this.fpErrorDrawingListSetting.AccessibleDescription = "";
            this.fpErrorDrawingListSetting.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpErrorDrawingListSetting.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.fpErrorDrawingListSetting.Location = new System.Drawing.Point(12, 59);
            this.fpErrorDrawingListSetting.Name = "fpErrorDrawingListSetting";
            this.fpErrorDrawingListSetting.Size = new System.Drawing.Size(1218, 295);
            this.fpErrorDrawingListSetting.TabIndex = 12;
            this.fpErrorDrawingListSetting.ButtonClicked += new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpErrorDrawingListSetting_ButtonClicked);
            // 
            // radButtonDeleteAll
            // 
            this.radButtonDeleteAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDeleteAll.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDeleteAll.Location = new System.Drawing.Point(1142, 367);
            this.radButtonDeleteAll.Name = "radButtonDeleteAll";
            this.radButtonDeleteAll.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.radButtonDeleteAll.Size = new System.Drawing.Size(90, 23);
            this.radButtonDeleteAll.TabIndex = 49;
            this.radButtonDeleteAll.Text = "Delete All";
            this.radButtonDeleteAll.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonDeleteAll.ThemeName = "Office2010Silver";
            this.radButtonDeleteAll.Click += new System.EventHandler(this.radButtonDeleteAll_Click);
            // 
            // radButtonAdd
            // 
            this.radButtonAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAdd.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAdd.Location = new System.Drawing.Point(1046, 367);
            this.radButtonAdd.Name = "radButtonAdd";
            this.radButtonAdd.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.radButtonAdd.Size = new System.Drawing.Size(90, 23);
            this.radButtonAdd.TabIndex = 48;
            this.radButtonAdd.Text = "Add";
            this.radButtonAdd.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonAdd.ThemeName = "Office2010Silver";
            this.radButtonAdd.Click += new System.EventHandler(this.radButtonAdd_Click);
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radButtonDeleteAll);
            this.radPanel1.Controls.Add(this.radDropDownListProjectList);
            this.radPanel1.Controls.Add(this.radDropDownListPlant);
            this.radPanel1.Controls.Add(this.radButtonAdd);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.fpErrorDrawingListSetting);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1237, 836);
            this.radPanel1.TabIndex = 51;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // UserControlSPPIDDrawingList
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlSPPIDDrawingList";
            this.Size = new System.Drawing.Size(1237, 836);
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListPlant)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpErrorDrawingListSetting)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDeleteAll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectList;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListPlant;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private FarPoint.Win.Spread.FpSpread fpErrorDrawingListSetting;
        private Telerik.WinControls.UI.RadButton radButtonDeleteAll;
        private Telerik.WinControls.UI.RadButton radButtonAdd;
        private Telerik.WinControls.UI.RadPanel radPanel1;
    }
}
