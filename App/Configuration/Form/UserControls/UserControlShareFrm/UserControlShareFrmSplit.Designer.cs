namespace Configuration
{
    partial class UserControlShareFrmSplit
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ucShareFrmSplit1 = new EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit_H_MB();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListProject = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListListType = new Telerik.WinControls.UI.RadDropDownList();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).BeginInit();
            this.SuspendLayout();
            // 
            // ucShareFrmSplit1
            // 
            this.ucShareFrmSplit1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ucShareFrmSplit1.Location = new System.Drawing.Point(0, 31);
            this.ucShareFrmSplit1.Name = "ucShareFrmSplit1";
            this.ucShareFrmSplit1.Size = new System.Drawing.Size(858, 641);
            this.ucShareFrmSplit1.TabIndex = 0;
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(326, 4);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(41, 17);
            this.radLabel1.TabIndex = 8;
            this.radLabel1.Text = "Project";
            // 
            // radDropDownListProject
            // 
            this.radDropDownListProject.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProject.Location = new System.Drawing.Point(394, 2);
            this.radDropDownListProject.Name = "radDropDownListProject";
            this.radDropDownListProject.Size = new System.Drawing.Size(246, 20);
            this.radDropDownListProject.TabIndex = 7;
            this.radDropDownListProject.Text = "radDropDownList1";
            this.radDropDownListProject.ThemeName = "Windows8";
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(3, 3);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(30, 18);
            this.radLabel2.TabIndex = 10;
            this.radLabel2.Text = "Type";
            // 
            // radDropDownListListType
            // 
            this.radDropDownListListType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListListType.Location = new System.Drawing.Point(71, 2);
            this.radDropDownListListType.Name = "radDropDownListListType";
            this.radDropDownListListType.Size = new System.Drawing.Size(246, 20);
            this.radDropDownListListType.TabIndex = 9;
            this.radDropDownListListType.Text = "radDropDownList2";
            this.radDropDownListListType.ThemeName = "Windows8";
            // 
            // UserControlShareFrmSplit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radLabel2);
            this.Controls.Add(this.radDropDownListListType);
            this.Controls.Add(this.radLabel1);
            this.Controls.Add(this.radDropDownListProject);
            this.Controls.Add(this.ucShareFrmSplit1);
            this.Name = "UserControlShareFrmSplit";
            this.Size = new System.Drawing.Size(858, 672);
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit_H_MB ucShareFrmSplit1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProject;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListListType;

    }
}
