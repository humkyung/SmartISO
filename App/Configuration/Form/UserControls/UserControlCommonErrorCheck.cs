using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;

using Telerik.WinControls.UI;
using Configuration.Properties;
using EDCS.WF.DSL.Entities;
using CommonLibrary;

namespace Configuration
{
    public partial class UserControlCommonErrorCheck : UserControlBase
    {
        long mProjectMasterID = -1;
        long mSystemID = -1;
        int mListTypeID = -1;
        string mItemType = string.Empty;

        private Dictionary<string, string> mdicProperty = new Dictionary<string, string>();

        public Information mInformation;

        private enum enColumn { Check, ErrorCheckID, ProjectMasterID, ItemType, ListTypeID, SystemID, AttributeName, Last };
        private string[] mColumnName = new string[] { "Check", "ErrorCheckID", "ProjectMasterID", "ItemType", "ListTypeID", "SystemID", "Attribute Name" };
        private int[] mColumnWidth = new int[] { 30, 0, 0, 0, 0, 0, 230 };
        private bool[] mColumnLock = new bool[] { false, false, false, false, false, false, false };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        private enum enColumnDetail { ErrorCheckID, ErrorCheckDetailID, ErrorCheckType, Value };
        private string[] mColumnNameValue = new string[] { "ErrorCheckID", "ErrorCheckDetailID", "ErrorCheckType", "Attribute Value" };
        private string[] mColumnNamePattern = new string[] { "ErrorCheckID", "ErrorCheckDetailID", "ErrorCheckType", "Pattern" };
        private int[] mColumnDetailWidth = new int[] { 0, 0, 0, 230 };
        SpreadUtil.eFarpointCellType[] mColumnDetailType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        public UserControlCommonErrorCheck()
        {
            InitializeComponent();
            //InitDropDownControl();
            //InitSpreadSheet_Attribute();
            //InitSpreadSheet_Detail();
        }

        public override void LoadData()
        {
            InitDropDownControl();
            InitSpreadSheet_Attribute();
            InitSpreadSheet_Detail();
        }

        public void InitSpreadSheet_Attribute()
        {
            SpreadUtil.InitSpread(fpSpreadData, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);
        }

        public void InitSpreadSheet_Detail()
        {
            SpreadUtil.InitSpread(fpSpreadValue, mColumnDetailWidth, mColumnNameValue, null, null, null, mColumnDetailType, null, null, Color.LightGray, true, true);
            SpreadUtil.InitSpread(fpSpreadPattern, mColumnDetailWidth, mColumnNamePattern, null, null, null, mColumnDetailType, null, null, Color.LightGray, true, true);
        }

