using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FarPoint.Win.Spread;
// TODO LGS : BSL SpreadUtil 통합 작업, using 추가
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using TechSun.Theme;
using Configuration.Properties;

namespace Configuration
{
    public partial class UserControlCommonConnectLog : UserControlBase
    {
        private enum LTColumn { Level1, Level2, Level3, LoginMonth, LoginDay, GroupName, EmployeeNo, EmployeeNameView, Position, Login, Logout };

        private string[] mColumnName = new string[] { "Level 1", "Level 2", "Level 3", "년월", "년일", "부서", "사번", "이름", "직위", "로그인 시간", "로그아웃시간" };

        private int[] marColumnWidths = new int[] { 150, 150, 150, 80, 90, 150, 80, 80, 120, 130, 130 };


        private enum LTColumn_Daily { Level1, Level2, Level3, LoginMonth, LoginDay, GroupName, EmployeeNo, EmployeeNameView, Position };

        private string[] mColumnName_Daily = new string[] { "Level 1", "Level 2", "Level 3", "년월", "년일", "부서", "사번", "이름", "직위" };

        private int[] marColumnWidths_Daily = new int[] { 150, 150, 150, 80, 90, 150, 80, 80, 120 };


        public UserControlCommonConnectLog()
        {
            InitializeComponent();

            this.radButtonSearch.Click += new System.EventHandler(this.radButtonSearch_Click);

            this.MethodDesignForm();
            InitSpread_Daily();
        }

        private void MethodDesignForm()
        {
            radDateTimePickerStart.Value = System.DateTime.Now;
            radDateTimePickerEnd.Value = System.DateTime.Now;
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);

            SheetView sheetView = fpSpreadData.ActiveSheet;

