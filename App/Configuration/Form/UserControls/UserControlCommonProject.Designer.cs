using System.Collections.Generic;
namespace Configuration
{
    partial class UserControlCommonProject
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
            Telerik.WinControls.UI.GridViewCheckBoxColumn gridViewCheckBoxColumn1 = new Telerik.WinControls.UI.GridViewCheckBoxColumn();
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn1 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            Telerik.WinControls.UI.GridViewComboBoxColumn gridViewComboBoxColumn1 = new Telerik.WinControls.UI.GridViewComboBoxColumn();
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn2 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn3 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn4 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn5 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            this.radGridViewProject = new Telerik.WinControls.UI.RadGridView();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radButtonDelete = new Telerik.WinControls.UI.RadButton();
            this.radButtonAttributeSave = new Telerik.WinControls.UI.RadButton();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewProject)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewProject.MasterTemplate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAttributeSave)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            this.SuspendLayout();
            // 
            // radGridViewProject
            // 
            this.radGridViewProject.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGridViewProject.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGridViewProject.Location = new System.Drawing.Point(10, 44);
            // 
            // radGridViewProject
            // 
            this.radGridViewProject.MasterTemplate.AllowAddNewRow = false;
            this.radGridViewProject.MasterTemplate.AllowCellContextMenu = false;
            this.radGridViewProject.MasterTemplate.AllowDeleteRow = false;
            this.radGridViewProject.MasterTemplate.AutoGenerateColumns = false;
            this.radGridViewProject.MasterTemplate.AutoSizeColumnsMode = Telerik.WinControls.UI.GridViewAutoSizeColumnsMode.Fill;
            gridViewCheckBoxColumn1.HeaderText = "...";
            gridViewCheckBoxColumn1.MaxWidth = 26;
            gridViewCheckBoxColumn1.MinWidth = 26;
            gridViewCheckBoxColumn1.Name = "Check";
            gridViewCheckBoxColumn1.Width = 26;
            gridViewTextBoxColumn1.FieldName = "ProjectNo";
            gridViewTextBoxColumn1.FormatString = "";
            gridViewTextBoxColumn1.HeaderText = "Project No";
            gridViewTextBoxColumn1.MaxWidth = 150;
            gridViewTextBoxColumn1.MinWidth = 150;
            gridViewTextBoxColumn1.Name = "ProjectNo";
            gridViewTextBoxColumn1.Width = 150;
            gridViewComboBoxColumn1.DisplayMember = null;
            gridViewComboBoxColumn1.FieldName = "Type";
            gridViewComboBoxColumn1.HeaderText = "Type";
            gridViewComboBoxColumn1.MaxWidth = 100;
            gridViewComboBoxColumn1.MinWidth = 100;
            gridViewComboBoxColumn1.Name = "Type";
            gridViewComboBoxColumn1.ValueMember = null;
            gridViewComboBoxColumn1.Width = 100;
            gridViewTextBoxColumn2.FieldName = "Owner";
            gridViewTextBoxColumn2.HeaderText = "Owner";
            gridViewTextBoxColumn2.MaxWidth = 150;
            gridViewTextBoxColumn2.MinWidth = 150;
            gridViewTextBoxColumn2.Name = "Owner";
            gridViewTextBoxColumn2.Width = 150;
            gridViewTextBoxColumn3.FieldName = "Desc";
            gridViewTextBoxColumn3.FormatString = "";
            gridViewTextBoxColumn3.HeaderText = "Desc";
            gridViewTextBoxColumn3.Name = "Desc";
            gridViewTextBoxColumn3.Width = 623;
            gridViewTextBoxColumn4.FieldName = "UID";
            gridViewTextBoxColumn4.HeaderText = "UID";
            gridViewTextBoxColumn4.IsVisible = false;
            gridViewTextBoxColumn4.Name = "UID";
            gridViewTextBoxColumn4.ReadOnly = true;
            gridViewTextBoxColumn4.Width = 44;
            gridViewTextBoxColumn5.FieldName = "DeleteYN";
            gridViewTextBoxColumn5.HeaderText = "DeleteYN";
            gridViewTextBoxColumn5.IsVisible = false;
            gridViewTextBoxColumn5.Name = "DeleteYN";
            gridViewTextBoxColumn5.ReadOnly = true;
            gridViewTextBoxColumn5.Width = 47;
            this.radGridViewProject.MasterTemplate.Columns.AddRange(new Telerik.WinControls.UI.GridViewDataColumn[] {
            gridViewCheckBoxColumn1,
            gridViewTextBoxColumn1,
            gridViewComboBoxColumn1,
            gridViewTextBoxColumn2,
            gridViewTextBoxColumn3,
            gridViewTextBoxColumn4,
            gridViewTextBoxColumn5});
            this.radGridViewProject.MasterTemplate.ShowRowHeaderColumn = false;
            this.radGridViewProject.Name = "radGridViewProject";
            this.radGridViewProject.ShowGroupPanel = false;
            this.radGridViewProject.Size = new System.Drawing.Size(1047, 387);
            this.radGridViewProject.TabIndex = 48;
            this.radGridViewProject.Text = "radGridView1";
            this.radGridViewProject.ThemeName = "Office2010Silver";
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.groupBox1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 443);
            this.radPanel1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.radGridViewProject);
            this.groupBox1.Controls.Add(this.radButtonDelete);
            this.groupBox1.Controls.Add(this.radButtonAttributeSave);
            this.groupBox1.Controls.Add(this.radButtonCreate);
            this.groupBox1.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Bold);
            this.groupBox1.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1068, 437);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Project Setting";
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(1003, 14);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
            this.radButtonDelete.TabIndex = 47;
            this.radButtonDelete.ThemeName = "Office2010Silver";
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radButtonAttributeSave
            // 
            this.radButtonAttributeSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAttributeSave.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAttributeSave.Image = global::Configuration.Properties.Resources.SmallSave;
            this.radButtonAttributeSave.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonAttributeSave.Location = new System.Drawing.Point(1033, 14);
            this.radButtonAttributeSave.Name = "radButtonAttributeSave";
            this.radButtonAttributeSave.Size = new System.Drawing.Size(24, 24);
            this.radButtonAttributeSave.TabIndex = 44;
            this.radButtonAttributeSave.ThemeName = "Office2010Silver";
            this.radButtonAttributeSave.Click += new System.EventHandler(this.radButtonAttributeSave_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(973, 14);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 24);
            this.radButtonCreate.TabIndex = 43;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // UserControlCommonProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonProject";
            this.Size = new System.Drawing.Size(1075, 443);
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewProject.MasterTemplate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewProject)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAttributeSave)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private Telerik.WinControls.UI.RadButton radButtonAttributeSave;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadButton radButtonDelete;
        private Telerik.WinControls.UI.RadGridView radGridViewProject;
    }
}
