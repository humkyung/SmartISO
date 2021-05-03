using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;

using Telerik.WinControls.UI;
using Configuration.Properties;
using EDCS.WF.BSL.Entities;
using CommonLibrary.CL;
using EDCS.WF.DSL.Entities;
using CommonLibrary;

namespace Configuration
{
    public partial class UserControlCommonAttributeMapping : UserControlBase
    {
        private enum LTColumn { LTSelect, LTEDB, LTSPPID, LTSP3D };
        private string[] mColumnName = new string[] { " ", "List", ToolType.SPPID, ToolType.SP3D };
        private int[] marColumnWidths = new int[] { 60, 200, 200, 200 };

        private Dictionary<string, string> mdicProperty_EDB = new Dictionary<string, string>();
        private Dictionary<string, string> mdicProperty_SPPID = new Dictionary<string, string>();
        private Dictionary<string, string> mdicProperty_SP3D = new Dictionary<string, string>();
        public UserControlCommonAttributeMapping()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            try
            {
                InitDropDownControl();

                EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;
                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                InitSetting(cpProject.ProjectMasterID, eListType);

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

        private void InitSetting(long lProjectMasterID, EntityEDB_ListType eListType)
        {
            InitSpread();
            initPropertyArray(lProjectMasterID, eListType);
            setSpread();
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void initPropertyArray(long lProjectMasterID, EntityEDB_ListType eListType)
        {
            mdicProperty_EDB.Clear();
            mdicProperty_SPPID.Clear();
            mdicProperty_SP3D.Clear();

            string sError = string.Empty;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //if (eListType.ListTypeID == (int)EnumNameCheck.InstrumentIndex || eListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
                if (eListType.ListTypeID == (int)EnumNameCheck.InstrumentIndex)
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
                        if (!mdicProperty_EDB.ContainsKey(item.Label))
                            mdicProperty_EDB.Add(item.Label, item.LabelView);
                    }

                    columnDynamics.Clear();

                    //columnDynamics = proxy.ReadCommonColumnDynamics(false, (int)EnumNameCheck.InstrumentList)
                    // .Select(s => new EntityEDB_ColumnDynamicDefault()
                    // {
                    //     Label = s.Name,
                    //     LabelView = s.NameView,
                    // }).ToList();

                    //foreach (var item in columnDynamics)
                    //{
                    //    if (!mdicProperty_EDB.ContainsKey(item.Label))
                    //        mdicProperty_EDB.Add(item.Label, item.LabelView);
                    //}

                    //columnDynamics.Clear();
                }
                else if (eListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
                {
                    var ColumnDynamicParentDefaults = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == (int)EnumNameCheck.InstrumentList).ToList();
                    List<EntityColumnHeaderMergeProperty> columnHeaderMergeProperties = null;
                    if (!String.IsNullOrWhiteSpace(ColumnDynamicParentDefaults.First().ColumnHeaderMergeProperty))
                    {
                        columnHeaderMergeProperties = XmlConverter.DeserializeObject<List<
                                               EntityColumnHeaderMergeProperty>>(ColumnDynamicParentDefaults.First().ColumnHeaderMergeProperty).Where(w => w.RowIndex == 0).ToList();
                        var columnDynamics = proxy.ReadColumnDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseDynamicDefaultName == ColumnDynamicParentDefaults.First().CaseDynamicDefaultName
                            && w.ListTypeID == (int)EnumNameCheck.InstrumentList).Select(s => new EntityEDB_ColumnDynamicDefault()
                            {
                                ColumnIndex = s.ColumnIndex,
                                Label = s.Label,
                                LabelView = s.LabelView,
                            }).ToList();

                        string oldMergeName = "";
                        int rowCount = 1;
                        foreach (var item in columnDynamics)
                        {
                            var columnHeaderMergeProperty = columnHeaderMergeProperties.Where(w => w.ColumnIndex == item.ColumnIndex).First();
                            int colSpan = columnHeaderMergeProperty.ColumnSpan;
                            if (colSpan > 1)
                            {
                                oldMergeName = columnHeaderMergeProperty.Text;
                                rowCount = colSpan;
                            }
                            else if (rowCount == 1)
                            {
                                oldMergeName = "";
                                rowCount = 1;
                            }
                            else
                            {
                                --rowCount;
                            }

                            string labelView = oldMergeName == "" ? item.LabelView : string.Format("{0} - {1}", oldMergeName, item.LabelView);

                            if (!mdicProperty_EDB.ContainsKey(item.Label))
                                mdicProperty_EDB.Add(item.Label, labelView);
                        }
                    }
                }
                else
                {
                    // Process data
                    var columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, eListType.ListTypeID)
                           .Select(s => new EntityEDB_ColumnDynamicDefault()
                           {
                               Label = s.Name,
                               LabelView = s.NameView,
                           }).ToList();