            sheetView.Columns[(int)LTColumn.LoginMonth].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn.LoginDay].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn.EmployeeNo].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn.EmployeeNameView].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn.Login].HorizontalAlignment = CellHorizontalAlignment.Left;
            sheetView.Columns[(int)LTColumn.Logout].HorizontalAlignment = CellHorizontalAlignment.Left;
        }


        private void InitSpread_Daily()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths_Daily, mColumnName_Daily, null, null, null, null, null, null, Color.LightGray, true, true);
            SheetView sheetView = fpSpreadData.ActiveSheet;
            sheetView.Columns[(int)LTColumn_Daily.LoginMonth].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn_Daily.LoginDay].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn_Daily.EmployeeNo].HorizontalAlignment = CellHorizontalAlignment.Center;
            sheetView.Columns[(int)LTColumn_Daily.EmployeeNameView].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        private void SetSpread()
        {
            string sLoadingTexts = "Loading data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                InitSpread();
                SheetView sheetView = fpSpreadData.ActiveSheet;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {

                    var startDate = radCheckBoxAll.Checked ? radDateTimePickerStart.Value.AddYears(-20) : new DateTime(radDateTimePickerStart.Value.Year, radDateTimePickerStart.Value.Month, radDateTimePickerStart.Value.Day);
                    var endDate = radCheckBoxAll.Checked ? DateTime.Now.AddDays(1) : new DateTime(radDateTimePickerEnd.Value.Year, radDateTimePickerEnd.Value.Month, radDateTimePickerEnd.Value.Day, 23, 59, 59);

                    var systems = proxy.ReadCommon_System(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    List<long> lstsystems = new List<long>();

                    foreach (var lsistem in systems)
                    {
                        lstsystems.Add(lsistem.SystemID);
                    }

                    List<EntityConnectLogDynamicsAll> lstConnectLogDynamicsAll = proxy.ReadConnectLogDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, startDate, endDate, lstsystems);

                    lstConnectLogDynamicsAll.ForEach(f => f.LoginMonth = f.Login.ToString("yyyy-MM"));
                    lstConnectLogDynamicsAll.ForEach(f => f.LoginDay = f.Login.ToString("yyyy-MM-dd"));

                    var userList = new List<EntityEDB_UserInfo>();
                    proxy.ReadInsaUserInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref userList);

                    List<EntityGroupInfo> entityGroupInfos = new List<EntityGroupInfo>();
                    proxy.ReadInsaGroupInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref entityGroupInfos);

                    foreach (var entityUserInfo in userList.GroupBy(g => g.GroupCode))
                    {
                        var entityGroupInfo = entityGroupInfos.FirstOrDefault(s => s.GroupCode == entityUserInfo.Key);
                        if (entityGroupInfo != null)
                        {
                            entityUserInfo.ToList().ForEach(f => f.GroupName = entityGroupInfo.GroupName);
                        }
                    }

                    foreach (var dept in lstConnectLogDynamicsAll.GroupBy(g => g.EmployeeNo))
                    {
                        var user = userList.Where(w => w.UserID == dept.Key).ToList();
                        if (user.Count > 0)
                        {
                            dept.ToList().ConvertAll(c => c.GroupName = user[0].GroupName);
                            dept.ToList().ConvertAll(c => c.Position = user[0].Position.Replace("갑", "").Replace("을", ""));
                        }
                    }

                    foreach (var vConnectLogDynamics in lstConnectLogDynamicsAll.OrderBy(o => o.Login).ToList())
                    {
                        int iRow = sheetView.Rows.Count++;
                        sheetView.Cells[iRow, (int)LTColumn.Level1].Value = string.Format("{1} {0}", vConnectLogDynamics.Level1, vConnectLogDynamics.MainOrders.ToString());
                        sheetView.Cells[iRow, (int)LTColumn.Level2].Value = string.Format("{1} {0}", vConnectLogDynamics.Level2, vConnectLogDynamics.SubOrders.ToString());
                        sheetView.Cells[iRow, (int)LTColumn.Level3].Value = string.Format("{1} {0}", vConnectLogDynamics.Level3, vConnectLogDynamics.No.ToString().PadLeft(2, '0'));
                        sheetView.Cells[iRow, (int)LTColumn.LoginMonth].Value = vConnectLogDynamics.Login.ToString("yyyy-MM");
                        sheetView.Cells[iRow, (int)LTColumn.LoginDay].Value = vConnectLogDynamics.Login.ToString("yyyy-MM-dd");
                        sheetView.Cells[iRow, (int)LTColumn.GroupName].Value = vConnectLogDynamics.GroupName;
                        sheetView.Cells[iRow, (int)LTColumn.EmployeeNo].Value = vConnectLogDynamics.EmployeeNo;
                        sheetView.Cells[iRow, (int)LTColumn.EmployeeNameView].Value = vConnectLogDynamics.EmployeeNameView;
                        sheetView.Cells[iRow, (int)LTColumn.Position].Value = vConnectLogDynamics.Position;
                        sheetView.Cells[iRow, (int)LTColumn.Login].Value = vConnectLogDynamics.Login;
                        sheetView.Cells[iRow, (int)LTColumn.Logout].Value = vConnectLogDynamics.Logout;
                    }

                    SpreadUtil.SetAllColumnAutoFilterAndSort(sheetView);

                }
            }
            catch (Exception ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
            }


            frmPopupWaitingBarCallMethod.MethodClose();

        }

        private void SetSpread_Daily()
        {
            string sLoadingTexts = "Loading data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                InitSpread_Daily();
                SheetView sheetView = fpSpreadData.ActiveSheet;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var startDate = radCheckBoxAll.Checked ? radDateTimePickerStart.Value.AddYears(-20) : new DateTime(radDateTimePickerStart.Value.Year, radDateTimePickerStart.Value.Month, radDateTimePickerStart.Value.Day);
                    var endDate = radCheckBoxAll.Checked ? DateTime.Now.AddDays(1) : new DateTime(radDateTimePickerEnd.Value.Year, radDateTimePickerEnd.Value.Month, radDateTimePickerEnd.Value.Day, 23, 59, 59);

                    var systems = proxy.ReadCommon_System(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    List<long> lstsystems = new List<long>();

                    foreach (var lsistem in systems)
                    {
                        lstsystems.Add(lsistem.SystemID);
                    }

                    List<EntityConnectLogDynamicsAll> lstConnectLogDynamicsAll = proxy.ReadConnectLogDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, startDate, endDate, lstsystems);

                    lstConnectLogDynamicsAll.ForEach(f => f.LoginMonth = f.Login.ToString("yyyy-MM"));
                    lstConnectLogDynamicsAll.ForEach(f => f.LoginDay = f.Login.ToString("yyyy-MM-dd"));

                    var userList = new List<EntityEDB_UserInfo>();
                    proxy.ReadInsaUserInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref userList);

                    List<EntityGroupInfo> entityGroupInfos = new List<EntityGroupInfo>();
                    proxy.ReadInsaGroupInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref entityGroupInfos);

                    foreach (var entityUserInfo in userList.GroupBy(g => g.GroupCode))
                    {
                        var entityGroupInfo = entityGroupInfos.FirstOrDefault(s => s.GroupCode == entityUserInfo.Key);
                        if (entityGroupInfo != null)
                        {
                            entityUserInfo.ToList().ForEach(f => f.GroupName = entityGroupInfo.GroupName);
                        }
                    }

                    foreach (var dept in lstConnectLogDynamicsAll.GroupBy(g => g.EmployeeNo))
                    {
                        var user = userList.Where(w => w.UserID == dept.Key).ToList();
                        if (user.Count > 0)
                        {
                            dept.ToList().ConvertAll(c => c.GroupName = user[0].GroupName);
                            dept.ToList().ConvertAll(c => c.Position = user[0].Position.Replace("갑", "").Replace("을", ""));
                        }
                    }

                    List<EntityConnectLogDynamicsAll> lstConnectLogDynamicsDistinct = lstConnectLogDynamicsAll
                                                                                      .GroupBy(g => new { g.Level1, g.Level2, g.Level3, g.LoginMonth, g.LoginDay, g.EmployeeNo, g.EmployeeNameView })
                                                                                      .Select(s => s.First())
                                                                                      .ToList();

                    foreach (var vConnectLogDynamics in lstConnectLogDynamicsDistinct.OrderBy(tb => tb.LoginDay).ToList())
                    {
                        int iRow = sheetView.Rows.Count++;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.Level1].Value = string.Format("{1} {0}", vConnectLogDynamics.Level1, vConnectLogDynamics.MainOrders.ToString());
                        sheetView.Cells[iRow, (int)LTColumn_Daily.Level2].Value = string.Format("{1} {0}", vConnectLogDynamics.Level2, vConnectLogDynamics.SubOrders.ToString());
                        sheetView.Cells[iRow, (int)LTColumn_Daily.Level3].Value = string.Format("{1} {0}", vConnectLogDynamics.Level3, vConnectLogDynamics.No.ToString().PadLeft(2, '0'));
                        sheetView.Cells[iRow, (int)LTColumn_Daily.LoginMonth].Value = vConnectLogDynamics.LoginMonth;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.LoginDay].Value = vConnectLogDynamics.LoginDay;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.GroupName].Value = vConnectLogDynamics.GroupName;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.EmployeeNo].Value = vConnectLogDynamics.EmployeeNo;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.EmployeeNameView].Value = vConnectLogDynamics.EmployeeNameView;
                        sheetView.Cells[iRow, (int)LTColumn_Daily.Position].Value = vConnectLogDynamics.Position;
                    }

                    SpreadUtil.SetAllColumnAutoFilterAndSort(sheetView);

                }
            }
            catch (Exception ex)
            {
                frmPopupWaitingBarCallMethod.MethodClose();
            }


            frmPopupWaitingBarCallMethod.MethodClose();

        }

        private void radButtonSearch_Click(object sender, EventArgs e)
        {
            if (radCheckBoxDaily.Checked)
                SetSpread_Daily();
            else
                SetSpread();
        }

        public override void ExportToExcel()
        {
            if (SpreadUtil.SaveExcel(fpSpreadData, string.Empty, string.Empty, true))
            {
                frmInfomation frmInformation = new frmInfomation(Resources.Message10000);
                frmInformation.ShowDialog(this);
            }
        }

    }
}
