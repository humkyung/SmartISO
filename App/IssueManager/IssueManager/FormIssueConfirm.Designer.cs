namespace SmartISO
{
	partial class FormIssueConfirm
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
			this.label1 = new System.Windows.Forms.Label();
			this.checkBoxUpdateDatabase = new System.Windows.Forms.CheckBox();
			this.checkBoxTransferIsoDwg = new System.Windows.Forms.CheckBox();
			this.buttonNO = new System.Windows.Forms.Button();
			this.buttonYes = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Font = new System.Drawing.Font("Gulim", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(245, 24);
			this.label1.TabIndex = 0;
			this.label1.Text = "Do you want to issue?";
			// 
			// checkBoxUpdateDatabase
			// 
			this.checkBoxUpdateDatabase.AutoSize = true;
			this.checkBoxUpdateDatabase.Checked = true;
			this.checkBoxUpdateDatabase.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxUpdateDatabase.Enabled = false;
			this.checkBoxUpdateDatabase.Location = new System.Drawing.Point(17, 50);
			this.checkBoxUpdateDatabase.Name = "checkBoxUpdateDatabase";
			this.checkBoxUpdateDatabase.Size = new System.Drawing.Size(276, 16);
			this.checkBoxUpdateDatabase.TabIndex = 1;
			this.checkBoxUpdateDatabase.Text = "Update remote database with local database";
			this.checkBoxUpdateDatabase.UseVisualStyleBackColor = true;
			this.checkBoxUpdateDatabase.CheckedChanged += new System.EventHandler(this.checkBoxUpdateDatabase_CheckedChanged);
			// 
			// checkBoxTransferIsoDwg
			// 
			this.checkBoxTransferIsoDwg.AutoSize = true;
			this.checkBoxTransferIsoDwg.Location = new System.Drawing.Point(17, 73);
			this.checkBoxTransferIsoDwg.Name = "checkBoxTransferIsoDwg";
			this.checkBoxTransferIsoDwg.Size = new System.Drawing.Size(210, 16);
			this.checkBoxTransferIsoDwg.TabIndex = 2;
			this.checkBoxTransferIsoDwg.Text = "Transfer iso dwg file to ftp server";
			this.checkBoxTransferIsoDwg.UseVisualStyleBackColor = true;
			this.checkBoxTransferIsoDwg.CheckedChanged += new System.EventHandler(this.checkBoxTransferIsoDwg_CheckedChanged);
			// 
			// buttonNO
			// 
			this.buttonNO.Location = new System.Drawing.Point(257, 109);
			this.buttonNO.Name = "buttonNO";
			this.buttonNO.Size = new System.Drawing.Size(75, 23);
			this.buttonNO.TabIndex = 3;
			this.buttonNO.Text = "NO";
			this.buttonNO.UseVisualStyleBackColor = true;
			this.buttonNO.Click += new System.EventHandler(this.buttonNO_Click);
			// 
			// buttonYes
			// 
			this.buttonYes.Location = new System.Drawing.Point(176, 109);
			this.buttonYes.Name = "buttonYes";
			this.buttonYes.Size = new System.Drawing.Size(75, 23);
			this.buttonYes.TabIndex = 4;
			this.buttonYes.Text = "Yes";
			this.buttonYes.UseVisualStyleBackColor = true;
			this.buttonYes.Click += new System.EventHandler(this.buttonYes_Click);
			// 
			// FormIssueConfirm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(344, 144);
			this.Controls.Add(this.buttonYes);
			this.Controls.Add(this.buttonNO);
			this.Controls.Add(this.checkBoxTransferIsoDwg);
			this.Controls.Add(this.checkBoxUpdateDatabase);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormIssueConfirm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Issue Confirm";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox checkBoxUpdateDatabase;
		private System.Windows.Forms.CheckBox checkBoxTransferIsoDwg;
		private System.Windows.Forms.Button buttonNO;
		private System.Windows.Forms.Button buttonYes;
	}
}