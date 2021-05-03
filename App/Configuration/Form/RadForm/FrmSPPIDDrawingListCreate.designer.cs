using TechSun.Theme;
namespace Configuration
{
    partial class FrmSPPIDDrawingListCreate : KRadForm
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
            this.radGroupBox1 = new Telerik.WinControls.UI.RadGroupBox();
            this.radTextBoxCategoryName = new Telerik.WinControls.UI.RadTextBox();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radGroupBox2 = new Telerik.WinControls.UI.RadGroupBox();
            this.radLabelRowCount = new Telerik.WinControls.UI.RadLabel();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.fpSpreadDrawingList = new FarPoint.Win.Spread.FpSpread();
            this.radButtonRemoveAll = new Telerik.WinControls.UI.RadButton();
            this.radButtonRemove = new Telerik.WinControls.UI.RadButton();
            this.radButtonAdd = new Telerik.WinControls.UI.RadButton();
            this.radButtonAddAll = new Telerik.WinControls.UI.RadButton();
            this.radButtonClose = new Telerik.WinControls.UI.RadButton();
            this.radButtonOk = new Telerik.WinControls.UI.RadButton();
            this.radGroupBox3 = new Telerik.WinControls.UI.RadGroupBox();
            this.radLabelFontColor = new Telerik.WinControls.UI.RadLabel();
            this.radLabel3 = new Telerik.WinControls.UI.RadLabel();
            this.radTextBoxDisplayLabel = new Telerik.WinControls.UI.RadTextBox();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.userControlPBS = new Configuration.UserControlPBS();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).BeginInit();
            this.radGroupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxCategoryName)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox2)).BeginInit();
            this.radGroupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelRowCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDrawingList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveAll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemove)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAddAll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOk)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox3)).BeginInit();
            this.radGroupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelFontColor)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxDisplayLabel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radGroupBox1
            // 
            this.radGroupBox1.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGroupBox1.Controls.Add(this.radTextBoxCategoryName);
            this.radGroupBox1.Controls.Add(this.radLabel1);
            this.radGroupBox1.HeaderText = "";
            this.radGroupBox1.Location = new System.Drawing.Point(12, 12);
            this.radGroupBox1.Name = "radGroupBox1";
            this.radGroupBox1.Size = new System.Drawing.Size(652, 44);
            this.radGroupBox1.TabIndex = 1;
            this.radGroupBox1.ThemeName = "Office2010Silver";
            // 
            // radTextBoxCategoryName
            // 
            this.radTextBoxCategoryName.Location = new System.Drawing.Point(69, 13);
            this.radTextBoxCategoryName.Name = "radTextBoxCategoryName";
            this.radTextBoxCategoryName.Size = new System.Drawing.Size(216, 20);
            this.radTextBoxCategoryName.TabIndex = 2;
            this.radTextBoxCategoryName.TabStop = false;
            this.radTextBoxCategoryName.ThemeName = "Office2010Silver";
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(8, 14);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(57, 18);
            this.radLabel1.TabIndex = 1;
            this.radLabel1.Text = "Category :";
            // 
            // radGroupBox2
            // 
            this.radGroupBox2.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGroupBox2.Controls.Add(this.radLabelRowCount);
            this.radGroupBox2.Controls.Add(this.radPanel1);
            this.radGroupBox2.Controls.Add(this.fpSpreadDrawingList);
            this.radGroupBox2.Controls.Add(this.radButtonRemoveAll);
            this.radGroupBox2.Controls.Add(this.radButtonRemove);
            this.radGroupBox2.Controls.Add(this.radButtonAdd);
            this.radGroupBox2.Controls.Add(this.radButtonAddAll);
            this.radGroupBox2.HeaderText = "";
            this.radGroupBox2.Location = new System.Drawing.Point(12, 138);
            this.radGroupBox2.Name = "radGroupBox2";
            this.radGroupBox2.Size = new System.Drawing.Size(652, 547);
            this.radGroupBox2.TabIndex = 2;
            this.radGroupBox2.ThemeName = "Office2010Silver";
            // 
            // radLabelRowCount
            // 
            this.radLabelRowCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.radLabelRowCount.BackColor = System.Drawing.Color.Transparent;
            this.radLabelRowCount.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabelRowCount.Location = new System.Drawing.Point(342, 525);
            this.radLabelRowCount.Name = "radLabelRowCount";
            this.radLabelRowCount.Size = new System.Drawing.Size(78, 17);
            this.radLabelRowCount.TabIndex = 18;
            this.radLabelRowCount.Text = "Row Count : 0";
            // 
            // radPanel1
            // 
            this.radPanel1.BackColor = System.Drawing.Color.Transparent;
            this.radPanel1.Controls.Add(this.userControlPBS);
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(12, 15);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(278, 520);
            this.radPanel1.TabIndex = 17;
            this.radPanel1.Text = "radPanel1";
            ((Telerik.WinControls.UI.RadPanelElement)(this.radPanel1.GetChildAt(0))).Text = "radPanel1";
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Collapsed;
            // 
            // fpSpreadDrawingList
            // 
            this.fpSpreadDrawingList.AccessibleDescription = "";
            this.fpSpreadDrawingList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.fpSpreadDrawingList.Location = new System.Drawing.Point(343, 14);
            this.fpSpreadDrawingList.Name = "fpSpreadDrawingList";
            this.fpSpreadDrawingList.Size = new System.Drawing.Size(299, 509);
            this.fpSpreadDrawingList.TabIndex = 16;
            // 
            // radButtonRemoveAll
            // 
            this.radButtonRemoveAll.Location = new System.Drawing.Point(302, 321);
            this.radButtonRemoveAll.Name = "radButtonRemoveAll";
            this.radButtonRemoveAll.Size = new System.Drawing.Size(34, 24);
            this.radButtonRemoveAll.TabIndex = 9;
            this.radButtonRemoveAll.Text = "<<";
            this.radButtonRemoveAll.ThemeName = "Office2010Silver";
            this.radButtonRemoveAll.Click += new System.EventHandler(this.radButtonRemoveAll_Click);
            // 
            // radButtonRemove
            // 
            this.radButtonRemove.Location = new System.Drawing.Point(302, 291);
            this.radButtonRemove.Name = "radButtonRemove";
            this.radButtonRemove.Size = new System.Drawing.Size(34, 24);
            this.radButtonRemove.TabIndex = 8;
            this.radButtonRemove.Text = "<";
            this.radButtonRemove.ThemeName = "Office2010Silver";
            this.radButtonRemove.Click += new System.EventHandler(this.radButtonRemove_Click);
            // 
            // radButtonAdd
            // 
            this.radButtonAdd.Location = new System.Drawing.Point(301, 249);
            this.radButtonAdd.Name = "radButtonAdd";
            this.radButtonAdd.Size = new System.Drawing.Size(34, 24);
            this.radButtonAdd.TabIndex = 7;
            this.radButtonAdd.Text = ">";
            this.radButtonAdd.ThemeName = "Office2010Silver";
            this.radButtonAdd.Click += new System.EventHandler(this.radButtonAdd_Click);
            // 
            // radButtonAddAll
            // 
            this.radButtonAddAll.Location = new System.Drawing.Point(301, 219);
            this.radButtonAddAll.Name = "radButtonAddAll";
            this.radButtonAddAll.Size = new System.Drawing.Size(34, 24);
            this.radButtonAddAll.TabIndex = 6;
            this.radButtonAddAll.Text = ">>";
            this.radButtonAddAll.ThemeName = "Office2010Silver";
            this.radButtonAddAll.Click += new System.EventHandler(this.radButtonAddAll_Click);
            // 
            // radButtonClose
            // 
            this.radButtonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonClose.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonClose.ForeColor = System.Drawing.Color.Black;
            this.radButtonClose.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonClose.Location = new System.Drawing.Point(582, 690);
            this.radButtonClose.Name = "radButtonClose";
            this.radButtonClose.Size = new System.Drawing.Size(80, 23);
            this.radButtonClose.TabIndex = 15;
            this.radButtonClose.Text = "Cancel";
            this.radButtonClose.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonClose.ThemeName = "Office2010Silver";
            this.radButtonClose.Click += new System.EventHandler(this.radButtonClose_Click);
            // 
            // radButtonOk
            // 
            this.radButtonOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonOk.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonOk.ForeColor = System.Drawing.Color.Black;
            this.radButtonOk.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonOk.Location = new System.Drawing.Point(496, 690);
            this.radButtonOk.Name = "radButtonOk";
            this.radButtonOk.Size = new System.Drawing.Size(80, 23);
            this.radButtonOk.TabIndex = 16;
            this.radButtonOk.Text = " Ok";
            this.radButtonOk.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonOk.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonOk.ThemeName = "Office2010Silver";
            this.radButtonOk.Click += new System.EventHandler(this.radButtonOk_Click);
            // 
            // radGroupBox3
            // 
            this.radGroupBox3.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox3.Controls.Add(this.radLabelFontColor);
            this.radGroupBox3.Controls.Add(this.radLabel3);
            this.radGroupBox3.Controls.Add(this.radTextBoxDisplayLabel);
            this.radGroupBox3.Controls.Add(this.radLabel2);
            this.radGroupBox3.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGroupBox3.HeaderText = "Display Setting";
            this.radGroupBox3.Location = new System.Drawing.Point(12, 62);
            this.radGroupBox3.Name = "radGroupBox3";
            this.radGroupBox3.Size = new System.Drawing.Size(652, 70);
            this.radGroupBox3.TabIndex = 17;
            this.radGroupBox3.Text = "Display Setting";
            this.radGroupBox3.ThemeName = "Office2010Silver";
            // 
            // radLabelFontColor
            // 
            this.radLabelFontColor.AutoSize = false;
            this.radLabelFontColor.BackColor = System.Drawing.Color.Black;
            this.radLabelFontColor.Location = new System.Drawing.Point(93, 44);
            this.radLabelFontColor.Name = "radLabelFontColor";
            this.radLabelFontColor.Size = new System.Drawing.Size(59, 17);
            this.radLabelFontColor.TabIndex = 23;
            this.radLabelFontColor.Click += new System.EventHandler(this.radLabelFontColor_Click);
            // 
            // radLabel3
            // 
            this.radLabel3.Location = new System.Drawing.Point(10, 44);
            this.radLabel3.Name = "radLabel3";
            this.radLabel3.Size = new System.Drawing.Size(67, 18);
            this.radLabel3.TabIndex = 2;
            this.radLabel3.Text = "Font Color : ";
            // 
            // radTextBoxDisplayLabel
            // 
            this.radTextBoxDisplayLabel.Location = new System.Drawing.Point(92, 20);
            this.radTextBoxDisplayLabel.Name = "radTextBoxDisplayLabel";
            this.radTextBoxDisplayLabel.Size = new System.Drawing.Size(209, 20);
            this.radTextBoxDisplayLabel.TabIndex = 1;
            this.radTextBoxDisplayLabel.TabStop = false;
            this.radTextBoxDisplayLabel.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(9, 21);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(81, 18);
            this.radLabel2.TabIndex = 0;
            this.radLabel2.Text = "Display Label : ";
            // 
            // userControlPBS
            // 
            this.userControlPBS.BackColor = System.Drawing.Color.Transparent;
            this.userControlPBS.Dock = System.Windows.Forms.DockStyle.Fill;
            this.userControlPBS.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.userControlPBS.Location = new System.Drawing.Point(0, 0);
            this.userControlPBS.Name = "userControlPBS";
            this.userControlPBS.Size = new System.Drawing.Size(278, 520);
            this.userControlPBS.TabIndex = 0;
            // 
            // FrmDrawingList_Create
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(674, 717);
            this.Controls.Add(this.radGroupBox3);
            this.Controls.Add(this.radButtonOk);
            this.Controls.Add(this.radButtonClose);
            this.Controls.Add(this.radGroupBox2);
            this.Controls.Add(this.radGroupBox1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FrmDrawingList_Create";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Drawing List Setting :: Add/Edit";
            this.ThemeName = "Windows7";
            this.Load += new System.EventHandler(this.FrmDrawingList_Create_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).EndInit();
            this.radGroupBox1.ResumeLayout(false);
            this.radGroupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxCategoryName)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox2)).EndInit();
            this.radGroupBox2.ResumeLayout(false);
            this.radGroupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelRowCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadDrawingList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveAll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemove)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAdd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAddAll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOk)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox3)).EndInit();
            this.radGroupBox3.ResumeLayout(false);
            this.radGroupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelFontColor)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxDisplayLabel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadGroupBox radGroupBox1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadGroupBox radGroupBox2;
        private Telerik.WinControls.UI.RadButton radButtonRemoveAll;
        private Telerik.WinControls.UI.RadButton radButtonRemove;
        private Telerik.WinControls.UI.RadButton radButtonAdd;
        private Telerik.WinControls.UI.RadButton radButtonAddAll;
        private FarPoint.Win.Spread.FpSpread fpSpreadDrawingList;
        private Telerik.WinControls.UI.RadButton radButtonClose;
        private Telerik.WinControls.UI.RadTextBox radTextBoxCategoryName;
        private Telerik.WinControls.UI.RadButton radButtonOk;
        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabelRowCount;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadGroupBox radGroupBox3;
        private Telerik.WinControls.UI.RadLabel radLabel3;
        private Telerik.WinControls.UI.RadTextBox radTextBoxDisplayLabel;
        public Telerik.WinControls.UI.RadLabel radLabelFontColor;
        private UserControlPBS userControlPBS;
    }
}
