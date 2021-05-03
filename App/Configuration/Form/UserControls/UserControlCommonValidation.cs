using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Configuration.Properties;
using CommonLibrary;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using EDCS.WF.DSL.Entities;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using TechSun.Theme;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlCommonValidation : UserControlBase
    {
        long mSystemID = -1;
        //TODO : 메뉴 개발 완료 후 사용여부 확인
        int mListTypeID = -1;

        private const string CONST_NODATA = "UserControlNoData";
        private const string CONST_TYPE_ITEMTAG = "UserControlCommonValidation_ItemTag";
        private const string CONST_TYPE_ProcessData = "UserControlCommonValidation_ProcessData";
        private const string CONST_TYPE_FilterCondition = "UserControlCommonValidation_FilterCondition";

        private enum enColumn { ValidationType, Last };
        private string[] mColumnName = new string[] { "Validation Type" };
        private int[] mColumnWidth = new int[] { 200 };
        private bool[] mColumnLock = new bool[] { true };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text };

        private string DEFAULT_TOOLTYPE = ToolType.SPPID;
        private string mToolType = string.Empty;
        private string mProjectNo = string.Empty;
        private long mProjectMasterID = -1;

        public UserControlCommonValidation()
        {
            InitializeComponent();

        }

        public override void LoadData()
        {
            // Control 초기화 : System,  Project, ItemType
            InitDropDownControl();
            
            // ValidationType
            InitSpreadSheet();

            // Spread 초기화
            SetSpreadValidationType();

        }



        /// <summary>
        /// Project List 초기화
        /// </summary>
        private void SetDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, mToolType);
        }

        public void SetSpreadValidationType()
        {
            SheetView sheetview = fpSpreadValidationType.ActiveSheet;

            // 테스트 기간에는 SPPID만 사용
            if(mToolType == ToolType.SPPID)
            {
                //ToolType이 늘어나고 validation type이 다양해지면 DB에서 가져올것
                sheetview.Rows.Count = 3;
                sheetview.Cells[0, 0].Tag = CONST_TYPE_ITEMTAG;
                sheetview.Cells[0, 0].Text = "ItemTag";
                sheetview.Cells[1, 0].Tag = CONST_TYPE_ProcessData;
                sheetview.Cells[1, 0].Text = "Process Data";
                sheetview.Cells[2, 0].Tag = CONST_TYPE_FilterCondition;
                sheetview.Cells[2, 0].Text = "Filter Condition";

                string sUserControlName = sheetview.Cells[sheetview.ActiveRow.Index, (int)enColumn.ValidationType].Tag.ToString();
                SetUserControl(sUserControlName);
            }
            else
            {
                sheetview.Rows.Clear();
                SetUserControl(CONST_NODATA);
            }

            if(sheetview.Rows.Count > 0)
                SetControlStatus(true);
            else
                SetControlStatus(false);

            
        }

        private void InitDropDownControl()
        {
            this.radDropDownListProjectList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListSystemList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSystemList_SelectedIndexChanged);
            this.comboTreeBoxItemType.SelectedChanged -= new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);

            // System List 초기화
            GeneralUtil.SetDropdownListSystemInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListSystemList);

            // 기본 System 설정 : SPPID
            foreach(RadListDataItem item in radDropDownListSystemList.Items)
            {
                var DataBoundItem = item.DataBoundItem as EntityCommon_System;

                if (DataBoundItem.ToolType == DEFAULT_TOOLTYPE)
                {
                    radDropDownListSystemList.SelectedIndex = item.Index;
                    mSystemID = DataBoundItem.SystemID;
                    mToolType = DEFAULT_TOOLTYPE;
                    break;
                }
            }

            // Project List 초기화
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, DEFAULT_TOOLTYPE);

            // ItemType 초기화 : 기본 Drawing
            InitDropdownListItemType(string.Empty);

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListSystemList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSystemList_SelectedIndexChanged);
            this.comboTreeBoxItemType.SelectedChanged += new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);
        }

        private void InitDropdownListItemType(string sItemType)
        {
            GeneralUtil.SetComboTreeBoxItemType(ref comboTreeBoxItemType);

            if (!string.IsNullOrEmpty(sItemType))
                comboTreeBoxItemType.Text = sItemType;
            else
                comboTreeBoxItemType.Text = Information.DRAWING;
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadValidationType, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
        }

        /// <summary>
        /// UserControl setup
        /// </summary>
        /// <param name="eSelectedProject"></param>
        /// <param name="sItemType"></param>
        private void SetUserControl(string sUserControlName)
        {
            UserControlBase ucbaseControl = new UserControlBase();
            radPanelUserControl.Controls.Clear();

            if (sUserControlName != CONST_NODATA)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                long lProjectMasterID = cpProject.ProjectMasterID;
                string sProjectNo = cpProject.ProjectNo;
                string sItemType = comboTreeBoxItemType.Text;

                switch (sUserControlName)
                {
                    case CONST_TYPE_ITEMTAG:
                        ucbaseControl = new UserControlCommonValidation_ItemTag((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem, sItemType);
                        break;
                    case CONST_TYPE_ProcessData:
                        ucbaseControl = new UserControlCommonValidation_ProcessData(lProjectMasterID, mSystemID, sItemType, (EntityCommon_System)radDropDownListSystemList.SelectedItem.DataBoundItem);
                        break;
                    case CONST_TYPE_FilterCondition:
                        ucbaseControl = new UserControlCommonValidation_FilterCondition((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem, sItemType);
                        break;
                }
            }
            else
            {
                ucbaseControl = new UserControlNoData();
            }

            ucbaseControl.Dock = DockStyle.Fill;
            radPanelUserControl.Controls.Add(ucbaseControl);

            ucbaseControl.LoadData();
        }

        /// <summary>
        /// System change EVENT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListSystemList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this.radDropDownListProjectList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);

            if (radDropDownListSystemList.SelectedItem != null)
            {
                var item = radDropDownListSystemList.SelectedItem.DataBoundItem as EntityCommon_System;

                if (item != null)
                {
                    mSystemID = item.SystemID;
                    mListTypeID = item.ListTypeID;
                    mToolType = item.ToolType;

                    SetDropdownListProject();

                    SetSpreadValidationType();
                }
            }
            else
            {
                mSystemID = -1;
                mListTypeID = -1;
                mToolType = string.Empty;
            }

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
        }

        /// <summary>
        /// Project change EVENT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (mToolType == ToolType.Admin)
            {
                var cpProject = (EntityCommon_ProjectMaster)radDropDownListProjectList.SelectedItem.DataBoundItem;
                mProjectMasterID = cpProject.ProjectMasterID;
                mProjectNo = cpProject.ProjectNo;
            }
            else if (mToolType == ToolType.EDB)
            {
                var cpProject = (EntityEDB_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                mProjectMasterID = cpProject.ProjectMasterID;
                //mProjectNo = 
            }
            else if (mToolType == ToolType.SPPID || mToolType == ToolType.SP3D)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                mProjectMasterID = cpProject.ProjectMasterID;
                mProjectNo = cpProject.ProjectNo;

                SheetView sheetview = fpSpreadValidationType.ActiveSheet;
                string sUserControlName = sheetview.Cells[sheetview.ActiveRow.Index, (int)enColumn.ValidationType].Tag.ToString();
                SetUserControl(sUserControlName);
            }

        }

        /// <summary>
        /// ItemType change EVENT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            // ToolType.SPPID 외에 다른 Tool은 ItemType을 사용금지 시킴, SetSpreadValidationType() 참조
            if (mToolType == ToolType.Admin)
            {
            }
            else if (mToolType == ToolType.EDB)
            {
            }
            else if (mToolType == ToolType.SP3D)
            {
            }
            else if (mToolType == ToolType.SPPID)
            {
                SheetView sheetview = fpSpreadValidationType.ActiveSheet;
                string sUserControlName = sheetview.Cells[sheetview.ActiveRow.Index, (int)enColumn.ValidationType].Tag.ToString();
                SetUserControl(sUserControlName);
            }
        }

        /// <summary>
        /// Validation type click EVENT
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void fpSpreadValidationType_CellClick(object sender, CellClickEventArgs e)
        {
            string sUserControlName = fpSpreadValidationType.ActiveSheet.Cells[e.Row, (int)enColumn.ValidationType].Tag.ToString();
            SetUserControl(sUserControlName);
        }

        /// <summary>
        /// ToolType에 따라 사용 가능한 control 세팅
        /// </summary>
        /// <param name="bProjectListEnable"></param>
        private void SetControlStatus(bool bProjectListEnable)
        {
            radDropDownListProjectList.Enabled = bProjectListEnable;

            if (mToolType == ToolType.Admin)
            {
                comboTreeBoxItemType.Enabled = false;
            }
            else if (mToolType == ToolType.EDB)
            {
                comboTreeBoxItemType.Enabled = false;
            }
            else if (mToolType == ToolType.SP3D)
            {
                comboTreeBoxItemType.Enabled = false;
            }
            else if (mToolType == ToolType.SPPID)
            {
                comboTreeBoxItemType.Enabled = true;
                
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void SaveData()
        {
            foreach(UserControlBase ucbaseControl  in radPanelUserControl.Controls)
            {
                ucbaseControl.SaveData();
                break;
            }
        }
    }
}
