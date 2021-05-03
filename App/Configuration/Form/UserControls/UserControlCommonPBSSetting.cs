using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using EDCS.WF.DSL.Entities;
using TechSun.Theme;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI.Data;
using Configuration.Properties;
using EDCS.WF.DSL.DataProperty;

namespace Configuration
{
    public partial class UserControlCommonPBSSetting : UserControlBase
    {
        private enum menColumn_Setting { Select, PBSType, PBSSettingID, ProjectMasterID, ProcessEquipment, LineList, SPPID, SP3D, SPI, SortOrder, DeleteFlag, Last };
        private string[] mColumnName_Setting = new string[] { "Check", "TypeName", "PBSSettingID", "ProjectMasterID", "ProcessEquipment", "LineList", "SPPID", "SP3D", "SPI", "SortOrder", "DeleteFlag" };
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

        public UserControlCommonPBSSetting()
        {
            InitializeComponent();

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
            marPBSTypes_SP3D_Setting[2] =  PBSMappingLevel.Area;
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
            try
            {
                // TODO 공사중
                //new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme("공사중").RadMessageBoxShow();
                //return;

                InitControl_Setting();
                setSpread_Setting();
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
        public override void InitializeControl()
        {
            InitDropDownControl_Setting();

            this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
        }
        /// <summary>
        /// Initialize Project List
        /// </summary>
        private void InitDropDownControl_Setting()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
        }

        private void InitControl_Setting()
        {
            if (radDropDownListProject.SelectedItem != null)
            {
                try
                {
                    FrmProgressStatus.FormShow();
                    FrmProgressStatus.SetMarquee();
                    FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
                    FrmProgressStatus.ProgressPer = "Select PBS list.  Please wait!";

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

                    FrmProgressStatus.FormClose();
                }
                catch (Exception ex)
                {
                    FrmProgressStatus.FormClose();
                }
            }
        }
        private void initSpread_Setting()
        {
            SpreadUtil.InitSpread(fpSpreadPBSSetting, mColumnWidth_Setting, mColumnName_Setting, mColumnLock_Setting, null, null, null, null, null, Color.LightGray, true, true);

        }
        /// <summary>
        /// PBS setting에 사용할 Tool(system) list up
        /// </summary>
        /// <param name="cpProject"></param>
        private void GetToolUsingInfo(EntityCommon_ProjectMaster cpProject)
        {
            m_dicbProjectSettingInfo_Setting = new Dictionary<int, bool>();

            m_dicbProjectSettingInfo_Setting.Add((int)menColumn_Setting.ProcessEquipment, false);
            m_dicbProjectSettingInfo_Setting.Add((int)menColumn_Setting.LineList, false);

            if (cpProject.EDBFlag == true)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var edbProject = proxy.ReadProjectMasterByEntity(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cpProject.ProjectMasterID);

                    if (edbProject != null)
                    {
                        m_dicbProjectSettingInfo_Setting[(int)menColumn_Setting.ProcessEquipment] = edbProject.ProcessEquipmentFlag;
                        m_dicbProjectSettingInfo_Setting[(int)menColumn_Setting.LineList] = edbProject.LineFlag;
                    }
                }
            }

            m_dicbProjectSettingInfo_Setting.Add((int)menColumn_Setting.SPPID, cpProject.SPPIDFlag);
            m_dicbProjectSettingInfo_Setting.Add((int)menColumn_Setting.SP3D, cpProject.SP3DFlag);
            m_dicbProjectSettingInfo_Setting.Add((int)menColumn_Setting.SPI, cpProject.SPIFlag);
        }

        /// <summary>
        /// 컬럼별 cell type 지정 
        /// </summary>
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

        /// <summary>
        /// Tool별 ComboBoxCellType 초기화
        /// </summary>
        /// <param name="common_ProjectMaster"></param>
        private void InitSpreadCombobox_Setting()
        {
            FrmProgressStatus.ProgressPer = "Initialize PBS Type combobox.  Please wait!";
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

        /// <summary>
        /// SpreadSheet PBS setting 데이터 로드
        /// </summary>
        /// <param name="lstPBSMapping"></param>
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

        public override void SaveData()
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
        /// <summary>
        /// [Common_PBSSetting] SortOrder 순서 조정
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonUp_Click(object sender, EventArgs e)
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
        /// <summary>
        /// [Common_PBSSetting] SortOrder 순서 조정
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonDown_Click(object sender, EventArgs e)
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
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonCreate_Click(object sender, EventArgs e)
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
        /// <summary>
        /// DeleteCommon_PBSSetting에 lPBSSettingID로 식별해 DeleteFlag를 true로 변경
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonDelete_Click(object sender, EventArgs e)
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

        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            InitControl_Setting();
        }

    }
}
