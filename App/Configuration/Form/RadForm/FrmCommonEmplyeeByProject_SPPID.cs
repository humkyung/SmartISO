using System;
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
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class FrmCommonEmplyeeByProject_SPPID : KRadForm
    {
        private int mEmployeeID = 0;
        private long mProjectMasterID = 0;
        private string mEmployeeName = string.Empty;

        private enum enColumn { Check, DefaultProjectFlag, ProjectID, ProjectNo, ProjectDescription, Last };
        private string[] mColumnName = new string[] { " ", "DefaultProjectFlag", "Project ID", "Project No", "Project Description" };
        private int[] mColumnWidth = new int[] { 80, 0, 0, 100, 220 };
        private bool[] mColumnLock = new bool[] { false, true, true, true, true };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        public FrmCommonEmplyeeByProject_SPPID()
        {
            InitializeComponent();
        }

        public FrmCommonEmplyeeByProject_SPPID(bool isIE, int iEmployeeID, long lProjectMasterID, string sEmployeeName)
        {
            InitializeComponent();

            mEmployeeID = iEmployeeID;
            mProjectMasterID = lProjectMasterID;
            mEmployeeName = sEmployeeName;

            radLabelName.Text = string.Format("Name :  {0}", mEmployeeName);

            InitSpreadSheet();
        }


        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadData, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
            fpSpreadData.ActiveSheet.Columns[(int)enColumn.ProjectNo].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        private void FrmCommonEmplyeeByProject_SPPID_Load(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var vlstProject = proxy.ReadSPToolProjectDetailInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, ToolType.SPPID);
                    var vEmployeeProjectInfo = proxy.ReadCommon_EmployeebyProject_SPPIDDB_BylProjectMaster(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mEmployeeID, mProjectMasterID);

                    foreach (var vProject in vlstProject)
                    {
                        int iRow = fpSpreadData.ActiveSheet.Rows.Count++;

                        if (vProject.DefaultProjectFlag == true)
                        {
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = true;
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Locked = true;
                        }
                        else if (vEmployeeProjectInfo.Where(w => w.ProjectID == vProject.ProjectID).Count() > 0)
                        {
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = true;
                        }
                        else
                        {
                            fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.Check].Value = false;
                        }

                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.DefaultProjectFlag].Value = vProject.DefaultProjectFlag;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectID].Value = vProject.ProjectID;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectNo].Value = vProject.ProjectNo;
                        fpSpreadData.ActiveSheet.Cells[iRow, (int)enColumn.ProjectDescription].Value = vProject.Description;
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

        private void radButtonCreate_Click(object sender, EventArgs e)
        {

            try
            {
                DataTable dtSave = new DataTable("Common_EmployeebyProject_SPPIDDB");
                dtSave.Columns.Add("EmployeebyProject_ID", typeof(long));
                dtSave.Columns.Add("EmployeeID", typeof(int));
                dtSave.Columns.Add("ProjectMasterID", typeof(long));
                dtSave.Columns.Add("ProjectID", typeof(long));

                string sErrorMessage = string.Empty;

                if (fpSpreadData.ActiveSheet != null)
                {

                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        for (int i = 0; i < fpSpreadData.ActiveSheet.Rows.Count; i++)
                        {
                            string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.Check].Value.ToString();
                            string sDefaultProjectFlag = fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.DefaultProjectFlag].Value.ToString();

                            if (sCheck.ToUpper().Equals("TRUE") && !sDefaultProjectFlag.ToUpper().Equals("TRUE"))
                            {
                                long lProjectID = long.Parse(fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.ProjectID].Value.ToString());

                                DataRow drNew = dtSave.NewRow();

                                drNew["EmployeeID"] = mEmployeeID;
                                drNew["ProjectMasterID"] = mProjectMasterID;
                                drNew["ProjectID"] = lProjectID;

                                dtSave.Rows.Add(drNew);
                            }
                        }

                        proxy.SaveCommon_EmployeebyProject_SPPIDDB(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mEmployeeID, mProjectMasterID, dtSave, ref sErrorMessage);

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

        private void radButtonClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
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
