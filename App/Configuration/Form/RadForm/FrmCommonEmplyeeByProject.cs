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
using Telerik.WinControls.UI;
using TechSun.Theme;

namespace Configuration
{
    public partial class FrmCommonEmplyeeByProject : KRadForm
    {
        private int mEmployeeID = 0;
        private string mEmployeeNo = string.Empty;
        private string mEmployeeName = string.Empty;

        private enum enColumn { Check, ProjectID, ProjectNo, ProjectDescription, ProjectType, Type, Last };
        private string[] mColumnName = new string[] { " ", "Project ID", "ProjectNo", "Project Description", "Project Type", "Type" };
        private int[] mColumnWidth = new int[] { 80, 0, 100, 220, 100, 100 };
        private bool[] mColumnLock = new bool[] { false, true, true, true, true, true };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        List<EntityCommon_UserInfo> mlstUserEnsembleprojectInfoList = new List<EntityCommon_UserInfo>();
        List<EntityCommon_ProjectMaster> mlstProject = new List<EntityCommon_ProjectMaster>();

        public FrmCommonEmplyeeByProject()
        {
            InitializeComponent();
        }

        public FrmCommonEmplyeeByProject(bool isIE, int iEmployeeID, string sEmployeeNo, string sEmployeeName, List<EntityCommon_UserInfo> lstUserEnsembleprojectInfoList, List<EntityCommon_ProjectMaster> lstProject)
        {
            InitializeComponent();

            mEmployeeID = iEmployeeID;
            mEmployeeNo = sEmployeeNo;
            mEmployeeName = sEmployeeName;

            mlstUserEnsembleprojectInfoList = lstUserEnsembleprojectInfoList;
            mlstProject = lstProject;

            radLabelName.Text = string.Format("Name :  {0}", mEmployeeName);

            InitSpreadSheet();
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadData, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);

            fpSpreadData.ActiveSheet.Columns[(int)enColumn.ProjectNo].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.ProjectType].HorizontalAlignment = CellHorizontalAlignment.Center;
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.Type].HorizontalAlignment = CellHorizontalAlignment.Center;

            SpreadUtil.SetAutoFilterAndAutoSort(fpSpreadData.ActiveSheet, AutoFilterMode.EnhancedContextMenu);

        }

        private void FrmCommonEmplyeeByProject_Load(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    // 앙상블 내 프로젝트 권한 조회
                    List<EntityCommon_UserInfo> lstUserProject = mlstUserEnsembleprojectInfoList.Where(w => w.UserID == mEmployeeNo).ToList();

                    // 추가 프로젝트 조회
                    var vEmployeebyProject = proxy.ReadEmployeebyProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mEmployeeID);

                    // 프로젝트 세팅
                    foreach (var sProject in mlstProject)
                    {
                        int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectID].Value = sProject.ProjectMasterID;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectNo].Value = sProject.ProjectNo;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectDescription].Value = sProject.NameView;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectType].Value = sProject.ProjectType ? "Project" : "Proposal";
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectType].ForeColor = sProject.ProjectType ? Color.Blue : Color.Red;

                        var vSelectProject = lstUserProject.Find(w => w.No == sProject.ProjectNo && w.Type.ToString().ToUpper().Equals(sProject.ProjectType.ToString().ToUpper()));

                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = false;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Type].Value = string.Empty;

                        if (vSelectProject != null)
                        {
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Locked = true;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = true;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Type].Value = "Ensemble";
                        }
                        else
                        {
                            // 추가 프로젝트 세팅 / 앙상블 프로젝트이면 미적용
                            if (vEmployeebyProject.Where(w => w.ProjectMasterID == sProject.ProjectMasterID).Count() > 0)
                            {
                                fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = true;
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

        private void radButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    DataTable dtSave = new DataTable("Common_EmployeebyProject");
                    dtSave.Columns.Add("EmployeebyProjectID", typeof(long));
                    dtSave.Columns.Add("EmployeeID", typeof(int));
                    dtSave.Columns.Add("ProjectMasterID", typeof(long));

                    string sErrorMessage = string.Empty;

                    if (fpSpreadData.ActiveSheet != null)
                    {
                        for (int i = 0; i < fpSpreadData.ActiveSheet.Rows.Count; i++)
                        {
                            string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.Check].Value.ToString();
                            string sType = fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.Type].Value.ToString();

                            if (sCheck.ToUpper().Equals("TRUE") && !sType.Equals("Ensemble"))
                            {
                                long lProjectMasterID = long.Parse(fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.ProjectID].Value.ToString());

                                DataRow drNew = dtSave.NewRow();

                                drNew["EmployeeID"] = mEmployeeID;
                                drNew["ProjectMasterID"] = lProjectMasterID;

                                dtSave.Rows.Add(drNew);
                            }
                        }

                        proxy.SaveCommon_EmployeebyProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mEmployeeID, dtSave, ref sErrorMessage);

                        this.DialogResult = DialogResult.OK;
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

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)enColumn.Check)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
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
                                if (sheetView.Cells[i, 0].Locked == false)
                                    sheetView.Cells[i, 0].Value = check;
                            }
                        }
                    }
                }
            }
        }

    }
}