                    foreach (var item in columnDynamics)
                    {
                        if (!mdicProperty_EDB.ContainsKey(item.Label))
                            mdicProperty_EDB.Add(item.Label, item.LabelView);
                    }

                    columnDynamics.Clear();
                }

                if (eListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
                {
                    // Line List Property를 가져온다.
                    var columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (int)EnumNameCheck.LineList)
                           .Select(s => new EntityEDB_ColumnDynamicDefault()
                           {
                               Label = s.Name,
                               LabelView = s.NameView,
                           }).ToList();

                    foreach (var item in columnDynamics)
                    {
                        if (!mdicProperty_SPPID.ContainsKey(item.Label))
                            mdicProperty_SPPID.Add(item.Label, item.LabelView);
                    }
                }
                else
                {
                    // SPPID
                    var spProject = proxy.ReadSPTool_ProjectList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, ToolType.SPPID);
                    if (spProject != null)
                    {
                        Information Information = new Information();
                        Information.Project = spProject;
                        GeneralUtil.SetSPPIDPlantSchemaInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, spProject, ref Information);
                        DataTable dtDataFilter_EQUIPMENT = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, eListType.MappingSPPID);

                        GetDataFilter(dtDataFilter_EQUIPMENT, ref mdicProperty_SPPID);

                        if (eListType.MappingSPPID == Information.EQUIPMENT)
                        {
                            DataTable dtDataFilter_VESSEL = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, Information.VESSEL);
                            GetDataFilter(dtDataFilter_VESSEL, ref mdicProperty_SPPID);
                            DataTable dtDataFilter_EXCHANGER = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, Information.EXCHANGER);
                            GetDataFilter(dtDataFilter_EXCHANGER, ref mdicProperty_SPPID);
                            DataTable dtDataFilter_MECHANICAL = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, Information.MECHANICAL);
                            GetDataFilter(dtDataFilter_MECHANICAL, ref mdicProperty_SPPID);
                            DataTable dtDataFilter_EQUIPMENTOTHER = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, Information.EQUIPMENTOTHER);
                            GetDataFilter(dtDataFilter_EQUIPMENTOTHER, ref mdicProperty_SPPID);
                        }

                        Property.SetAddProperty(spProject.ProjectNo, ref mdicProperty_SPPID);

                        dtDataFilter_EQUIPMENT.Clear();
                        //lstDataColumn.Clear();
                    }

                    Information mInformation = new Information();
                    mdicProperty_SP3D = proxy.GetSP3DAttribute(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, eListType.ListTypeID);
                }

            }
        }

        private void GetDataFilter(DataTable dtColumn, ref Dictionary<string, string> dicProperty)
        {
            foreach (DataRow dr in dtColumn.Rows)
            {
                string sColumnName = CommonUtil.CommonLib.Util.NullToSpace(dr["NAME"]);
                string sColumnDisplay = CommonUtil.CommonLib.Util.NullToSpace(dr["DISPLAYNAME"]);

                if (!dicProperty.ContainsKey(sColumnName))
                    dicProperty.Add(sColumnName, sColumnDisplay);
            }

            dtColumn.Clear();
        }

        private void InitDropDownControl()
        {
            this.radDropDownListProject.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
            this.radDropDownListListType.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListListType_SelectedIndexChanged);

            #region # Project
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
            #endregion

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                radDropDownListListType.Items.Clear();
                var lstListTypes = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.TagNamingRuleFlag == true).ToList();

                foreach (var vListTypes in lstListTypes)
                {
                    RadListDataItem item = new RadListDataItem();
                    item.Text = vListTypes.NameView;
                    item.Value = vListTypes.ListTypeID;
                    item.Tag = vListTypes;

                    radDropDownListListType.Items.Add(item);
                }

                if (radDropDownListListType.Items.Count > 0)
                {
                    radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                }

            }

            this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);
            this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListListType_SelectedIndexChanged);

        }

        private void setSpread()
        {
            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            int iComboMaxDropCount = 18;

            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();
                cbtColumn.Editable = true;
                arItems = null;
                arItemsData = null;
                int iCellCount = 0;

                switch (i)
                {
                    case (int)LTColumn.LTSelect:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();

                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.LTEDB:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty_EDB.Count];
                        arItemsData = new string[mdicProperty_EDB.Count];
                        iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty_EDB.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                    case (int)LTColumn.LTSPPID:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty_SPPID.Count];
                        arItemsData = new string[mdicProperty_SPPID.Count];
                        iCellCount = 0;
                        foreach (KeyValuePair<string, string> pair in mdicProperty_SPPID.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                    case (int)LTColumn.LTSP3D:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty_SP3D.Count];
                        arItemsData = new string[mdicProperty_SP3D.Count];
                        iCellCount = 0;

                        EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;
                        if ("PipeRun" == eListType.MappingSP3D)
                        {
                            mdicProperty_SP3D["NPD"] = "Nominal Diameter";
                            mdicProperty_SP3D["DESIGN_PRESS"] = "DESIGN PRESSURE";
                            mdicProperty_SP3D["OPER_PRESS"] = "OPERERATING PRESSURE";
                            mdicProperty_SP3D["TEST_PRESS"] = "TEST PRESSURE";
                            mdicProperty_SP3D["DESIGN_TEMP"] = "DESIGN TEMPERATURE";
                            mdicProperty_SP3D["OPER_TEMP"] = "OPER TEMPERATURE";
                            mdicProperty_SP3D["INSUL_CODE"] = "INSULATION CODE";
                            mdicProperty_SP3D["INSUL_THK"] = "INSULATION THICKNESS";
                        }

                        foreach (KeyValuePair<string, string> pair in mdicProperty_SP3D.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value; // view
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

                if (i == (int)LTColumn.LTEDB)
                {
                    fpSpreadData.ActiveSheet.Columns[i].Locked = true;
                }
            }

            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                    EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;
                    // DataSetting
                    var lstAttributeMapping = proxy.ReadCommon_AttributeMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID, eListType.ListTypeID);

                    foreach (var itemEDB in mdicProperty_EDB)
                    {
                        int iRows = fpSpreadData.ActiveSheet.Rows.Count++;

                        var item = lstAttributeMapping.Where(w => w.Name_EDB == itemEDB.Key).FirstOrDefault();

                        fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.LTEDB].Value = itemEDB.Key;

                        if (item != null && item.Name_SPPID != null)
                            fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.LTSPPID].Value = item.Name_SPPID;

                        if (item != null && item.Name_SP3D != null)
                            fpSpreadData.ActiveSheet.Cells[iRows, (int)LTColumn.LTSP3D].Value = item.Name_SP3D;
                    }

                    if (eListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
                    {
                        fpSpreadData.ActiveSheet.Columns[(int)LTColumn.LTSPPID].Label = "Line List";
                        fpSpreadData.ActiveSheet.Columns[(int)LTColumn.LTSP3D].Visible = false;
                    }
                    else
                    {
                        fpSpreadData.ActiveSheet.Columns[(int)LTColumn.LTSPPID].Label = mColumnName[(int)LTColumn.LTSPPID];
                        fpSpreadData.ActiveSheet.Columns[(int)LTColumn.LTSP3D].Visible = true;
                    }
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            if (fpSpreadData.ActiveSheet != null)
            {
                int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
            }
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            for (int i = fpSpreadData.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSelect].Text;
                if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadData.ActiveSheet.Rows.Remove(i, 1);
                }
            }
        }

        public override void SaveData()
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {
                try
                {
                    EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                    EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;
                    string sMessage = string.Empty;

                    //if (CheckSaveAttributeMappingData())
                    //{
                    SaveAttributeMappingData(cpProject, eListType, ref sMessage);

                    if (sMessage.Length > 0)
                        MessageBox.Show(sMessage.ToString());
                    else
                        MessageBox.Show(this, Resources.Message10000, this.Text);
                    //}
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

        public bool CheckSaveAttributeMappingData()
        {
            bool bResult = true;

            List<string> lstEDB = new List<string>();
            List<string> lstSPPID = new List<string>();
            List<string> lstSP3D = new List<string>();

            Dictionary<int, string> dicoverlap_Item = new Dictionary<int, string>();

            for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
            {
                lstEDB.Add(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTEDB].Text);
                lstSPPID.Add(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSPPID].Text);
                lstSP3D.Add(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSP3D].Text);
            }

            int iRow = 0;
            foreach (string sItem in lstEDB.Distinct().ToArray())
            {
                if (!string.IsNullOrEmpty(sItem) && lstEDB.Count(n => n == sItem) > 1)
                {
                    if (!dicoverlap_Item.ContainsKey(iRow))
                        dicoverlap_Item.Add(iRow, string.Format("EDB : {0}", sItem));
                }

                iRow++;
            }

            iRow = 0;
            foreach (string sItem in lstSPPID.Distinct().ToArray())
            {
                if (!string.IsNullOrEmpty(sItem) && lstSPPID.Count(n => n == sItem) > 1)
                {
                    if (!dicoverlap_Item.ContainsKey(iRow))
                        dicoverlap_Item.Add(iRow, string.Format("SPPID : {0}", sItem));
                }

                iRow++;
            }

            iRow = 0;
            foreach (string sItem in lstSP3D.Distinct().ToArray())
            {
                if (!string.IsNullOrEmpty(sItem) && lstSP3D.Count(n => n == sItem) > 1)
                {
                    if (!dicoverlap_Item.ContainsKey(iRow))
                        dicoverlap_Item.Add(iRow, string.Format("SP3D : {0}", sItem));
                }

                iRow++;
            }

            if (dicoverlap_Item.Count > 0)
            {
                bResult = false;

                StringBuilder sbMessage = new StringBuilder();
                sbMessage.Append("Duplicate Items");
                foreach (KeyValuePair<int, string> kv in dicoverlap_Item)
                {
                    sbMessage.Append(string.Format("Item : {0}", kv.Value));
                }

                MessageBox.Show(this, sbMessage.ToString(), this.Text);
            }

            return bResult;
        }

        public void SaveAttributeMappingData(EntityCommon_ProjectMaster cpProject, EntityEDB_ListType eListType, ref string sMessage)
        {
            bool bResult = false;

            DataTable dt = new DataTable("Table1");
            dt.Columns.Add("MappingID", typeof(long));
            dt.Columns.Add("ProjectMasterID", typeof(long));
            dt.Columns.Add("ListTypeID", typeof(int));
            dt.Columns.Add("Name_EDB");
            dt.Columns.Add("NameView_EDB");
            dt.Columns.Add("Name_SPPID");
            dt.Columns.Add("NameView_SPPID");
            dt.Columns.Add("Name_SP3D");
            dt.Columns.Add("NameView_SP3D");
            dt.Columns.Add("SortOrder", typeof(int));

            int iSortOrder = 1;

            for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
            {
                bool bSaveFlag = false;

                bool bSPPIDFlag = false;
                bool bSP3DFlag = false;

                if (!string.IsNullOrEmpty(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSPPID].Text.Trim()))
                {
                    bSPPIDFlag = true;
                }

                if (!string.IsNullOrEmpty(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSP3D].Text.Trim()))
                {
                    bSP3DFlag = true;
                }

                if (bSPPIDFlag || bSP3DFlag)
                    bSaveFlag = true;

                if (bSaveFlag)
                {
                    DataRow drNew = dt.NewRow();

                    drNew["ProjectMasterID"] = cpProject.ProjectMasterID;
                    drNew["ListTypeID"] = eListType.ListTypeID;

                    if (!string.IsNullOrEmpty(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTEDB].Text.Trim()))
                    {
                        drNew["Name_EDB"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTEDB].Value.ToString();
                        drNew["NameView_EDB"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTEDB].Text;
                    }

                    if (!string.IsNullOrEmpty(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSPPID].Text.Trim()))
                    {
                        drNew["Name_SPPID"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSPPID].Value.ToString();
                        drNew["NameView_SPPID"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSPPID].Text;
                    }

                    if (!string.IsNullOrEmpty(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSP3D].Text.Trim()))
                    {
                        drNew["Name_SP3D"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSP3D].Value.ToString();
                        drNew["NameView_SP3D"] = fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.LTSP3D].Text;
                    }

                    drNew["SortOrder"] = iSortOrder;
                    dt.Rows.Add(drNew);
                    iSortOrder++;
                }
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bResult = proxy.SaveCommon_AttributeMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, cpProject.ProjectMasterID, eListType.ListTypeID, dt, ref sMessage);
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadData.ActiveSheet;

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

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadData.ActiveSheet;
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

        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {
                EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;

                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                InitSetting(cpProject.ProjectMasterID, eListType);

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
            }
        }

        private void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListListType.SelectedItem != null)
            {
                EntityEDB_ListType eListType = this.radDropDownListListType.SelectedItem.Tag as EntityEDB_ListType;

                EntityCommon_ProjectMaster cpProject = (EntityCommon_ProjectMaster)radDropDownListProject.SelectedItem.DataBoundItem;
                InitSetting(cpProject.ProjectMasterID, eListType);

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProject, this._entityEDBInformation, ToolType.Admin);
            }
        }
    }
}
