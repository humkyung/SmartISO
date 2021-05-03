namespace Configuration
{
    partial class UserControlCommonValidation : UserControlBase
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
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.radSplitContainer1 = new Telerik.WinControls.UI.RadSplitContainer();
            this.splitPanel1 = new Telerik.WinControls.UI.SplitPanel();
            this.fpSpreadValidationType = new FarPoint.Win.Spread.FpSpread();
            this.splitPanel2 = new Telerik.WinControls.UI.SplitPanel();
            this.radLabel4 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListSystemList = new Telerik.WinControls.UI.RadDropDownList();
            this.radDropDownListProjectList = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radPanelUserControl = new Telerik.WinControls.UI.RadPanel();
            this.radLabel3 = new Telerik.WinControls.UI.RadLabel();
            this.comboTreeBoxItemType = new Configuration.UserControlComboTreeBox();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).BeginInit();
            this.radSplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).BeginInit();
            this.splitPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadValidationType)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).BeginInit();
            this.splitPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSystemList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelUserControl)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radSplitContainer1);
            this.radPanel1.Controls.Add(this.radLabel4);
            this.radPanel1.Controls.Add(this.radDropDownListSystemList);
            this.radPanel1.Controls.Add(this.radDropDownListProjectList);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1237, 715);
            this.radPanel1.TabIndex = 0;
            this.radPanel1.ThemeName = "Office2010Silver";
            // 
            // radSplitContainer1
            // 
            this.radSplitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radSplitContainer1.Controls.Add(this.splitPanel1);
            this.radSplitContainer1.Controls.Add(this.splitPanel2);
            this.radSplitContainer1.Location = new System.Drawing.Point(12, 54);
            this.radSplitContainer1.Name = "radSplitContainer1";
            // 
            // 
            // 
            this.radSplitContainer1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.radSplitContainer1.Size = new System.Drawing.Size(1213, 649);
            this.radSplitContainer1.TabIndex = 69;
            this.radSplitContainer1.TabStop = false;
            this.radSplitContainer1.Text = "radSplitContainer1";
            ((Telerik.WinControls.Primitives.FillPrimitive)(this.radSplitContainer1.GetChildAt(0).GetChildAt(0))).BackColor = System.Drawing.Color.Transparent;
            // 
            // splitPanel1
            // 
            this.splitPanel1.Controls.Add(this.fpSpreadValidationType);
            this.splitPanel1.Location = new System.Drawing.Point(0, 0);
            this.splitPanel1.Name = "splitPanel1";
            // 
            // 
            // 
            this.splitPanel1.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel1.Size = new System.Drawing.Size(271, 649);
            this.splitPanel1.SizeInfo.AbsoluteSize = new System.Drawing.Size(305, 200);
            this.splitPanel1.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(-0.2758478F, 0F);
            this.splitPanel1.SizeInfo.SplitterCorrection = new System.Drawing.Size(-157, 0);
            this.splitPanel1.TabIndex = 0;
            this.splitPanel1.TabStop = false;
            ((Telerik.WinControls.UI.SplitPanelElement)(this.splitPanel1.GetChildAt(0))).StretchHorizontally = true;
            ((Telerik.WinControls.UI.SplitPanelElement)(this.splitPanel1.GetChildAt(0))).StretchVertically = true;
            // 
            // fpSpreadValidationType
            // 
            this.fpSpreadValidationType.AccessibleDescription = "fpUnSelectedDataFilter, Sheet1, Row 0, Column 0, ";
            this.fpSpreadValidationType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fpSpreadValidationType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.fpSpreadValidationType.Location = new System.Drawing.Point(0, 0);
            this.fpSpreadValidationType.Name = "fpSpreadValidationType";
            this.fpSpreadValidationType.Size = new System.Drawing.Size(271, 649);
            this.fpSpreadValidationType.TabIndex = 71;
            this.fpSpreadValidationType.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadValidationType_CellClick);
            // 
            // splitPanel2
            // 
            this.splitPanel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.splitPanel2.Controls.Add(this.comboTreeBoxItemType);
            this.splitPanel2.Controls.Add(this.radLabel3);
            this.splitPanel2.Controls.Add(this.radPanelUserControl);
            this.splitPanel2.Location = new System.Drawing.Point(275, 0);
            this.splitPanel2.Name = "splitPanel2";
            // 
            // 
            // 
            this.splitPanel2.RootElement.MinSize = new System.Drawing.Size(0, 0);
            this.splitPanel2.Size = new System.Drawing.Size(938, 649);
            this.splitPanel2.SizeInfo.AutoSizeScale = new System.Drawing.SizeF(-0.001104951F, 0F);
            this.splitPanel2.SizeInfo.SplitterCorrection = new System.Drawing.Size(157, 0);
            this.splitPanel2.TabIndex = 1;
            this.splitPanel2.TabStop = false;
            // 
            // radLabel4
            // 
            this.radLabel4.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel4.Location = new System.Drawing.Point(12, 9);
            this.radLabel4.Name = "radLabel4";
            this.radLabel4.Size = new System.Drawing.Size(50, 17);
            this.radLabel4.TabIndex = 68;
            this.radLabel4.Text = "System :";
            // 
            // radDropDownListSystemList
            // 
            this.radDropDownListSystemList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListSystemList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListSystemList.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListSystemList.Name = "radDropDownListSystemList";
            this.radDropDownListSystemList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListSystemList.TabIndex = 67;
            this.radDropDownListSystemList.ThemeName = "Office2010Silver";
            this.radDropDownListSystemList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSystemList_SelectedIndexChanged);
            // 
            // radDropDownListProjectList
            // 
            this.radDropDownListProjectList.DropDownHeight = 80;
            this.radDropDownListProjectList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListProjectList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListProjectList.Location = new System.Drawing.Point(80, 32);
            this.radDropDownListProjectList.Name = "radDropDownListProjectList";
            this.radDropDownListProjectList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListProjectList.TabIndex = 50;
            this.radDropDownListProjectList.ThemeName = "Office2010Silver";
            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 32);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(48, 17);
            this.radLabel1.TabIndex = 51;
            this.radLabel1.Text = "Project :";
            // 
            // radPanelUserControl
            // 
            this.radPanelUserControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radPanelUserControl.Location = new System.Drawing.Point(3, 28);
            this.radPanelUserControl.Name = "radPanelUserControl";
            this.radPanelUserControl.Size = new System.Drawing.Size(930, 616);
            this.radPanelUserControl.TabIndex = 4;
            // 
            // radLabel3
            // 
            this.radLabel3.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel3.Location = new System.Drawing.Point(5, 7);
            this.radLabel3.Name = "radLabel3";
            this.radLabel3.Size = new System.Drawing.Size(65, 17);
            this.radLabel3.TabIndex = 82;
            this.radLabel3.Text = "Item Type :";
            // 
            // comboTreeBoxItemType
            // 
            this.comboTreeBoxItemType.DropDownHeight = 106;
            this.comboTreeBoxItemType.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.comboTreeBoxItemType.Location = new System.Drawing.Point(72, 3);
            this.comboTreeBoxItemType.Name = "comboTreeBoxItemType";
            this.comboTreeBoxItemType.Size = new System.Drawing.Size(246, 21);
            this.comboTreeBoxItemType.TabIndex = 83;
            // 
            // UserControlCommonValidation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlCommonValidation";
            this.Size = new System.Drawing.Size(1237, 715);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radSplitContainer1)).EndInit();
            this.radSplitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel1)).EndInit();
            this.splitPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadValidationType)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.splitPanel2)).EndInit();
            this.splitPanel2.ResumeLayout(false);
            this.splitPanel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSystemList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListProjectList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelUserControl)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListProjectList;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadLabel radLabel4;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListSystemList;
        private Telerik.WinControls.UI.RadSplitContainer radSplitContainer1;
        private Telerik.WinControls.UI.SplitPanel splitPanel1;
        private Telerik.WinControls.UI.SplitPanel splitPanel2;
        private FarPoint.Win.Spread.FpSpread fpSpreadValidationType;
        private UserControlComboTreeBox comboTreeBoxItemType;
        private Telerik.WinControls.UI.RadLabel radLabel3;
        private Telerik.WinControls.UI.RadPanel radPanelUserControl;
    }
}
