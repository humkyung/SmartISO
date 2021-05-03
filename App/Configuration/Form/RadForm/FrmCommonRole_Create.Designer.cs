using TechSun.Theme;
namespace Configuration
{
    partial class FrmCommonRole_Create : KRadForm
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
            this.windows7Theme1 = new Telerik.WinControls.Themes.Windows7Theme();
            this.office2010SilverTheme1 = new Telerik.WinControls.Themes.Office2010SilverTheme();
            this.radGroupBox1 = new Telerik.WinControls.UI.RadGroupBox();
            this.radCheckBoxVisible = new Telerik.WinControls.UI.RadCheckBox();
            this.radTextBoxNameView = new Telerik.WinControls.UI.RadTextBox();
            this.radTextBoxName = new Telerik.WinControls.UI.RadTextBox();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radLabelNameView = new Telerik.WinControls.UI.RadLabel();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.radButtonClose = new Telerik.WinControls.UI.RadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).BeginInit();
            this.radGroupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxVisible)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxNameView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxName)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelNameView)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radGroupBox1
            // 
            this.radGroupBox1.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGroupBox1.Controls.Add(this.radCheckBoxVisible);
            this.radGroupBox1.Controls.Add(this.radTextBoxNameView);
            this.radGroupBox1.Controls.Add(this.radTextBoxName);
            this.radGroupBox1.Controls.Add(this.radLabel2);
            this.radGroupBox1.Controls.Add(this.radLabelNameView);
            this.radGroupBox1.Controls.Add(this.radLabel1);
            this.radGroupBox1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGroupBox1.HeaderText = "";
            this.radGroupBox1.Location = new System.Drawing.Point(13, 10);
            this.radGroupBox1.Name = "radGroupBox1";
            this.radGroupBox1.Size = new System.Drawing.Size(506, 86);
            this.radGroupBox1.TabIndex = 21;
            this.radGroupBox1.ThemeName = "Office2010Silver";
            // 
            // radCheckBoxVisible
            // 
            this.radCheckBoxVisible.CheckState = System.Windows.Forms.CheckState.Checked;
            this.radCheckBoxVisible.Location = new System.Drawing.Point(103, 62);
            this.radCheckBoxVisible.Name = "radCheckBoxVisible";
            this.radCheckBoxVisible.Size = new System.Drawing.Size(15, 15);
            this.radCheckBoxVisible.TabIndex = 4;
            this.radCheckBoxVisible.ThemeName = "Office2010Silver";
            this.radCheckBoxVisible.ToggleState = Telerik.WinControls.Enumerations.ToggleState.On;
            // 
            // radTextBoxNameView
            // 
            this.radTextBoxNameView.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxNameView.Location = new System.Drawing.Point(103, 36);
            this.radTextBoxNameView.Name = "radTextBoxNameView";
            this.radTextBoxNameView.Size = new System.Drawing.Size(398, 19);
            this.radTextBoxNameView.TabIndex = 1;
            this.radTextBoxNameView.TabStop = false;
            this.radTextBoxNameView.ThemeName = "Office2010Silver";
            // 
            // radTextBoxName
            // 
            this.radTextBoxName.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxName.Location = new System.Drawing.Point(103, 12);
            this.radTextBoxName.Name = "radTextBoxName";
            this.radTextBoxName.Size = new System.Drawing.Size(398, 19);
            this.radTextBoxName.TabIndex = 1;
            this.radTextBoxName.TabStop = false;
            this.radTextBoxName.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(5, 60);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(47, 18);
            this.radLabel2.TabIndex = 0;
            this.radLabel2.Text = "Visible : ";
            // 
            // radLabelNameView
            // 
            this.radLabelNameView.Location = new System.Drawing.Point(5, 37);
            this.radLabelNameView.Name = "radLabelNameView";
            this.radLabelNameView.Size = new System.Drawing.Size(72, 18);
            this.radLabelNameView.TabIndex = 0;
            this.radLabelNameView.Text = "Description : ";
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(5, 13);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(37, 18);
            this.radLabel1.TabIndex = 0;
            this.radLabel1.Text = "Role : ";
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonCreate.ForeColor = System.Drawing.Color.Black;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonCreate.Location = new System.Drawing.Point(353, 100);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(80, 23);
            this.radButtonCreate.TabIndex = 23;
            this.radButtonCreate.Text = "OK";
            this.radButtonCreate.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonCreate.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonCreate.ThemeName = "Office2010Silver";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // radButtonClose
            // 
            this.radButtonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonClose.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonClose.ForeColor = System.Drawing.Color.Black;
            this.radButtonClose.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonClose.Location = new System.Drawing.Point(439, 100);
            this.radButtonClose.Name = "radButtonClose";
            this.radButtonClose.Size = new System.Drawing.Size(80, 23);
            this.radButtonClose.TabIndex = 22;
            this.radButtonClose.Text = "Cancel";
            this.radButtonClose.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonClose.ThemeName = "Office2010Silver";
            this.radButtonClose.Click += new System.EventHandler(this.radButtonClose_Click);
            // 
            // FrmCommonRole_Create
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(531, 133);
            this.Controls.Add(this.radGroupBox1);
            this.Controls.Add(this.radButtonCreate);
            this.Controls.Add(this.radButtonClose);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "FrmCommonRole_Create";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Create Role";
            this.ThemeName = "Windows7";
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).EndInit();
            this.radGroupBox1.ResumeLayout(false);
            this.radGroupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxVisible)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxNameView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxName)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelNameView)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
        private Telerik.WinControls.Themes.Office2010SilverTheme office2010SilverTheme1;
        private Telerik.WinControls.UI.RadGroupBox radGroupBox1;
        private Telerik.WinControls.UI.RadTextBox radTextBoxName;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadButton radButtonClose;
        public Telerik.WinControls.UI.RadCheckBox radCheckBoxVisible;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadTextBox radTextBoxNameView;
        private Telerik.WinControls.UI.RadLabel radLabelNameView;
    }
}
