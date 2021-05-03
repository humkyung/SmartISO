namespace Configuration
{
    partial class UserControlSPPIDMarkTagNoSetting
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
            this.comboTreeBoxItemType = new Configuration.UserControlComboTreeBox();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProjectList = new Telerik.WinControls.UI.RadDropDownList();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radSplitContainerTagRule = new Telerik.WinControls.UI.RadSplitContainer();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.comboTreeBoxItemType);
            this.radPanel1.Controls.Add(this.radLabel2);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Controls.Add(this.radDropDownListProjectList);
            this.radPanel1.Controls.Add(this.groupBox1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1237, 715);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // comboTreeBoxItemType
            // 
            this.comboTreeBoxItemType.DropDownHeight = 106;
            this.comboTreeBoxItemType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.comboTreeBoxItemType.Location = new System.Drawing.Point(410, 8);
            this.comboTreeBoxItemType.Name = "comboTreeBoxItemType";
            this.comboTreeBoxItemType.Size = new System.Drawing.Size(246, 21);
            this.comboTreeBoxItemType.TabIndex = 47;
            this.comboTreeBoxItemType.SelectedChanged += new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);
            // 
            // radLabel2
            // 
            this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel2.Location = new System.Drawing.Point(342, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(65, 17);
            this.radLabel2.TabIndex = 46;
            this.radLabel2.Text = "Item Type :";
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(48, 17);
            this.radLabel1.TabIndex = 45;
            this.radLabel1.Text = "Project :";
            // 
            // radDropDownListProjectList
            // 
            this.radDropDownListProjectList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectList.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListProjectList.Name = "radDropDownListProjectList";
            this.radDropDownListProjectList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectList.TabIndex = 44;
            this.radDropDownListProjectList.ThemeName = "Office2010Silver";
            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.radSplitContainerTagRule);
            this.groupBox1.Location = new System.Drawing.Point(12, 35);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1212, 668);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tag No Setting";
            // 
            // radSplitContainerTagRule
            // 
            this.radSplitContainerTagRule.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radSplitContainerTagRule.Location = new System.Drawing.Point(3, 18);
            this.radSplitContainerTagRule.Name = "radSplitContainerTagRule";
            // 
            // 
            // 
            this.radSplitContainerTagRule.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.radSplitContainerTagRule.Size = new System.Drawing.Size(1206, 647);
            this.radSplitContainerTagRule.SplitterWidth = 3;
            this.radSplitContainerTagRule.TabIndex = 0;
            this.radSplitContainerTagRule.TabStop = false;
            this.radSplitContainerTagRule.Text = "radSplitContainer1";
            this.radSplitContainerTagRule.ThemeName = "Office2010Silver";
            // 
            // UserControlSPPIDMarkTagNoSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlSPPIDMarkTagNoSetting";
            this.Size = new System.Drawing.Size(1237, 715);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).EndInit();
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainerTagRule)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private UserControlComboTreeBox comboTreeBoxItemType;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectList;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainerTagRule;
    }
}
