using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Linq;

using Telerik.WinControls.UI;
using Configuration.Properties;
using SmartISO.Service.Interface;
using System.Data;

namespace Configuration
{
    public partial class UserControlCommonProject : UserControlBase
    {
        private enum menColumn { Select, ProjectMasterID, ProjectNo, ProjectType, NameView, EDBFlag, SPPIDFlag, SP3DFlag, SPIFlag, PDSFlag, EnsembleType, Type3D, TypeInstrument, DeleteFlag, Status, Last };
        //private string[] marColumnName = { " ", "ProjectMasterID", "ProjectNo", "ProjectType", "Project Description", "List", ToolType.SPPID, "3D", "Inst", ToolType.PDS, ToolType.Ensemble, "3D Type", "Inst Tool Type", "Delete", "Status" };
        private int[] marColumnWidth = { 40, 0, 100, 100, 250, 100, 100, 100, 100, 0, 100, 100, 120, 0, 0, 0 };
        private bool[] mColumnLock = new bool[] { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
        //private List<EntityEDB_Project> MdmProjects = new List<EntityEDB_Project>();
        private string[] projectNos = null;

        public UserControlCommonProject()
        {
            InitializeComponent();
            {
                (this.radGridViewProject.Columns["Type"] as GridViewComboBoxColumn).DataSource = new List<string>() { "PDS", "PDMS", "SP3D" };
            }

            Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
        }

        private void OnIdleUpdate()
        {
            var oCheckedRows = from Telerik.WinControls.UI.GridViewRowInfo row in this.radGridViewProject.Rows
                              where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                              select row;
            this.radButtonDelete.Enabled = (oCheckedRows.Count() > 0);
        }

        public override void LoadData()
        {
            string sLoadingTexts = "Loading data.  Please wait.";
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(3, new List<string>(){sLoadingTexts});
            frmPopupWaitingBarCallMethod.MethodSetStart();
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

            try
            {
                using (ISmartISOService svcProxy = SmartISO.Framework.Service.ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
                {
                    using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetAllProjectListCommand", null))
                    {
                        this.radGridViewProject.DataSource = ds.Tables[0];
                    }

                    foreach (GridViewRowInfo oGridRow in this.radGridViewProject.Rows)
                    {
                        oGridRow.IsVisible = (false.ToString() == oGridRow.Cells["DeleteYN"].Value.ToString());
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            finally
            {
                frmPopupWaitingBarCallMethod.MethodClose();
            }
        }

        private void InitSpread_ProjectMaster()
        {
            
        }

        private void SetSpread_ProjectMaster()
        {
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            GridViewRowInfo oGridRow = this.radGridViewProject.Rows.AddNew();

            //if (fpSpreadProjectMaster.ActiveSheet != null)
            //{
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.ProjectMasterID].Value = "-1";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.ProjectType].Value = "True";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.EDBFlag].Value = "False";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SPPIDFlag].Value = "False";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SP3DFlag].Value = "False";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SPIFlag].Value = "False";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.PDSFlag].Value = "False";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.EnsembleType].Value = "0";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.Type3D].Value = " ";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.TypeInstrument].Value = " ";
            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.DeleteFlag].Value = false;

            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.EDBFlag], false);
            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SPPIDFlag], false);
            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SP3DFlag], false);
            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.SPIFlag], false);
            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.PDSFlag], false);
            //    SetCellSettingColor(fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.EnsembleType], false);

            //    fpSpreadProjectMaster.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text = SpreadFlag.Add;

            //    fpSpreadProjectMaster.ActiveSheet.SetActiveCell(fpSpreadProjectMaster.ActiveSheet.Rows.Count - 1, (int)menColumn.ProjectNo, true);
            //    fpSpreadProjectMaster.ShowRow(0, fpSpreadProjectMaster.ActiveSheet.Rows.Count, FarPoint.Win.Spread.VerticalPosition.Bottom);

            //    InitSplitContainer();

            //}
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            using (ISmartISOService svcProxy = SmartISO.Framework.Service.ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
            {
                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "DeleteProjectListCommand"))
                {
                    var oCheckedRows = from Telerik.WinControls.UI.GridViewRowInfo row in this.radGridViewProject.Rows
                                       where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                                       select row;
                    foreach (Telerik.WinControls.UI.GridViewRowInfo row in oCheckedRows)
                    {
                        DataRow dr = dsParam.Tables["Params"].NewRow();
                        {
                            dr["UID"] = row.Cells["UID"].Value.ToString();
                        }
                        dsParam.Tables["Params"].Rows.Add(dr);

                        this.radGridViewProject.Rows[row.Index].Cells["DeleteYN"].Value = true;
                        this.radGridViewProject.Rows[row.Index].IsVisible = false;
                    }

                    svcProxy.ExecuteCommand("SmartISO.BSL", "DeleteProjectListCommand", dsParam);
                }
            }
        }

        private void radButtonAttributeSave_Click(object sender, EventArgs e)
        {
            try
            {
                SaveProjectMaster(false);
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

        public void SaveProjectMaster(bool bOnlyDeleteItem)
        {
            try
            {
                using (ISmartISOService svcProxy = SmartISO.Framework.Service.ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveProjectListCommand"))
                    {
                        foreach (GridViewRowInfo oGridRow in this.radGridViewProject.Rows)
                        {
                            DataRow dr = dsParam.Tables["Params"].NewRow();
                            {
                                object oValue = oGridRow.Cells["UID"].Value;
                                dr["UID"] = (null != oValue) ? oValue.ToString() : string.Empty;
                                dr["ProjectNo"] = oGridRow.Cells["ProjectNo"].Value.ToString();
                                dr["Type"] = oGridRow.Cells["Type"].Value.ToString();
                                oValue = oGridRow.Cells["Desc"].Value;
                                dr["Desc"] = (null != oValue) ? oValue.ToString() : string.Empty;
                                oValue = oGridRow.Cells["Owner"].Value;
                                dr["Owner"] = (null != oValue) ? oValue.ToString() : string.Empty;
                                oValue = oGridRow.Cells["DeleteYN"].Value;
                                dr["DeleteYN"] = (true.ToString() == oValue.ToString());
                            }
                            dsParam.Tables["Params"].Rows.Add(dr);
                        }

                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveProjectListCommand", dsParam);
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        private void MethodInsertEDBProject(long p_projectMasterID, string p_no, string p_type)
        {
            //var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            //using (proxy as IDisposable)
            //{
            //    var edbProject = proxy.ReadProjectByProjectMasterID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_projectMasterID);
            //    //edb 프로젝트가 없을 경우에 생성해줌.
            //    if (edbProject == null)
            //    {
            //        var mdmproject = MdmProjects.Where(w => w.No == p_no && w.Type == p_type).FirstOrDefault();
            //        var newProjects = new List<EDB_Project>();
            //        newProjects.Add(new EDB_Project()
            //        {
            //            ProjectMasterID = p_projectMasterID,
            //            CaseMainFlag = true,
            //            ListTypeID = null,
            //            CreationDate = DateTime.Now,
            //            CaseName = "BASE",
            //            No = p_no,
            //            Type = p_type,
            //            Type2 = mdmproject == null ? null : mdmproject.Type2,
            //        });
            //        proxy.CreateProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newProjects, true);
            //        StringBuilder sb = new StringBuilder();
            //        sb.AppendFormat("UPDATE EDB_Project set CaseProjectID = {0} where ProjectId = {0}", newProjects[0].ProjectID);
            //        proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
            //    }
            //}
        }

        private void InitSplitContainer()
        {
            //radSplitContainerProjectDetail.Controls.Clear();

            //if (fpSpreadProjectMaster.ActiveSheet != null)
            //{
            //    string sStatus = fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Value.ToString();

            //    labelProjectNo.Text = fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.ProjectNo].Text;
            //    labelProjectType.Text = fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.ProjectType].Text;
            //    labelDescription.Text = fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.NameView].Text;

            //    if (sStatus != SpreadFlag.Add)
            //    {
            //        long iMasterProjctID = long.Parse(fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.ProjectMasterID].Value.ToString());

            //        if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.EDBFlag].Value.ToString() == "True")
            //        {
            //            SplitPanel splitPanel = new SplitPanel();
            //            this.MethodInsertEDBProject(iMasterProjctID, labelProjectNo.Text, labelProjectType.Text);
            //            UserControlCommonProjectSetting_EDB uc = new UserControlCommonProjectSetting_EDB(EDCS.WF.BSL.Variable.SharedVariable.IsIE, iMasterProjctID);
            //            uc.Dock = DockStyle.Fill;
            //            splitPanel.Controls.Add(uc);
            //            radSplitContainerProjectDetail.Controls.Add(splitPanel);
            //        }

            //        if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.SPPIDFlag].Value.ToString() == "True")
            //        {
            //            SetSPTools_Setting(iMasterProjctID, ToolType.SPPID);
            //        }

            //        if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.SP3DFlag].Value.ToString() == "True")
            //        {
            //            if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.Type3D].Value.ToString() == Type3D.SP3D)
            //            {
            //                SetSPTools_Setting(iMasterProjctID, ToolType.SP3D);
            //            }
            //            else if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.Type3D].Value.ToString() == Type3D.PDS)
            //            {
            //                SetSPTools_Setting(iMasterProjctID, ToolType.PDS);
            //            }
            //            else if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.Type3D].Value.ToString() == Type3D.PDMS)
            //            {
            //                SetSPTools_Setting(iMasterProjctID, ToolType.PDMS);
            //            }
            //        }

            //        if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.SPIFlag].Value.ToString() == "True")
            //        {
            //            if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.TypeInstrument].Value.ToString() == TypeInstrument.SPI)
            //            {
            //                SetSPTools_Setting(iMasterProjctID, ToolType.SPI);
            //            }
            //            else if (fpSpreadProjectMaster.ActiveSheet.Cells[fpSpreadProjectMaster.ActiveSheet.ActiveRowIndex, (int)menColumn.TypeInstrument].Value.ToString() == TypeInstrument.AI)
            //            {
            //                SetSPTools_Setting(iMasterProjctID, ToolType.AI);
            //            }
            //        }
            //    }
            //}
        }

        private void SetSPTools_Setting(long lMasterProjectID, string sToolType)
        {
            //SplitPanel splitPanel = new SplitPanel();
            //UserControlCommonProjectSetting_SPTool uc = new UserControlCommonProjectSetting_SPTool(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lMasterProjectID, sToolType);
            //uc.Dock = DockStyle.Fill;
            //splitPanel.Controls.Add(uc);
            //radSplitContainerProjectDetail.Controls.Add(splitPanel);
        }
    }
}
