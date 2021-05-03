using TechSun.Theme;
namespace Configuration
{
    partial class FrmProgressStatus
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmProgressStatus));
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.textBoxTime = new System.Windows.Forms.TextBox();
            this.btnCustom = new TechSun.Theme.KRadButton();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.labelProgress = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.radLabelTooltip = new Telerik.WinControls.UI.RadLabel();
            this.panel11 = new System.Windows.Forms.Panel();
            this.radLabelTooltipText = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.btnCustom)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltip)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltipText)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(21, 339);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(1, 1);
            this.progressBar1.TabIndex = 0;
            this.progressBar1.UseWaitCursor = true;
            this.progressBar1.Value = 5;
            // 
            // textBoxTime
            // 
            this.textBoxTime.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(241)))), ((int)(((byte)(242)))));
            this.textBoxTime.Location = new System.Drawing.Point(316, 274);
            this.textBoxTime.Name = "textBoxTime";
            this.textBoxTime.ReadOnly = true;
            this.textBoxTime.Size = new System.Drawing.Size(1, 21);
            this.textBoxTime.TabIndex = 5;
            this.textBoxTime.TabStop = false;
            this.textBoxTime.UseWaitCursor = true;
            // 
            // btnCustom
            // 
            this.btnCustom.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.btnCustom.Location = new System.Drawing.Point(167, 292);
            this.btnCustom.Name = "btnCustom";
            this.btnCustom.Size = new System.Drawing.Size(84, 26);
            this.btnCustom.TabIndex = 6;
            this.btnCustom.Text = " Stop";
            this.btnCustom.ThemeName = "Office2013Light";
            this.btnCustom.UseWaitCursor = true;
            // 
            // radPanel1
            // 
            this.radPanel1.BackColor = System.Drawing.Color.White;
            this.radPanel1.Controls.Add(this.radLabelTooltipText);
            this.radPanel1.Controls.Add(this.btnCustom);
            this.radPanel1.Controls.Add(this.progressBar1);
            this.radPanel1.Controls.Add(this.textBoxTime);
            this.radPanel1.Controls.Add(this.labelProgress);
            this.radPanel1.Controls.Add(this.pictureBox1);
            this.radPanel1.Controls.Add(this.radLabelTooltip);
            this.radPanel1.Controls.Add(this.panel11);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(427, 330);
            this.radPanel1.TabIndex = 14;
            this.radPanel1.UseWaitCursor = true;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).Width = 4F;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.None;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94)))));
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanel1.GetChildAt(0).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Visible;
            // 
            // labelProgress
            // 
            this.labelProgress.BackColor = System.Drawing.Color.White;
            this.labelProgress.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.labelProgress.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(45)))));
            this.labelProgress.Location = new System.Drawing.Point(5, 135);
            this.labelProgress.Name = "labelProgress";
            this.labelProgress.Size = new System.Drawing.Size(417, 22);
            this.labelProgress.TabIndex = 1;
            this.labelProgress.Text = "Loading...";
            this.labelProgress.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.labelProgress.UseWaitCursor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.White;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(148, 5);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(128, 128);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.UseWaitCursor = true;
            // 
            // radLabelTooltip
            // 
            this.radLabelTooltip.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold);
            this.radLabelTooltip.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(69)))), ((int)(((byte)(69)))), ((int)(((byte)(69)))));
            this.radLabelTooltip.Location = new System.Drawing.Point(31, 189);
            this.radLabelTooltip.Name = "radLabelTooltip";
            this.radLabelTooltip.Size = new System.Drawing.Size(59, 20);
            this.radLabelTooltip.TabIndex = 11;
            this.radLabelTooltip.Text = "Tool tip";
            this.radLabelTooltip.UseWaitCursor = true;
            // 
            // panel11
            // 
            this.panel11.BackColor = System.Drawing.Color.Black;
            this.panel11.Cursor = System.Windows.Forms.Cursors.WaitCursor;
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
            this.radLabelTooltipText.TabIndex = 13;
            this.radLabelTooltipText.UseWaitCursor = true;
            // 
            // FrmProgressStatus
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(427, 330);
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FrmProgressStatus";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Loading";
            this.ThemeName = "Windows7";
            ((System.ComponentModel.ISupportInitialize)(this.btnCustom)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltip)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelTooltipText)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.TextBox textBoxTime;
        private TechSun.Theme.KRadButton btnCustom;
        private Telerik.WinControls.UI.RadPanel radPanel1;
        private System.Windows.Forms.Label labelProgress;
        private System.Windows.Forms.PictureBox pictureBox1;
        private Telerik.WinControls.UI.RadLabel radLabelTooltip;
        private System.Windows.Forms.Panel panel11;
        private Telerik.WinControls.UI.RadLabel radLabelTooltipText;
    }
}
