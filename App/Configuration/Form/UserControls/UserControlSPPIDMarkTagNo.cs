using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;

using System.Text;
using Configuration.Properties;
using CommonLibrary;

namespace Configuration
{
    public partial class UserControlSPPIDMarkTagNo : UserControl
    {

        Information m_Information;

        private enum LTColumn { LTSelect, LTProperty, LTAllowNull, LTSeparator, LTLength, LTFillString, LTAlignment, LTLast };
        private string[] mColumnName = new string[] { " ", "Property", "AllowNulls", "Separator", "Length", "Fill String", "Alignment" };
        private int[] marColumnWidths = new int[] { 60, 250, 100, 100, 100, 100, 100 };
        private bool[] marColumnLock = new bool[] { false, false, false, false, false, false, false };

        string mProjectNo = string.Empty;
        long mProjectID = -1;
        string mType = string.Empty;
        string mItemType = string.Empty;

        string mMappingSPPID_ItemType = string.Empty;
        string mMappingSPI_ItemType = string.Empty;
        string mMappingSP3D_ItemType = string.Empty;

        private Dictionary<string, string> mdicProperty = new Dictionary<string, string>();

        public UserControlSPPIDMarkTagNo()
        {
            InitializeComponent();
        }

        public UserControlSPPIDMarkTagNo(bool isIE, string sProjectNo, string sType, long lProjectID, string sItemType, bool bSaveButtonViewFlag)
        {
            InitializeComponent();

            m_Information = Information.GetInstance();
            mProjectNo = sProjectNo;
            mProjectID = lProjectID;
            mItemType = sItemType;
            mType = sType;

            radLabelTool.Text = string.Format("[{0} : {1}]", sProjectNo, sItemType);

            initPropertyArray(sProjectNo, lProjectID, sItemType);
            initSpread();
            setSpread();

            radButtonSave.Visible = bSaveButtonViewFlag;
        }

