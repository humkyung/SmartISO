using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL.Method;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using EDCS.WF.BSL;
using System;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlCommonEmplyeeByProject : UserControlBase
    {
        private const char CONST_SPLIT_WORD = '§';

        private enum enColumn { EmployeeID, No, Name, GroupName, RoleName, ProjectMasterID, ProjectName, ProjectType, Edit, Last };
        private string[] mColumnName = new string[] { "EmployeeID", "No", "Name", "GroupName", "Role", "ProjectMasterID", "Project", "Type", "Edit" };
        private int[] mColumnWidth = new int[] { 0, 100, 150, 100, 82, 0, 80, 80, 80 };
        private bool[] mColumnLock = new bool[] { true, true, true, true, true, true, true, false };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.NormalButton };

        private enum enColumn_SPPIDDB { EmployeeID, EmployeeName, ProjectMasterID, ProjectID, SPPIDDB, Description, Edit }
        private string[] mColumnName_SPPIDDB = new string[] { "EmployeeID", "EmployeeName", "ProjectMasterID", "ProjectID", "SPPID Database", "Description", "Edit" };
        private int[] mColumnWidth_SPPIDDB = new int[] { 0, 0, 0, 0, 100, 200, 80 };
        private bool[] mColumnLock_SPPIDDB = new bool[] { true, true, true, true, false };
        SpreadUtil.eFarpointCellType[] mColumnType_SPPIDDB = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.NormalButton };

        List<EntityCommon_EmployeebyProject> mlstEmployee = new List<EntityCommon_EmployeebyProject>();
        
        // 앙상블 User Project Info 
        List<EntityCommon_UserInfo> mlstUserEnsembleprojectInfoList = new List<EntityCommon_UserInfo>();

        List<EntityCommon_ProjectMaster> mlstProject = new List<EntityCommon_ProjectMaster>();
        List<EntityCommon_EmployeebyProject> mlstEmployeebyProject = new List<EntityCommon_EmployeebyProject>();

        public UserControlCommonEmplyeeByProject()
        {
            InitializeComponent();

        }

        public override void LoadData()
        {
            GetProjectInfo();

            InitSpreadSheet();
            //SetSpreadSheet(string.Empty);
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadData, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);

            fpSpreadData.ActiveSheet.Columns[(int)enColumn.No].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.GroupName].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.ProjectName].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.ProjectType].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        public void InitSpreadSheet_SPPIDDB()
        {
            SpreadUtil.InitSpread(fpSpreadData_SPPIDDB, mColumnWidth_SPPIDDB, mColumnName_SPPIDDB, mColumnLock_SPPIDDB, null, null, mColumnType_SPPIDDB, null, null, Color.LightGray, true, true);
        }

        private void GetProjectInfo()
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Load user info.  Please wait!";

            mlstUserEnsembleprojectInfoList = SharedMethod.MethodGetProjectInfoAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // User 부서 정보 조회
                List<EntityGroupInfo> entityGroupInfos = new List<EntityGroupInfo>();
                var userList = new List<EntityEDB_UserInfo>();
                proxy.ReadInsaUserInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref userList);
                proxy.ReadInsaGroupInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref entityGroupInfos);

                // EDB+ Project User Info 조회
                mlstEmployee = proxy.ReadEntityEmployeebyProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                mlstProject = proxy.ReadProjectMasterList(EDCS.WF.BSL.Variable.SharedVariable.IsIE);

                foreach (var entityUserInfo in userList.GroupBy(g => g.GroupCode))
                {
                    var entityGroupInfo = entityGroupInfos.FirstOrDefault(s => s.GroupCode == entityUserInfo.Key);
                    if (entityGroupInfo != null)
                    {
                        entityUserInfo.ToList().ForEach(f => f.GroupName = entityGroupInfo.GroupName);
                    }
                }

                this.mlstEmployee.ConvertAll(ca => ca.GroupName = userList.FirstOrDefault(f => f.UserID.ToUpper() == ca.No.ToUpper()) == null ? "" : userList.FirstOrDefault(f => f.UserID.ToUpper() == ca.No.ToUpper()).GroupName);
                this.mlstEmployee.ConvertAll(ca => ca.GroupCode = userList.FirstOrDefault(f => f.UserID.ToUpper() == ca.No.ToUpper()) == null ? "" : userList.FirstOrDefault(f => f.UserID.ToUpper() == ca.No.ToUpper()).GroupCode);

                mlstEmployeebyProject = proxy.ReadEmployeebyProjectAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            }

            FrmProgressStatus.FormClose();
        }

        public void SetSpreadSheet(string sName)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "set user info.  Please wait!";

            if (fpSpreadData.ActiveSheet != null)
            {
                foreach (var itemEmployee in mlstEmployee.Where(w => w.Name.Contains(sName)).OrderBy(w => w.Name).ThenBy(w => w.No))
                {
                    List<EntityCommon_UserInfo> lstUserProject = mlstUserEnsembleprojectInfoList.Where(w => w.UserID == itemEmployee.No).OrderByDescending(w => w.No).ToList();

                    // ex ) ProjectNo / ProjectType : 130075/TRUE
                    Dictionary<string, EntityCommon_ProjectMaster> dicUserProject_Distinct = new Dictionary<string, EntityCommon_ProjectMaster>();


                    foreach (var vUserProject in lstUserProject)
                    {
                        // 중복 제거 차후 Class Distinct로 변경 할것
                        var vSelectProject = mlstProject.Find(w => w.ProjectNo == vUserProject.No && w.ProjectType.ToString().ToUpper().Equals(vUserProject.Type));

                        if (vSelectProject != null)
                        {
                            string sKey = string.Format("{0}{1}{2}", vSelectProject.ProjectNo, CONST_SPLIT_WORD, vSelectProject.ProjectType.ToString());

                            if (!dicUserProject_Distinct.ContainsKey(sKey))
                            {
                                dicUserProject_Distinct.Add(sKey, vSelectProject);
                            }
                        }
                    }

                    var vEmployeebyProject = mlstEmployeebyProject.Where(w => w.EmployeeID == itemEmployee.EmployeeID).ToList();

                    foreach (var iProject in vEmployeebyProject)
                    {
                        var vSelectProject = mlstProject.Find(w => w.ProjectMasterID == iProject.ProjectMasterID);

                        if (vSelectProject != null)
                        {
                            string sKey = string.Format("{0}{1}{2}", vSelectProject.ProjectNo, CONST_SPLIT_WORD, vSelectProject.ProjectType.ToString());

                            if (!dicUserProject_Distinct.ContainsKey(sKey))
                            {
                                dicUserProject_Distinct.Add(sKey, vSelectProject);
                            }
                        }
                    }

                    int iFirstRow = 0;
                    int iLastRowCount = 1;

                    var vUserProjectList = dicUserProject_Distinct.OrderByDescending(w => w.Value.ProjectNo);
                    if (vUserProjectList.Count() > 0)
                    {
                        foreach (var item in vUserProjectList)
                        {
                            int iRow = fpSpreadData.ActiveSheet.Rows.Count++;

                            if (iLastRowCount == 1)
                            {
                                iFirstRow = iRow;
                            }

                            //string[] arrKey = item.Key.Split(CONST_SPLIT_WORD);

                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.EmployeeID].Value = itemEmployee.EmployeeID;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.No].Value = itemEmployee.No;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value = itemEmployee.Name;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.GroupName].Value = itemEmployee.GroupName;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.RoleName].Value = itemEmployee.RoleName;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectMasterID].Value = item.Value.ProjectMasterID;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectName].Value = item.Value.ProjectNo;

                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectType].Value = item.Value.ProjectType ? "Project" : "Proposal";
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectType].ForeColor = item.Value.ProjectType ? Color.Blue : Color.Red;
                            iLastRowCount++;
                        }
                    }
                    else
                    {
                        // 앙상블 권한이 없는 사람의 임시 프로젝트 추가
                        int iRow = fpSpreadData.ActiveSheet.Rows.Count++;

                        iFirstRow = iRow;

                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.EmployeeID].Value = itemEmployee.EmployeeID;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.No].Value = itemEmployee.No;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Name].Value = itemEmployee.Name;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.GroupName].Value = itemEmployee.GroupName;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.RoleName].Value = itemEmployee.RoleName;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectMasterID].Value = -1;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectName].Value = string.Empty;

                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectType].Value = "";

                        iLastRowCount++;
                    }

                    // 같은 이름으로 병합
                    if (dicUserProject_Distinct.Count > 0 && iLastRowCount != 1)
                    {
                        int iRowSpan = iLastRowCount - 1;
                        fpSpreadData.ActiveSheet.Cells[iFirstRow, (int)enColumn.No].RowSpan = iRowSpan;
                        fpSpreadData.ActiveSheet.Cells[iFirstRow, (int)enColumn.Name].RowSpan = iRowSpan;
                        fpSpreadData.ActiveSheet.Cells[iFirstRow, (int)enColumn.GroupName].RowSpan = iRowSpan;
                        fpSpreadData.ActiveSheet.Cells[iFirstRow, (int)enColumn.RoleName].RowSpan = iRowSpan;
                        fpSpreadData.ActiveSheet.Cells[iFirstRow, (int)enColumn.Edit].RowSpan = iRowSpan;
                    }

                    vEmployeebyProject.Clear();
                    dicUserProject_Distinct.Clear();
                    //lstUserProject.Clear();
                }
            }

            FrmProgressStatus.FormClose();
        }

        private void radTextBoxSearchName_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                InitSpreadSheet();
                SetSpreadSheet(radTextBoxSearchName.Text);
            }
        }

        private void fpSpreadData_ButtonClicked(object sender, EditorNotifyEventArgs e)
        {
            if (e.Column == (int)enColumn.Edit)
            {
                int iEmployeeID = int.Parse(fpSpreadData.ActiveSheet.Cells[e.Row, (int)enColumn.EmployeeID].Value.ToString());
                string sEmployeeNo = fpSpreadData.ActiveSheet.Cells[e.Row, (int)enColumn.No].Value.ToString();
                string sEmployeeName = fpSpreadData.ActiveSheet.Cells[e.Row, (int)enColumn.Name].Value.ToString();

                FrmCommonEmplyeeByProject frm = new FrmCommonEmplyeeByProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, iEmployeeID, sEmployeeNo, sEmployeeName, mlstUserEnsembleprojectInfoList, mlstProject);

                if (frm.ShowDialog(this) == DialogResult.OK)
                {
                    GetProjectInfo();
                    InitSpreadSheet();
                    SetSpreadSheet(radTextBoxSearchName.Text);
                }
            }
        }

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            if (e.Column != (int)enColumn.Edit)
            {
                SetSPPIDDB(e.Row);
            }
        }

        private void SetSPPIDDB(int iRowIndex)
        {
            SheetView sheetView = fpSpreadData.ActiveSheet;
            if (sheetView != null)
            {
                InitSpreadSheet_SPPIDDB();

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    long lProjectMasterID = long.Parse(Util.NullToSpace(sheetView.Cells[iRowIndex, (int)enColumn.ProjectMasterID].Text));
                    string sEmployeeID = Util.NullToSpace(sheetView.Cells[iRowIndex, (int)enColumn.EmployeeID].Text);
                    string sEmployeeName = Util.NullToSpace(sheetView.Cells[iRowIndex, (int)enColumn.Name].Text);

                    // Default Project 
                    var lstProject = proxy.ReadSPToolProjectDetailInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, ToolType.SPPID);
                    var vEmployeeProjectInfo = proxy.ReadCommon_EmployeebyProject_SPPIDDB_BylProjectMaster(EDCS.WF.BSL.Variable.SharedVariable.IsIE, int.Parse(sEmployeeID), lProjectMasterID);

                    int iLastRowIndex = 1;

                    foreach (var vProject in lstProject)
                    {
                        if (vProject.DefaultProjectFlag == true || vEmployeeProjectInfo.Where(w => w.ProjectID == vProject.ProjectID).Count() > 0)
                        {
                            int iRow = fpSpreadData_SPPIDDB.ActiveSheet.Rows.Count++;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.EmployeeID].Value = sEmployeeID;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.EmployeeName].Value = sEmployeeName;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.ProjectMasterID].Value = vProject.ProjectMasterID;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.ProjectID].Value = vProject.ProjectID;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.SPPIDDB].Value = vProject.ProjectNo;
                            fpSpreadData_SPPIDDB.ActiveSheet.Cells[iRow, (int)enColumn_SPPIDDB.Description].Value = vProject.Description;

                            iLastRowIndex++;
                        }
                    }

                    if (iLastRowIndex > 1)
                        fpSpreadData_SPPIDDB.ActiveSheet.Cells[0, (int)enColumn_SPPIDDB.Edit].RowSpan = iLastRowIndex;
                }
            }
        }

        private void fpSpreadData_SPPIDDB_ButtonClicked(object sender, EditorNotifyEventArgs e)
        {
            if (e.Column == (int)enColumn_SPPIDDB.Edit)
            {
                int iRowIndex = fpSpreadData.ActiveSheet.ActiveRowIndex;

                int iEmployeeID = int.Parse(fpSpreadData_SPPIDDB.ActiveSheet.Cells[e.Row, (int)enColumn_SPPIDDB.EmployeeID].Value.ToString());
                long lProjectMasterID = long.Parse(fpSpreadData_SPPIDDB.ActiveSheet.Cells[e.Row, (int)enColumn_SPPIDDB.ProjectMasterID].Value.ToString());
                string sEmployeeName = fpSpreadData_SPPIDDB.ActiveSheet.Cells[e.Row, (int)enColumn_SPPIDDB.EmployeeName].Value.ToString();

                FrmCommonEmplyeeByProject_SPPID frm = new FrmCommonEmplyeeByProject_SPPID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, iEmployeeID, lProjectMasterID, sEmployeeName);

                if (frm.ShowDialog(this) == DialogResult.OK)
                {
                    // 재조회 
                    SetSPPIDDB(iRowIndex);
                }
            }
        }

    }
}
