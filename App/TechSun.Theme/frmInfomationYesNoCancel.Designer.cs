namespace TechSun.Theme
{
    partial class frmInformationYesNoCancel
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmInformationYesNoCancel));
            this.radLabelWarning = new Telerik.WinControls.UI.RadLabel();
            this.radButtonNo = new TechSun.Theme.KRadButton();
            this.TextBoxInfomation = new System.Windows.Forms.TextBox();
            this.kRadButtonYes = new TechSun.Theme.KRadButton();
            this.pictureBoxWarning = new System.Windows.Forms.PictureBox();
            this.windows7Theme1 = new Telerik.WinControls.Themes.Windows7Theme();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.panel11 = new System.Windows.Forms.Panel();
            this.kradButtonCancel = new TechSun.Theme.KRadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelWarning)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonNo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonYes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxWarning)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonCancel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabelWarning
            // 
            this.radLabelWarning.Font = new System.Drawing.Font("Malgun Gothic", 17F);
            this.radLabelWarning.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.radLabelWarning.Location = new System.Drawing.Point(158, 31);
            this.radLabelWarning.Name = "radLabelWarning";
            this.radLabelWarning.Size = new System.Drawing.Size(114, 35);
            this.radLabelWarning.TabIndex = 1;
            this.radLabelWarning.Text = "CONFIRM";
            this.radLabelWarning.ThemeName = "Windows8";
            // 
            // radButtonNo
            // 
            this.radButtonNo.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonNo.Location = new System.Drawing.Point(241, 217);
            this.radButtonNo.Name = "radButtonNo";
            this.radButtonNo.Size = new System.Drawing.Size(80, 26);
            this.radButtonNo.TabIndex = 5;
            this.radButtonNo.Text = "No";
            this.radButtonNo.ThemeName = "Windows8";
            this.radButtonNo.Click += new System.EventHandler(this.radButtonNo_Click);
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
            // kRadButtonYes
            // 
            this.kRadButtonYes.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonYes.Location = new System.Drawing.Point(155, 217);
            this.kRadButtonYes.Name = "kRadButtonYes";
            this.kRadButtonYes.Size = new System.Drawing.Size(80, 26);
            this.kRadButtonYes.TabIndex = 52;
            this.kRadButtonYes.Text = "Yes";
            this.kRadButtonYes.ThemeName = "Windows8";
            this.kRadButtonYes.Click += new System.EventHandler(this.kRadButtonYes_Click);
            // 
            // pictureBoxWarning
            // 
            this.pictureBoxWarning.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxWarning.Image")));
            this.pictureBoxWarning.Location = new System.Drawing.Point(125, 36);
            this.pictureBoxWarning.Name = "pictureBoxWarning";
            this.pictureBoxWarning.Size = new System.Drawing.Size(24, 24);
            this.pictureBoxWarning.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBoxWarning.TabIndex = 0;
            this.pictureBoxWarning.TabStop = false;
            // 
            // panel11
            // 
            this.panel11.BackColor = System.Drawing.Color.Black;
            this.panel11.Location = new System.Drawing.Point(14, 78);
            this.panel11.Name = "panel11";
            this.panel11.Size = new System.Drawing.Size(393, 1);
            this.panel11.TabIndex = 9;
            // 
            // kradButtonCancel
            // 
            this.kradButtonCancel.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kradButtonCancel.Location = new System.Drawing.Point(327, 217);
            this.kradButtonCancel.Name = "kradButtonCancel";
            this.kradButtonCancel.Size = new System.Drawing.Size(80, 26);
            this.kradButtonCancel.TabIndex = 6;
            this.kradButtonCancel.Text = "Cancel";
            this.kradButtonCancel.ThemeName = "Office2013Light";
            this.kradButtonCancel.Click += new System.EventHandler(this.kradButtonCancel_Click);
            // 
            // frmInformationYesNoCancel
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(419, 255);
            this.Controls.Add(this.kradButtonCancel);
            this.Controls.Add(this.kRadButtonYes);
            this.Controls.Add(this.TextBoxInfomation);
            this.Controls.Add(this.panel11);
            this.Controls.Add(this.radButtonNo);
            this.Controls.Add(this.radLabelWarning);
            this.Controls.Add(this.pictureBoxWarning);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "frmInformationYesNoCancel";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "   Confirm";
            this.ThemeName = "Windows8";
            ((System.ComponentModel.ISupportInitialize)(this.radLabelWarning)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonNo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonYes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxWarning)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonCancel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabelWarning;
        private TechSun.Theme.KRadButton radButtonNo;
        private System.Windows.Forms.PictureBox pictureBoxWarning;
        private System.Windows.Forms.TextBox TextBoxInfomation;
        private KRadButton kRadButtonYes;
        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private System.Windows.Forms.Panel panel11;
        private KRadButton kradButtonCancel;
    }
}