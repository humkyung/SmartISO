namespace Configuration.Form.RadForm
{
    partial class RadFormCopySetting
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
			this.radDropDownListSite = new Telerik.WinControls.UI.RadDropDownList();
			this.radButtonCopy = new TechSun.Theme.KRadButton();
			this.radButtonClose = new TechSun.Theme.KRadButton();
			this.radDropDownListProject = new Telerik.WinControls.UI.RadDropDownList();
			this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
			((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radDropDownListSite)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonCopy)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
			this.SuspendLayout();
			// 
			// radLabel1
			// 
			this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radLabel1.Location = new System.Drawing.Point(12, 35);
			this.radLabel1.Name = "radLabel1";
			this.radLabel1.Size = new System.Drawing.Size(36, 17);
			this.radLabel1.TabIndex = 0;
			this.radLabel1.Text = "Site : ";
			// 
			// radDropDownListSite
			// 
			this.radDropDownListSite.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
			this.radDropDownListSite.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radDropDownListSite.Location = new System.Drawing.Point(59, 35);
			this.radDropDownListSite.Name = "radDropDownListSite";
			this.radDropDownListSite.Size = new System.Drawing.Size(278, 19);
			this.radDropDownListSite.TabIndex = 1;
			this.radDropDownListSite.ThemeName = "Office2010Silver";
			// 
			// radButtonCopy
			// 
			this.radButtonCopy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.radButtonCopy.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radButtonCopy.Location = new System.Drawing.Point(111, 65);
			this.radButtonCopy.Name = "radButtonCopy";
			this.radButtonCopy.Size = new System.Drawing.Size(110, 24);
			this.radButtonCopy.TabIndex = 2;
			this.radButtonCopy.Text = "Copy";
			this.radButtonCopy.ThemeName = "Office2010Silver";
			this.radButtonCopy.Click += new System.EventHandler(this.radButtonCopy_Click);
			// 
			// radButtonClose
			// 
			this.radButtonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.radButtonClose.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radButtonClose.Location = new System.Drawing.Point(227, 65);
			this.radButtonClose.Name = "radButtonClose";
			this.radButtonClose.Size = new System.Drawing.Size(110, 24);
			this.radButtonClose.TabIndex = 3;
			this.radButtonClose.Text = "Close";
			this.radButtonClose.ThemeName = "Office2010Silver";
			this.radButtonClose.Click += new System.EventHandler(this.radButtonClose_Click);
			// 
			// radDropDownListProject
			// 
			this.radDropDownListProject.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
			this.radDropDownListProject.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radDropDownListProject.Location = new System.Drawing.Point(59, 12);
			this.radDropDownListProject.Name = "radDropDownListProject";
			this.radDropDownListProject.Size = new System.Drawing.Size(278, 19);
			this.radDropDownListProject.TabIndex = 5;
			this.radDropDownListProject.ThemeName = "Office2010Silver";
			this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
			// 
			// radLabel2
			// 
			this.radLabel2.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radLabel2.Location = new System.Drawing.Point(12, 12);
			this.radLabel2.Name = "radLabel2";
			this.radLabel2.Size = new System.Drawing.Size(52, 17);
			this.radLabel2.TabIndex = 4;
			this.radLabel2.Text = "Project : ";
			// 
			// RadFormIDCSCopySetting
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(349, 101);
			this.Controls.Add(this.radDropDownListProject);
			this.Controls.Add(this.radLabel2);
			this.Controls.Add(this.radButtonClose);
			this.Controls.Add(this.radButtonCopy);
			this.Controls.Add(this.radDropDownListSite);
			this.Controls.Add(this.radLabel1);
			this.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "RadFormIDCSCopySetting";
			// 
			// 
			// 
			this.RootElement.ApplyShapeToControl = true;
			this.ShowIcon = false;
			this.Text = "Copy Setting";
			this.ThemeName = "Windows7";
			this.Load += new System.EventHandler(this.RadFormIDCSCopySetting_Load);
			((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radDropDownListSite)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonCopy)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radDropDownListProject)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

		private Telerik.WinControls.UI.RadLabel radLabel1;
		private Telerik.WinControls.UI.RadDropDownList radDropDownListSite;
		private TechSun.Theme.KRadButton radButtonCopy;
		private TechSun.Theme.KRadButton radButtonClose;
		private Telerik.WinControls.UI.RadDropDownList radDropDownListProject;
		private Telerik.WinControls.UI.RadLabel radLabel2;
    }
}
