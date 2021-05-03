namespace Configuration
{
    partial class UserControlPBS
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
            this.radLabelTotalCount = new Telerik.WinControls.UI.RadLabel();
            this.radCheckBoxExpandAll = new Telerik.WinControls.UI.RadCheckBox();
            this.radTreeViewPBS = new Telerik.WinControls.UI.RadTreeView();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.office2013LightTheme1 = new Telerik.WinControls.Themes.Office2013LightTheme();
            this.windows8Theme1 = new Telerik.WinControls.Themes.Windows8Theme();
            this.pictureBoxBookMarkAdd = new System.Windows.Forms.PictureBox();
            this.pictureBoxBookMark = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTotalCount)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxExpandAll)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTreeViewPBS)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBookMarkAdd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBookMark)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabelTotalCount
            // 
            this.radLabelTotalCount.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.radLabelTotalCount.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabelTotalCount.Location = new System.Drawing.Point(-3, 435);
            this.radLabelTotalCount.Name = "radLabelTotalCount";
            this.radLabelTotalCount.Size = new System.Drawing.Size(181, 17);
            this.radLabelTotalCount.TabIndex = 8;
            this.radLabelTotalCount.Text = "Total Count : 0, Selected Count : 0";
            // 
            // radCheckBoxExpandAll
            // 
            this.radCheckBoxExpandAll.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.radCheckBoxExpandAll.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radCheckBoxExpandAll.Location = new System.Drawing.Point(0, 416);
            this.radCheckBoxExpandAll.Name = "radCheckBoxExpandAll";
            this.radCheckBoxExpandAll.Size = new System.Drawing.Size(72, 17);
            this.radCheckBoxExpandAll.TabIndex = 10;
            this.radCheckBoxExpandAll.Text = "Expand All";
            this.radCheckBoxExpandAll.ToggleStateChanged += new Telerik.WinControls.UI.StateChangedEventHandler(this.radCheckBoxExpandAll_ToggleStateChanged);
            // 
            // radTreeViewPBS
            // 
            this.radTreeViewPBS.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radTreeViewPBS.CheckBoxes = true;
            this.radTreeViewPBS.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTreeViewPBS.Location = new System.Drawing.Point(0, 1);
            this.radTreeViewPBS.Name = "radTreeViewPBS";
            this.radTreeViewPBS.ShowLines = true;
            this.radTreeViewPBS.Size = new System.Drawing.Size(345, 412);
            this.radTreeViewPBS.SpacingBetweenNodes = -1;
            this.radTreeViewPBS.TabIndex = 12;
            this.radTreeViewPBS.Text = "radTreeView1";
            this.radTreeViewPBS.ThemeName = "Office2010Blue";
            this.radTreeViewPBS.NodeCheckedChanged += new Telerik.WinControls.UI.TreeNodeCheckedEventHandler(this.radTreeViewPBS_NodeCheckedChanged);
            // 
            // pictureBoxBookMarkAdd
            // 
            this.pictureBoxBookMarkAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxBookMarkAdd.Location = new System.Drawing.Point(327, 415);
            this.pictureBoxBookMarkAdd.Name = "pictureBoxBookMarkAdd";
            this.pictureBoxBookMarkAdd.Size = new System.Drawing.Size(18, 18);
            this.pictureBoxBookMarkAdd.TabIndex = 13;
            this.pictureBoxBookMarkAdd.TabStop = false;
            // 
            // pictureBoxBookMark
            // 
            this.pictureBoxBookMark.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxBookMark.Location = new System.Drawing.Point(305, 415);
            this.pictureBoxBookMark.Name = "pictureBoxBookMark";
            this.pictureBoxBookMark.Size = new System.Drawing.Size(18, 18);
            this.pictureBoxBookMark.TabIndex = 11;
            this.pictureBoxBookMark.TabStop = false;
            // 
            // UserControlPBS
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.pictureBoxBookMarkAdd);
            this.Controls.Add(this.radCheckBoxExpandAll);
            this.Controls.Add(this.radTreeViewPBS);
            this.Controls.Add(this.pictureBoxBookMark);
            this.Controls.Add(this.radLabelTotalCount);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.Name = "UserControlPBS";
            this.Size = new System.Drawing.Size(345, 450);
            this.Load += new System.EventHandler(this.UserControlPBS_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTotalCount)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxExpandAll)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTreeViewPBS)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBookMarkAdd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxBookMark)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabelTotalCount;
        private Telerik.WinControls.UI.RadCheckBox radCheckBoxExpandAll;
        private System.Windows.Forms.PictureBox pictureBoxBookMark;
        private Telerik.WinControls.UI.RadTreeView radTreeViewPBS;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private Telerik.WinControls.Themes.Office2013LightTheme office2013LightTheme1;
        private Telerik.WinControls.Themes.Windows8Theme windows8Theme1;
        private System.Windows.Forms.PictureBox pictureBoxBookMarkAdd;
    }
}
