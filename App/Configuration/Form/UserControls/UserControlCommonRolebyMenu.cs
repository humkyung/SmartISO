using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI.Data;
using Configuration.Properties;

namespace Configuration
{
    public partial class UserControlCommonRolebyMenu : UserControlBase
    {
        //private enum enColumn { Check, RoleID, Name, NameView, VisibleFlag, DeleteFlag, Orders, Edit, Last };
        //private int[] mColumnWidth = new int[] { 60, 0, 120, 150, 80, 0, 0, 75 };
        //private string[] mColumnName = new string[] { " ", "RoleID", "Role", "Description", "Visible", "DeleteFlag", "Orders", "EDIT" };
        //private bool[] mColumnLock = new bool[] { false, true, true, true, true, true, true, false };
        //SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.NormalButton };

        private enum enColumn { SystemID, Name, NameView, Last };
        private int[] mColumnWidth = new int[] { 0, 180, 0 };
        private string[] mColumnName = new string[] { "SystemID", "Name", "NameView" };
        private bool[] mColumnLock = new bool[] { true, true, true };

        private enum enColumn_Menu { MenuID, SystemID, ControlName, Orders, Name, NameView, Description, Last };
        private int[] mColumnWidth_Menu = new int[] { 0, 0, 0, 80, 250, 0, 0 };
        private string[] mColumnName_Menu = new string[] { "MenuID", "SystemID", "ControlName", "Orders", "Menu Name", "Menu NameView", "Description", };
        private bool[] mColumnLock_Menu = new bool[] { true, true, true, true, true, true, true };

        private long mlSystemID = -1;

        public UserControlCommonRolebyMenu()
        {
            InitializeComponent();
        }
        public override void LoadData()
        {
            GeneralUtil.SetDropdownListRoleInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListRole);
            InitSpread_System();
            if (radDropDownListRole.SelectedItem != null && radDropDownListRole.SelectedItem.Value != null)
            {
                SetSpread_System();
            }
        }

        private void InitSpread_System()
        {
            SpreadUtil.InitSpread(fpSpreadSystem, mColumnWidth, mColumnName, mColumnLock, null, null, null
               , null, null, Color.LightGray, true, true);
        }

        private void InitSpread_Menu()
        {
            SpreadUtil.InitSpread(fpSpreadNotAvailableMenu, mColumnWidth_Menu, mColumnName_Menu, mColumnLock_Menu, null, null, null, null, null, Color.LightGray, true, true);

            SpreadUtil.InitSpread(fpSpreadAvailableMenu, mColumnWidth_Menu, mColumnName_Menu, mColumnLock_Menu, null, null, null, null, null, Color.LightGray, true, true);

            fpSpreadNotAvailableMenu.ActiveSheet.Columns[(int)enColumn_Menu.Orders].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadAvailableMenu.ActiveSheet.Columns[(int)enColumn_Menu.Orders].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        private void SetSpread_System()
        {
            InitSpread_System();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var lstReadCommon_System = proxy.ReadCommon_System(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                int iRowCount = 0;

                foreach (var role in lstReadCommon_System)
                {
                    iRowCount++;
                    fpSpreadSystem.ActiveSheet.Rows.Count = iRowCount;

                    fpSpreadSystem.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.SystemID].Value = role.SystemID;
                    fpSpreadSystem.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Name].Value = role.Name;
                    fpSpreadSystem.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.NameView].Value = role.NameView;
                }

                if (fpSpreadSystem.ActiveSheet.Rows.Count > 0)
                {
                    mlSystemID = long.Parse(fpSpreadSystem.ActiveSheet.Cells[0, (int)enColumn.SystemID].Value.ToString());

                    radLabelSystem.Text = string.Format("System : {0}", fpSpreadSystem.ActiveSheet.Cells[0, (int)enColumn.Name].Text);
                }
                else
                {
                    radLabelSystem.Text = "System";
                }

