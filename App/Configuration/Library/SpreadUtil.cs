using System;
using System.Drawing;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.DSL;
using FarPoint.Excel;
using FarPoint.Win;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using Admin.WF.Properties;

namespace Admin.WF
{
    public static class SpreadUtil
    {
        [Flags]
        public enum SearchConditionFlags { CellText = 0x1, CellTag = 0x2, CellNote = 0x4 };

        public enum m_enComplexBorder_Type { LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3 };

        public enum eFarpointCellType { NoType, DisEnableMultiCheckBox, CheckBox, Text, MultiLineText, NormalButton, DeleteButton,  DateTime, Image };

        public static void InitSpreadSheet(FpSpread spread, SheetView sheetView, int[] arColumnWidth, string[] arColumnName,
           bool[] arColumnLock, Color[] arColumnColor, Color[] arColumnHeaderColor,
           eFarpointCellType[] arColumnCellType,
           Font[] arColumnDataFont, Font[] arColumnHeaderFont
           , Color cGrayAreaBackColor, bool bVisibleRowHeader, bool bAutoFilterAndSort)
        {
            // TODO 구버전
            //SetColumnWidth(sheetView, arColumnWidth);
            //SetColumnName(sheetView, arColumnName);
            //SetColumnLock(sheetView, arColumnLock);
            //SetColumnBackColor(sheetView, arColumnColor);
            //SetColumnHeaderBackColor(sheetView, arColumnHeaderColor);

            //sheetView.RowCount = 0;
            //sheetView.ColumnCount = arColumnWidth.Length;
            //sheetView.GrayAreaBackColor = cGrayAreaBackColor;
            //sheetView.RowHeader.Visible = bVisibleRowHeader;

            /////// 신버전
            if (sheetView == null)
            {
                sheetView = new SheetView();
                spread.Sheets.Add(sheetView);
                spread.ActiveSheet = sheetView;
            }

            SetColumnWidth(sheetView, arColumnWidth);
            SetColumnName(sheetView, arColumnName);
            SetColumnLock(sheetView, arColumnLock);
            //SetColumnBackColor(sheetView, arColumnColor);
            //SetColumnHeaderBackColor(sheetView, arColumnHeaderColor);
            //SetColumnDataFont(sheetView, arColumnDataFont);

            if (sheetView != null)
            {
                sheetView.RowCount = 0;
                sheetView.ColumnCount = arColumnWidth.Length;
                //sheetView.GrayAreaBackColor = cGrayAreaBackColor;
                sheetView.RowHeader.Visible = bVisibleRowHeader;
            }
            ///////

            if (bAutoFilterAndSort)
                SetAutoFilterAndAutoSort(sheetView, AutoFilterMode.EnhancedContextMenu);

            //CellType 지정시 AutoFilter&Sort를 False로 변경.
            SetColumnCellType(sheetView, arColumnCellType);

            spread.VerticalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            spread.HorizontalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            spread.Font = Information.FONT_REGULAR;
        }

        public static void InitSpread(FpSpread spread, int[] arColumnWidth, string[] arColumnName,
        bool[] arColumnLock, Color[] arColumnColor, Color[] arColumnHeaderColor,
        eFarpointCellType[] arColumnCellType,
        Font[] arColumnDataFont, Font[] arColumnHeaderFont
        , Color cGrayAreaBackColor, bool bVisibleRowHeader, bool bAutoFilterAndSort, bool reset = true)
        {
            // TODO 구버전
            //if (reset)
            //    spread.Reset();
            SheetView sheetView = spread.ActiveSheet;

            InitSpreadSheet(spread, sheetView, arColumnWidth, arColumnName, arColumnLock, arColumnColor, arColumnHeaderColor, arColumnCellType, arColumnDataFont, arColumnHeaderFont, cGrayAreaBackColor, bVisibleRowHeader, bAutoFilterAndSort);
        }