        private void InitDropDownControl()
        {
            this.radDropDownListProjectList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListSystemList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListSystemList_SelectedIndexChanged);
            this.comboTreeBoxItemType.SelectedChanged -= new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);

            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.Admin);
            GeneralUtil.SetDropdownListSystemInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListSystemList);
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

            mItemType = comboTreeBoxItemType.Text;

        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProjectList.SelectedItem != null)
            {
                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProjectList.SelectedItem.DataBoundItem;

                mProjectMasterID = cpProject.ProjectMasterID;
                SetSpread();

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.Admin);
            }
            else
            {
                mProjectMasterID = -1;
            }
        }

        /// <summary>
        /// Error Check spread data setting
        /// </summary>
        private void SetSpread()
        {
            long lFirstErrorCheckID = -1;

            InitSpreadSheet_Attribute();
            initPropertyArray();
            lFirstErrorCheckID = SetSpread_Attribute();
            SetSpread_detail(lFirstErrorCheckID);
        }

        /// <summary>
        /// Value and Pattern Spread setting
        /// </summary>
        private void SetSpread_detail(long lCheckErrorID)
        {
            InitSpreadSheet_Detail();
            SetSpread_ValueAndPattern(lCheckErrorID);
        }

        private void radDropDownListSystemList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListSystemList.SelectedItem != null)
            {
                var item = radDropDownListSystemList.SelectedItem.DataBoundItem as EntityCommon_System;

                if (item != null)
                {
                    mListTypeID = item.ListTypeID;
                    mSystemID = item.SystemID;

                    string sToolType = item.ToolType;

                    if (sToolType == ToolType.EDB)
                    {
                        comboTreeBoxItemType.Enabled = false;
                        InitDropdownListItemType(string.Empty);
                    }
                    else
                    {
                        comboTreeBoxItemType.Enabled = true;
                        InitDropdownListItemType(mItemType);
                    }

                    SetSpread();
                }
            }
            else
            {
                mSystemID = -1;
                mListTypeID = -1;
            }
        }

        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            mItemType = comboTreeBoxItemType.Text;
            SetSpread();
        }

        private long SetSpread_Attribute()
        {
            int iRowCount = 0;
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();
                cbtColumn.Editable = true;
                arItems = null;
                arItemsData = null;
                int iCellCount = 0;

                switch (i)
                {
                    case (int)enColumn.Check:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();

                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)enColumn.AttributeName:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty.Count];
                        arItemsData = new string[mdicProperty.Count];
                        iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
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

            if (radDropDownListProjectList.SelectedItem != null)
            {
                if (mSystemID != -1)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        List<EntityCommon_ErrorCheck> lstCommon_ErrorCheck = proxy.ReadErrorCheck(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mSystemID, mItemType);

                        if (lstCommon_ErrorCheck.Count > 0)
                        {
                            FrmProgressStatus.FormShow();
                            FrmProgressStatus.SetMarquee();
                            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
                            FrmProgressStatus.ProgressPer = "Load validation Data.  Please wait!";

                            long lFirstErrorCheckID = -1;
                            int iExtendColumnWidth = 0;
                            iRowCount = 0;
                            foreach (EntityCommon_ErrorCheck cmErrorCheck in lstCommon_ErrorCheck)
                            {
                                iRowCount++;
                                fpSpreadData.ActiveSheet.Rows.Count = iRowCount;

                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Check].Value = false;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.ErrorCheckID].Value = cmErrorCheck.ErrorCheckID;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.ProjectMasterID].Value = cmErrorCheck.ProjectMasterID;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.ItemType].Value = cmErrorCheck.ItemType;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.ListTypeID].Value = cmErrorCheck.ListTypeID;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.SystemID].Value = cmErrorCheck.SystemID;
                                fpSpreadData.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.AttributeName].Value = cmErrorCheck.AttributeName;

                                // Extend Column width
                                if ((cmErrorCheck.AttributeName.Length) * 7 > mColumnWidth[(int)enColumn.AttributeName])
                                {
                                    if (iExtendColumnWidth < (cmErrorCheck.AttributeName.Length) * 8)
                                        iExtendColumnWidth = (cmErrorCheck.AttributeName.Length) * 8;
                                }

                                //Get First Error check ID for show detail data.
                                if ((lFirstErrorCheckID == -1) && (cmErrorCheck.ErrorCheckID != null))
                                    lFirstErrorCheckID = cmErrorCheck.ErrorCheckID;
                            }

                            if (iExtendColumnWidth > 0)
                                fpSpreadData.ActiveSheet.Columns[(int)enColumn.AttributeName].Width = iExtendColumnWidth;

                            FrmProgressStatus.FormClose();

                            return lFirstErrorCheckID;
                        }
                    }


                }
            }
            return -1;
        }

        private void SetSpread_ValueAndPattern(long lCheckErrorID)
        {
            if (lCheckErrorID > -1)
            {
                fpSpreadValue.ActiveSheet.Rows.Count = 50;
                fpSpreadPattern.ActiveSheet.Rows.Count = 50;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    List<EntityCommon_ErrorCheck_Detail> lstCommon_ErrorCheckDetail = proxy.ReadErrorCheckDetail(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lCheckErrorID);
                    int iRowCount_Value = 0;
                    int iRowCount_Pattern = 0;

                    foreach (var item in lstCommon_ErrorCheckDetail)
                    {
                        if (item.ErrorCheckType == ErrorCheckType.VALUE)
                        {
                            SetDetailItem(fpSpreadValue.ActiveSheet, iRowCount_Value, item.ErrorCheckType, item.ErrorCheckDetailID, item.Value);
                            iRowCount_Value++;
                        }
                        else if (item.ErrorCheckType == ErrorCheckType.PATTERN)
                        {
                            SetDetailItem(fpSpreadPattern.ActiveSheet, iRowCount_Pattern, item.ErrorCheckType, item.ErrorCheckDetailID, item.Value);
                            iRowCount_Pattern++;
                        }
                    }
                }


            }
        }

        private void SetDetailItem(SheetView sh, int iRow, string sErrorCheckType, long lErrorCheckID, string sValue)
        {
            sh.Cells[iRow, (int)enColumnDetail.ErrorCheckID].Value = lErrorCheckID;
            sh.Cells[iRow, (int)enColumnDetail.ErrorCheckType].Value = sErrorCheckType;
            sh.Cells[iRow, (int)enColumnDetail.Value].Value = sValue;
        }

        private void fpSpreadData_CellClick(object sender, CellClickEventArgs e)
        {
            long iErrorCheckID = long.Parse(fpSpreadData.ActiveSheet.Cells[e.Row, (int)enColumn.ErrorCheckID].Value.ToString());
            SetSpread_detail(iErrorCheckID);

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
                                sheetView.Cells[i, 0].Value = check;
                            }
                        }

                    }
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)enColumn.Check].Value = false;
            fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.Rows.Count - 1, (int)enColumn.ErrorCheckID].Value = -1;
        }

        private void fpSpreadData_Change(object sender, ChangeEventArgs e)
        {
            try
            {
                if (e.Column == (int)enColumn.AttributeName)
                {
                    if (fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)enColumn.AttributeName].Value != null)
                    {
                        string sNewAttName = fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)enColumn.AttributeName].Value.ToString();
                        long lErrorCheckID = long.Parse(fpSpreadData.ActiveSheet.Cells[e.Row, (int)enColumn.ErrorCheckID].Value.ToString());
                        string sError = string.Empty;
                        var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy as IDisposable)
                        {
                            List<EntityCommon_ErrorCheck> cmCommon_ErrorCheck = proxy.ReadErrorCheck(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mSystemID, mItemType);

                            // Check Exists Attribute Name
                            if (cmCommon_ErrorCheck.Where(w => w.AttributeName == sNewAttName).Count() > 0)
                            {
                                fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)enColumn.AttributeName].Value = string.Empty;
                                MessageBox.Show(string.Format(Resources.Message10012, "Attribute"), "Create validation Group", MessageBoxButtons.OK, MessageBoxIcon.Information);
                                return;
                            }

                            if (!string.IsNullOrEmpty(sNewAttName))
                            {
                                lErrorCheckID = proxy.AddErrorCheckGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mItemType, mListTypeID, mSystemID, lErrorCheckID, sNewAttName, ref sError);
                                fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)enColumn.ErrorCheckID].Value = lErrorCheckID;
                                SetSpread_detail(lErrorCheckID);
                            }
                        }
                    }
                    // Attribute name Null, refresh
                    else
                    {
                        InitSpreadSheet_Attribute();
                        SetSpread_Attribute();
                        fpSpreadData.ActiveSheet.SetActiveCell(e.Row, e.Column);
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
        /// <summary>
        /// Pattern 입력 후 유효성 검사
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void fpSpreadPattern_Change(object sender, ChangeEventArgs e)
        {
            if (IsInvalidPatternText(fpSpreadPattern.ActiveSheet.Cells[e.Row, e.Column].Text) == false)
                fpSpreadPattern.ActiveSheet.Cells[e.Row, e.Column].Text = string.Empty;
        }

        /// <summary>
        /// 받은 문자열에서 숫자/문자 패턴에 맞는지 확인 하는 함수(#,?)
        /// </summary>
        /// <param name="sPatternText"></param>
        /// <returns></returns>
        private bool IsInvalidPatternText(string sPatternText)
        {
            foreach (char c in sPatternText)
            {
                if (c != '#' && c != '?')
                {
                    return false;
                }
            }

            return true;
        }

        public override void SaveData()
        {
            try
            {
                long iErrorCheckID = long.Parse(fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)enColumn.ErrorCheckID].Value.ToString());

                if (iErrorCheckID > -1)
                {
                    string sError = string.Empty;

                    DataTable dtCommon_ErrorCheckDetailValue = new DataTable("dbo.Common_ErrorCheck_Detail");
                    dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckDetailID", typeof(long));
                    dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckID", typeof(long));
                    dtCommon_ErrorCheckDetailValue.Columns.Add("ErrorCheckType", typeof(string));
                    dtCommon_ErrorCheckDetailValue.Columns.Add("Value", typeof(string));

                    for (int i = 0; i < fpSpreadValue.ActiveSheet.Rows.Count; i++)
                    {
                        string sValue = fpSpreadValue.ActiveSheet.Cells[i, (int)enColumnDetail.Value].Text;

                        if (!string.IsNullOrEmpty(sValue))
                        {
                            DataRow drErrorCheckDetailValue = dtCommon_ErrorCheckDetailValue.NewRow();
                            drErrorCheckDetailValue["ErrorCheckID"] = iErrorCheckID;
                            drErrorCheckDetailValue["ErrorCheckType"] = ErrorCheckType.VALUE;
                            drErrorCheckDetailValue["Value"] = sValue;
                            dtCommon_ErrorCheckDetailValue.Rows.Add(drErrorCheckDetailValue);

                        }
                    }

                    for (int i = 0; i < fpSpreadPattern.ActiveSheet.Rows.Count; i++)
                    {
                        string sValue = fpSpreadPattern.ActiveSheet.Cells[i, (int)enColumnDetail.Value].Text;

                        if (!string.IsNullOrEmpty(sValue))
                        {
                            DataRow drErrorCheckDetailValue = dtCommon_ErrorCheckDetailValue.NewRow();
                            drErrorCheckDetailValue["ErrorCheckID"] = iErrorCheckID;
                            drErrorCheckDetailValue["ErrorCheckType"] = ErrorCheckType.PATTERN;
                            drErrorCheckDetailValue["Value"] = sValue;
                            dtCommon_ErrorCheckDetailValue.Rows.Add(drErrorCheckDetailValue);
                        }
                    }

                    if (dtCommon_ErrorCheckDetailValue.Rows.Count > 0)
                    {

                        var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy as IDisposable)
                        {
                            proxy.SaveCommon_ErrorCheckDetail(EDCS.WF.BSL.Variable.SharedVariable.IsIE, iErrorCheckID, dtCommon_ErrorCheckDetailValue, ref sError);
                        }

                        if (!string.IsNullOrEmpty(sError))
                        {
                            MessageBox.Show(sError, "Save validation setting");
                            return;
                        }
                        MessageBox.Show(Resources.Message10000, "Save validation setting", MessageBoxButtons.OK);
                    }
                    else
                    {
                        MessageBox.Show(string.Format(Resources.Message10011, "Attribute"), "Save validation setting", MessageBoxButtons.OK);
                    }
                }
                else
                {
                    MessageBox.Show(Resources.Message10005, "Save validation setting", MessageBoxButtons.OK);
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

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                int iCountCheckAtt = 0;

                if (MessageBox.Show(Resources.Message10003, "Validation Delete", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        //long lSystemID = long.Parse(radDropDownListSystemList.SelectedItem.Value.ToString());
                        for (int i = fpSpreadData.ActiveSheet.Rows.Count - 1; i >= 0; i--)
                        {
                            string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.Check].Value.ToString();
                            long lErrorCheckID = long.Parse(fpSpreadData.ActiveSheet.Cells[i, (int)enColumn.ErrorCheckID].Value.ToString());

                            if (sCheck.ToUpper().Equals("TRUE"))
                            {
                                iCountCheckAtt++;
                                proxy.DeleteCommon_ErrorCheckSettings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lErrorCheckID);
                            }
                        }

                        if (iCountCheckAtt == 0)
                        {
                            MessageBox.Show(Resources.Message10008, "Attribute Delete", MessageBoxButtons.OK);
                        }
                        else
                        {
                            SetSpread();
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

        private void initPropertyArray()
        {
            mdicProperty.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                string sError = string.Empty;

                mdicProperty.Add("", " ");

                if (radDropDownListSystemList.SelectedItem != null)
                {
                    var itemCommon_System = radDropDownListSystemList.SelectedItem.DataBoundItem as EntityCommon_System;

                    if (itemCommon_System != null)
                    {
                        // Add SPPID Property
                        if (itemCommon_System.ToolType == ToolType.SPPID)
                        {
                            var spProject = proxy.ReadSPTool_ProjectList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, ToolType.SPPID);
                            if (spProject != null)
                            {
                                var lstDataColumn = proxy.ReadDataColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, spProject.ProjectID, mItemType);

                                foreach (var item in lstDataColumn)
                                {
                                    if (!mdicProperty.ContainsKey(item.ColumnName))
                                        mdicProperty.Add(item.ColumnName, item.ColumnDisplay);
                                }

                                Property.SetAddProperty(spProject.ProjectNo, ref mdicProperty);

                                lstDataColumn.Clear();
                            }
                        }
                        // Add SP3D Property
                        else if (itemCommon_System.ToolType == ToolType.SP3D)
                        {
                            Information mInformation = new Information();

                            mdicProperty = proxy.GetSP3DAttribute(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mListTypeID);
                        }
                        // Add EDB Property
                        else
                        {
                            if (mListTypeID == (int)EnumNameCheck.InstrumentIndex || mListTypeID == (int)EnumNameCheck.InstrumentList)
                            {
                                // Insturment Data
                                var columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (int)EnumNameCheck.InstrumentIndex)
                                 .Select(s => new EntityEDB_ColumnDynamicDefault()
                                 {
                                     Label = s.Name,
                                     LabelView = s.NameView,
                                 }).ToList();

                                foreach (var item in columnDynamics)
                                {
                                    if (!mdicProperty.ContainsKey(item.Label))
                                        mdicProperty.Add(item.Label, item.LabelView);
                                }

                                columnDynamics.Clear();

                                columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (int)EnumNameCheck.InstrumentList)
                                 .Select(s => new EntityEDB_ColumnDynamicDefault()
                                 {
                                     Label = s.Name,
                                     LabelView = s.NameView,
                                 }).ToList();

                                foreach (var item in columnDynamics)
                                {
                                    if (!mdicProperty.ContainsKey(item.Label))
                                        mdicProperty.Add(item.Label, item.LabelView);
                                }

                                columnDynamics.Clear();
                            }
                            else
                            {
                                // Process data
                                var columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, mListTypeID)
                                       .Select(s => new EntityEDB_ColumnDynamicDefault()
                                       {
                                           Label = s.Name,
                                           LabelView = s.NameView,
                                       }).ToList();

                                foreach (var item in columnDynamics)
                                {
                                    if (!mdicProperty.ContainsKey(item.Label))
                                        mdicProperty.Add(item.Label, item.LabelView);
                                }

                                columnDynamics.Clear();
                            }
                        }
                    }
                }
            }
        }



    }
}
