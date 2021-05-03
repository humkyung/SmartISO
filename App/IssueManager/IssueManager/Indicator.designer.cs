namespace framework.core.forms
{
	partial class Indicator
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
            this.pictureBoxIndicator = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxIndicator)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxIndicator
            // 
            this.pictureBoxIndicator.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxIndicator.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxIndicator.Image = global::IssueManager.Properties.Resources.ajax_activity_indicators_download_animated_indicator_big;
            this.pictureBoxIndicator.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxIndicator.Name = "pictureBoxIndicator";
            this.pictureBoxIndicator.Size = new System.Drawing.Size(110, 129);
            this.pictureBoxIndicator.TabIndex = 1;
            this.pictureBoxIndicator.TabStop = false;
            // 
            // Indicator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(110, 129);
            this.Controls.Add(this.pictureBoxIndicator);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Indicator";
            this.ShowInTaskbar = false;
            this.Text = "Indicator";
            this.TransparencyKey = System.Drawing.Color.White;
            this.Load += new System.EventHandler(this.Indicator_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxIndicator)).EndInit();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PictureBox pictureBoxIndicator;
	}
}