        public static void SetSpreadSheet(FpSpread spread, SheetView sheetView, int[] arColumnWidth, string[] arColumnName,
           bool[] arColumnLock, Color[] arColumnColor, Color[] arColumnHeaderColor,
           eFarpointCellType[] arColumnCellType,
           Font[] arColumnDataFont, Font[] arColumnHeaderFont
           , Color cGrayAreaBackColor, bool bVisibleRowHeader, bool bAutoFilterAndSort)
        {
            SetColumnWidth(sheetView, arColumnWidth);
            SetColumnName(sheetView, arColumnName);
            SetColumnLock(sheetView, arColumnLock);
            SetColumnBackColor(sheetView, arColumnColor);
            SetColumnHeaderBackColor(sheetView, arColumnHeaderColor);

            sheetView.ColumnCount = arColumnWidth.Length;
            sheetView.GrayAreaBackColor = cGrayAreaBackColor;
            sheetView.RowHeader.Visible = bVisibleRowHeader;

            if (bAutoFilterAndSort)
                SetAutoFilterAndAutoSort(sheetView, AutoFilterMode.EnhancedContextMenu);

            //CellType 지정시 AutoFilter&Sort를 False로 변경.
            SetColumnCellType(sheetView, arColumnCellType);
        }

        public static void SetColumnCellType(SheetView sheetview, eFarpointCellType[] arColumnCellType)
        {
            if (sheetview == null)
                return;

            if (arColumnCellType == null)
                return;

            for (int i = 0; i < arColumnCellType.Length; i++)
            {
                if (arColumnCellType[i] == eFarpointCellType.Text)
                {
                    TextCellType ctText = new TextCellType();
                    sheetview.Columns[i].CellType = ctText;
                }
                else if (arColumnCellType[i] == eFarpointCellType.MultiLineText)
                {
                    TextCellType ctText = new TextCellType();
                    ctText.Multiline = true;
                    sheetview.Columns[i].CellType = ctText;

                }
                else if (arColumnCellType[i] == eFarpointCellType.NormalButton)
                {
                    ButtonCellType ctButton = new ButtonCellType();
                    ctButton.Text = sheetview.Columns[i].Label;
                    sheetview.Columns[i].CellType = ctButton;

                    sheetview.Columns[i].AllowAutoFilter = false;
                    sheetview.Columns[i].AllowAutoSort = false;
                }
                else if (arColumnCellType[i] == eFarpointCellType.DeleteButton)
                {
                    ButtonCellType ctButton = new ButtonCellType();
                    ctButton.Text = sheetview.Columns[i].Label;
                    ctButton.UseVisualStyleBackColor = false;
                    ctButton.ButtonColor = Color.Red;
                    ctButton.TextColor = Color.White;
                    sheetview.Columns[i].CellType = ctButton;

                    sheetview.Columns[i].AllowAutoFilter = false;
                    sheetview.Columns[i].AllowAutoSort = false;
                }
                else if (arColumnCellType[i] == eFarpointCellType.CheckBox)
                {
                    CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                    sheetview.Columns[i].CellType = ctCheckBox;
                    sheetview.ColumnHeader.Cells[0, i].CellType = ctCheckBox;
                    sheetview.ColumnHeader.Cells[0, i].Value = false;
                    sheetview.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                    sheetview.Columns[i].AllowAutoFilter = false;
                    sheetview.Columns[i].AllowAutoSort = false;
                }
                else if (arColumnCellType[i] == eFarpointCellType.DisEnableMultiCheckBox)
                {
                    CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                    sheetview.Columns[i].CellType = ctCheckBox;
                    sheetview.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                    sheetview.Columns[i].AllowAutoFilter = false;
                    sheetview.Columns[i].AllowAutoSort = false;
                }
                else if (arColumnCellType[i] == eFarpointCellType.Image)
                {
                    ImageCellType ctImage = new ImageCellType();
                    sheetview.Columns[i].CellType = ctImage;

                    sheetview.Columns[i].AllowAutoFilter = false;
                    sheetview.Columns[i].AllowAutoSort = false;
                }
                else if (arColumnCellType[i] == eFarpointCellType.DateTime)
                {
                    DateTimeCellType ctDateTime = new DateTimeCellType();
                    sheetview.Columns[i].CellType = ctDateTime;
                }

                else if (arColumnCellType[i] == eFarpointCellType.NoType)
                {
                    //Nothing
                }
            }

        }

