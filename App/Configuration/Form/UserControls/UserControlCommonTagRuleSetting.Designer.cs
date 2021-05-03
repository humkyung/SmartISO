namespace Configuration
{
    partial class UserControlCommonTagRuleSetting
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radSplitContainerTagRule = new Telerik.WinControls.UI.RadSplitContainer();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListListType = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProject = new Telerik.WinControls.UI.RadDropDownList();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.groupBox1);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.radDropDownListListType);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radDropDownListProject);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(929, 810);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.radSplitContainerTagRule);
            this.groupBox1.Location = new System.Drawing.Point(12, 33);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(903, 766);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tag Rule Setting";
            // 
            // radSplitContainerTagRule
            // 
            this.radSplitContainerTagRule.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radSplitContainerTagRule.Location = new System.Drawing.Point(3, 17);
            this.radSplitContainerTagRule.Name = "radSplitContainerTagRule";
            this.radSplitContainerTagRule.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // 
            // 
            this.radSplitContainerTagRule.RootElement.MinSize = new System.Drawing.Size(25, 25);
            this.radSplitContainerTagRule.Size = new System.Drawing.Size(897, 746);
            this.radSplitContainerTagRule.SplitterWidth = 3;
            this.radSplitContainerTagRule.TabIndex = 11;
            this.radSplitContainerTagRule.TabStop = false;
            this.radSplitContainerTagRule.Text = "radSplitContainer1";
            this.radSplitContainerTagRule.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(342, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(48, 17);
            this.radLabel2.TabIndex = 6;
            this.radLabel2.Text = "ListType";
            // 
            // radDropDownListListType
            // 
            this.radDropDownListListType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListListType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListListType.Location = new System.Drawing.Point(410, 8);
            this.radDropDownListListType.Name = "radDropDownListListType";
            this.radDropDownListListType.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListListType.TabIndex = 5;
            this.radDropDownListListType.Text = "radDropDownList2";
            this.radDropDownListListType.ThemeName = "Office2010Silver";
            this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListListType_SelectedIndexChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(41, 17);
            this.radLabel1.TabIndex = 4;
            this.radLabel1.Text = "Project";
            // 
            // radDropDownListProject
            // 
            this.radDropDownListProject.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProject.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProject.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListProject.Name = "radDropDownListProject";
            this.radDropDownListProject.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProject.TabIndex = 3;
            this.radDropDownListProject.Text = "radDropDownList1";
            this.radDropDownListProject.ThemeName = "Office2010Silver";
            this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
            // 
            // UserControlCommonTagRuleSetting
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonTagRuleSetting";
            this.Size = new System.Drawing.Size(929, 810);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProject;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListListType;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainerTagRule;
        private System.Windows.Forms.GroupBox groupBox1;
    }
}
