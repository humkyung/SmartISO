using TechSun.Theme;
namespace Configuration
{
    partial class FrmSearchCodeList : KRadForm
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
            this.radTextBoxSearch = new Telerik.WinControls.UI.RadTextBox();
            this.fpSpreadData = new FarPoint.Win.Spread.FpSpread();
            this.radButtonOK = new Telerik.WinControls.UI.RadButton();
            this.radTextBoxCode = new Telerik.WinControls.UI.RadTextBox();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxSearch)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxCode)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this)).BeginInit();
            this.SuspendLayout();
            // 
            // radTextBoxSearch
            // 
            this.radTextBoxSearch.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radTextBoxSearch.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxSearch.Location = new System.Drawing.Point(168, 4);
            this.radTextBoxSearch.Name = "radTextBoxSearch";
            this.radTextBoxSearch.NullText = "Search...";
            this.radTextBoxSearch.Size = new System.Drawing.Size(489, 19);
            this.radTextBoxSearch.TabIndex = 2;
            this.radTextBoxSearch.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.radTextBoxSearch_KeyPress);
            // 
            // fpSpreadData
            // 
            this.fpSpreadData.AccessibleDescription = "";
            this.fpSpreadData.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadData.Location = new System.Drawing.Point(4, 29);
            this.fpSpreadData.Name = "fpSpreadData";
            this.fpSpreadData.Size = new System.Drawing.Size(739, 415);
            this.fpSpreadData.TabIndex = 3;
            this.fpSpreadData.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadData_CellClick);
            // 
            // radButtonOK
            // 
            this.radButtonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonOK.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radButtonOK.ForeColor = System.Drawing.Color.Black;
            this.radButtonOK.ImageAlignment = System.Drawing.ContentAlignment.MiddleRight;
            this.radButtonOK.Location = new System.Drawing.Point(663, 2);
            this.radButtonOK.Name = "radButtonOK";
            this.radButtonOK.Size = new System.Drawing.Size(80, 23);
            this.radButtonOK.TabIndex = 18;
            this.radButtonOK.Text = "OK";
            this.radButtonOK.ThemeName = "Office2010Silver";
            this.radButtonOK.Click += new System.EventHandler(this.radButtonOK_Click);
            // 
            // radTextBoxCode
            // 
            this.radTextBoxCode.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radTextBoxCode.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxCode.Location = new System.Drawing.Point(4, 4);
            this.radTextBoxCode.Name = "radTextBoxCode";
            this.radTextBoxCode.NullText = "Search...";
            this.radTextBoxCode.Size = new System.Drawing.Size(158, 19);
            this.radTextBoxCode.TabIndex = 3;
            this.radTextBoxCode.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.radTextBoxCode_KeyPress);
            // 
            // RadFormSearchCodeList
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(747, 449);
            this.Controls.Add(this.radTextBoxCode);
            this.Controls.Add(this.radButtonOK);
            this.Controls.Add(this.fpSpreadData);
            this.Controls.Add(this.radTextBoxSearch);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "RadFormSearchCodeList";
            // 
            // 
            // 
            this.RootElement.ApplyShapeToControl = true;
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Search";
            this.ThemeName = "Windows7";
            this.Load += new System.EventHandler(this.RadFormSearchCodeList_Load);
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxSearch)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadData)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonOK)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxCode)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Telerik.WinControls.Themes.Windows7Theme windows7Theme1;
        private Telerik.WinControls.UI.RadTextBox radTextBoxSearch;
        private FarPoint.Win.Spread.FpSpread fpSpreadData;
        public Telerik.WinControls.UI.RadButton radButtonOK;
        private Telerik.WinControls.UI.RadTextBox radTextBoxCode;
    }
}