        private void initSpread()
        {
            SpreadUtil.InitSpread(fpSpreadTagFormat, marColumnWidths, mColumnName, marColumnLock, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void initPropertyArray(string sProjectNo, long lProjectID, string sItemType)
        {

            mdicProperty.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {

                List<EntitySPPID_DataColumn> lstDataColumn = proxy.ReadDataColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectID, sItemType);

                foreach (var item in lstDataColumn)
                {
                    if (!mdicProperty.ContainsKey(item.ColumnName))
                        mdicProperty.Add(item.ColumnName, item.ColumnDisplay);
                }

                Property.SetAddProperty(sProjectNo, ref mdicProperty);
            }

        }

        private void setSpread()
        {
            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                arItems = null;
                arItemsData = null;

                switch (i)
                {
                    case (int)LTColumn.LTSelect:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadTagFormat.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.LTProperty:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty.Count];
                        arItemsData = new string[mdicProperty.Count];
                        int iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                    case (int)LTColumn.LTAllowNull:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBoxs = new CheckBoxCellType();
                        fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = ctCheckBoxs;
                        fpSpreadTagFormat.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.LTSeparator:
                        arItems = new string[] { "", "-", "=", "/", "|" };
                        arItemsData = new string[] { "", "-", "=", "/", "|" };
                        break;
                    case (int)LTColumn.LTLength:
                        arItems = new string[] { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
                        arItemsData = new string[] { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
                        break;
                    case (int)LTColumn.LTAlignment:
                        arItems = new string[] { "", "Left", "Right" };
                        arItemsData = new string[] { "", "Left", "Right" };
                        break;
                    case (int)LTColumn.LTFillString:
                        arItems = new string[] { "", "' '", "0", "*" };
                        arItemsData = new string[] { "", "' '", "0", "*" };
                        break;
                }

                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // 데이터 조회
                List<EntitySPPID_TagNoSetting> lstTagRule = proxy.ReadSPPID_MarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectID, mItemType, mType);
                int iRowCount = 0;
                foreach (EntitySPPID_TagNoSetting tag in lstTagRule)
                {
                    iRowCount = fpSpreadTagFormat.ActiveSheet.Rows.Count++;

                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTProperty].Value = tag.AttributeName;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTAllowNull].Value = tag.AllowNullFlag;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTSeparator].Text = tag.Separator;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTLength].Text = tag.Length.ToString();
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTFillString].Text = tag.FillString;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTAlignment].Text = tag.Alignment;
                }

                SetTagFormat();
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadTagFormat.ActiveSheet.Rows.Count++;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            for (int i = fpSpreadTagFormat.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSelect].Text;
                if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadTagFormat.ActiveSheet.Rows.Remove(i, 1);
                }
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagFormat.ActiveSheet;

            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;
                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if ((cr.Row + i) == 0)
                        return;
                    iFrom = cr.Row + i;
                    iTo = iFrom - 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = cr.Row - 1;
                sheetView.AddSelection(cr.Row - 1, cr.Column, cr.RowCount, cr.ColumnCount);

            }
            catch (Exception ex)
            {

            }

            SetTagFormat();
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagFormat.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;

                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if (cr.Row + cr.RowCount == sheetView.RowCount)
                        return;
                    iFrom = cr.Row + cr.RowCount - 1 - i;
                    iTo = iFrom + 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = iTo;
                sheetView.AddSelection(iTo, cr.Column, cr.RowCount, cr.ColumnCount);
            }
            catch (Exception ex)
            {

            }

            SetTagFormat();
        }

        private void fpSpreadTagFormat_EditChange(object sender, EditorNotifyEventArgs e)
        {
            SetTagFormat();
        }

        private void SetTagFormat()
        {
            textBoxTagFormat.Text = "";

            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.RowCount; i++)
            {
                if (!string.IsNullOrEmpty(fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text))
                    textBoxTagFormat.Text += string.Format(" {0} [{1}]", fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text, fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text);
                else
                    textBoxTagFormat.Text += string.Format(" [{0}]", fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text);
            }
        }

        public bool SaveTagRule(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;

            DataTable dt = new DataTable("Table1");

            dt.Columns.Add("TagID", typeof(long));
            dt.Columns.Add("ItemType");
            dt.Columns.Add("Type");
            dt.Columns.Add("ProjectID", typeof(long));
            dt.Columns.Add("AttributeName");
            dt.Columns.Add("Separator");
            dt.Columns.Add("Length", typeof(int));
            dt.Columns.Add("FillString");
            dt.Columns.Add("Alignment");
            dt.Columns.Add("AllowNullFlag", typeof(bool));
            dt.Columns.Add("AllowDuplicationFlag", typeof(bool));
            dt.Columns.Add("SortOrder", typeof(int));

            bool bSaveFlag = true;

            int iSortOrder = 0;
            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.RowCount; i++)
            {
                DataRow drNew = dt.NewRow();

                string sProperty = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text;

                if (string.IsNullOrEmpty(sProperty))
                {
                    bSaveFlag = false;
                    break;
                }

                drNew["ItemType"] = mItemType;
                drNew["Type"] = mType;
                drNew["ProjectID"] = mProjectID;
                drNew["AttributeName"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Value.ToString();
                drNew["Separator"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text.ToString();

                if (!string.IsNullOrEmpty(fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTLength].Text.Trim()))
                    drNew["Length"] = int.Parse(fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTLength].Text.ToString());

                drNew["FillString"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTFillString].Text.ToString();
                drNew["Alignment"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTAlignment].Text.ToString();

                if (fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTAllowNull].Text.ToUpper().Equals("TRUE"))
                    drNew["AllowNullFlag"] = true;
                else
                    drNew["AllowNullFlag"] = false;

                drNew["AllowDuplicationFlag"] = false;

                drNew["SortOrder"] = iSortOrder;

                dt.Rows.Add(drNew);
                iSortOrder++;
            }

            if (bSaveFlag)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.SaveSPPID_TagNoSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectID, mItemType, mType, dt, ref sMessage);
                }
            }
            else
            {
                sMessage = string.Format("{0} : 'Property' is empty", mType);
            }

            return bResult;
        }

        private void radButtonSave_Click(object sender, EventArgs e)
        {
            try
            {
                string sMessage = string.Empty;
                StringBuilder sbErrorMessage = new StringBuilder();

                bool bResult = SaveTagRule(ref sMessage);

                if (!bResult)
                {
                    sbErrorMessage.AppendLine(sMessage);
                }

                if (sMessage.Length > 0)
                    MessageBox.Show(sMessage.ToString());
                else
                    MessageBox.Show(this, Resources.Message10000, this.Text);
            }
            catch (System.ServiceModel.FaultException ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }
    }
}