        public static void SetAutoRowHeight(SheetView sheetview)
        {
            foreach (Row row in sheetview.Rows)
            {
                row.Height = row.GetPreferredHeight() + 5;
            }
        }

        public static void SetAllCheckBox(FpSpread fpSpread, CellClickEventArgs e)
        {
            CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
            HitTestInformation hti;
            HeaderHitTestInformation hi;

            hti = fpSpread.HitTest(e.X, e.Y);
            hi = hti.HeaderInfo;

            if (hti.Type == HitTestType.ColumnHeader)
            {
                if (hi.Column == 0)
                {
                    SheetView sheetView = fpSpread.ActiveSheet;

                    bool bAllCheck = !Convert.ToBoolean(sheetView.ColumnHeader.Cells[0, hi.Column].Value);
                    sheetView.ColumnHeader.Cells[0, hi.Column].Value = bAllCheck;

                    if (sheetView != null)
                    {
                        for (int i = 0; i < sheetView.Rows.Count; i++)
                        {
                            sheetView.Cells[i, 0].Value = bAllCheck;
                        }
                    }
                }
            }
        }


        public static void SetAutoFilterAndAutoSort(SheetView sheetview, AutoFilterMode autoFilterMode)
        {
            if (sheetview == null)
                return;

            foreach (Column col in sheetview.Columns)
            {
                //col.AllowAutoFilter = true;
                //col.AllowAutoSort = true;
                col.AllowAutoFilter = false;
                col.AllowAutoSort = false;
            }

            sheetview.AutoFilterMode = autoFilterMode;
        }

        public static void SetAutoFilterAndAutoSortReal(SheetView sheetview, AutoFilterMode autoFilterMode)
        {
            if (sheetview == null)
                return;

            foreach (Column col in sheetview.Columns)
            {
                col.AllowAutoFilter = true;
                col.AllowAutoSort = true;
            }

            sheetview.AutoFilterMode = autoFilterMode;
        }


        public static void SetColumnBackColor(SheetView sheetview, Color[] arColor)
        {
            if (sheetview == null)
                return;

            if (arColor == null)
                return;

            for (int i = 0; i < arColor.Length; i++)
            {
                sheetview.Columns[i].BackColor = arColor[i];
            }
        }

        //public static void SetColumnDataFont(SheetView sheetview, Font[] arFont)
        //{
        //    if (sheetview == null)
        //        return;

        //    for (int i = 0; i < sheetview.ColumnCount; i++)
        //    {
        //        if (arFont == null)
        //            sheetview.Columns[i].Font = Information.FONT_REGULAR;
        //        else
        //            sheetview.Columns[i].Font = arFont[i];
        //    }
        //}

        public static void SetColumnHeaderFont(SheetView sheetview, Font[] arFont)
        {
            if (sheetview == null)
                return;

            for (int i = 0; i < sheetview.ColumnCount; i++)
            {
                if (arFont == null)
                    sheetview.Columns[i].Font = Information.FONT_REGULAR;
                else
                    sheetview.ColumnHeader.Cells[0, i].Font = arFont[i];
            }
        }

        public static void SetColumnHeaderBackColor(SheetView sheetview, Color[] arColor)
        {
            if (sheetview == null)
                return;

            if (arColor == null)
                return;

            for (int i = 0; i < arColor.Length; i++)
            {
                sheetview.ColumnHeader.Columns[i].BackColor = arColor[i];
            }
        }

        public static void SetColumnName(SheetView sheetview, string[] arColumnName)
        {
            if (sheetview == null)
                return;

            if (arColumnName == null)
                return;

            for (int i = 0; i < arColumnName.Length; i++)
            {
                sheetview.Columns[i].Label = arColumnName[i];
            }
        }

