using TechSun.Theme;
namespace Configuration
{
    partial class UserControlFilter
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
            Telerik.WinControls.UI.RadListDataItem radListDataItem10 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem11 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem12 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem13 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem14 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem15 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem16 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem17 = new Telerik.WinControls.UI.RadListDataItem();
            Telerik.WinControls.UI.RadListDataItem radListDataItem18 = new Telerik.WinControls.UI.RadListDataItem();
            this.radButtonEndBracket = new TechSun.Theme.KRadButton();
            this.radTextBoxFilters = new Telerik.WinControls.UI.RadTextBox();
            this.radButtonAddOr = new TechSun.Theme.KRadButton();
            this.radButtonAndAdd = new TechSun.Theme.KRadButton();
            this.radTextBoxValue = new Telerik.WinControls.UI.RadTextBox();
            this.radDropDownListOperator = new Telerik.WinControls.UI.RadDropDownList();
            this.radDropDownListColumns = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            this.radButtonAllClear = new TechSun.Theme.KRadButton();
            this.radButtonStartBracket = new TechSun.Theme.KRadButton();
            this.kradButtonAddANDNewLine = new TechSun.Theme.KRadButton();
            this.kradButtonAddORNewLine = new TechSun.Theme.KRadButton();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonEndBracket)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxFilters)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAddOr)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAndAdd)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxValue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListOperator)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListColumns)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAllClear)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonStartBracket)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonAddANDNewLine)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonAddORNewLine)).BeginInit();
            this.SuspendLayout();
            // 
            // radButtonEndBracket
            // 
            this.radButtonEndBracket.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonEndBracket.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonEndBracket.Location = new System.Drawing.Point(570, 163);
            this.radButtonEndBracket.Name = "radButtonEndBracket";
            this.radButtonEndBracket.Size = new System.Drawing.Size(80, 26);
            this.radButtonEndBracket.TabIndex = 26;
            this.radButtonEndBracket.Text = "Add )";
            this.radButtonEndBracket.ThemeName = "Office2013Light";
            this.radButtonEndBracket.Click += new System.EventHandler(this.radButtonEndBracket_Click);
            // 
            // radTextBoxFilters
            // 
            this.radTextBoxFilters.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radTextBoxFilters.AutoSize = false;
            this.radTextBoxFilters.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radTextBoxFilters.Location = new System.Drawing.Point(3, 35);
            this.radTextBoxFilters.Multiline = true;
            this.radTextBoxFilters.Name = "radTextBoxFilters";
            this.radTextBoxFilters.Size = new System.Drawing.Size(561, 186);
            this.radTextBoxFilters.TabIndex = 24;
            this.radTextBoxFilters.TabStop = false;
            this.radTextBoxFilters.ThemeName = "Office2010Silver";
            // 
            // radButtonAddOr
            // 
            this.radButtonAddOr.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAddOr.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAddOr.Location = new System.Drawing.Point(570, 35);
            this.radButtonAddOr.Name = "radButtonAddOr";
            this.radButtonAddOr.Size = new System.Drawing.Size(127, 26);
            this.radButtonAddOr.TabIndex = 23;
            this.radButtonAddOr.Text = "Add to Filter(Or)";
            this.radButtonAddOr.ThemeName = "Office2013Light";
            this.radButtonAddOr.Click += new System.EventHandler(this.radButtonAddOr_Click);
            // 
            // radButtonAndAdd
            // 
            this.radButtonAndAdd.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAndAdd.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAndAdd.Location = new System.Drawing.Point(570, 3);
            this.radButtonAndAdd.Name = "radButtonAndAdd";
            this.radButtonAndAdd.Size = new System.Drawing.Size(127, 26);
            this.radButtonAndAdd.TabIndex = 22;
            this.radButtonAndAdd.Text = "Add to Filter(And)";
            this.radButtonAndAdd.ThemeName = "Office2013Light";
            this.radButtonAndAdd.Click += new System.EventHandler(this.radButtonAndAdd_Click);
            // 
            // radTextBoxValue
            // 
            this.radTextBoxValue.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radTextBoxValue.Location = new System.Drawing.Point(421, 7);
            this.radTextBoxValue.Name = "radTextBoxValue";
            this.radTextBoxValue.Size = new System.Drawing.Size(143, 19);
            this.radTextBoxValue.TabIndex = 21;
            this.radTextBoxValue.TabStop = false;
            this.radTextBoxValue.ThemeName = "Office2010Silver";
            this.radTextBoxValue.TextChanged += new System.EventHandler(this.radTextBoxValue_TextChanged);
            this.radTextBoxValue.Click += new System.EventHandler(this.radTextBoxValue_TextChanged);
            // 
            // radDropDownListOperator
            // 
            this.radDropDownListOperator.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            radListDataItem10.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem10.Text = "=";
            radListDataItem11.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem11.Text = "<>";
            radListDataItem12.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem12.Text = ">";
            radListDataItem13.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem13.Text = ">=";
            radListDataItem14.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem14.Text = "<";
            radListDataItem15.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem15.Text = "<=";
            radListDataItem16.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem16.Text = "LIKE";
            radListDataItem17.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem17.Text = "IS NULL";
            radListDataItem18.Font = new System.Drawing.Font("Tahoma", 8.25F);
            radListDataItem18.Text = "IS NOT NULL";
            this.radDropDownListOperator.Items.Add(radListDataItem10);
            this.radDropDownListOperator.Items.Add(radListDataItem11);
            this.radDropDownListOperator.Items.Add(radListDataItem12);
            this.radDropDownListOperator.Items.Add(radListDataItem13);
            this.radDropDownListOperator.Items.Add(radListDataItem14);
            this.radDropDownListOperator.Items.Add(radListDataItem15);
            this.radDropDownListOperator.Items.Add(radListDataItem16);
            this.radDropDownListOperator.Items.Add(radListDataItem17);
            this.radDropDownListOperator.Items.Add(radListDataItem18);
            this.radDropDownListOperator.Location = new System.Drawing.Point(304, 7);
            this.radDropDownListOperator.Name = "radDropDownListOperator";
            this.radDropDownListOperator.Size = new System.Drawing.Size(111, 19);
            this.radDropDownListOperator.TabIndex = 20;
            this.radDropDownListOperator.ThemeName = "Office2010Silver";
            this.radDropDownListOperator.TextChanged += new System.EventHandler(this.radDropDownListOperator_TextChanged);
            // 
            // radDropDownListColumns
            // 
            this.radDropDownListColumns.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.radDropDownListColumns.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.radDropDownListColumns.Location = new System.Drawing.Point(61, 7);
            this.radDropDownListColumns.Name = "radDropDownListColumns";
            this.radDropDownListColumns.Size = new System.Drawing.Size(237, 19);
            this.radDropDownListColumns.TabIndex = 19;
            this.radDropDownListColumns.ThemeName = "Office2010Silver";
            this.radDropDownListColumns.TextChanged += new System.EventHandler(this.radDropDownListColumns_TextChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(3, 7);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(60, 17);
            this.radLabel1.TabIndex = 18;
            this.radLabel1.Text = "Columns : ";
            // 
            // radButtonAllClear
            // 
            this.radButtonAllClear.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonAllClear.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonAllClear.Location = new System.Drawing.Point(570, 195);
            this.radButtonAllClear.Name = "radButtonAllClear";
            this.radButtonAllClear.Size = new System.Drawing.Size(80, 26);
            this.radButtonAllClear.TabIndex = 27;
            this.radButtonAllClear.Text = "Clear All";
            this.radButtonAllClear.ThemeName = "Office2013Light";
            this.radButtonAllClear.Click += new System.EventHandler(this.radButtonAllClear_Click);
            // 
            // radButtonStartBracket
            // 
            this.radButtonStartBracket.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonStartBracket.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonStartBracket.Location = new System.Drawing.Point(570, 131);
            this.radButtonStartBracket.Name = "radButtonStartBracket";
            this.radButtonStartBracket.Size = new System.Drawing.Size(80, 26);
            this.radButtonStartBracket.TabIndex = 25;
            this.radButtonStartBracket.Text = "Add (";
            this.radButtonStartBracket.ThemeName = "Office2013Light";
            this.radButtonStartBracket.Click += new System.EventHandler(this.radButtonStartBracket_Click);
            // 
            // kradButtonAddANDNewLine
            // 
            this.kradButtonAddANDNewLine.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.kradButtonAddANDNewLine.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kradButtonAddANDNewLine.Location = new System.Drawing.Point(570, 67);
            this.kradButtonAddANDNewLine.Name = "kradButtonAddANDNewLine";
            this.kradButtonAddANDNewLine.Size = new System.Drawing.Size(80, 26);
            this.kradButtonAddANDNewLine.TabIndex = 26;
            this.kradButtonAddANDNewLine.Text = "Add AND";
            this.kradButtonAddANDNewLine.ThemeName = "Office2013Light";
            this.kradButtonAddANDNewLine.Click += new System.EventHandler(this.kradButtonAddANDNewLine_Click);
            // 
            // kradButtonAddORNewLine
            // 
            this.kradButtonAddORNewLine.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.kradButtonAddORNewLine.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.kradButtonAddORNewLine.Location = new System.Drawing.Point(570, 99);
            this.kradButtonAddORNewLine.Name = "kradButtonAddORNewLine";
            this.kradButtonAddORNewLine.Size = new System.Drawing.Size(80, 26);
            this.kradButtonAddORNewLine.TabIndex = 26;
            this.kradButtonAddORNewLine.Text = "Add OR";
            this.kradButtonAddORNewLine.ThemeName = "Office2013Light";
            this.kradButtonAddORNewLine.Click += new System.EventHandler(this.kradButtonAddORNewLine_Click);
            // 
            // UserControlFilter
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackColor = System.Drawing.Color.White;
            this.Controls.Add(this.kradButtonAddORNewLine);
            this.Controls.Add(this.kradButtonAddANDNewLine);
            this.Controls.Add(this.radButtonAllClear);
            this.Controls.Add(this.radButtonEndBracket);
            this.Controls.Add(this.radButtonStartBracket);
            this.Controls.Add(this.radTextBoxFilters);
            this.Controls.Add(this.radButtonAddOr);
            this.Controls.Add(this.radButtonAndAdd);
            this.Controls.Add(this.radTextBoxValue);
            this.Controls.Add(this.radDropDownListOperator);
            this.Controls.Add(this.radDropDownListColumns);
            this.Controls.Add(this.radLabel1);
            this.Name = "UserControlFilter";
            this.Size = new System.Drawing.Size(700, 224);
            ((System.ComponentModel.ISupportInitialize)(this.radButtonEndBracket)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxFilters)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAddOr)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAndAdd)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radTextBoxValue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListOperator)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListColumns)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonAllClear)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonStartBracket)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonAddANDNewLine)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.kradButtonAddORNewLine)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private KRadButton radButtonEndBracket;
        private KRadButton radButtonStartBracket;
        private Telerik.WinControls.UI.RadTextBox radTextBoxFilters;
        private KRadButton radButtonAddOr;
        private KRadButton radButtonAndAdd;
        private Telerik.WinControls.UI.RadTextBox radTextBoxValue;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListOperator;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListColumns;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private KRadButton radButtonAllClear;
        private KRadButton kradButtonAddANDNewLine;
        private KRadButton kradButtonAddORNewLine;
    }
}
