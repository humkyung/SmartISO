using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using Configuration.Properties;

namespace Configuration
{
    public partial class UserControlSPPIDDataFilterSetting : UserControlBase
    {
        private enum enColumn { Check, DefaultAttributeFlag, ColumnName, Last };
        private string[] mColumnName = new string[] { "Check", "Default", "Attribute Name" };
        private int[] mColumnWidth = new int[] { 50, 50, 700 };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text };

        public const int CONST_ALLOWEMPTYROW = 3;
        private const int CONST_ROWMAXCOUNT = 5000;

        public UserControlSPPIDDataFilterSetting()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitDropdownListItemType(string.Empty);
            this.comboTreeBoxItemType.SelectedChanged += new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);

            setSpread();
        }

        private void InitDropdownListItemType(string sItemType)
        {
            GeneralUtil.SetComboTreeBoxItemType(ref comboTreeBoxItemType);

            if (!string.IsNullOrEmpty(sItemType))
                comboTreeBoxItemType.Text = sItemType;
            else
                comboTreeBoxItemType.Text = Information.DRAWING;
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadDataFilter, mColumnWidth, mColumnName, null, null, null, mColumnType, null, null, Color.LightGray, true, true);

            fpSpreadDataFilter.ActiveSheet.Columns[0].Visible = false;

        }

        // Load 'Attribute List' from dbo.SPPID_Default_DataFilter 
        private void setSpread()
        {
            InitSpreadSheet();
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                fpSpreadDataFilter.AllowRowMove = true;

                if (comboTreeBoxItemType.Text != null)
                {
                    // Load to spread sheet 'Default_DataFilter Data'
                    List<EntitySPPID_Default_DataFilter> lstDataFilter = proxy.ReadSPPID_Default_DataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, comboTreeBoxItemType.Text);
                    int iRowCount = 0;

                    fpSpreadDataFilter.ActiveSheet.Rows.Count = CONST_ROWMAXCOUNT;
                    fpSpreadDataFilter.ActiveSheet.Columns[(int)enColumn.ColumnName].HorizontalAlignment = CellHorizontalAlignment.Left;

                    if (lstDataFilter.Count > 0)
                    {
                        foreach (EntitySPPID_Default_DataFilter DataFilter in lstDataFilter)
                        {
                            fpSpreadDataFilter.ActiveSheet.Cells[iRowCount, (int)enColumn.DefaultAttributeFlag].Value = DataFilter.DefaultAttributeFlag;

                            fpSpreadDataFilter.ActiveSheet.Cells[iRowCount, (int)enColumn.ColumnName].Value = DataFilter.ColumnName;
                            fpSpreadDataFilter.ActiveSheet.Cells[iRowCount, (int)enColumn.ColumnName].Tag = DataFilter.DataFilterID;

                            iRowCount++;
                        }
                    }

                    SetSpreadCount();
                }
            }
        }

        private void SetSpreadCount()
        {
            int iAttCount = 0;
            for (int iRow = 0; iRow < fpSpreadDataFilter.ActiveSheet.Rows.Count; iRow++)
            {
                if (fpSpreadDataFilter.ActiveSheet.Cells[iRow, (int)enColumn.ColumnName].Value != null)
                    iAttCount++;
            }
            radLabelSelectedDataFilterCount.Text = string.Format("Row Count : {0}", iAttCount);
        }

        public override void SaveData()
        {

            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    string sMessage = string.Empty;
                    bool bSaveFlag = true;
                    List<string> lstAttName = new List<string>();

                    DataTable dtDefault_DataFilter = new DataTable("Table1");
                    dtDefault_DataFilter.Columns.Add("DataFilterID");
                    dtDefault_DataFilter.Columns.Add("ItemType");
                    // DefaultAttributeFlag 로 설정된 데이터는 기본 속성으로 지정 할때 기본 어트리뷰트 속성으로 된다.
                    dtDefault_DataFilter.Columns.Add("DefaultAttributeFlag", typeof(bool));
                    dtDefault_DataFilter.Columns.Add("ColumnName");
                    dtDefault_DataFilter.Columns.Add("ColumnOrder", typeof(int));

                    if (fpSpreadDataFilter.ActiveSheet.RowCount > 0)
                    {
                        int iOrderNo = 1;
                        for (int iRow = 0; iRow < fpSpreadDataFilter.ActiveSheet.Rows.Count; iRow++)
                        {
                            string sNewColumnName = Util.NullToSpace(fpSpreadDataFilter.ActiveSheet.Cells[iRow, (int)enColumn.ColumnName].Value);

                            string sDefaultAttributeFlag = Util.NullToSpace(fpSpreadDataFilter.ActiveSheet.Cells[iRow, (int)enColumn.DefaultAttributeFlag].Value);

                            if (string.IsNullOrEmpty(sNewColumnName))
                            {
                                continue;
                            }

                            if (!lstAttName.Contains(sNewColumnName))
                            {
                                lstAttName.Add(sNewColumnName);
                                DataRow drNew = dtDefault_DataFilter.NewRow();
                                drNew["ItemType"] = comboTreeBoxItemType.Text;


                                if (sDefaultAttributeFlag.ToUpper().Equals("TRUE"))
                                {
                                    drNew["DefaultAttributeFlag"] = true;
                                }
                                else
                                {
                                    drNew["DefaultAttributeFlag"] = false;
                                }


                                drNew["ColumnName"] = sNewColumnName;
                                drNew["ColumnOrder"] = iOrderNo++;
                                dtDefault_DataFilter.Rows.Add(drNew);
                            }
                            else
                            {
                                continue;
                            }
                        }

                        if (bSaveFlag == true)
                        {
                            bool bResult = proxy.SaveSPPID_Default_DataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, dtDefault_DataFilter, comboTreeBoxItemType.Text, ref sMessage);

                            if (bResult == false)
                            {
                                MessageBox.Show(sMessage.ToString());
                            }
                            else
                            {
                                MessageBox.Show(Resources.Message10000, "Save Default DataFilter", MessageBoxButtons.OK, MessageBoxIcon.Information);
                            }
                        }
                    }

                    lstAttName.Clear();
                }

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

        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            setSpread();
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadDataFilter.ActiveSheet;

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
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadDataFilter.ActiveSheet;
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
        }

        private void fpSpreadDataFilter_Change(object sender, ChangeEventArgs e)
        {
            SetSpreadCount();
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (fpSpreadDataFilter.ActiveSheet != null)
                    {
                        for (int i = fpSpreadDataFilter.ActiveSheet.Rows.Count - 1; i >= 0; i--)
                        {
                            string sCheck = fpSpreadDataFilter.ActiveSheet.Cells[i, (int)enColumn.Check].Text;

                            if (sCheck.ToUpper().Equals("TRUE"))
                            {
                                if (fpSpreadDataFilter.ActiveSheet.Cells[i, (int)enColumn.ColumnName].Tag != null)
                                {
                                    long lDataFilterID = long.Parse(fpSpreadDataFilter.ActiveSheet.Cells[i, (int)enColumn.ColumnName].Tag.ToString());

                                    string sError = string.Empty;
                                    proxy.DeleteSPPID_Default_DataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lDataFilterID, ref sError);
                                }

                                fpSpreadDataFilter.ActiveSheet.Rows.Remove(i, 1);
                            }
                        }
                    }
                }
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

        private void fpSpreadDataFilter_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)enColumn.Check)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadDataFilter.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 0)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadDataFilter.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadDataFilter.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadDataFilter.ActiveSheet;

                        if (sheetView != null)
                        {
                            for (int i = 0; i < sheetView.Rows.Count; i++)
                            {
                                if (sheetView.Cells[i, 2].Value != null)
                                    sheetView.Cells[i, 0].Value = check;
                            }
                        }
                    }
                }
            }

            if (e.Column == (int)enColumn.DefaultAttributeFlag)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadDataFilter.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 1)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadDataFilter.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadDataFilter.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadDataFilter.ActiveSheet;

                        if (sheetView != null)
                        {
                            for (int i = 0; i < sheetView.Rows.Count; i++)
                            {
                                if (sheetView.Cells[i, 2].Value != null)
                                    sheetView.Cells[i, 1].Value = check;
                            }
                        }
                    }
                }
            }

        }
    }
}
