using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using EDCS.WF.DSL.Entities;
using FarPoint.Win;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using Configuration.Properties;

namespace Configuration
{
    public partial class UserControlCommonRole : UserControlBase
    {
        private enum enColumn { Check, RoleID, Name, NameView, VisibleFlag, DeleteFlag, Orders, Last };
        private string[] mColumnName = new string[] { "Check", "Role ID", "Role", "Description", "Visible", "Delete", "Orders" };
        private string[] mColumnName_Detail = new string[] { " ", "StandardRoleID", "Role", "Department", "Department Code" };
        private int[] mColumnWidth = new int[] { 50, 200, 190, 340, 100, 100, 100 };
        private int[] mColumnWidth_Detail = new int[] { 50, 0, 0, 250, 250 };
        private bool[] mColumnLock = new bool[] { false, true, false, false, false, false, false };
        private bool[] mColumnLock_Detail = new bool[] { false, true, false, false, false };

        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };
        SpreadUtil.eFarpointCellType[] mColumnType_Detail = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        private Dictionary<string, string> mdicProperty_Role = new Dictionary<string, string>();
        private enum LTColumn { Select, StandardRoleID, RoleID, Department, DepartmentCD, Last };

        public UserControlCommonRole()
        {
            InitializeComponent();
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadRole, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
            
        }
        public void InitSpreadSheetDetail()
        {
            SpreadUtil.InitSpread(fpSpreadRoleDetail, mColumnWidth_Detail, mColumnName_Detail, mColumnLock_Detail, null, null, mColumnType_Detail, null, null, Color.LightGray, true, true);
        }
        public override void LoadData()
        {
            setSpread();
            initPropertyArray();
            setSpread_Column(-1);
        }