                if (radDropDownListRole.SelectedItem != null && radDropDownListRole.SelectedItem.Value != null)
                {
                    long lRoleID = long.Parse(radDropDownListRole.SelectedItem.Value.ToString());
                    SetSpread_Menu(mlSystemID, lRoleID);
                }
            }
        }

        private void SetSpread_Menu(long lSystemID, long lRoleID)
        {
            InitSpread_Menu();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var vmenu = proxy.ReadCommon_Menu(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lSystemID);

                var vmenu_Use = proxy.ReadCommon_RoleMenuRel(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lSystemID, lRoleID);

                int iRowCount_NotUse = 0;
                int iRowCount_Use = 0;
                foreach (var item in vmenu)
                {
                    if (vmenu_Use.Where(w => w.MenuID == item.MenuID).Count() == 0)
                    {
                        iRowCount_NotUse++;
                        fpSpreadNotAvailableMenu.ActiveSheet.Rows.Count = iRowCount_NotUse;

                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.MenuID].Text = Util.NullToSpace(item.MenuID);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.SystemID].Text = Util.NullToSpace(item.SystemID);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.ControlName].Text = Util.NullToSpace(item.ProgramName);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.Orders].Text = Util.NullToSpace(item.Orders);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.Name].Text = Util.NullToSpace(item.Name);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.NameView].Text = Util.NullToSpace(item.NameView);
                        fpSpreadNotAvailableMenu.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.Description].Text = Util.NullToSpace(item.Description);
                    }
                    else
                    {
                        iRowCount_Use++;
                        fpSpreadAvailableMenu.ActiveSheet.Rows.Count = iRowCount_Use;

                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.MenuID].Text = Util.NullToSpace(item.MenuID);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.SystemID].Text = Util.NullToSpace(item.SystemID);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.ControlName].Text = Util.NullToSpace(item.ProgramName);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.Orders].Text = Util.NullToSpace(item.Orders);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.Name].Text = Util.NullToSpace(item.Name);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.NameView].Text = Util.NullToSpace(item.NameView);
                        fpSpreadAvailableMenu.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.Description].Text = Util.NullToSpace(item.Description);
                    }
                }
            }
        }

        private void radDropDownListSystemList_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            if (radDropDownListRole.SelectedItem != null && radDropDownListRole.SelectedItem.Value != null)
            {
                long lRoleID = long.Parse(radDropDownListRole.SelectedItem.Value.ToString());
                SetSpread_Menu(mlSystemID, lRoleID);
            }
        }

        private void radButtonAttributeAddAll_Click(object sender, EventArgs e)
        {
            AddAllItem(fpSpreadNotAvailableMenu, fpSpreadAvailableMenu);
        }

        private void radButtonAttributeAdd_Click(object sender, EventArgs e)
        {
            AddItem(fpSpreadNotAvailableMenu, fpSpreadAvailableMenu);
        }

        private void radButtonAttributeRemove_Click(object sender, EventArgs e)
        {
            DeleteItem(fpSpreadNotAvailableMenu, fpSpreadAvailableMenu);
        }

        private void radButtonAttributeRemoveAll_Click(object sender, EventArgs e)
        {
            DeleteAllItem(fpSpreadNotAvailableMenu, fpSpreadAvailableMenu);
        }

        private void AddAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_NotUse.ActiveSheet != null)
            {
                int iRowCount = FpSpread_NotUse.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }

                for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }

            }
        }

        private void AddItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_NotUse.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_NotUse.ActiveSheet.ActiveCell != null)
            {
                FpSpread_Use.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                {
                    FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                    i--;
                }
            }

            FpSpread_Use.ActiveSheet.SetActiveCell(FpSpread_Use.ActiveSheet.Rows.Count - 1, (int)enColumn_Menu.NameView, true);
            FpSpread_Use.ShowRow(0, FpSpread_Use.ActiveSheet.Rows.Count, VerticalPosition.Bottom);

        }

        private void DeleteItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_Use.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_NotUse.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_Use.ActiveSheet.ActiveCell != null)
            {
                FpSpread_NotUse.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                {
                    FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_Use.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    string sAttributeName = FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn_Menu.NameView].Text;

                    FpSpread_Use.ActiveSheet.Rows[i].Remove();
                    i--;
                }
            }

            FpSpread_NotUse.ActiveSheet.SetActiveCell(FpSpread_NotUse.ActiveSheet.Rows.Count - 1, (int)enColumn_Menu.NameView, true);
            FpSpread_NotUse.ShowRow(0, FpSpread_NotUse.ActiveSheet.Rows.Count, VerticalPosition.Bottom);
        }

        private void DeleteAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_Use.ActiveSheet != null)
            {
                int iRowCount = FpSpread_Use.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_NotUse.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }

                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_Use.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }
            }
        }

        public override void SaveData()
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    DataTable dtSave = new DataTable("Common_RoleMenuRel");
                    dtSave.Columns.Add("RelID", typeof(long));
                    dtSave.Columns.Add("SystemID", typeof(long));
                    dtSave.Columns.Add("RoleID", typeof(long));
                    dtSave.Columns.Add("MenuID", typeof(long));

                    if (radDropDownListRole.SelectedItem != null && radDropDownListRole.SelectedItem.Value != null)
                    {
                        long lRoleID = long.Parse(radDropDownListRole.SelectedItem.Value.ToString());
                        long lSystemID = long.Parse(fpSpreadSystem.ActiveSheet.Cells[fpSpreadSystem.ActiveSheet.ActiveCell.Row.Index, (int)enColumn.SystemID].Value.ToString());

                        for (int i = 0; i < fpSpreadAvailableMenu.ActiveSheet.Rows.Count; i++)
                        {
                            long lMenuID = long.Parse(fpSpreadAvailableMenu.ActiveSheet.Cells[i, (int)enColumn_Menu.MenuID].Value.ToString());

                            DataRow drNew = dtSave.NewRow();
                            drNew["SystemID"] = lSystemID;
                            drNew["RoleID"] = lRoleID;
                            drNew["MenuID"] = lMenuID;
                            dtSave.Rows.Add(drNew);
                        }

                        string sError = string.Empty;
                        proxy.SaveCommon_RoleMenuRel(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lRoleID, lSystemID, dtSave, ref sError);

                        if (string.IsNullOrEmpty(sError))
                            MessageBox.Show(this, Resources.Message10000, this.Text);
                        else
                            MessageBox.Show(sError, "Import Data");
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

        private void fpSpreadSystem_CellClick(object sender, CellClickEventArgs e)
        {
            ClickRoleRow(e.Row);
        }

        private void ClickRoleRow(int iRow)
        {
            if (radDropDownListRole.SelectedItem != null && radDropDownListRole.SelectedItem.Value != null)
            {
                radLabelSystem.Text = string.Format("System : {0}", fpSpreadSystem.ActiveSheet.Cells[iRow, (int)enColumn.Name].Text);
                long lRoleID = long.Parse(radDropDownListRole.SelectedItem.Value.ToString());
                long lSystemID = long.Parse(fpSpreadSystem.ActiveSheet.Cells[iRow, (int)enColumn.SystemID].Value.ToString());
                mlSystemID = lSystemID;
                SetSpread_Menu(lSystemID, lRoleID);
            }
        }

    }
}