        public static void SetAllColumnLock(SheetView sheetview)
        {
            if (sheetview == null)
                return;

            foreach (Column col in sheetview.Columns)
            {
                col.Locked = true;
            }
        }

        public static void SetColumnLock(SheetView sheetview, bool[] arColumnLock)
        {
            if (sheetview == null)
                return;

            if (arColumnLock == null)
                return;

            for (int i = 0; i < arColumnLock.Length; i++)
            {
                if (arColumnLock[i] == true)
                    sheetview.Columns[i].Locked = true;
            }
        }

        public static void SetColumnWidth(SheetView sheetview, int[] arColumnWidth)
        {
            if (sheetview == null)
                return;

            if (arColumnWidth == null)
                return;

            for (int i = 0; i < arColumnWidth.Length; i++)
            {
                if (arColumnWidth[i] == 0)
                    sheetview.Columns[i].Visible = false;
                else
                    sheetview.Columns[i].Width = arColumnWidth[i];
            }
        }

        public static void SetGoMoveToNextRow(FpSpread spread)
        {
            InputMap inputMap = spread.GetInputMap(InputMapMode.WhenAncestorOfFocused);
            inputMap.Put(new Keystroke(Keys.Enter, Keys.None), SpreadActions.MoveToNextRow);
        }

        public static void SetColumnPerferredWidth(SheetView sheetview, int iMinWidth)
        {
            if (sheetview == null)
                return;

            foreach (Column col in sheetview.Columns)
            {
                col.Width = col.GetPreferredWidth() + 5;

                if (iMinWidth == 0)
                    continue;

                if (col.Width < iMinWidth)
                    col.Width = iMinWidth;
            }
        }

        public static void SetRowPerferredHeight(SheetView sheetview)
        {
            if (sheetview == null)
                return;

            foreach (Row row in sheetview.Rows)
            {
                row.Height = row.GetPreferredHeight() + 5;
            }
        }

        public static SheetView FindSheetViewBySheetName(FpSpread spread, string sheetName)
        {
            foreach (SheetView sheetView in spread.Sheets)
            {
                if (sheetView.SheetName.Equals(sheetName))
                    return sheetView;
            }

            return null;
        }

        public static void FilteringText(SheetView sheetview, int iFilteringColIdx, string sText)
        {
            if (sheetview == null)
                return;


            for (int i = 0; i < sheetview.Rows.Count; i++)
            {
                string sAttributeName = sheetview.GetText(i, iFilteringColIdx);

                if (sText == string.Empty)
                {
                    if (sheetview.Rows[i].Visible == false)
                    {
                        sheetview.Rows[i].Visible = true;
                    }
                }
                else
                {
                    if (!sAttributeName.ToUpper().Contains(sText.ToUpper()))
                    {
                        sheetview.Rows[i].Visible = false;
                    }
                    else
                    {
                        sheetview.Rows[i].Visible = true;
                    }
                }
            }
        }

        public static int GetCheckRowCount(SheetView sheetView, int iCheckColumnIdx)
        {
            int iCheckedCount = 0;

            for (int i = 0; i < sheetView.RowCount; i++)
            {
                if (sheetView.Cells[i, iCheckColumnIdx].Text == "True")
                {
                    iCheckedCount++;
                }
            }

            return iCheckedCount;

        }

        public static string LoadExcel(IWin32Window winObject, FpSpread spread, string sDefaultFilePath, bool bIncludeColumnHeader)
        {
            string sFilePath = sDefaultFilePath;

            if (sFilePath == string.Empty)
            {
                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Title = "Open Excel Data File";
                ofd.Filter = "Excel Data File(*.xls, *.xlsx)|*.xls; *.xlsx";
                ofd.FilterIndex = 1;

                if (ofd.ShowDialog(winObject) == DialogResult.OK)
                {
                    sFilePath = ofd.FileName;
                }
                else
                {
                    return string.Empty;
                }
            }

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Opening excel file. please wait!";

            spread.Reset();

            try
            {
                if (bIncludeColumnHeader)
                    spread.OpenExcel(sFilePath, ExcelOpenFlags.ColumnHeaders);
                else
                    spread.OpenExcel(sFilePath);
            }
            catch (ApplicationException)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(string.Format(Resources.Message10018, sFilePath), "Load Excel");
                return string.Empty;
            }

