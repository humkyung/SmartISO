using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using EDCS.WF.BSL;
using Telerik.WinControls.UI.Data;
using Configuration.Properties;

namespace Configuration
{
    public partial class UserControlCommonMenu : UserControlBase
    {
        private enum enColumn { Check, MenuID, ProgramName, Name, NameView, Description, Orders, Last };
        private string[] mColumnName = new string[] { "Check", "Menu ID", "ProgramName", "Name", "Menu", "Menu Name", "Orders" };
        private int[] mColumnWidth = new int[] { 50, 100, 250, 300, 300, 400, 0 };
        private bool[] mColumnLock = new bool[] { false, true, false, false, false, false };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        public UserControlCommonMenu()
        {
            InitializeComponent();

        }

        public override void LoadData()
        {
            InitDropDownControl();
            InitSpreadSheet();
            setSpread();
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadMenu, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);

            //// Setting columns
            //fpSpreadMenu.ActiveSheet.Columns[(int)enColumn.MenuID].Visible = false;
            fpSpreadMenu.ActiveSheet.Columns[(int)enColumn.Orders].Visible = false;
        }

        private void InitDropDownControl()
        {
            GeneralUtil.SetDropdownListSystemInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListSystemList);
        }

        private void setSpread()
        {
            // Load to spread sheet 'Menu Data'

            string[] arItemsData = { };
            string[] arItems = { };

            //if (mdicMenu.Count > 0)
            //{
            //    for (int i = 0; i < fpSpreadMenu.ActiveSheet.Columns.Count; i++)
            //    {
            //        ComboBoxCellType cbtColumn = new ComboBoxCellType();
            //        cbtColumn.Editable = true;
            //        arItems = null;
            //        arItemsData = null;
            //        int iCellCount = 0;

            //        switch (i)
            //        {
            //            case (int)enColumn.NameView:
            //                //arItems = m_arProperty;

            //                arItems = new string[mdicMenu.Count];
            //                arItemsData = new string[mdicMenu.Count];
            //                iCellCount = 0;
            //                foreach (KeyValuePair<string, string> pair in mdicMenu)
            //                {
            //                    arItems[iCellCount] = pair.Value;
            //                    arItemsData[iCellCount] = pair.Key;
            //                    iCellCount++;
            //                }
            //                cbtColumn.MaxDrop = 18;

            //                break;
            //        }

            //        if (arItems != null)
            //        {
            //            cbtColumn.Items = arItems;
            //            cbtColumn.ItemData = arItemsData;
            //            cbtColumn.EditorValue = EditorValue.ItemData;
            //            fpSpreadMenu.ActiveSheet.Columns[i].CellType = cbtColumn;
            //        }
            //    }
            //}

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntityCommon_Menu> lstMenu = proxy.ReadCommon_Menu(EDCS.WF.BSL.Variable.SharedVariable.IsIE, long.Parse(radDropDownListSystemList.SelectedItem.Value.ToString()));

                int iRowCount = 0;
                foreach (EntityCommon_Menu menu in lstMenu)
                {
                    iRowCount = fpSpreadMenu.ActiveSheet.Rows.Count++;
                    fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.MenuID].Value = menu.MenuID;
                    fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.ProgramName].Value = menu.ProgramName;
                    fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.Name].Value = menu.Name;
                    fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.NameView].Value = menu.NameView;
                    fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.Description].Value = menu.Description;
                }
            }
        }

        private void radDropDownListSystemList_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            if (radDropDownListSystemList.SelectedItem != null)
            {
                long lSystemID = long.Parse(radDropDownListSystemList.SelectedItem.Value.ToString());
                //mdicMenu = CommonMethod.GetMenu(lSystemID);

                InitSpreadSheet();
                setSpread();
            }
        }

        public override void SaveData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    string sMessage = string.Empty;

                    if (!SaveMenuSettings(ref sMessage))
                    {
                        MessageBox.Show(sMessage.ToString());
                    }
                    else
                    {
                        InitSpreadSheet();
                        setSpread();
                        MessageBox.Show(Resources.Message10000, "Save Menu", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        public bool SaveMenuSettings(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;

            DataTable dtMenuSetting = new DataTable("Table1");
            dtMenuSetting.Columns.Add("MenuID", typeof(long));
            dtMenuSetting.Columns.Add("SystemID", typeof(long));
            dtMenuSetting.Columns.Add("ProgramName");
            dtMenuSetting.Columns.Add("Name");
            dtMenuSetting.Columns.Add("NameView");
            dtMenuSetting.Columns.Add("Description");
            dtMenuSetting.Columns.Add("Orders", typeof(int));

            long lSystemID = long.Parse(radDropDownListSystemList.SelectedItem.Value.ToString());

            int iRowCount = 1;

            List<string> lstCheckValue = new List<string>();

            bool bNullCheck = false;
            bool bDistinct = false;

            string sProgramName = string.Empty;
            int iNull = 0;
            for (int iRow = 0; iRow < fpSpreadMenu.ActiveSheet.RowCount; iRow++)
            {
                // Menu 저장시 필수 컬럼 체크
                sProgramName = Util.NullToSpace(fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.ProgramName].Text);

                if (string.IsNullOrEmpty(sProgramName))
                {
                    iNull = iRow + 1;
                    bNullCheck = true;
                    break;
                }

                if (lstCheckValue.Contains(sProgramName))
                {
                    bDistinct = true;
                    break;
                }

                lstCheckValue.Add(sProgramName);

                DataRow drNew = dtMenuSetting.NewRow();
                //기존 항목은 Update, 신규는 ID를 -1로 마킹해 Update시에 활용
                drNew["MenuID"] = long.Parse(fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.MenuID].Value.ToString());
                drNew["SystemID"] = lSystemID;
                drNew["ProgramName"] = fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.ProgramName].Value.ToString().Trim();
                drNew["Name"] = fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value.ToString().Trim();
                drNew["NameView"] = fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.NameView].Text.ToString().Trim();
                drNew["Description"] = fpSpreadMenu.ActiveSheet.Cells[iRow, (int)enColumn.Description].Text.ToString().Trim();
                drNew["Orders"] = iRowCount; //for 루프 내에 분기문이 발생하는 경우 Orders count에 주의할것
                dtMenuSetting.Rows.Add(drNew);
                iRowCount++;

            }//fpSpreadMenu Row count

            if (bNullCheck)
            {
                dtMenuSetting.Clear();
                sMessage = string.Format("'{0}' item is empty!", iNull.ToString());
            }
            else if (bDistinct)
            {
                dtMenuSetting.Clear();
                sMessage = string.Format("'{0}' is duplicated!", sProgramName);
            }
            else
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.UpdateCommon_Menu(EDCS.WF.BSL.Variable.SharedVariable.IsIE, dtMenuSetting, ref sMessage);
                }
            }

            return bResult;
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadMenu.ActiveSheet;

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
            SheetView sheetView = fpSpreadMenu.ActiveSheet;
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

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadMenu.ActiveSheet.Rows.Count++;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.Check].Value = false;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.MenuID].Value = -1;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.ProgramName].Value = string.Empty;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.Name].Value = string.Empty;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.NameView].Value = string.Empty;
            fpSpreadMenu.ActiveSheet.Cells[fpSpreadMenu.ActiveSheet.Rows.Count - 1, (int)enColumn.Description].Value = string.Empty;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (fpSpreadMenu.ActiveSheet.RowCount > 0)
                {
                    if (fpSpreadMenu.ActiveSheet != null)
                    {
                        bool bCheckedRow = false;
                        List<long> lstSelectedMenuID = new List<long>();

                        // Count checked column
                        for (int iRowCount = 0; iRowCount < fpSpreadMenu.ActiveSheet.RowCount; iRowCount++)
                        {
                            if (fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.Check].Text.ToUpper().Equals("TRUE"))
                            {
                                lstSelectedMenuID.Add(long.Parse(fpSpreadMenu.ActiveSheet.Cells[iRowCount, (int)enColumn.MenuID].Value.ToString()));
                                bCheckedRow = true;
                            }
                        }

                        if (bCheckedRow == true)
                        {
                            if (DeleteData(lstSelectedMenuID) == true)
                            {
                                InitSpreadSheet();
                                setSpread();
                                MessageBox.Show(Resources.Message10000, "Delete Menu", MessageBoxButtons.OK);
                            }
                        }
                        else
                        {
                            MessageBox.Show(Resources.Message10008, "Delete Menu", MessageBoxButtons.OK);
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

        public bool DeleteData(List<long> lstSelectedMenuID)
        {
            string sMessage = string.Empty;
            bool bSaveFlag = true;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                foreach (long lMenuID in lstSelectedMenuID)
                {
                    bSaveFlag = proxy.DeleteCommon_Menu(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lMenuID, ref sMessage);

                    if (bSaveFlag == false)
                        MessageBox.Show(sMessage.ToString());
                }
            }

            return bSaveFlag;
        }

        private void fpSpreadMenu_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)enColumn.Check)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadMenu.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 0)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadMenu.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadMenu.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadMenu.ActiveSheet;

                        if (sheetView != null)
                        {
                            for (int i = 0; i < sheetView.Rows.Count; i++)
                            {
                                sheetView.Cells[i, 0].Value = check;
                            }
                        }
                    }
                }
            }
        }
    }
}
