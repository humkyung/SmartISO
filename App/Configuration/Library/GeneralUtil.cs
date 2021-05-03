using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Linq;
using EDCS.WF.DSL;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;
using Configuration.BSL.Entities;

using EDCS.WF.DSL.Entities;
using CommonLibrary.CL;

namespace Configuration
{
    public static class GeneralUtil
    {
        public static void FitIcon(RadForm radForm, bool bChangeBorderStyleToSizable)
        {
            radForm.FormElement.TitleBar.IconPrimitive.ImageScaling = ImageScaling.SizeToFit;

            if (bChangeBorderStyleToSizable)
                radForm.FormBorderStyle = FormBorderStyle.Sizable;
        }

        public static void FitIcon(ref RadForm radForm, bool bChangeBorderStyleToSizable)
        {
            radForm.FormElement.TitleBar.IconPrimitive.ImageScaling = ImageScaling.SizeToFit;

            if (bChangeBorderStyleToSizable)
                radForm.FormBorderStyle = FormBorderStyle.Sizable;
        }

        public static string MergeListToString(List<string> lst, string sDelimeter)
        {
            string sResult = string.Empty;

            foreach (string s in lst)
            {
                if (sResult == string.Empty)
                    sResult = s;
                else
                    sResult += sDelimeter + s;
            }

            return sResult;
        }


        public static string GetExportFileName(string sMenuName, string sProjectName)
        {
            return GetExportFileName(sMenuName, sProjectName, string.Empty);
        }

        public static string GetExportFileName(string sMenuName, string sProjectName, string sItemType)
        {
            string sResult_Filename = string.Empty;

            if (!string.IsNullOrEmpty(sItemType))
            {
                sResult_Filename = string.Format("SPPID Data_{1}_{0}_{2}({3})", sMenuName, sProjectName, sItemType, DateTime.Now.ToString("yyMMdd"));
            }
            else
            {
                sResult_Filename = string.Format("SPPID Data_{1}_{0}({2})", sMenuName, sProjectName, DateTime.Now.ToString("yyMMdd"));
            }

            return sResult_Filename;
        }

        public static string GetExportFileNameNotMenuName(string sProjectName, string sItemType)
        {
            string sResult_Filename = string.Empty;

            if (!string.IsNullOrEmpty(sItemType))
            {
                sResult_Filename = string.Format("SPPID Data_{0}_{1}({2})", sProjectName, sItemType, DateTime.Now.ToString("yyMMdd"));
            }
            else
            {
                sResult_Filename = string.Format("SPPID Data_{0}({1})", sProjectName, DateTime.Now.ToString("yyMMdd"));
            }

            return sResult_Filename;
        }

        public static void SetComboTreeBoxItemType(ref UserControlComboTreeBox cct)
        {
            cct.Nodes.Clear();
            cct.DropDownHeight = 220;

            foreach (string sitemType in Information.ITEMTYPE)
            {
                TreeNode nod = new TreeNode();

                if (Information.EQUIP_ITEMTYPE.Contains(sitemType))
                {
                    if (!sitemType.Equals(Information.EQUIPMENT))
                    {
                        nod = cct.Nodes[Information.EQUIPMENT].Nodes.Add(sitemType, sitemType);
                    }
                    else
                    {
                        cct.Nodes.Add(sitemType, sitemType);
                    }
                }
                else if (Information.PIPINGCOMP_ITEMTYPE.Contains(sitemType))
                {
                    if (!sitemType.Equals(Information.PIPINGCOMP))
                    {
                        nod = cct.Nodes[Information.PIPINGCOMP].Nodes.Add(sitemType, sitemType);
                    }
                    else
                    {
                        cct.Nodes.Add(sitemType, sitemType);
                    }
                }
                else
                {
                    cct.Nodes.Add(sitemType, sitemType);
                }
            }
        }

        public static void SetSchemaInfo(bool isIE, RadDropDownList radDropDownListProjectList, ref Information information, ref string sError)
        {
            if (radDropDownListProjectList.SelectedItem == null)
                return;

            information.Project = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
            information.Plant = null;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(isIE, information, information.Project, GeneralUtil.InitConnection(information));

                for (int i = 0; i < lstPlant.Count; i++)
                {
                    if (i == 0)
                    {
                        information.Plant = lstPlant[i];
                    }
                }
            }

        }

        public static void SetSPPIDPlantSchemaInfo(bool isIE, EntitySPTool_Project project, ref Information information)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(isIE, information, information.Project, GeneralUtil.InitConnection(information));