            FrmProgressStatus.FormClose();
            return sFilePath;
        }

        public static bool SaveExcel(FpSpread spread)
        {
            SaveFileDialog dlgSaveFile = new SaveFileDialog();
            dlgSaveFile.Title = "Save Excel File";
            dlgSaveFile.Filter = "Excel File(*.xlsx)|*.xlsx";
            dlgSaveFile.FilterIndex = 1;
            dlgSaveFile.RestoreDirectory = true;
            dlgSaveFile.OverwritePrompt = true;

            if (dlgSaveFile.ShowDialog() != DialogResult.OK)
                return false;

            Application.DoEvents();
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Saving excel.  Please wait.";

            try
            {
                string sFilePath = dlgSaveFile.FileName;

                try
                {
                    spread.SaveExcel(sFilePath, ExcelSaveFlags.UseOOXMLFormat | ExcelSaveFlags.SaveCustomColumnHeaders | ExcelSaveFlags.ComboDataOnly | ExcelSaveFlags.NoFormulas | ExcelSaveFlags.SaveAsViewed);
                }
                catch (Exception ex)
                {
                    FrmProgressStatus.FormClose();
                    MessageBox.Show(string.Format("{0}\r\nPath-{1}", ex.Message, sFilePath), "Save Excel", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    return false;
                }


            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(ex.Message, "Export Excel", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return false;
            }

            FrmProgressStatus.FormClose();
            MessageBox.Show(Resources.Message10000, "Export Excel", MessageBoxButtons.OK, MessageBoxIcon.Information);
            return true;
        }

        public static int GetColumnIdxByColumnName(string column)
        {
            int retVal = 0;
            string col = column.ToUpper();
            for (int iChar = col.Length - 1; iChar >= 0; iChar--)
            {
                char colPiece = col[iChar];
                int colNum = colPiece - 64;
                retVal = retVal + colNum * (int)Math.Pow(26, col.Length - (iChar + 1));
            }
            return retVal;
        }

        public static string GetColumnNameByColumnIndex(int columnNumber)
        {
            int dividend = columnNumber;
            string columnName = string.Empty;
            int modulo;

            while (dividend > 0)
            {
                modulo = (dividend - 1) % 26;
                columnName = Convert.ToChar(65 + modulo).ToString() + columnName;
                dividend = (int)((dividend - modulo) / 26);
            }

            return columnName;
        }

        public static bool SaveExcel(FpSpread spread, string sDefaultFilePath)
        {
            return SaveExcel(spread, sDefaultFilePath, string.Empty, true);
        }

        public static bool SaveExcel(FpSpread spread, string sDefaultFilePath, bool bIncludeColumnHeader)
        {
            return SaveExcel(spread, sDefaultFilePath, string.Empty, bIncludeColumnHeader);
        }

        public static bool SaveExcel(FpSpread spread, string sDefaultFilePath, string sDefaultFileFileName, bool bIncludeColumnHeader)
        {
            string sFilePath = sDefaultFilePath;

            if (spread.ActiveSheet == null ||
               spread.ActiveSheet.Rows.Count == 0)
            {
                MessageBox.Show(Resources.Message10019, "Save Excel");
                return false;
            }

            SetHeaderLine(spread, true);

            if (sFilePath == string.Empty)
            {
                SaveFileDialog sfd = new SaveFileDialog();
                sfd.Title = "Save Excel File";
                sfd.Filter = "Excel File(*.xlsx)|*.xlsx";
                sfd.FilterIndex = 1;
                sfd.RestoreDirectory = true;
                sfd.OverwritePrompt = true;

                if (!string.IsNullOrEmpty(sDefaultFileFileName))
                {
                    sfd.FileName = sDefaultFileFileName;
                }

                if (sfd.ShowDialog() != DialogResult.OK)
                    return false;

                sFilePath = sfd.FileName;
            }

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Saving excel. Please wait.";

            try
            {
                if (spread.Sheets.Count > 0)
                {
                    foreach (SheetView Sheets in spread.Sheets)
                    {
                        Sheets.Protect = false;
                    }
                }

                if (bIncludeColumnHeader)
                {
                    spread.SaveExcel(sFilePath, ExcelSaveFlags.UseOOXMLFormat | ExcelSaveFlags.SaveCustomColumnHeaders);
                }
                else
                    spread.SaveExcel(sFilePath, ExcelSaveFlags.UseOOXMLFormat);
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(ex.Message + "\r\nPath-" + sFilePath, "Save Excel");
                return false;
            }

            SetHeaderLine(spread, false);

            FrmProgressStatus.FormClose();

            return true;
        }

        public static void SetHeaderLine(FpSpread spread, bool bLineFlag)
        {
            foreach (SheetView shview in spread.Sheets)
            {
                SetHeaderLine(shview, bLineFlag, Color.DarkGray);
            }
        }

        public static int GetColumnCount(SheetView sheetView)
        {
            int iColCount = 0;

            if (sheetView == null)
                return iColCount;


            for (int i = 0; i < sheetView.ColumnCount; i++)
            {
                if (sheetView.Columns[i].Visible == true && sheetView.Columns[i].Width != 0)
                {

                    iColCount++;
                }
            }

            return iColCount;
        }
        public static void SetHeaderLine(SheetView sheetview, bool bLineFlag, Color crBorder)
        {
            for (int i = 0; i < sheetview.ColumnHeaderRowCount; i++)
            {
                for (int j = 0; j < sheetview.ColumnHeader.Columns.Count; j++)
                {
                    if (bLineFlag)
                        sheetview.ColumnHeader.Cells[i, j].Border = new LineBorder(crBorder);
                    else
                        sheetview.ColumnHeader.Cells[i, j].Border = null;
                }
            }
        }

        public static void SetAllColumnAutoFilterAndSort(SheetView sheetview)
        {
            if (sheetview == null)
                return;

            sheetview.AutoFilterMode = AutoFilterMode.EnhancedContextMenu;

            foreach (Column col in sheetview.Columns)
            {
                col.AllowAutoFilter = true;
                col.AllowAutoSort = true;
            }

            sheetview.AutoFilterMode = AutoFilterMode.EnhancedContextMenu;
            sheetview.GrayAreaBackColor = Color.FromArgb(241, 242, 243);
        }

        public static Point Search(FpSpread spread, string searchString, SearchConditionFlags searchConditionFlags, int startRowIndex, int startColumnIndex)
        {
            int foundRowIndex = -1;
            int foundColumnIndex = -1;

            bool bCellText = false;
            bool bCellTag = false;
            bool bCellNote = false;

            switch (searchConditionFlags)
            {
                case SearchConditionFlags.CellText:
                    bCellText = true;
                    break;
                case SearchConditionFlags.CellTag:
                    bCellTag = true;
                    break;
                case SearchConditionFlags.CellText | SearchConditionFlags.CellTag:
                    bCellText = true;
                    bCellTag = true;
                    break;
                case SearchConditionFlags.CellNote:
                    bCellNote = true;
                    break;
                case SearchConditionFlags.CellText | SearchConditionFlags.CellNote:
                    bCellText = true;
                    bCellNote = true;
                    break;
                case SearchConditionFlags.CellTag | SearchConditionFlags.CellNote:
                    bCellTag = true;
                    bCellNote = true;
                    break;
                case SearchConditionFlags.CellText | SearchConditionFlags.CellTag | SearchConditionFlags.CellNote:
                    bCellText = true;
                    bCellTag = true;
                    bCellNote = true;
                    break;
                default:
                    break;
            }

            spread.Search(spread.ActiveSheetIndex, searchString, false, false, false, true, bCellText, bCellNote, bCellTag, startRowIndex, startColumnIndex, ref foundRowIndex, ref foundColumnIndex);

            return new Point(foundRowIndex, foundColumnIndex);
        }

        public static void FilteringText(ref SheetView sheetview, int iFilteringColIdx, string sText)
        {
            if (sheetview == null)
                return;

            foreach (Row row in sheetview.Rows)
            {
                row.Visible = true;
            }

            if (sText == string.Empty)
            {
                return;
            }
            else
            {

                for (int i = 0; i < sheetview.Rows.Count; i++)
                {
                    string sAttributeName = sheetview.GetText(i, iFilteringColIdx);

                    if (!sAttributeName.ToUpper().Contains(sText.ToUpper()))
                    {
                        sheetview.Rows[i].Visible = false;
                    }

                }
            }
        }



        public static void SetBorder(FarPoint.Win.Spread.Cell cell, int iComplexBorder_Type)
        {
            ComplexBorderSide Defaultborder = new ComplexBorderSide(ComplexBorderSideStyle.None, Color.Transparent);
            ComplexBorderSide border = new ComplexBorderSide(ComplexBorderSideStyle.ThinLine, Color.Black);

            switch (iComplexBorder_Type)
            {
                case 0:
                    cell.Border = new ComplexBorder(border, null, null, null);
                    break;
                case 1:
                    cell.Border = new ComplexBorder(null, border, null, null);
                    break;
                case 2:
                    cell.Border = new ComplexBorder(null, null, border, null);
                    break;
                case 3:
                    cell.Border = new ComplexBorder(null, null, null, border);
                    break;
            }
        }

        public static void SetBorder_Bottom_TypeL(FarPoint.Win.Spread.Cell cell)
        {
            ComplexBorderSide Defaultborder = new ComplexBorderSide(ComplexBorderSideStyle.None, Color.Transparent);
            ComplexBorderSide border = new ComplexBorderSide(ComplexBorderSideStyle.ThinLine, Color.Black);

            cell.Border = new ComplexBorder(border, null, null, border);

        }

        public static void SetBorder_Bottom_TypeJ(FarPoint.Win.Spread.Cell cell)
        {
            ComplexBorderSide Defaultborder = new ComplexBorderSide(ComplexBorderSideStyle.None, Color.Transparent);
            ComplexBorderSide border = new ComplexBorderSide(ComplexBorderSideStyle.ThinLine, Color.Black);

            cell.Border = new ComplexBorder(null, null, border, border);
        }

        /// <summary>
        /// Get Sheet View
        /// </summary>
        /// <param name="spread"></param>
        /// <param name="sheetName"></param>
        /// <returns>만약 Sheet가 존재하지 않는 경우 Null을 Return</returns>
        public static SheetView GetSheetView(FpSpread spread, string sheetName)
        {
            foreach (SheetView sheetView in spread.Sheets)
            {
                if (sheetView.SheetName.Equals(sheetName))
                    return sheetView;
            }

            return null;
        }


        /// <summary>
        /// Get Row Count(
        /// </summary>
        /// <param name="sheetview">찾을 SheetView</param>
        /// <param name="startRowIdx">찾기 시작행 : 0부터 시작</param>
        /// <param name="columnIdx">찾을 열 : 0부터 시작</param>
        /// <returns></returns>
        public static int GetRowCount(SheetView sheetview, int startRowIdx, int columnIdx)
        {
            int iRowCount = 0;

            for (int i = startRowIdx; i < sheetview.RowCount; i++)
            {
                string sValue = Util.NullToString(sheetview.Cells[i, columnIdx].Value);
                if (string.IsNullOrEmpty(sValue) == true)
                {
                    break;
                }
                iRowCount++;
            }

            return iRowCount;
        }


        public static void SetPreferredHeight(SheetView sheetview)
        {
            foreach (Row row in sheetview.Rows)
            {
                row.Height = row.GetPreferredHeight() + 5;
            }

            sheetview.GrayAreaBackColor = Color.LightGray;
        }
    }
}

