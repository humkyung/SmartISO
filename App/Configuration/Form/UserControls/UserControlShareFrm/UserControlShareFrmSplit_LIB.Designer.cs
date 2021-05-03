namespace Configuration
{
    partial class UserControlShareFrmSplit_LIB
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
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListListType = new Telerik.WinControls.UI.RadDropDownList();
            this.ucShareFrmSplit_LIB1 = new EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit_LIB();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(3, 3);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(30, 18);
            this.radLabel2.TabIndex = 14;
            this.radLabel2.Text = "Type";
            // 
            // radDropDownListListType
            // 
            this.radDropDownListListType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListListType.Location = new System.Drawing.Point(71, 2);
            this.radDropDownListListType.Name = "radDropDownListListType";
            this.radDropDownListListType.Size = new System.Drawing.Size(246, 20);
            this.radDropDownListListType.TabIndex = 13;
            this.radDropDownListListType.Text = "radDropDownList2";
            this.radDropDownListListType.ThemeName = "Windows8";
            // 
            // ucShareFrmSplit_LIB1
            // 
            this.ucShareFrmSplit_LIB1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ucShareFrmSplit_LIB1.Location = new System.Drawing.Point(3, 28);
            this.ucShareFrmSplit_LIB1.Name = "ucShareFrmSplit_LIB1";
            this.ucShareFrmSplit_LIB1.Size = new System.Drawing.Size(852, 641);
            this.ucShareFrmSplit_LIB1.TabIndex = 15;
            // 
            // UserControlShareFrmSplit_LIB
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ucShareFrmSplit_LIB1);
            this.Controls.Add(this.radLabel2);
            this.Controls.Add(this.radDropDownListListType);
            this.Name = "UserControlShareFrmSplit_LIB";
            this.Size = new System.Drawing.Size(858, 672);
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListListType;
        private EDCS.WF.BSL.SubForms.UCShare.UCShareFrmSplit_LIB ucShareFrmSplit_LIB1;
    }
}
