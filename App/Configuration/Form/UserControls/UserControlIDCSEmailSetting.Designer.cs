namespace Configuration
{
	partial class UserControlIDCSEmailSetting
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
			Telerik.WinControls.UI.GridViewCheckBoxColumn gridViewCheckBoxColumn1 = new Telerik.WinControls.UI.GridViewCheckBoxColumn();
			Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn1 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
			Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn2 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
			Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn3 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
			Telerik.WinControls.UI.TableViewDefinition tableViewDefinition1 = new Telerik.WinControls.UI.TableViewDefinition();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserControlIDCSEmailSetting));
			this.radLabelSendTo = new Telerik.WinControls.UI.RadLabel();
			this.radGridViewMailSetting = new Telerik.WinControls.UI.RadGridView();
			this.radButtonAdd = new TechSun.Theme.KRadPlusImageButton();
			this.radButtonDelete = new TechSun.Theme.KRadCloseImageButton();
			((System.ComponentModel.ISupportInitialize)(this.radLabelSendTo)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewMailSetting)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewMailSetting.MasterTemplate)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
			this.SuspendLayout();
			// 
			// radLabelSendTo
			// 
			this.radLabelSendTo.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radLabelSendTo.Location = new System.Drawing.Point(3, 3);
			this.radLabelSendTo.Name = "radLabelSendTo";
			this.radLabelSendTo.Size = new System.Drawing.Size(30, 17);
			this.radLabelSendTo.TabIndex = 0;
			this.radLabelSendTo.Text = "To : ";
			this.radLabelSendTo.ThemeName = "Office2010Silver";
			// 
			// radGridViewMailSetting
			// 
			this.radGridViewMailSetting.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.radGridViewMailSetting.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radGridViewMailSetting.Location = new System.Drawing.Point(3, 33);
			// 
			// 
			// 
			this.radGridViewMailSetting.MasterTemplate.AllowAddNewRow = false;
			this.radGridViewMailSetting.MasterTemplate.AllowDeleteRow = false;
			this.radGridViewMailSetting.MasterTemplate.AllowRowResize = false;
			gridViewCheckBoxColumn1.EnableHeaderCheckBox = true;
			gridViewCheckBoxColumn1.HeaderText = "";
			gridViewCheckBoxColumn1.MaxWidth = 26;
			gridViewCheckBoxColumn1.MinWidth = 26;
			gridViewCheckBoxColumn1.Name = "Check";
			gridViewCheckBoxColumn1.Width = 26;
			gridViewTextBoxColumn1.HeaderText = "ID";
			gridViewTextBoxColumn1.Name = "ID";
			gridViewTextBoxColumn2.HeaderText = "Name";
			gridViewTextBoxColumn2.Name = "Name";
			gridViewTextBoxColumn3.HeaderText = "Email";
			gridViewTextBoxColumn3.Name = "Address";
			this.radGridViewMailSetting.MasterTemplate.Columns.AddRange(new Telerik.WinControls.UI.GridViewDataColumn[] {
            gridViewCheckBoxColumn1,
            gridViewTextBoxColumn1,
            gridViewTextBoxColumn2,
            gridViewTextBoxColumn3});
			this.radGridViewMailSetting.MasterTemplate.ShowRowHeaderColumn = false;
			this.radGridViewMailSetting.MasterTemplate.ViewDefinition = tableViewDefinition1;
			this.radGridViewMailSetting.Name = "radGridViewMailSetting";
			this.radGridViewMailSetting.ShowGroupPanel = false;
			this.radGridViewMailSetting.Size = new System.Drawing.Size(568, 421);
			this.radGridViewMailSetting.TabIndex = 1;
			this.radGridViewMailSetting.Text = "radGridView1";
			this.radGridViewMailSetting.ThemeName = "Office2010Silver";
			this.radGridViewMailSetting.CellEndEdit += new Telerik.WinControls.UI.GridViewCellEventHandler(this.radGridViewMailSetting_CellEndEdit);
			// 
			// radButtonAdd
			// 
			this.radButtonAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.radButtonAdd.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radButtonAdd.Image = ((System.Drawing.Image)(resources.GetObject("radButtonAdd.Image")));
			this.radButtonAdd.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
			this.radButtonAdd.Location = new System.Drawing.Point(517, 3);
			this.radButtonAdd.Name = "radButtonAdd";
			this.radButtonAdd.Size = new System.Drawing.Size(24, 24);
			this.radButtonAdd.TabIndex = 6;
			this.radButtonAdd.ThemeName = "Office2010Silver";
			this.radButtonAdd.Click += new System.EventHandler(this.radButtonAdd_Click);
			// 
			// radButtonDelete
			// 
			this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radButtonDelete.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDelete.Image")));
			this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
			this.radButtonDelete.Location = new System.Drawing.Point(547, 3);
			this.radButtonDelete.Name = "radButtonDelete";
			this.radButtonDelete.Size = new System.Drawing.Size(24, 24);
			this.radButtonDelete.TabIndex = 5;
			this.radButtonDelete.ThemeName = "Office2010Silver";
			this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
			// 
			// UserControlIDCSEmailSetting
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.radButtonAdd);
			this.Controls.Add(this.radButtonDelete);
			this.Controls.Add(this.radGridViewMailSetting);
			this.Controls.Add(this.radLabelSendTo);
			this.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.Name = "UserControlIDCSEmailSetting";
			this.Size = new System.Drawing.Size(574, 457);
			this.Load += new System.EventHandler(this.UserControlIDCSEmailSetting_Load);
			((System.ComponentModel.ISupportInitialize)(this.radLabelSendTo)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewMailSetting.MasterTemplate)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewMailSetting)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private Telerik.WinControls.UI.RadLabel radLabelSendTo;
		private Telerik.WinControls.UI.RadGridView radGridViewMailSetting;
		private TechSun.Theme.KRadPlusImageButton radButtonAdd;
		private TechSun.Theme.KRadCloseImageButton radButtonDelete;
	}
}
