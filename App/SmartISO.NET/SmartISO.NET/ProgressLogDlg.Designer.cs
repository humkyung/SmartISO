namespace SmartISO.NET
{
	partial class ProgressLogDlg
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
			this.progressBarStatus = new System.Windows.Forms.ProgressBar();
			this.textBoxFileName = new System.Windows.Forms.TextBox();
			this.listViewLog = new System.Windows.Forms.ListView();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.buttonClose = new System.Windows.Forms.Button();
			this.buttonSaveData = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// progressBarStatus
			// 
			this.progressBarStatus.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.progressBarStatus.Location = new System.Drawing.Point(12, 39);
			this.progressBarStatus.Name = "progressBarStatus";
			this.progressBarStatus.Size = new System.Drawing.Size(553, 23);
			this.progressBarStatus.TabIndex = 0;
			// 
			// textBoxFileName
			// 
			this.textBoxFileName.Location = new System.Drawing.Point(12, 12);
			this.textBoxFileName.Name = "textBoxFileName";
			this.textBoxFileName.ReadOnly = true;
			this.textBoxFileName.Size = new System.Drawing.Size(553, 21);
			this.textBoxFileName.TabIndex = 1;
			// 
			// listViewLog
			// 
			this.listViewLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listViewLog.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.listViewLog.Location = new System.Drawing.Point(12, 68);
			this.listViewLog.Name = "listViewLog";
			this.listViewLog.Size = new System.Drawing.Size(553, 335);
			this.listViewLog.TabIndex = 2;
			this.listViewLog.UseCompatibleStateImageBehavior = false;
			this.listViewLog.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "파일 이름";
			this.columnHeader1.Width = 180;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "내용";
			this.columnHeader2.Width = 350;
			// 
			// buttonClose
			// 
			this.buttonClose.Location = new System.Drawing.Point(490, 409);
			this.buttonClose.Name = "buttonClose";
			this.buttonClose.Size = new System.Drawing.Size(75, 30);
			this.buttonClose.TabIndex = 3;
			this.buttonClose.Text = "Close";
			this.buttonClose.UseVisualStyleBackColor = true;
			this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
			// 
			// buttonSaveData
			// 
			this.buttonSaveData.Location = new System.Drawing.Point(12, 409);
			this.buttonSaveData.Name = "buttonSaveData";
			this.buttonSaveData.Size = new System.Drawing.Size(75, 30);
			this.buttonSaveData.TabIndex = 4;
			this.buttonSaveData.Text = "Save Data";
			this.buttonSaveData.UseVisualStyleBackColor = true;
			this.buttonSaveData.Click += new System.EventHandler(this.buttonSaveData_Click);
			// 
			// ProgressLogDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(577, 451);
			this.Controls.Add(this.buttonSaveData);
			this.Controls.Add(this.buttonClose);
			this.Controls.Add(this.listViewLog);
			this.Controls.Add(this.textBoxFileName);
			this.Controls.Add(this.progressBarStatus);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ProgressLogDlg";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "ProgressLogDlg";
			this.Load += new System.EventHandler(this.ProgressLogDlg_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		public System.Windows.Forms.ProgressBar progressBarStatus;
		public System.Windows.Forms.TextBox textBoxFileName;
		public System.Windows.Forms.ListView listViewLog;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		public System.Windows.Forms.Button buttonClose;
		public System.Windows.Forms.Button buttonSaveData;
	}
}