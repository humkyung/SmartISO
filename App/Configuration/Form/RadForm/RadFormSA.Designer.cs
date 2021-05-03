namespace Configuration.Form.RadForm
{
    partial class RadFormSA
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
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListSA = new Telerik.WinControls.UI.RadDropDownList();
            this.radButtonOK = new TechSun.Theme.KRadButton();
            this.radButtonCancel = new TechSun.Theme.KRadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSA)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCancel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(12, 15);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(30, 17);
            this.radLabel1.TabIndex = 0;
            this.radLabel1.Text = "SA : ";
            // 
            // radDropDownListSA
            // 
            this.radDropDownListSA.DropDownAnimationEnabled = true;
            this.radDropDownListSA.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListSA.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListSA.Location = new System.Drawing.Point(48, 12);
            this.radDropDownListSA.Name = "radDropDownListSA";
            this.radDropDownListSA.ShowImageInEditorArea = true;
            this.radDropDownListSA.Size = new System.Drawing.Size(238, 23);
            this.radDropDownListSA.TabIndex = 1;
            this.radDropDownListSA.ThemeName = "Windows7";
            // 
            // radButtonOK
            // 
            this.radButtonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonOK.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonOK.Location = new System.Drawing.Point(132, 44);
            this.radButtonOK.Name = "radButtonOK";
            this.radButtonOK.Size = new System.Drawing.Size(74, 24);
            this.radButtonOK.TabIndex = 2;
            this.radButtonOK.Text = "OK";
            this.radButtonOK.ThemeName = "Windows7";
            this.radButtonOK.Click += new System.EventHandler(this.radButtonOK_Click);
            // 
            // radButtonCancel
            // 
            this.radButtonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCancel.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCancel.Location = new System.Drawing.Point(212, 44);
            this.radButtonCancel.Name = "radButtonCancel";
            this.radButtonCancel.Size = new System.Drawing.Size(74, 24);
            this.radButtonCancel.TabIndex = 3;
            this.radButtonCancel.Text = "Cancel";
            this.radButtonCancel.ThemeName = "Windows7";
            this.radButtonCancel.Click += new System.EventHandler(this.radButtonCancel_Click);
            // 
            // RadFormSA
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(298, 80);
            this.Controls.Add(this.radButtonCancel);
            this.Controls.Add(this.radButtonOK);
            this.Controls.Add(this.radDropDownListSA);
            this.Controls.Add(this.radLabel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "RadFormSA";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Change System Administrator";
            this.ThemeName = "Windows7";
            this.Load += new System.EventHandler(this.RadFormSA_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSA)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCancel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListSA;
        private TechSun.Theme.KRadButton radButtonOK;
        private TechSun.Theme.KRadButton radButtonCancel;
    }
}
