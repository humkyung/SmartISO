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
using System.Xml;


namespace Configuration
{
    public partial class UserControlSPPIDTagReadOnly : UserControlBase
    {
        private Dictionary<string, string> mdicProperty = new Dictionary<string, string>();

        private enum menColumn { Select,  ProjectID, ColumnName, SortOrder, Last };
        private string[] mColumnName = new string[] { " ",  "ProjectID", "ColumnName", "SortOrder"};
        private int[] marColumnWidths = new int[] { 60, 100, 300, 100};
        private bool[] marColumnLock = new bool[] { false, true, false, true};
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };
        

        // TRUE : 최초 로드시 DB에서 가져온 data가 있을때, False였지만 saveData메뉴에서 저장 완료 했을때
        // FALSE : 최초 로드시 DB에서 가져온 data가 없을때,  saveData메뉴에서 저장 실패 했을때
        private bool m_IsStoredData = false;
        private string m_SiteItemTagFormatXMLPath = string.Empty;

        private string ItemType
        {
            get
            {
                if (comboTreeBoxItemType.Nodes.Count > 0)
                    return comboTreeBoxItemType.Text;

                return string.Empty;
            }
        }
        public UserControlSPPIDTagReadOnly()
        {
            InitializeComponent();
        }
        public override void LoadData()
        {
            InitDropdownListProject();
            InitDropdownListItemType(string.Empty);

            this.RegisterEventHandler();

            // ItemTagFormat.xml 경로 가져오기, ItemTagFormat.xml 출력
            if (radDropDownListProjectList.SelectedItem != null)
            {
                textBoxXMLPath.Text = GetItemTagFormatXMLPath((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem);

               InitTagFormatSetting((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem, ItemType);
            }

            SetSpreadSheet();
        }

        /// <summary>
        /// UserControlSPPIDItemTagFormat 가져오기, xml file 출력
        /// </summary>
        /// <param name="eSelectedProject"></param>
        /// <param name="sItemType"></param>
        private void InitTagFormatSetting(EntitySPTool_Project eSelectedProject, string sItemType)
        {
            var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

            radSplitContainerTagRule.Controls.Clear();

            SplitPanel splitPanel = new SplitPanel();

            UserControlSPPIDItemTagFormat ItemTagFormat = new UserControlSPPIDItemTagFormat(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectNo, TagNoSetting.TagNo, cpProject.ProjectID, sItemType, GetXMLpath());

            ItemTagFormat.Dock = DockStyle.Fill;
            splitPanel.Controls.Add(ItemTagFormat);
            radSplitContainerTagRule.Controls.Add(splitPanel);

            // XML Load에 따라 Generate 버튼 사용 유무 변경
            SetControlStatus();
        }
        /// <summary>
        ///  spread 초기화 및 Tag ReadOnly Data 가져오기
        /// </summary>
        private void SetSpreadSheet()
        {
            if (radDropDownListProjectList.SelectedItem != null)
            {
                var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

                
                InitSpreadSheet(cpProject, ItemType);
            }
        }

        public void RegisterEventHandler()
        {
            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);

            this.comboTreeBoxItemType.SelectedChanged += new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);
        }

