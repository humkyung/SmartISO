using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CommonLibrary.CL;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI.Data;
using System.Reflection;
using System.Data;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlEDBCopyProject : UserControlBase
    {
        public UserControlEDBCopyProject()
        {
            try
            {
                this.InitializeComponent();
                this.Dock = DockStyle.Fill;

                // General
                //this.MethodDesignForm();
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }
        public override void LoadData()
        {
            try
            {
                //new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme("공사중").RadMessageBoxShow();
                //return;
                //this.radTextBoxConnectionStringSource.Text = ConfigurationManager.ConnectionStrings["EDCSEntities"].ConnectionString;
                //this.radTextBoxConnectionStringDest.Text = ConfigurationManager.ConnectionStrings["EDCSEntities"].ConnectionString;
                this.radDropDownListListType.ValueMember = "ListTypeID";
                this.radDropDownListListType.DisplayMember = "ComplexNameView";
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                }

                this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                this.MethodBindingServer(this.radDropDownListServerDest);
                this.MethodBindingServer(this.radDropDownListServerSource);
                this.MethodBindingProjectSource();
                this.MethodBindingProjectDest();
                this.radDropDownListServerDest.SelectedIndexChanged += radDropDownListServerDest_SelectedIndexChanged;
                this.radDropDownListServerSource.SelectedIndexChanged += radDropDownListServerSource_SelectedIndexChanged;
                this.radDropDownListListType.SelectedIndexChanged += radDropDownListListType_SelectedIndexChanged;
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

        public override void SaveData()
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.Save();
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {

                new TechSun.Theme.frmWarning(ex.Message.ToString() + ex.StackTrace.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {

                new TechSun.Theme.frmWarning(ex.Message.ToString() + ex.StackTrace.ToString()).ShowDialog(this);
            }

        }
        void radDropDownListServerSource_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this.MethodBindingProjectSource();
        }

        void radDropDownListServerDest_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this.MethodBindingProjectDest();
        }
        private void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.ListType = this.radDropDownListListType.SelectedItem.DataBoundItem as EntityEDB_ListType;
            this.MethodBindingProjectSource();
            this.MethodBindingProjectDest();
        }

        public void MethodBindingServer(RadDropDownList p_dropDownList)
        {
            var urlList = typeof(EDCS.WF.DSL.WcfHelper).GetFields().Where(w => w.Name.Contains("Url") && !w.Name.Contains("DMCS") && !w.Name.Contains("Ensemble")).ToList();

            var dataTable = new DataTable();
            // Columns Setting
            dataTable.Columns.Add("Value", typeof(string));
            dataTable.Columns.Add("Display", typeof(string));
            foreach (var url in urlList)
            {
                dataTable.Rows.Add((string)url.GetValue(null), string.Format("{0} ({1})", url.Name, (string)url.GetValue(null)));
            }

            p_dropDownList.ValueMember = "Value";
            p_dropDownList.DisplayMember = "Display";
            p_dropDownList.DataSource = dataTable;
        }

        public void MethodBindingProjectSource()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerSource.SelectedValue);
            using (proxy as IDisposable)
            {
                var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(ob => ob.No).ToList();
                projects.RemoveAll(r => !r.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID));
                projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                this.radDropDownListProjectSource.ValueMember = "ProjectID";
                this.radDropDownListProjectSource.DisplayMember = "ComplexNameView";
                this.radDropDownListProjectSource.DataSource = projects;
            }
        }
        public void MethodBindingProjectDest()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerDest.SelectedValue);
            using (proxy as IDisposable)
            {
                var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(ob => ob.No).ToList();
                projects.RemoveAll(r => !r.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID));
                projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                this.radDropDownListProjectDest.ValueMember = "ProjectID";
                this.radDropDownListProjectDest.DisplayMember = "ComplexNameView";
                this.radDropDownListProjectDest.DataSource = projects;
            }
        }
        private void radTextBoxConnectionStringSource_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                this.MethodBindingProjectSource();
            }
        }
        private void radTextBoxConnectionStringDest_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 13)
            {
                this.MethodBindingProjectDest();
            }
        }
        private void MethodUpdateFirstProjectRowDynamicID(int[] p_sheetDynamicIDs)
        {
            //var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            //using (proxy as IDisposable)
            //{
            //    var rowDynamics = proxy.ReadRowDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProjectDest.SelectedValue), this._entityEDBInformation.ListType.ListTypeID, this._entityEDBInformation.ListType.DisciplineID, p_sheetDynamicIDs.ToList(), p_connectionString: this.radTextBoxConnectionStringDest.Text);
            //    if (rowDynamics.Count > 0)
            //    {
            //        var maxFirstProjectRowDynamicID = proxy.ReadRowDynamicMaxFirstProjectRowDynamicID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProjectDest.SelectedValue), this._entityEDBInformation.ListType.ListTypeID, p_connectionString: this.radTextBoxConnectionStringDest.Text);
            //        maxFirstProjectRowDynamicID++;
            //        //var maxFirstProjectRowDynamicID = rowDynamics.Max(m => m.FirstProjectRowDynamicID) + 1;
            //        StringBuilder sb = new StringBuilder();
            //        foreach (var rowDynamic in rowDynamics.Where(w => w.FirstProjectRowDynamicID == 0))
            //        {
            //            sb.AppendFormat("UPDATE EDB_ROwDynamic Set FirstProjectRowDynamicID = {0} where RowDynamicID = {1};"
            //                , maxFirstProjectRowDynamicID, rowDynamic.RowDynamicID);
            //            maxFirstProjectRowDynamicID++;
            //        }
            //        if (sb.Length > 0)
            //        {
            //            proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb, p_connectionString: this.radTextBoxConnectionStringDest.Text);
            //        }
            //    }
            //}
        }
        private string[] MethodGetSourceExcelTemplateList(int p_sourceCaseProjectID)
        {
            string connectionString = ConfigurationManager.ConnectionStrings["EDCSConnectionString"].ConnectionString;
            List<string[]> cs = new List<string[]>();
            foreach (var item in connectionString.Split(';'))
            {
                cs.Add(item.Split('='));
            }
            string dataSource = cs[0][1].Trim();
            string initialCatalog = cs[1][1].Trim();
            string userID = cs[2][1].Trim();
            string password = cs[3][1].Trim();
            //본서버
            bool flagDebug = true;
            if (dataSource.Equals(SharedVariable._serverUrl) && initialCatalog.Equals("EDCS"))
            {
                flagDebug = false;
            }

            var fileList = SharedMethod.MethodExcelTemplateGetFiles(true);
            //해당 Source ProjecTID 를 포함하는모든 파일 목록 가지고 옴.
            fileList = fileList.Where(w => !w.Contains("~$")
                && w.Contains(String.Format("Template_{0}_{1}_", this._entityEDBInformation.ListType.ListTypeID, p_sourceCaseProjectID))).ToArray();
            //테스트, 운영 체크 하여 해당하는 목록을 가지고 옴.
            if (flagDebug == true)
            {
                fileList = fileList.Where(w => w.Contains("Debug")).ToArray();
            }
            else
            {
                fileList = fileList.Where(w => !w.Contains("Debug")).ToArray();
            }

            return fileList;
        }
        private void Save()
        {
            var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(14, SharedVariable.ToolTip); // 생성 및 단계 수 설정
            frmPopupWaitingBarCallMethod.MethodSetStart(); // 시작
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Init"); // 단계 업데이트

            var newProject = (EntityEDB_Project)this.radDropDownListProjectDest.SelectedItem.DataBoundItem;
            var copyProject = (EntityEDB_Project)this.radDropDownListProjectSource.SelectedItem.DataBoundItem;
            var copyProjectID = copyProject.ProjectID;
            var newProjectID = newProject.ProjectID;
            var copyProjectMasterID = copyProject.ProjectMasterID;
            var newProjectMasterID = newProject.ProjectMasterID;
            int listTypeID = this._entityEDBInformation.ListType.ListTypeID;
            var oldSheetDynamicIDs = new List<int>();
            var oldDistrictDynamicIDs = new List<int>();
            var sheetDynamicIDs = new Dictionary<int, int>();
            var sheetDynamicIDsdesc = new Dictionary<int, int>();
            var caseDynamicIDs = new Dictionary<int, int>();
            var columnDynamicIDs = new Dictionary<int, int>();
            var equipmentTypeDynamicIDs = new Dictionary<int, int>();
            var subEquipmentTypeDynamicIDs = new Dictionary<int, int>();
            var multiPropertyIDs = new Dictionary<int, int>();

            var firstRowDynamicIDUpdateString = new StringBuilder();
            List<EntityEDB_RowDynamic> updateRowDynamics = null;
            frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Sheet"); // 단계 업데이트

            IEdcsDac proxyCopy = null;
            var proxyNew = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerDest.SelectedValue);
            using (proxyNew as IDisposable)
            {
                proxyCopy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerSource.SelectedValue);
                using (proxyCopy as IDisposable)
                {
                    var copyCaseDynamics = proxyCopy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    var copyColumnDynamicParents = proxyCopy.ReadColumnDynamicParents(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    var newCaseDynamics = copyCaseDynamics.Select(s => new EDB_CaseDynamic()
                    {
                        ListTypeID = listTypeID,
                        ProjectID = newProjectID,
                        VisibleFlag = s.VisibleFlag,
                        Orders = s.Orders,
                        Name = s.Name,
                        NameView = s.NameView,
                    }).ToList();
                    proxyNew.CreateCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newCaseDynamics, newProject.ProjectID, listTypeID);
                    foreach (var newCaseDynamic in newCaseDynamics)
                    {
                        caseDynamicIDs.Add(copyCaseDynamics.Where(w => w.Orders.Equals(newCaseDynamic.Orders)).SingleOrDefault().CaseDynamicID, newCaseDynamic.CaseDynamicID);
                    }

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy PBS"); // 단계 업데이트

                    var copyDistrictTypes = proxyCopy.ReadDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    var copyDistrictDynamics = proxyCopy.ReadDistrictDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    var newDistrictTypes = copyDistrictTypes
                        .Select(s => new EDB_DistrictType()
                        {
                            DistrictTypeID = s.DistrictTypeID,
                            ListTypeID = listTypeID,
                            ProjectID = newProjectID,
                            Orders = s.Orders,
                            Name = s.Name,
                            NameView = s.NameView,
                            RevisionUnit = s.RevisionUnit,
                        }).ToList();
                    foreach (var newDistrictType in newDistrictTypes)
                    {
                        var copyDistrictType = copyDistrictTypes.Where(w => w.DistrictTypeID.Equals(newDistrictType.DistrictTypeID)).SingleOrDefault();
                        //newDistrictType.DistrictTypeID = 0;
                        foreach (var districtDynamic in copyDistrictType.DistrictDynamics)
                        {
                            var newDistrictDynamic = new EDB_DistrictDynamic();
                            newDistrictDynamic.DistrictTypeID = 0;
                            newDistrictDynamic.DistrictDynamicID = 0;
                            //districtDynamic.ParentID = districtDynamic.DistrictDynamicParentID;
                            newDistrictDynamic.Orders = districtDynamic.Orders;
                            newDistrictDynamic.Name = districtDynamic.Name;
                            newDistrictDynamic.NameView = districtDynamic.NameView;

                            oldDistrictDynamicIDs.Add(districtDynamic.DistrictDynamicID);

                            if (!districtDynamic.ParentID.Equals(0))
                            {
                                //오류
                                var disctrictDynamic = copyDistrictDynamics.Where(w => w.DistrictDynamicID.Equals(districtDynamic.ParentID)).SingleOrDefault();
                                if (disctrictDynamic != null)
                                {
                                    newDistrictDynamic.ParentID = disctrictDynamic.Orders;
                                }
                            }
                            if (districtDynamic.SheetDynamics != null)
                            {
                                foreach (var sheetDynamic in districtDynamic.SheetDynamics)
                                {
                                    List<int> hideInfo = null;
                                    if (sheetDynamic.CaseDynamicHideInfo != null)
                                    {
                                        hideInfo = XmlConverter.DeserializeObject<List<int>>(sheetDynamic.CaseDynamicHideInfo);
                                        if (hideInfo != null)
                                        {
                                            for (int i = 0; i < hideInfo.Count; i++)
                                            {
                                                hideInfo[i] = caseDynamicIDs[hideInfo[i]];
                                            }
                                        }
                                    }
                                    newDistrictDynamic.EDB_SheetDynamic.Add(new EDB_SheetDynamic()
                                    {
                                        SheetDynamicID = sheetDynamic.SheetDynamicID,
                                        DistrictDynamicID = sheetDynamic.DistrictDynamicID,
                                        Orders = sheetDynamic.Orders,
                                        Name = sheetDynamic.Name,
                                        NameView = sheetDynamic.NameView,
                                        CaseDynamicHideInfo = hideInfo != null ? XmlConverter.SerializeObject<List<int>>(hideInfo) : null,
                                    });
                                    oldSheetDynamicIDs.Add(sheetDynamic.SheetDynamicID);
                                }
                            }
                            newDistrictType.EDB_DistrictDynamic.Add(newDistrictDynamic);
                        }
                    }
                    newDistrictTypes.ConvertAll(ca => ca.DistrictTypeID = 0);
                    #region ProcessEquipmentList 일때 HazardousSourceList 에 PBS 복사 :: 단, 초기 한번 만!!!
                    if (listTypeID == (int)EnumNameCheck.ProcessEquipmentList)
                    {
                        List<EntityEDB_DistrictType> simpleDistrictTypes = null;
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxyNew.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, (int)EnumNameCheck.HazardousSourceList));
                        if (simpleDistrictTypes.Count == 0)
                        {
                            var newDistrictTypesCopy = StaticMethod.GenericCopier<List<EDB_DistrictType>>.DeepCopy(newDistrictTypes);
                            newDistrictTypesCopy.ConvertAll(ca => ca.ListTypeID = (int)EnumNameCheck.HazardousSourceList);
                            proxyNew.CreateDistrictType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newDistrictTypesCopy, newProjectID, (int)EnumNameCheck.HazardousSourceList);
                        }
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxyNew.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, (int)EnumNameCheck.BuildingList));
                        if (simpleDistrictTypes.Count == 0)
                        {
                            var newDistrictTypesCopy = StaticMethod.GenericCopier<List<EDB_DistrictType>>.DeepCopy(newDistrictTypes);
                            newDistrictTypesCopy.ConvertAll(ca => ca.ListTypeID = (int)EnumNameCheck.BuildingList);
                            proxyNew.CreateDistrictType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newDistrictTypesCopy, newProjectID, (int)EnumNameCheck.BuildingList);
                        }
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxyNew.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, (int)EnumNameCheck.StructuralEquipmentList));
                        if (simpleDistrictTypes.Count == 0)
                        {
                            var newDistrictTypesCopy = StaticMethod.GenericCopier<List<EDB_DistrictType>>.DeepCopy(newDistrictTypes);
                            newDistrictTypesCopy.ConvertAll(ca => ca.ListTypeID = (int)EnumNameCheck.StructuralEquipmentList);
                            proxyNew.CreateDistrictType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newDistrictTypesCopy, newProjectID, (int)EnumNameCheck.StructuralEquipmentList);
                        }
                        
                        
                    } 
                    #endregion
                    proxyNew.CreateDistrictType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newDistrictTypes, newProjectID, listTypeID);


                    var districtTypes = proxyNew.ReadDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, listTypeID);

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Template Files"); // 단계 업데이트
                    //파일은 추후에 경로 관련되서 손봐야됨.
                    //var defaultSourceFileName = String.Format("Template_{0}_{1}_", this._entityEDBInformation.ListType.ListTypeID, copyProjectID);
                    //var defaultDestFileName = String.Format("Template_{0}_{1}_", this._entityEDBInformation.ListType.ListTypeID, newProjectID);

                    //proxyNew.MethodGetFiles(

                    //파일목록을 함수에서 가지고 오자.
                    //var fileList = this.MethodGetSourceExcelTemplateList(sourceProjectID);
                    //var excelExtension = ".xls";
                    //if (this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.ElectricalEquipmentList)))
                    //{
                    //    excelExtension = ".xlsx";
                    //}

                    //var projectExcelFile = fileList.Where(w => Path.GetFileName(w) == defaultSourceFileName + excelExtension).SingleOrDefault();
                    //if (projectExcelFile != null)
                    //{
                    //    File.Copy(projectExcelFile, Path.Combine(Path.GetDirectoryName(projectExcelFile), defaultDestFileName + excelExtension), true);
                    //}
                    //var projectXmlFile = fileList.Where(w => Path.GetFileName(w) == defaultSourceFileName + ".xml").SingleOrDefault();
                    //if (projectXmlFile != null)
                    //{
                    //    File.Copy(projectXmlFile, Path.Combine(Path.GetDirectoryName(projectXmlFile), defaultDestFileName + ".xml"), true);
                    //}

                    //System.IO.File.Copy(projectFile, System.IO.Path.Combine(System.IO.Path.GetDirectoryName(projectFile), defaultDestFileName + excelExtension));
                    //System.String.Format("Template_{0}_{1}_", this._entityFrmCopyProject.ListType.ListTypeID, p_sourceCaseProjectID)

                    //fileList
                    var index = 0;
                    var districtDynamicIndex = 0;
                    foreach (var newDistrictType in districtTypes)
                    {
                        foreach (var districtDynamic in newDistrictType.DistrictDynamics)
                        {
                            //x템플릿 복사를 합시다.
                            //defaultSourceFileName = String.Format("Template_{0}_{1}_{2}", this._entityEDBInformation.ListType.ListTypeID, sourceProjectID, oldDistrictDynamicIDs[districtDynamicIndex]);
                            //defaultDestFileName = String.Format("Template_{0}_{1}_{2}", this._entityEDBInformation.ListType.ListTypeID, sourceProjectID, districtDynamic.DistrictDynamicID);

                            //projectExcelFile = fileList.Where(w => Path.GetFileName(w) == defaultSourceFileName + excelExtension).SingleOrDefault();
                            //if (projectExcelFile != null)
                            //{
                            //    File.Copy(projectExcelFile, Path.Combine(Path.GetDirectoryName(projectExcelFile), defaultDestFileName + excelExtension));
                            //}
                            //projectXmlFile = fileList.Where(w => Path.GetFileName(w) == defaultSourceFileName + ".xml").SingleOrDefault();
                            //if (projectXmlFile != null)
                            //{
                            //    File.Copy(projectXmlFile, Path.Combine(Path.GetDirectoryName(projectXmlFile), defaultDestFileName + ".xml"));
                            //}
                            districtDynamicIndex++;
                            foreach (var sheetDynamic in districtDynamic.SheetDynamics)
                            {
                                sheetDynamicIDs.Add(oldSheetDynamicIDs[index], sheetDynamic.SheetDynamicID);
                                index++;
                            }
                        }
                    }

                    // DistrictDynamic districtType sheetDynamic master 에 id 넣음.
                    proxyNew.UpdateMasterAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE);


                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Tag Validation Rule"); // 단계 업데이트
                    var copyValidationDynamics = proxyCopy.ReadValidationDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    var newValidationDynamics = copyValidationDynamics.Select(s => new EDB_ValidationDynamic()
                                                {
                                                    ProjectID = newProjectID,
                                                    ListTypeID = listTypeID,
                                                    ColumnLabel = s.ColumnLabel,
                                                    Regex = s.Regex,
                                                    NumberFlag = s.NumberFlag,
                                                    AllowNull = s.AllowNull,
                                                    Exception = s.Exception,
                                                    MainColorRealTimeFlag = s.MainColorRealTimeFlag,
                                                }).ToList();
                    proxyNew.CreateValidationDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newValidationDynamics, newProjectID, listTypeID);


                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Tag Naming Rule"); // 단계 업데이트
                    //TagNamingRule
                    //Master 가 추가되면서 다시 생각해보자.
                    var copyTagNamingRuleDynamics = proxyCopy.ReadTagNamingRuleDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectMasterID, listTypeID);
                    if (copyTagNamingRuleDynamics.Count > 0)
                    {
                        var newTagNamingRuleDynamics = copyTagNamingRuleDynamics
                            .Select(s => new EDB_TagNamingRuleDynamic()
                            {
                                Label = s.Label,
                                ProjectMasterID = newProjectMasterID,
                                ListTypeID = listTypeID,
                                Type = s.Type,
                                ColumnName = s.ColumnName
                            }).ToList();
                        proxyNew.CreateTagNamingRuleDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newTagNamingRuleDynamics, newProjectMasterID, listTypeID);
                    }

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Attribute"); // 단계 업데이트
                    var oldColumnDynamicIDs = new List<int>();
                    index = 0;
                    foreach (var copyColumnDynamicParent in copyColumnDynamicParents)
                    {
                        var newColumnParentDynamic = new EDB_ColumnDynamicParent();
                        newColumnParentDynamic.CaseDynamicID = caseDynamicIDs[copyColumnDynamicParent.CaseDynamicID];
                        newColumnParentDynamic.ColumnHeaderMergeProperty = copyColumnDynamicParent.ColumnHeaderMergeProperty;
                        foreach (var columnDynamic in copyColumnDynamicParent.ColumnDynamics)
                        {
                            var specSizeOrder = 0;
                            newColumnParentDynamic.EDB_ColumnDynamic.Add(new EDB_ColumnDynamic()
                            {
                                CommonUnitID = columnDynamic.CommonUnitID,
                                VisibleFlag = columnDynamic.VisibleFlag,
                                Orders = columnDynamic.Orders,
                                ColumnIndex = columnDynamic.ColumnIndex,
                                Label = columnDynamic.Label,
                                LabelView = columnDynamic.LabelView,
                                CellType = columnDynamic.CellType,
                                Locked = columnDynamic.Locked,
                                Width = columnDynamic.Width,
                                HorizontalAlignment = columnDynamic.HorizontalAlignment,
                                SpecSizeFlag = columnDynamic.SpecSizeFlag,
                                SpecSizeOrder = columnDynamic.SpecSizeFlag == true ? ++specSizeOrder : 0,
                                OwnershipDisciplineID = columnDynamic.OwnershipDisciplineID,
                            });
                            oldColumnDynamicIDs.Add(columnDynamic.ColumnDynamicID);
                        }

                        proxyNew.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newColumnParentDynamic, newProjectID, listTypeID, null);
                        var columnDynamicParent = proxyNew.ReadColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, listTypeID, caseDynamicIDs[copyColumnDynamicParent.CaseDynamicID]);
                        foreach (var columnDynamic in columnDynamicParent.ColumnDynamics)
                        {
                            columnDynamicIDs.Add(oldColumnDynamicIDs[index], columnDynamic.ColumnDynamicID);
                            index++;
                        }
                    }

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy EquipmentType"); // 단계 업데이트

                    var copyEquipmentTypeDynamics = proxyCopy.ReadEquipmentTypeDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    List<EntityEDB_MultiProperty> copyMultiProperties = null;
                    if (listTypeID == (int)EnumNameCheck.PowerEquipmentList)
                    {
                        copyMultiProperties = proxyCopy.ReadMultiProperties(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID, p_powerEquipment: true);
                    }
                    else
                    {
                        copyMultiProperties = proxyCopy.ReadMultiProperties(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    }
                    //copyEquipmentTypeDynamics = copyEquipmentTypeDynamics.Where(w => w.ParentID)).ToList();

                    List<EDB_EquipmentTypeDynamic> newEquipmentTypeDynamics = new List<EDB_EquipmentTypeDynamic>();
                    foreach (var copyEquipmentType in copyEquipmentTypeDynamics.Where(w => w.ParentID.Equals(0)))
                    {
                        EDB_EquipmentTypeDynamic newEquipmentTypeDynamic = new EDB_EquipmentTypeDynamic();

                        newEquipmentTypeDynamic.ProjectID = newProjectID;
                        newEquipmentTypeDynamic.ListTypeID = listTypeID;
                        newEquipmentTypeDynamic.Orders = copyEquipmentType.Orders;
                        newEquipmentTypeDynamic.Name = copyEquipmentType.Name;
                        newEquipmentTypeDynamic.NameView = copyEquipmentType.NameView;
                        newEquipmentTypeDynamic.ParentID = copyEquipmentType.ParentID;
                        newEquipmentTypeDynamic.GroupCode = copyEquipmentType.GroupCode;
                        if (copyEquipmentType.CaseDynamicID.HasValue == false)
                        {
                            newEquipmentTypeDynamic.CaseDynamicID = null;
                        }
                        else
                        {
                            if (caseDynamicIDs.ContainsKey(copyEquipmentType.CaseDynamicID.Value))
                            {
                                newEquipmentTypeDynamic.CaseDynamicID = (int?)caseDynamicIDs[copyEquipmentType.CaseDynamicID.Value];
                            }
                            //mis pac int 
                            else
                            {
                                var copyCase = copyCaseDynamics.Where(w => w.Name == "int" || w.Name == "mis" || w.Name == "pac").OrderBy(ob => ob.Name).FirstOrDefault();
                                if (copyCase == null)
                                {
                                }
                                else
                                {
                                    newEquipmentTypeDynamic.CaseDynamicID = (int?)caseDynamicIDs[copyCase.CaseDynamicID];
                                }
                            }
                        }
                        newEquipmentTypeDynamics.Add(newEquipmentTypeDynamic);
                    }
                    proxyNew.CreateEquipmentTypeDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  newEquipmentTypeDynamics, newProjectID, listTypeID);

                    foreach (var newEquipmentTypeDynamic in newEquipmentTypeDynamics)
                    {
                        equipmentTypeDynamicIDs.Add(copyEquipmentTypeDynamics.Where(w => w.ParentID.Equals(0) && w.Orders.Equals(newEquipmentTypeDynamic.Orders)).SingleOrDefault().EquipmentTypeDynamicID, newEquipmentTypeDynamic.EquipmentTypeDynamicID);
                    }

                    List<EDB_EquipmentTypeDynamic> newEquipmentTypeSubDynamics = new List<EDB_EquipmentTypeDynamic>();
                    foreach (var copyEquipmentType in copyEquipmentTypeDynamics.Where(w => !w.ParentID.Equals(0)))
                    {
                        EDB_EquipmentTypeDynamic newEquipmentTypeDynamic = new EDB_EquipmentTypeDynamic();

                        newEquipmentTypeDynamic.ProjectID = newProjectID;
                        newEquipmentTypeDynamic.ListTypeID = listTypeID;
                        newEquipmentTypeDynamic.Orders = copyEquipmentType.Orders;
                        newEquipmentTypeDynamic.Name = copyEquipmentType.Name;
                        newEquipmentTypeDynamic.NameView = copyEquipmentType.NameView;
                        newEquipmentTypeDynamic.ParentID = equipmentTypeDynamicIDs[copyEquipmentType.ParentID];
                        newEquipmentTypeDynamic.GroupCode = copyEquipmentType.GroupCode;
                        if (copyEquipmentType.CaseDynamicID.HasValue == false)
                        {
                            newEquipmentTypeDynamic.CaseDynamicID = null;
                        }
                        else
                        {
                            if (caseDynamicIDs.ContainsKey(copyEquipmentType.CaseDynamicID.Value))
                            {
                                newEquipmentTypeDynamic.CaseDynamicID = (int?)caseDynamicIDs[copyEquipmentType.CaseDynamicID.Value];
                            }
                            //mis pac int 
                            else
                            {

                                var copyCase = copyCaseDynamics.Where(w => w.Name == "int" || w.Name == "mis" || w.Name == "pac").OrderBy(ob => ob.Name).FirstOrDefault();
                                if (copyCase == null)
                                {
                                }
                                else
                                {
                                    newEquipmentTypeDynamic.CaseDynamicID = (int?)caseDynamicIDs[copyCase.CaseDynamicID];
                                }
                            }
                        }
                        newEquipmentTypeSubDynamics.Add(newEquipmentTypeDynamic);
                    }
                    //var newEquipmentTypeSubDynamics = copyEquipmentTypeDynamics
                    //    .Where(w => !w.ParentID.Equals(0))
                    //    .Select(s => new EDB_EquipmentTypeDynamic()
                    //    {
                    //        ProjectID = projectID,
                    //        ListTypeID = listTypeID,
                    //        Orders = s.Orders,
                    //        Name = s.Name,
                    //        NameView = s.NameView,
                    //        ParentID = equipmentTypeDynamicIDs[s.ParentID],
                    //        GroupCode = s.GroupCode,
                    //        CaseDynamicID = s.CaseDynamicID.HasValue == false ? null : (caseDynamicIDs.ContainsKey(s.CaseDynamicID.Value) ? (int?)caseDynamicIDs[s.CaseDynamicID.Value] : null),
                    //    }).ToList();
                    proxyNew.CreateEquipmentTypeDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newEquipmentTypeSubDynamics, newProject.ProjectID, listTypeID, true);

                    if (listTypeID == (int)EnumNameCheck.ElectricalEquipmentList)
                    {
                        foreach (var newEquipmentTypeSubDynamic in newEquipmentTypeSubDynamics)
                        {
                            subEquipmentTypeDynamicIDs.Add(copyEquipmentTypeDynamics.Where(w => !w.ParentID.Equals(0) && w.Orders.Equals(newEquipmentTypeSubDynamic.Orders)).SingleOrDefault().EquipmentTypeDynamicID, newEquipmentTypeSubDynamic.EquipmentTypeDynamicID);
                        }
                    }


                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy MultiProperty"); // 단계 업데이트

                    copyMultiProperties = copyMultiProperties.Where(w => w.CaseDynamicID == null || caseDynamicIDs.ContainsKey(w.CaseDynamicID.Value)).ToList();

                    var newMultiProperties = new List<EDB_MultiProperty>();
                    if (listTypeID == (int)EnumNameCheck.ElectricalEquipmentList)// 전기는 Sub Equipment Type에 따라서 MultiProperty가 바뀜
                    {
                        copyMultiProperties = copyMultiProperties.Where(w => w.EquipmentTypeDynamicID == null || subEquipmentTypeDynamicIDs.ContainsKey(w.EquipmentTypeDynamicID.Value)).ToList();
                        newMultiProperties = copyMultiProperties
                            .Select(s => new EDB_MultiProperty()
                            {
                                ProjectID = s.ProjectID.HasValue ? (int?)newProjectID : null,
                                CaseDynamicID = s.CaseDynamicID.HasValue ? (int?)caseDynamicIDs[s.CaseDynamicID.Value] : null,
                                EquipmentTypeDynamicID = s.EquipmentTypeDynamicID.HasValue ? (int?)subEquipmentTypeDynamicIDs[s.EquipmentTypeDynamicID.Value] : null,
                                Orders = s.Orders,
                                Name = s.Name,
                                NameView = s.NameView,
                                OrderCheck = s.OrderCheck,
                                UOM = s.UOM,
                                IsExcel = s.IsExcel,
                                IsBQ = s.IsBQ,
                                Lvl = s.Level,
                                Code = s.Code,
                            }).ToList();
                    }
                    else
                    {
                        copyMultiProperties = copyMultiProperties.Where(w => w.EquipmentTypeDynamicID == null || equipmentTypeDynamicIDs.ContainsKey(w.EquipmentTypeDynamicID.Value)).ToList();
                        newMultiProperties = copyMultiProperties
                            .Select(s => new EDB_MultiProperty()
                            {
                                ProjectID = s.ProjectID.HasValue ? (int?)newProjectID : null,
                                CaseDynamicID = s.CaseDynamicID.HasValue ? (int?)caseDynamicIDs[s.CaseDynamicID.Value] : null,
                                EquipmentTypeDynamicID = s.EquipmentTypeDynamicID.HasValue ? (int?)equipmentTypeDynamicIDs[s.EquipmentTypeDynamicID.Value] : null,
                                Orders = s.Orders,
                                Name = s.Name,
                                NameView = s.NameView,
                                OrderCheck = s.OrderCheck,
                                UOM = s.UOM,
                                IsExcel = s.IsExcel,
                                IsBQ = s.IsBQ,
                                Lvl = s.Level,
                                Code = s.Code,
                            }).ToList();
                    }


                    if (listTypeID == (int)EnumNameCheck.PowerEquipmentList)
                    {
                        proxyNew.CreateMultiProperty(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newMultiProperties, newProjectID, listTypeID, p_powerEquipment: true);
                    }
                    else
                    {
                        proxyNew.CreateMultiProperty(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newMultiProperties, newProjectID, listTypeID);
                    }

                    index = 0;
                    foreach (var newMultiProperty in newMultiProperties)
                    {
                        multiPropertyIDs.Add(copyMultiProperties[index].MultiPropertyID, newMultiProperties[index].MultiPropertyID);
                        index++;
                    }

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Attribute DropDownList"); // 단계 업데이트
                    var copyColumnDynamicDropDownList = proxyCopy.ReadColumnDynamicDropDownList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProject.ProjectID, listTypeID);
                    var newColumnDynamicDropDownList = copyColumnDynamicDropDownList
                        .Select(s => new EDB_ColumnDynamicDropDownList()
                        {
                            ProjectID = newProject.ProjectID,
                            ListTypeID = listTypeID,
                            ColumnDynamicLabel = s.ColumnDynamicLabel,
                            Orders = s.Orders,
                            Name = s.Name,
                            NameView = s.NameView,
                        }).ToList();
                    proxyNew.CreateColumnDynamicDropDownList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newColumnDynamicDropDownList, newProject.ProjectID, listTypeID);

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy RowDynamic"); // 단계 업데이트
                    var newFirstRowDynamicIDs = new List<int>();
                    var copyFirstRowDynamicIDs = new List<int>();
                    var firstRowDynamicIDs = new Dictionary<int, int>();


                    var copyRowDynamics = proxyCopy.ReadRowDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID, listTypeID);
                    foreach (var disciplines in copyRowDynamics.Where(w => w.DisciplineID == this._entityEDBInformation.ListType.DisciplineID)
                        .GroupBy(gb => gb.DisciplineID).OrderBy(ob => ob.Key))
                    {
                        foreach (var rowDynamics in disciplines.GroupBy(gb => gb.SheetDynamicID).OrderBy(ob => ob.Key))
                        {
                            var newRowDynamics = new List<EDB_RowDynamic>();
                            if (listTypeID == (int)EnumNameCheck.ElectricalEquipmentList) // 전기는 Sub Equipment Type에 따라서 MultiProperty가 바뀜
                            {
                                newRowDynamics = rowDynamics
                                .Select(s => new EDB_RowDynamic()
                                {
                                    SheetDynamicID = sheetDynamicIDs[s.SheetDynamicID],
                                    CaseDynamicID = caseDynamicIDs[s.CaseDynamicID],
                                    DisciplineID = s.DisciplineID,
                                    EquipmentTypeDynamicID = s.EquipmentTypeDynamicID.HasValue ? (int?)subEquipmentTypeDynamicIDs[s.EquipmentTypeDynamicID.Value] : null,
                                    OriginDisciplineID = s.OriginDisciplineID,
                                    RowIndex = s.RowIndex,
                                    RowHeight = s.RowHeight,
                                    DeleteItemTag = s.DeleteItemTag,
                                    FirstRowDynamicID = 0,
                                    FirstProjectRowDynamicID = 0,
                                }).ToList();
                            }
                            else
                            {
                                newRowDynamics = rowDynamics
                                .Select(s => new EDB_RowDynamic()
                                {
                                    SheetDynamicID = sheetDynamicIDs[s.SheetDynamicID],
                                    CaseDynamicID = caseDynamicIDs[s.CaseDynamicID],
                                    DisciplineID = s.DisciplineID,
                                    EquipmentTypeDynamicID = s.EquipmentTypeDynamicID.HasValue ? (int?)equipmentTypeDynamicIDs[s.EquipmentTypeDynamicID.Value] : null,
                                    OriginDisciplineID = s.OriginDisciplineID,
                                    RowIndex = s.RowIndex,
                                    RowHeight = s.RowHeight,
                                    DeleteItemTag = s.DeleteItemTag,
                                    FirstRowDynamicID = 0,
                                    FirstProjectRowDynamicID = 0,
                                }).ToList();
                            }
                            copyFirstRowDynamicIDs.AddRange(rowDynamics.OrderBy(ob => ob.CaseDynamicID).ThenBy(t => t.RowIndex).Select(s => s.FirstRowDynamicID));

                            proxyNew.CreateRowDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newRowDynamics, new List<int>() { sheetDynamicIDs[rowDynamics.Key] }, caseDynamicIDs.Select(s => s.Value).Distinct().ToList(), disciplines.Key);
                        }
                    }

                    StringBuilder sb = new StringBuilder();
                    foreach (var caseDynamicID in newCaseDynamics.Select(s => s.CaseDynamicID))
                    {
                        sb.AppendFormat("Update EDB_RowDynamic Set FirstRowDynamicID = RowDynamicID where CaseDynamicId = {0}", caseDynamicID);
                    }
                    if (sb.Length > 0)
                    {
                        proxyNew.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                    }
                    updateRowDynamics = proxyNew.ReadRowDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, listTypeID);
                    foreach (var disciplines in copyRowDynamics.Where(w => w.DisciplineID == this._entityEDBInformation.ListType.DisciplineID)
                        .GroupBy(gb => gb.DisciplineID).OrderBy(ob => ob.Key))
                    {
                        foreach (var rowDynamics in disciplines.GroupBy(gb => gb.SheetDynamicID).OrderBy(ob => ob.Key))
                        {
                            newFirstRowDynamicIDs.AddRange(rowDynamics.OrderBy(ob => ob.CaseDynamicID).ThenBy(t => t.RowIndex).Select(s => s.FirstRowDynamicID));
                        }
                    }

                    for (int i = 0; i < copyFirstRowDynamicIDs.Count; i++)
                    {
                        firstRowDynamicIDs.Add(copyFirstRowDynamicIDs[i], newFirstRowDynamicIDs[i]);

                    }
                    var maxFirstProjectRowDynamicID = 1;
                    sb.Clear();
                    foreach (var rowDynamic in updateRowDynamics.Where(w => w.FirstProjectRowDynamicID == 0))
                    {
                        sb.AppendFormat("UPDATE EDB_ROwDynamic Set FirstProjectRowDynamicID = {0} where DEleteFlag = 0 and RowDynamicID = {1};"
                            , maxFirstProjectRowDynamicID, rowDynamic.RowDynamicID);
                        maxFirstProjectRowDynamicID++;
                    }
                    if (sb.Length > 0)
                    {
                        proxyNew.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                    }

                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy SpreadSheetInfo"); // 단계 업데이트
                    var copySpreadSheetInfoDynamics = proxyCopy.ReadSpreadSheetInfoDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProject.ProjectID, listTypeID);
                    foreach (var spreadSheetInfoDynamics in copySpreadSheetInfoDynamics.GroupBy(gb => gb.SheetDynamicID))
                    {
                        var newSpreadSheetInfoDynamics = spreadSheetInfoDynamics
                                  .Select(s => new EDB_SpreadSheetInfoDynamic
                                  {
                                      SheetDynamicID = sheetDynamicIDs[s.SheetDynamicID],
                                      CaseDynamicID = caseDynamicIDs[s.CaseDynamicID],
                                      GroupInfo = s.GroupInfo,
                                      HideInfo = s.HideInfo,
                                      FreezeInfo = s.FreezeInfo
                                  }).ToList();
                        proxyNew.CreateSpreadSheetInfoDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newSpreadSheetInfoDynamics, new List<int>() { sheetDynamicIDs[spreadSheetInfoDynamics.Key] });
                    }
                }
            }
            proxyNew = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerDest.SelectedValue);
            using (proxyNew as IDisposable)
            {
                proxyCopy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_url: (string)this.radDropDownListServerSource.SelectedValue);
                using (proxyCopy as IDisposable)
                {
                    frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy CellDynamic"); // 단계 업데이트
                    //cellDynamic
                    var copyCellDynamics = proxyCopy.ReadCellDynamicsByDisciplineID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProject.ProjectID, listTypeID);
                    var specSizeCellDynamics = copyCellDynamics.Where(w => w.CellType.Equals(100));
                    //multiproperty 설정
                    foreach (var specSizeCellDynamic in specSizeCellDynamics)
                    {
                        var specSizeValues = XmlConverter.DeserializeObject<List<ESpec>>(specSizeCellDynamic.Value);
                        foreach (var specSizeValue in specSizeValues)
                        {
                            if (multiPropertyIDs.Where(w => w.Key.Equals(specSizeValue.M)).Count() > 0)
                            {
                                specSizeValue.M = multiPropertyIDs[specSizeValue.M];
                            }
                        }
                        specSizeCellDynamic.Value = XmlConverter.SerializeObject<List<EDCS.WF.BSL.Entities.ESpec>>(specSizeValues);
                    }
                    foreach (var cellDynamicDiscipline in copyCellDynamics.Where(w => w.DisciplineID == this._entityEDBInformation.ListType.DisciplineID)
                        .GroupBy(gb => gb.DisciplineID))
                    {
                        foreach (var cellDynamics in cellDynamicDiscipline.GroupBy(gb => gb.SheetDynamicID))
                        {
                            var newCellDynamics = cellDynamics
                                      .Select(s => new EDB_CellDynamic
                                      {
                                          SheetDynamicID = sheetDynamicIDs[cellDynamics.Key],
                                          CaseDynamicID = caseDynamicIDs[s.CaseDynamicID],
                                          ColumnDynamicID = columnDynamicIDs[s.ColumnDynamicID],
                                          DisciplineID = cellDynamicDiscipline.Key,
                                          OriginDisciplineID = s.OriginDisciplineID,
                                          FirstRowDynamicID = s.FirstRowDynamicID,
                                          //OriginVersionDynamicParentID = s.OriginVersionDynamicParentID,
                                          EmployeeID = s.EmployeeID,
                                          RowIndex = s.RowIndex,
                                          FirstRowIndex = s.FirstRowIndex,
                                          Value = s.Value,
                                          Formula = s.Formula,
                                          Note = s.Note,
                                          FontName = s.FontName,
                                          FontSize = s.FontSize,
                                          FontItalic = s.FontItalic,
                                          FontBold = s.FontBold,
                                          FontUnderline = s.FontUnderline,
                                          FontStrikeout = s.FontStrikeout,
                                          BackColorBlue = s.BackColorBlue,
                                          BackColorGreen = s.BackColorGreen,
                                          BackColorRed = s.BackColorRed,
                                          ForeColorRed = s.ForeColorRed,
                                          ForeColorBlue = s.ForeColorBlue,
                                          ForeColorGreen = s.ForeColorGreen,
                                          BorderBottom = s.BorderBottom,
                                          BorderLeft = s.BorderLeft,
                                          BorderRight = s.BorderRight,
                                          BorderTop = s.BorderTop,
                                          VerticalAlignment = s.VerticalAlignment,
                                          HorizontalAlignment = s.HorizontalAlignment,
                                          TextIndent = s.TextIndent,
                                          //UserUpdate = s.UserUpdate,
                                          IsKeyin = s.IsKeyin,
                                          WordWrap = s.WordWrap
                                      }).ToList();
                            byte[] buffer = WcfHelper.GetBytesFromObject(newCellDynamics, true);
                            proxyNew.CreateCellDynamicCompression(EDCS.WF.BSL.Variable.SharedVariable.IsIE, buffer, new List<int>() { sheetDynamicIDs[cellDynamics.Key] }, new List<int>() { }, cellDynamicDiscipline.Key);
                        }
                    }

                    //cell 의 firstRowDynamicID 를 채우기 전에 Row 와 versionRow 의 FirstRowDynamcID 를 정비해야됨.매우중요 ☆☆☆☆☆☆
                    updateRowDynamics = proxyNew.ReadRowDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID, listTypeID);
                    foreach (var rowDynamic in updateRowDynamics)
                    {
                        firstRowDynamicIDUpdateString.AppendFormat("UPDATE EDB_CellDynamic Set FirstRowDynamicID = {0} Where DeleteFlag = 0 and SheetDynamicID = {1} and CaseDynamicID = {2} and DisciplineID = {3} and RowIndex = {4};"
                                    , rowDynamic.FirstRowDynamicID, rowDynamic.SheetDynamicID, rowDynamic.CaseDynamicID, rowDynamic.DisciplineID, rowDynamic.RowIndex);
                    }
                    proxyNew.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, firstRowDynamicIDUpdateString);
                    firstRowDynamicIDUpdateString.Clear();


                    if (listTypeID == (int)EnumNameCheck.PowerEquipmentList)
                    {
                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy System Mapping"); // 단계 업데이트

                        var SystemMappings = proxyCopy.ReadPowerEquipmentSystemMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProject.ProjectID);
                        if (SystemMappings.Count > 0)
                        {
                            var powerEquipmentSystemMappings = new List<EDB_PowerEquipmentSystemMapping>();
                            foreach (var systemMapping in SystemMappings)
                            {
                                var newPowerEquipmentSystemMapping = new EDB_PowerEquipmentSystemMapping();
                                newPowerEquipmentSystemMapping.CommonSystemDynamicID = systemMapping.CommonSystemDynamicID;
                                newPowerEquipmentSystemMapping.ProjectID = newProjectID;
                                newPowerEquipmentSystemMapping.Orders = systemMapping.Orders;
                                newPowerEquipmentSystemMapping.ModifiedDate = DateTime.Now;
                                newPowerEquipmentSystemMapping.Name = systemMapping.Name;
                                newPowerEquipmentSystemMapping.NameView = systemMapping.NameView;

                                powerEquipmentSystemMappings.Add(newPowerEquipmentSystemMapping);
                            }

                            proxyNew.CreatePowerEquipmentSystemMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, powerEquipmentSystemMappings, newProjectID);
                        }

                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Service"); // 단계 업데이트
                        // 서비스 복사
                        var newPowerEquipmentServices = new List<EDB_PowerEquipmentService>();

                        var EquipmentServices = proxyCopy.ReadPowerEquipmentServices(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID);
                        if (EquipmentServices.Count > 0)
                        {
                            foreach (var service in EquipmentServices)
                            {
                                var powerEquipmentService = new EDB_PowerEquipmentService();
                                powerEquipmentService.CommonSystemDynamicID = service.CommonSystemDynamicID;
                                powerEquipmentService.ProjectID = newProjectID;
                                powerEquipmentService.Orders = service.Orders;
                                powerEquipmentService.ModifiedDate = DateTime.Now;
                                powerEquipmentService.Name = service.Name;
                                powerEquipmentService.NameView = service.NameView;
                                newPowerEquipmentServices.Add(powerEquipmentService);
                            }
                            proxyNew.CreatePowerEquipmentService(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newPowerEquipmentServices, newProjectID);
                        }
                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy Service MApping"); // 단계 업데이트

                        // 서비스 맵핑 복사
                        var ServiceMappings = proxyCopy.ReadPowerEquipmentServiceMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, copyProjectID);
                        if (ServiceMappings.Count > 0)
                        {
                            Dictionary<int, int> equipmentServiceIDs = new Dictionary<int, int>();
                            var newEquipmentService = proxyNew.ReadPowerEquipmentServices(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, newProjectID);
                            newEquipmentService = newEquipmentService.OrderBy(ob => ob.CommonSystemDynamicID).ThenBy(tb => tb.Orders).ToList();
                            EquipmentServices = EquipmentServices.OrderBy(ob => ob.CommonSystemDynamicID).ThenBy(tb => tb.Orders).ToList();
                            for (int i = 0; i < newEquipmentService.Count; i++)
                            {
                                equipmentServiceIDs.Add(EquipmentServices[i].PowerEquipmentServiceID, newEquipmentService[i].PowerEquipmentServiceID);
                            }

                            var powerEquipmentServiceMappings = new List<EDB_PowerEquipmentServiceMapping>();
                            foreach (var serviceMapping in ServiceMappings)
                            {
                                if (equipmentServiceIDs.ContainsKey(serviceMapping.PowerEquipmentServiceID))
                                {
                                    var powerEquipmentServiceMapping = new EDB_PowerEquipmentServiceMapping();
                                    powerEquipmentServiceMapping.PowerEquipmentServiceID = equipmentServiceIDs[serviceMapping.PowerEquipmentServiceID];
                                    powerEquipmentServiceMapping.ProjectID = newProjectID;
                                    powerEquipmentServiceMapping.Orders = serviceMapping.Orders;
                                    powerEquipmentServiceMapping.ModifiedDate = DateTime.Now;
                                    powerEquipmentServiceMapping.Name = serviceMapping.Name;
                                    powerEquipmentServiceMapping.NameView = serviceMapping.NameView;
                                    powerEquipmentServiceMappings.Add(powerEquipmentServiceMapping);
                                }
                            }
                            proxyNew.CreatePowerEquipmentServiceMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, powerEquipmentServiceMappings, newProjectID);
                        }
                    }
                    else
                    {
                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText(""); // 단계 업데이트
                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText(""); // 단계 업데이트
                        frmPopupWaitingBarCallMethod.MethodStatusUpdateText(""); // 단계 업데이트
                    }

                    //frmPopupWaitingBarCallMethod.MethodStatusUpdateText("Copy DMCSREvision"); // 단계 업데이트
                    //StringBuilder dmcsrevisionDelete = new StringBuilder();
                    //dmcsrevisionDelete.AppendFormat("Update EDB_DMCSRevision Set DEleteFlag = 1 Where ProjectID = {0} and ListTypeID = {1};", newProjectID, listTypeID);
                    //proxyNew.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, dmcsrevisionDelete);
                    //var dmcsRevisionIDs = new Dictionary<int, int>();
                    //var dmcsRevisions = proxyCopy.ReadProjectDMCSRevisions(EDCS.WF.BSL.Variable.SharedVariable.IsIE, copyProjectID, listTypeID);
                    //var newDMCSRevisionHistories = dmcsRevisions.Select(s => new EDB_DMCSRevision()
                    //{
                    //    CaseProjectID = newProject.CaseProjectID,
                    //    ProjectID = newProjectID,
                    //    ListTypeID = listTypeID,
                    //    ModifiedDate = s.ModifiedDate,
                    //    RevisionNo = s.RevisionNo,
                    //}).ToList();
                    //proxyNew.CreateDMCSRevisions(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newDMCSRevisionHistories);

                    //index = 0;
                    //foreach (var history in newDMCSRevisionHistories)
                    //{
                    //    dmcsRevisionIDs.Add(dmcsRevisions[index].DMCSRevisionID, history.DMCSRevisionID);
                    //    index++;
                    //}
                }
            }
            frmPopupWaitingBarCallMethod.MethodClose();
        }
    }
}
