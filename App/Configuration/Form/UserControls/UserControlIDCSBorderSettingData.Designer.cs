namespace Configuration
{
	partial class UserControlBorderSettingData
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
			Telerik.WinControls.UI.TableViewDefinition tableViewDefinition1 = new Telerik.WinControls.UI.TableViewDefinition();
			this.radGroupBoxBorderSettingData = new Telerik.WinControls.UI.RadGroupBox();
			this.radGridViewBorderSettingData = new Telerik.WinControls.UI.RadGridView();
			((System.ComponentModel.ISupportInitialize)(this.radGroupBoxBorderSettingData)).BeginInit();
			this.radGroupBoxBorderSettingData.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewBorderSettingData)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewBorderSettingData.MasterTemplate)).BeginInit();
			this.SuspendLayout();
			// 
			// radGroupBoxBorderSettingData
			// 
			this.radGroupBoxBorderSettingData.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
			this.radGroupBoxBorderSettingData.Controls.Add(this.radGridViewBorderSettingData);
			this.radGroupBoxBorderSettingData.Dock = System.Windows.Forms.DockStyle.Fill;
			this.radGroupBoxBorderSettingData.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radGroupBoxBorderSettingData.HeaderText = "Iso Dwg Area";
			this.radGroupBoxBorderSettingData.Location = new System.Drawing.Point(0, 0);
			this.radGroupBoxBorderSettingData.Name = "radGroupBoxBorderSettingData";
			this.radGroupBoxBorderSettingData.Size = new System.Drawing.Size(713, 258);
			this.radGroupBoxBorderSettingData.TabIndex = 0;
			this.radGroupBoxBorderSettingData.Text = "Iso Dwg Area";
			this.radGroupBoxBorderSettingData.ThemeName = "Office2010Silver";
			// 
			// radGridViewBorderSettingData
			// 
			this.radGridViewBorderSettingData.Dock = System.Windows.Forms.DockStyle.Fill;
			this.radGridViewBorderSettingData.Font = new System.Drawing.Font("Tahoma", 8.25F);
			this.radGridViewBorderSettingData.Location = new System.Drawing.Point(2, 18);
			// 
			// 
			// 
			this.radGridViewBorderSettingData.MasterTemplate.AllowAddNewRow = false;
			this.radGridViewBorderSettingData.MasterTemplate.AllowDeleteRow = false;
			this.radGridViewBorderSettingData.MasterTemplate.AllowRowResize = false;
			gridViewTextBoxColumn1.HeaderText = "Name";
			gridViewTextBoxColumn1.Name = "Name";
			gridViewCommandColumn1.HeaderText = "...";
			gridViewCommandColumn1.MaxWidth = 25;
			gridViewCommandColumn1.MinWidth = 25;
			gridViewCommandColumn1.Name = "Command";
			gridViewCommandColumn1.Width = 25;
			this.radGridViewBorderSettingData.MasterTemplate.Columns.AddRange(new Telerik.WinControls.UI.GridViewDataColumn[] {
            gridViewTextBoxColumn1,
            gridViewCommandColumn1});
			this.radGridViewBorderSettingData.MasterTemplate.ShowRowHeaderColumn = false;
			this.radGridViewBorderSettingData.MasterTemplate.ViewDefinition = tableViewDefinition1;
			this.radGridViewBorderSettingData.Name = "radGridViewBorderSettingData";
			this.radGridViewBorderSettingData.ShowGroupPanel = false;
			this.radGridViewBorderSettingData.Size = new System.Drawing.Size(709, 238);
			this.radGridViewBorderSettingData.TabIndex = 0;
			this.radGridViewBorderSettingData.Text = "radGridView1";
			this.radGridViewBorderSettingData.ThemeName = "Office2010Silver";
			this.radGridViewBorderSettingData.CommandCellClick += new Telerik.WinControls.UI.CommandCellClickEventHandler(this.radGridViewBorderSettingData_CommandCellClick);
			// 
			// UserControlBorderSettingData
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 14F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.radGroupBoxBorderSettingData);
			this.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.Name = "UserControlBorderSettingData";
			this.Size = new System.Drawing.Size(713, 258);
			this.Load += new System.EventHandler(this.UserControlBorderSettingData_Load);
			((System.ComponentModel.ISupportInitialize)(this.radGroupBoxBorderSettingData)).EndInit();
			this.radGroupBoxBorderSettingData.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.radGridViewBorderSettingData.MasterTemplate)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.radGridViewBorderSettingData)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private Telerik.WinControls.UI.RadGroupBox radGroupBoxBorderSettingData;
		private Telerik.WinControls.UI.RadGridView radGridViewBorderSettingData;
	}
}
