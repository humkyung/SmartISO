using TechSun.Theme;
namespace Configuration
{
    partial class FrmSPPIDAttributeSetting_Create : KRadForm
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
            this.radTextBoxNewAttributeSet = new Telerik.WinControls.UI.RadTextBox();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.radButtonClose = new Telerik.WinControls.UI.RadButton();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radGroupBox1 = new Telerik.WinControls.UI.RadGroupBox();
            this.radDropDownListAttributeSet = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxNewAttributeSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).BeginInit();
            this.radGroupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListAttributeSet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radTextBoxNewAttributeSet
            // 
            this.radTextBoxNewAttributeSet.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxNewAttributeSet.Location = new System.Drawing.Point(103, 12);
            this.radTextBoxNewAttributeSet.Name = "radTextBoxNewAttributeSet";
            this.radTextBoxNewAttributeSet.Size = new System.Drawing.Size(206, 19);
            this.radTextBoxNewAttributeSet.TabIndex = 1;
            this.radTextBoxNewAttributeSet.TabStop = false;
            this.radTextBoxNewAttributeSet.ThemeName = "Office2010Silver";
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonCreate.ForeColor = System.Drawing.Color.Black;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonCreate.Location = new System.Drawing.Point(173, 85);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(80, 23);
            this.radButtonCreate.TabIndex = 20;
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
            this.radButtonClose.Location = new System.Drawing.Point(259, 85);
            this.radButtonClose.Name = "radButtonClose";
            this.radButtonClose.Size = new System.Drawing.Size(80, 23);
            this.radButtonClose.TabIndex = 19;
            this.radButtonClose.Text = "Cancel";
            this.radButtonClose.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonClose.ThemeName = "Office2010Silver";
            this.radButtonClose.Click += new System.EventHandler(this.radButtonClose_Click);
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(5, 13);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(45, 18);
            this.radLabel1.TabIndex = 0;
            this.radLabel1.Text = "Name : ";
            // 
            // radGroupBox1
            // 
            this.radGroupBox1.AccessibleRole = System.Windows.Forms.AccessibleRole.Grouping;
            this.radGroupBox1.Controls.Add(this.radDropDownListAttributeSet);
            this.radGroupBox1.Controls.Add(this.radLabel2);
            this.radGroupBox1.Controls.Add(this.radTextBoxNewAttributeSet);
            this.radGroupBox1.Controls.Add(this.radLabel1);
            this.radGroupBox1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radGroupBox1.HeaderText = "";
            this.radGroupBox1.Location = new System.Drawing.Point(12, 12);
            this.radGroupBox1.Name = "radGroupBox1";
            this.radGroupBox1.Size = new System.Drawing.Size(327, 69);
            this.radGroupBox1.TabIndex = 18;
            this.radGroupBox1.ThemeName = "Office2010Silver";
            // 
            // radDropDownListAttributeSet
            // 
            this.radDropDownListAttributeSet.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListAttributeSet.Location = new System.Drawing.Point(103, 37);
            this.radDropDownListAttributeSet.Name = "radDropDownListAttributeSet";
            this.radDropDownListAttributeSet.Size = new System.Drawing.Size(206, 20);
            this.radDropDownListAttributeSet.TabIndex = 16;
            this.radDropDownListAttributeSet.ThemeName = "Office2010Silver";
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(5, 37);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(94, 18);
            this.radLabel2.TabIndex = 15;
            this.radLabel2.Text = "Template Name : ";
            // 
            // FrmSPPIDAttributeSetting_Create
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(352, 116);
            this.Controls.Add(this.radGroupBox1);
            this.Controls.Add(this.radButtonCreate);
            this.Controls.Add(this.radButtonClose);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "FrmSPPIDAttributeSetting_Create";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "New Attribute Set";
            this.ThemeName = "Windows7";
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxNewAttributeSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGroupBox1)).EndInit();
            this.radGroupBox1.ResumeLayout(false);
            this.radGroupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListAttributeSet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadTextBox radTextBoxNewAttributeSet;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadButton radButtonClose;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadGroupBox radGroupBox1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListAttributeSet;
        private Telerik.WinControls.UI.RadLabel radLabel2;
    }
}