        /// <summary>
        /// Project List 초기화
        /// </summary>
        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }
        /// <summary>
        /// ItemType List 초기화
        /// </summary>
        /// <param name="sItemType"></param>
        private void InitDropdownListItemType(string sItemType)
        {
            GeneralUtil.SetComboTreeBoxItemType(ref comboTreeBoxItemType);

            if (!string.IsNullOrEmpty(sItemType))
                comboTreeBoxItemType.Text = sItemType;
            else
                comboTreeBoxItemType.Text = Information.DRAWING;
        }
        
        /// <summary>
        /// spread 초기화 및 Tag ReadOnly Data 가져오기
        /// (comboBox초기화, column setting 등)
        /// </summary>
        /// <param name="cpProject"></param>
        /// <param name="sItemType"></param>
        private void InitSpreadSheet(EntitySPTool_Project cpProject, string sItemType)
        {
            if (radDropDownListProjectList.SelectedItem != null)
            {
                string sLoadingTexts = "Loading data.  Please wait.";
                var frmPopupWaitingBarCallMethod = new TechSun.Theme.frmSplashScreen(1, EDCS.WF.BSL.Variable.SharedVariable.ToolTip);
                frmPopupWaitingBarCallMethod.MethodSetStart();
                frmPopupWaitingBarCallMethod.MethodStatusUpdateText(sLoadingTexts);

                try
                {
                    //콤보박스 초기화(Attribute combobox)
                    InitAttributeCombobox(cpProject, sItemType);
                    
                    //컬럼 세팅(셀타입, Visible, 정렬 등)
                    setSpread();

                    //SetTagReadOnly 데이터 로드()
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        List<EntitySPPID_TagNoSetting> lstTagReadOnly = proxy.ReadSPPID_MarkTagNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, long.Parse(cpProject.ProjectID.ToString()), ItemType, TagNoSetting.TagNoReadOnly);

                        // 현재 Data가 DB에서 가져온 data인지 구분해서 Delete row후 save 여부를 판단
                        m_IsStoredData = lstTagReadOnly.Count > 0 ? true : false;

                        // Spread 출력
                        int iRowCount = 0;
                        foreach (EntitySPPID_TagNoSetting column in lstTagReadOnly)
                        {
                            iRowCount = fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count++;
                            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[iRowCount, (int)menColumn.ProjectID].Value = cpProject.ProjectID;
                            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[iRowCount, (int)menColumn.ColumnName].Value = column.AttributeName;
                            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[iRowCount, (int)menColumn.SortOrder].Text = column.SortOrder.ToString();
                        }
                    }

                    frmPopupWaitingBarCallMethod.MethodClose();
                }
                catch (Exception ex)
                {
                    frmPopupWaitingBarCallMethod.MethodClose();
                    new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
                }
            }
        }
        /// <summary>
        /// ComboBox 세팅을 위해 SPPID_DataColum에서 column list를 가져와 mdicProperty에 담기
        /// 일부 Project는 ColumnName, ColumnDisplay이 다르게 Mapping되므로 매번 새로 가져옴(ex:프로젝트 변경시)
        /// </summary>
        /// <param name="cpProject"></param>
        /// <param name="sItemType"></param>
        private void InitAttributeCombobox(EntitySPTool_Project cpProject, string sItemType)
        {
            mdicProperty.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EntitySPPID_DataColumn> lstDataColumn = proxy.ReadDataColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, long.Parse(cpProject.ProjectID.ToString()), sItemType);

                foreach (var item in lstDataColumn)
                {
                    if (!mdicProperty.ContainsKey(item.ColumnName))
                        mdicProperty.Add(item.ColumnName, item.ColumnDisplay);
                }

                // 일부 프로젝트는 ColumnName으로 Data를 가져 올 수 없으므로 Mapping되는 ColumnName으로 교체한다
                Property.SetAddProperty(cpProject.ProjectID.ToString(), ref mdicProperty);
            }

        }
        /// <summary>
        /// 컬럼 세팅(셀타입, Visible, 정렬 등)
        /// </summary>
        private void setSpread()
        {
            //컬럼 헤드 초기화
            initSpread();

            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadTagReadOnlySetting.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                arItems = null;
                arItemsData = null;

                switch (i)
                {
                    case (int)menColumn.Select:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadTagReadOnlySetting.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadTagReadOnlySetting.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.ProjectID:
                        fpSpreadTagReadOnlySetting.ActiveSheet.Columns[i].Visible = false;
                        break;
                    case (int)menColumn.ColumnName:
                        // InitAttributeCombobox에서 가져온 column list를 Item에 넣는다
                        arItems = new string[mdicProperty.Count];
                        arItemsData = new string[mdicProperty.Count];
                        int iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                    case (int)menColumn.SortOrder:
                        fpSpreadTagReadOnlySetting.ActiveSheet.Columns[i].Visible = false;
                        break;
                }

                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadTagReadOnlySetting.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }
        }
        private void initSpread()
        {
            SpreadUtil.InitSpread(fpSpreadTagReadOnlySetting, marColumnWidths, mColumnName, marColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
        }
        //프로젝트 변경에 따른 refresh Spreads(TagSetting, ItemTagFormat 둘다) ,  사용자 프로젝트 히스토리 저장, XML로드
        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSpreadSheet();

            // ItemTagFormat.xml 경로 가져오기, ItemTagFormat.xml 출력
            if (radDropDownListProjectList.SelectedItem != null)
            {
                textBoxXMLPath.Text = GetItemTagFormatXMLPath((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem);

                InitTagFormatSetting((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem, ItemType);
            }
        }
        //ItemType 변경에 따른 Spread refresh
        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            SetSpreadSheet();

            // ItemTagFormat.xml 경로 가져오기, ItemTagFormat.xml 출력
            if (radDropDownListProjectList.SelectedItem != null)
            {
                InitTagFormatSetting((EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem, ItemType);
            }
        }
        /// <summary>
        /// 신규 Row 추가
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            EntitySPTool_Project cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

            int iRow = fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count++;

            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count - 1, (int)menColumn.Select].Value = false;
            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count - 1, (int)menColumn.ProjectID].Value = cpProject.ProjectID;
            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count - 1, (int)menColumn.ColumnName].Value = string.Empty;
            fpSpreadTagReadOnlySetting.ActiveSheet.Cells[fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count - 1, (int)menColumn.SortOrder].Value =fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count;

        }
        /// <summary>
        /// Row 순서 변경(sortorder번호 변경)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagReadOnlySetting.ActiveSheet;

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
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }
        /// <summary>
        /// Row 순서 변경(sortorder번호 변경)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagReadOnlySetting.ActiveSheet;
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
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }
        /// <summary>
        /// Spread sheet에서 체크된 row를 삭제하고 SaveData()로 변동사항을 저장
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            bool bDeleted = false;

            for (int i = fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadTagReadOnlySetting.ActiveSheet.Cells[i, (int)menColumn.Select].Text;
                if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadTagReadOnlySetting.ActiveSheet.Rows.Remove(i, 1);
                    bDeleted = true;
                }
            }

            //// Row 삭제 후  SaveData() 조건 : 1. 1개이상 Row삭제가 정상 완료됨, 2. 현재 Data가 DB에서 가져온 경우
            if (bDeleted == true)
            {
                if (m_IsStoredData == true)
                {
                    SaveData();
                }
            }

            //ColumnHeader check 해제
            if ((fpSpreadTagReadOnlySetting.ActiveSheet.ColumnHeader.Cells[0, (int)menColumn.Select].Value).ToString().ToUpper().Equals("TRUE"))
                (fpSpreadTagReadOnlySetting.ActiveSheet.ColumnHeader.Cells[0, (int)menColumn.Select].Value) = false;
        }
        /// <summary>
        /// ItemTag ReadOnly Data Setting 저장
        /// </summary>
        public override void SaveData()
        {
            string sMessage = string.Empty;
            bool bResult = false;

            DataTable dtTagReadOnlySettings = new DataTable("Table1");
            dtTagReadOnlySettings.Columns.Add("TagID", typeof(long));
            dtTagReadOnlySettings.Columns.Add("ItemType");
            dtTagReadOnlySettings.Columns.Add("Type");
            dtTagReadOnlySettings.Columns.Add("ProjectID", typeof(long));
            dtTagReadOnlySettings.Columns.Add("AttributeName");
            dtTagReadOnlySettings.Columns.Add("Separator");
            dtTagReadOnlySettings.Columns.Add("Length", typeof(int));
            dtTagReadOnlySettings.Columns.Add("FillString");
            dtTagReadOnlySettings.Columns.Add("Alignment");
            dtTagReadOnlySettings.Columns.Add("AllowNullFlag", typeof(bool));
            dtTagReadOnlySettings.Columns.Add("AllowDuplicationFlag", typeof(bool));
            dtTagReadOnlySettings.Columns.Add("SortOrder", typeof(int));

            EntitySPTool_Project cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
            long lProjectID = cpProject.ProjectID;

            bool bSaveFlag = true;
            int iSortOrder = 0;
            for (int iRow = 0; iRow < fpSpreadTagReadOnlySetting.ActiveSheet.RowCount; iRow++)
            {
                DataRow drNew = dtTagReadOnlySettings.NewRow();

                string sProperty = fpSpreadTagReadOnlySetting.ActiveSheet.Cells[iRow, (int)menColumn.ColumnName].Text;

                if (string.IsNullOrEmpty(sProperty))
                {
                    bSaveFlag = false;
                    break;
                }

                drNew["ItemType"] = ItemType;
                drNew["Type"] = TagNoSetting.TagNoReadOnly;
                drNew["ProjectID"] = lProjectID;
                drNew["AttributeName"] = fpSpreadTagReadOnlySetting.ActiveSheet.Cells[iRow, (int)menColumn.ColumnName].Value.ToString();
                drNew["AllowNullFlag"] = false;
                drNew["AllowDuplicationFlag"] = false;
                drNew["SortOrder"] = iSortOrder;
                
                dtTagReadOnlySettings.Rows.Add(drNew);
                iSortOrder++;
            }

            if (bSaveFlag)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.SaveSPPID_TagNoSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectID, ItemType, TagNoSetting.TagNoReadOnly, dtTagReadOnlySettings, ref sMessage);
                }
            }
            else
            {
                new frmInfomation(string.Format(Resources.Message10011, "Column Name")).ShowDialog(this);
            }

            if (bResult == true)
            {
                if(fpSpreadTagReadOnlySetting.ActiveSheet.RowCount > 0)
                    m_IsStoredData = true;
                else
                    m_IsStoredData = false;
                
                new frmInfomation(Resources.Message10000).ShowDialog(this);
            }
            else
            {
                m_IsStoredData = false;
                new frmInfomation(sMessage).ShowDialog(this);
            }
            
        }
        /// <summary>
        /// 선택된 Project 정보를 이용해 서버의 ItemTagFormat.xml 경로를 가져온다
        /// </summary>
        string GetItemTagFormatXMLPath(EntitySPTool_Project eProject)
        {
            string sPath = "File Path : ";

            // (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
            Information information = new Information();
            information.Project = eProject;
            information.Plant = null;

            if (information.Project != null)
            {
                List<InfoPlant> lstPlant = new List<InfoPlant>();
                var proxy2 = WcfHelper.CreateProxyNetTcp<IEdcsDac>(true);
                using (proxy2 as IDisposable)
                {
                    lstPlant = proxy2.GetPlantNames(true, information, information.Project, GeneralUtil.InitConnection(information));
                }

                if (lstPlant.Count != 0)
                {
                    sPath = "File Path : " + lstPlant[0].SiteItemTagFormatXMLPath.ToString();
                }
            }

            return sPath;
        }

        private void fpSpreadTagReadOnlySetting_CellClick(object sender, CellClickEventArgs e)
        {
            // for'Check-All' cell Clicked
            if (e.Column == (int)menColumn.Select)
            {
                CheckBoxCellType checkBoxCellType1 = new CheckBoxCellType();
                HitTestInformation hti;
                HeaderHitTestInformation hi;
                hti = fpSpreadTagReadOnlySetting.HitTest(e.X, e.Y);
                hi = hti.HeaderInfo;
                if (hti.Type == HitTestType.ColumnHeader)
                {
                    if (hi.Column == 0)
                    {
                        bool check = !Convert.ToBoolean(fpSpreadTagReadOnlySetting.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value);

                        fpSpreadTagReadOnlySetting.ActiveSheet.ColumnHeader.Cells[0, hi.Column].Value = check;

                        SheetView sheetView = fpSpreadTagReadOnlySetting.ActiveSheet;

                        if (sheetView != null)
                        {
                            for (int i = 0; i < sheetView.Rows.Count; i++)
                            {
                                sheetView.Cells[i, 0].Value = check;
                            }
                        }
                    }
                }
            }
        }
        /// <summary>
        /// textBoxXMLPath에서 Split으로 주소만 가져옴
        /// </summary>
        /// <returns></returns>
        private string GetXMLpath()
        {
            string[] arXMLPath = textBoxXMLPath.Text.Split(":".ToCharArray());

            if (arXMLPath[1].Length > 1)
            {
                return arXMLPath[1].Trim();
            }

            return string.Empty;
        }

        private void SetControlStatus()
        {
            if(!string.IsNullOrEmpty(textBoxXMLPath.Text))
                kRadButtonGenerate.Enabled = true;
            else
                kRadButtonGenerate.Enabled = false;
        }

        private SheetView GetItemTagFormatSheetview()
        {
            if (radSplitContainerTagRule.Controls.Count > 0)
            {
                foreach (SplitPanel splitpanel in radSplitContainerTagRule.Controls)
                {
                    if (splitpanel.Controls[0].Controls[0].Controls["fpSpreadTagFormat"] != null)
                    {
                        Control control = splitpanel.Controls[0].Controls[0].Controls["fpSpreadTagFormat"];
                        return (control as FarPoint.Win.Spread.FpSpread).ActiveSheet;
                    }
                    else
                    {
                        return null;
                    }
                }
            }

            return null;
        }
        /// <summary>
        /// 프로젝트별 XML file에서 Attribute 리스트를 가져와 자동으로 SPPID_TagNoSetting에 저장하는 기능
        /// ItemType별로 저장하되 Equipment, PipingComp는 하위 ItemType들도 동일하게 저장
        /// MXL의 Attribute name과 맞지 않는 것은 Mismatch attribute 메세지를 작성해 출력(사용자 추가 작업 요함)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void kRadButtonGenerate_Click(object sender, EventArgs e)
        {
            string sMismatchMessage = "Mismatch Attribute(s) : " + "\r\n";
            string sMessage = string.Empty;
            bool IsMismatchAttribute = false;
            bool bResult = true;

            // ItemType group(ex: equipment)에서 복사할 하위 ItemType list(ex: vessel)
            List<string> lstEquipment_Itemtype = new List<string>();
            List<string> lstPipingComp_ItemType = new List<string>();

            lstEquipment_Itemtype = Information.EQUIP_ITEMTYPE;
            lstPipingComp_ItemType = Information.PIPINGCOMP_ITEMTYPE;

            //Get idxProperty
            SheetView svItemTagSheetview = GetItemTagFormatSheetview();

            int idxPropertyColumn = -1;
            foreach (FarPoint.Win.Spread.Column column in svItemTagSheetview.Columns)
            {
                if (column.Label.ToString() == "Property")
                {
                    idxPropertyColumn = column.Index;
                    break;
                }
            }

            // set AttributeName list from XML file
            Dictionary<string, List<string>> dicAttributesByType = GetItemTagFormatAttributeList();

            var cpProject = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;
                     
            // SaveData() - ItemType별 반복
            foreach (KeyValuePair<string, List<string>> kv in dicAttributesByType)
            {
                InitAttributeCombobox(cpProject, kv.Key);

                // dicAttributesByType 별 반복
                List<string> lstAttributes = dicAttributesByType[kv.Key];
                List<string> lstAttributes_New = new List<string>();
                sMessage += "[" + kv.Key + "]";

                foreach (string sAttributes in lstAttributes)
                {
                    if (mdicProperty.ContainsKey(sAttributes))
                    {
                        lstAttributes_New.Add(sAttributes);
                    }
                    else
                    {
                        // "UnitCode"는 "PlantGroup.Name"로 사용하는 프로젝트가 대부분이라 하드코딩됨 - Common > Validation > ItemTag 메뉴 참고
                        if(string.Equals(sAttributes, "UnitCode"))
                        {
                            lstAttributes_New.Add("PlantGroup.Name");
                        }
                        else
                        {
                            // 에러메세지에 추가
                            // sErrorMessage
                            // 원본에서 제외
                            sMismatchMessage = sMismatchMessage + "[" + kv.Key + "] " + sAttributes + "\r\n";
                            IsMismatchAttribute = true;
                        }
                    }
                }

                // SaveData
                if (SaveData_Generate(cpProject.ProjectID, kv.Key, lstAttributes_New, ref sMessage) == false)
                    bResult = false;

                //Equipment, Piping Component는 하위에도 동일한 list를 추가 저장
                if(kv.Key == Information.EQUIPMENT)
                {
                    foreach(string sItemType in lstEquipment_Itemtype)
                    {
                        if(!dicAttributesByType.ContainsKey(sItemType))
                        { 
                            if (SaveData_Generate(cpProject.ProjectID, sItemType, lstAttributes_New, ref sMessage) == false)
                                bResult = false;
                        }
                    }
                }
                else if (kv.Key == Information.PIPINGCOMP)
                {
                    foreach (string sItemType in lstPipingComp_ItemType)
                    {
                        if (!dicAttributesByType.ContainsKey(sItemType))
                        {
                            if (SaveData_Generate(cpProject.ProjectID, sItemType, lstAttributes_New, ref sMessage) == false)
                                bResult = false;
                        }
                    }
                }

                sMessage += "\r\n";
            }

            if (bResult == true)
                new frmInfomation(Resources.Message10000).ShowDialog(this);
            else
                new frmInfomation(sMessage).ShowDialog(this);

            m_IsStoredData = false;

            if(IsMismatchAttribute)
            {
                new frmInfomation(sMismatchMessage).ShowDialog(this);
            }
        }

        /// <summary>
        /// XML 분석해 Dictionary에 담기
        /// </summary>
        /// <returns></returns>
        public Dictionary<string, List<string>> GetItemTagFormatAttributeList()
        {
            Dictionary<string, List<string>> dicAttributesByType = new Dictionary<string, List<string>>();
            string sSiteItemTagFormatXMLPath = GetXMLpath();

            try
            {
                // ItemTagFormat.xml 에서 데이터를 조회 한다. 
                if (!string.IsNullOrEmpty(sSiteItemTagFormatXMLPath))
                {
                    XmlDocument xmlDoc = new XmlDocument();
                    xmlDoc.Load(sSiteItemTagFormatXMLPath);

                    foreach (XmlNode node in xmlDoc.ChildNodes[0])
                    {
                        string sItemType = node.Attributes["ITEMTYPE"].Value;

                        if ((Information.ITEMTYPE).Contains(sItemType))
                        {
                            XmlNode childNodes = null;

                            if (node.ChildNodes.Count > 0)
                            {
                                childNodes = node.ChildNodes[0];
                            }

                            string sItemTagFormat = string.Empty;
                            List<string> lstProperty = new List<string>();

                            foreach (XmlAttribute attribute in childNodes.Attributes)
                            {
                                if (string.Equals(attribute.Name,"ItemTagFormat"))
                                {
                                        sItemTagFormat = attribute.Value;
                                        // (TagPrefix)-(Unit...UnitCode)(TagSequenceNo)(TagSuffix) 에서 Attribute Name만 추가
                                        SetFillAttributeList(sItemTagFormat, lstProperty);
                                }
                            }

                            if(!dicAttributesByType.ContainsKey(sItemType))
                            {
                                dicAttributesByType.Add(sItemType, lstProperty);
                            }
                            else
                            {
                                Console.Write("Duplicate ItemType!!");
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                //RadFormProgressForm.FormClose();
            }

            return dicAttributesByType;
        }

        /// <summary>
        /// lstProperty 에 ItemTag를 구성하는 Property 각각 담기
        /// </summary>
        /// <param name="sAttributeValue"></param>
        /// <param name="lst"></param>
        private void SetFillAttributeList(string sAttributeValue, List<string> lst)
        {
            foreach (string sTemp in sAttributeValue.Split('('))
            {
                if (sTemp.Contains(")"))
                {
                    string sAttributeName = sTemp.Substring(0, sTemp.IndexOf(")")).Trim();
                    lst.Add(sAttributeName);
                }
            }
        }

        /// <summary>
        /// Generate 기능으로 생성된 lstAttributes로 Attribute ReadOnly Setting 저장
        /// </summary>
        public bool SaveData_Generate(long lProjectID, string sItemType, List<string> lstAttributes, ref string sMessage)
        {
            bool bResult = false;

            DataTable dtTagReadOnlySettings = new DataTable("Table1");
            dtTagReadOnlySettings.Columns.Add("TagID", typeof(long));
            dtTagReadOnlySettings.Columns.Add("ItemType");
            dtTagReadOnlySettings.Columns.Add("Type");
            dtTagReadOnlySettings.Columns.Add("ProjectID", typeof(long));
            dtTagReadOnlySettings.Columns.Add("AttributeName");
            dtTagReadOnlySettings.Columns.Add("Separator");
            dtTagReadOnlySettings.Columns.Add("Length", typeof(int));
            dtTagReadOnlySettings.Columns.Add("FillString");
            dtTagReadOnlySettings.Columns.Add("Alignment");
            dtTagReadOnlySettings.Columns.Add("AllowNullFlag", typeof(bool));
            dtTagReadOnlySettings.Columns.Add("AllowDuplicationFlag", typeof(bool));
            dtTagReadOnlySettings.Columns.Add("SortOrder", typeof(int));

            int iSortOrder = 0;
            foreach (string sAttribute in lstAttributes)
            {
                DataRow drNew = dtTagReadOnlySettings.NewRow();

                drNew["ItemType"] = sItemType;
                drNew["Type"] = TagNoSetting.TagNoReadOnly;
                drNew["ProjectID"] = lProjectID;
                drNew["AttributeName"] = sAttribute;
                drNew["AllowNullFlag"] = false;
                drNew["AllowDuplicationFlag"] = false;
                drNew["SortOrder"] = iSortOrder;

                dtTagReadOnlySettings.Rows.Add(drNew);
                iSortOrder++;
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bResult = proxy.SaveSPPID_TagNoSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectID, sItemType, TagNoSetting.TagNoReadOnly, dtTagReadOnlySettings, ref sMessage);
            }

            m_IsStoredData = false;

            return bResult;
        }
    }
}
