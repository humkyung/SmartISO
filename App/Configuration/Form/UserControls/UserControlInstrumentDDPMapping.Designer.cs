using System;
namespace Configuration
{
    partial class UserControlInstrumentDDPMapping
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
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.radPanel3 = new Telerik.WinControls.UI.RadPanel();
            this.radGridView1 = new Telerik.WinControls.UI.RadGridView();
            this.radPanel2 = new Telerik.WinControls.UI.RadPanel();
            this.radButtonInsertRow = new Telerik.WinControls.UI.RadButton();
            this.radButtonRemoveRow = new Telerik.WinControls.UI.RadButton();
            this.radLabel3 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListSheet = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownListBody = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel2 = new Telerik.WinControls.UI.RadLabel();
            this.radDropDownCataType = new Telerik.WinControls.UI.RadDropDownList();
            this.radPanelProject = new Telerik.WinControls.UI.RadPanel();
            this.radButtonDMCSExtraction = new Telerik.WinControls.UI.RadButton();
            this.windows8Theme1 = new Telerik.WinControls.Themes.Windows8Theme();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel3)).BeginInit();
            this.radPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1.MasterTemplate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel2)).BeginInit();
            this.radPanel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSheet)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListBody)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownCataType)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelProject)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDMCSExtraction)).BeginInit();
            this.SuspendLayout();
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.radPanel3);
            this.radPanel1.Controls.Add(this.radPanel2);
            this.radPanel1.Controls.Add(this.radPanelProject);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(910, 804);
            this.radPanel1.TabIndex = 0;
            // 
            // radPanel3
            // 
            this.radPanel3.Controls.Add(this.radGridView1);
            this.radPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel3.Location = new System.Drawing.Point(0, 40);
            this.radPanel3.Name = "radPanel3";
            this.radPanel3.Size = new System.Drawing.Size(910, 764);
            this.radPanel3.TabIndex = 51;
            // 
            // radGridView1
            // 
            this.radGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radGridView1.Location = new System.Drawing.Point(0, 0);
            // 
            // 
            // 
            this.radGridView1.MasterTemplate.ViewDefinition = tableViewDefinition1;
            this.radGridView1.Name = "radGridView1";
            this.radGridView1.Size = new System.Drawing.Size(910, 764);
            this.radGridView1.TabIndex = 0;
            this.radGridView1.Text = "radGridView1";
            this.radGridView1.ThemeName = "Office2010Silver";
            // 
            // radPanel2
            // 
            this.radPanel2.Controls.Add(this.radButtonInsertRow);
            this.radPanel2.Controls.Add(this.radButtonRemoveRow);
            this.radPanel2.Controls.Add(this.radLabel3);
            this.radPanel2.Controls.Add(this.radDropDownListSheet);
            this.radPanel2.Controls.Add(this.radLabel1);
            this.radPanel2.Controls.Add(this.radDropDownListBody);
            this.radPanel2.Controls.Add(this.radLabel2);
            this.radPanel2.Controls.Add(this.radDropDownCataType);
            this.radPanel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.radPanel2.Location = new System.Drawing.Point(0, 0);
            this.radPanel2.Name = "radPanel2";
            this.radPanel2.Size = new System.Drawing.Size(910, 40);
            this.radPanel2.TabIndex = 50;
            // 
            // radButtonInsertRow
            // 
            this.radButtonInsertRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonInsertRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonInsertRow.Image = global::Configuration.Properties.Resources.SmallAdd;
            this.radButtonInsertRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonInsertRow.Location = new System.Drawing.Point(848, 7);
            this.radButtonInsertRow.Name = "radButtonInsertRow";
            this.radButtonInsertRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonInsertRow.TabIndex = 10;
            this.radButtonInsertRow.ThemeName = "Office2010Silver";
            this.radButtonInsertRow.Click += new System.EventHandler(this.radButtonInsertRow_Click);
            // 
            // radButtonRemoveRow
            // 
            this.radButtonRemoveRow.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonRemoveRow.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonRemoveRow.Image = global::Configuration.Properties.Resources.SmallDelete;
            this.radButtonRemoveRow.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonRemoveRow.Location = new System.Drawing.Point(878, 7);
            this.radButtonRemoveRow.Name = "radButtonRemoveRow";
            this.radButtonRemoveRow.Size = new System.Drawing.Size(24, 24);
            this.radButtonRemoveRow.TabIndex = 9;
            this.radButtonRemoveRow.ThemeName = "Office2010Silver";
            this.radButtonRemoveRow.Click += new System.EventHandler(this.radButtonRemoveRow_Click);
            // 
            // radLabel3
            // 
            this.radLabel3.Location = new System.Drawing.Point(549, 9);
            this.radLabel3.Name = "radLabel3";
            this.radLabel3.Size = new System.Drawing.Size(34, 18);
            this.radLabel3.TabIndex = 4;
            this.radLabel3.Text = "Sheet";
            // 
            // radDropDownListSheet
            // 
            this.radDropDownListSheet.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListSheet.Location = new System.Drawing.Point(604, 8);
            this.radDropDownListSheet.Name = "radDropDownListSheet";
            this.radDropDownListSheet.Size = new System.Drawing.Size(189, 20);
            this.radDropDownListSheet.TabIndex = 3;
            this.radDropDownListSheet.Text = "radDropDownList2";
            this.radDropDownListSheet.ThemeName = "Windows8";
            this.radDropDownListSheet.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSheet_SelectedIndexChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Location = new System.Drawing.Point(282, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(30, 18);
            this.radLabel1.TabIndex = 4;
            this.radLabel1.Text = "Type";
            // 
            // radDropDownListBody
            // 
            this.radDropDownListBody.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListBody.Location = new System.Drawing.Point(337, 8);
            this.radDropDownListBody.Name = "radDropDownListBody";
            this.radDropDownListBody.Size = new System.Drawing.Size(189, 20);
            this.radDropDownListBody.TabIndex = 3;
            this.radDropDownListBody.Text = "radDropDownList2";
            this.radDropDownListBody.ThemeName = "Windows8";
            this.radDropDownListBody.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListBody_SelectedIndexChanged);
            // 
            // radLabel2
            // 
            this.radLabel2.Location = new System.Drawing.Point(12, 9);
            this.radLabel2.Name = "radLabel2";
            this.radLabel2.Size = new System.Drawing.Size(29, 18);
            this.radLabel2.TabIndex = 4;
            this.radLabel2.Text = "Cata";
            // 
            // radDropDownCataType
            // 
            this.radDropDownCataType.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownCataType.Location = new System.Drawing.Point(58, 8);
            this.radDropDownCataType.Name = "radDropDownCataType";
            this.radDropDownCataType.Size = new System.Drawing.Size(189, 20);
            this.radDropDownCataType.TabIndex = 3;
            this.radDropDownCataType.Text = "radDropDownList2";
            this.radDropDownCataType.ThemeName = "Windows8";
            this.radDropDownCataType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownCataType_SelectedIndexChanged);
            // 
            // radPanelProject
            // 
            this.radPanelProject.AutoSize = true;
            this.radPanelProject.Location = new System.Drawing.Point(3, 4);
            this.radPanelProject.Name = "radPanelProject";
            this.radPanelProject.Size = new System.Drawing.Size(0, 0);
            this.radPanelProject.TabIndex = 7;
            ((Telerik.WinControls.Primitives.BorderPrimitive)(this.radPanelProject.GetChildAt(0).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Collapsed;
            // 
            // radButtonDMCSExtraction
            // 
            this.radButtonDMCSExtraction.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDMCSExtraction.Location = new System.Drawing.Point(0, 0);
            this.radButtonDMCSExtraction.Name = "radButtonDMCSExtraction";
            this.radButtonDMCSExtraction.Size = new System.Drawing.Size(100, 20);
            this.radButtonDMCSExtraction.TabIndex = 5;
            this.radButtonDMCSExtraction.Text = "Extraction";
            this.radButtonDMCSExtraction.ThemeName = "Office2010Silver";
            this.radButtonDMCSExtraction.Visible = false;
            // 
            // UserControlInstrumentDDPMapping
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Name = "UserControlInstrumentDDPMapping";
            this.Size = new System.Drawing.Size(910, 804);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel3)).EndInit();
            this.radPanel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1.MasterTemplate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel2)).EndInit();
            this.radPanel2.ResumeLayout(false);
            this.radPanel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonInsertRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonRemoveRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSheet)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListBody)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownCataType)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radPanelProject)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDMCSExtraction)).EndInit();
            this.ResumeLayout(false);

        }

        

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadGridView radGridView1;
        private Telerik.WinControls.UI.RadButton radButtonDMCSExtraction;
        private Telerik.WinControls.UI.RadLabel radLabel2;
        private Telerik.WinControls.UI.RadDropDownList radDropDownCataType;
        private Telerik.WinControls.UI.RadPanel radPanelProject;
        private Telerik.WinControls.UI.RadButton radButtonInsertRow;
        private Telerik.WinControls.UI.RadButton radButtonRemoveRow;
        private Telerik.WinControls.UI.RadPanel radPanel3;
        private Telerik.WinControls.UI.RadPanel radPanel2;
        private Telerik.WinControls.UI.RadLabel radLabel3;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListSheet;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListBody;
        private Telerik.WinControls.Themes.Windows8Theme windows8Theme1;

    }
}
