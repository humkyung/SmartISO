namespace IsoCreator.Forms {
	partial class VirtualIsoCreator {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose( bool disposing ) {
			if ( disposing && ( components != null ) ) {
				components.Dispose();
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode( "Node5" );
			System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode( "Node13" );
			System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode( "Node14" );
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode( "Node6", new System.Windows.Forms.TreeNode[] {
            treeNode2,
            treeNode3} );
			System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode( "Node7" );
			System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode( "Node1", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode4,
            treeNode5} );
			System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode( "Node15" );
			System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode( "Node16" );
			System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode( "Node17" );
			System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode( "Node18" );
			System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode( "Node8", new System.Windows.Forms.TreeNode[] {
            treeNode7,
            treeNode8,
            treeNode9,
            treeNode10} );
			System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode( "Node9" );
			System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode( "Node2", new System.Windows.Forms.TreeNode[] {
            treeNode11,
            treeNode12} );
			System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode( "Node10" );
			System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode( "Node21" );
			System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode( "Node22" );
			System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode( "Node24" );
			System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode( "Node25" );
			System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode( "Node26" );
			System.Windows.Forms.TreeNode treeNode20 = new System.Windows.Forms.TreeNode( "Node27" );
			System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode( "Node23", new System.Windows.Forms.TreeNode[] {
            treeNode17,
            treeNode18,
            treeNode19,
            treeNode20} );
			System.Windows.Forms.TreeNode treeNode22 = new System.Windows.Forms.TreeNode( "Node19", new System.Windows.Forms.TreeNode[] {
            treeNode15,
            treeNode16,
            treeNode21} );
			System.Windows.Forms.TreeNode treeNode23 = new System.Windows.Forms.TreeNode( "Node20" );
			System.Windows.Forms.TreeNode treeNode24 = new System.Windows.Forms.TreeNode( "Node11", new System.Windows.Forms.TreeNode[] {
            treeNode22,
            treeNode23} );
			System.Windows.Forms.TreeNode treeNode25 = new System.Windows.Forms.TreeNode( "Node12" );
			System.Windows.Forms.TreeNode treeNode26 = new System.Windows.Forms.TreeNode( "Node3", new System.Windows.Forms.TreeNode[] {
            treeNode14,
            treeNode24,
            treeNode25} );
			System.Windows.Forms.TreeNode treeNode27 = new System.Windows.Forms.TreeNode( "Node4" );
			System.Windows.Forms.TreeNode treeNode28 = new System.Windows.Forms.TreeNode( "Node0", new System.Windows.Forms.TreeNode[] {
            treeNode6,
            treeNode13,
            treeNode26,
            treeNode27} );
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.labelStatus = new System.Windows.Forms.Label();
			this.progressBar = new System.Windows.Forms.ProgressBar();
			this.buttonStartAbort = new System.Windows.Forms.Button();
			this.treeView1 = new System.Windows.Forms.TreeView();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.textBoxIsoPath = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.buttonBrowseIso = new System.Windows.Forms.Button();
			this.textBoxVolumeName = new System.Windows.Forms.TextBox();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add( this.labelStatus );
			this.groupBox2.Controls.Add( this.progressBar );
			this.groupBox2.Location = new System.Drawing.Point( 12, 375 );
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size( 378, 59 );
			this.groupBox2.TabIndex = 16;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Progress";
			// 
			// labelStatus
			// 
			this.labelStatus.AutoSize = true;
			this.labelStatus.Dock = System.Windows.Forms.DockStyle.Right;
			this.labelStatus.ForeColor = System.Drawing.Color.DarkRed;
			this.labelStatus.Location = new System.Drawing.Point( 277, 16 );
			this.labelStatus.Name = "labelStatus";
			this.labelStatus.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
			this.labelStatus.Size = new System.Drawing.Size( 98, 13 );
			this.labelStatus.TabIndex = 15;
			this.labelStatus.Text = "Process not started";
			this.labelStatus.TextAlign = System.Drawing.ContentAlignment.TopRight;
			// 
			// progressBar
			// 
			this.progressBar.Location = new System.Drawing.Point( 9, 32 );
			this.progressBar.Name = "progressBar";
			this.progressBar.Size = new System.Drawing.Size( 363, 18 );
			this.progressBar.TabIndex = 14;
			// 
			// buttonStartAbort
			// 
			this.buttonStartAbort.Location = new System.Drawing.Point( 315, 440 );
			this.buttonStartAbort.Name = "buttonStartAbort";
			this.buttonStartAbort.Size = new System.Drawing.Size( 75, 23 );
			this.buttonStartAbort.TabIndex = 17;
			this.buttonStartAbort.Text = "Start";
			this.buttonStartAbort.UseVisualStyleBackColor = true;
			this.buttonStartAbort.Click += new System.EventHandler( this.buttonStartAbort_Click );
			// 
			// treeView1
			// 
			this.treeView1.Location = new System.Drawing.Point( 9, 19 );
			this.treeView1.Name = "treeView1";
			treeNode1.Name = "Node5";
			treeNode1.Text = "Node5";
			treeNode2.Name = "Node13";
			treeNode2.Text = "Node13";
			treeNode3.Name = "Node14";
			treeNode3.Text = "Node14";
			treeNode4.Name = "Node6";
			treeNode4.Text = "Node6";
			treeNode5.Name = "Node7";
			treeNode5.Text = "Node7";
			treeNode6.Name = "Node1";
			treeNode6.Text = "Node1";
			treeNode7.Name = "Node15";
			treeNode7.Text = "Node15";
			treeNode8.Name = "Node16";
			treeNode8.Text = "Node16";
			treeNode9.Name = "Node17";
			treeNode9.Text = "Node17";
			treeNode10.Name = "Node18";
			treeNode10.Text = "Node18";
			treeNode11.Name = "Node8";
			treeNode11.Text = "Node8";
			treeNode12.Name = "Node9";
			treeNode12.Text = "Node9";
			treeNode13.Name = "Node2";
			treeNode13.Text = "Node2";
			treeNode14.Name = "Node10";
			treeNode14.Text = "Node10";
			treeNode15.Name = "Node21";
			treeNode15.Text = "Node21";
			treeNode16.Name = "Node22";
			treeNode16.Text = "Node22";
			treeNode17.Name = "Node24";
			treeNode17.Text = "Node24";
			treeNode18.Name = "Node25";
			treeNode18.Text = "Node25";
			treeNode19.Name = "Node26";
			treeNode19.Text = "Node26";
			treeNode20.Name = "Node27";
			treeNode20.Text = "Node27";
			treeNode21.Name = "Node23";
			treeNode21.Text = "Node23";
			treeNode22.Name = "Node19";
			treeNode22.Text = "Node19";
			treeNode23.Name = "Node20";
			treeNode23.Text = "Node20";
			treeNode24.Name = "Node11";
			treeNode24.Text = "Node11";
			treeNode25.Name = "Node12";
			treeNode25.Text = "Node12";
			treeNode26.Name = "Node3";
			treeNode26.Text = "Node3";
			treeNode27.Name = "Node4";
			treeNode27.Text = "Node4";
			treeNode28.Name = "Node0";
			treeNode28.Text = "Node0";
			this.treeView1.Nodes.AddRange( new System.Windows.Forms.TreeNode[] {
            treeNode28} );
			this.treeView1.Size = new System.Drawing.Size( 363, 250 );
			this.treeView1.TabIndex = 18;
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add( this.treeView1 );
			this.groupBox3.Location = new System.Drawing.Point( 12, 12 );
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size( 378, 275 );
			this.groupBox3.TabIndex = 19;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Virtual ISO Directory Structure";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add( this.textBoxIsoPath );
			this.groupBox1.Controls.Add( this.label3 );
			this.groupBox1.Controls.Add( this.label2 );
			this.groupBox1.Controls.Add( this.buttonBrowseIso );
			this.groupBox1.Controls.Add( this.textBoxVolumeName );
			this.groupBox1.Location = new System.Drawing.Point( 12, 293 );
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size( 378, 76 );
			this.groupBox1.TabIndex = 20;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "ISO Details";
			// 
			// textBoxIsoPath
			// 
			this.textBoxIsoPath.Location = new System.Drawing.Point( 94, 21 );
			this.textBoxIsoPath.Name = "textBoxIsoPath";
			this.textBoxIsoPath.ReadOnly = true;
			this.textBoxIsoPath.Size = new System.Drawing.Size( 197, 20 );
			this.textBoxIsoPath.TabIndex = 8;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point( 6, 50 );
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size( 73, 13 );
			this.label3.TabIndex = 7;
			this.label3.Text = "Volume Name";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point( 6, 24 );
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size( 69, 13 );
			this.label2.TabIndex = 1;
			this.label2.Text = "ISO File Path";
			// 
			// buttonBrowseIso
			// 
			this.buttonBrowseIso.Location = new System.Drawing.Point( 297, 19 );
			this.buttonBrowseIso.Name = "buttonBrowseIso";
			this.buttonBrowseIso.Size = new System.Drawing.Size( 75, 23 );
			this.buttonBrowseIso.TabIndex = 5;
			this.buttonBrowseIso.Text = "Browse...";
			this.buttonBrowseIso.UseVisualStyleBackColor = true;
			this.buttonBrowseIso.Click += new System.EventHandler( this.buttonBrowseIso_Click );
			// 
			// textBoxVolumeName
			// 
			this.textBoxVolumeName.Location = new System.Drawing.Point( 94, 47 );
			this.textBoxVolumeName.Name = "textBoxVolumeName";
			this.textBoxVolumeName.Size = new System.Drawing.Size( 278, 20 );
			this.textBoxVolumeName.TabIndex = 3;
			// 
			// VirtualIsoCreator
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF( 6F, 13F );
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size( 404, 471 );
			this.Controls.Add( this.groupBox1 );
			this.Controls.Add( this.groupBox3 );
			this.Controls.Add( this.buttonStartAbort );
			this.Controls.Add( this.groupBox2 );
			this.Name = "VirtualIsoCreator";
			this.Text = "Virtual ISO Creator";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler( this.VirtualIsoCreator_FormClosing );
			this.groupBox2.ResumeLayout( false );
			this.groupBox2.PerformLayout();
			this.groupBox3.ResumeLayout( false );
			this.groupBox1.ResumeLayout( false );
			this.groupBox1.PerformLayout();
			this.ResumeLayout( false );

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label labelStatus;
		private System.Windows.Forms.ProgressBar progressBar;
		private System.Windows.Forms.Button buttonStartAbort;
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox textBoxIsoPath;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button buttonBrowseIso;
		private System.Windows.Forms.TextBox textBoxVolumeName;
	}
}