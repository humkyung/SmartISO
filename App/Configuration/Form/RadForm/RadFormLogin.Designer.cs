namespace Configuration.Form.RadForm
{
    partial class RadFormLogin
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
            this.radGroupBox1 = new Telerik.WinControls.UI.RadGroupBox();
            this.radTextBoxPassword = new Telerik.WinControls.UI.RadTextBox();
            this.radTextBoxID = new Telerik.WinControls.UI.RadTextBox();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonRefresh = new Telerik.WinControls.UI.RadButton();
            this.radButtonOK = new Telerik.WinControls.UI.RadButton();
            this.radButtonCancel = new Telerik.WinControls.UI.RadButton();
            this.radLabelMessage = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).BeginInit();
            this.radGroupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxPassword)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRefresh)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCancel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelMessage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radGroupBox1
            // 
            this.radGroupBox1.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox1.Controls.Add(this.radTextBoxPassword);
            this.radGroupBox1.Controls.Add(this.radTextBoxID);
            this.radGroupBox1.Controls.Add(this.radLabel2);
            this.radGroupBox1.Controls.Add(this.radLabel1);
            this.radGroupBox1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGroupBox1.FooterImageIndex = -1;
            this.radGroupBox1.FooterImageKey = "";
            this.radGroupBox1.HeaderImageIndex = -1;
            this.radGroupBox1.HeaderImageKey = "";
            this.radGroupBox1.HeaderMargin = new System.Windows.Forms.Padding(0);
            this.radGroupBox1.HeaderText = "User";
            this.radGroupBox1.Location = new System.Drawing.Point(12, 12);
            this.radGroupBox1.Name = "radGroupBox1";
            this.radGroupBox1.Padding = new System.Windows.Forms.Padding(2, 18, 2, 2);
            // 
            // 
            // 
            this.radGroupBox1.RootElement.Padding = new System.Windows.Forms.Padding(2, 18, 2, 2);
            this.radGroupBox1.Size = new System.Drawing.Size(274, 98);
            this.radGroupBox1.TabIndex = 0;
            this.radGroupBox1.Text = "User";
            this.radGroupBox1.ThemeName = "Windows7";
            // 
            // radTextBoxPassword
            // 
            this.radTextBoxPassword.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxPassword.Location = new System.Drawing.Point(80, 55);
            this.radTextBoxPassword.Name = "radTextBoxPassword";
            this.radTextBoxPassword.Size = new System.Drawing.Size(189, 21);
            this.radTextBoxPassword.TabIndex = 3;
            this.radTextBoxPassword.TabStop = false;
            this.radTextBoxPassword.ThemeName = "Windows7";
            this.radTextBoxPassword.KeyDown += new System.Windows.Forms.KeyEventHandler(this.radTextBoxPassword_KeyDown);
            // 
            // radTextBoxID
            // 
            this.radTextBoxID.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxID.Location = new System.Drawing.Point(80, 28);
            this.radTextBoxID.Name = "radTextBoxID";
            this.radTextBoxID.Size = new System.Drawing.Size(189, 21);
            this.radTextBoxID.TabIndex = 2;
            this.radTextBoxID.TabStop = false;
            this.radTextBoxID.ThemeName = "Windows7";
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(5, 57);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(64, 17);
            this.radLabel2.TabIndex = 1;
            this.radLabel2.Text = "Password : ";
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(5, 31);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(29, 17);
            this.radLabel1.TabIndex = 0;
            this.radLabel1.Text = "ID : ";
            // 
            // radButtonRefresh
            // 
            this.radButtonRefresh.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.radButtonRefresh.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonRefresh.Location = new System.Drawing.Point(12, 173);
            this.radButtonRefresh.Name = "radButtonRefresh";
            this.radButtonRefresh.Size = new System.Drawing.Size(24, 24);
            this.radButtonRefresh.TabIndex = 1;
            this.radButtonRefresh.ThemeName = "Windows7";
            this.radButtonRefresh.Click += new System.EventHandler(this.radButtonRefresh_Click);
            // 
            // radButtonOK
            // 
            this.radButtonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonOK.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonOK.Location = new System.Drawing.Point(140, 173);
            this.radButtonOK.Name = "radButtonOK";
            this.radButtonOK.Size = new System.Drawing.Size(70, 24);
            this.radButtonOK.TabIndex = 2;
            this.radButtonOK.Text = "OK";
            this.radButtonOK.ThemeName = "Windows7";
            this.radButtonOK.Click += new System.EventHandler(this.radButtonOK_Click);
            // 
            // radButtonCancel
            // 
            this.radButtonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCancel.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCancel.Location = new System.Drawing.Point(216, 173);
            this.radButtonCancel.Name = "radButtonCancel";
            this.radButtonCancel.Size = new System.Drawing.Size(70, 24);
            this.radButtonCancel.TabIndex = 2;
            this.radButtonCancel.Text = "Cancel";
            this.radButtonCancel.ThemeName = "Windows7";
            this.radButtonCancel.Click += new System.EventHandler(this.radButtonCancel_Click);
            // 
            // radLabelMessage
            // 
            this.radLabelMessage.ForeColor = System.Drawing.Color.Red;
            this.radLabelMessage.Location = new System.Drawing.Point(12, 116);
            this.radLabelMessage.Name = "radLabelMessage";
            this.radLabelMessage.Size = new System.Drawing.Size(10, 17);
            this.radLabelMessage.TabIndex = 3;
            this.radLabelMessage.Text = "-";
            // 
            // RadFormLogin
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(298, 209);
            this.Controls.Add(this.radLabelMessage);
            this.Controls.Add(this.radButtonCancel);
            this.Controls.Add(this.radButtonOK);
            this.Controls.Add(this.radButtonRefresh);
            this.Controls.Add(this.radGroupBox1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RadFormLogin";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Login";
            this.ThemeName = "Windows7";
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).EndInit();
            this.radGroupBox1.ResumeLayout(false);
            this.radGroupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxPassword)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRefresh)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCancel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelMessage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadGroupBox radGroupBox1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadButton radButtonRefresh;
        private Telerik.WinControls.UI.RadButton radButtonOK;
        private Telerik.WinControls.UI.RadButton radButtonCancel;
        private Telerik.WinControls.UI.RadTextBox radTextBoxPassword;
        private Telerik.WinControls.UI.RadTextBox radTextBoxID;
        private Telerik.WinControls.UI.RadLabel radLabelMessage;
    }
}
