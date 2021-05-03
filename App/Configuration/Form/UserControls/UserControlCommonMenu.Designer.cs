namespace Configuration
{
    partial class UserControlCommonMenu
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
            FarPoint.Win.Spread.CellType.FlatRowHeaderRenderer flatRowHeaderRenderer1 = new FarPoint.Win.Spread.CellType.FlatRowHeaderRenderer();
            FarPoint.Win.Spread.CellType.EnhancedRowHeaderRenderer enhancedRowHeaderRenderer1 = new FarPoint.Win.Spread.CellType.EnhancedRowHeaderRenderer();
            FarPoint.Win.Spread.CellType.EnhancedColumnHeaderRenderer enhancedColumnHeaderRenderer1 = new FarPoint.Win.Spread.CellType.EnhancedColumnHeaderRenderer();
            FarPoint.Win.Spread.CellType.EnhancedRowHeaderRenderer enhancedRowHeaderRenderer2 = new FarPoint.Win.Spread.CellType.EnhancedRowHeaderRenderer();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(UserControlCommonMenu));
            FarPoint.Win.Spread.NamedStyle namedStyle1 = new FarPoint.Win.Spread.NamedStyle("DataAreaDefault");
            FarPoint.Win.Spread.CellType.GeneralCellType generalCellType1 = new FarPoint.Win.Spread.CellType.GeneralCellType();
            FarPoint.Win.Spread.NamedStyle namedStyle2 = new FarPoint.Win.Spread.NamedStyle("Style1");
            FarPoint.Win.ComplexBorder complexBorder1 = new FarPoint.Win.ComplexBorder(new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.None), new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.None, System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))))), new FarPoint.Win.ComplexBorderSide(System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))))), new FarPoint.Win.ComplexBorderSide(System.Drawing.Color.FromArgb(((int)(((byte)(82)))), ((int)(((byte)(89)))), ((int)(((byte)(105)))))), new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.ThinLine), false, false);
            FarPoint.Win.Spread.NamedStyle namedStyle3 = new FarPoint.Win.Spread.NamedStyle("Style6");
            FarPoint.Win.ComplexBorder complexBorder2 = new FarPoint.Win.ComplexBorder(new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.None), new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.None, System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))))), new FarPoint.Win.ComplexBorderSide(System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))))), new FarPoint.Win.ComplexBorderSide(System.Drawing.Color.FromArgb(((int)(((byte)(82)))), ((int)(((byte)(89)))), ((int)(((byte)(105)))))), new FarPoint.Win.ComplexBorderSide(FarPoint.Win.ComplexBorderSideStyle.ThinLine), false, false);
            FarPoint.Win.Spread.NamedStyle namedStyle4 = new FarPoint.Win.Spread.NamedStyle("FilterBarDefaultEnhanced");
            FarPoint.Win.Spread.CellType.FilterBarCellType filterBarCellType1 = new FarPoint.Win.Spread.CellType.FilterBarCellType();
            FarPoint.Win.Spread.NamedStyle namedStyle5 = new FarPoint.Win.Spread.NamedStyle("RowHeaderDefaultEnhanced");
            FarPoint.Win.Spread.NamedStyle namedStyle6 = new FarPoint.Win.Spread.NamedStyle("ColumnHeaderDefaultEnhanced");
            FarPoint.Win.Spread.NamedStyle namedStyle7 = new FarPoint.Win.Spread.NamedStyle("Style3");
            FarPoint.Win.Spread.NamedStyle namedStyle8 = new FarPoint.Win.Spread.NamedStyle("Style2");
            FarPoint.Win.Spread.CellType.FlatCornerHeaderRenderer flatCornerHeaderRenderer1 = new FarPoint.Win.Spread.CellType.FlatCornerHeaderRenderer();
            FarPoint.Win.Spread.NamedStyle namedStyle9 = new FarPoint.Win.Spread.NamedStyle("CornerFooterDefaultEnhanced");
            FarPoint.Win.Spread.CellType.FlatCornerFooterRenderer flatCornerFooterRenderer1 = new FarPoint.Win.Spread.CellType.FlatCornerFooterRenderer();
            FarPoint.Win.Spread.NamedStyle namedStyle10 = new FarPoint.Win.Spread.NamedStyle("Style4");
            FarPoint.Win.Spread.CellType.GeneralCellType generalCellType2 = new FarPoint.Win.Spread.CellType.GeneralCellType();
            FarPoint.Win.Spread.SpreadSkin spreadSkin1 = new FarPoint.Win.Spread.SpreadSkin();
            FarPoint.Win.Spread.FlatFocusIndicatorRenderer flatFocusIndicatorRenderer1 = new FarPoint.Win.Spread.FlatFocusIndicatorRenderer(3, System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0))))), System.Drawing.Color.White);
            FarPoint.Win.Spread.EnhancedInterfaceRenderer enhancedInterfaceRenderer1 = new FarPoint.Win.Spread.EnhancedInterfaceRenderer();
            this.radPanel1 = new Telerik.WinControls.UI.RadPanel();
            this.fpSpreadMenu = new FarPoint.Win.Spread.FpSpread();
            this.fpSpreadMenu_Sheet1 = new FarPoint.Win.Spread.SheetView();
            this.radButtonDown = new TechSun.Theme.KRadDownImageButton();
            this.radButtonUp = new TechSun.Theme.KRadUpImageButton();
            this.radButtonDelete = new TechSun.Theme.KRadCloseImageButton();
            this.radButtonCreate = new TechSun.Theme.KRadPlusImageButton();
            this.radDropDownListSystemList = new Telerik.WinControls.UI.RadDropDownList();
            this.radLabel1 = new Telerik.WinControls.UI.RadLabel();
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).BeginInit();
            this.radPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadMenu)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadMenu_Sheet1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSystemList)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).BeginInit();
            this.SuspendLayout();
            flatRowHeaderRenderer1.ActiveForeColor = System.Drawing.Color.White;
            flatRowHeaderRenderer1.ActiveMouseOverBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(187)))), ((int)(((byte)(196)))), ((int)(((byte)(209)))));
            flatRowHeaderRenderer1.ActiveSelectedBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(74)))), ((int)(((byte)(101)))), ((int)(((byte)(140)))));
            flatRowHeaderRenderer1.ContainSelectBackColor = System.Drawing.Color.FromArgb(((int)(((byte)(74)))), ((int)(((byte)(101)))), ((int)(((byte)(140)))));
            flatRowHeaderRenderer1.Name = "flatRowHeaderRenderer1";
            flatRowHeaderRenderer1.PictureZoomEffect = false;
            flatRowHeaderRenderer1.TextRotationAngle = 0D;
            flatRowHeaderRenderer1.ZoomFactor = 1F;
            enhancedRowHeaderRenderer1.BackColor = System.Drawing.SystemColors.Control;
            enhancedRowHeaderRenderer1.Font = new System.Drawing.Font("Gulim", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            enhancedRowHeaderRenderer1.ForeColor = System.Drawing.SystemColors.ControlText;
            enhancedRowHeaderRenderer1.Name = "enhancedRowHeaderRenderer1";
            enhancedRowHeaderRenderer1.NormalBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(216)))), ((int)(((byte)(226)))));
            enhancedRowHeaderRenderer1.NormalGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))));
            enhancedRowHeaderRenderer1.PictureZoomEffect = false;
            enhancedRowHeaderRenderer1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            enhancedRowHeaderRenderer1.SelectedActiveBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(249)))), ((int)(((byte)(252)))), ((int)(((byte)(230)))));
            enhancedRowHeaderRenderer1.SelectedBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(249)))), ((int)(((byte)(252)))), ((int)(((byte)(230)))));
            enhancedRowHeaderRenderer1.SelectedGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))));
            enhancedRowHeaderRenderer1.TextRotationAngle = 0D;
            enhancedRowHeaderRenderer1.ZoomFactor = 1F;
            enhancedColumnHeaderRenderer1.ActiveBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedColumnHeaderRenderer1.Name = "enhancedColumnHeaderRenderer1";
            enhancedColumnHeaderRenderer1.NormalGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(230)))), ((int)(((byte)(230)))));
            enhancedColumnHeaderRenderer1.PictureZoomEffect = false;
            enhancedColumnHeaderRenderer1.SelectedActiveBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedColumnHeaderRenderer1.SelectedBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(223)))), ((int)(((byte)(240)))), ((int)(((byte)(248)))));
            enhancedColumnHeaderRenderer1.SelectedGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(230)))), ((int)(((byte)(230)))));
            enhancedColumnHeaderRenderer1.TextRotationAngle = 0D;
            enhancedColumnHeaderRenderer1.ZoomFactor = 1F;
            enhancedRowHeaderRenderer2.ActiveBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedRowHeaderRenderer2.Name = "enhancedRowHeaderRenderer2";
            enhancedRowHeaderRenderer2.NormalGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(230)))), ((int)(((byte)(230)))));
            enhancedRowHeaderRenderer2.PictureZoomEffect = false;
            enhancedRowHeaderRenderer2.SelectedActiveBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedRowHeaderRenderer2.SelectedBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(223)))), ((int)(((byte)(240)))), ((int)(((byte)(248)))));
            enhancedRowHeaderRenderer2.SelectedGridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(230)))), ((int)(((byte)(230)))));
            enhancedRowHeaderRenderer2.TextRotationAngle = 0D;
            enhancedRowHeaderRenderer2.ZoomFactor = 1F;
            // 
            // radPanel1
            // 
            this.radPanel1.Controls.Add(this.fpSpreadMenu);
            this.radPanel1.Controls.Add(this.radButtonDown);
            this.radPanel1.Controls.Add(this.radButtonUp);
            this.radPanel1.Controls.Add(this.radButtonDelete);
            this.radPanel1.Controls.Add(this.radButtonCreate);
            this.radPanel1.Controls.Add(this.radDropDownListSystemList);
            this.radPanel1.Controls.Add(this.radLabel1);
            this.radPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.radPanel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radPanel1.Location = new System.Drawing.Point(0, 0);
            this.radPanel1.Name = "radPanel1";
            this.radPanel1.Size = new System.Drawing.Size(1075, 807);
            this.radPanel1.TabIndex = 1;
            // 
            // fpSpreadMenu
            // 
            this.fpSpreadMenu.AccessibleDescription = "fpSpread1, Sheet1, Row 0, Column 0, ";
            this.fpSpreadMenu.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.fpSpreadMenu.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("fpSpreadMenu.BackgroundImage")));
            this.fpSpreadMenu.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.fpSpreadMenu.HorizontalScrollBarPolicy = FarPoint.Win.Spread.ScrollBarPolicy.AsNeeded;
            this.fpSpreadMenu.Location = new System.Drawing.Point(3, 32);
            this.fpSpreadMenu.Name = "fpSpreadMenu";
            namedStyle1.BackColor = System.Drawing.SystemColors.Window;
            generalCellType1.AllowEditorVerticalAlign = true;
            namedStyle1.CellType = generalCellType1;
            namedStyle1.ForeColor = System.Drawing.SystemColors.WindowText;
            namedStyle1.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            namedStyle1.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle1.Renderer = generalCellType1;
            namedStyle1.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            namedStyle1.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(216)))), ((int)(((byte)(226)))));
            namedStyle2.Border = complexBorder1;
            namedStyle2.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            namedStyle2.ForeColor = System.Drawing.Color.Black;
            namedStyle2.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle2.Locked = false;
            namedStyle2.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle2.NoteStyle = FarPoint.Win.Spread.NoteStyle.PopupNote;
            namedStyle2.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle2.VisualStyles = FarPoint.Win.VisualStyles.Off;
            namedStyle3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(216)))), ((int)(((byte)(226)))));
            namedStyle3.Border = complexBorder2;
            namedStyle3.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            namedStyle3.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle3.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle3.Locked = false;
            namedStyle3.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle3.NoteStyle = FarPoint.Win.Spread.NoteStyle.PopupNote;
            namedStyle3.Renderer = flatRowHeaderRenderer1;
            namedStyle3.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle3.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle4.BackColor = System.Drawing.Color.White;
            filterBarCellType1.FormatString = "";
            namedStyle4.CellType = filterBarCellType1;
            namedStyle4.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle4.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle4.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle4.Renderer = filterBarCellType1;
            namedStyle4.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle4.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle5.BackColor = System.Drawing.Color.White;
            namedStyle5.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle5.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle5.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle5.Renderer = enhancedRowHeaderRenderer2;
            namedStyle5.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle5.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle6.BackColor = System.Drawing.Color.White;
            namedStyle6.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle6.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle6.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle6.Renderer = enhancedColumnHeaderRenderer1;
            namedStyle6.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle6.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle7.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            namedStyle7.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle7.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle7.Locked = false;
            namedStyle7.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle7.NoteStyle = FarPoint.Win.Spread.NoteStyle.PopupNote;
            namedStyle7.Renderer = enhancedRowHeaderRenderer1;
            namedStyle7.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle7.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle8.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(216)))), ((int)(((byte)(226)))));
            namedStyle8.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle8.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle8.Locked = false;
            namedStyle8.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle8.NoteStyle = FarPoint.Win.Spread.NoteStyle.PopupNote;
            flatCornerHeaderRenderer1.ActiveForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            flatCornerHeaderRenderer1.ActiveMouseOverForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            flatCornerHeaderRenderer1.GridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(134)))), ((int)(((byte)(138)))), ((int)(((byte)(150)))));
            flatCornerHeaderRenderer1.NormalTriangleColor = System.Drawing.Color.FromArgb(((int)(((byte)(82)))), ((int)(((byte)(89)))), ((int)(((byte)(105)))));
            namedStyle8.Renderer = flatCornerHeaderRenderer1;
            namedStyle8.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle8.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle9.BackColor = System.Drawing.Color.White;
            namedStyle9.ForeColor = System.Drawing.SystemColors.ControlText;
            namedStyle9.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            namedStyle9.NoteIndicatorColor = System.Drawing.Color.Red;
            flatCornerFooterRenderer1.ActiveForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            flatCornerFooterRenderer1.ActiveMouseOverForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            flatCornerFooterRenderer1.GridLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(230)))), ((int)(((byte)(230)))));
            flatCornerFooterRenderer1.NormalTriangleColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            namedStyle9.Renderer = flatCornerFooterRenderer1;
            namedStyle9.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle9.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            namedStyle10.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            namedStyle10.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(68)))), ((int)(((byte)(68)))), ((int)(((byte)(68)))));
            namedStyle10.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            namedStyle10.Locked = false;
            namedStyle10.NoteIndicatorColor = System.Drawing.Color.Red;
            namedStyle10.NoteStyle = FarPoint.Win.Spread.NoteStyle.PopupNote;
            generalCellType2.AllowEditorVerticalAlign = true;
            namedStyle10.Renderer = generalCellType2;
            namedStyle10.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.Center;
            namedStyle10.VisualStyles = FarPoint.Win.VisualStyles.Auto;
            this.fpSpreadMenu.NamedStyles.AddRange(new FarPoint.Win.Spread.NamedStyle[] {
            namedStyle1,
            namedStyle2,
            namedStyle3,
            namedStyle4,
            namedStyle5,
            namedStyle6,
            namedStyle7,
            namedStyle8,
            namedStyle9,
            namedStyle10});
            this.fpSpreadMenu.Sheets.AddRange(new FarPoint.Win.Spread.SheetView[] {
            this.fpSpreadMenu_Sheet1});
            this.fpSpreadMenu.Size = new System.Drawing.Size(1069, 772);
            spreadSkin1.ColumnFooterDefaultStyle = namedStyle6;
            spreadSkin1.ColumnHeaderDefaultStyle = namedStyle3;
            spreadSkin1.CornerDefaultStyle = namedStyle8;
            spreadSkin1.CornerFooterDefaultStyle = namedStyle9;
            spreadSkin1.DefaultStyle = namedStyle10;
            spreadSkin1.FilterBarDefaultStyle = namedStyle4;
            spreadSkin1.FilterBarHeaderDefaultStyle = namedStyle5;
            flatFocusIndicatorRenderer1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            flatFocusIndicatorRenderer1.Thickness = 3;
            spreadSkin1.FocusRenderer = flatFocusIndicatorRenderer1;
            enhancedInterfaceRenderer1.ArrowColorDisabled = System.Drawing.Color.FromArgb(((int)(((byte)(178)))), ((int)(((byte)(178)))), ((int)(((byte)(178)))));
            enhancedInterfaceRenderer1.ArrowColorEnabled = System.Drawing.Color.FromArgb(((int)(((byte)(166)))), ((int)(((byte)(172)))), ((int)(((byte)(179)))));
            enhancedInterfaceRenderer1.GrayAreaColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.RangeGroupBackgroundColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.RangeGroupButtonBorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(102)))), ((int)(((byte)(102)))), ((int)(((byte)(102)))));
            enhancedInterfaceRenderer1.RangeGroupLineColor = System.Drawing.Color.FromArgb(((int)(((byte)(102)))), ((int)(((byte)(102)))), ((int)(((byte)(102)))));
            enhancedInterfaceRenderer1.ScrollBoxBackgroundColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.SheetTabBorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(166)))), ((int)(((byte)(172)))), ((int)(((byte)(179)))));
            enhancedInterfaceRenderer1.SheetTabLowerActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedInterfaceRenderer1.SheetTabLowerNormalColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.SheetTabUpperActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedInterfaceRenderer1.SheetTabUpperNormalColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.SplitBarBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(223)))), ((int)(((byte)(227)))), ((int)(((byte)(240)))));
            enhancedInterfaceRenderer1.SplitBarDarkColor = System.Drawing.Color.FromArgb(((int)(((byte)(223)))), ((int)(((byte)(227)))), ((int)(((byte)(240)))));
            enhancedInterfaceRenderer1.SplitBarLightColor = System.Drawing.Color.FromArgb(((int)(((byte)(223)))), ((int)(((byte)(227)))), ((int)(((byte)(240)))));
            enhancedInterfaceRenderer1.SplitBoxBackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            enhancedInterfaceRenderer1.SplitBoxBorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(213)))), ((int)(((byte)(213)))), ((int)(((byte)(213)))));
            enhancedInterfaceRenderer1.TabStripBackgroundColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.TabStripButtonBorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(166)))), ((int)(((byte)(172)))), ((int)(((byte)(179)))));
            enhancedInterfaceRenderer1.TabStripButtonFlatStyle = System.Windows.Forms.FlatStyle.Flat;
            enhancedInterfaceRenderer1.TabStripButtonLowerActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedInterfaceRenderer1.TabStripButtonLowerNormalColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.TabStripButtonLowerPressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedInterfaceRenderer1.TabStripButtonStyle = FarPoint.Win.Spread.EnhancedInterfaceRenderer.ButtonStyles.Flat;
            enhancedInterfaceRenderer1.TabStripButtonUpperActiveColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            enhancedInterfaceRenderer1.TabStripButtonUpperNormalColor = System.Drawing.Color.White;
            enhancedInterfaceRenderer1.TabStripButtonUpperPressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(186)))), ((int)(((byte)(234)))), ((int)(((byte)(253)))));
            spreadSkin1.InterfaceRenderer = enhancedInterfaceRenderer1;
            spreadSkin1.Name = "kSkin";
            spreadSkin1.RowHeaderDefaultStyle = namedStyle7;
            spreadSkin1.SelectionRenderer = new FarPoint.Win.Spread.DefaultSelectionRenderer();
            this.fpSpreadMenu.Skin = spreadSkin1;
            this.fpSpreadMenu.TabIndex = 70;
            this.fpSpreadMenu.VerticalScrollBarPolicy = FarPoint.Win.Spread.ScrollBarPolicy.AsNeeded;
            this.fpSpreadMenu.CellClick += new FarPoint.Win.Spread.CellClickEventHandler(this.fpSpreadMenu_CellClick);
            // 
            // fpSpreadMenu_Sheet1
            // 
            this.fpSpreadMenu_Sheet1.Reset();
            this.fpSpreadMenu_Sheet1.SheetName = "Sheet1";
            // Formulas and custom names must be loaded with R1C1 reference style
            this.fpSpreadMenu_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.R1C1;
            this.fpSpreadMenu_Sheet1.ColumnHeader.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.Center;
            this.fpSpreadMenu_Sheet1.ColumnHeader.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadMenu_Sheet1.ColumnHeader.DefaultStyle.Parent = "Style6";
            this.fpSpreadMenu_Sheet1.ColumnHeader.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadMenu_Sheet1.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadMenu_Sheet1.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadMenu_Sheet1.DefaultStyle.Parent = "Style4";
            this.fpSpreadMenu_Sheet1.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadMenu_Sheet1.RowHeader.DefaultStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadMenu_Sheet1.RowHeader.DefaultStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadMenu_Sheet1.RowHeader.DefaultStyle.Parent = "Style3";
            this.fpSpreadMenu_Sheet1.RowHeader.DefaultStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadMenu_Sheet1.SheetCornerStyle.HorizontalAlignment = FarPoint.Win.Spread.CellHorizontalAlignment.General;
            this.fpSpreadMenu_Sheet1.SheetCornerStyle.NoteIndicatorColor = System.Drawing.Color.Red;
            this.fpSpreadMenu_Sheet1.SheetCornerStyle.Parent = "Style2";
            this.fpSpreadMenu_Sheet1.SheetCornerStyle.VerticalAlignment = FarPoint.Win.Spread.CellVerticalAlignment.General;
            this.fpSpreadMenu_Sheet1.ReferenceStyle = FarPoint.Win.Spread.Model.ReferenceStyle.A1;
            // 
            // radButtonDown
            // 
            this.radButtonDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDown.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDown.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDown.Image")));
            this.radButtonDown.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDown.Location = new System.Drawing.Point(1048, 3);
            this.radButtonDown.Name = "radButtonDown";
            this.radButtonDown.Size = new System.Drawing.Size(24, 26);
            this.radButtonDown.TabIndex = 62;
            this.radButtonDown.ThemeName = "Office2013Light";
            this.radButtonDown.Click += new System.EventHandler(this.radButtonDown_Click);
            // 
            // radButtonUp
            // 
            this.radButtonUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonUp.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonUp.Image = ((System.Drawing.Image)(resources.GetObject("radButtonUp.Image")));
            this.radButtonUp.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonUp.Location = new System.Drawing.Point(1018, 3);
            this.radButtonUp.Name = "radButtonUp";
            this.radButtonUp.Size = new System.Drawing.Size(24, 26);
            this.radButtonUp.TabIndex = 61;
            this.radButtonUp.ThemeName = "Office2013Light";
            this.radButtonUp.Click += new System.EventHandler(this.radButtonUp_Click);
            // 
            // radButtonDelete
            // 
            this.radButtonDelete.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonDelete.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonDelete.Image = ((System.Drawing.Image)(resources.GetObject("radButtonDelete.Image")));
            this.radButtonDelete.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonDelete.Location = new System.Drawing.Point(988, 3);
            this.radButtonDelete.Name = "radButtonDelete";
            this.radButtonDelete.Size = new System.Drawing.Size(24, 26);
            this.radButtonDelete.TabIndex = 60;
            this.radButtonDelete.ThemeName = "Office2013Light";
            this.radButtonDelete.Click += new System.EventHandler(this.radButtonDelete_Click);
            // 
            // radButtonCreate
            // 
            this.radButtonCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.radButtonCreate.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radButtonCreate.Image = ((System.Drawing.Image)(resources.GetObject("radButtonCreate.Image")));
            this.radButtonCreate.ImageAlignment = System.Drawing.ContentAlignment.MiddleCenter;
            this.radButtonCreate.Location = new System.Drawing.Point(958, 3);
            this.radButtonCreate.Name = "radButtonCreate";
            this.radButtonCreate.Size = new System.Drawing.Size(24, 26);
            this.radButtonCreate.TabIndex = 59;
            this.radButtonCreate.ThemeName = "Office2013Light";
            this.radButtonCreate.Click += new System.EventHandler(this.radButtonCreate_Click);
            // 
            // radDropDownListSystemList
            // 
            this.radDropDownListSystemList.DropDownStyle = Telerik.WinControls.RadDropDownStyle.DropDownList;
            this.radDropDownListSystemList.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radDropDownListSystemList.Location = new System.Drawing.Point(80, 8);
            this.radDropDownListSystemList.Name = "radDropDownListSystemList";
            this.radDropDownListSystemList.Size = new System.Drawing.Size(246, 19);
            this.radDropDownListSystemList.TabIndex = 66;
            this.radDropDownListSystemList.ThemeName = "Office2010Silver";
            this.radDropDownListSystemList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSystemList_SelectedIndexChanged);
            // 
            // radLabel1
            // 
            this.radLabel1.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.radLabel1.Location = new System.Drawing.Point(12, 9);
            this.radLabel1.Name = "radLabel1";
            this.radLabel1.Size = new System.Drawing.Size(42, 17);
            this.radLabel1.TabIndex = 59;
            this.radLabel1.Text = "System";
            // 
            // UserControlCommonMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.radPanel1);
            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            this.Name = "UserControlCommonMenu";
            this.Size = new System.Drawing.Size(1075, 807);
            ((System.ComponentModel.ISupportInitialize)(this.radPanel1)).EndInit();
            this.radPanel1.ResumeLayout(false);
            this.radPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadMenu)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.fpSpreadMenu_Sheet1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonDelete)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radButtonCreate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radDropDownListSystemList)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.radLabel1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Telerik.WinControls.UI.RadPanel radPanel1;
        private Telerik.WinControls.UI.RadLabel radLabel1;
        private Telerik.WinControls.UI.RadDropDownList radDropDownListSystemList;
        private TechSun.Theme.KRadDownImageButton radButtonDown;
        private TechSun.Theme.KRadUpImageButton radButtonUp;
        private TechSun.Theme.KRadCloseImageButton radButtonDelete;
        private TechSun.Theme.KRadPlusImageButton radButtonCreate;
        public FarPoint.Win.Spread.FpSpread fpSpreadMenu;
        private FarPoint.Win.Spread.SheetView fpSpreadMenu_Sheet1;
    }
}
