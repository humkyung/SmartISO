using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using EDCS.WF.DSL.Entities;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using EDCS.WF.BSL.Enums;

namespace Configuration
{
    public partial class UserControlCommonProjectSetting_EDB : UserControl
    {
        private long mProjectMasterID = -1;

        private enum menColumn { ProjectID, ProjectMasterID, ProjectNo, CreationDate, ProcessEQList, PowerEQList, ElecEQList, LineList, InstrumentList, LoadData, HazardousSourceFlag, HazardousType, HazardousAPI, BuildingList, StructualEquipmnetList };
        private string[] marColumnName = { "ProjectID", "ProjectMasterID", "Project No", "Creation Date", "Process EQ. List", "Power EQ. List", "Elec EQ. List", "Line List", "Instrument List", "Load Data", "HazardousSourceList", "Hazardous Type", "Hazardous API", "BuildingList", "StructualEquipmnetList" };
        private int[] marColumnWidth_EDB = { 0, 0, 100, 0, 150, 150, 150, 150, 150, 150, 0, 150, 150, 150, 150 };
        private bool[] marColumnLock_EDB = { true, true, true, true, false, false, false, false, false, false, false, false, false, false, false };

        public UserControlCommonProjectSetting_EDB(bool isIE, long lProjectMasterID)
        {
            InitializeComponent();

            mProjectMasterID = lProjectMasterID;
            labelTools.Text = "List";
            SetSpread(lProjectMasterID);
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidth_EDB, marColumnName, marColumnLock_EDB, null, null, null, null, null, Color.LightGray, true, false);
            fpSpreadData.ActiveSheet.ColumnHeader.Rows[0].Height = 30;

            fpSpreadData.ActiveSheet.Rows.Count = 1;

            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
            {
                arItems = null;
                arItemsData = null;

                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                switch (i)
                {
                    case (int)menColumn.ProcessEQList:
                    case (int)menColumn.PowerEQList:
                    case (int)menColumn.ElecEQList:
                    case (int)menColumn.LineList:
                    case (int)menColumn.LoadData:
                    case (int)menColumn.InstrumentList:
                    case (int)menColumn.HazardousSourceFlag:
                    case (int)menColumn.BuildingList:
                    case (int)menColumn.StructualEquipmnetList:
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.HazardousType:
                        arItems = EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousType.GetType().ToArray();
                        arItemsData = EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousType.GetType().ToArray();
                        break;
                    case (int)menColumn.HazardousAPI:
                        arItems = EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousNEC_SubType.GetType().ToArray();
                        arItemsData = EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousNEC_SubType.GetType().ToArray();
                        break;
                }

                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadData.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }
        }

