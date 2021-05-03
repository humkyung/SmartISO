using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using Configuration.Properties;
using CommonUtil.CommonLib;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;

using Telerik.WinControls.UI;
using Telerik.WinControls.UI.Docking;
using EDCS.WF.DSL.Entities;
using EDCS.WF.DSL.DataProperty;
using TechSun.Theme;

namespace Configuration
{
    public partial class UserControlCommonPBSMappingSetting : UserControlBase
    {
        #region # Mapping
        private enum menColumn { Select, Status, PBSMappingID, Type, Name, ProcessEquipment, LineList, SPPID, SP3D, SPI, Last };
        private string[] marColumnName = { " ", "Status", "PBSMappingID", "Type", "Name", "Process Equipment", "Line List", "SPPID", "SP3D", "SPI" };
        private int[] marColumnWidth = { 60, 0, 0, 100, 150, 150, 150, 150, 150, 300 };

        private bool[] mColumnLock = new bool[] { false, false, false, false, false, false, false, false };

        int m_iCount = 0;

        // 기준 PBS Setting 정보
        List<EntityCommon_PBSSetting> mlstPBSSetting = new List<EntityCommon_PBSSetting>();
        Dictionary<string, int> dicPBSSettingOrder = new Dictionary<string, int>();

        ComboBoxCellType mcbtColumn_ProcessEquipment_SITE = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_ProcessEquipment_PLANT = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_ProcessEquipment_AREA = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_ProcessEquipment_UNIT = new ComboBoxCellType();

        ComboBoxCellType mcbtColumn_LineList_SITE = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_LineList_PLANT = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_LineList_AREA = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_LineList_UNIT = new ComboBoxCellType();

        ComboBoxCellType mcbtColumn_SPPID_SITE = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPPID_PLANT = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPPID_AREA = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPPID_UNIT = new ComboBoxCellType();

        ComboBoxCellType mcbtColumn_SP3D_SITE = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SP3D_PLANT = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SP3D_AREA = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SP3D_UNIT = new ComboBoxCellType();

        ComboBoxCellType mcbtColumn_SPI_SITE = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPI_PLANT = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPI_AREA = new ComboBoxCellType();
        ComboBoxCellType mcbtColumn_SPI_UNIT = new ComboBoxCellType();

        //DataTable dtPBS = new DataTable("Table1");
        List<EntityEDB_DistrictDynamic> mlstPBS_EquipmentList = null;
        List<EntityEDB_DistrictDynamic> mlstPBS_LineList = null;
        DataTable mdtPBS_SPPID = new DataTable("Table1");
        DataTable mdtPBS_SP3D = new DataTable("Table1");
        DataTable mdtPBS_SPI = new DataTable("Table1");

        public const string CONST_SITE_PLANT_TYPE = "69";
        public const string CONST_PLANT_PLANT_TYPE = "70";
        public const string CONST_AREA_PLANT_TYPE = "72";
        public const string CONST_UNIT_PLANT_TYPE = "65";
        public const string CONST_DRAWING_PLANT_TYPE = "0";

        private int mProjectSettingMaxCount = 5;
        Dictionary<int, bool> m_dicbProjectSettingInfo = new Dictionary<int, bool>();
        #endregion

        #region # PBS Setting
        private enum menColumn_Setting { Select, PBSType, PBSSettingID, ProjectMasterID, ProcessEquipment, LineList, SPPID, SP3D, SPI, SortOrder, DeleteFlag, Last };
        private string[] mColumnName_Setting = new string[] { "Check", "Type", "PBSSettingID", "ProjectMasterID", "ProcessEquipment", "LineList", "SPPID", "SP3D", "SPI", "SortOrder", "DeleteFlag" };
        private int[] mColumnWidth_Setting = new int[] { 50, 100, 100, 100, 150, 100, 100, 100, 100, 80, 80 };
        private bool[] mColumnLock_Setting = new bool[] { false, false, true, true, false, false, false, false, false, true, true };

        // Add const
        ComboBoxCellType mcbtPBS_TYPE_Setting = new ComboBoxCellType();

        ComboBoxCellType mcbtEDBColumn_TYPE_Setting = new ComboBoxCellType();

        ComboBoxCellType mcbtProcessEQColumn_TYPE_Setting = new ComboBoxCellType();
        ComboBoxCellType mcbtLineColumn_TYPE_Setting = new ComboBoxCellType();
        ComboBoxCellType mcbtSPPIDColumn_TYPE_Setting = new ComboBoxCellType();
        ComboBoxCellType mcbtSP3DColumn_TYPE_Setting = new ComboBoxCellType();
        ComboBoxCellType mcbtSPIColumn_TYPE_Setting = new ComboBoxCellType();

        Dictionary<int, bool> m_dicbProjectSettingInfo_Setting = new Dictionary<int, bool>();

        string[] marPBSType_Setting = { };

        string[] marPBSTypes_EDB_Setting = { };
        string[] marPBSTypes_SPPID_Setting = { };
        string[] marPBSTypes_SP3D_Setting = { };
        string[] marPBSTypes_SPI_Setting = { };

        Dictionary<string, string> mdicPBSTypes_EDB_Setting = new Dictionary<string, string>();
        #endregion

        public UserControlCommonPBSMappingSetting()
        {
            InitializeComponent();
            InitPBSSetting();
        }

