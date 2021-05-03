using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CommonUtil.CommonLib;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI;
using Configuration.Properties;

using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSPPIDAttributeSetting : UserControlBase
    {
        private bool mbChanged = false;

        public Information mInformation = new Information();

        private const string CONST_DEFAULT = "DEFAULT";

        //private int[] mColumnWidth = new int[] { 180, 180, 0, 0 };
        private int[] mColumnWidth = new int[] { 0, 180, 180, 0, 0, 0 };

        private int[] mColumnWidth_Attribute = new int[] { 60, 150, 150, 0, 0, 0 };

        private enum enColumn { No, AttributeName, DisplayName, AttName, Path, Key, Last };
        private string[] mColumnName = new string[] { "No", "Attribute Name", "Display Name", "Att Name", "Path", "Key" };
        private bool[] mColumnLock = new bool[] { true, true, true, true, true };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        private string mPreProject = string.Empty;
        private string mPreItemType = string.Empty;

        List<string> mAttributeUseItme = new List<string>();

        List<EntitySPPID_DataColumnGroupValue> mlstDefaultDataColumnGroupValue = null;

        private string Project
        {
            get
            {
                if (radDropDownListProjectList.Items.Count > 0)
                    return radDropDownListProjectList.Text;

                return string.Empty;
            }
        }

        private string ItemType
        {
            get
            {
                if (comboTreeBoxItemType.Nodes.Count > 0)
                    return comboTreeBoxItemType.Text;

                return string.Empty;
            }
        }

        public string AttributeSet
        {
            get
            {
                if (radDropDownListAttributeSet.Items.Count > 0)
                    return radDropDownListAttributeSet.Text;

                return string.Empty;
            }
        }

        public long AttributeID
        {
            get
            {
                if (radDropDownListAttributeSet.SelectedItem != null && radDropDownListAttributeSet.SelectedItem.Tag != null)
                    return (long)radDropDownListAttributeSet.SelectedItem.Tag;

                return -1;
            }
        }

        public UserControlSPPIDAttributeSetting()
        {
            InitializeComponent();
            //mInformation = Information.GetInstance();
        }

        public override void LoadData()
        {
            SetSpreadCount();
            InitDropdownListProject();
            InitDropdownListItemType(string.Empty);

            string sError = string.Empty;

            GeneralUtil.SetSchemaInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, radDropDownListProjectList, ref mInformation, ref sError);

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.comboTreeBoxItemType.SelectedChanged += new Configuration.UserControlComboTreeBox.TextChangingHandler(this.comboTreeBoxItemType_SelectedChanged);

            mPreItemType = comboTreeBoxItemType.Text;
            InitAttributeSetDropdownList(CONST_DEFAULT, ItemType);
            InitSpread();
            SetSpread();

            radDropDownListAttributeCheck.Text = "All";
        }

        private void SetSpreadCount()
        {
            labelUnSelectedDataFilterCount.Text = string.Format("Row Count : {0}", fpUnSelectedDataFilter.ActiveSheet.Rows.Count.ToString());
            labelSelectedDataFilterCount.Text = string.Format("Row Count : {0}", fpSelectedDataFilter.ActiveSheet.Rows.Count.ToString());
            labelSelectedAttributeCount.Text = string.Format("Row Count : {0}", fpSelectedAttribute.ActiveSheet.Rows.Count.ToString());
        }

        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void InitDropdownListItemType(string sItemType)
        {
            GeneralUtil.SetComboTreeBoxItemType(ref comboTreeBoxItemType);

            if (!string.IsNullOrEmpty(sItemType))
                comboTreeBoxItemType.Text = sItemType;
            else
                comboTreeBoxItemType.Text = Information.DRAWING;
        }

        private void InitAttributeSetDropdownList(string sAttributeName, string sItemType)
        {
            if (!string.IsNullOrEmpty(ItemType))
            {
                radDropDownListAttributeSet.Items.Clear();
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var vAttributeSet = proxy.ReadDataColumnGroup(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, sItemType, "Admin", false, true);

                    RadListDataItem oitemDefault = null;
                    RadListDataItem oitemSelectedAttributeSet = null;

                    foreach (var result in vAttributeSet)
                    {
                        RadListDataItem item = new RadListDataItem();
                        item.Text = result.Name;
                        item.Tag = result.ColumnGroupID;

                        if (sAttributeName == result.Name)
                        {
                            oitemSelectedAttributeSet = item;
                        }

                        if (CONST_DEFAULT == result.Name.ToUpper())
                            oitemDefault = item;

                        radDropDownListAttributeSet.Items.Add(item);
                    }

                    if (oitemSelectedAttributeSet != null)
                        radDropDownListAttributeSet.SelectedItem = oitemSelectedAttributeSet;
                    else if (oitemDefault != null)
                        radDropDownListAttributeSet.SelectedItem = oitemDefault;
                    else
                    {
                        if (radDropDownListAttributeSet.Items.Count > 0)
                        {
                            radDropDownListAttributeSet.Items[0].Selected = true;
                        }
                    }
                }
            }
        }

        private void InitSpread()
        {
            SetSpreadCount();

            SpreadUtil.InitSpread(fpUnSelectedDataFilter, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType
                , null, null, Color.LightGray, true, true);

            SpreadUtil.InitSpread(fpSelectedDataFilter, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType
                , null, null, Color.LightGray, true, true);

            SpreadUtil.InitSpread(fpSelectedAttribute, mColumnWidth_Attribute, mColumnName, mColumnLock, null, null, mColumnType
                , null, null, Color.LightGray, true, true);

            fpSelectedDataFilter.AllowRowMove = true;
            fpSelectedAttribute.AllowRowMove = true;

            foreach (Column col in fpUnSelectedDataFilter.ActiveSheet.Columns)
            {
                col.AllowAutoFilter = true;
                col.AllowAutoSort = true;
            }

            foreach (Column col in fpSelectedDataFilter.ActiveSheet.Columns)
            {
                col.AllowAutoFilter = true;
                col.AllowAutoSort = true;
            }

            foreach (Column col in fpSelectedAttribute.ActiveSheet.Columns)
            {
                col.AllowAutoFilter = true;
                col.AllowAutoSort = true;
            }
        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListProjectList.SelectedItem != null)
            {
                mbChanged = false;
                string sError = string.Empty;
                GeneralUtil.SetSchemaInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, radDropDownListProjectList, ref mInformation, ref sError);
                InitAttributeSetDropdownList(CONST_DEFAULT, ItemType);
                SetSpread();

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
            }
        }

        private void SetSpreadDataFilter()
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Load attribute set.  Please wait!";

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (mInformation != null)
                {
                    if (mInformation.Project != null)
                    {
                        if (!string.IsNullOrEmpty(ItemType))
                        {
                            mAttributeUseItme.Clear();

                            DataTable dtDataFilter = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, ItemType);
                            dtDataFilter.Columns.Add("ColumnOrder", typeof(int));

                            // 사용 정렬 순서 변경
                            List<EntitySPPID_DataFilter_ColumnOrder> lstSPPID_DataFilter_ColumnOrder = proxy.ReadSPPID_DataFilterbyItemType_ColumnOrder(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, ItemType);

                            if (lstSPPID_DataFilter_ColumnOrder.Count > 0)
                            {
                                foreach (DataRow drDataFilter in dtDataFilter.Rows)
                                {
                                    string sAtt_Name = Util.NullToSpace(drDataFilter["ATT_NAME"]);
                                    string sPATH = Util.NullToSpace(drDataFilter["PATH"]);
                                    string sKey = sAtt_Name + sPATH;

                                    EntitySPPID_DataFilter_ColumnOrder DataFilter = lstSPPID_DataFilter_ColumnOrder.Where(w => w.AttName == sAtt_Name && w.Path == sPATH).FirstOrDefault();

                                    if (DataFilter == null)
                                    {
                                        drDataFilter["ColumnOrder"] = 99999;
                                    }
                                    else
                                    {
                                        drDataFilter["ColumnOrder"] = DataFilter.ColumnOrder;
                                    }
                                }
                            }

                            // 미사용 항목
                            List<EntitySPPID_DataFilter> lstSPPID_DataFilter = proxy.ReadSPPID_DataFilterbyItemType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, ItemType);

                            List<EntitySPPID_DataColumnGroupValue> lstSPPID_AttributeSetValue = null;

                            if (radDropDownListAttributeSet.SelectedItem != null && radDropDownListAttributeSet.SelectedItem.Tag != null)
                            {
                                lstSPPID_AttributeSetValue = proxy.ReadDataColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, AttributeID);
                            }

                            mlstDefaultDataColumnGroupValue = proxy.ReadDataColumnGroupValue_Default(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, ItemType);

                            // UnSelect
                            int iRowCount = 0;
                            int iRowCountSelect = 0;

                            List<string> lstUseItem = new List<string>();

                            iRowCount = 0;
                            foreach (DataRow drDataFilter in dtDataFilter.Select("", "ColumnOrder, NAME"))
                            {
                                bool bNotUseFlag = true;

                                string sAtt_Name = Util.NullToSpace(drDataFilter["ATT_NAME"]);
                                string sNAME = Util.NullToSpace(drDataFilter["NAME"]);
                                string sDISPLAYNAME = Util.NullToSpace(drDataFilter["DISPLAYNAME"]);
                                string sPATH = Util.NullToSpace(drDataFilter["PATH"]);
                                string sKey = sAtt_Name + sPATH;

                                // 미사용 체크
                                EntitySPPID_DataFilter DataFilter = lstSPPID_DataFilter.Where(w => w.AttName == sAtt_Name && w.Path == sPATH).FirstOrDefault();

                                // 사용 항목
                                //EntitySPPID_DataFilter_ColumnOrder DataFilter_ColumnOrder = lstSPPID_DataFilter_ColumnOrder.Where(w => w.AttName == sAtt_Name && w.Path == sPATH).FirstOrDefault();

                                if (DataFilter == null)
                                {
                                    bNotUseFlag = false;
                                }

                                //if (!bNotUseFlag)
                                //{
                                //    if (DataFilter_ColumnOrder == null)
                                //    {
                                //        bNotUseFlag = true;
                                //    }
                                //}

                                if (bNotUseFlag)
                                {
                                    // 미사용
                                    iRowCount++;
                                    fpUnSelectedDataFilter.ActiveSheet.Rows.Count = iRowCount;
                                    fpUnSelectedDataFilter.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.AttributeName].Text = sNAME;
                                    fpUnSelectedDataFilter.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.DisplayName].Text = sDISPLAYNAME;
                                    fpUnSelectedDataFilter.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.AttName].Text = sAtt_Name;
                                    fpUnSelectedDataFilter.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Path].Text = sPATH;
                                    fpUnSelectedDataFilter.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Key].Text = sKey;
                                }
                                else
                                {
                                    // 사용
                                    iRowCountSelect++;
                                    fpSelectedDataFilter.ActiveSheet.Rows.Count = iRowCountSelect;

                                    fpSelectedDataFilter.ActiveSheet.Cells[iRowCountSelect - 1, (int)enColumn.AttributeName].Text = sNAME;
                                    fpSelectedDataFilter.ActiveSheet.Cells[iRowCountSelect - 1, (int)enColumn.DisplayName].Text = sDISPLAYNAME;
                                    fpSelectedDataFilter.ActiveSheet.Cells[iRowCountSelect - 1, (int)enColumn.AttName].Text = sAtt_Name;
                                    fpSelectedDataFilter.ActiveSheet.Cells[iRowCountSelect - 1, (int)enColumn.Path].Text = sPATH;
                                    fpSelectedDataFilter.ActiveSheet.Cells[iRowCountSelect - 1, (int)enColumn.Key].Text = sKey;

                                    lstUseItem.Add(sNAME);
                                }
                            }

                            // Attribute
                            iRowCount = 0;
                            if (lstSPPID_AttributeSetValue != null)
                            {
                                foreach (EntitySPPID_DataColumnGroupValue attribute in lstSPPID_AttributeSetValue)
                                {
                                    DataRow[] drRow = dtDataFilter.Select(string.Format("NAME = '{0}'", attribute.Name));

                                    if (drRow.Length > 0)
                                    {
                                        if (lstUseItem.Contains(attribute.Name))
                                        {

                                            iRowCount++;
                                            fpSelectedAttribute.ActiveSheet.Rows.Count = iRowCount;

                                            string sAtt_Name = Util.NullToSpace(drRow[0]["ATT_NAME"]);
                                            string sNAME = Util.NullToSpace(drRow[0]["NAME"]);
                                            string sDISPLAYNAME = Util.NullToSpace(drRow[0]["DISPLAYNAME"]);
                                            string sPATH = Util.NullToSpace(drRow[0]["PATH"]);

                                            string sKey = sAtt_Name + sPATH;

                                            mAttributeUseItme.Add(sKey);

                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.No].Value = iRowCount;
                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.AttributeName].Text = sNAME;
                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.DisplayName].Text = sDISPLAYNAME;
                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.AttName].Text = sAtt_Name;
                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Path].Text = sPATH;
                                            fpSelectedAttribute.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.Key].Text = sKey;
                                        }
                                    }
                                }
                            }

                            lstUseItem.Clear();

                            if (dtDataFilter != null)
                            {
                                dtDataFilter.Clear();
                                dtDataFilter.Dispose();
                            }
                        }
                    }
                }

                SetSpreadCount();
                mbChanged = false;
            }

            FrmProgressStatus.FormClose();
        }

        private void SetSpread()
        {
            InitSpread();
            SetSpreadDataFilter();
        }

        private void comboTreeBoxItemType_SelectedChanged(object sender, EventArgs e)
        {
            if (mPreItemType != string.Empty && mPreItemType != comboTreeBoxItemType.Text)
            {
                //if (mbChanged == true)
                //{
                //    if (SaveSetting(mPreItemType) == false)
                //    {
                //        comboTreeBoxItemType.Text = mPreItemType;
                //        return;
                //    }
                //}

                InitAttributeSetDropdownList(CONST_DEFAULT, ItemType);
                SetSpread();
            }

            mPreItemType = comboTreeBoxItemType.Text;
        }

        private void radButtonDataFilterAddAll_Click(object sender, EventArgs e)
        {
            AddAllItem(fpUnSelectedDataFilter, fpSelectedDataFilter);
            SetSpreadCount();
        }

        private void radButtonDataFilterAdd_Click(object sender, EventArgs e)
        {
            AddItem(fpUnSelectedDataFilter, fpSelectedDataFilter);
            SetSpreadCount();
        }

        private void radButtonDataFilterRemove_Click(object sender, EventArgs e)
        {
            DeleteItem(fpUnSelectedDataFilter, fpSelectedDataFilter);
            SetSpreadCount();
        }

        private void radButtonDataFilterRemoveAll_Click(object sender, EventArgs e)
        {
            DeleteAllItem(fpUnSelectedDataFilter, fpSelectedDataFilter);
            SetSpreadCount();
        }

        private void AddAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_NotUse.ActiveSheet != null)
            {
                int iRowCount = FpSpread_NotUse.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }

                for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }

                mbChanged = true;
            }
        }

        private void AddAllItemAttribute(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_NotUse.ActiveSheet != null)
            {
                FpSpread_Use.ActiveSheet.Rows.Clear();
                mAttributeUseItme.Clear();

                int iRowCount = FpSpread_NotUse.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }

                    mAttributeUseItme.Add(FpSpread_NotUse.ActiveSheet.Cells[i, (int)enColumn.Key].Text);
                }

                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn.No].Value = i + 1;
                }

                mbChanged = true;
            }
        }


        private void AddItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_NotUse.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_NotUse.ActiveSheet.ActiveCell != null)
            {
                FpSpread_Use.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn.Last; j++)
                {
                    FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                    i--;
                }
            }

            FpSpread_Use.ActiveSheet.SetActiveCell(FpSpread_Use.ActiveSheet.Rows.Count - 1, (int)enColumn.AttName, true);
            FpSpread_Use.ShowRow(0, FpSpread_Use.ActiveSheet.Rows.Count, VerticalPosition.Bottom);

            mbChanged = true;
        }

        private int GetMaxNo(FpSpread FpSpread_Use)
        {
            int iNo = 1;
            int iMaxNo = 1;

            for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
            {
                iNo = int.Parse(FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn.No].Text);
                if (iMaxNo < iNo)
                {
                    iMaxNo = iNo;
                }
            }

            return iMaxNo;
        }

        private void AddAttributeItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_NotUse.ActiveSheet.GetSelections();

            int iMaxNo = GetMaxNo(FpSpread_Use);

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    string sKey = FpSpread_NotUse.ActiveSheet.Cells[i, (int)enColumn.Key].Text;

                    if (!mAttributeUseItme.Contains(sKey))
                    {
                        FpSpread_Use.ActiveSheet.Rows.Count++;

                        for (int j = 0; j < (int)enColumn.Last; j++)
                        {
                            FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;

                            if (j == (int)enColumn.No)
                                FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Value = ++iMaxNo;
                        }

                        mAttributeUseItme.Add(sKey);
                    }
                }
            }
            else if (FpSpread_NotUse.ActiveSheet.ActiveCell != null)
            {
                string sKey = FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Index, (int)enColumn.Key].Text;

                if (!mAttributeUseItme.Contains(sKey))
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;
                    for (int j = 0; j < (int)enColumn.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Index, j].Text;

                        if (j == (int)enColumn.No)
                            FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Value = ++iMaxNo;
                    }

                    mAttributeUseItme.Add(sKey);
                }
            }

            FpSpread_Use.ActiveSheet.SetActiveCell(FpSpread_Use.ActiveSheet.Rows.Count - 1, (int)enColumn.AttName, true);
            FpSpread_Use.ShowRow(0, FpSpread_Use.ActiveSheet.Rows.Count, VerticalPosition.Bottom);

            mbChanged = true;
        }

        private void DeleteItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_Use.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_NotUse.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn.Last; j++)
                    {
                        FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_Use.ActiveSheet.ActiveCell != null)
            {
                FpSpread_NotUse.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn.Last; j++)
                {
                    FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_Use.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            List<string> lstDeleteItemName = new List<string>();
            bool bDeleteFalg = true;

            for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    string sAttributeName = FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text;

                    if (mlstDefaultDataColumnGroupValue.Where(w => w.Name == sAttributeName).Count() > 0)
                    {
                        lstDeleteItemName.Add(sAttributeName);
                    }
                }
            }

            if (lstDeleteItemName.Count > 0)
            {
                StringBuilder sb = new StringBuilder();
                sb.Append("Default Attribute setting");
                sb.AppendLine("Are you sure you want to move?");

                foreach (string sName in lstDeleteItemName)
                {
                    sb.AppendLine(string.Format("'{0}'", sName));
                }

                DialogResult result = MessageBox.Show(sb.ToString(), this.Text, MessageBoxButtons.OKCancel, MessageBoxIcon.Information);

                if (result == DialogResult.Cancel)
                    bDeleteFalg = false;

            }

            if (bDeleteFalg)
            {
                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        string sKey = FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn.Key].Text;
                        mAttributeUseItme.Remove(sKey);
                        int irefRow = 0;
                        int irefCol = 0;
                        fpSelectedAttribute.Search(fpSelectedAttribute.ActiveSheetIndex, sKey, false, false, false, false, 0, (int)enColumn.Key, ref irefRow, ref irefCol);

                        if (irefRow > -1)
                            fpSelectedAttribute.ActiveSheet.Rows[irefRow].Remove();

                        FpSpread_Use.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }

                lstDeleteItemName.Clear();

                FpSpread_NotUse.ActiveSheet.SetActiveCell(FpSpread_NotUse.ActiveSheet.Rows.Count - 1, (int)enColumn.AttName, true);
                FpSpread_NotUse.ShowRow(0, FpSpread_NotUse.ActiveSheet.Rows.Count, VerticalPosition.Bottom);

                mbChanged = true;
            }
            else
            {
                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_Use.ActiveSheet.Rows[i].Tag = null;
                    }
                }
            }
        }

        private void DeleteAttributeItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_Use.ActiveSheet.GetSelections();

            bool bDeleteFlag = false;

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }

                bDeleteFlag = true;
            }
            else if (FpSpread_Use.ActiveSheet.ActiveCell != null)
            {
                FpSpread_Use.ActiveSheet.ActiveCell.Row.Tag = "Delete";

                bDeleteFlag = true;
            }

            if (bDeleteFlag)
            {
                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        mAttributeUseItme.Remove(FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn.Key].Text);
                        FpSpread_Use.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }
                mbChanged = true;
            }
        }

        private void DeleteAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            bool bDeleteFalg = true;

            StringBuilder sb = new StringBuilder();
            sb.Append("Are you sure you want to move?");
            DialogResult result = MessageBox.Show(sb.ToString(), this.Text, MessageBoxButtons.OKCancel, MessageBoxIcon.Information);

            if (result == DialogResult.Cancel)
                bDeleteFalg = false;

            {
                if (FpSpread_Use.ActiveSheet != null)
                {
                    int iRowCount = FpSpread_Use.ActiveSheet.Rows.Count;

                    for (int i = 0; i < iRowCount; i++)
                    {
                        FpSpread_NotUse.ActiveSheet.Rows.Count++;

                        for (int j = 0; j < (int)enColumn.Last; j++)
                        {
                            FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                                = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                        }
                        FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                    }

                    for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                    {
                        if (i < 0)
                            break;

                        if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                        {
                            FpSpread_Use.ActiveSheet.Rows[i].Remove();
                            i--;
                        }
                    }

                    fpSelectedAttribute.ActiveSheet.Rows.Clear();
                    mAttributeUseItme.Clear();

                    mbChanged = true;
                }
            }
        }

        private void DeleteAttributeAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_Use.ActiveSheet != null)
            {
                FpSpread_Use.ActiveSheet.Rows.Clear();
                mAttributeUseItme.Clear();
                mbChanged = true;
            }
        }

        private void radButtonAttributeAddAll_Click(object sender, EventArgs e)
        {
            AddAllItemAttribute(fpSelectedDataFilter, fpSelectedAttribute);
            SetSpreadCount();
        }

        private void radButtonAttributeAdd_Click(object sender, EventArgs e)
        {
            AddAttributeItem(fpSelectedDataFilter, fpSelectedAttribute);
            SetSpreadCount();
        }

        private void radButtonAttributeRemove_Click(object sender, EventArgs e)
        {
            DeleteAttributeItem(fpSelectedDataFilter, fpSelectedAttribute);
            SetSpreadCount();
        }

        private void radButtonAttributeRemoveAll_Click(object sender, EventArgs e)
        {
            DeleteAttributeAllItem(fpSelectedDataFilter, fpSelectedAttribute);
            SetSpreadCount();
        }

        private void radTextBoxUnSelectedDataFilter_TextChanged(object sender, EventArgs e)
        {
            string sText = radTextBoxUnSelectedDataFilter.Text;

            SheetView sheetView = fpUnSelectedDataFilter.ActiveSheet;
            SpreadUtil.FilteringText(ref sheetView, (int)enColumn.AttributeName, sText);
        }

        private void radTextBoxSelectedDataFilter_TextChanged(object sender, EventArgs e)
        {
            string sText = radTextBoxSelectedDataFilter.Text;

            SheetView sheetView = fpSelectedDataFilter.ActiveSheet;
            SpreadUtil.FilteringText(ref sheetView, (int)enColumn.AttributeName, sText);
        }

        private void radTextBoxAttributeFilter_TextChanged(object sender, EventArgs e)
        {
            string sText = radTextBoxAttributeFilter.Text;

            SheetView sheetView = fpSelectedAttribute.ActiveSheet;
            SpreadUtil.FilteringText(ref sheetView, (int)enColumn.AttributeName, sText);
        }

        public override void SaveData()
        {
            //bool bSaveFlag = false;

            //bSaveFlag = SaveSetting(ItemType);

            //if (bSaveFlag)
            //{
            //    InitAttributeSetDropdownList(radDropDownListAttributeSet.SelectedItem.Text, ItemType);
            //    MessageBox.Show(this, "Saved successfully!", this.Text);
            //}
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            CreateData(ItemType);
        }

        //private bool SaveSetting(string sItemType)
        //{
        //    bool bReturn = false;

        //    if (mbChanged)
        //    {
        //        if (MessageBox.Show(string.Format("Do you want to save current setting?"), this.Text, MessageBoxButtons.YesNo) == DialogResult.Yes)
        //        {
        //            if (fpSelectedDataFilter.ActiveSheet.Rows.Count == 0 && fpSelectedAttribute.ActiveSheet.Rows.Count == 0)
        //            {
        //                MessageBox.Show("Select attribute!", this.Text);
        //                return bReturn;
        //            }
        //        }
        //        else
        //        {
        //            return true;
        //        }
        //    }

        //    bReturn = false;

        //    if (radDropDownListAttributeSet.Text == string.Empty)
        //    {
        //        bReturn = CreateData(sItemType);
        //    }
        //    else
        //    {
        //        bReturn = SaveData(sItemType, AttributeSet);
        //    }

        //    return bReturn;
        //}

        private bool CreateData(string sItemType)
        {
            bool bReturn = true;

            try
            {
                if (mInformation == null || mInformation.Project == null)
                {
                    MessageBox.Show(string.Format(Resources.Message10021, "Project"), this.Text, MessageBoxButtons.YesNo);
                    return false;
                }

                if (ItemType == string.Empty)
                {
                    MessageBox.Show(string.Format(Resources.Message10021, "Item Type"), this.Text, MessageBoxButtons.YesNo);
                    return false;
                }

                FrmSPPIDAttributeSetting_Create frm = new FrmSPPIDAttributeSetting_Create(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sItemType);
                if (frm.ShowDialog(this) == DialogResult.OK)
                {
                    this.radDropDownListAttributeSet.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListAttributeSet_SelectedIndexChanged);
                    radDropDownListAttributeSet.Items.Add(frm.AttributeSetName);

                    RadListDataItem oitemSelectedAttributeSet = null;
                    foreach (RadListDataItem it in radDropDownListAttributeSet.Items)
                    {
                        if (frm.AttributeSetName != string.Empty)
                        {
                            if (it.Text == frm.AttributeSetName)
                            {
                                it.Tag = (long)-1;
                                oitemSelectedAttributeSet = it;
                            }
                        }
                    }

                    if (oitemSelectedAttributeSet != null)
                        radDropDownListAttributeSet.SelectedItem = oitemSelectedAttributeSet;

                    this.radDropDownListAttributeSet.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListAttributeSet_SelectedIndexChanged);

                    SaveAttribute(true, frm.DefaultAttributeGroupID, frm.AllFlag);
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

            return bReturn;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            try
            {
                if (AttributeSet == string.Empty)
                {
                    MessageBox.Show(string.Format(Resources.Message10021, "Attribute name"), this.Text);
                    return;
                }

                if (MessageBox.Show(string.Format(Resources.Message10016, AttributeSet), this.Text, MessageBoxButtons.YesNo) == DialogResult.Yes)
                {
                    try
                    {
                        FrmProgressStatus.FormShow();
                        FrmProgressStatus.SetMarquee();
                        FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
                        FrmProgressStatus.ProgressPer = "Delete attribute set.  Please wait!";

                        string sError = string.Empty;
                        var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy as IDisposable)
                        {
                            proxy.DeleteAdminAttributeSet(EDCS.WF.BSL.Variable.SharedVariable.IsIE, AttributeID, ref sError);
                        }

                        InitAttributeSetDropdownList(CONST_DEFAULT, ItemType);
                        SetSpread();
                        FrmProgressStatus.FormClose();
                    }
                    catch (Exception ex)
                    {
                        FrmProgressStatus.FormClose();
                        MessageBox.Show(string.Format(Resources.Message10015, "Delete", ex.Message), "Attribute Setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }

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

        private void radDropDownListAttributeSet_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetSpread();
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            SetButtonUp(fpSelectedAttribute);
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            SetButtonDown(fpSelectedAttribute);
        }

        private void SetButtonDown(FpSpread spread)
        {
            CellRange cr;
            SheetView sheetView = spread.ActiveSheet;
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

                mbChanged = true;
            }
            catch (Exception ex)
            {

            }
        }

        private void SetButtonUp(FpSpread spread)
        {
            CellRange cr;
            SheetView sheetView = spread.ActiveSheet;

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

                mbChanged = true;
            }
            catch (Exception ex)
            {

            }
        }

        private void radButtonItemFilterSave_Click(object sender, EventArgs e)
        {
            SaveDataFilter(ItemType);
        }

        private bool SaveDataFilter(string sItemType)
        {
            bool bReturn = false;

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Saving attribute set.  Please wait!";

            DataTable dtSPPID_DataFilter = new DataTable("SPPID_DataFilter");
            dtSPPID_DataFilter.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnOrder", typeof(int));

            DataTable dtSPPID_DataFilter_ColumnOrder = new DataTable("SPPID_DataFilter_ColumnOrder");
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnOrder", typeof(int));

            try
            {
                for (int i = 0; i < fpUnSelectedDataFilter.ActiveSheet.Rows.Count; i++)
                {
                    DataRow drInsertrow = dtSPPID_DataFilter.NewRow();
                    drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                    drInsertrow["ItemType"] = sItemType;
                    drInsertrow["AttName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttName].Text.ToString().Trim();
                    drInsertrow["ColumnName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                    //drInsertrow["ColumnName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.DisplayName].Text.ToString().Trim();
                    drInsertrow["Path"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.Path].Text.ToString().Trim();
                    drInsertrow["ColumnOrder"] = i;
                    dtSPPID_DataFilter.Rows.Add(drInsertrow);
                }

                for (int i = 0; i < fpSelectedDataFilter.ActiveSheet.Rows.Count; i++)
                {
                    DataRow drInsertrow = dtSPPID_DataFilter_ColumnOrder.NewRow();
                    drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                    drInsertrow["ItemType"] = sItemType;
                    drInsertrow["AttName"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttName].Text.ToString().Trim();
                    drInsertrow["ColumnName"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                    //drInsertrow["ColumnName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.DisplayName].Text.ToString().Trim();
                    drInsertrow["Path"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.Path].Text.ToString().Trim();
                    drInsertrow["ColumnOrder"] = i;
                    dtSPPID_DataFilter_ColumnOrder.Rows.Add(drInsertrow);
                }

                string sError = string.Empty;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    proxy.SaveAdminDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, sItemType, dtSPPID_DataFilter, dtSPPID_DataFilter_ColumnOrder, ref sError);
                }

                bReturn = true;
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(string.Format(Resources.Message10015, "Save Data", ex.Message), "Attribute Setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
                bReturn = false;
            }
            finally
            {
                if (dtSPPID_DataFilter != null)
                {
                    dtSPPID_DataFilter.Clear();
                    dtSPPID_DataFilter.Dispose();
                }

                if (dtSPPID_DataFilter_ColumnOrder != null)
                {
                    dtSPPID_DataFilter_ColumnOrder.Clear();
                    dtSPPID_DataFilter_ColumnOrder.Dispose();
                }
            }

            FrmProgressStatus.FormClose();

            return bReturn;
        }


        private bool SaveDataFilter_Order(string sItemType)
        {
            bool bReturn = false;

            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Saving attribute set.  Please wait!";

            DataTable dtSPPID_DataFilter = new DataTable("SPPID_DataFilter");
            dtSPPID_DataFilter.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter.Columns.Add("ColumnOrder", typeof(int));

            DataTable dtSPPID_DataFilter_ColumnOrder = new DataTable("SPPID_DataFilter_ColumnOrder");
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("DataFilterID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ProjectID", typeof(long));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ItemType", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("AttName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnName", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("Path", typeof(String));
            dtSPPID_DataFilter_ColumnOrder.Columns.Add("ColumnOrder", typeof(int));

            try
            {
                for (int i = 0; i < fpUnSelectedDataFilter.ActiveSheet.Rows.Count; i++)
                {
                    // 미사용 항목
                    DataRow drInsertrow = dtSPPID_DataFilter.NewRow();
                    drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                    drInsertrow["ItemType"] = sItemType;
                    drInsertrow["AttName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttName].Text.ToString().Trim();
                    drInsertrow["ColumnName"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                    drInsertrow["Path"] = fpUnSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.Path].Text.ToString().Trim();
                    drInsertrow["ColumnOrder"] = i;
                    dtSPPID_DataFilter.Rows.Add(drInsertrow);
                }

                for (int i = 0; i < fpSelectedDataFilter.ActiveSheet.Rows.Count; i++)
                {
                    // 사용 항목 (정렬 순서를  Default + 알파벳 순으로)
                    DataRow drInsertrow = dtSPPID_DataFilter_ColumnOrder.NewRow();
                    drInsertrow["ProjectID"] = mInformation.Project.ProjectID;
                    drInsertrow["ItemType"] = sItemType;
                    drInsertrow["AttName"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttName].Text.ToString().Trim();
                    drInsertrow["ColumnName"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                    drInsertrow["Path"] = fpSelectedDataFilter.ActiveSheet.Cells[i, (int)enColumn.Path].Text.ToString().Trim();
                    drInsertrow["ColumnOrder"] = 9999999;
                    dtSPPID_DataFilter_ColumnOrder.Rows.Add(drInsertrow);
                }

                int iLastOrder = 0;

                for (int i = 0; i < fpSelectedAttribute.ActiveSheet.Rows.Count; i++)
                {
                    // 정렬 순서를 Default 순으로 변경
                    string sAttributeName = fpSelectedAttribute.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                    string sPath = fpSelectedAttribute.ActiveSheet.Cells[i, (int)enColumn.Path].Text.ToString().Trim();

                    DataRow[] dr = dtSPPID_DataFilter_ColumnOrder.Select(string.Format("ColumnName ='{0}' and Path='{1}' ", sAttributeName, sPath));
                    if (dr.Length > 0)
                    {
                        dr[0]["ColumnOrder"] = i;
                        iLastOrder = i;
                    }
                }

                DataRow[] drOrder = dtSPPID_DataFilter_ColumnOrder.Select("", "ColumnOrder , ColumnName ASC");

                foreach (DataRow dr in drOrder)
                {
                    string sColumnOrder = Util.NullToSpace(dr["ColumnOrder"]);

                    if (sColumnOrder == "9999999")
                    {
                        dr["ColumnOrder"] = ++iLastOrder;
                    }

                }

                string sError = string.Empty;

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    proxy.SaveAdminDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, sItemType, dtSPPID_DataFilter, dtSPPID_DataFilter_ColumnOrder, ref sError);
                }

                bReturn = true;
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                MessageBox.Show(string.Format(Resources.Message10015, "Save Data", ex.Message), "Attribute Setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
                bReturn = false;
            }
            finally
            {
                if (dtSPPID_DataFilter != null)
                {
                    dtSPPID_DataFilter.Clear();
                    dtSPPID_DataFilter.Dispose();
                }

                if (dtSPPID_DataFilter_ColumnOrder != null)
                {
                    dtSPPID_DataFilter_ColumnOrder.Clear();
                    dtSPPID_DataFilter_ColumnOrder.Dispose();
                }
            }

            FrmProgressStatus.FormClose();

            return bReturn;
        }

        private void SaveAttribute(bool Create, long lDefaultAttributeGroupID, bool bAllFlag)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                DataTable dtSPPID_AttributeSetValue = new DataTable("SPPID_DataColumnGroupValue");
                dtSPPID_AttributeSetValue.Columns.Add("ColumnID", typeof(long));
                dtSPPID_AttributeSetValue.Columns.Add("ColumnGroupID", typeof(long));
                dtSPPID_AttributeSetValue.Columns.Add("Name", typeof(String));
                dtSPPID_AttributeSetValue.Columns.Add("ColumnOrder", typeof(int));

                try
                {
                    long lAttributeSetID = -1;

                    if (!Create)
                    {
                        if (radDropDownListAttributeSet.SelectedItem != null && radDropDownListAttributeSet.SelectedItem.Tag != null)
                        {
                            lAttributeSetID = AttributeID;
                        }

                        for (int i = 0; i < fpSelectedAttribute.ActiveSheet.Rows.Count; i++)
                        {
                            DataRow drInsertAttribute = dtSPPID_AttributeSetValue.NewRow();

                            drInsertAttribute["Name"] = fpSelectedAttribute.ActiveSheet.Cells[i, (int)enColumn.AttributeName].Text.ToString().Trim();
                            drInsertAttribute["ColumnOrder"] = i;

                            dtSPPID_AttributeSetValue.Rows.Add(drInsertAttribute);

                        }
                    }
                    else
                    {
                        if (!bAllFlag)
                        {
                            List<EntitySPPID_DataColumnGroupValue> lstDataColumnGroupValue = proxy.ReadDataColumnGroupValue(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lDefaultAttributeGroupID);

                            if (lstDataColumnGroupValue.Count > 0)
                            {
                                int i = 0;
                                foreach (var GroupValue in lstDataColumnGroupValue)
                                {
                                    DataRow drInsertAttribute = dtSPPID_AttributeSetValue.NewRow();

                                    drInsertAttribute["Name"] = GroupValue.Name;
                                    drInsertAttribute["ColumnOrder"] = i;

                                    dtSPPID_AttributeSetValue.Rows.Add(drInsertAttribute);
                                    i++;
                                }
                            }
                        }
                        else
                        {
                            List<EntitySPPID_DataFilter_ColumnOrder> lstSPPID_DataFilter_ColumnOrder = proxy.ReadSPPID_DataFilterbyItemType_ColumnOrder(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, ItemType);

                            if (lstSPPID_DataFilter_ColumnOrder.Count > 0)
                            {
                                int i = 0;
                                foreach (var GroupValue in lstSPPID_DataFilter_ColumnOrder)
                                {
                                    DataRow drInsertAttribute = dtSPPID_AttributeSetValue.NewRow();

                                    drInsertAttribute["Name"] = GroupValue.ColumnName;
                                    drInsertAttribute["ColumnOrder"] = i;

                                    dtSPPID_AttributeSetValue.Rows.Add(drInsertAttribute);
                                    i++;
                                }
                            }
                        }
                    }

                    string sError = string.Empty;
                    string sAttributeSet = AttributeSet;

                    proxy.SaveAdminAttributeSet(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, ItemType, lAttributeSetID, AttributeSet, dtSPPID_AttributeSetValue, ref sError);
                    InitAttributeSetDropdownList(sAttributeSet, ItemType);
                }
                catch (Exception ex)
                {
                    FrmProgressStatus.FormClose();
                    MessageBox.Show(string.Format(Resources.Message10015, "Save Data", ex.Message), "Attribute Setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                finally
                {
                    if (dtSPPID_AttributeSetValue != null)
                    {
                        dtSPPID_AttributeSetValue.Clear();
                        dtSPPID_AttributeSetValue.Dispose();
                    }
                }

                mbChanged = false;
            }

        }

        private void radButtonAttributeSave_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Saving attribute set.  Please wait!";
            try
            {
                //if (AttributeSet.ToUpper() == CONST_DEFAULT)
                SaveDataFilter_Order(ItemType);
                SaveAttribute(false, -1, false);

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

        private void radButtonUpFilter_Click(object sender, EventArgs e)
        {
            SetButtonUp(fpSelectedDataFilter);
        }

        private void radButtonDownFilter_Click(object sender, EventArgs e)
        {
            SetButtonDown(fpSelectedDataFilter);
        }

        private void radButtonCheckAttribute_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Check Data.  Please wait!";

            try
            {
                if (radDropDownListAttributeCheck.Text == "All")
                {
                    SetCheckAttribute("Not Available", ItemType, fpUnSelectedDataFilter.ActiveSheet);
                    SetCheckAttribute("Available", ItemType, fpSelectedDataFilter.ActiveSheet);
                }
                else if (radDropDownListAttributeCheck.Text == "Available")
                {
                    SetCheckAttribute("Available", ItemType, fpSelectedDataFilter.ActiveSheet);
                }
                else
                {
                    SetCheckAttribute("Not Available", ItemType, fpUnSelectedDataFilter.ActiveSheet);
                }
                FrmProgressStatus.FormClose();
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
        }

        private void SetCheckAttribute(string sTitle, string sItemType, SheetView sheet)
        {

            // Not Available 과 Available 의 데이터를 50건식 조회 해서 데이터 가 있는지 체크 해서 표시
            // 모든 플랜트를 뒤져서 데이터 조회

            //입력 : 열 흰색
            //입력 X : 열 회색
            //입력 데이터 있는것 : 텍스트 색상 변경 (파란색류로)
            //입력 X 데이터 있는것 : 텍스트 색상 변경 (파란색류로)

            // ReadOnly ReadDataColumn_sItemType 함수 참조
            // 데이터 있는 것은 딕셔너리로 받을것

            // GetConfigData_Read
            // SetItemType_AttributesAndSelected_Read

            // CreateDataValue

            if (sheet.Rows.Count == 0)
                return;

            List<string> lstItemType = new List<string>();
            lstItemType.Add(sItemType);

            int iMaxCount = 100;

            List<string> lstItem = new List<string>();

            List<string> lstReadOnlyColumn = new List<string>();
            List<string> lstUseColumn = new List<string>();

            // 모든 플랜트 체크 
            List<InfoPlant> lstPlant = new List<InfoPlant>();
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, mInformation.Project, GeneralUtil.InitConnection(mInformation));
            }

            foreach (InfoPlant infoPlant in lstPlant)
            {
                mInformation.Plant = infoPlant;

                FrmProgressStatus.ProgressPer = string.Format("{0}-{1} : check data", sTitle, mInformation.Plant.SiteName);

                for (int i = 0; i < sheet.Rows.Count; i++)
                {
                    FrmProgressStatus.ProgressPer = string.Format("{0}-{1} : check data [{2}/{3}]", sTitle, mInformation.Plant.SiteName, i, sheet.Rows.Count);

                    string sAttributeName = sheet.Cells[i, (int)enColumn.AttributeName].Text;

                    if (!lstItem.Contains(sAttributeName))
                    {
                        lstItem.Add(sAttributeName);
                    }

                    if ((i % iMaxCount == 0 && i != 0) || lstItem.Count > 0 && i == sheet.Rows.Count - 1)
                    {
                        // Plant 단위로 100개식 끈어서 체크

                        var proxy2 = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                        using (proxy2 as IDisposable)
                        {
                            DataSet dsConfigs = proxy2.GetConfigData_Read(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, lstItemType, lstItem);

                            Dictionary<string, bool> dicitemValue = proxy2.CreateDataValue_Read(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, sItemType, dsConfigs);

                            // 사용한 Plant 데이터가 있으면 제외
                            foreach (KeyValuePair<string, bool> kv in dicitemValue)
                            {
                                if (kv.Value && !lstUseColumn.Contains(kv.Key))
                                    lstUseColumn.Add(kv.Key.ToUpper());
                            }

                            DataTable dtDataColumn = dsConfigs.Tables[ItemType + "_Selected"];

                            foreach (DataRow drSelect in dtDataColumn.Rows)
                            {
                                string sNAME = Util.NullToString(drSelect["NAME"]);
                                string sPATH = Util.NullToString(drSelect["PATH"]);
                                string sATT_NAME = Util.NullToString(drSelect["ATT_NAME"]);
                                string sKey = string.Format("{0}%{1}%{2}", sNAME, sPATH, sATT_NAME);

                                string sReadOnlyFlag = Util.NullToString(drSelect["ReadOnlyMask"]);
                                bool bReadOnlyFlag = false;
                                switch (sReadOnlyFlag)
                                {
                                    case "":
                                        bReadOnlyFlag = false;
                                        break;
                                    case "0":
                                        bReadOnlyFlag = false;
                                        break;
                                    default:
                                        bReadOnlyFlag = true;
                                        break;
                                }

                                if (bReadOnlyFlag)
                                    lstReadOnlyColumn.Add(sNAME.ToUpper());
                            }
                        }

                        lstItem.Clear();
                    }
                }
            }

            // 색상 표시 ReadOnly 설정
            for (int i = 0; i < sheet.Rows.Count; i++)
            {
                FrmProgressStatus.ProgressPer = string.Format("Attribute Color Setting [{0}/{1}]", i, sheet.Rows.Count);

                string sAttributeName = sheet.Cells[i, (int)enColumn.AttributeName].Text.ToUpper();
                string sPath = sheet.Cells[i, (int)enColumn.Path].Text.ToUpper();
                string sAttName = sheet.Cells[i, (int)enColumn.AttName].Text.ToUpper();

                string sKey = string.Format("{0}%{1}%{2}", sAttributeName, sPath, sAttName);

                bool readOnly = false;

                if (lstReadOnlyColumn.Contains(sAttributeName))
                {
                    readOnly = true;
                }
                else
                {
                    if (CheckReadOnlyAttribute(sAttributeName) == true)
                        readOnly = true;
                }

                if (sAttributeName.Equals("PLANTGROUP.NAME"))
                    readOnly = false;

                if (readOnly)
                {
                    sheet.Rows[i].BackColor = Color.LightGray;
                }

                // 데이터 있는 행 ForeColor blue 

                if (!lstUseColumn.Contains(sKey) && !lstUseColumn.Contains(sAttributeName))
                    sheet.Rows[i].ForeColor = Color.Blue;
            }

        }

        public bool CheckReadOnlyAttribute(string sAttributeName)
        {
            bool bReadOnly = false;
            string[] tempName = sAttributeName.Split('.');

            //아래 인 경우를 제외하고 "."이 포함된 경우에는 ReadOnly로 한다.
            if (tempName.Length > 1)
            {
                bool tempIsFind = false;

                switch (tempName[0])
                {
                    case "REVISION":
                    case "HOLDSTATUS":
                    case "PROCESSDESIGN":
                    case "PROCESSDESIGNTUBE":
                    case "PROCESSOPERATINGTUBE":
                    case "PROCESSALTERNATEOPERATINGTUBE":
                    case "PROCESSOPERATING":
                    case "PROCESSALTERNATEDESIGN":
                    case "PROCESSALTERNATEDESIGNTUBE":
                    case "PROCESSALTERNATEOPERATING":
                    case "PROCESSTEST":
                    case "PLANTGROUP":
                        tempIsFind = true;
                        break;
                }

                if (!tempIsFind)
                    bReadOnly = true;
            }
            else if (sAttributeName.Equals("ITEMTAG"))
            {
                bReadOnly = true;
            }

            return bReadOnly;
        }

    }
}
