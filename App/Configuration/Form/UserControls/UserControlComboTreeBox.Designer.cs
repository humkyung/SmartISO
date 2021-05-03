namespace Configuration
{
    partial class UserControlComboTreeBox
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserControlComboTreeBox));
            this.trvMain = new System.Windows.Forms.TreeView();
            this.txtMain = new System.Windows.Forms.TextBox();
            this.btnDropDown = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // trvMain
            // 
            this.trvMain.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.trvMain.Location = new System.Drawing.Point(0, 21);
            this.trvMain.Name = "trvMain";
            this.trvMain.Size = new System.Drawing.Size(150, 97);
            this.trvMain.TabIndex = 1;
            this.trvMain.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler(this.trvMain_NodeMouseDoubleClick);
            this.trvMain.Leave += new System.EventHandler(this.trvMain_Leave);
            // 
            // txtMain
            // 
            this.txtMain.BackColor = System.Drawing.Color.White;
            this.txtMain.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.txtMain.Location = new System.Drawing.Point(0, 0);
            this.txtMain.Name = "txtMain";
            this.txtMain.ReadOnly = true;
            this.txtMain.Size = new System.Drawing.Size(150, 21);
            this.txtMain.TabIndex = 2;
            this.txtMain.SizeChanged += new System.EventHandler(this.txtMain_SizeChanged);
            this.txtMain.TextChanged += new System.EventHandler(this.txtMain_TextChanged);
            // 
            // btnDropDown
            // 
            this.btnDropDown.Image = ((System.Drawing.Image)(resources.GetObject("btnDropDown.Image")));
            this.btnDropDown.Location = new System.Drawing.Point(130, 2);
            this.btnDropDown.Name = "btnDropDown";
            this.btnDropDown.Size = new System.Drawing.Size(18, 17);
            this.btnDropDown.TabIndex = 3;
            this.btnDropDown.UseVisualStyleBackColor = true;
            this.btnDropDown.Click += new System.EventHandler(this.btnDropDown_Click);
            // 
            // ComboTreeBox
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.btnDropDown);
            this.Controls.Add(this.txtMain);
            this.Controls.Add(this.trvMain);
            this.Name = "ComboTreeBox";
            this.Size = new System.Drawing.Size(150, 119);
            this.SizeChanged += new System.EventHandler(this.ComboTreeBox_SizeChanged);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ComboTreeBox_Paint);
            this.Leave += new System.EventHandler(this.ComboTreeBox_Leave);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TreeView trvMain;
        private System.Windows.Forms.TextBox txtMain;
        private System.Windows.Forms.Button btnDropDown;
    }
}