        private void SetSpread(long lProjectMasterID)
        {
            InitSpread();
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var project = proxy.ReadProjectByProjectMasterID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, lProjectMasterID);
                if (project != null)
                {
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProjectID].Value = project.ProjectID;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProjectNo].Value = project.No;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.CreationDate].Value = project.CreationDate.HasValue == true ? project.CreationDate.Value.ToString("yyyy-MM-dd") : null;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProcessEQList].Value = project.ProcessEquipmentFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.PowerEQList].Value = project.PowerEquipmentFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ElecEQList].Value = project.ElectricalEquipmentFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.LineList].Value = project.LineFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.InstrumentList].Value = project.InstrumentFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.LoadData].Value = project.LoadDataFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousSourceFlag].Value = project.HazardousSourceFlag;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousType].Value = project.HazardousType;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousAPI].Value = project.HazardousAPI;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.BuildingList].Value = project.BuildingList;
                    fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.StructualEquipmnetList].Value = project.StructuralEquipmentList;
                }
                fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProjectMasterID].Value = lProjectMasterID;
            }
        }

        private void radButtonAttributeSave_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.Cursor = Cursors.WaitCursor;
                    var project = new EDB_Project();
                    project.ProjectID = Convert.ToInt32(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProjectID].Value);

                    project.ProcessEquipmentFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ProcessEQList].Value);
                    project.PowerEquipmentFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.PowerEQList].Value);
                    project.ElectricalEquipmentFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.ElecEQList].Value);
                    project.LineFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.LineList].Value);
                    project.InstrumentFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.InstrumentList].Value);
                    project.LoadDataFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.LoadData].Value);
                    project.HazardousSourceFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousSourceFlag].Value);
                    project.BuildingListFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.BuildingList].Value);
                    project.StructuralEquipmentFlag = Convert.ToBoolean(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.StructualEquipmnetList].Value);

                    if (CommonUtil.CommonLib.Util.NullToSpace(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousType].Value) != string.Empty)
                        project.HazardousType = CommonUtil.CommonLib.Util.NullToSpace(fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousType].Value);
                    else
                        project.HazardousType = null;

                    if (!string.IsNullOrEmpty(project.HazardousType))
                        project.HazardousAPI = fpSpreadData.ActiveSheet.Cells[0, (int)menColumn.HazardousAPI].Text;
                    else
                        project.HazardousAPI = string.Empty;

                    var oldProject = proxy.ReadProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, project.ProjectID);
                    proxy.CreateProjectUpdateListFlagOnly(EDCS.WF.BSL.Variable.SharedVariable.IsIE, project);

                    var listTypeID = 0;
                    if (oldProject.ProcessEquipmentFlag == false && project.ProcessEquipmentFlag == true)
                    {
                        listTypeID = 4;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    if (oldProject.PowerEquipmentFlag == false && project.PowerEquipmentFlag == true)
                    {
                        listTypeID = 5;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    if (oldProject.ElectricalEquipmentFlag == false && project.ElectricalEquipmentFlag == true)
                    {
                        listTypeID = 11;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    if (oldProject.InstrumentFlag == false && project.InstrumentFlag == true)
                    {
                        listTypeID = 3;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);

                        listTypeID = 17;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);

                        listTypeID = 23;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                    }
                    if (oldProject.LineFlag == false && project.LineFlag == true)
                    {
                        listTypeID = 2;
                        this.ReplaceInit(project.ProjectID, listTypeID); //Replace 생성
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    if (oldProject.LoadDataFlag == false && project.LoadDataFlag == true)
                    {
                        listTypeID = 24;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    //if (oldProject.HazardousSourceFlag == false && project.HazardousSourceFlag == true)
                    //{
                    //    //HazardousSourceFlag 안쓰는 컬럼이라 7시 이후 지워야함 로그 보고 테이블에서 삭제후 재배포 해야함
                    //    listTypeID = 25;
                    //    this.MethodSettingDefault(project.ProjectID, listTypeID);
                    //    this.ValidationInit(project.ProjectID, listTypeID);
                    //}
                    if (project.HazardousType != null && oldProject.HazardousType != project.HazardousType)
                    {
                        listTypeID = 25;
                        this.MethodSettingDefault(project.ProjectID, listTypeID, project.HazardousType);
                        this.ValidationInit(project.ProjectID, listTypeID);
                    }
                    if (oldProject.BuildingList == false && project.BuildingListFlag == true)
                    {
                        listTypeID = 26;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                    }
                    if (oldProject.StructuralEquipmentList == false && project.StructuralEquipmentFlag == true)
                    {
                        listTypeID = 27;
                        this.MethodSettingDefault(project.ProjectID, listTypeID);
                    }

                    this.Cursor = Cursors.Default;
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
        private void MethodSettingDefault(int p_projectID, int p_listTypeID, string p_hazardousType = null)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var caseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_projectID, p_listTypeID);
                //caseDynamic 이 0이 아니면 체크된 이력이 있다고 판단하고 초기화 하지 않음.
                if (caseDynamics.Count == 0)
                {
                    this.MethodSettingEquipmentTypeDefaultFromDMCS(p_listTypeID);

                    var columnDynamicDefaults = proxy.ReadColumnDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == p_listTypeID).ToList();
                    var caseDynamicDefaults = proxy.ReadCaseDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == p_listTypeID).ToList();
                    var columnDynamicParentDefaults = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == p_listTypeID).ToList();
                    var equipmentTypeDynamicDefaults = proxy.ReadEquipmentTypeDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == p_listTypeID).ToList();
                    var multiPropertyDefaults = proxy.ReadMultiPropertyDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == p_listTypeID).ToList();
                    var columnDynamicDropDownListDefaults = proxy.ReadColumnDynamicDropDownListDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.ListTypeID == p_listTypeID).ToList();

                    var listPowerEquipmentServiceDefaults = new List<EntityEDB_PowerEquipmentServiceDefault>();

                    if (p_listTypeID.Equals(5))
                    {
                        listPowerEquipmentServiceDefaults = proxy.ReadPowerEquipmentServiceDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    }

                    var listCaseDynamicDefaults = caseDynamicDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID));
                    var listEquipmentTypeDynamicDefaults = equipmentTypeDynamicDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID));
                    var listColumnDynamicDropDownListDefaults = columnDynamicDropDownListDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID));
                    var newCaseDynamicDefaults = new List<EDB_CaseDynamic>();
                    var newEquipmentTypeDynamicDefaults = new List<EDB_EquipmentTypeDynamic>();
                    var newColumnDynamicDropDownListDefaults = new List<EDB_ColumnDynamicDropDownList>();
                    var newMultiProperties = new List<EDB_MultiProperty>();
                    var newPowerEquipmentServices = new List<EDB_PowerEquipmentService>();
                    var listColumnDynamicParentDefaults = columnDynamicParentDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID)).ToList();

                    //ColumnDynamicDropDownList
                    foreach (var listColumnDynamicDropDownListDefault in listColumnDynamicDropDownListDefaults)
                    {
                        newColumnDynamicDropDownListDefaults.Add(new EDB_ColumnDynamicDropDownList()
                        {
                            ProjectID = p_projectID,
                            ListTypeID = p_listTypeID,
                            ColumnDynamicLabel = listColumnDynamicDropDownListDefault.ColumnDynamicLabel,
                            Orders = listColumnDynamicDropDownListDefault.Orders,
                            Name = listColumnDynamicDropDownListDefault.Name,
                            NameView = listColumnDynamicDropDownListDefault.NameView,
                        });
                    }
                    foreach (var listPowerEquipmentServiceDefault in listPowerEquipmentServiceDefaults)
                    {
                        newPowerEquipmentServices.Add(new EDB_PowerEquipmentService()
                        {
                            CommonSystemDynamicID = listPowerEquipmentServiceDefault.CommonSystemDynamicID,
                            ProjectID = p_projectID,
                            Orders = listPowerEquipmentServiceDefault.Orders,
                            Name = listPowerEquipmentServiceDefault.Name,
                            NameView = listPowerEquipmentServiceDefault.NameView,
                        });
                    }
                    if (string.IsNullOrEmpty(p_hazardousType))
                    {
                        // p_hazardousType 빈값이면 하자도스가 아닌것으로 
                        // hazardous가 아닐때
                        foreach (var listCaseDynamicDefault in listCaseDynamicDefaults)
                        {

                            newCaseDynamicDefaults.Add(new EDB_CaseDynamic()
                            {
                                ProjectID = p_projectID,
                                ListTypeID = p_listTypeID,
                                Orders = listCaseDynamicDefault.Orders,
                                Name = listCaseDynamicDefault.Name,
                                NameView = listCaseDynamicDefault.NameView,
                            });
                        }
                    }
                    else
                    {
                        foreach (var listCaseDynamicDefault in listCaseDynamicDefaults)
                        {
                            bool bCaseDynamicDefaultSaveFlag = false;

                            // hazardous일 때 해당 시트만 복사 되는 로직 추가
                            if (p_hazardousType == EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousType.NEC && (listCaseDynamicDefault.Name == "S_NEC_F" || listCaseDynamicDefault.Name == "S_NEC_S"))
                                bCaseDynamicDefaultSaveFlag = true;
                            if (p_hazardousType == EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousType.IEC && (listCaseDynamicDefault.Name == "S_IEC_F" || listCaseDynamicDefault.Name == "S_IEC_S"))
                                bCaseDynamicDefaultSaveFlag = true;
                            if (p_hazardousType == EDCS.WF.BSL.Variable.SharedVariable.StaticHazardousType.IP && (listCaseDynamicDefault.Name == "S_IP_F" || listCaseDynamicDefault.Name == "S_IP_S"))
                                bCaseDynamicDefaultSaveFlag = true;

                            if (bCaseDynamicDefaultSaveFlag)
                            {
                                newCaseDynamicDefaults.Add(new EDB_CaseDynamic()
                                {
                                    ProjectID = p_projectID,
                                    ListTypeID = p_listTypeID,
                                    Orders = listCaseDynamicDefault.Orders,
                                    Name = listCaseDynamicDefault.Name,
                                    NameView = listCaseDynamicDefault.NameView.Substring(0, listCaseDynamicDefault.NameView.LastIndexOf('_'))
                                });
                            }
                        }
                    }

                    if (newCaseDynamicDefaults.Count > 0)
                    {
                        proxy.CreateCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  newCaseDynamicDefaults, p_projectID, p_listTypeID);

                        foreach (var newCaseDynamicDefault in newCaseDynamicDefaults)
                        {
                            EDB_ColumnDynamicParent createColumnDynamicParent = null;
                            EntityEDB_ColumnDynamicParentDefault listColumnDynamicParentDefault = null;
                            if (listColumnDynamicParentDefaults != null)
                            {
                                listColumnDynamicParentDefault = listColumnDynamicParentDefaults.SingleOrDefault(s => s.CaseDynamicDefaultName == newCaseDynamicDefault.Name);
                            }
                            //기본 방식. 리스트 타입하나에 하나의 Parent (모든 시트가 동일한 컬럼)
                            if (listColumnDynamicParentDefault == null)
                            {
                                //새로운 프로젝트의 경우 해당하는 ColumnDynamicParent 가 없기 떄문에 만들어 줌
                                createColumnDynamicParent = new EDB_ColumnDynamicParent()
                                {
                                    CaseDynamicID = newCaseDynamicDefault.CaseDynamicID,
                                    ColumnHeaderMergeProperty = listColumnDynamicParentDefaults != null ? (listColumnDynamicParentDefaults.Count > 0 ? listColumnDynamicParentDefaults[0].ColumnHeaderMergeProperty : null) : null,
                                };
                            }
                            //Process EQuip, BM/BQ 적용되면서 기본시트별로 컬럼이 다름.
                            else
                            {
                                //새로운 프로젝트의 경우 해당하는 ColumnDynamicParent 가 없기 떄문에 만들어 줌
                                createColumnDynamicParent = new EDB_ColumnDynamicParent()
                                {
                                    CaseDynamicID = newCaseDynamicDefault.CaseDynamicID,
                                    ColumnHeaderMergeProperty = listColumnDynamicParentDefault.ColumnHeaderMergeProperty,
                                };
                            }

                            proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref createColumnDynamicParent, p_projectID, p_listTypeID, null);

                            //ListType 별로 나누어져있는 데이터를 COlumnDynamic 테이블에 맞게 데이터 생성
                            var columnDynamics = new List<EDB_ColumnDynamic>();
                            foreach (var columnDynamicDefault in columnDynamicDefaults.Where(w => w.CaseDynamicDefaultName == newCaseDynamicDefault.Name || w.CaseDynamicDefaultName == null))
                            {
                                EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                columnDynamic.ColumnIndex = columnDynamicDefault.ColumnIndex;
                                columnDynamic.CommonUnitID = columnDynamicDefault.CommonUnitID;
                                columnDynamic.Orders = columnDynamicDefault.Orders;
                                columnDynamic.Label = columnDynamicDefault.Label;
                                columnDynamic.LabelView = columnDynamicDefault.LabelView;
                                columnDynamic.CellType = columnDynamicDefault.CellType;
                                columnDynamic.Locked = columnDynamicDefault.Locked;
                                columnDynamic.Width = columnDynamicDefault.Width;
                                columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                                columnDynamic.OwnershipDisciplineID = columnDynamicDefault.OwnershipDisciplineID;
                                columnDynamic.HorizontalAlignment = columnDynamicDefault.HorizontalAlignment;
                                columnDynamics.Add(columnDynamic);

                            } // end foreach
                            //COlumndynamic 에 데이터 추가
                            proxy.CreateColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, columnDynamics, p_projectID, p_listTypeID, createColumnDynamicParent.CaseDynamicID, null);
                        }
                    }
                    //값이 없을 경우 임의로 하나 추가.
                    else
                    {
                        //새로운 프로젝트의 경우 해당하는 Case 가 없기 떄문에 만들어줌(ColumnDynamicParent 에서 사용)
                        var caseDynamic = new EDB_CaseDynamic()
                        {
                            ProjectID = p_projectID,
                            ListTypeID = p_listTypeID,
                            Orders = 100,
                            Name = "C1",
                            NameView = "Case1",
                        };
                        proxy.CreateCaseDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, caseDynamic);

                        //새로운 프로젝트의 경우 해당하는 ColumnDynamicParent 가 없기 떄문에 만들어 줌
                        var createColumnDynamicParent = new EDB_ColumnDynamicParent()
                        {
                            CaseDynamicID = caseDynamic.CaseDynamicID,
                            ColumnHeaderMergeProperty = listColumnDynamicParentDefaults != null ? (listColumnDynamicParentDefaults.Count > 0 ? listColumnDynamicParentDefaults[0].ColumnHeaderMergeProperty : null) : null,
                        };
                        proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  createColumnDynamicParent, p_projectID, p_listTypeID, null);

                        var columnDynamics = new List<EDB_ColumnDynamic>();
                        //ListType 별로 나누어져있는 데이터를 COlumnDynamic 테이블에 맞게 데이터 생성
                        foreach (var columnDynamicDefault in columnDynamicDefaults)
                        {
                            EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                            columnDynamic.ColumnIndex = columnDynamicDefault.ColumnIndex;
                            columnDynamic.CommonUnitID = columnDynamicDefault.CommonUnitID;
                            columnDynamic.Orders = columnDynamicDefault.Orders;
                            columnDynamic.Label = columnDynamicDefault.Label;
                            columnDynamic.LabelView = columnDynamicDefault.LabelView;
                            columnDynamic.CellType = columnDynamicDefault.CellType;
                            columnDynamic.Locked = columnDynamicDefault.Locked;
                            columnDynamic.Width = columnDynamicDefault.Width;
                            columnDynamic.HorizontalAlignment = columnDynamicDefault.HorizontalAlignment;
                            columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                            columnDynamics.Add(columnDynamic);
                        } // end foreach
                        //COlumndynamic 에 데이터 추가
                        proxy.CreateColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, columnDynamics, p_projectID, p_listTypeID, createColumnDynamicParent.CaseDynamicID, null);
                    }
                    //DH EquipmentType
                    foreach (var listEquipmentTypeDynamicDefault in listEquipmentTypeDynamicDefaults)
                    {
                        //var caseDynamic = newCaseDynamicDefaults.SingleOrDefault(s => s.Name == listEquipmentTypeDynamicDefault.CaseDynamicName);
                        var caseDynamic = newCaseDynamicDefaults.SingleOrDefault(s => s.Name == listEquipmentTypeDynamicDefault.CaseDynamicInitialName);
                        newEquipmentTypeDynamicDefaults.Add(new EDB_EquipmentTypeDynamic()
                        {
                            ProjectID = p_projectID,
                            ListTypeID = p_listTypeID,
                            Orders = listEquipmentTypeDynamicDefault.Orders,
                            Name = listEquipmentTypeDynamicDefault.Name,
                            NameView = listEquipmentTypeDynamicDefault.NameView,

                            CaseDynamicID = caseDynamic == null ? null : (int?)caseDynamic.CaseDynamicID,
                        });
                    }
                    //DH EquipmentType
                    if (newEquipmentTypeDynamicDefaults.Count > 0)
                    {
                        proxy.CreateEquipmentTypeDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  newEquipmentTypeDynamicDefaults, p_projectID, p_listTypeID);
                        //표준화

                        foreach (var newEquipmentTypeDynamicDefault in newEquipmentTypeDynamicDefaults.Where(w => w.Name.Length < 5))
                        {
                            var listEquipmentTypeDynamicDefault = listEquipmentTypeDynamicDefaults.Where(w => w.Name.Equals(newEquipmentTypeDynamicDefault.Name)).SingleOrDefault();

                            foreach (var multiProperty in multiPropertyDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID) && w.EquipmentTypeDynamicName.Equals(newEquipmentTypeDynamicDefault.Name)))
                            {
                                newMultiProperties.Add(new EDB_MultiProperty()
                                {
                                    EquipmentTypeDynamicID = newEquipmentTypeDynamicDefault.EquipmentTypeDynamicID,
                                    VisibleFlag = multiProperty.VisibleFlag,
                                    Orders = multiProperty.Orders,
                                    Name = multiProperty.Name,
                                    NameView = multiProperty.NameView,
                                    OrderCheck = true,
                                });
                            }
                            //newMultiProperties
                        }
                        //Elec 만 들어와야 합니다.
                        foreach (var newEquipmentTypeDynamicDefault in newEquipmentTypeDynamicDefaults.Where(w => w.Name.Length > 5))
                        {
                            var listEquipmentTypeDynamicDefault = listEquipmentTypeDynamicDefaults.Where(w => w.Name.Equals(newEquipmentTypeDynamicDefault.Name)).SingleOrDefault();
                            foreach (var multiProperty in multiPropertyDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID) && w.EquipmentTypeDynamicName.Equals(newEquipmentTypeDynamicDefault.Name)))
                            {
                                //Elec 만 들어와야 합니다.
                                newMultiProperties.Add(new EDB_MultiProperty()
                                {
                                    EquipmentTypeDynamicID = newEquipmentTypeDynamicDefault.EquipmentTypeDynamicID,
                                    VisibleFlag = multiProperty.VisibleFlag,
                                    Orders = multiProperty.Orders,
                                    Name = multiProperty.Name,
                                    NameView = multiProperty.NameView,
                                    OrderCheck = true,
                                    UOM = multiProperty.UOM,
                                    IsExcel = multiProperty.IsExcel,
                                    IsBQ = multiProperty.IsBQ,
                                    Lvl = multiProperty.Level,
                                    Code = multiProperty.Code,
                                });

                            }
                        }
                        var newEquipmentTypeSubDynamicDefaults = newEquipmentTypeDynamicDefaults.Where(w => w.Name.Length > 4).ToList();
                        foreach (var newEquipmentTypeDynamic in newEquipmentTypeSubDynamicDefaults)
                        {
                            //Name 짜르기 전에 실행
                            var newEquipmentTypeDynamicDefault = newEquipmentTypeDynamicDefaults.Where(w => w.Name.Equals(newEquipmentTypeDynamic.Name.Substring(0, 4))).SingleOrDefault();
                            newEquipmentTypeDynamic.GroupCode = newEquipmentTypeDynamic.Name.Substring(0, 2);
                            //2단계 EquipmentType 의 ParentID 를 결정함.
                            newEquipmentTypeDynamic.ParentID = newEquipmentTypeDynamicDefault.EquipmentTypeDynamicID;
                            newEquipmentTypeDynamic.Name = newEquipmentTypeDynamic.Name.Substring(4);
                            newEquipmentTypeDynamic.CaseDynamicID = newEquipmentTypeDynamicDefault.CaseDynamicID;
                        }
                        //Sub 업데이트 Name, ParentID, DisciplineID
                        proxy.UpdateEquipmentSubTypeDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newEquipmentTypeSubDynamicDefaults);

                        var newEquipmentTypeMainDynamicDefaults = newEquipmentTypeDynamicDefaults.Where(w => w.ParentID.Equals(0)).ToList();
                        foreach (var newEquipmentTypeDynamic in newEquipmentTypeMainDynamicDefaults)
                        {
                            //Name 짜르기 전에 실행
                            newEquipmentTypeDynamic.GroupCode = newEquipmentTypeDynamic.Name.Substring(0, 2);
                            newEquipmentTypeDynamic.Name = newEquipmentTypeDynamic.Name.Substring(2);
                        }
                        //Main 업데이트 Name ,DisciplineID
                        proxy.UpdateEquipmentSubTypeDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newEquipmentTypeMainDynamicDefaults);

                        //parentId 가 0이고,

                        if (p_listTypeID == 5)
                        {
                            foreach (var multiProperty in multiPropertyDefaults.Where(w => w.ListTypeID.Equals(p_listTypeID)))
                            {
                                newMultiProperties.Add(new EDB_MultiProperty()
                                {
                                    ProjectID = p_projectID,
                                    Orders = multiProperty.Orders,
                                    Name = multiProperty.Name,
                                    NameView = multiProperty.NameView,
                                    OrderCheck = true,
                                });
                            }
                        }
                        ///
                        ///// 기존 
                        //foreach (var newEquipmentTypeDynamicDefault in newEquipmentTypeDynamicDefaults)
                        //{
                        //     //CategoryDefault 가 존재하는 경우에만.
                        //    newCategoryDynamics.Add(new DSL.CategoryDynamic()
                        //    {
                        //        EquipmentTypeDynamicID = newEquipmentTypeDynamicDefault.EquipmentTypeDynamicID,
                        //        Orders = 100,
                        //        Name = newEquipmentTypeDynamicDefault.Name,
                        //        NameView = newEquipmentTypeDynamicDefault.Name,
                        //    });

                        //    foreach (var multiProperty in multiPropertyDefaults.Where(w => w.ListTypeID.Equals(columnDynamicDefaultsGroup.Key) && w.EquipmentTypeDynamicName.Equals(newEquipmentTypeDynamicDefault.Name)))
                        //    {
                        //        newMultiProperties.Add(new DSL.MultiProperty()
                        //        {
                        //            EquipmentTypeDynamicID = newEquipmentTypeDynamicDefault.EquipmentTypeDynamicID,
                        //            VisibleFlag = multiProperty.VisibleFlag,
                        //            Orders = multiProperty.Orders,
                        //            Name = multiProperty.Name,
                        //            NameView = multiProperty.NameView,
                        //            OrderCheck = true,
                        //        });
                        //    }
                        //    //newMultiProperties
                        //}
                        //////

                        if (newMultiProperties.Count > 0)
                        {
                            proxy.CreateMultiProperty(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newMultiProperties, p_projectID, p_listTypeID);
                        }
                    }

                    if (newColumnDynamicDropDownListDefaults.Count > 0)
                    {
                        proxy.CreateColumnDynamicDropDownList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newColumnDynamicDropDownListDefaults, p_projectID, p_listTypeID);
                    }
                    if (newPowerEquipmentServices.Count > 0)
                    {
                        proxy.CreatePowerEquipmentService(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newPowerEquipmentServices, p_projectID);
                    }

                }
            }


        }
        private void MethodSettingEquipmentTypeDefaultFromDMCS(int p_listTypeID)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var insertEquipmentType = new List<EDB_EquipmentTypeDynamicDefault>();
                insertEquipmentType.Add(new EDB_EquipmentTypeDynamicDefault() { Name = "IN14", NameView = "Control Valve (IN)" });
                insertEquipmentType.Add(new EDB_EquipmentTypeDynamicDefault() { Name = "CV06", NameView = "Equipment Foundation (CV)" });
                var insertEquipmentTypeSub = new List<EDB_EquipmentTypeDynamicDefault>();
                insertEquipmentTypeSub.Add(new EDB_EquipmentTypeDynamicDefault() { Name = "IN1440", NameView = "Desuperheater", Orders = 100 });
                insertEquipmentTypeSub.Add(new EDB_EquipmentTypeDynamicDefault() { Name = "CV0633", NameView = "Lining for Containment (Pond, Pit, Dike)", Orders = 100 });

                var removeEquipmentNameView = new string[] { "Package(Static)", "Package (Rotating)" };
                var defaultDisciplines = new Dictionary<int, string[]>();
                defaultDisciplines.Add(4, new string[] { "PR", "RO", "ST", "FF", "CO" });
                defaultDisciplines.Add(5, new string[] { "PW" });
                Stopwatch stopwatch = new Stopwatch();
                stopwatch.Start();
                var equipmentTypeDynamicDefaults = proxy.ReadEquipmentTypeDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                equipmentTypeDynamicDefaults.RemoveAll(ra => ra.Name.Contains("IN"));
                equipmentTypeDynamicDefaults.RemoveAll(ra => ra.Name.Contains("CV"));
                equipmentTypeDynamicDefaults.RemoveAll(ra => ra.ListTypeID == 11);

                foreach (var defaultDiscipline in defaultDisciplines.Where(w => w.Key == p_listTypeID))
                {
                    //DMCS 객체는 Default 모두다 붙임.
                    var equipmentTypeDynamicDefaultFromDMCS = new List<EntityEDB_EquipmentTypeDefaultFromDMCS>();
                    var equipmentTypeDynamicList = equipmentTypeDynamicDefaults.Where(w => w.ListTypeID == defaultDiscipline.Key).ToList();
                    proxy.ReadEquipmentTypeDynamicDefaultFromDMCS(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref equipmentTypeDynamicDefaultFromDMCS, defaultDiscipline.Value);

                    if (this.MethodCheckEquipmentTypeDefaultFromDMCS(equipmentTypeDynamicDefaultFromDMCS, equipmentTypeDynamicList
                        , removeEquipmentNameView, defaultDiscipline.Key) == true)
                    {
                        var oldEquipmentTypeDynamics = equipmentTypeDynamicDefaultFromDMCS.Distinct();
                        //var newEquipmentTypeDynamics = new List<EDCS.WF.DSL.EquipmentTypeDynamicDefault>();

                        var newEquipmentTypeDynamics = oldEquipmentTypeDynamics
                            .Select(s => new EDB_EquipmentTypeDynamicDefault()
                            {
                                Name = s.EquipmentTypeNameEDBStyle,
                                NameView = p_listTypeID == 4 ? (!removeEquipmentNameView.Contains(s.EquipmentTypeNameView) ? s.EquipmentTypeNameView + " (" + s.GroupCode + ")" : s.EquipmentTypeNameView)
                                 : s.EquipmentTypeNameView,
                                //임시.
                                //Unit = s.GroupCode,
                                ListTypeID = defaultDiscipline.Key,
                            }).ToList();
                        //newEquipmentTypeDynamics.Where(w => w.ListTypeID == 4 && !removeEquipmentNameView.Contains(w.NameView)).ToList().ConvertAll(ca => ca.NameView = ca.NameView + " (" + ca.Unit + ")");
                        //.ConvertAll(ca => ca.NameView = ca.NameView + " (" + ca.Unit + ")");
                        newEquipmentTypeDynamics.AddRange(equipmentTypeDynamicDefaultFromDMCS
                           .Select(s => new EDB_EquipmentTypeDynamicDefault()
                           {
                               Name = s.EquipmentTypeSubNameEDBStyle,
                               NameView = s.EquipmentTypeSubNameView,
                           }));

                        var orders = 100;
                        foreach (var newEquipmentTypeDynamic in newEquipmentTypeDynamics.Where(w => w.Name.Length < 5))
                        {
                            newEquipmentTypeDynamic.Orders = orders;
                            orders += 100;
                            var subOrders = 100;
                            foreach (var newEquipmentTypeSubDynamic in newEquipmentTypeDynamics.Where(w => w.Name.Length > 5 && w.Name.Contains(newEquipmentTypeDynamic.Name)).OrderBy(ob => ob.Name))
                            {
                                newEquipmentTypeSubDynamic.Orders = subOrders;
                                subOrders += 100;
                            }
                        }
                        if (defaultDiscipline.Key == 4)
                        {
                            foreach (var equipmentType in insertEquipmentType)
                            {
                                equipmentType.Orders += orders;
                                orders += 100;
                                newEquipmentTypeDynamics.Add(equipmentType);
                            }
                            newEquipmentTypeDynamics.AddRange(insertEquipmentTypeSub);
                        }
                        //newEquipmentTypeDynamics = newEquipmentTypeDynamics.OrderBy(s => s.Name.Length).ThenBy(t => t.Orders).ToList();
                        newEquipmentTypeDynamics = newEquipmentTypeDynamics.OrderBy(s => s.Name).ToList();
                        newEquipmentTypeDynamics.ConvertAll(ca => ca.ListTypeID = defaultDiscipline.Key);
                        proxy.CreateEquipmentTypeDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newEquipmentTypeDynamics, defaultDiscipline.Key);
                    }
                }
                stopwatch.Stop();
                Debug.WriteLine("MethodSettingEquipmentTypeDefaultFromDMCS {0}", stopwatch.ElapsedMilliseconds);
            }

        }
        private bool MethodCheckEquipmentTypeDefaultFromDMCS(
            List<EntityEDB_EquipmentTypeDefaultFromDMCS> p_equipmentTypeDynamicDefaultFromDMCS
            , List<EntityEDB_EquipmentTypeDynamicDefault> p_equipmentTypeDynamicList
            , string[] p_removeEquipmentNameView
            , int p_listTypeID
            )
        {
            foreach (var equipmentTypeDynamicDefaultGroupCode in p_equipmentTypeDynamicDefaultFromDMCS.GroupBy(gb => gb.GroupCode))
            {
                var equipmentTypeDynamicGroupCode = p_equipmentTypeDynamicList.Where(w => w.Name.Contains(equipmentTypeDynamicDefaultGroupCode.Key)).ToList();
                foreach (var defaultName in equipmentTypeDynamicDefaultGroupCode.GroupBy(gb => gb.EquipmentTypeNameEDBStyle))
                {
                    var equipmentType = equipmentTypeDynamicGroupCode.SingleOrDefault(w => w.Name == defaultName.Key);
                    if (equipmentType == null) { Console.WriteLine("{0} {1}", defaultName.Key, defaultName.ElementAt(0).EquipmentTypeNameView); return true; } //잘못됨.\
                    else
                    {
                        var defaultNameView = string.Empty;
                        defaultNameView = defaultName.ElementAt(0).EquipmentTypeNameView;
                        if (p_listTypeID == 4 && !p_removeEquipmentNameView.Contains(equipmentType.NameView)) defaultNameView = defaultName.ElementAt(0).EquipmentTypeNameView + " (" + equipmentTypeDynamicDefaultGroupCode.Key + ")";
                        if (equipmentType.NameView.ToUpper() != defaultNameView.ToUpper()) { Console.WriteLine("{0} {1}", defaultName.Key, defaultName.ElementAt(0).EquipmentTypeNameView); return true; }//잘못됨.
                        else p_equipmentTypeDynamicList.Remove(equipmentType);
                    }

                    foreach (var equipmentdefault in defaultName)
                    {
                        equipmentType = equipmentTypeDynamicGroupCode.SingleOrDefault(w => w.Name == equipmentdefault.EquipmentTypeSubNameEDBStyle);
                        if (equipmentType == null) { Console.WriteLine("{0} {1}", equipmentdefault.EquipmentTypeSubNameEDBStyle, equipmentdefault.EquipmentTypeSubNameView); return true; } //잘못됨.
                        else if (equipmentType.NameView.ToUpper() != equipmentdefault.EquipmentTypeSubNameView.ToUpper()) { Console.WriteLine("{0} {1}", equipmentdefault.EquipmentTypeSubNameEDBStyle, equipmentdefault.EquipmentTypeSubNameView); return true; } //잘못됨.
                        else p_equipmentTypeDynamicList.Remove(equipmentType);
                    }
                }
            }
            if (p_equipmentTypeDynamicList.Count > 0)
            {
                return true;
            }
            return false;
        }
        private void fpSpreadData_Change(object sender, ChangeEventArgs e)
        {
            if (e.Column == (int)menColumn.HazardousType)
            {
                if (fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.HazardousType].Value == null || fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.HazardousType].Value.ToString() == string.Empty)
                {
                    fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.HazardousAPI].Value = "";
                    fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.HazardousAPI].Locked = true;
                }
                else
                {
                    fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.HazardousAPI].Locked = false;
                }
            }
        }
        private void ValidationInit(int p_projectId, int p_listtypeId)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EDB_ValidationDynamic> cValidationDynamics = new List<EDB_ValidationDynamic>();
                var ValidationDynaimcDefaults = proxy.ReadValidationDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_listtypeId);
                var ValidationDynamics = proxy.ReadValidationDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_projectId, p_listtypeId);

                foreach (var validationdynamicdefaulte in ValidationDynaimcDefaults.Where(w => w.ListTypeID.Equals(p_listtypeId)))
                {
                    var ValidationDynamic = new EDB_ValidationDynamic()
                    {
                        ListTypeID = p_listtypeId,
                        ProjectID = p_projectId,
                        ColumnLabel = validationdynamicdefaulte.ColumnLabel,
                        Regex = validationdynamicdefaulte.Regex,
                        ModifiedDate = DateTime.Now,
                        NumberFlag = validationdynamicdefaulte.NumberFlag,
                        AllowNull = validationdynamicdefaulte.AllowNull,
                        Exception = validationdynamicdefaulte.Exception,
                    };
                    cValidationDynamics.Add(ValidationDynamic);
                }
                proxy.CreateValidationDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cValidationDynamics, p_projectId, p_listtypeId);
            }
        }


        // 2016.12.23 Replace Default 추가 사항 변경되어야함
        private void ReplaceInit(int p_projectId, int p_listTypeId)
        {
            var replaceDynamics = new List<EDB_ReplaceDynamic>();
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var replaceDynamicDefaults = proxy.ReadReplaceDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_listTypeId);

                foreach (var replaceDynamicDefault in replaceDynamicDefaults)
                {
                    var replaceDynamic = new EDB_ReplaceDynamic();

                    replaceDynamic.ReplaceID = 0;
                    replaceDynamic.ProjectID = p_projectId;
                    replaceDynamic.ListTypeID = p_listTypeId;
                    replaceDynamic.CommonColumnTypeID = replaceDynamicDefault.CommonColumnTypeID;
                    replaceDynamic.Orders = replaceDynamicDefault.Orders;
                    replaceDynamic.Name = replaceDynamicDefault.Name;
                    replaceDynamic.NameView = replaceDynamicDefault.NameView;

                    replaceDynamic.Value = replaceDynamicDefault.Value;
                    replaceDynamics.Add(replaceDynamic);
                }
                proxy.CreateReplaceDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, replaceDynamics, p_projectId, p_listTypeId);
            }
        }
    }
}