        private void setSpread()
        {
            InitSpreadSheet();
            InitSpreadSheetDetail();

            fpSpreadRole.ActiveSheet.OperationMode = OperationMode.RowMode;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // Setting columns
                CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                fpSpreadRole.ActiveSheet.Columns[(int)enColumn.RoleID].Visible = false;
                fpSpreadRole.ActiveSheet.Columns[(int)enColumn.VisibleFlag].Visible = false;
                fpSpreadRole.ActiveSheet.Columns[(int)enColumn.DeleteFlag].Visible = false;
                fpSpreadRole.ActiveSheet.Columns[(int)enColumn.Orders].Visible = false;

                // Load to spread sheet 'Role Data'
                var lstRole = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                var vRole_inUsing = lstRole.Where(w => w.DeleteFlag == false);

                int iRowCount = 0;
                foreach (var role in vRole_inUsing)
                {
                    iRowCount = fpSpreadRole.ActiveSheet.Rows.Count++;

                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.RoleID].Value = role.RoleID;
                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.Name].Value = role.Name;
                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.Name].Tag = role.RoleID;
                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.NameView].Text = role.NameView;

                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.VisibleFlag].CellType = ctCheckBox;
                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.VisibleFlag].Value = role.VisibleFlag;

                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.DeleteFlag].CellType = ctCheckBox;
                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.DeleteFlag].Value = role.DeleteFlag;

                    fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.Orders].Text = role.Orders.ToString();
                }
            }
        }

        public override void SaveData()
        {
            try
            {
                string sMessage = string.Empty;
                StringBuilder sbErrorMessage = new StringBuilder();

                if (fpSpreadRole.ActiveSheet.RowCount > 0)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        var lstReadCommon_Role = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                        for (int iRow = 0; iRow < fpSpreadRole.ActiveSheet.RowCount; iRow++)
                        {
                            if (fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value != null)
                            {
                                // 수정/신규 row의 ControlName.Tag는 null로 마킹함.
                                if (fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Tag == null)
                                {
                                    // Check Exist/Null new 'Role name' 
                                    string sNewRoleName = fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value.ToString().Trim();

                                    if (sNewRoleName == string.Empty)
                                    {
                                        MessageBox.Show(string.Format(Resources.Message10011, "Role"), "Save Role Setting data", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                        return;
                                    }

                                    if (lstReadCommon_Role.Where(w => w.Name.ToUpper() == sNewRoleName.ToUpper()).Count() > 0)
                                    {
                                        fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value = string.Empty;
                                        MessageBox.Show(string.Format(Resources.Message10012, "Role"), "Save Role Setting data", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                        return;
                                    }
                                }
                            }
                            else
                            {
                                MessageBox.Show(string.Format(Resources.Message10011, "Role"), "Save Role Setting data", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                return;
                            }
                        }
                    }
                }

                bool bResult = SaveRoleSettings(ref sMessage);

                if (bResult == false)
                {
                    MessageBox.Show(sMessage.ToString());
                }
                else
                {
                    setSpread();
                    MessageBox.Show(Resources.Message10000, "Save Role Setting", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sMessage"></param>
        /// <returns></returns>
        public bool SaveRoleSettings(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;
            bool bSaveFlag = true;

            DataTable dtRoleSetting = new DataTable("Table1");
            dtRoleSetting.Columns.Add("RoleID", typeof(long));
            dtRoleSetting.Columns.Add("Name");
            dtRoleSetting.Columns.Add("NameView");
            dtRoleSetting.Columns.Add("VisibleFlag", typeof(bool));
            dtRoleSetting.Columns.Add("DeleteFlag", typeof(bool));
            dtRoleSetting.Columns.Add("Orders", typeof(int));

            for (int iRow = 0; iRow < fpSpreadRole.ActiveSheet.RowCount; iRow++)
            {
                // Role 저장시 필수 컬럼 체크
                string sRoleName = fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Text;

                if (string.IsNullOrEmpty(sRoleName))
                {
                    bSaveFlag = false;
                    break;
                }

                // Check new Role
                if (fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Text != null)
                {
                    DataRow drNew = dtRoleSetting.NewRow();

                    //기존 항목은 Update, 신규는 ID를 -1로 마킹해 Update시에 활용
                    drNew["RoleID"] = long.Parse(fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.RoleID].Value.ToString());
                    drNew["Name"] = fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.Name].Text.ToString();
                    drNew["NameView"] = fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.NameView].Text.ToString();

                    if (fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.VisibleFlag].Text.ToUpper().Equals("TRUE"))
                        drNew["VisibleFlag"] = true;
                    else
                        drNew["VisibleFlag"] = false;

                    if (fpSpreadRole.ActiveSheet.Cells[iRow, (int)enColumn.DeleteFlag].Text.ToUpper().Equals("TRUE"))
                        drNew["DeleteFlag"] = true;
                    else
                        drNew["DeleteFlag"] = false;

                    drNew["Orders"] = iRow + 1; //for 루프 내에 분기문이 발생하는 경우 Order count변수를 생성해야함.

                    dtRoleSetting.Rows.Add(drNew);
                }

            }//fpSpreadRole Row count


            if (bSaveFlag)
            {
                dtRoleSetting.TableName = "Table1";

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.UpdateCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE, dtRoleSetting, ref sMessage);
                }
            }
            else
            {
                sMessage = string.Format("{0} : Column 'Role' is empty");
            }

            return bResult;
        }

        public bool DeleteRoleSettings(List<long> lstSelectedRoleID, ref string sMessage)
        {
            bool bResult = false;

            DataTable dtRoleSetting = new DataTable("Table1");
            dtRoleSetting.Columns.Add("RoleID", typeof(long));
            dtRoleSetting.Columns.Add("Name");
            dtRoleSetting.Columns.Add("NameView");
            dtRoleSetting.Columns.Add("VisibleFlag", typeof(bool));
            dtRoleSetting.Columns.Add("DeleteFlag", typeof(bool));
            dtRoleSetting.Columns.Add("Orders", typeof(int));

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var lstRole = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                foreach (var roleAll in lstRole)
                {
                    foreach (long lRoleID in lstSelectedRoleID)
                    {
                        // DeleteFlag 컬럼만 True로 바꾸어 DB조회시 false인것만 출력하도록 함.
                        if (roleAll.RoleID == lRoleID)
                        {
                            DataRow drNew = dtRoleSetting.NewRow();
                            drNew["RoleID"] = roleAll.RoleID;
                            drNew["Name"] = roleAll.Name;
                            drNew["NameView"] = roleAll.NameView;
                            drNew["VisibleFlag"] = roleAll.VisibleFlag;
                            drNew["DeleteFlag"] = true;
                            drNew["Orders"] = roleAll.Orders;
                            dtRoleSetting.Rows.Add(drNew);
                        }
                    }

                }
                bResult = proxy.UpdateCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE, dtRoleSetting, ref sMessage);
            }

            return bResult;
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadRole.ActiveSheet;

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
            SheetView sheetView = fpSpreadRole.ActiveSheet;
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
            fpSpreadRole.ActiveSheet.Rows.Count++;
            CheckBoxCellType ctCheckBox = new CheckBoxCellType();
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.Check].Value = false;
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.RoleID].Value = -1;
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.Name].Tag = null;
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.VisibleFlag].CellType = ctCheckBox;
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.VisibleFlag].Value = true;
            fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.Rows.Count - 1, (int)enColumn.DeleteFlag].CellType = ctCheckBox;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            string sMessage = string.Empty;

            if (fpSpreadRole.ActiveSheet.RowCount > 0)
            {
                bool bCheckedRow = false;
                List<long> lstSelectedRoleID = new List<long>();

                for (int iRowCount = 0; iRowCount < fpSpreadRole.ActiveSheet.RowCount; iRowCount++)
                {
                    if (fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.Check].Text.ToUpper().Equals("TRUE"))
                    {
                        lstSelectedRoleID.Add(long.Parse(fpSpreadRole.ActiveSheet.Cells[iRowCount, (int)enColumn.RoleID].Value.ToString()));
                        bCheckedRow = true;
                    }
                }

                if (bCheckedRow == true)
                {
                    if (DeleteRoleSettings(lstSelectedRoleID, ref sMessage) == true)
                    {
                        setSpread();
                        MessageBox.Show(Resources.Message10000, "Delete Role setting data", MessageBoxButtons.OK);
                    }
                    else
                    {
                        MessageBox.Show(sMessage.ToString());
                    }
                }
                else
                {
                    MessageBox.Show(Resources.Message10008, "Delete Role setting data", MessageBoxButtons.OK);
                }
            }
        }

        private void fpSpreadRole_Change(object sender, ChangeEventArgs e)
        {
            if (e.Column == (int)enColumn.Name)
                fpSpreadRole.ActiveSheet.Cells[e.Row, (int)enColumn.Name].Tag = null;
        }

        private void fpSpreadRole_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)enColumn.Check)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadRole.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 0)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadRole.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadRole.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadRole.ActiveSheet;

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

            if (fpSpreadRole.ActiveSheet.Cells[e.Row, (int)enColumn.Name].Value != null)
            {
                setSpread_Column(long.Parse(fpSpreadRole.ActiveSheet.Cells[e.Row, (int)enColumn.RoleID].Value.ToString()));
            }
        }
        private void initPropertyArray()
        {
            mdicProperty_Role.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var lstCommon_Role = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                foreach (var item in lstCommon_Role)
                {
                    if (!mdicProperty_Role.ContainsKey(item.RoleID.ToString()))
                        mdicProperty_Role.Add(item.RoleID.ToString(), item.Name);
                }

                lstCommon_Role.Clear();
            }
        }
        private void setSpread_Column(long lSelectedRoleID)
        {
            if (lSelectedRoleID != -1)
                InitSpreadSheetDetail();

            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadRoleDetail.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();
                cbtColumn.Editable = true;
                arItems = null;
                arItemsData = null;
                int iCellCount = 0;

                switch (i)
                {
                    case (int)LTColumn.Select:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();

                        fpSpreadRoleDetail.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadRoleDetail.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.RoleID:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty_Role.Count];
                        arItemsData = new string[mdicProperty_Role.Count];
                        iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty_Role)
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                }

                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.Editable = false;
                    cbtColumn.DropDownWhenStartEditing = false;
                    cbtColumn.DropDownOptions = DropDownOptions.Button;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadRoleDetail.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntityCommon_StandardRole> lstCommon_StandardRole = proxy.ReadStandardRoleDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.RoleID == lSelectedRoleID).ToList();
                foreach (var item in lstCommon_StandardRole)
                {
                    int iRow = fpSpreadRoleDetail.ActiveSheet.Rows.Count++;
                    fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.StandardRoleID].Value = item.StandardRoleID;
                    fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.RoleID].Value = item.RoleID.ToString();
                    fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.Department].Value = item.DepartmentName;
                    fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.DepartmentCD].Value = item.DepartmentCode;
                }
            }

        }

        private void radButtonCreateDetail_Click(object sender, EventArgs e)
        {
            if (fpSpreadRoleDetail.ActiveSheet != null)
            {
                int iRow = fpSpreadRoleDetail.ActiveSheet.Rows.Count++;
                fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.StandardRoleID].Value = -1;
            }
        }

        private void radButtonDeleteDetail_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(Resources.Message10003, "Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    for (int i = fpSpreadRoleDetail.ActiveSheet.Rows.Count - 1; i >= 0; i--)
                    {
                        string sCheck = fpSpreadRoleDetail.ActiveSheet.Cells[i, (int)LTColumn.Select].Text;
                        long lStandardRoleID = long.Parse(fpSpreadRoleDetail.ActiveSheet.Cells[i, (int)LTColumn.StandardRoleID].Text);

                        if (sCheck.ToUpper().Equals("TRUE") && lStandardRoleID == -1)
                        {
                            fpSpreadRoleDetail.ActiveSheet.Rows.Remove(i, 1);
                        }
                        else if (sCheck.ToUpper().Equals("TRUE") && lStandardRoleID > -1)
                        {
                            if (proxy.DeleteCommon_StandardRole(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lStandardRoleID))
                            {
                                fpSpreadRoleDetail.ActiveSheet.Rows.Remove(i, 1);
                            }
                        }
                    }
                    // 재조회
                    setSpread_Column(long.Parse(fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.ActiveRowIndex, (int)enColumn.RoleID].Value.ToString()));
                }
            }
        }
        private void ClickRoleHead(CellClickEventArgs e)
        {
            HitTestInformation hti;
            HeaderHitTestInformation hi;
            hti = fpSpreadRoleDetail.HitTest(e.X, e.Y);
            hi = hti.HeaderInfo;

            if (hti.Type == HitTestType.ColumnHeader)
            {
                if (hi.Column == 0)
                {
                    bool check = !Convert.ToBoolean(fpSpreadRoleDetail.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                    fpSpreadRoleDetail.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                    SheetView sheetView = fpSpreadRoleDetail.ActiveSheet;

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
        private void ClickRoleRow(int iRow, int iColumn)
        {
            if (fpSpreadRoleDetail.ActiveSheet != null)
            {
                string sType = string.Empty;

                if (iColumn == (int)LTColumn.DepartmentCD || iColumn == (int)LTColumn.Department)
                {
                    sType = CodeListType.DEPARTMENT;
                }

                if (!string.IsNullOrEmpty(sType))
                {
                    FrmSearchCodeList frm = new FrmSearchCodeList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sType);
                    if (frm.ShowDialog() == DialogResult.OK)
                    {
                        fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.DepartmentCD].Value = frm.CodeID;
                        fpSpreadRoleDetail.ActiveSheet.Cells[iRow, (int)LTColumn.Department].Value = frm.CodeName;
                    }
                }
            }
        }

        private void fpSpreadRoleDetail_CellClick(object sender, CellClickEventArgs e)
        {
            ClickRoleHead(e);
            ClickRoleRow(e.Row, e.Column);
        }
        public void SaveRoleDetail(long lSelectedRoleID )
        {
            try
            {
                StringBuilder sbMessage = new StringBuilder();

                DataTable dt = new DataTable("Table1");
                dt.Columns.Add("StandardRoleID", typeof(long));
                dt.Columns.Add("RoleID", typeof(long));
                dt.Columns.Add("DepartmentCode");
                dt.Columns.Add("DepartmentName");

                bool bSaveFlag = true;

                string sRole = string.Empty;
                string sDepartment = string.Empty;
                int iEmptyRow = 0;

                for (int i = 0; i < fpSpreadRoleDetail.ActiveSheet.RowCount; i++)
                {
                    iEmptyRow = i;
                    sRole = string.Empty;
                    sDepartment = string.Empty;

                    sRole = lSelectedRoleID.ToString();
                    sDepartment = fpSpreadRoleDetail.ActiveSheet.Cells[i, (int)LTColumn.DepartmentCD].Text;

                    if (string.IsNullOrEmpty(sRole) || string.IsNullOrEmpty(sDepartment))
                    {
                        continue;
                    }
                    else if (string.IsNullOrEmpty(sRole))
                    {
                        bSaveFlag = false;
                        break;
                    }
                    else if (string.IsNullOrEmpty(sDepartment))
                    {
                        bSaveFlag = false;
                        break;
                    }

                    DataRow drNew = dt.NewRow();
                    drNew["RoleID"] = lSelectedRoleID;
                    drNew["DepartmentCode"] = fpSpreadRoleDetail.ActiveSheet.Cells[i, (int)LTColumn.DepartmentCD].Value.ToString();
                    drNew["DepartmentName"] = fpSpreadRoleDetail.ActiveSheet.Cells[i, (int)LTColumn.Department].Value.ToString();
                    dt.Rows.Add(drNew);
                }

                if (bSaveFlag)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        //저장
                        string sMessage = string.Empty;
                        if (proxy.SavStandardRole(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lSelectedRoleID, dt, ref sMessage))
                        {
                            sbMessage.AppendLine("Saved successfully!");
                        }
                        else
                        {
                            sbMessage.AppendLine(sMessage);
                        }
                    }
                }
                else
                {
                    if (string.IsNullOrEmpty(sRole))
                        sbMessage.AppendLine(string.Format("{0} Row : 'Role' is empty", iEmptyRow.ToString()));

                    if (string.IsNullOrEmpty(sDepartment))
                        sbMessage.AppendLine(string.Format("{0} Row : 'Department' is empty", iEmptyRow.ToString()));
                }

                // 재조회
                setSpread_Column(-1);

                MessageBox.Show(sbMessage.ToString());
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

        private void kRadButtonSaveRoleDetail_Click(object sender, EventArgs e)
        {
            if (fpSpreadRoleDetail.ActiveSheet.RowCount != 0)
            {
                SaveRoleDetail(long.Parse(fpSpreadRole.ActiveSheet.Cells[fpSpreadRole.ActiveSheet.ActiveRowIndex, (int)enColumn.RoleID].Value.ToString()));
            }
        }


    }
}