                for (int i = 0; i < lstPlant.Count; i++)
                {
                    if (i == 0)
                    {
                        information.Plant = lstPlant[i];
                    }
                }
            }
        }

        public static void SetSchemaInfoCopy(bool isIE, RadDropDownList radDropDownListProjectList, ref Information information, ref string sError)
        {
            if (radDropDownListProjectList.SelectedItem == null)
                return;

            information.Project = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.Tag;
            information.Plant = null;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(isIE, information, information.Project, GeneralUtil.InitConnection(information));

                for (int i = 0; i < lstPlant.Count; i++)
                {
                    if (i == 0)
                    {
                        information.Plant = lstPlant[i];
                    }
                }
            }

        }

        public static void SetDropdownListSystemInfo(bool isIE, ref RadDropDownList radDropDownListSystemList)
        {
            radDropDownListSystemList.Items.Clear();

            radDropDownListSystemList.ValueMember = "SystemID";
            radDropDownListSystemList.DisplayMember = "NameView";

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                radDropDownListSystemList.DataSource = proxy.ReadCommon_System(isIE);

                if (radDropDownListSystemList.Items.Count > 0)
                {
                    radDropDownListSystemList.SelectedIndex = 0;
                }
            }
        }

        public static void SetDropdownListRoleInfo(bool isIE, ref RadDropDownList radDropDownList)
        {
            radDropDownList.Items.Clear();

            radDropDownList.ValueMember = "RoleID";
            radDropDownList.DisplayMember = "Name";

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                radDropDownList.DataSource = proxy.ReadCommon_Role(isIE).Where(w => w.DeleteFlag == false).ToList();

                if (radDropDownList.Items.Count > 0)
                {
                    radDropDownList.SelectedIndex = 0;
                }
            }
        }

        public static void SaveRadDropDownListProjectNo(RadDropDownList p_radDropDownList, EntityEDBInformation p_entityEDBInformation, string p_Tooltype)
        {
            try
            {
                if (p_radDropDownList.SelectedItem != null)
                {
                    if (p_Tooltype == ToolType.Admin)
                    {
                        EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)p_radDropDownList.SelectedItem.DataBoundItem;
                        //p_entityEDBInformation.ProjectMaster = cpProject;
                        p_entityEDBInformation.EmployeeHistory.ProjectMasterID = cpProject.ProjectMasterID;
                        p_entityEDBInformation.EmployeeHistory.ProjectNo = cpProject.ProjectNo;
                    }
                    else if (p_Tooltype == ToolType.EDB)
                    {
                        EntityEDB_Project cpProject = (EntityEDB_Project)p_radDropDownList.SelectedItem.DataBoundItem;
                        //p_entityEDBInformation.Project = cpProject;
                        p_entityEDBInformation.EmployeeHistory.ProjectID_EDB = cpProject.ProjectID;
                    }
                    else if (p_Tooltype == ToolType.SPPID)
                    {
                        EntitySPTool_Project cpProject = (EntitySPTool_Project)p_radDropDownList.SelectedItem.DataBoundItem;
                        //p_entityEDBInformation.Project_SPPID = cpProject;
                        p_entityEDBInformation.EmployeeHistory.ProjectID_SPPID = cpProject.ProjectID;
                    }
                }
            }
            catch (Exception ex)
            {
            }
        }

        public static void SetRadDropDownListProjectNo(bool isIE, ref RadDropDownList p_radDropDownList, EntityEDBInformation p_entityEDBInformation, string p_Tooltype)
        {
            // DropdownSetting
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                p_radDropDownList.Items.Clear();

                if (p_Tooltype == ToolType.Admin)
                {
                    var projects = proxy.ListProjectMasterEntity(isIE).ToList();
                    if (projects.Count > 0)
                    {
                        projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                        p_radDropDownList.ValueMember = "ProjectMasterID";
                        p_radDropDownList.DisplayMember = "ComplexNameView";
                        p_radDropDownList.DataSource = projects;
                        p_radDropDownList.SelectedValue = p_entityEDBInformation.EmployeeHistory.ProjectMasterID;
                    }
                    else
                    {
                        //throw new Exception(Resources.Message20002);
                    }
                }
                else if (p_Tooltype == ToolType.EDB)
                {
                    var projects = proxy.ReadProjects(isIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(odb => odb.No).ToList();
                    if (projects.Count > 0)
                    {
                        projects.ConvertAll(ca => ca.ComplexNameView + " - " + ca.Type);
                        p_radDropDownList.ValueMember = "ProjectID";
                        p_radDropDownList.DisplayMember = "ComplexNameView";
                        p_radDropDownList.DataSource = projects;
                        p_radDropDownList.SelectedValue = p_entityEDBInformation.EmployeeHistory.ProjectID_EDB;
                    }
                    else
                    {
                        //throw new Exception(Resources.Message20002);
                    }
                }
                else if (p_Tooltype == ToolType.SPPID || p_Tooltype == ToolType.SP3D)
                {
                    var projects = proxy.ListSPTool_ProjectListEntity(isIE, p_Tooltype).ToList();
                    if (projects.Count > 0)
                    {
                        p_radDropDownList.ValueMember = "ProjectID";
                        p_radDropDownList.DisplayMember = "ComplexNameView";
                        p_radDropDownList.DataSource = projects;
                        p_radDropDownList.SelectedValue = p_entityEDBInformation.EmployeeHistory.ProjectID_SPPID;
                       
                    }
                    else
                    {
                        //throw new Exception(Resources.Message20002);
                    }
                }

                p_radDropDownList.DropDownHeight = 300;
            }
        }

        public static string InitConnection(Information Information)
        {
            EncryptDecryptQueryString cq = new EncryptDecryptQueryString();

            string sOracleConnectionString = string.Empty;
            try
            {
                sOracleConnectionString = string.Format("Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST={0})(PORT={1}))(CONNECT_DATA=(SERVICE_NAME={2})));User Id={3};Password={4};", Information.Project.DBHost, Information.Project.DBPort, Information.Project.DBService, Information.Project.DBUserID, cq.Decrypt(Information.Project.DBUserPW));
            }
            catch { }

            return sOracleConnectionString;
        }

    }
}
