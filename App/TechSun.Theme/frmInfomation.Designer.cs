namespace TechSun.Theme
{
    partial class frmInfomation
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.radLabelWarning = new Telerik.WinControls.UI.RadLabel();
            this.radButtonOK = new TechSun.Theme.KRadButton();
            this.panel11 = new System.Windows.Forms.Panel();
            this.pictureBoxWarning = new System.Windows.Forms.PictureBox();
            this.TextBoxInfomation = new System.Windows.Forms.TextBox();
            this.windows7Theme1 = new Telerik.WinControls.Themes.Windows7Theme();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.radButtonExportTagList = new TechSun.Theme.KRadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelWarning)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxWarning)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonExportTagList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabelWarning
            // 
            this.radLabelWarning.Font = new System.Drawing.Font("Malgun Gothic", 17F);
            this.radLabelWarning.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.radLabelWarning.Location = new System.Drawing.Point(125, 31);
            this.radLabelWarning.Name = "radLabelWarning";
            this.radLabelWarning.Size = new System.Drawing.Size(170, 35);
            this.radLabelWarning.TabIndex = 1;
            this.radLabelWarning.Text = "INFORMATION";
            this.radLabelWarning.ThemeName = "Windows8";
            // 
            // radButtonOK
            // 
            this.radButtonOK.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonOK.Location = new System.Drawing.Point(327, 217);
            this.radButtonOK.Name = "radButtonOK";
            this.radButtonOK.Size = new System.Drawing.Size(80, 26);
            this.radButtonOK.TabIndex = 5;
            this.radButtonOK.Text = "OK";
            this.radButtonOK.ThemeName = "Windows8";
            this.radButtonOK.Click += new System.EventHandler(this.radButtonOK_Click);
            // 
            // panel11
            // 
            this.panel11.BackColor = System.Drawing.Color.Black;
            this.panel11.Location = new System.Drawing.Point(14, 78);
            this.panel11.Name = "panel11";
            this.panel11.Size = new System.Drawing.Size(393, 1);
            this.panel11.TabIndex = 9;
            // 
            // pictureBoxWarning
            // 
            this.pictureBoxWarning.Image = global::TechSun.Theme.Properties.Resources.ico_information;
            this.pictureBoxWarning.Location = new System.Drawing.Point(93, 36);
            this.pictureBoxWarning.Name = "pictureBoxWarning";
            this.pictureBoxWarning.Size = new System.Drawing.Size(24, 24);
            this.pictureBoxWarning.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxWarning.TabIndex = 0;
            this.pictureBoxWarning.TabStop = false;
            // 
            // TextBoxInfomation
            // 
            this.TextBoxInfomation.BackColor = System.Drawing.Color.White;
            this.TextBoxInfomation.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.TextBoxInfomation.Font = new System.Drawing.Font("Malgun Gothic", 9F);
            this.TextBoxInfomation.Location = new System.Drawing.Point(35, 110);
            this.TextBoxInfomation.Multiline = true;
            this.TextBoxInfomation.Name = "TextBoxInfomation";
            this.TextBoxInfomation.ReadOnly = true;
            this.TextBoxInfomation.Size = new System.Drawing.Size(343, 77);
            this.TextBoxInfomation.TabIndex = 51;
            this.TextBoxInfomation.TabStop = false;
            // 
            // radButtonExportTagList
            // 
            this.radButtonExportTagList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonExportTagList.Location = new System.Drawing.Point(231, 217);
            this.radButtonExportTagList.Name = "radButtonExportTagList";
            this.radButtonExportTagList.Size = new System.Drawing.Size(90, 26);
            this.radButtonExportTagList.TabIndex = 6;
            this.radButtonExportTagList.Text = "Export Tag List";
            this.radButtonExportTagList.ThemeName = "Windows8";
            // 
            // frmInfomation
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(419, 255);
            this.Controls.Add(this.radButtonExportTagList);
            this.Controls.Add(this.TextBoxInfomation);
            this.Controls.Add(this.panel11);
            this.Controls.Add(this.radButtonOK);
            this.Controls.Add(this.radLabelWarning);
            this.Controls.Add(this.pictureBoxWarning);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmInfomation";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "   Information";
            this.ThemeName = "Windows8";
            ((System.ComponentModel.ISupportInitialize)(this.radLabelWarning)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxWarning)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonExportTagList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabelWarning;
        private TechSun.Theme.KRadButton radButtonOK;
        private System.Windows.Forms.Panel panel11;
        private System.Windows.Forms.PictureBox pictureBoxWarning;
        public System.Windows.Forms.TextBox TextBoxInfomation;
        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private KRadButton radButtonExportTagList;
    }
}