namespace SmartISO
{
    partial class ServerMachine
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
			this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
			this.listView_ISSUE_DATE = new System.Windows.Forms.ListView();
			this.groupBox_LineData = new System.Windows.Forms.GroupBox();
			this.btnExport = new System.Windows.Forms.Button();
			this.chkModify = new System.Windows.Forms.CheckBox();
			this.btnDelete = new System.Windows.Forms.Button();
			this.dataGridView_LineData = new System.Windows.Forms.DataGridView();
			this.groupBox_IssueDate = new System.Windows.Forms.GroupBox();
			this.groupBox_NextGrade = new System.Windows.Forms.GroupBox();
			this.listView_GRADE = new System.Windows.Forms.ListView();
			this.dataGridView_RevisionData = new System.Windows.Forms.DataGridView();
			this.chkINSERT = new System.Windows.Forms.CheckBox();
			this.groupBox_RevisionData = new System.Windows.Forms.GroupBox();
			this.chkALL = new System.Windows.Forms.CheckBox();
			this.btnRevDelete = new System.Windows.Forms.Button();
			this.IssueManagerHelpProvider = new System.Windows.Forms.HelpProvider();
			this.panelSelection = new SmartISO.SizeablePanel();
			this.groupBox_Select = new System.Windows.Forms.GroupBox();
			this.btnQuery = new System.Windows.Forms.Button();
			this.lvwREV = new System.Windows.Forms.ListView();
			this.chkREV = new System.Windows.Forms.CheckBox();
			this.checkBox_ISSUE_DATE = new System.Windows.Forms.CheckBox();
			this.comboBox_REVISION_OPTION = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.listISSUE_DATE = new System.Windows.Forms.ListView();
			this.listSECTION = new System.Windows.Forms.ListBox();
			this.label4 = new System.Windows.Forms.Label();
			this.listUNIT = new System.Windows.Forms.ListBox();
			this.label3 = new System.Windows.Forms.Label();
			this.listAREA = new System.Windows.Forms.ListBox();
			this.label2 = new System.Windows.Forms.Label();
			this.listPLANT = new System.Windows.Forms.ListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox_LineData.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView_LineData)).BeginInit();
			this.groupBox_IssueDate.SuspendLayout();
			this.groupBox_NextGrade.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView_RevisionData)).BeginInit();
			this.groupBox_RevisionData.SuspendLayout();
			this.panelSelection.SuspendLayout();
			this.groupBox_Select.SuspendLayout();
			this.SuspendLayout();
			// 
			// OpenFileDialog
			// 
			this.OpenFileDialog.FileName = "openFileDialog1";
			// 
			// listView_ISSUE_DATE
			// 
			this.listView_ISSUE_DATE.Activation = System.Windows.Forms.ItemActivation.OneClick;
			this.listView_ISSUE_DATE.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.listView_ISSUE_DATE.FullRowSelect = true;
			this.listView_ISSUE_DATE.HideSelection = false;
			this.listView_ISSUE_DATE.HotTracking = true;
			this.listView_ISSUE_DATE.HoverSelection = true;
			this.listView_ISSUE_DATE.Location = new System.Drawing.Point(6, 20);
			this.listView_ISSUE_DATE.MultiSelect = false;
			this.listView_ISSUE_DATE.Name = "listView_ISSUE_DATE";
			this.listView_ISSUE_DATE.Size = new System.Drawing.Size(241, 254);
			this.listView_ISSUE_DATE.TabIndex = 7;
			this.listView_ISSUE_DATE.UseCompatibleStateImageBehavior = false;
			this.listView_ISSUE_DATE.View = System.Windows.Forms.View.Details;
			this.listView_ISSUE_DATE.Visible = false;
			// 
			// groupBox_LineData
			// 
			this.groupBox_LineData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox_LineData.Controls.Add(this.btnExport);
			this.groupBox_LineData.Controls.Add(this.chkModify);
			this.groupBox_LineData.Controls.Add(this.btnDelete);
			this.groupBox_LineData.Controls.Add(this.dataGridView_LineData);
			this.groupBox_LineData.Location = new System.Drawing.Point(320, 7);
			this.groupBox_LineData.Name = "groupBox_LineData";
			this.groupBox_LineData.Size = new System.Drawing.Size(1182, 710);
			this.groupBox_LineData.TabIndex = 20;
			this.groupBox_LineData.TabStop = false;
			this.groupBox_LineData.Text = "Line Data";
			// 
			// btnExport
			// 
			this.btnExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnExport.Image = global::IssueManager.Properties.Resources.excel;
			this.btnExport.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.btnExport.Location = new System.Drawing.Point(1042, 20);
			this.btnExport.Name = "btnExport";
			this.btnExport.Size = new System.Drawing.Size(134, 35);
			this.btnExport.TabIndex = 22;
			this.btnExport.Text = "Export";
			this.btnExport.UseVisualStyleBackColor = true;
			this.btnExport.Click += new System.EventHandler(this.btnExport_Click);
			// 
			// chkModify
			// 
			this.chkModify.AutoSize = true;
			this.chkModify.Location = new System.Drawing.Point(133, 31);
			this.chkModify.Name = "chkModify";
			this.chkModify.Size = new System.Drawing.Size(62, 16);
			this.chkModify.TabIndex = 21;
			this.chkModify.Text = "Modify";
			this.chkModify.UseVisualStyleBackColor = true;
			this.chkModify.Visible = false;
			// 
			// btnDelete
			// 
			this.btnDelete.Image = global::IssueManager.Properties.Resources.delete;
			this.btnDelete.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.btnDelete.Location = new System.Drawing.Point(6, 20);
			this.btnDelete.Name = "btnDelete";
			this.btnDelete.Size = new System.Drawing.Size(105, 35);
			this.btnDelete.TabIndex = 14;
			this.btnDelete.Text = "Delete";
			this.btnDelete.UseVisualStyleBackColor = true;
			this.btnDelete.Visible = false;
			// 
			// dataGridView_LineData
			// 
			this.dataGridView_LineData.AllowUserToAddRows = false;
			this.dataGridView_LineData.AllowUserToDeleteRows = false;
			this.dataGridView_LineData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridView_LineData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView_LineData.Location = new System.Drawing.Point(6, 61);
			this.dataGridView_LineData.MultiSelect = false;
			this.dataGridView_LineData.Name = "dataGridView_LineData";
			this.dataGridView_LineData.RowTemplate.Height = 23;
			this.dataGridView_LineData.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
			this.dataGridView_LineData.Size = new System.Drawing.Size(1170, 643);
			this.dataGridView_LineData.TabIndex = 11;
			this.dataGridView_LineData.CurrentCellChanged += new System.EventHandler(this.dataGridView_LineData_CurrentCellChanged);
			this.dataGridView_LineData.CurrentCellDirtyStateChanged += new System.EventHandler(this.dataGridView_LineData_CurrentCellDirtyStateChanged);
			// 
			// groupBox_IssueDate
			// 
			this.groupBox_IssueDate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.groupBox_IssueDate.Controls.Add(this.listView_ISSUE_DATE);
			this.groupBox_IssueDate.Location = new System.Drawing.Point(1249, 717);
			this.groupBox_IssueDate.Name = "groupBox_IssueDate";
			this.groupBox_IssueDate.Size = new System.Drawing.Size(253, 280);
			this.groupBox_IssueDate.TabIndex = 19;
			this.groupBox_IssueDate.TabStop = false;
			this.groupBox_IssueDate.Text = "Issue Date";
			this.groupBox_IssueDate.Visible = false;
			// 
			// groupBox_NextGrade
			// 
			this.groupBox_NextGrade.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.groupBox_NextGrade.Controls.Add(this.listView_GRADE);
			this.groupBox_NextGrade.Location = new System.Drawing.Point(1036, 717);
			this.groupBox_NextGrade.Name = "groupBox_NextGrade";
			this.groupBox_NextGrade.Size = new System.Drawing.Size(205, 280);
			this.groupBox_NextGrade.TabIndex = 18;
			this.groupBox_NextGrade.TabStop = false;
			this.groupBox_NextGrade.Text = "Next grade";
			this.groupBox_NextGrade.Visible = false;
			// 
			// listView_GRADE
			// 
			this.listView_GRADE.Activation = System.Windows.Forms.ItemActivation.OneClick;
			this.listView_GRADE.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.listView_GRADE.FullRowSelect = true;
			this.listView_GRADE.HideSelection = false;
			this.listView_GRADE.HotTracking = true;
			this.listView_GRADE.HoverSelection = true;
			this.listView_GRADE.Location = new System.Drawing.Point(6, 20);
			this.listView_GRADE.MultiSelect = false;
			this.listView_GRADE.Name = "listView_GRADE";
			this.listView_GRADE.Size = new System.Drawing.Size(193, 254);
			this.listView_GRADE.TabIndex = 5;
			this.listView_GRADE.UseCompatibleStateImageBehavior = false;
			this.listView_GRADE.View = System.Windows.Forms.View.Details;
			this.listView_GRADE.Visible = false;
			// 
			// dataGridView_RevisionData
			// 
			this.dataGridView_RevisionData.AllowUserToAddRows = false;
			this.dataGridView_RevisionData.AllowUserToDeleteRows = false;
			this.dataGridView_RevisionData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.dataGridView_RevisionData.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView_RevisionData.Location = new System.Drawing.Point(6, 18);
			this.dataGridView_RevisionData.Name = "dataGridView_RevisionData";
			this.dataGridView_RevisionData.RowTemplate.Height = 23;
			this.dataGridView_RevisionData.Size = new System.Drawing.Size(1170, 257);
			this.dataGridView_RevisionData.TabIndex = 5;
			// 
			// chkINSERT
			// 
			this.chkINSERT.AutoSize = true;
			this.chkINSERT.Location = new System.Drawing.Point(229, 30);
			this.chkINSERT.Name = "chkINSERT";
			this.chkINSERT.Size = new System.Drawing.Size(68, 16);
			this.chkINSERT.TabIndex = 22;
			this.chkINSERT.Text = "INSERT";
			this.chkINSERT.UseVisualStyleBackColor = true;
			this.chkINSERT.Visible = false;
			// 
			// groupBox_RevisionData
			// 
			this.groupBox_RevisionData.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox_RevisionData.Controls.Add(this.chkINSERT);
			this.groupBox_RevisionData.Controls.Add(this.chkALL);
			this.groupBox_RevisionData.Controls.Add(this.btnRevDelete);
			this.groupBox_RevisionData.Controls.Add(this.dataGridView_RevisionData);
			this.groupBox_RevisionData.Location = new System.Drawing.Point(320, 717);
			this.groupBox_RevisionData.Name = "groupBox_RevisionData";
			this.groupBox_RevisionData.Size = new System.Drawing.Size(1183, 280);
			this.groupBox_RevisionData.TabIndex = 17;
			this.groupBox_RevisionData.TabStop = false;
			this.groupBox_RevisionData.Text = "Revision Data";
			// 
			// chkALL
			// 
			this.chkALL.AutoSize = true;
			this.chkALL.Location = new System.Drawing.Point(133, 30);
			this.chkALL.Name = "chkALL";
			this.chkALL.Size = new System.Drawing.Size(46, 16);
			this.chkALL.TabIndex = 21;
			this.chkALL.Text = "ALL";
			this.chkALL.UseVisualStyleBackColor = true;
			this.chkALL.Visible = false;
			// 
			// btnRevDelete
			// 
			this.btnRevDelete.Image = global::IssueManager.Properties.Resources.delete;
			this.btnRevDelete.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			this.btnRevDelete.Location = new System.Drawing.Point(6, 20);
			this.btnRevDelete.Name = "btnRevDelete";
			this.btnRevDelete.Size = new System.Drawing.Size(105, 35);
			this.btnRevDelete.TabIndex = 12;
			this.btnRevDelete.Text = "Delete";
			this.btnRevDelete.UseVisualStyleBackColor = true;
			this.btnRevDelete.Visible = false;
			// 
			// panelSelection
			// 
			this.panelSelection.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.panelSelection.BackColor = System.Drawing.Color.White;
			this.panelSelection.Controls.Add(this.groupBox_Select);
			this.panelSelection.Location = new System.Drawing.Point(3, 0);
			this.panelSelection.Name = "panelSelection";
			this.panelSelection.Size = new System.Drawing.Size(304, 997);
			this.panelSelection.TabIndex = 22;
			// 
			// groupBox_Select
			// 
			this.groupBox_Select.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.groupBox_Select.Controls.Add(this.btnQuery);
			this.groupBox_Select.Controls.Add(this.lvwREV);
			this.groupBox_Select.Controls.Add(this.chkREV);
			this.groupBox_Select.Controls.Add(this.checkBox_ISSUE_DATE);
			this.groupBox_Select.Controls.Add(this.comboBox_REVISION_OPTION);
			this.groupBox_Select.Controls.Add(this.label6);
			this.groupBox_Select.Controls.Add(this.listISSUE_DATE);
			this.groupBox_Select.Controls.Add(this.listSECTION);
			this.groupBox_Select.Controls.Add(this.label4);
			this.groupBox_Select.Controls.Add(this.listUNIT);
			this.groupBox_Select.Controls.Add(this.label3);
			this.groupBox_Select.Controls.Add(this.listAREA);
			this.groupBox_Select.Controls.Add(this.label2);
			this.groupBox_Select.Controls.Add(this.listPLANT);
			this.groupBox_Select.Controls.Add(this.label1);
			this.groupBox_Select.Location = new System.Drawing.Point(6, 7);
			this.groupBox_Select.Name = "groupBox_Select";
			this.groupBox_Select.Size = new System.Drawing.Size(295, 987);
			this.groupBox_Select.TabIndex = 17;
			this.groupBox_Select.TabStop = false;
			this.groupBox_Select.Text = "Select WBS Condition";
			// 
			// btnQuery
			// 
			this.btnQuery.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnQuery.Location = new System.Drawing.Point(183, 946);
			this.btnQuery.Name = "btnQuery";
			this.btnQuery.Size = new System.Drawing.Size(105, 35);
			this.btnQuery.TabIndex = 22;
			this.btnQuery.Text = "Query";
			this.btnQuery.UseVisualStyleBackColor = true;
			this.btnQuery.Click += new System.EventHandler(this.btnQuery_Click);
			// 
			// lvwREV
			// 
			this.lvwREV.CheckBoxes = true;
			this.lvwREV.Location = new System.Drawing.Point(8, 528);
			this.lvwREV.Name = "lvwREV";
			this.lvwREV.Size = new System.Drawing.Size(278, 154);
			this.lvwREV.TabIndex = 19;
			this.lvwREV.UseCompatibleStateImageBehavior = false;
			this.lvwREV.View = System.Windows.Forms.View.Details;
			// 
			// chkREV
			// 
			this.chkREV.AutoSize = true;
			this.chkREV.Location = new System.Drawing.Point(7, 503);
			this.chkREV.Name = "chkREV";
			this.chkREV.Size = new System.Drawing.Size(74, 16);
			this.chkREV.TabIndex = 17;
			this.chkREV.Text = "REV NO.";
			this.chkREV.UseVisualStyleBackColor = true;
			// 
			// checkBox_ISSUE_DATE
			// 
			this.checkBox_ISSUE_DATE.AutoSize = true;
			this.checkBox_ISSUE_DATE.Location = new System.Drawing.Point(8, 688);
			this.checkBox_ISSUE_DATE.Name = "checkBox_ISSUE_DATE";
			this.checkBox_ISSUE_DATE.Size = new System.Drawing.Size(95, 16);
			this.checkBox_ISSUE_DATE.TabIndex = 12;
			this.checkBox_ISSUE_DATE.Text = "ISSUE DATE";
			this.checkBox_ISSUE_DATE.UseVisualStyleBackColor = true;
			// 
			// comboBox_REVISION_OPTION
			// 
			this.comboBox_REVISION_OPTION.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.comboBox_REVISION_OPTION.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBox_REVISION_OPTION.FormattingEnabled = true;
			this.comboBox_REVISION_OPTION.Location = new System.Drawing.Point(7, 914);
			this.comboBox_REVISION_OPTION.Name = "comboBox_REVISION_OPTION";
			this.comboBox_REVISION_OPTION.Size = new System.Drawing.Size(279, 20);
			this.comboBox_REVISION_OPTION.TabIndex = 11;
			// 
			// label6
			// 
			this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(5, 899);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(111, 12);
			this.label6.TabIndex = 10;
			this.label6.Text = "REVISION OPTION";
			// 
			// listISSUE_DATE
			// 
			this.listISSUE_DATE.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.listISSUE_DATE.CheckBoxes = true;
			this.listISSUE_DATE.Location = new System.Drawing.Point(8, 710);
			this.listISSUE_DATE.Name = "listISSUE_DATE";
			this.listISSUE_DATE.Size = new System.Drawing.Size(278, 178);
			this.listISSUE_DATE.TabIndex = 9;
			this.listISSUE_DATE.UseCompatibleStateImageBehavior = false;
			this.listISSUE_DATE.View = System.Windows.Forms.View.Details;
			// 
			// listSECTION
			// 
			this.listSECTION.FormattingEnabled = true;
			this.listSECTION.ItemHeight = 12;
			this.listSECTION.Location = new System.Drawing.Point(8, 409);
			this.listSECTION.Name = "listSECTION";
			this.listSECTION.Size = new System.Drawing.Size(280, 88);
			this.listSECTION.TabIndex = 7;
			this.listSECTION.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listSECTION_MouseDoubleClick);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 394);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(81, 12);
			this.label4.TabIndex = 6;
			this.label4.Text = "SECTION NO";
			// 
			// listUNIT
			// 
			this.listUNIT.FormattingEnabled = true;
			this.listUNIT.ItemHeight = 12;
			this.listUNIT.Location = new System.Drawing.Point(6, 289);
			this.listUNIT.Name = "listUNIT";
			this.listUNIT.Size = new System.Drawing.Size(280, 88);
			this.listUNIT.TabIndex = 5;
			this.listUNIT.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listUNIT_MouseDoubleClick);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 274);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(33, 12);
			this.label3.TabIndex = 4;
			this.label3.Text = "UNIT";
			// 
			// listAREA
			// 
			this.listAREA.FormattingEnabled = true;
			this.listAREA.ItemHeight = 12;
			this.listAREA.Location = new System.Drawing.Point(6, 168);
			this.listAREA.Name = "listAREA";
			this.listAREA.Size = new System.Drawing.Size(280, 88);
			this.listAREA.TabIndex = 3;
			this.listAREA.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listAREA_MouseDoubleClick);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 153);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(37, 12);
			this.label2.TabIndex = 2;
			this.label2.Text = "AREA";
			// 
			// listPLANT
			// 
			this.listPLANT.FormattingEnabled = true;
			this.listPLANT.ItemHeight = 12;
			this.listPLANT.Location = new System.Drawing.Point(8, 47);
			this.listPLANT.Name = "listPLANT";
			this.listPLANT.Size = new System.Drawing.Size(280, 88);
			this.listPLANT.TabIndex = 1;
			this.listPLANT.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listPLANT_MouseDoubleClick);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 31);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "PLANT";
			// 
			// ServerMachine
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.panelSelection);
			this.Controls.Add(this.groupBox_LineData);
			this.Controls.Add(this.groupBox_IssueDate);
			this.Controls.Add(this.groupBox_NextGrade);
			this.Controls.Add(this.groupBox_RevisionData);
			this.Name = "ServerMachine";
			this.Size = new System.Drawing.Size(1514, 1005);
			this.Load += new System.EventHandler(this.ServerMachine_Load);
			this.SizeChanged += new System.EventHandler(this.ServerMachine_SizeChanged);
			this.groupBox_LineData.ResumeLayout(false);
			this.groupBox_LineData.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView_LineData)).EndInit();
			this.groupBox_IssueDate.ResumeLayout(false);
			this.groupBox_NextGrade.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridView_RevisionData)).EndInit();
			this.groupBox_RevisionData.ResumeLayout(false);
			this.groupBox_RevisionData.PerformLayout();
			this.panelSelection.ResumeLayout(false);
			this.groupBox_Select.ResumeLayout(false);
			this.groupBox_Select.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.OpenFileDialog OpenFileDialog;
		private System.Windows.Forms.ListView listView_ISSUE_DATE;
		private System.Windows.Forms.GroupBox groupBox_LineData;
		private System.Windows.Forms.Button btnExport;
		private System.Windows.Forms.CheckBox chkModify;
		private System.Windows.Forms.Button btnDelete;
		private System.Windows.Forms.DataGridView dataGridView_LineData;
		private System.Windows.Forms.GroupBox groupBox_IssueDate;
		private System.Windows.Forms.GroupBox groupBox_NextGrade;
		private System.Windows.Forms.ListView listView_GRADE;
		private System.Windows.Forms.DataGridView dataGridView_RevisionData;
		private System.Windows.Forms.Button btnRevDelete;
		private System.Windows.Forms.CheckBox chkINSERT;
		private System.Windows.Forms.GroupBox groupBox_RevisionData;
		private System.Windows.Forms.CheckBox chkALL;
		private System.Windows.Forms.HelpProvider IssueManagerHelpProvider;
		private SizeablePanel panelSelection;
		private System.Windows.Forms.GroupBox groupBox_Select;
		private System.Windows.Forms.ListView lvwREV;
		private System.Windows.Forms.CheckBox chkREV;
		private System.Windows.Forms.CheckBox checkBox_ISSUE_DATE;
		private System.Windows.Forms.ComboBox comboBox_REVISION_OPTION;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.ListView listISSUE_DATE;
		private System.Windows.Forms.ListBox listSECTION;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ListBox listUNIT;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ListBox listAREA;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ListBox listPLANT;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btnQuery;
    }
}