        public override void InitializeControl()
        {
            InitDropDownControl();
            this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
            this.fpSpreadMapping.Change += new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadMapping_Change);
            this.fpSpreadMapping.ComboSelChange += new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpSpreadMapping_ComboSelChange);
        }

        private void InitPBSSetting()
        {
            marPBSType_Setting = new string[5];
            marPBSType_Setting[0] = string.Empty;
            marPBSType_Setting[1] = PBSMappingLevel.Site;
            marPBSType_Setting[2] = PBSMappingLevel.Plant;
            marPBSType_Setting[3] = PBSMappingLevel.Area;
            marPBSType_Setting[4] = PBSMappingLevel.Unit;

            marPBSTypes_SPPID_Setting = new string[5];
            marPBSTypes_SPPID_Setting[0] = string.Empty;
            marPBSTypes_SPPID_Setting[1] = PBSMappingLevel.Site;
            marPBSTypes_SPPID_Setting[2] = PBSMappingLevel.Plant;
            marPBSTypes_SPPID_Setting[3] = PBSMappingLevel.Area;
            marPBSTypes_SPPID_Setting[4] = PBSMappingLevel.Unit;

            marPBSTypes_SP3D_Setting = new string[4];
            marPBSTypes_SP3D_Setting[0] = string.Empty;
            marPBSTypes_SP3D_Setting[1] = PBSMappingLevel.Plant;
            marPBSTypes_SP3D_Setting[2] = PBSMappingLevel.Area;
            marPBSTypes_SP3D_Setting[3] = PBSMappingLevel.Unit;

            marPBSTypes_SPI_Setting = new string[5];
            marPBSTypes_SPI_Setting[0] = string.Empty;
            marPBSTypes_SPI_Setting[1] = PBSMappingLevel.Domain;
            marPBSTypes_SPI_Setting[2] = PBSMappingLevel.Plant;
            marPBSTypes_SPI_Setting[3] = PBSMappingLevel.Area;
            marPBSTypes_SPI_Setting[4] = PBSMappingLevel.Unit;

            mdicPBSTypes_EDB_Setting.Clear();
            mdicPBSTypes_EDB_Setting.Add("", "");
            mdicPBSTypes_EDB_Setting.Add("100", PBSMappingLevel.Area);
            mdicPBSTypes_EDB_Setting.Add("200", PBSMappingLevel.Unit);
        }

        public override void LoadData()
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Select PBS list.  Please wait!";

            try
            {
                InitControl_Setting();
                SetSplitPanelControl_NameSetting();
                InitTab();
                InitControl_Mapping();

                FrmProgressStatus.FormClose();
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
            }
        }

        private void InitTab()
        {
            (radPageViewPBS.ViewElement as RadPageViewStripElement).ShowItemCloseButton = false;
            (radPageViewPBS.ViewElement as RadPageViewStripElement).StripButtons = StripViewButtons.None;
        }

        private void SetSplitPanelControl_NameSetting()
        {
            splitPanel1.Text = ((int)menColumn.ProcessEquipment).ToString();
            splitPanel5.Text = ((int)menColumn.LineList).ToString();
            splitPanel2.Text = ((int)menColumn.SPPID).ToString();
            splitPanel3.Text = ((int)menColumn.SP3D).ToString();
            splitPanel4.Text = ((int)menColumn.SPI).ToString();
        }

        #region # Mapping Spread
        private void InitDropDownControl()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
        }

        private void InitSpreadCombobox(EntityCommon_ProjectMaster common_ProjectMaster)
        {
            try
            {
                FrmProgressStatus.ProgressPer = "Select Process Equipment List.  Please wait!";
                SetEDBPBSData(common_ProjectMaster.ProjectMasterID, (int)EnumNameCheck.ProcessEquipmentList, ref mlstPBS_EquipmentList, ref mcbtColumn_ProcessEquipment_PLANT, ref mcbtColumn_ProcessEquipment_AREA, ref mcbtColumn_ProcessEquipment_UNIT);
            }
            catch
            {

            }

            try
            {
                FrmProgressStatus.ProgressPer = "Select Process Line List.  Please wait!";
                SetEDBPBSData(common_ProjectMaster.ProjectMasterID, (int)EnumNameCheck.LineList, ref mlstPBS_LineList, ref mcbtColumn_LineList_PLANT, ref mcbtColumn_LineList_AREA, ref mcbtColumn_LineList_UNIT);
            }
            catch
            {

            }

            try
            {
                FrmProgressStatus.ProgressPer = "Select SPPID.  Please wait!";
                SetSPPIDPBSData(common_ProjectMaster.ProjectMasterID);
            }
            catch
            {

            }

            try
            {
                FrmProgressStatus.ProgressPer = "Select SP3D.  Please wait!";
                SetSP3DPBSData(common_ProjectMaster.ProjectMasterID);
            }
            catch
            {

            }

            try
            {
                FrmProgressStatus.ProgressPer = "Select SPI.  Please wait!";
                SetSPIPBSData(common_ProjectMaster.ProjectMasterID);
            }
            catch
            {

            }
        }

        public void SetEDBPBSData(long lProjectMasterID, int iListType, ref List<EntityEDB_DistrictDynamic> lstPBS, ref ComboBoxCellType mcbtColumn_PLANT, ref ComboBoxCellType mcbtColumn_AREA, ref ComboBoxCellType mcbtColumn_UNIT)
        {

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                mcbtColumn_PLANT = new ComboBoxCellType();
                mcbtColumn_AREA = new ComboBoxCellType();
                mcbtColumn_UNIT = new ComboBoxCellType();

                lstPBS = null;

                lstPBS = proxy.ReadDistrictDynamicsPBSMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, lProjectMasterID, iListType);

                // 컬럼 세팅
                string[] arItems = { };
                string[] arItemsData = { };
                string sPBS = string.Empty;

                foreach (var vPbsSetting in mlstPBSSetting)
                {
                    sPBS = string.Empty;

                    List<EntityEDB_DistrictDynamic> lstPBS_Item = new List<EntityEDB_DistrictDynamic>();

                    if (iListType == (int)EnumNameCheck.ProcessEquipmentList)
                    {
                        sPBS = vPbsSetting.ProcessEquipment_Type;
                        if (string.IsNullOrEmpty(sPBS))
                            continue;

                        lstPBS_Item = lstPBS.Where(w => w.DistrictTypeOrders == int.Parse(vPbsSetting.ProcessEquipment_Type)).ToList();
                    }
                    else if (iListType == (int)EnumNameCheck.LineList)
                    {
                        sPBS = vPbsSetting.LineList_Type;
                        if (string.IsNullOrEmpty(sPBS))
                            continue;

                        lstPBS_Item = lstPBS.Where(w => w.DistrictTypeOrders == int.Parse(vPbsSetting.LineList_Type)).ToList();
                    }

                    arItems = new string[lstPBS_Item.Count + 1];
                    arItemsData = new string[lstPBS_Item.Count + 1];

                    arItemsData[0] = " ";
                    arItemsData[0] = " ";

                    int i = 1;
                    foreach (EntityEDB_DistrictDynamic etdb in lstPBS_Item)
                    {
                        string sID = etdb.DistrictDynamicID.ToString();
                        string sName = etdb.NameView.ToString();

                        arItemsData[i] = sID;
                        arItems[i] = sName;
                        i++;
                    }

                    if (sPBS == "100")
                    {
                        mcbtColumn_AREA.Items = arItems;
                        mcbtColumn_AREA.ItemData = arItemsData;
                        mcbtColumn_AREA.EditorValue = EditorValue.ItemData;

                        mcbtColumn_AREA.Editable = true;
                        mcbtColumn_AREA.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                        mcbtColumn_AREA.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;
                    }
                    else if (sPBS == "200")
                    {
                        mcbtColumn_UNIT.Items = arItems;
                        mcbtColumn_UNIT.ItemData = arItemsData;
                        mcbtColumn_UNIT.EditorValue = EditorValue.ItemData;

                        mcbtColumn_UNIT.Editable = true;
                        mcbtColumn_UNIT.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                        mcbtColumn_UNIT.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;
                    }

                }
            }
        }

        public void SetSPPIDPBSData(long lProjectMasterID)
        {
            mcbtColumn_SPPID_PLANT = new ComboBoxCellType();
            mcbtColumn_SPPID_AREA = new ComboBoxCellType();
            mcbtColumn_SPPID_UNIT = new ComboBoxCellType();
            mdtPBS_SPPID = null;
            mdtPBS_SPPID = new DataTable("Table1");

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                Information information = new Information();

                string sError = string.Empty;
                DataSet dsPBSConfig = proxy.GetSPPIDPBSStructure(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID);

                if (dsPBSConfig.Tables.Contains("PBS"))
                    mdtPBS_SPPID = dsPBSConfig.Tables["PBS"].Copy();

                // 컬럼 세팅
                string[] arItems = { };
                string[] arItemsData = { };

                bool bDIR_PATH = false;

                foreach (var vPBSSetting in mlstPBSSetting)
                {
                    bDIR_PATH = false;

                    foreach (DataTable dt in dsPBSConfig.Tables)
                    {
                        if (vPBSSetting.SPPID_Type == dt.TableName)
                        {
                            if (dt.Columns.Contains("DIR_PATH"))
                                bDIR_PATH = true;

                            arItems = new string[dt.Rows.Count + 1];
                            arItemsData = new string[dt.Rows.Count + 1];

                            arItems[0] = " ";
                            arItemsData[0] = " ";


                            // 여기쯤 수정
                            int i = 1;
                            foreach (DataRow dr in dt.Rows)
                            {
                                string sID = Util.NullToSpace(dr["ID"]);
                                string sName = Util.NullToSpace(dr["NAME"]);

                                //if (bDIR_PATH)
                                //{
                                //    string sDIR_PATH = Util.NullToSpace(dr["DIR_PATH"]);

                                //    if (!string.IsNullOrEmpty(sDIR_PATH))
                                //    {
                                //        // \여기\Unit <- Area 정보 가지고 오기
                                //        //int iIndexOf = sDIR_PATH.IndexOf('\\');
                                //        //int iLastIndexOf = sDIR_PATH.LastIndexOf('\\');

                                //        //sDIR_PATH = sDIR_PATH.Substring(iIndexOf,iLastIndexOf -1);
                                //        string[] arrDIR_PATH = sDIR_PATH.Split('\\');

                                //        if (arrDIR_PATH.Length > 1)
                                //            sName = sName + " [" + arrDIR_PATH[1] + "]";

                                //    }
                                //}

                                arItemsData[i] = sID;
                                arItems[i] = sName;
                                i++;
                            }

                            SetComboData(dt, mcbtColumn_SPPID_SITE, mcbtColumn_SPPID_PLANT, mcbtColumn_SPPID_AREA, mcbtColumn_SPPID_UNIT, arItems, arItemsData);

                            break;
                        }
                    }
                }

                dsPBSConfig.Dispose();
            }
        }

        public void SetSP3DPBSData(long lProjectMasterID)
        {
            mcbtColumn_SP3D_PLANT = new ComboBoxCellType();
            mcbtColumn_SP3D_AREA = new ComboBoxCellType();
            mcbtColumn_SP3D_UNIT = new ComboBoxCellType();
            mdtPBS_SP3D = null;
            mdtPBS_SP3D = new DataTable("Table1");

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                DataSet dsPBSConfig = proxy.GetSP3DPBSStructure(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID);

                if (dsPBSConfig.Tables.Contains("PBS"))
                    mdtPBS_SP3D = dsPBSConfig.Tables["PBS"].Copy();

                // 컬럼 세팅
                string[] arItems = { };
                string[] arItemsData = { };

                foreach (var vPBSSetting in mlstPBSSetting)
                {
                    foreach (DataTable dt in dsPBSConfig.Tables)
                    {
                        if (vPBSSetting.SP3D_Type == dt.TableName)
                        {
                            arItems = new string[dt.Rows.Count + 1];
                            arItemsData = new string[dt.Rows.Count + 1];

                            arItems[0] = " ";
                            arItemsData[0] = " ";

                            int i = 1;
                            foreach (DataRow dr in dt.Rows)
                            {
                                string sID = Util.NullToSpace(dr["ID"].ToString());
                                string sName = Util.NullToSpace(dr["NAME"].ToString());

                                arItemsData[i] = sID;
                                arItems[i] = sName;
                                i++;
                            }

                            SetComboData(dt, mcbtColumn_SP3D_SITE, mcbtColumn_SP3D_PLANT, mcbtColumn_SP3D_AREA, mcbtColumn_SP3D_UNIT, arItems, arItemsData);
                            break;
                        }
                    }
                }

                dsPBSConfig.Dispose();
            }
        }

        public void SetSPIPBSData(long lProjectMasterID)
        {
            mcbtColumn_SPI_PLANT = new ComboBoxCellType();
            mcbtColumn_SPI_AREA = new ComboBoxCellType();
            mcbtColumn_SPI_UNIT = new ComboBoxCellType();
            mdtPBS_SPI = null;
            mdtPBS_SPI = new DataTable("Table1");

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                string sError = string.Empty;
                DataSet dsPBSConfig = proxy.GetSPIPBSStructure(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID);

                if (dsPBSConfig.Tables.Contains("PBS"))
                    mdtPBS_SPI = dsPBSConfig.Tables["PBS"].Copy();

                //// 컬럼 세팅
                string[] arItems = { };
                string[] arItemsData = { };

                foreach (var vPBSSetting in mlstPBSSetting)
                {
                    foreach (DataTable dt in dsPBSConfig.Tables)
                    {
                        if (vPBSSetting.SPI_Type == dt.TableName)
                        {
                            arItems = new string[dt.Rows.Count + 1];
                            arItemsData = new string[dt.Rows.Count + 1];

                            arItems[0] = " ";
                            arItemsData[0] = " ";

                            int i = 1;
                            foreach (DataRow dr in dt.Rows)
                            {
                                string sID = Util.NullToSpace(dr["ID"].ToString());
                                string sName = Util.NullToSpace(dr["NAME"].ToString());

                                arItemsData[i] = sID;
                                arItems[i] = sName;
                                i++;
                            }

                            SetComboData(dt, mcbtColumn_SPI_SITE, mcbtColumn_SPI_PLANT, mcbtColumn_SPI_AREA, mcbtColumn_SPI_UNIT, arItems, arItemsData);
                            break;
                        }
                    }
                }

                dsPBSConfig.Dispose();
            }
        }

        private void SetComboData(DataTable dt, ComboBoxCellType Site, ComboBoxCellType Plant, ComboBoxCellType Area, ComboBoxCellType Unit, string[] arItems, string[] arItemsData)
        {
            if (dt.TableName.Equals(EDCS.WF.DSL.DataProperty.PBSMappingLevel.Site))
            {
                Site.Items = arItems;
                Site.ItemData = arItemsData;
                Site.EditorValue = EditorValue.ItemData;
                Site.Editable = true;
                Site.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                Site.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;

            }
            else if (dt.TableName.Equals(EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant))
            {
                Plant.Items = arItems;
                Plant.ItemData = arItemsData;
                Plant.EditorValue = EditorValue.ItemData;

                Plant.Editable = true;
                Plant.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                Plant.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;

            }
            else if (dt.TableName.Equals(EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area))
            {
                Area.Items = arItems;
                Area.ItemData = arItemsData;
                Area.EditorValue = EditorValue.ItemData;

                Area.Editable = true;
                Area.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                Area.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;

            }
            else if (dt.TableName.Equals(EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit))
            {
                Unit.Items = arItems;
                Unit.ItemData = arItemsData;
                Unit.EditorValue = EditorValue.ItemData;

                Unit.Editable = true;
                Unit.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
                Unit.AutoSearch = FarPoint.Win.AutoSearch.MultipleCharacter;
            }
        }

        private void FindChildCount(string sSPID, DataTable dt)
        {
            DataRow[] arRows = dt.Select(string.Format(@"PARENTID='{0}' and PLANTGROUPTYPE <> '0'", sSPID));
            if (arRows.Length == 0)
                m_iCount++;
            else
            {
                foreach (DataRow dr in arRows)
                {
                    FindChildCount(dr["SP_ID"].ToString(), dt);
                }
            }
        }
        private void initSpread()
        {
            SpreadUtil.InitSpread(fpSpreadMapping, marColumnWidth, marColumnName, mColumnLock, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadMapping.ActiveSheet.Rows.Count++;
            fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.PBSMappingID].Value = -1;
            fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text = SpreadFlag.Add;
        }

        private void setSpread()
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem != null)
            {
                // 컬럼 세팅
                string[] arItems = { };
                string[] arItemsData = { };

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                    mlstPBSSetting = proxy.ReadCommon_PBSSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID);

                    dicPBSSettingOrder.Clear();
                    if (mlstPBSSetting.Count > 0)
                    {
                        for (int j = 0; j < mlstPBSSetting.Count; j++)
                        {
                            dicPBSSettingOrder.Add(mlstPBSSetting[j].TypeName, j);
                        }
                    }
                }

                for (int i = 0; i < fpSpreadMapping.ActiveSheet.Columns.Count; i++)
                {
                    ComboBoxCellType cbtColumn = new ComboBoxCellType();

                    arItems = null;
                    arItemsData = null;

                    switch (i)
                    {
                        case (int)menColumn.Select:
                            CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                            fpSpreadMapping.ActiveSheet.Columns[i].CellType = ctCheckBox;
                            fpSpreadMapping.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                            break;
                        case (int)menColumn.Name:
                            TextCellType ctText = new TextCellType();
                            fpSpreadMapping.ActiveSheet.Columns[i].CellType = ctText;
                            //fpSpreadMapping.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                            break;
                        case (int)menColumn.Type:
                            if (mlstPBSSetting.Count > 0)
                            {
                                arItems = new string[mlstPBSSetting.Count];
                                arItemsData = new string[mlstPBSSetting.Count];

                                for (int j = 0; j < mlstPBSSetting.Count; j++)
                                {
                                    arItems[j] = mlstPBSSetting[j].TypeName;
                                    arItemsData[j] = mlstPBSSetting[j].TypeName;
                                }
                            }
                            break;
                    }

                    if (arItems != null)
                    {
                        cbtColumn.Items = arItems;
                        cbtColumn.ItemData = arItemsData;
                        cbtColumn.EditorValue = EditorValue.ItemData;
                        fpSpreadMapping.ActiveSheet.Columns[i].CellType = cbtColumn;
                    }
                }
            }
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            for (int i = fpSpreadMapping.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Select].Text;
                string sStatus = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Status].Text;

                if (sCheck.ToUpper().Equals("TRUE") && sStatus.Equals(SpreadFlag.Add))
                {
                    fpSpreadMapping.ActiveSheet.Rows.Remove(i, 1);
                }
                else if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Status].Text = SpreadFlag.Delete;
                }
            }

            SaveData();
        }

        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                FrmProgressStatus.FormShow();
                FrmProgressStatus.SetMarquee();
                FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
                FrmProgressStatus.ProgressPer = "Select PBS list.  Please wait!";

                InitControl_Setting();
                InitControl_Mapping();

                if (radDropDownListProject.SelectedItem != null)
                {
                    GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
                }

                FrmProgressStatus.FormClose();
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
            }
        }

        private void fpSpreadMapping_Change(object sender, ChangeEventArgs e)
        {
            this.fpSpreadMapping.Change -= new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadMapping_Change);
            CheckEditData();
            this.fpSpreadMapping.Change += new FarPoint.Win.Spread.ChangeEventHandler(this.fpSpreadMapping_Change);
        }

        private void fpSpreadMapping_ComboSelChange(object sender, EditorNotifyEventArgs e)
        {
            this.fpSpreadMapping.ComboSelChange -= new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpSpreadMapping_ComboSelChange);
            CheckEditData();
            this.fpSpreadMapping.ComboSelChange += new FarPoint.Win.Spread.EditorNotifyEventHandler(this.fpSpreadMapping_ComboSelChange);
        }

        private void SetSpreadData(List<EntityCommon_PBSMapping> lstPBSMapping)
        {
            fpSpreadMapping.ActiveSheet.Rows.Count = 0;

            foreach (EntityCommon_PBSMapping PBSMapping in lstPBSMapping)
            {
                int iRows = fpSpreadMapping.ActiveSheet.Rows.Count++;

                SetSpreadComboBox(iRows, PBSMapping.Type);
                fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.Status].Text = SpreadFlag.None;
                fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.PBSMappingID].Value = PBSMapping.PBSMappingID;

                fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.Type].Value = PBSMapping.Type;
                fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.Name].Text = GetPreviewText(PBSMapping.Type) + PBSMapping.Name;

                if (PBSMapping.ProcessEquipment_DistrictDynamicID != null)
                    fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.ProcessEquipment].Value = PBSMapping.ProcessEquipment_DistrictDynamicID.ToString();

                if (PBSMapping.LineList_DistrictDynamicID != null)
                    fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.LineList].Value = PBSMapping.LineList_DistrictDynamicID.ToString();

                if (PBSMapping.SPPID_SPID != null)
                    fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.SPPID].Value = PBSMapping.SPPID_SPID;

                if (PBSMapping.SP3D_OID != null)
                    fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.SP3D].Value = PBSMapping.SP3D_OID;

                if (PBSMapping.SPI_ID != null)
                    fpSpreadMapping.ActiveSheet.Cells[iRows, (int)menColumn.SPI].Value = PBSMapping.SPI_ID;
            }

            foreach (KeyValuePair<int, bool> kv in m_dicbProjectSettingInfo)
            {
                fpSpreadMapping.ActiveSheet.Columns[kv.Key].Visible = kv.Value;
            }
        }

        private void SetSpreadComboBox(int iRowIndex, string sType)
        {
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.Name].Text = "";

            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].Value = "";

            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.ProcessEquipment].Value = "";
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.LineList].Value = "";

            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].Value = "";
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].Value = "";

            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].CellType = new TextCellType();
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.ProcessEquipment].CellType = new TextCellType();
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.LineList].CellType = new TextCellType();
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].CellType = new TextCellType();
            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].CellType = new TextCellType();

            if (mlstPBSSetting != null)
            {
                EntityCommon_PBSSetting vTypes = mlstPBSSetting.Where(w => w.TypeName == sType).FirstOrDefault();

                if (vTypes != null)
                {
                    // Equipment_List
                    switch (vTypes.ProcessEquipment_Type)
                    {
                        case "100":
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.ProcessEquipment].CellType = mcbtColumn_ProcessEquipment_AREA;
                            break;
                        case "200":
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.ProcessEquipment].CellType = mcbtColumn_ProcessEquipment_UNIT;
                            break;
                    }

                    // LineList
                    switch (vTypes.ProcessEquipment_Type)
                    {
                        case "100":
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.LineList].CellType = mcbtColumn_LineList_AREA;
                            break;
                        case "200":
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.LineList].CellType = mcbtColumn_LineList_UNIT;
                            break;
                    }

                    // SPPID
                    switch (vTypes.SPPID_Type)
                    {
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Site:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].CellType = mcbtColumn_SPPID_SITE;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].CellType = mcbtColumn_SPPID_PLANT;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].CellType = mcbtColumn_SPPID_AREA;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPPID].CellType = mcbtColumn_SPPID_UNIT;
                            break;
                    }

                    // SP3D
                    switch (vTypes.SP3D_Type)
                    {
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Site:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].CellType = mcbtColumn_SP3D_SITE;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].CellType = mcbtColumn_SP3D_PLANT;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].CellType = mcbtColumn_SP3D_AREA;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SP3D].CellType = mcbtColumn_SP3D_UNIT;
                            break;
                    }

                    // SPI
                    switch (vTypes.SPI_Type)
                    {
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Site:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].CellType = mcbtColumn_SPI_SITE;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].CellType = mcbtColumn_SPI_PLANT;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].CellType = mcbtColumn_SPI_AREA;
                            break;
                        case EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit:
                            fpSpreadMapping.ActiveSheet.Cells[iRowIndex, (int)menColumn.SPI].CellType = mcbtColumn_SPI_UNIT;
                            break;
                    }

                }
            }

        }

        private void CheckEditData()
        {
            if (fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Text != SpreadFlag.Add && fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Text != SpreadFlag.Delete)
            {
                fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Text = SpreadFlag.Update;
            }

            if (fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.Type)
            {
                string sType = fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Type].Text;
                SetSpreadComboBox(fpSpreadMapping.ActiveSheet.ActiveRowIndex, sType);
            }
            else if (fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.ProcessEquipment
                || fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.LineList
                || fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.SPPID
                || fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.SP3D
                || fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.SPI
                )
            {
                if (fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text.Trim() == string.Empty)
                {
                    string sType = fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Type].Text;
                    fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text = GetPreviewText(sType) + fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, fpSpreadMapping.ActiveSheet.ActiveColumnIndex].Text;
                }
            }
            else if (fpSpreadMapping.ActiveSheet.ActiveColumnIndex == (int)menColumn.Name)
            {
                string sType = fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Type].Text;
                fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text = GetPreviewText(sType) + fpSpreadMapping.ActiveSheet.Cells[fpSpreadMapping.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text.Trim();
            }
        }

        private string GetPreviewText(string sType)
        {
            string sPreviewText = string.Empty;

            if (dicPBSSettingOrder.ContainsKey(sType))
            {
                int i = dicPBSSettingOrder[sType];
                switch (i)
                {
                    case 0:
                        sPreviewText = string.Empty;
                        break;
                    case 1:
                        sPreviewText = "   ";
                        break;
                    case 2:
                        sPreviewText = "      ";
                        break;
                }
            }
            return sPreviewText;
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadMapping.ActiveSheet;

            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;
                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if ((cr.Row + i) == 0)
                        return;
                    iFrom = cr.Row + i;
                    iTo = iFrom - 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = cr.Row - 1;
                sheetView.AddSelection(cr.Row - 1, cr.Column, cr.RowCount, cr.ColumnCount);

                for (int iRow = 0; iRow < fpSpreadMapping.ActiveSheet.RowCount; iRow++)
                {
                    if (fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text != SpreadFlag.Add && fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text != SpreadFlag.Delete)
                    {
                        fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text = SpreadFlag.Update;
                    }
                }
            }
            catch (Exception ex)
            {

            }
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadMapping.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;

                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if (cr.Row + cr.RowCount == sheetView.RowCount)
                        return;
                    iFrom = cr.Row + cr.RowCount - 1 - i;
                    iTo = iFrom + 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = iTo;
                sheetView.AddSelection(iTo, cr.Column, cr.RowCount, cr.ColumnCount);

                for (int iRow = 0; iRow < fpSpreadMapping.ActiveSheet.RowCount; iRow++)
                {
                    if (fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text != SpreadFlag.Add && fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text != SpreadFlag.Delete)
                    {
                        fpSpreadMapping.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text = SpreadFlag.Update;
                    }
                }


            }
            catch (Exception ex)
            {

            }
        }

        public override void SaveData()
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem != null)
            {
                if (radPageViewPBS.SelectedPage.Text == "Hierarchy")
                {
                    try
                    {
                        string sMessage = string.Empty;

                        if (!SavePBSSettings_Setting(ref sMessage))
                        {
                            MessageBox.Show(sMessage.ToString());
                        }
                        else
                        {
                            MessageBox.Show(Resources.Message10000, "Save PBS Settings", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
                else if (radPageViewPBS.SelectedPage.Text == "Mapping")
                {
                    try
                    {
                        EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                        string sMessage = string.Empty;
                        SavePBSData(cpProject, ref sMessage);

                        // 재 조회 
                        InitControl_Mapping();

                        if (sMessage.Length > 0)
                            MessageBox.Show(sMessage.ToString());
                        else
                        {
                            MessageBox.Show(this, Resources.Message10000, this.Text);
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
            }
        }

        public void SavePBSData(EntityCommon_ProjectMaster cpProject, ref string sMessage)
        {
            // 진행중
            sMessage = string.Empty;
            bool bResult = false;

            DataTable dt = new DataTable("Table1");
            dt.Columns.Add("Status");
            dt.Columns.Add("PBSMappingID", typeof(long));
            dt.Columns.Add("ProjectMasterID", typeof(long));
            dt.Columns.Add("Type");
            dt.Columns.Add("Name");
            dt.Columns.Add("NameView");
            dt.Columns.Add("ProcessEquipment_DistrictDynamicID", typeof(int));
            dt.Columns.Add("ProcessEquipment_Name");

            dt.Columns.Add("LineList_DistrictDynamicID", typeof(int));
            dt.Columns.Add("LineList_Name");

            dt.Columns.Add("SPPID_SPID");
            dt.Columns.Add("SPPID_Name");
            dt.Columns.Add("SP3D_OID");
            dt.Columns.Add("SP3D_Name");
            dt.Columns.Add("SPI_ID");
            dt.Columns.Add("SPI_Name");
            dt.Columns.Add("SortOrder", typeof(int));

            int iSortOrder = 1;

            for (int i = 0; i < fpSpreadMapping.ActiveSheet.RowCount; i++)
            {
                string sProperty = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Name].Text.Trim();
                string sType = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Type].Text;

                if (string.IsNullOrEmpty(sProperty)
                    && string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.ProcessEquipment].Text)
                    && string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.LineList].Text)
                    && string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPPID].Text)
                    && string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SP3D].Text)
                    && string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPI].Text)
                    )
                {
                    continue;
                }

                DataRow drNew = dt.NewRow();

                drNew["Status"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Status].Text;
                drNew["PBSMappingID"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.PBSMappingID].Text;
                drNew["ProjectMasterID"] = cpProject.ProjectMasterID;

                drNew["Type"] = sType;
                drNew["Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Name].Text.Trim();
                drNew["NameView"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.Name].Text;

                if (!string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.ProcessEquipment].Text.Trim()))
                {
                    drNew["ProcessEquipment_DistrictDynamicID"] = int.Parse(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.ProcessEquipment].Value.ToString());
                    drNew["ProcessEquipment_Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.ProcessEquipment].Text;
                }

                if (!string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.LineList].Text.Trim()))
                {
                    drNew["LineList_DistrictDynamicID"] = int.Parse(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.LineList].Value.ToString());
                    drNew["LineList_Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.LineList].Text;
                }

                if (!string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPPID].Text.Trim()))
                {
                    drNew["SPPID_SPID"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPPID].Value.ToString();
                    drNew["SPPID_Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPPID].Text;
                }

                if (!string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SP3D].Text.Trim()))
                {
                    drNew["SP3D_OID"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SP3D].Value.ToString();
                    drNew["SP3D_Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SP3D].Text;
                }

                if (!string.IsNullOrEmpty(fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPI].Text.Trim()))
                {
                    drNew["SPI_ID"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPI].Value.ToString();
                    drNew["SPI_Name"] = fpSpreadMapping.ActiveSheet.Cells[i, (int)menColumn.SPI].Text;
                }

                drNew["SortOrder"] = iSortOrder;
                dt.Rows.Add(drNew);
                iSortOrder++;
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bResult = proxy.SaveCommon_PBSMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID, dt, ref sMessage);
            }

        }

        #endregion

        private void radPageViewPBS_SelectedPageChanging(object sender, RadPageViewCancelEventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Select PBS list.  Please wait!";

            try
            {
                if (e.Page.Text == "Hierarchy")
                {
                    InitControl_Setting();
                }
                else if (e.Page.Text == "Mapping")
                {
                    InitControl_Mapping();
                }
                else if (e.Page.Text == "View")
                {
                    if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem != null)
                    {
                        EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;

                        var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy as IDisposable)
                        {
                            List<EntityCommon_PBSMapping> lstPBSMapping = proxy.ReadCommon_PBSMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID);
                            SetTreeView(lstPBSMapping);
                        }

                        //GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
                    }
                }

                FrmProgressStatus.FormClose();
            }
            catch (System.ServiceModel.FaultException ex)
            {
                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }

        private void InitControl_Mapping()
        {
            if (radDropDownListProject.SelectedItem != null)
            {
                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                GetToolUsingInfo(cpProject);

                initSpread();
                setSpread();

                string sError = string.Empty;
                InitSpreadCombobox(cpProject);
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntityCommon_PBSMapping> lstPBSMapping = proxy.ReadCommon_PBSMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID);
                    SetSpreadData(lstPBSMapping);
                    SetTreeView(lstPBSMapping);
                }
            }
        }

        private void GetToolUsingInfo(EntityCommon_ProjectMaster cpProject)
        {
            m_dicbProjectSettingInfo = new Dictionary<int, bool>();

            m_dicbProjectSettingInfo.Add((int)menColumn.ProcessEquipment, false);
            m_dicbProjectSettingInfo.Add((int)menColumn.LineList, false);

            if (cpProject.EDBFlag == true)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var edbProject = proxy.ReadProjectMasterByEntity(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cpProject.ProjectMasterID);

                    if (edbProject != null)
                    {
                        m_dicbProjectSettingInfo[(int)menColumn.ProcessEquipment] = edbProject.ProcessEquipmentFlag;
                        m_dicbProjectSettingInfo[(int)menColumn.LineList] = edbProject.LineFlag;
                    }
                }
            }

            m_dicbProjectSettingInfo.Add((int)menColumn.SPPID, cpProject.SPPIDFlag);
            m_dicbProjectSettingInfo.Add((int)menColumn.SP3D, cpProject.SP3DFlag);
            m_dicbProjectSettingInfo.Add((int)menColumn.SPI, cpProject.SPIFlag);

            mProjectSettingMaxCount = m_dicbProjectSettingInfo.Where(p => p.Value == true).Count();
        }

        private void SetTreeView(List<EntityCommon_PBSMapping> lstPBSMapping)
        {

            int i = 0;

            radSplitContainer2.SuspendLayout();

            foreach (Control ctl in radSplitContainer2.Controls)
            {
                SplitPanel sp = ctl as SplitPanel;

                if (sp != null)
                {
                    sp.SizeInfo.SizeMode = Telerik.WinControls.UI.Docking.SplitPanelSizeMode.Auto;

                    if (m_dicbProjectSettingInfo.ContainsKey(int.Parse(sp.Text)))
                    {
                        if (!m_dicbProjectSettingInfo[int.Parse(sp.Text)])
                        {
                            radSplitContainer2.SplitPanels[i].Hide();
                            radSplitContainer2.SplitPanels[i].Collapsed = true;
                            //radSplitContainer2.SplitPanels[i].Width = 0;
                            //this.radSplitContainer2.Splitters[i - 1].Visibility = Telerik.WinControls.ElementVisibility.Collapsed;
                        }
                        else
                        {
                            radSplitContainer2.SplitPanels[i].Show();
                            radSplitContainer2.SplitPanels[i].Collapsed = false;
                            //this.radSplitContainer2.Splitters[i - 1].Visibility = Telerik.WinControls.ElementVisibility.Visible;
                        }

                    }
                }

                i++;
            }

            radSplitContainer2.ResumeLayout(true);

            SetTreeView_Default(lstPBSMapping);
            SetTreeView_SPPID(lstPBSMapping);
            SetTreeView_SP3D(lstPBSMapping); // sp3d
            SetTreeView_SPI(lstPBSMapping);
            SetTreeView_EDB(ToolType.ProcessEquipmentList, radTreeViewProcessEquipmentList, lstPBSMapping, mlstPBS_EquipmentList, radLabelAllMapping_ProcessEquipmentList, radLabelPartMapping_ProcessEquipmentList, radLabelNotMapping_ProcessEquipmentList);
            SetTreeView_EDB(ToolType.LineList, radTreeViewLineList, lstPBSMapping, mlstPBS_LineList, radLabelAllMapping_LineList, radLabelPartMapping_LineList, radLabelNotMapping_LineList);
        }

        private void SetPBSColor(List<EntityCommon_PBSMapping> lstPBSMapping, string sToolType, string sNodeType, string sNodeName, string sNodeText, ref RadTreeNode node, ref int iMappingCount, ref int iPartMappingCount, ref int iNotMappingCount)
        {
            List<EntityCommon_PBSMapping> pbs = new List<EntityCommon_PBSMapping>();

            if (sToolType == ToolType.ProcessEquipmentList)
            {
                pbs = lstPBSMapping.Where(w => w.Type == sNodeType && w.ProcessEquipment_DistrictDynamicID == int.Parse(sNodeName)).ToList();
            }
            if (sToolType == ToolType.LineList)
            {
                pbs = lstPBSMapping.Where(w => w.Type == sNodeType && w.LineList_DistrictDynamicID == int.Parse(sNodeName)).ToList();
            }
            else if (sToolType == ToolType.SPPID)
            {
                pbs = lstPBSMapping.Where(w => w.Type == sNodeType && w.SPPID_SPID == sNodeName).ToList();
            }
            else if (sToolType == ToolType.SP3D)
            {
                pbs = lstPBSMapping.Where(w => w.Type == sNodeType && (w.SP3D_OID + w.SP3D_Name) == sNodeName).ToList();
            }
            else if (sToolType == ToolType.SPI)
            {
                pbs = lstPBSMapping.Where(w => w.Type == sNodeType && (w.SPI_ID + w.SPI_Name)== sNodeName).ToList();
            }

            SetColor(pbs, sNodeName, sNodeText, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);

            pbs.Clear();
        }

        private void SetColor(List<EntityCommon_PBSMapping> pbs, string sNodeName, string sNodeText, ref RadTreeNode node, ref int iMappingCount, ref int iPartMappingCount, ref int iNotMappingCount)
        {
            Dictionary<string, bool> dicMapping = new Dictionary<string, bool>();

            dicMapping.Add("ProcessEquipment", false);
            dicMapping.Add("LineList", false);
            dicMapping.Add("SPPID", false);
            dicMapping.Add("SP3D", false);
            dicMapping.Add("SPI", false);

            node.Image = Resources.smallRed;
            node.Name = sNodeName;

            string sName = string.Empty;

            List<string> lstName = new List<string>();

            if (pbs.Count > 0)
            {
                foreach (EntityCommon_PBSMapping cpbs in pbs)
                {
                    if (cpbs.ProcessEquipment_DistrictDynamicID != null)
                        dicMapping["ProcessEquipment"] = true;

                    if (cpbs.LineList_DistrictDynamicID != null)
                        dicMapping["LineList"] = true;

                    if (cpbs.SPPID_SPID != null)
                        dicMapping["SPPID"] = true;

                    if (cpbs.SP3D_OID != null)
                        dicMapping["SP3D"] = true;

                    if (cpbs.SPI_ID != null)
                        dicMapping["SPI"] = true;

                    if (!lstName.Contains(cpbs.Name))
                        lstName.Add(cpbs.Name);
                }

                int iCount = 0;

                foreach (KeyValuePair<string, bool> kv in dicMapping)
                {
                    if (kv.Value)
                        iCount++;
                }

                if (mProjectSettingMaxCount == iCount)
                {
                    iMappingCount++;
                    node.Image = Resources.smallGreen;
                }
                else
                {
                    iPartMappingCount++;
                    node.Image = Resources.smallYellow;
                }

                bool bfirst = true;
                foreach (string name in lstName)
                {
                    if (bfirst)
                    {
                        sName = name;
                        bfirst = false;
                    }
                    else
                        sName += " / " + name;
                }

                node.Text = string.Format("{0} - [{1}]", sNodeText, sName);

            }
            else
            {
                node.Text = sNodeText;
                iNotMappingCount++;
            }

            dicMapping.Clear();
        }

        private void SetTreeView_Default(List<EntityCommon_PBSMapping> lstPBSMapping)
        {
            radTreeViewDefault.Nodes.Clear();
            radTreeViewDefault.Font = Information.FONT_REGULAR;

            RadTreeNode node = new RadTreeNode();

            int iMappingCount = 0;
            int iPartMappingCount = 0;
            int iNotMappingCount = 0;

            if (lstPBSMapping.Count > 0)
            {
                RadTreeNode nodePlant = new RadTreeNode();
                RadTreeNode nodeArea = new RadTreeNode();

                foreach (EntityCommon_PBSMapping cpbs in lstPBSMapping)
                {
                    List<bool> lstCount = new List<bool>();

                    if (cpbs.ProcessEquipment_DistrictDynamicID != null)
                        lstCount.Add(true);
                    else
                        lstCount.Add(false);

                    if (cpbs.LineList_DistrictDynamicID != null)
                        lstCount.Add(true);
                    else
                        lstCount.Add(false);

                    if (cpbs.SPPID_SPID != null)
                        lstCount.Add(true);
                    else
                        lstCount.Add(false);

                    if (cpbs.SP3D_OID != null)
                        lstCount.Add(true);
                    else
                        lstCount.Add(false);

                    if (cpbs.SPI_ID != null)
                        lstCount.Add(true);
                    else
                        lstCount.Add(false);

                    int iCount = (from num in lstCount
                                  where num == true
                                  select num).Count();

                    Bitmap bp = Resources.smallRed;

                    if (mProjectSettingMaxCount == iCount)
                    {
                        iMappingCount++;
                        bp = Resources.smallGreen;
                    }
                    else if (iCount > 0)
                    {
                        iPartMappingCount++;
                        bp = Resources.smallYellow;
                    }
                    else
                    {
                        iNotMappingCount++;
                    }

                    if (dicPBSSettingOrder.ContainsKey(cpbs.Type))
                    {
                        int i = dicPBSSettingOrder[cpbs.Type];

                        switch (i)
                        {
                            case 0:
                                nodePlant = new RadTreeNode();
                                nodePlant.Font = Information.FONT_REGULAR;
                                nodePlant.Image = bp;
                                nodePlant.Text = cpbs.Name;
                                radTreeViewDefault.Nodes.Add(nodePlant);
                                break;
                            case 1:
                                nodeArea = new RadTreeNode();
                                nodeArea.Font = Information.FONT_REGULAR;
                                nodeArea.Image = bp;
                                nodeArea.Text = cpbs.Name;
                                nodePlant.Nodes.Add(nodeArea);
                                break;
                            case 2:
                                node = new RadTreeNode();
                                node.Font = Information.FONT_REGULAR;
                                node.Image = bp;
                                node.Text = cpbs.Name;
                                nodeArea.Nodes.Add(node);
                                break;

                        }
                    }

                    lstCount.Clear();
                }
            }
            else
            {
                node = new RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewSPPID.Nodes.Add(node);
            }

            radLabelAllMapping_Default.Text = string.Format("All Mapping Count : {0}", iMappingCount.ToString());
            radLabelPartMapping_Default.Text = string.Format("Part Mapping Count : {0}", iPartMappingCount.ToString());
            radLabelNotMapping_Default.Text = string.Format("Not Mapping Count : {0}", iNotMappingCount.ToString());

            radTreeViewDefault.ExpandAll();
        }

        private void SetTreeView_SPPID(List<EntityCommon_PBSMapping> lstPBSMapping)
        {
            radTreeViewSPPID.Nodes.Clear();
            radTreeViewSPPID.Font = Information.FONT_REGULAR;

            RadTreeNode node = new RadTreeNode();

            int iMappingCount = 0;
            int iPartMappingCount = 0;
            int iNotMappingCount = 0;

            if (mdtPBS_SPPID.Rows.Count > 0)
            {
                foreach (DataRow dataRow in mdtPBS_SPPID.Rows)
                {
                    node = new RadTreeNode();
                    node.Font = Information.FONT_REGULAR;
                    string sName = dataRow["SP_ID"].ToString().Trim();
                    string sText = dataRow["NAME"].ToString();

                    RadTreeNode nodParent = radTreeViewSPPID.GetNodeByName(dataRow["PARENTID"].ToString().Trim());
                    string sPlantGroupType = dataRow["PLANTGROUPTYPE"].ToString();

                    // connection Icon
                    switch (sPlantGroupType)
                    {
                        case CONST_SITE_PLANT_TYPE:
                            SetPBSColor(lstPBSMapping, ToolType.SPPID, EDCS.WF.DSL.DataProperty.PBSMappingLevel.Site, sName, sText, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                        case CONST_PLANT_PLANT_TYPE:
                            SetPBSColor(lstPBSMapping, ToolType.SPPID, EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant, sName, sText, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                        case CONST_AREA_PLANT_TYPE:
                            SetPBSColor(lstPBSMapping, ToolType.SPPID, EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area, sName, sText, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                        case CONST_UNIT_PLANT_TYPE:
                            SetPBSColor(lstPBSMapping, ToolType.SPPID, EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit, sName, sText, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                    }

                    if (nodParent == null)
                    {
                        radTreeViewSPPID.Nodes.Add(node);
                    }
                    else
                    {
                        nodParent.Nodes.Add(node);
                    }
                }
            }
            else
            {
                node = new RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewSPPID.Nodes.Add(node);
            }

            radLabelAllMapping_SPPID.Text = string.Format("All Mapping Count : {0}", iMappingCount.ToString());
            radLabelPartMapping_SPPID.Text = string.Format("Part Mapping Count : {0}", iPartMappingCount.ToString());
            radLabelNotMapping_SPPID.Text = string.Format("Not Mapping Count : {0}", iNotMappingCount.ToString());

            radTreeViewSPPID.ExpandAll();
        }

        private void SetTreeView_SP3D(List<EntityCommon_PBSMapping> lstPBSMapping)
        {
            radTreeViewSP3D.Nodes.Clear();
            radTreeViewSP3D.Font = Information.FONT_REGULAR;

            RadTreeNode node = new RadTreeNode();

            int iMappingCount = 0;
            int iPartMappingCount = 0;
            int iNotMappingCount = 0;

            if (mdtPBS_SP3D.Rows.Count > 0)
            {
                string sPBSMappingLevel_Plant = string.Empty;
                string sPBSMappingLevel_Area = string.Empty;
                string sPBSMappingLevel_Unit = string.Empty;

                if (mlstPBSSetting != null)
                {
                    var vPlant = mlstPBSSetting.Where(w => w.SP3D_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant).FirstOrDefault();
                    var vArea = mlstPBSSetting.Where(w => w.SP3D_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area).FirstOrDefault();
                    var vUnit = mlstPBSSetting.Where(w => w.SP3D_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit).FirstOrDefault();

                    if (vPlant != null)
                        sPBSMappingLevel_Plant = vPlant.TypeName;

                    if (vArea != null)
                        sPBSMappingLevel_Area = vArea.TypeName;

                    if (vUnit != null)
                        sPBSMappingLevel_Unit = vUnit.TypeName;
                }

                foreach (DataRow dataRow in mdtPBS_SP3D.Rows)
                {
                    string sPlantID = dataRow["PLANT_OID"].ToString();
                    string sPLANT = dataRow["PLANT"].ToString();
                    string sAREA_OID = dataRow["AREA_OID"].ToString();
                    string sAREA = dataRow["AREA"].ToString();
                    string sUNIT_OID = dataRow["UNIT_OID"].ToString();
                    string sUNIT = dataRow["UNIT"].ToString();

                    string sItemPlantID = sPlantID + sPLANT;
                    string sItemAreaID = sAREA_OID + sAREA;
                    string sItemUnitID = sUNIT_OID + sUNIT;

                    // Plant

                    RadTreeNode nodPlant = radTreeViewSP3D.GetNodeByName(sItemPlantID);
                    if (nodPlant == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        SetPBSColor(lstPBSMapping, ToolType.SP3D, sPBSMappingLevel_Plant, sItemPlantID, sPLANT, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                        radTreeViewSP3D.Nodes.Add(node);
                        nodPlant = node;
                    }

                    // Area
                    RadTreeNode nodArea = radTreeViewSP3D.GetNodeByName(sItemAreaID);
                    if (nodArea == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        node.Name = sAREA;
                        node.Text = sAREA;
                        SetPBSColor(lstPBSMapping, ToolType.SP3D, sPBSMappingLevel_Area, sItemAreaID, sAREA, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                        nodPlant.Nodes.Add(node);
                        nodArea = node;
                    }

                    RadTreeNode nodUnit = radTreeViewSP3D.GetNodeByName(sItemUnitID);
                    if (nodUnit == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        node.Name = sUNIT;
                        node.Text = sUNIT;
                        SetPBSColor(lstPBSMapping, ToolType.SP3D, sPBSMappingLevel_Unit, sItemUnitID, sUNIT, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);

                        nodArea.Nodes.Add(node);
                    }
                }
            }
            else
            {
                node = new RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewSP3D.Nodes.Add(node);
            }

            radLabelAllMapping_SP3D.Text = string.Format("All Mapping Count : {0}", iMappingCount.ToString());
            radLabelPartMapping_SP3D.Text = string.Format("Part Mapping Count : {0}", iPartMappingCount.ToString());
            radLabelNotMapping_SP3D.Text = string.Format("Not Mapping Count : {0}", iNotMappingCount.ToString());

            radTreeViewSP3D.ExpandAll();
        }


        private void SetTreeView_SPI(List<EntityCommon_PBSMapping> lstPBSMapping)
        {
            radTreeViewSPI.Nodes.Clear();
            radTreeViewSPI.Font = Information.FONT_REGULAR;

            RadTreeNode node = new RadTreeNode();

            int iMappingCount = 0;
            int iPartMappingCount = 0;
            int iNotMappingCount = 0;

            if (mdtPBS_SPI.Rows.Count > 0)
            {
                string sPBSMappingLevel_Plant = string.Empty;
                string sPBSMappingLevel_Area = string.Empty;
                string sPBSMappingLevel_Unit = string.Empty;

                if (mlstPBSSetting != null)
                {
                    var vPlant = mlstPBSSetting.Where(w => w.SPI_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Plant).FirstOrDefault();
                    var vArea = mlstPBSSetting.Where(w => w.SPI_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Area).FirstOrDefault();
                    var vUnit = mlstPBSSetting.Where(w => w.SPI_Type == EDCS.WF.DSL.DataProperty.PBSMappingLevel.Unit).FirstOrDefault();

                    if (vPlant != null)
                        sPBSMappingLevel_Plant = vPlant.TypeName;

                    if (vArea != null)
                        sPBSMappingLevel_Area = vArea.TypeName;

                    if (vUnit != null)
                        sPBSMappingLevel_Unit = vUnit.TypeName;
                }

                foreach (DataRow dataRow in mdtPBS_SPI.Rows)
                {
                    string sPlantID = dataRow["PLANT_ID"].ToString();
                    string sPLANT = dataRow["PLANT_NAME"].ToString();
                    string sAREA_OID = dataRow["AREA_ID"].ToString();
                    string sAREA = dataRow["AREA_NAME"].ToString();
                    string sUNIT_OID = dataRow["UNIT_ID"].ToString();
                    string sUNIT = dataRow["UNIT_NAME"].ToString();

                    string sItemPlantID = sPlantID + sPLANT;
                    string sItemAreaID = sAREA_OID + sAREA;
                    string sItemUnitID = sUNIT_OID + sUNIT;

                    // Plant
                    RadTreeNode nodPlant = radTreeViewSPI.GetNodeByName(sItemPlantID);
                    if (nodPlant == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;

                        SetPBSColor(lstPBSMapping, ToolType.SPI, sPBSMappingLevel_Plant, sItemPlantID, sPLANT, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                        radTreeViewSPI.Nodes.Add(node);
                        nodPlant = node;
                    }

                    // Area
                    RadTreeNode nodArea = radTreeViewSPI.GetNodeByName(sItemAreaID);
                    if (nodArea == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;

                        SetPBSColor(lstPBSMapping, ToolType.SPI, sPBSMappingLevel_Area, sItemAreaID, sAREA, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                        nodPlant.Nodes.Add(node);
                        nodArea = node;
                    }

                    RadTreeNode nodUnit = radTreeViewSPI.GetNodeByName(sItemUnitID);
                    if (nodUnit == null)
                    {
                        node = new RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        SetPBSColor(lstPBSMapping, ToolType.SPI, sPBSMappingLevel_Unit, sItemUnitID, sUNIT, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                        nodArea.Nodes.Add(node);
                    }
                }
            }
            else
            {
                node = new RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewSPI.Nodes.Add(node);
            }

            radLabelAllMapping_SPI.Text = string.Format("All Mapping Count : {0}", iMappingCount.ToString());
            radLabelPartMapping_SPI.Text = string.Format("Part Mapping Count : {0}", iPartMappingCount.ToString());
            radLabelNotMapping_SPI.Text = string.Format("Not Mapping Count : {0}", iNotMappingCount.ToString());

            radTreeViewSPI.ExpandAll();
        }

        private void SetTreeView_EDB(string sToolType, RadTreeView radTreeViewItem, List<EntityCommon_PBSMapping> lstPBSMapping, List<EntityEDB_DistrictDynamic> lstPBS, RadLabel radLabelAllMapping, RadLabel radLabelPartMapping, RadLabel radLabelNotMapping)
        {
            radTreeViewItem.Nodes.Clear();
            radTreeViewItem.Font = Information.FONT_REGULAR;

            RadTreeNode node = new RadTreeNode();

            int iMappingCount = 0;
            int iPartMappingCount = 0;
            int iNotMappingCount = 0;

            if (lstPBS != null)
            {
                string sPBSMappingLevel_Area = string.Empty;
                string sPBSMappingLevel_Unit = string.Empty;

                if (mlstPBSSetting != null)
                {
                    if (sToolType == ToolType.ProcessEquipmentList)
                    {
                        var vArea = mlstPBSSetting.Where(w => w.ProcessEquipment_Type == "100").FirstOrDefault();
                        var vUnit = mlstPBSSetting.Where(w => w.ProcessEquipment_Type == "200").FirstOrDefault();

                        if (vArea != null)
                            sPBSMappingLevel_Area = vArea.TypeName;

                        if (vUnit != null)
                            sPBSMappingLevel_Unit = vUnit.TypeName;
                    }
                    else
                    {
                        var vArea = mlstPBSSetting.Where(w => w.LineList_Type == "100").FirstOrDefault();
                        var vUnit = mlstPBSSetting.Where(w => w.LineList_Type == "200").FirstOrDefault();

                        if (vArea != null)
                            sPBSMappingLevel_Area = vArea.TypeName;

                        if (vUnit != null)
                            sPBSMappingLevel_Unit = vUnit.TypeName;
                    }
                }

                foreach (EntityEDB_DistrictDynamic districtDynamic in lstPBS)
                {
                    node = new RadTreeNode();
                    node.Font = Information.FONT_REGULAR;

                    RadTreeNode nodParent = radTreeViewItem.GetNodeByName(districtDynamic.ParentID.ToString());

                    string sDistrictTypeOrders = districtDynamic.DistrictTypeOrders.ToString();

                    // connection Icon
                    switch (sDistrictTypeOrders)
                    {
                        case "100":
                            SetPBSColor(lstPBSMapping, sToolType, sPBSMappingLevel_Area, districtDynamic.DistrictDynamicID.ToString(), districtDynamic.NameView, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                        case "200":
                            SetPBSColor(lstPBSMapping, sToolType, sPBSMappingLevel_Unit, districtDynamic.DistrictDynamicID.ToString(), districtDynamic.NameView, ref node, ref iMappingCount, ref iPartMappingCount, ref iNotMappingCount);
                            break;
                    }

                    if (nodParent == null)
                    {
                        radTreeViewItem.Nodes.Add(node);
                    }
                    else
                    {
                        nodParent.Nodes.Add(node);
                    }
                }
            }
            else
            {
                node = new RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewItem.Nodes.Add(node);
            }

            radLabelAllMapping.Text = string.Format("All Mapping Count : {0}", iMappingCount.ToString());
            radLabelPartMapping.Text = string.Format("Part Mapping Count : {0}", iPartMappingCount.ToString());
            radLabelNotMapping.Text = string.Format("Not Mapping Count : {0}", iNotMappingCount.ToString());

            radTreeViewItem.ExpandAll();
        }

        private void InitControl_Setting()
        {
            if (radDropDownListProject.SelectedItem != null)
            {
                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;

                // 사용 프로젝트 
                GetToolUsingInfo(cpProject);
                InitSpreadCombobox_Setting();
                initSpread_Setting();
                setSpread_Setting();

                string sError = string.Empty;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntityCommon_PBSSetting> lstPBSSetting = proxy.ReadCommon_PBSSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID);
                    SetSpreadData_Setting(lstPBSSetting);
                }
            }
        }

        private void initSpread_Setting()
        {
            SpreadUtil.InitSpread(fpSpreadPBSSetting, mColumnWidth_Setting, mColumnName_Setting, mColumnLock_Setting, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void setSpread_Setting()
        {
            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadPBSSetting.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                arItems = null;
                arItemsData = null;
                int j = 0;

                switch (i)
                {
                    case (int)menColumn_Setting.Select:
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadPBSSetting.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadPBSSetting.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn_Setting.PBSType:
                        arItems = marPBSType_Setting;
                        arItemsData = marPBSType_Setting;
                        break;
                    case (int)menColumn_Setting.ProcessEquipment:
                        arItems = new string[mdicPBSTypes_EDB_Setting.Count];
                        arItemsData = new string[mdicPBSTypes_EDB_Setting.Count];

                        j = 0;
                        foreach (var vedb in mdicPBSTypes_EDB_Setting)
                        {
                            arItems[j] = vedb.Value;
                            arItemsData[j] = vedb.Key;
                            j++;
                        }
                        break;
                    case (int)menColumn_Setting.LineList:
                        arItems = new string[mdicPBSTypes_EDB_Setting.Count];
                        arItemsData = new string[mdicPBSTypes_EDB_Setting.Count];
                        j = 0;
                        foreach (var vedb in mdicPBSTypes_EDB_Setting)
                        {
                            arItems[j] = vedb.Value;
                            arItemsData[j] = vedb.Key;
                            j++;
                        }
                        break;
                    case (int)menColumn_Setting.SPPID:
                        arItems = marPBSTypes_SPPID_Setting;
                        arItemsData = marPBSTypes_SPPID_Setting;
                        break;
                    case (int)menColumn_Setting.SP3D:
                        arItems = marPBSTypes_SP3D_Setting;
                        arItemsData = marPBSTypes_SP3D_Setting;
                        break;
                    case (int)menColumn_Setting.SPI:
                        arItems = marPBSTypes_SPI_Setting;
                        arItemsData = marPBSTypes_SPI_Setting;
                        break;

                    case (int)menColumn_Setting.PBSSettingID:
                    case (int)menColumn_Setting.ProjectMasterID:
                    case (int)menColumn_Setting.SortOrder:
                    case (int)menColumn_Setting.DeleteFlag:
                        fpSpreadPBSSetting.ActiveSheet.Columns[i].Visible = false;
                        break;

                }

                // 추후 다른 컬럼 디자인시 사용할것
                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadPBSSetting.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }

            foreach (KeyValuePair<int, bool> kv in m_dicbProjectSettingInfo_Setting)
            {
                fpSpreadPBSSetting.ActiveSheet.Columns[kv.Key].Visible = kv.Value;
            }

        }

        private void InitSpreadCombobox_Setting()
        {
            mcbtPBS_TYPE_Setting = new ComboBoxCellType();
            mcbtPBS_TYPE_Setting.Items = marPBSType_Setting;
            mcbtPBS_TYPE_Setting.ItemData = marPBSType_Setting;
            mcbtPBS_TYPE_Setting.EditorValue = EditorValue.ItemData;

            //[ProcessEquipment_Type]
            mcbtProcessEQColumn_TYPE_Setting = new ComboBoxCellType();
            string[] arItems_ProcEQ = new string[mdicPBSTypes_EDB_Setting.Count];
            string[] arItemsData_ProcEQ = new string[mdicPBSTypes_EDB_Setting.Count];

            int i = 0;
            foreach (var vedb in mdicPBSTypes_EDB_Setting)
            {
                arItems_ProcEQ[i] = vedb.Value;
                arItemsData_ProcEQ[i] = vedb.Key;
                i++;
            }

            mcbtProcessEQColumn_TYPE_Setting.Items = arItems_ProcEQ;
            mcbtProcessEQColumn_TYPE_Setting.ItemData = arItemsData_ProcEQ;
            mcbtProcessEQColumn_TYPE_Setting.EditorValue = EditorValue.ItemData;

            //[LineList_Type]
            mcbtLineColumn_TYPE_Setting = new ComboBoxCellType();
            string[] arItems_Line = new string[mdicPBSTypes_EDB_Setting.Count];
            string[] arItemsData_Line = new string[mdicPBSTypes_EDB_Setting.Count];

            i = 0;
            foreach (var vedb in mdicPBSTypes_EDB_Setting)
            {
                arItems_Line[i] = vedb.Value;
                arItemsData_Line[i] = vedb.Key;
                i++;
            }

            mcbtLineColumn_TYPE_Setting.Items = arItems_Line;
            mcbtLineColumn_TYPE_Setting.ItemData = arItemsData_Line;
            mcbtLineColumn_TYPE_Setting.EditorValue = EditorValue.ItemData;

            //[SPPID_Type]
            mcbtSPPIDColumn_TYPE_Setting = new ComboBoxCellType();
            mcbtSPPIDColumn_TYPE_Setting.Items = marPBSTypes_SPPID_Setting;
            mcbtSPPIDColumn_TYPE_Setting.ItemData = marPBSTypes_SPPID_Setting;
            mcbtSPPIDColumn_TYPE_Setting.EditorValue = EditorValue.ItemData;

            //[SP3D_Type]
            mcbtSP3DColumn_TYPE_Setting = new ComboBoxCellType();
            mcbtSP3DColumn_TYPE_Setting.Items = marPBSTypes_SP3D_Setting;
            mcbtSP3DColumn_TYPE_Setting.ItemData = marPBSTypes_SP3D_Setting;
            mcbtSP3DColumn_TYPE_Setting.EditorValue = EditorValue.ItemData;

            //[SPI_Type]
            mcbtSPIColumn_TYPE_Setting = new ComboBoxCellType();
            mcbtSPIColumn_TYPE_Setting.Items = marPBSTypes_SPI_Setting;
            mcbtSPIColumn_TYPE_Setting.ItemData = marPBSTypes_SPI_Setting;
            mcbtSPIColumn_TYPE_Setting.EditorValue = EditorValue.ItemData;

        }

        private void SetSpreadData_Setting(List<EntityCommon_PBSSetting> lstPBSSetting)
        {
            fpSpreadPBSSetting.ActiveSheet.Rows.Count = 0;

            int iRows = 0;
            // 가져온 데이터 order by로 정렬할것
            foreach (EntityCommon_PBSSetting PBSSetting in lstPBSSetting)
            {
                iRows = fpSpreadPBSSetting.ActiveSheet.Rows.Count++;

                fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.PBSType].Value = PBSSetting.TypeName.ToString();
                fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.PBSSettingID].Value = PBSSetting.PBSSettingID.ToString();
                fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.ProjectMasterID].Value = PBSSetting.ProjectMasterID.ToString();

                if (PBSSetting.ProcessEquipment_Type != null)
                    fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.ProcessEquipment].Value = PBSSetting.ProcessEquipment_Type.ToString();
                if (PBSSetting.LineList_Type != null)
                    fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.LineList].Value = PBSSetting.LineList_Type.ToString();

                if (PBSSetting.SPPID_Type != null)
                    fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.SPPID].Value = PBSSetting.SPPID_Type.ToString();
                if (PBSSetting.SP3D_Type != null)
                    fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.SP3D].Value = PBSSetting.SP3D_Type.ToString();
                if (PBSSetting.SPI_Type != null)
                    fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.SPI].Value = PBSSetting.SPI_Type.ToString();

                fpSpreadPBSSetting.ActiveSheet.Cells[iRows, (int)menColumn_Setting.SortOrder].Value = PBSSetting.SortOrder.ToString();

                iRows++;
            }
        }

        private void radButtonUp_Click_1(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadPBSSetting.ActiveSheet;

            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;
                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if ((cr.Row + i) == 0)
                        return;
                    iFrom = cr.Row + i;
                    iTo = iFrom - 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = cr.Row - 1;
                sheetView.AddSelection(cr.Row - 1, cr.Column, cr.RowCount, cr.ColumnCount);

            }
            catch (Exception ex)
            {

            }
        }

        private void radButtonDown_Click_1(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadPBSSetting.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;

                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if (cr.Row + cr.RowCount == sheetView.RowCount)
                        return;
                    iFrom = cr.Row + cr.RowCount - 1 - i;
                    iTo = iFrom + 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = iTo;
                sheetView.AddSelection(iTo, cr.Column, cr.RowCount, cr.ColumnCount);
            }
            catch (Exception ex)
            {

            }
        }

        private void radButtonDelete_Click_1(object sender, EventArgs e)
        {
            bool bResult = false;

            try
            {
                frmInformationYesNo frmConfirmMessage = new frmInformationYesNo(string.Format(Resources.Message10003, "Delete PBS Setting data"));

                if (frmConfirmMessage.ShowDialog() == DialogResult.Yes)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        int iCountCheckAtt = 0;
                        for (int iRow = 0; iRow < fpSpreadPBSSetting.ActiveSheet.Rows.Count; iRow++)
                        {
                            string sCheck = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.Select].Text.ToString();
                            long lPBSSettingID = long.Parse(fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.PBSSettingID].Value.ToString());

                            if (sCheck.ToUpper().Equals("TRUE"))
                            {
                                iCountCheckAtt++;
                                bResult = proxy.DeleteCommon_PBSSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lPBSSettingID);
                            }
                        }

                        if (iCountCheckAtt == 0)
                        {
                            new frmInfomation(Resources.Message10008).ShowDialog(this);
                            return;
                        }

                        if (bResult == true)
                        {
                            InitControl_Setting();
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

        private void radButtonCreate_Click_1(object sender, EventArgs e)
        {
            EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
            long lProjectMasterID = cpProject.ProjectMasterID;

            int iRow = fpSpreadPBSSetting.ActiveSheet.Rows.Count++;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.Select].Value = false;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.PBSType].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.PBSSettingID].Value = -1;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.ProjectMasterID].Value = lProjectMasterID;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.ProcessEquipment].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.LineList].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.SPPID].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.SP3D].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.SPI].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.SortOrder].Value = fpSpreadPBSSetting.ActiveSheet.Rows.Count;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.DeleteFlag].Value = false;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.PBSSettingID].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.ProjectMasterID].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1, (int)menColumn_Setting.SortOrder].HorizontalAlignment = CellHorizontalAlignment.Left;

            SetSpreadComboBox_Setting(fpSpreadPBSSetting.ActiveSheet.Rows.Count - 1);
        }

        private void SetSpreadComboBox_Setting(int iRowIndex)
        {
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.PBSType].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPPID].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.ProcessEquipment].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.LineList].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SP3D].Value = string.Empty;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPI].Value = string.Empty;

            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.PBSType].CellType = mcbtPBS_TYPE_Setting;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.ProcessEquipment].CellType = mcbtProcessEQColumn_TYPE_Setting;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.LineList].CellType = mcbtLineColumn_TYPE_Setting;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPPID].CellType = mcbtSPPIDColumn_TYPE_Setting;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SP3D].CellType = mcbtSP3DColumn_TYPE_Setting;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPI].CellType = mcbtSPIColumn_TYPE_Setting;

            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.ProcessEquipment].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.LineList].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPPID].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SP3D].HorizontalAlignment = CellHorizontalAlignment.Left;
            fpSpreadPBSSetting.ActiveSheet.Cells[iRowIndex, (int)menColumn_Setting.SPI].HorizontalAlignment = CellHorizontalAlignment.Left;
        }

        public bool SavePBSSettings_Setting(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;

            DataTable dtPBSSettings = new DataTable("Table1");
            dtPBSSettings.Columns.Add("PBSSettingID", typeof(long));
            dtPBSSettings.Columns.Add("ProjectMasterID", typeof(long));
            dtPBSSettings.Columns.Add("TypeName");
            dtPBSSettings.Columns.Add("ProcessEquipment_Type");
            dtPBSSettings.Columns.Add("LineList_Type");
            dtPBSSettings.Columns.Add("SPPID_Type");
            dtPBSSettings.Columns.Add("SP3D_Type");
            dtPBSSettings.Columns.Add("SPI_Type");
            dtPBSSettings.Columns.Add("SortOrder");
            dtPBSSettings.Columns.Add("DeleteFlag", typeof(bool));

            EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
            long lProjectMasterID = cpProject.ProjectMasterID;

            int iRowCount = 1;
            for (int iRow = 0; iRow < fpSpreadPBSSetting.ActiveSheet.RowCount; iRow++)
            {
                DataRow drNew = dtPBSSettings.NewRow();
                drNew["PBSSettingID"] = long.Parse(fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.PBSSettingID].Value.ToString());
                drNew["ProjectMasterID"] = lProjectMasterID;
                drNew["TypeName"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.PBSType].Text.ToString().Trim();

                if (fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.ProcessEquipment].Value.ToString().Trim() != "0")
                    drNew["ProcessEquipment_Type"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.ProcessEquipment].Value.ToString().Trim();
                else
                    drNew["ProcessEquipment_Type"] = "";

                if (fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.LineList].Value.ToString().Trim() != "0")
                    drNew["LineList_Type"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.LineList].Value.ToString().Trim();
                else
                    drNew["LineList_Type"] = "";

                drNew["SPPID_Type"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.SPPID].Text.ToString().Trim();
                drNew["SP3D_Type"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.SP3D].Text.ToString().Trim();
                drNew["SPI_Type"] = fpSpreadPBSSetting.ActiveSheet.Cells[iRow, (int)menColumn_Setting.SPI].Text.ToString().Trim();
                drNew["SortOrder"] = iRow + 1;
                drNew["DeleteFlag"] = false;
                dtPBSSettings.Rows.Add(drNew);
                iRowCount++;

            }//fpSpreadMenu Row count

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bResult = proxy.SaveCommon_PBSSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, dtPBSSettings, ref sMessage);
            }

            //Refresh spread
            InitControl_Setting();

            return bResult;
        }



    }
}
