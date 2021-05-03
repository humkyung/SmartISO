namespace Configuration
{
	partial class UserControlRemoveSetting
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
            Telerik.WinControls.UI.GridViewTextBoxColumn gridViewTextBoxColumn1 = new Telerik.WinControls.UI.GridViewTextBoxColumn();
            Telerik.WinControls.UI.GridViewCommandColumn gridViewCommandColumn1 = new Telerik.WinControls.UI.GridViewCommandColumn();
            this.radGroupBoxBorderSettingData = new Telerik.WinControls.UI.RadGroupBox();
            this.radGridViewRemoveSetting = new Telerik.WinControls.UI.RadGridView();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBoxBorderSettingData)).BeginInit();
            this.radGroupBoxBorderSettingData.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewRemoveSetting)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewRemoveSetting.MasterTemplate)).BeginInit();
            this.SuspendLayout();
            // 
            // radGroupBoxBorderSettingData
            // 
            this.radGroupBoxBorderSettingData.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBoxBorderSettingData.Controls.Add(this.radGridViewRemoveSetting);
            this.radGroupBoxBorderSettingData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radGroupBoxBorderSettingData.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGroupBoxBorderSettingData.FooterImageIndex = -1;
            this.radGroupBoxBorderSettingData.FooterImageKey = "";
            this.radGroupBoxBorderSettingData.HeaderImageIndex = -1;
            this.radGroupBoxBorderSettingData.HeaderImageKey = "";
            this.radGroupBoxBorderSettingData.HeaderMargin = new System.Windows.Forms.Padding(0);
            this.radGroupBoxBorderSettingData.HeaderText = "Remove Area";
            this.radGroupBoxBorderSettingData.Location = new System.Drawing.Point(0, 0);
            this.radGroupBoxBorderSettingData.Name = "radGroupBoxBorderSettingData";
            this.radGroupBoxBorderSettingData.Padding = new System.Windows.Forms.Padding(2, 18, 2, 2);
            // 
            // 
            // 
            this.radGroupBoxBorderSettingData.RootElement.Padding = new System.Windows.Forms.Padding(2, 18, 2, 2);
            this.radGroupBoxBorderSettingData.Size = new System.Drawing.Size(606, 248);
            this.radGroupBoxBorderSettingData.TabIndex = 1;
            this.radGroupBoxBorderSettingData.Text = "Remove Area";
            this.radGroupBoxBorderSettingData.ThemeName = "Office2010Silver";
            // 
            // radGridViewRemoveSetting
            // 
            this.radGridViewRemoveSetting.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radGridViewRemoveSetting.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGridViewRemoveSetting.Location = new System.Drawing.Point(2, 18);
            // 
            // radGridViewRemoveSetting
            // 
            this.radGridViewRemoveSetting.MasterTemplate.AllowAddNewRow = false;
            this.radGridViewRemoveSetting.MasterTemplate.AllowDeleteRow = false;
            this.radGridViewRemoveSetting.MasterTemplate.AllowRowResize = false;
            gridViewTextBoxColumn1.HeaderText = "Name";
            gridViewTextBoxColumn1.Name = "Name";
            gridViewCommandColumn1.HeaderText = "...";
            gridViewCommandColumn1.MaxWidth = 25;
            gridViewCommandColumn1.MinWidth = 25;
            gridViewCommandColumn1.Name = "Command";
            gridViewCommandColumn1.Width = 25;
            this.radGridViewRemoveSetting.MasterTemplate.Columns.AddRange(new Telerik.WinControls.UI.GridViewDataColumn[] {
            gridViewTextBoxColumn1,
            gridViewCommandColumn1});
            this.radGridViewRemoveSetting.MasterTemplate.ShowRowHeaderColumn = false;
            this.radGridViewRemoveSetting.Name = "radGridViewRemoveSetting";
            this.radGridViewRemoveSetting.ShowGroupPanel = false;
            this.radGridViewRemoveSetting.Size = new System.Drawing.Size(602, 228);
            this.radGridViewRemoveSetting.TabIndex = 0;
            this.radGridViewRemoveSetting.Text = "radGridView1";
            this.radGridViewRemoveSetting.ThemeName = "Office2010Silver";
            this.radGridViewRemoveSetting.CommandCellClick += new Telerik.WinControls.UI.CommandCellClickEventHandler(this.radGridViewRemoveSetting_CommandCellClick);
            // 
            // UserControlRemoveSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radGroupBoxBorderSettingData);
            this.Name = "UserControlRemoveSetting";
            this.Size = new System.Drawing.Size(606, 248);
            this.Load += new System.EventHandler(this.UserControlIDCSRemoveSetting_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBoxBorderSettingData)).EndInit();
            this.radGroupBoxBorderSettingData.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewRemoveSetting.MasterTemplate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewRemoveSetting)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

		private Telerik.WinControls.UI.RadGroupBox radGroupBoxBorderSettingData;
		private Telerik.WinControls.UI.RadGridView radGridViewRemoveSetting;

	}
}
