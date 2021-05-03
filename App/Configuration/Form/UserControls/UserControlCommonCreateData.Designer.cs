namespace Configuration
{
    partial class UserControlCommonCreateData
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
            this.radDropDownListProjectSP3D = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProjectSPPID = new Telerik.WinControls.UI.RadDropDownList();
            this.radButtonSP3DDownload = new Telerik.WinControls.UI.RadButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radButtonSPPIDDownload = new Telerik.WinControls.UI.RadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectSP3D)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectSPPID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonSP3DDownload)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonSPPIDDownload)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(6, 23);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(41, 17);
            this.radLabel1.TabIndex = 6;
            this.radLabel1.Text = "Project";
            // 
            // radDropDownListProjectSP3D
            // 
            this.radDropDownListProjectSP3D.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectSP3D.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectSP3D.Location = new System.Drawing.Point(108, 22);
            this.radDropDownListProjectSP3D.Name = "radDropDownListProjectSP3D";
            this.radDropDownListProjectSP3D.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectSP3D.TabIndex = 5;
            this.radDropDownListProjectSP3D.Text = "radDropDownList1";
            this.radDropDownListProjectSP3D.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(6, 23);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(95, 17);
            this.radLabel2.TabIndex = 11;
            this.radLabel2.Text = "SPPID DataBase :";
            // 
            // radDropDownListProjectSPPID
            // 
            this.radDropDownListProjectSPPID.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectSPPID.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectSPPID.Location = new System.Drawing.Point(108, 22);
            this.radDropDownListProjectSPPID.Name = "radDropDownListProjectSPPID";
            this.radDropDownListProjectSPPID.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectSPPID.TabIndex = 10;
            this.radDropDownListProjectSPPID.ThemeName = "Office2010Silver";
            // 
            // radButtonSP3DDownload
            // 
            this.radButtonSP3DDownload.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonSP3DDownload.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonSP3DDownload.Location = new System.Drawing.Point(360, 20);
            this.radButtonSP3DDownload.Name = "radButtonSP3DDownload";
            this.radButtonSP3DDownload.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.radButtonSP3DDownload.Size = new System.Drawing.Size(90, 23);
            this.radButtonSP3DDownload.TabIndex = 62;
            this.radButtonSP3DDownload.Text = "Download ";
            this.radButtonSP3DDownload.ThemeName = "Office2010Silver";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.radLabel1);
            this.groupBox2.Controls.Add(this.radDropDownListProjectSP3D);
            this.groupBox2.Controls.Add(this.radButtonSP3DDownload);
            this.groupBox2.Location = new System.Drawing.Point(3, 8);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(1028, 54);
            this.groupBox2.TabIndex = 63;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "SP3D Data Download";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.radButtonSPPIDDownload);
            this.groupBox1.Controls.Add(this.radLabel2);
            this.groupBox1.Controls.Add(this.radDropDownListProjectSPPID);
            this.groupBox1.Location = new System.Drawing.Point(3, 70);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1028, 82);
            this.groupBox1.TabIndex = 64;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "SPPID Data Download";
            // 
            // radButtonSPPIDDownload
            // 
            this.radButtonSPPIDDownload.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonSPPIDDownload.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonSPPIDDownload.Location = new System.Drawing.Point(360, 20);
            this.radButtonSPPIDDownload.Name = "radButtonSPPIDDownload";
            this.radButtonSPPIDDownload.Padding = new System.Windows.Forms.Padding(6, 0, 0, 0);
            this.radButtonSPPIDDownload.Size = new System.Drawing.Size(90, 23);
            this.radButtonSPPIDDownload.TabIndex = 63;
            this.radButtonSPPIDDownload.Text = "Download ";
            this.radButtonSPPIDDownload.ThemeName = "Office2010Silver";
            // 
            // UserControlCommonCreateData
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Name = "UserControlCommonCreateData";
            this.Size = new System.Drawing.Size(1034, 625);
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectSP3D)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectSPPID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonSP3DDownload)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonSPPIDDownload)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectSP3D;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectSPPID;
        private Telerik.WinControls.UI.RadButton radButtonSP3DDownload;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox1;
        private Telerik.WinControls.UI.RadButton radButtonSPPIDDownload;
    }
}
