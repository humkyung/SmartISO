namespace Configuration
{
    partial class UserControlSPPIDItemTagFormat
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
            this.fpSpreadTagFormat = new FarPoint.Win.Spread.FpSpread();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadTagFormat)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // fpSpreadTagFormat
            // 
            this.fpSpreadTagFormat.AccessibleDescription = "";
            this.fpSpreadTagFormat.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadTagFormat.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadTagFormat.Location = new System.Drawing.Point(10, 10);
            this.fpSpreadTagFormat.Name = "fpSpreadTagFormat";
            this.fpSpreadTagFormat.Size = new System.Drawing.Size(630, 219);
            this.fpSpreadTagFormat.TabIndex = 11;
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.fpSpreadTagFormat);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(650, 240);
            this.radPanel1.TabIndex = 13;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // UserControlSPPIDItemTagFormat
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlSPPIDItemTagFormat";
            this.Size = new System.Drawing.Size(650, 240);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadTagFormat)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private FarPoint.Win.Spread.FpSpread fpSpreadTagFormat;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private Telerik.WinControls.UI.RadPanel radPanel1;
    }
}
