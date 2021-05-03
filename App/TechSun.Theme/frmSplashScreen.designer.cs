namespace TechSun.Theme
{
    partial class frmSplashScreen
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmSplashScreen));
            this.labelProgress = new System.Windows.Forms.Label();
            this.panel11 = new System.Windows.Forms.Panel();
            this.radLabelTooltipText = new Telerik.WinControls.UI.RadLabel();
            this.radLabelTooltip = new Telerik.WinControls.UI.RadLabel();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.windows7Theme1 = new Telerik.WinControls.Themes.Windows7Theme();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltipText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltip)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // labelProgress
            // 
            this.labelProgress.BackColor = System.Drawing.Color.White;
            this.labelProgress.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelProgress.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(45)))));
            this.labelProgress.Location = new System.Drawing.Point(4, 135);
            this.labelProgress.Name = "labelProgress";
            this.labelProgress.Size = new System.Drawing.Size(419, 22);
            this.labelProgress.TabIndex = 1;
            this.labelProgress.Text = "Loading...";
            this.labelProgress.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelProgress.UseWaitCursor = true;
            // 
            // panel11
            // 
            this.panel11.BackColor = System.Drawing.Color.Black;
            this.panel11.Location = new System.Drawing.Point(17, 183);
            this.panel11.Name = "panel11";
            this.panel11.Size = new System.Drawing.Size(393, 1);
            this.panel11.TabIndex = 10;
            this.panel11.UseWaitCursor = true;
            // 
            // radLabelTooltipText
            // 
            this.radLabelTooltipText.AutoSize = false;
            this.radLabelTooltipText.Font = new System.Drawing.Font("Malgun Gothic", 8.25F);
            this.radLabelTooltipText.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.radLabelTooltipText.Location = new System.Drawing.Point(31, 210);
            this.radLabelTooltipText.Name = "radLabelTooltipText";
            this.radLabelTooltipText.Size = new System.Drawing.Size(365, 76);
            this.radLabelTooltipText.TabIndex = 12;
            this.radLabelTooltipText.UseWaitCursor = true;
            // 
            // radLabelTooltip
            // 
            this.radLabelTooltip.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold);
            this.radLabelTooltip.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(52)))), ((int)(((byte)(35)))), ((int)(((byte)(35)))));
            this.radLabelTooltip.Location = new System.Drawing.Point(31, 189);
            this.radLabelTooltip.Name = "radLabelTooltip";
            this.radLabelTooltip.Size = new System.Drawing.Size(59, 20);
            this.radLabelTooltip.TabIndex = 11;
            this.radLabelTooltip.Text = "Tool tip";
            this.radLabelTooltip.UseWaitCursor = true;
            // 
            // radPanel1
            // 
            this.radPanel1.BackColor = System.Drawing.Color.Transparent;
            this.radPanel1.Controls.Add(this.labelProgress);
            this.radPanel1.Controls.Add(this.radLabelTooltipText);
            this.radPanel1.Controls.Add(this.pictureBox1);
            this.radPanel1.Controls.Add(this.radLabelTooltip);
            this.radPanel1.Controls.Add(this.panel11);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(428, 301);
            this.radPanel1.TabIndex = 13;
            this.radPanel1.UseWaitCursor = true;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).Width = 4F;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94)))));
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).BackColor = System.Drawing.Color.White;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Visible;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.White;
            this.pictureBox1.Image = global::TechSun.Theme.Properties.Resources.Preloader_2;
            this.pictureBox1.Location = new System.Drawing.Point(148, 5);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(128, 128);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.UseWaitCursor = true;
            // 
            // frmSplashScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(427, 300);
            this.Controls.Add(this.radPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmSplashScreen";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Loading...";
            this.TopMost = true;
            this.UseWaitCursor = true;
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltipText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltip)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label labelProgress;
        private System.Windows.Forms.Panel panel11;
        private Telerik.WinControls.UI.RadLabel radLabelTooltipText;
        private Telerik.WinControls.UI.RadLabel radLabelTooltip;
        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
    }
}