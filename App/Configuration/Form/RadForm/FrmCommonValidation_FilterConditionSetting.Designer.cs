using TechSun.Theme;
namespace Configuration
{
    partial class FrmCommonValidation_FilterConditionSetting : KRadForm
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
            this.kRadButtonOK = new TechSun.Theme.KRadButton();
            this.kRadButtonClose = new TechSun.Theme.KRadButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.userControlFilter1 = new Configuration.UserControlFilter();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.radCheckBoxSkipErrorCheck = new Telerik.WinControls.UI.RadCheckBox();
            this.radTextBoxComment = new Telerik.WinControls.UI.RadTextBox();
            this.radLabel3 = new Telerik.WinControls.UI.RadLabel();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.radDropDownListPlant = new Telerik.WinControls.UI.RadDropDownList();
            this.fpSpreadQueryResult = new FarPoint.Win.Spread.FpSpread();
            this.kRadButtonExcute = new TechSun.Theme.KRadButton();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonOK)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonClose)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxSkipErrorCheck)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxComment)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListPlant)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadQueryResult)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonExcute)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // kRadButtonOK
            // 
            this.kRadButtonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.kRadButtonOK.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonOK.Location = new System.Drawing.Point(562, 536);
            this.kRadButtonOK.Name = "kRadButtonOK";
            this.kRadButtonOK.Size = new System.Drawing.Size(80, 26);
            this.kRadButtonOK.TabIndex = 2;
            this.kRadButtonOK.Text = "OK";
            this.kRadButtonOK.ThemeName = "Windows8";
            this.kRadButtonOK.Click += new System.EventHandler(this.kRadButtonOK_Click);
            // 
            // kRadButtonClose
            // 
            this.kRadButtonClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.kRadButtonClose.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonClose.Location = new System.Drawing.Point(648, 536);
            this.kRadButtonClose.Name = "kRadButtonClose";
            this.kRadButtonClose.Size = new System.Drawing.Size(80, 26);
            this.kRadButtonClose.TabIndex = 3;
            this.kRadButtonClose.Text = "Close";
            this.kRadButtonClose.ThemeName = "Windows8";
            this.kRadButtonClose.Click += new System.EventHandler(this.kRadButtonClose_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.userControlFilter1);
            this.groupBox1.Location = new System.Drawing.Point(15, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(713, 242);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Filter";
            // 
            // userControlFilter1
            // 
            this.userControlFilter1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.userControlFilter1.BackColor = System.Drawing.Color.White;
            this.userControlFilter1.Location = new System.Drawing.Point(6, 13);
            this.userControlFilter1.Name = "userControlFilter1";
            this.userControlFilter1.Size = new System.Drawing.Size(700, 225);
            this.userControlFilter1.TabIndex = 1;
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.radCheckBoxSkipErrorCheck);
            this.groupBox2.Controls.Add(this.radTextBoxComment);
            this.groupBox2.Controls.Add(this.radLabel3);
            this.groupBox2.Location = new System.Drawing.Point(15, 260);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(713, 71);
            this.groupBox2.TabIndex = 6;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Comment && Option";
            // 
            // radCheckBoxSkipErrorCheck
            // 
            this.radCheckBoxSkipErrorCheck.CheckState = System.Windows.Forms.CheckState.Checked;
            this.radCheckBoxSkipErrorCheck.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radCheckBoxSkipErrorCheck.Location = new System.Drawing.Point(6, 45);
            this.radCheckBoxSkipErrorCheck.Name = "radCheckBoxSkipErrorCheck";
            this.radCheckBoxSkipErrorCheck.Size = new System.Drawing.Size(103, 17);
            this.radCheckBoxSkipErrorCheck.TabIndex = 5;
            this.radCheckBoxSkipErrorCheck.Text = "Skip Check Error";
            this.radCheckBoxSkipErrorCheck.ThemeName = "Windows8";
            this.radCheckBoxSkipErrorCheck.ToggleState = Telerik.WinControls.Enumerations.ToggleState.On;
            // 
            // radTextBoxComment
            // 
            this.radTextBoxComment.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radTextBoxComment.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxComment.Location = new System.Drawing.Point(76, 20);
            this.radTextBoxComment.Name = "radTextBoxComment";
            this.radTextBoxComment.Size = new System.Drawing.Size(631, 19);
            this.radTextBoxComment.TabIndex = 4;
            this.radTextBoxComment.ThemeName = "Office2010Silver";
            // 
            // radLabel3
            // 
            this.radLabel3.Location = new System.Drawing.Point(6, 21);
            this.radLabel3.Name = "radLabel3";
            this.radLabel3.Size = new System.Drawing.Size(64, 18);
            this.radLabel3.TabIndex = 3;
            this.radLabel3.Text = "Comment : ";
            this.radLabel3.ThemeName = "Windows8";
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.radDropDownListPlant);
            this.groupBox3.Controls.Add(this.fpSpreadQueryResult);
            this.groupBox3.Controls.Add(this.kRadButtonExcute);
            this.groupBox3.Controls.Add(this.radLabel2);
            this.groupBox3.Location = new System.Drawing.Point(15, 337);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(713, 190);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Filter Result";
            // 
            // radDropDownListPlant
            // 
            this.radDropDownListPlant.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListPlant.ForeColor = System.Drawing.Color.Black;
            this.radDropDownListPlant.Location = new System.Drawing.Point(51, 20);
            this.radDropDownListPlant.Name = "radDropDownListPlant";
            this.radDropDownListPlant.Size = new System.Drawing.Size(206, 20);
            this.radDropDownListPlant.TabIndex = 71;
            this.radDropDownListPlant.ThemeName = "Office2010Silver";
            // 
            // fpSpreadQueryResult
            // 
            this.fpSpreadQueryResult.AccessibleDescription = "";
            this.fpSpreadQueryResult.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadQueryResult.Location = new System.Drawing.Point(5, 48);
            this.fpSpreadQueryResult.Name = "fpSpreadQueryResult";
            this.fpSpreadQueryResult.Size = new System.Drawing.Size(703, 136);
            this.fpSpreadQueryResult.TabIndex = 70;
            // 
            // kRadButtonExcute
            // 
            this.kRadButtonExcute.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.kRadButtonExcute.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kRadButtonExcute.Location = new System.Drawing.Point(598, 16);
            this.kRadButtonExcute.Name = "kRadButtonExcute";
            this.kRadButtonExcute.Size = new System.Drawing.Size(110, 26);
            this.kRadButtonExcute.TabIndex = 69;
            this.kRadButtonExcute.Text = "Excute";
            this.kRadButtonExcute.ThemeName = "Office2013Light";
            this.kRadButtonExcute.Click += new System.EventHandler(this.kRadButtonExcute_Click);
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(5, 20);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(40, 18);
            this.radLabel2.TabIndex = 68;
            this.radLabel2.Text = "Plant : ";
            this.radLabel2.ThemeName = "Windows8";
            // 
            // FrmCommonValidation_FilterConditionSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(743, 588);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.kRadButtonClose);
            this.Controls.Add(this.kRadButtonOK);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "FrmCommonValidation_FilterConditionSetting";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.Text = "   Filter Condition Setting";
            this.ThemeName = "Windows8";
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonOK)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonClose)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radCheckBoxSkipErrorCheck)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxComment)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListPlant)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadQueryResult)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kRadButtonExcute)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private KRadButton kRadButtonOK;
        private KRadButton kRadButtonClose;
        private System.Windows.Forms.GroupBox groupBox1;
        private UserControlFilter userControlFilter1;
        private System.Windows.Forms.GroupBox groupBox2;
        private Telerik.WinControls.UI.RadCheckBox radCheckBoxSkipErrorCheck;
        private Telerik.WinControls.UI.RadTextBox radTextBoxComment;
        private Telerik.WinControls.UI.RadLabel radLabel3;
        private System.Windows.Forms.GroupBox groupBox3;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListPlant;
        private FarPoint.Win.Spread.FpSpread fpSpreadQueryResult;
        private KRadButton kRadButtonExcute;
        private Telerik.WinControls.UI.RadLabel radLabel2;
    }
}