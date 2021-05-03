namespace Configuration.Form.UserControls
{
    partial class UserControlEDBValidationDefault
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            Telerik.WinControls.UI.TableViewDefinition tableViewDefinition1 = new Telerik.WinControls.UI.TableViewDefinition();
            this.radGridViewValidation = new Telerik.WinControls.UI.RadGridView();
            this.radPanelListType = new Telerik.WinControls.UI.RadPanel();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListListType = new Telerik.WinControls.UI.RadDropDownList();
            this.radButtonInsertRow = new Telerik.WinControls.UI.RadButton();
            this.radButtonRemoveRow = new Telerik.WinControls.UI.RadButton();
            this.radButtonDown = new Telerik.WinControls.UI.RadButton();
            this.radButtonUp = new Telerik.WinControls.UI.RadButton();
            this.windows8Theme1 = new Telerik.WinControls.Themes.Windows8Theme();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewValidation)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewValidation.MasterTemplate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelListType)).BeginInit();
            this.radPanelListType.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            this.SuspendLayout();
            // 
            // radGridViewValidation
            // 
            this.radGridViewValidation.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radGridViewValidation.Location = new System.Drawing.Point(6, 56);
            // 
            // 
            // 
            this.radGridViewValidation.MasterTemplate.ClipboardCopyMode = Telerik.WinControls.UI.GridViewClipboardCopyMode.Disable;
            this.radGridViewValidation.MasterTemplate.ShowRowHeaderColumn = false;
            this.radGridViewValidation.MasterTemplate.ViewDefinition = tableViewDefinition1;
            this.radGridViewValidation.Name = "radGridViewValidation";
            this.radGridViewValidation.Size = new System.Drawing.Size(882, 352);
            this.radGridViewValidation.TabIndex = 17;
            // 
            // radPanelListType
            // 
            this.radPanelListType.Controls.Add(this.radLabel2);
            this.radPanelListType.Controls.Add(this.radDropDownListListType);
            this.radPanelListType.Location = new System.Drawing.Point(3, 4);
            this.radPanelListType.Name = "radPanelListType";
            this.radPanelListType.Size = new System.Drawing.Size(637, 30);
            this.radPanelListType.TabIndex = 22;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanelListType.GetChildAt(0).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Collapsed;
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(9, 6);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(47, 18);
            this.radLabel2.TabIndex = 4;
            this.radLabel2.Text = "ListType";
            // 
            // radDropDownListListType
            // 
            this.radDropDownListListType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListListType.Location = new System.Drawing.Point(77, 5);
            this.radDropDownListListType.Name = "radDropDownListListType";
            this.radDropDownListListType.Size = new System.Drawing.Size(220, 20);
            this.radDropDownListListType.TabIndex = 3;
            this.radDropDownListListType.Text = "radDropDownList2";
            this.radDropDownListListType.ThemeName = "Windows8";
            // 
            // radButtonInsertRow
            // 
            this.radButtonInsertRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonInsertRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonInsertRow.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonInsertRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonInsertRow.Location = new System.Drawing.Point(832, 8);
            this.radButtonInsertRow.Name = "radButtonInsertRow";
            this.radButtonInsertRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonInsertRow.TabIndex = 12;
            this.radButtonInsertRow.ThemeName = "Office2010Silver";
            this.radButtonInsertRow.Click += new System.EventHandler(this.radButtonInsertRow_Click);
            // 
            // radButtonRemoveRow
            // 
            this.radButtonRemoveRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonRemoveRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonRemoveRow.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonRemoveRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonRemoveRow.Location = new System.Drawing.Point(862, 8);
            this.radButtonRemoveRow.Name = "radButtonRemoveRow";
            this.radButtonRemoveRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonRemoveRow.TabIndex = 11;
            this.radButtonRemoveRow.ThemeName = "Office2010Silver";
            this.radButtonRemoveRow.Click += new System.EventHandler(this.radButtonRemoveRow_Click);
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = global::Configuration.Properties.Resources.SmallDown;
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(802, 8);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 24);
            this.radButtonDown.TabIndex = 50;
            this.radButtonDown.ThemeName = "Office2010Silver";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = global::Configuration.Properties.Resources.SmallUp;
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(772, 8);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 24);
            this.radButtonUp.TabIndex = 49;
            this.radButtonUp.ThemeName = "Office2010Silver";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // UserControlEDBValidationDefault
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radButtonDown);
            this.Controls.Add(this.radButtonUp);
            this.Controls.Add(this.radButtonInsertRow);
            this.Controls.Add(this.radButtonRemoveRow);
            this.Controls.Add(this.radPanelListType);
            this.Controls.Add(this.radGridViewValidation);
            this.Name = "UserControlEDBValidationDefault";
            this.Size = new System.Drawing.Size(891, 441);
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewValidation.MasterTemplate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridViewValidation)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelListType)).EndInit();
            this.radPanelListType.ResumeLayout(false);
            this.radPanelListType.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListListType)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadGridView radGridViewValidation;
        private Telerik.WinControls.UI.RadPanel radPanelListType;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListListType;
        private Telerik.WinControls.UI.RadButton radButtonInsertRow;
        private Telerik.WinControls.UI.RadButton radButtonRemoveRow;
        private Telerik.WinControls.UI.RadButton radButtonDown;
        private Telerik.WinControls.UI.RadButton radButtonUp;
        private Telerik.WinControls.Themes.Windows8Theme windows8Theme1;

    }

}

