using System;
namespace Configuration
{
    partial class UserControlCommonEmployee
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
            Telerik.WinControls.UI.TableViewDefinition tableViewDefinition1 = new Telerik.WinControls.UI.TableViewDefinition();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.radButtonInsertRow = new Telerik.WinControls.UI.RadButton();
            this.radButtonRemoveRow = new Telerik.WinControls.UI.RadButton();
            this.radTextBox1 = new Telerik.WinControls.UI.RadTextBox();
            this.radGridView1 = new Telerik.WinControls.UI.RadGridView();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1.MasterTemplate)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radButtonInsertRow);
            this.radPanel1.Controls.Add(this.radButtonRemoveRow);
            this.radPanel1.Controls.Add(this.radTextBox1);
            this.radPanel1.Controls.Add(this.radGridView1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 807);
            this.radPanel1.TabIndex = 0;
            // 
            // radButtonInsertRow
            // 
            this.radButtonInsertRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonInsertRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonInsertRow.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonInsertRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonInsertRow.Location = new System.Drawing.Point(1014, 7);
            this.radButtonInsertRow.Name = "radButtonInsertRow";
            this.radButtonInsertRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonInsertRow.TabIndex = 50;
            this.radButtonInsertRow.ThemeName = "Office2010Silver";
            this.radButtonInsertRow.Click += new System.EventHandler(this.radButtonInsertRow_Click);
            // 
            // radButtonRemoveRow
            // 
            this.radButtonRemoveRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonRemoveRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonRemoveRow.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonRemoveRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonRemoveRow.Location = new System.Drawing.Point(1044, 7);
            this.radButtonRemoveRow.Name = "radButtonRemoveRow";
            this.radButtonRemoveRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonRemoveRow.TabIndex = 49;
            this.radButtonRemoveRow.ThemeName = "Office2010Silver";
            this.radButtonRemoveRow.Click += new System.EventHandler(this.radButtonRemoveRow_Click);
            // 
            // radTextBox1
            // 
            this.radTextBox1.Location = new System.Drawing.Point(4, 9);
            this.radTextBox1.Name = "radTextBox1";
            this.radTextBox1.NullText = "Search...";
            this.radTextBox1.Size = new System.Drawing.Size(196, 20);
            this.radTextBox1.TabIndex = 1;
            this.radTextBox1.ThemeName = "Office2010Silver";
            // 
            // radGridView1
            // 
            this.radGridView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGridView1.Location = new System.Drawing.Point(0, 37);
            // 
            // 
            // 
            this.radGridView1.MasterTemplate.ViewDefinition = tableViewDefinition1;
            this.radGridView1.Name = "radGridView1";
            this.radGridView1.Size = new System.Drawing.Size(1075, 770);
            this.radGridView1.TabIndex = 0;
            this.radGridView1.Text = "radGridView1";
            this.radGridView1.ThemeName = "Office2010Silver";
            // 
            // UserControlCommonEmployee
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonEmployee";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1.MasterTemplate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadGridView radGridView1;
        private Telerik.WinControls.UI.RadTextBox radTextBox1;
        private Telerik.WinControls.UI.RadButton radButtonInsertRow;
        private Telerik.WinControls.UI.RadButton radButtonRemoveRow;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;

    }
}
