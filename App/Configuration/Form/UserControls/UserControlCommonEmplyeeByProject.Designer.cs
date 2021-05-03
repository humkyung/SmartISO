namespace Configuration
{
    partial class UserControlCommonEmplyeeByProject
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
            this.radSplitContainer1 = new Telerik.WinControls.UI.RadSplitContainer();
            this.splitPanel1 = new Telerik.WinControls.UI.SplitPanel();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.splitPanel2 = new Telerik.WinControls.UI.SplitPanel();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.fpSpreadData_SPPIDDB = new FarPoint.Win.Spread.FpSpread();
            this.radTextBoxSearchName = new Telerik.WinControls.UI.RadTextBox();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).BeginInit();
            this.radSplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).BeginInit();
            this.splitPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).BeginInit();
            this.splitPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_SPPIDDB)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxSearchName)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radSplitContainer1);
            this.radPanel1.Controls.Add(this.radTextBoxSearchName);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1111, 619);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radSplitContainer1
            // 
            this.radSplitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radSplitContainer1.Controls.Add(this.splitPanel1);
            this.radSplitContainer1.Controls.Add(this.splitPanel2);
            this.radSplitContainer1.Location = new System.Drawing.Point(4, 30);
            this.radSplitContainer1.Name = "radSplitContainer1";
            // 
            // 
            // 
            this.radSplitContainer1.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.radSplitContainer1.Size = new System.Drawing.Size(1105, 586);
            this.radSplitContainer1.SplitterWidth = 3;
            this.radSplitContainer1.TabIndex = 4;
            this.radSplitContainer1.TabStop = false;
            this.radSplitContainer1.Text = "radSplitContainer1";
            this.radSplitContainer1.ThemeName = "Office2010Silver";
            // 
            // splitPanel1
            // 
            this.splitPanel1.Controls.Add(this.radLabel1);
            this.splitPanel1.Controls.Add(this.fpSpreadData);
            this.splitPanel1.Location = new System.Drawing.Point(0, 0);
            this.splitPanel1.Name = "splitPanel1";
            // 
            // 
            // 
            this.splitPanel1.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.splitPanel1.Size = new System.Drawing.Size(736, 586);
            this.splitPanel1.SizeInfo.AbsoluteSize = new System.Drawing.Size(736, 200);
            this.splitPanel1.SizeInfo.SizeMode = Telerik.WinControls.UI.Docking.SplitPanelSizeMode.Absolute;
            this.splitPanel1.SizeInfo.SplitterCorrection = new System.Drawing.Size(536, 0);
            this.splitPanel1.TabIndex = 0;
            this.splitPanel1.TabStop = false;
            this.splitPanel1.Text = "splitPanel1";
            this.splitPanel1.ThemeName = "Office2010Silver";
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(5, 8);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(65, 17);
            this.radLabel1.TabIndex = 9;
            this.radLabel1.Text = "Project Info";
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData.Location = new System.Drawing.Point(3, 32);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Size = new System.Drawing.Size(730, 550);
            this.fpSpreadData.TabIndex = 3;
            this.fpSpreadData.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadData_CellClick);
            this.fpSpreadData.ButtonClicked += new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpSpreadData_ButtonClicked);
            // 
            // splitPanel2
            // 
            this.splitPanel2.Controls.Add(this.radLabel2);
            this.splitPanel2.Controls.Add(this.fpSpreadData_SPPIDDB);
            this.splitPanel2.Location = new System.Drawing.Point(739, 0);
            this.splitPanel2.Name = "splitPanel2";
            // 
            // 
            // 
            this.splitPanel2.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.splitPanel2.Size = new System.Drawing.Size(366, 586);
            this.splitPanel2.SizeInfo.SplitterCorrection = new System.Drawing.Size(-536, 0);
            this.splitPanel2.TabIndex = 1;
            this.splitPanel2.TabStop = false;
            this.splitPanel2.Text = "splitPanel2";
            this.splitPanel2.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(3, 8);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(79, 17);
            this.radLabel2.TabIndex = 10;
            this.radLabel2.Text = "SPPID DB Info";
            // 
            // fpSpreadData_SPPIDDB
            // 
            this.fpSpreadData_SPPIDDB.AccessibleDescription = "";
            this.fpSpreadData_SPPIDDB.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData_SPPIDDB.Location = new System.Drawing.Point(3, 32);
            this.fpSpreadData_SPPIDDB.Name = "fpSpreadData_SPPIDDB";
            this.fpSpreadData_SPPIDDB.Size = new System.Drawing.Size(360, 550);
            this.fpSpreadData_SPPIDDB.TabIndex = 10;
            this.fpSpreadData_SPPIDDB.ButtonClicked += new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpSpreadData_SPPIDDB_ButtonClicked);
            // 
            // radTextBoxSearchName
            // 
            this.radTextBoxSearchName.Location = new System.Drawing.Point(4, 4);
            this.radTextBoxSearchName.Name = "radTextBoxSearchName";
            this.radTextBoxSearchName.NullText = "Search...";
            this.radTextBoxSearchName.Size = new System.Drawing.Size(196, 20);
            this.radTextBoxSearchName.TabIndex = 2;
            this.radTextBoxSearchName.KeyDown += new System.Windows.Forms.KeyEventHandler(this.radTextBoxSearchName_KeyDown);
            // 
            // UserControlCommonEmplyeeByProject
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlCommonEmplyeeByProject";
            this.Size = new System.Drawing.Size(1111, 619);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).EndInit();
            this.radSplitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).EndInit();
            this.splitPanel1.ResumeLayout(false);
            this.splitPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).EndInit();
            this.splitPanel2.ResumeLayout(false);
            this.splitPanel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData_SPPIDDB)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxSearchName)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private Telerik.WinControls.UI.RadTextBox radTextBoxSearchName;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainer1;
        private Telerik.WinControls.UI.SplitPanel splitPanel1;
        private Telerik.WinControls.UI.SplitPanel splitPanel2;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private FarPoint.Win.Spread.FpSpread fpSpreadData_SPPIDDB;
    }
}
