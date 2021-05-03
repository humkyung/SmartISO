namespace SmartISO
{
	partial class FormRemoteDatabaseSetting
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
			this.buttonClose = new System.Windows.Forms.Button();
			this.buttonOK = new System.Windows.Forms.Button();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPageRemoteDatabase = new System.Windows.Forms.TabPage();
			this.buttonConnectionTest = new System.Windows.Forms.Button();
			this.textBoxPassword = new System.Windows.Forms.TextBox();
			this.textBoxId = new System.Windows.Forms.TextBox();
			this.textBoxService = new System.Windows.Forms.TextBox();
			this.textBoxHost = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.tabPageFtpServer = new System.Windows.Forms.TabPage();
			this.textBoxFtpPassword = new System.Windows.Forms.TextBox();
			this.textBoxFtpId = new System.Windows.Forms.TextBox();
			this.textBoxFtpHost = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.textBoxRemoteDirectory = new System.Windows.Forms.TextBox();
			this.label8 = new System.Windows.Forms.Label();
			this.tabControl1.SuspendLayout();
			this.tabPageRemoteDatabase.SuspendLayout();
			this.tabPageFtpServer.SuspendLayout();
			this.SuspendLayout();
			// 
			// buttonClose
			// 
			this.buttonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.buttonClose.Location = new System.Drawing.Point(334, 196);
			this.buttonClose.Name = "buttonClose";
			this.buttonClose.Size = new System.Drawing.Size(87, 28);
			this.buttonClose.TabIndex = 1;
			this.buttonClose.Text = "Close";
			this.buttonClose.UseVisualStyleBackColor = true;
			this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
			// 
			// buttonOK
			// 
			this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.buttonOK.Location = new System.Drawing.Point(240, 196);
			this.buttonOK.Name = "buttonOK";
			this.buttonOK.Size = new System.Drawing.Size(87, 28);
			this.buttonOK.TabIndex = 2;
			this.buttonOK.Text = "OK";
			this.buttonOK.UseVisualStyleBackColor = true;
			this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPageRemoteDatabase);
			this.tabControl1.Controls.Add(this.tabPageFtpServer);
			this.tabControl1.Location = new System.Drawing.Point(6, 12);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(415, 177);
			this.tabControl1.TabIndex = 3;
			// 
			// tabPageRemoteDatabase
			// 
			this.tabPageRemoteDatabase.Controls.Add(this.buttonConnectionTest);
			this.tabPageRemoteDatabase.Controls.Add(this.textBoxPassword);
			this.tabPageRemoteDatabase.Controls.Add(this.textBoxId);
			this.tabPageRemoteDatabase.Controls.Add(this.textBoxService);
			this.tabPageRemoteDatabase.Controls.Add(this.textBoxHost);
			this.tabPageRemoteDatabase.Controls.Add(this.label4);
			this.tabPageRemoteDatabase.Controls.Add(this.label3);
			this.tabPageRemoteDatabase.Controls.Add(this.label2);
			this.tabPageRemoteDatabase.Controls.Add(this.label1);
			this.tabPageRemoteDatabase.Location = new System.Drawing.Point(4, 22);
			this.tabPageRemoteDatabase.Name = "tabPageRemoteDatabase";
			this.tabPageRemoteDatabase.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageRemoteDatabase.Size = new System.Drawing.Size(407, 151);
			this.tabPageRemoteDatabase.TabIndex = 0;
			this.tabPageRemoteDatabase.Text = "Remote Database";
			this.tabPageRemoteDatabase.UseVisualStyleBackColor = true;
			// 
			// buttonConnectionTest
			// 
			this.buttonConnectionTest.Location = new System.Drawing.Point(276, 113);
			this.buttonConnectionTest.Name = "buttonConnectionTest";
			this.buttonConnectionTest.Size = new System.Drawing.Size(119, 28);
			this.buttonConnectionTest.TabIndex = 16;
			this.buttonConnectionTest.Text = "Connection Test";
			this.buttonConnectionTest.UseVisualStyleBackColor = true;
			this.buttonConnectionTest.Click += new System.EventHandler(this.buttonConnectionTest_Click);
			// 
			// textBoxPassword
			// 
			this.textBoxPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxPassword.Location = new System.Drawing.Point(78, 85);
			this.textBoxPassword.Name = "textBoxPassword";
			this.textBoxPassword.PasswordChar = '*';
			this.textBoxPassword.Size = new System.Drawing.Size(318, 21);
			this.textBoxPassword.TabIndex = 15;
			// 
			// textBoxId
			// 
			this.textBoxId.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxId.Location = new System.Drawing.Point(78, 61);
			this.textBoxId.Name = "textBoxId";
			this.textBoxId.Size = new System.Drawing.Size(318, 21);
			this.textBoxId.TabIndex = 14;
			// 
			// textBoxService
			// 
			this.textBoxService.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxService.Location = new System.Drawing.Point(78, 37);
			this.textBoxService.Name = "textBoxService";
			this.textBoxService.Size = new System.Drawing.Size(318, 21);
			this.textBoxService.TabIndex = 13;
			// 
			// textBoxHost
			// 
			this.textBoxHost.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxHost.Location = new System.Drawing.Point(78, 13);
			this.textBoxHost.Name = "textBoxHost";
			this.textBoxHost.Size = new System.Drawing.Size(318, 21);
			this.textBoxHost.TabIndex = 12;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 87);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(70, 12);
			this.label4.TabIndex = 11;
			this.label4.Text = "Password :";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 63);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(23, 12);
			this.label3.TabIndex = 10;
			this.label3.Text = "Id :";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 39);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(55, 12);
			this.label2.TabIndex = 9;
			this.label2.Text = "Service :";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 15);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(38, 12);
			this.label1.TabIndex = 8;
			this.label1.Text = "Host :";
			// 
			// tabPageFtpServer
			// 
			this.tabPageFtpServer.Controls.Add(this.textBoxRemoteDirectory);
			this.tabPageFtpServer.Controls.Add(this.label8);
			this.tabPageFtpServer.Controls.Add(this.textBoxFtpPassword);
			this.tabPageFtpServer.Controls.Add(this.textBoxFtpId);
			this.tabPageFtpServer.Controls.Add(this.textBoxFtpHost);
			this.tabPageFtpServer.Controls.Add(this.label5);
			this.tabPageFtpServer.Controls.Add(this.label6);
			this.tabPageFtpServer.Controls.Add(this.label7);
			this.tabPageFtpServer.Location = new System.Drawing.Point(4, 22);
			this.tabPageFtpServer.Name = "tabPageFtpServer";
			this.tabPageFtpServer.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageFtpServer.Size = new System.Drawing.Size(407, 151);
			this.tabPageFtpServer.TabIndex = 1;
			this.tabPageFtpServer.Text = "Ftp Server";
			this.tabPageFtpServer.UseVisualStyleBackColor = true;
			// 
			// textBoxFtpPassword
			// 
			this.textBoxFtpPassword.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxFtpPassword.Location = new System.Drawing.Point(80, 64);
			this.textBoxFtpPassword.Name = "textBoxFtpPassword";
			this.textBoxFtpPassword.PasswordChar = '*';
			this.textBoxFtpPassword.Size = new System.Drawing.Size(318, 21);
			this.textBoxFtpPassword.TabIndex = 21;
			// 
			// textBoxFtpId
			// 
			this.textBoxFtpId.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxFtpId.Location = new System.Drawing.Point(80, 38);
			this.textBoxFtpId.Name = "textBoxFtpId";
			this.textBoxFtpId.Size = new System.Drawing.Size(318, 21);
			this.textBoxFtpId.TabIndex = 20;
			// 
			// textBoxFtpHost
			// 
			this.textBoxFtpHost.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxFtpHost.Location = new System.Drawing.Point(80, 12);
			this.textBoxFtpHost.Name = "textBoxFtpHost";
			this.textBoxFtpHost.Size = new System.Drawing.Size(318, 21);
			this.textBoxFtpHost.TabIndex = 19;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(8, 68);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(70, 12);
			this.label5.TabIndex = 18;
			this.label5.Text = "Password :";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(8, 42);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(23, 12);
			this.label6.TabIndex = 17;
			this.label6.Text = "Id :";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(8, 16);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(38, 12);
			this.label7.TabIndex = 16;
			this.label7.Text = "Host :";
			// 
			// textBoxRemoteDirectory
			// 
			this.textBoxRemoteDirectory.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxRemoteDirectory.Location = new System.Drawing.Point(123, 91);
			this.textBoxRemoteDirectory.Name = "textBoxRemoteDirectory";
			this.textBoxRemoteDirectory.Size = new System.Drawing.Size(275, 21);
			this.textBoxRemoteDirectory.TabIndex = 23;
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(8, 95);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(109, 12);
			this.label8.TabIndex = 22;
			this.label8.Text = "Remote directory :";
			// 
			// FormRemoteDatabaseSetting
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(427, 231);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.buttonOK);
			this.Controls.Add(this.buttonClose);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormRemoteDatabaseSetting";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Setting";
			this.Load += new System.EventHandler(this.FormRemoteDatabaseSetting_Load);
			this.tabControl1.ResumeLayout(false);
			this.tabPageRemoteDatabase.ResumeLayout(false);
			this.tabPageRemoteDatabase.PerformLayout();
			this.tabPageFtpServer.ResumeLayout(false);
			this.tabPageFtpServer.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button buttonClose;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageRemoteDatabase;
		private System.Windows.Forms.TextBox textBoxPassword;
		private System.Windows.Forms.TextBox textBoxId;
		private System.Windows.Forms.TextBox textBoxService;
		private System.Windows.Forms.TextBox textBoxHost;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TabPage tabPageFtpServer;
		private System.Windows.Forms.TextBox textBoxFtpPassword;
		private System.Windows.Forms.TextBox textBoxFtpId;
		private System.Windows.Forms.TextBox textBoxFtpHost;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Button buttonConnectionTest;
		private System.Windows.Forms.TextBox textBoxRemoteDirectory;
		private System.Windows.Forms.Label label8;
	}
}