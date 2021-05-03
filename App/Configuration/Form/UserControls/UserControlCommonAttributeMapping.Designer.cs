namespace Configuration
{
    partial class UserControlCommonAttributeMapping
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
            this.radButtonDown = new Telerik.WinControls.UI.RadButton();
            this.radButtonUp = new Telerik.WinControls.UI.RadButton();
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListListType = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProject = new Telerik.WinControls.UI.RadDropDownList();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radButtonDown);
            this.radPanel1.Controls.Add(this.radButtonUp);
            this.radPanel1.Controls.Add(this.radButtonDelete);
            this.radPanel1.Controls.Add(this.radButtonCreate);
            this.radPanel1.Controls.Add(this.groupBox1);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.radDropDownListListType);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radDropDownListProject);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1009, 807);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = global::Configuration.Properties.Resources.SmallDown;
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(971, 5);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 24);
            this.radButtonDown.TabIndex = 54;
            this.radButtonDown.ThemeName = "Office2010Silver";
            this.radButtonDown.Visible = false;
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = global::Configuration.Properties.Resources.SmallUp;
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(941, 5);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 24);
            this.radButtonUp.TabIndex = 53;
            this.radButtonUp.ThemeName = "Office2010Silver";
            this.radButtonUp.Visible = false;
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(911, 5);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 52;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Visible = false;
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(881, 5);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 24);
            this.radButtonCreate.TabIndex = 51;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Visible = false;
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.fpSpreadData);
            this.groupBox1.Location = new System.Drawing.Point(12, 33);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(983, 763);
            this.groupBox1.TabIndex = 11;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Attribute";
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fpSpreadData.Location = new System.Drawing.Point(3, 18);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Size = new System.Drawing.Size(977, 742);
            this.fpSpreadData.TabIndex = 0;
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(342, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(48, 17);
            this.radLabel2.TabIndex = 10;
            this.radLabel2.Text = "ListType";
            // 
            // radDropDownListListType
            // 
            this.radDropDownListListType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListListType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListListType.Location = new System.Drawing.Point(410, 8);
            this.radDropDownListListType.Name = "radDropDownListListType";
            this.radDropDownListListType.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListListType.TabIndex = 9;
            this.radDropDownListListType.Text = "radDropDownList2";
            this.radDropDownListListType.ThemeName = "Office2010Silver";
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(41, 17);
            this.radLabel1.TabIndex = 8;
            this.radLabel1.Text = "Project";
            // 
            // radDropDownListProject
            // 
            this.radDropDownListProject.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProject.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProject.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListProject.Name = "radDropDownListProject";
            this.radDropDownListProject.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProject.TabIndex = 7;
            this.radDropDownListProject.Text = "radDropDownList1";
            this.radDropDownListProject.ThemeName = "Office2010Silver";
            // 
            // UserControlCommonAttributeMapping
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonAttributeMapping";
            this.Size = new System.Drawing.Size(1009, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListListType;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProject;
        private System.Windows.Forms.GroupBox groupBox1;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private Telerik.WinControls.UI.RadButton radButtonDown;
        private Telerik.WinControls.UI.RadButton radButtonUp;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
    }
}
