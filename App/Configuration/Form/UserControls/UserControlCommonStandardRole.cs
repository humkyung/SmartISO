using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;

using Configuration.Properties;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlCommonStandardRole : UserControlBase
    {
        private enum LTColumn { Select, StandardRoleID, RoleID, Department, DepartmentCD, Last };
        private string[] mColumnName = new string[] { " ", "StandardRoleID", "Role", "Department", "Department Code" };
        private int[] marColumnWidths = new int[] { 60, 0, 300, 300, 300 };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        private Dictionary<string, string> mdicProperty_Role = new Dictionary<string, string>();

        public UserControlCommonStandardRole()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitSpread();
            initPropertyArray();
            setSpread_Column();
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, mColumnType, null, null, Color.LightGray, true, true);
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

        private void setSpread_Column()
        {
            InitSpread();

            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
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

                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
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
                    fpSpreadData.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntityCommon_StandardRole> lstCommon_StandardRole = proxy.ReadStandardRoleDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                foreach (var item in lstCommon_StandardRole)
                {
                    int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.StandardRoleID].Value = item.StandardRoleID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.RoleID].Value = item.RoleID.ToString();
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.Department].Value = item.DepartmentName;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.DepartmentCD].Value = item.DepartmentCode;
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            if (fpSpreadData.ActiveSheet != null)
            {
                int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
                fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.StandardRoleID].Value = -1;
            }
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(Resources.Message10003, "Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    for (int i = fpSpreadData.ActiveSheet.Rows.Count - 1; i >= 0; i--)
                    {
                        string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.Select].Text;
                        long lStandardRoleID = long.Parse(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.StandardRoleID].Text);

                        if (sCheck.ToUpper().Equals("TRUE") && lStandardRoleID == -1)
                        {
                            fpSpreadData.ActiveSheet.Rows.Remove(i, 1);
                        }
                        else if (sCheck.ToUpper().Equals("TRUE") && lStandardRoleID > -1)
                        {
                            if (proxy.DeleteCommon_StandardRole(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lStandardRoleID))
                            {
                                fpSpreadData.ActiveSheet.Rows.Remove(i, 1);
                            }
                        }
                    }
                    // 재조회
                    setSpread_Column();
                }
            }
        }

        private void ClickRoleHead(CellClickEventArgs e)
        {
            HitTestInformation hti;
            HeaderHitTestInformation hi;
            hti = fpSpreadData.HitTest(e.X, e.Y);
            hi = hti.HeaderInfo;

            if (hti.Type == HitTestType.ColumnHeader)
            {
                if (hi.Column == 0)
                {
                    bool check = !Convert.ToBoolean(fpSpreadData.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                    fpSpreadData.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                    SheetView sheetView = fpSpreadData.ActiveSheet;

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
            if (fpSpreadData.ActiveSheet != null)
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
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.DepartmentCD].Value = frm.CodeID;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)LTColumn.Department].Value = frm.CodeName;
                    }
                }
            }
        }

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            ClickRoleHead(e);
            ClickRoleRow(e.Row, e.Column);
        }

        public override void SaveData()
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

                for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
                {
                    iEmptyRow = i;
                    sRole = string.Empty;
                    sDepartment = string.Empty;

                    sRole = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.RoleID].Text;
                    sDepartment = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.DepartmentCD].Text;

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
                    drNew["RoleID"] = long.Parse(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.RoleID].Value.ToString());
                    drNew["DepartmentCode"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.DepartmentCD].Value.ToString();
                    drNew["DepartmentName"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.Department].Value.ToString();
                    dt.Rows.Add(drNew);
                }

                if (bSaveFlag)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        // 저장
                        string sMessage = string.Empty;
                        if (proxy.SavStandardRole(EDCS.WF.BSL.Variable.SharedVariable.IsIE, -1, dt, ref sMessage))
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
                setSpread_Column();

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
    }
}
