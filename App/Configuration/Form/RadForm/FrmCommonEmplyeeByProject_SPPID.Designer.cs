using TechSun.Theme;
namespace Configuration
{
    partial class FrmCommonEmplyeeByProject_SPPID : KRadForm
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
            this.radLabelName = new Telerik.WinControls.UI.RadLabel();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.radButtonCreate = new Telerik.WinControls.UI.RadButton();
            this.radButtonClose = new Telerik.WinControls.UI.RadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radLabelName)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radLabelName
            // 
            this.radLabelName.Location = new System.Drawing.Point(12, 6);
            this.radLabelName.Name = "radLabelName";
            this.radLabelName.Size = new System.Drawing.Size(52, 18);
            this.radLabelName.TabIndex = 27;
            this.radLabelName.Text = "Name :  -";
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData.Location = new System.Drawing.Point(12, 30);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Size = new System.Drawing.Size(717, 316);
            this.fpSpreadData.TabIndex = 30;
            this.fpSpreadData.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadData_CellClick);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonCreate.ForeColor = System.Drawing.Color.Black;
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonCreate.Location = new System.Drawing.Point(564, 352);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(80, 23);
            this.radButtonCreate.TabIndex = 29;
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
            this.radButtonClose.Location = new System.Drawing.Point(650, 352);
            this.radButtonClose.Name = "radButtonClose";
            this.radButtonClose.Size = new System.Drawing.Size(80, 23);
            this.radButtonClose.TabIndex = 28;
            this.radButtonClose.Text = "Cancel";
            this.radButtonClose.TextAlignment = System.Drawing.ContentAlignment.MiddleLeft;
            this.radButtonClose.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.radButtonClose.ThemeName = "Office2010Silver";
            this.radButtonClose.Click += new System.EventHandler(this.radButtonClose_Click);
            // 
            // FrmCommonEmplyeeByProject_SPPID
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(740, 381);
            this.Controls.Add(this.fpSpreadData);
            this.Controls.Add(this.radButtonCreate);
            this.Controls.Add(this.radButtonClose);
            this.Controls.Add(this.radLabelName);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "FrmCommonEmplyeeByProject_SPPID";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Add SPPID Project Database";
            this.ThemeName = "Windows7";
            this.Load += new System.EventHandler(this.FrmCommonEmplyeeByProject_SPPID_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radLabelName)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonClose)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.UI.RadLabel radLabelName;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        private Telerik.WinControls.UI.RadButton radButtonCreate;
        private Telerik.WinControls.UI.RadButton radButtonClose;
    }
}
