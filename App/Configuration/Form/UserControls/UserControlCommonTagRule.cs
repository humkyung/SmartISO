using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;

using Configuration.Properties;
using EDCS.WF.DSL.Entities;
using CommonLibrary;

namespace Configuration
{
    public partial class UserControlCommonTagRule : UserControl
    {


        Information m_Information;

        private enum LTColumn { LTSelect, LTProperty, LTAllowNull, LTSeparator, LTLength, LTFillString, LTAlignment, LTLast };
        private string[] mColumnName = new string[] { " ", "Property", "AllowNulls", "Separator", "Length", "Fill String", "Alignment" };
        private int[] marColumnWidths = new int[] { 60, 250, 100, 100, 100, 100, 100 };
        private bool[] marColumnLock = new bool[] { false, false, false, false, false, false, false };

        string mToolType = string.Empty;
        long mProjectMasterID = -1;
        EntityEDB_ListType mListType = null;

        string mMappingSPPID_ItemType = string.Empty;
        string mMappingSPI_ItemType = string.Empty;
        string mMappingSP3D_ItemType = string.Empty;

        private Dictionary<string, string> mdicProperty = new Dictionary<string, string>();

        public UserControlCommonTagRule()
        {
            InitializeComponent();
        }

        public UserControlCommonTagRule(bool isIE, string sToolType, long lProjectMasterID, EntityEDB_ListType eListType)
        {
            InitializeComponent();



            m_Information = Information.GetInstance();
            mToolType = sToolType;
            mProjectMasterID = lProjectMasterID;
            mListType = eListType;
            if (sToolType == ToolType.EDB)
                radLabelTool.Text = "List";
            else
                radLabelTool.Text = sToolType;

            initPropertyArray(sToolType, lProjectMasterID, eListType);
            initSpread();
            setSpread();
        }

        private void initSpread()
        {
            SpreadUtil.InitSpread(fpSpreadTagFormat, marColumnWidths, mColumnName, marColumnLock, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void initPropertyArray(string sToolType, long lProjectMasterID, EntityEDB_ListType eListType)
        {
            mdicProperty.Clear();
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (sToolType.Equals(ToolType.EDB))
                {
                    var columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, eListType.ListTypeID)
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

                }
                else if (sToolType.Equals(ToolType.SPPID))
                {
                    var spProject = proxy.ReadSPTool_ProjectList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, ToolType.SPPID);
                    if (spProject != null)
                    {
                        Information Information = new Information();
                        Information.Project = spProject;

                        GeneralUtil.SetSPPIDPlantSchemaInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, spProject, ref Information);
                        DataTable dtDataFilter = proxy.GetDataFilter(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information, eListType.MappingSPPID);

                        foreach (DataRow dr in dtDataFilter.Rows)
                        {
                            string sColumnName = CommonUtil.CommonLib.Util.NullToSpace(dr["NAME"]);
                            string sColumnDisplay = CommonUtil.CommonLib.Util.NullToSpace(dr["DISPLAYNAME"]);

                            if (!mdicProperty.ContainsKey(sColumnName))
                                mdicProperty.Add(sColumnName, sColumnDisplay);
                        }

                        //List<SPPID_DataColumn> lstDataColumn = db.ReadDataColumn(spProject.ProjectID, eListType.MappingSPPID);
                        //foreach (var item in lstDataColumn)
                        //{
                        //    if (!mdicProperty.ContainsKey(item.ColumnName))
                        //        mdicProperty.Add(item.ColumnName, item.ColumnDisplay);
                        //}

                        dtDataFilter.Clear();

                        Property.SetAddProperty(spProject.ProjectNo, ref mdicProperty);
                    }
                }
                else if (sToolType.Equals(ToolType.SP3D))
                {
                    // DB 화 시킬수 있으면 DB화 시킬것

                    string sError = string.Empty;

                    Information mInformation = new Information();
                    mdicProperty = proxy.GetSP3DAttribute(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, eListType.ListTypeID);
                }
                //else if (sToolType.Equals(ToolType.SPI))
                //{
                //    string sError = string.Empty;
                //    // SPI는 속성 조합으로 Tag를 구성할수 없음
                //    SPPID.WF.DSL.Information mInformation = new SPPID.WF.DSL.Information();
                //    SPPID.WF.DSL.OracleDac dbOracle = new SPPID.WF.DSL.OracleDac(mInformation);

                //    mdicProperty = dbOracle.GetTagColumnData_SPI(lProjectMasterID, ref sError);
                //}
            }
        }

        private void setSpread()
        {
            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.Columns.Count; i++)
            {
                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                arItems = null;
                arItemsData = null;

                switch (i)
                {
                    case (int)LTColumn.LTSelect:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadTagFormat.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.LTProperty:
                        //arItems = m_arProperty;
                        arItems = new string[mdicProperty.Count];
                        arItemsData = new string[mdicProperty.Count];
                        int iCellCount = 0;

                        if (mToolType == ToolType.SP3D && "PipeRun" == mListType.MappingSP3D)
                        {
                            mdicProperty["NPD"] = "Nominal Diameter";
                            mdicProperty["DESIGN_PRESS"] = "DESIGN PRESSURE";
                            mdicProperty["OPER_PRESS"] = "OPERERATING PRESSURE";
                            mdicProperty["TEST_PRESS"] = "TEST PRESSURE";
                            mdicProperty["DESIGN_TEMP"] = "DESIGN TEMPERATURE";
                            mdicProperty["OPER_TEMP"] = "OPER TEMPERATURE";
                            mdicProperty["INSUL_CODE"] = "INSULATION CODE";
                            mdicProperty["INSUL_THK"] = "INSULATION THICKNESS";
                        }

                        foreach (KeyValuePair<string, string> pair in mdicProperty.OrderBy(w => w.Value))
                        {
                            arItems[iCellCount] = pair.Value;
                            arItemsData[iCellCount] = pair.Key;
                            iCellCount++;
                        }
                        cbtColumn.MaxDrop = 18;
                        break;
                    case (int)LTColumn.LTAllowNull:
                        arItems = null;
                        arItemsData = null;
                        CheckBoxCellType ctCheckBoxs = new CheckBoxCellType();
                        fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = ctCheckBoxs;
                        fpSpreadTagFormat.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)LTColumn.LTSeparator:
                        arItems = new string[] { "", "-", "\"-", "=", "/", "|" };
                        arItemsData = new string[] { "", "-", "\"-", "=", "/", "|" };
                        break;
                    case (int)LTColumn.LTLength:
                        arItems = new string[] { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
                        arItemsData = new string[] { "", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
                        break;
                    case (int)LTColumn.LTAlignment:
                        arItems = new string[] { "", "Left", "Right" };
                        arItemsData = new string[] { "", "Left", "Right" };
                        break;
                    case (int)LTColumn.LTFillString:
                        arItems = new string[] { "", "' '", "0", "*" };
                        arItemsData = new string[] { "", "' '", "0", "*" };
                        break;
                }

                if (arItems != null)
                {
                    cbtColumn.Items = arItems;
                    cbtColumn.ItemData = arItemsData;
                    cbtColumn.EditorValue = EditorValue.ItemData;
                    fpSpreadTagFormat.ActiveSheet.Columns[i].CellType = cbtColumn;
                }
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // 데이터 조회
                List<EntityCommon_TagRule> lstTagRule = proxy.ReadCommon_TagRuleList(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mListType.ListTypeID, mToolType);
                int iRowCount = 0;
                foreach (EntityCommon_TagRule tag in lstTagRule)
                {
                    iRowCount = fpSpreadTagFormat.ActiveSheet.Rows.Count++;

                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTProperty].Value = tag.Name;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTProperty].Text = tag.NameView;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTAllowNull].Value = tag.AllowNullFlag;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTSeparator].Text = tag.Delimiter;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTLength].Text = tag.Length;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTFillString].Text = tag.FillString;
                    fpSpreadTagFormat.ActiveSheet.Cells[iRowCount, (int)LTColumn.LTAlignment].Text = tag.Alignment;
                }

                SetTagFormat();
            }


        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadTagFormat.ActiveSheet.Rows.Count++;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            for (int i = fpSpreadTagFormat.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSelect].Text;
                if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadTagFormat.ActiveSheet.Rows.Remove(i, 1);
                }
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagFormat.ActiveSheet;

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

            SetTagFormat();
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadTagFormat.ActiveSheet;
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

            SetTagFormat();
        }

        private void fpSpreadTagFormat_EditChange(object sender, EditorNotifyEventArgs e)
        {
            SetTagFormat();
        }

        private void SetTagFormat()
        {
            textBoxTagFormat.Text = "";

            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.RowCount; i++)
            {
                if (!string.IsNullOrEmpty(fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text))
                    textBoxTagFormat.Text += string.Format(" {0} [{1}]", fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text, fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text);
                else
                    textBoxTagFormat.Text += string.Format(" [{0}]", fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text);
            }
        }

        public bool SaveTagRule(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;

            DataTable dt = new DataTable("Table1");
            dt.Columns.Add("TagRuleID", typeof(long));
            dt.Columns.Add("ProjectMasterID", typeof(long));
            dt.Columns.Add("ListTypeID", typeof(int));
            dt.Columns.Add("ToolType");
            dt.Columns.Add("Name");
            dt.Columns.Add("NameView");
            dt.Columns.Add("AllowNullFlag", typeof(bool));
            dt.Columns.Add("Delimiter");
            dt.Columns.Add("Length");
            dt.Columns.Add("FillString");
            dt.Columns.Add("Alignment");
            dt.Columns.Add("SortOrder", typeof(int));
            dt.Columns.Add("UseFlag", typeof(bool));

            bool bSaveFlag = true;

            int iSortOrder = 1;
            for (int i = 0; i < fpSpreadTagFormat.ActiveSheet.RowCount; i++)
            {
                DataRow drNew = dt.NewRow();

                string sProperty = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Text;

                if (string.IsNullOrEmpty(sProperty))
                {
                    bSaveFlag = false;
                    break;
                }

                drNew["ProjectMasterID"] = mProjectMasterID;
                drNew["ListTypeID"] = mListType.ListTypeID;
                drNew["ToolType"] = mToolType;
                drNew["Name"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTProperty].Value.ToString();
                drNew["NameView"] = sProperty;

                if (fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTAllowNull].Text.ToUpper().Equals("TRUE"))
                    drNew["AllowNullFlag"] = true;
                else
                    drNew["AllowNullFlag"] = false;

                drNew["Delimiter"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTSeparator].Text;
                drNew["Length"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTLength].Text;
                drNew["FillString"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTFillString].Text;
                drNew["Alignment"] = fpSpreadTagFormat.ActiveSheet.Cells[i, (int)LTColumn.LTAlignment].Text;
                drNew["SortOrder"] = iSortOrder;
                drNew["UseFlag"] = true;

                dt.Rows.Add(drNew);
                iSortOrder++;
            }

            if (bSaveFlag)
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    bResult = proxy.SaveCommon_TagRule(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mProjectMasterID, mListType.ListTypeID, mToolType, dt, ref sMessage);
                }

            }
            else
            {
                sMessage = string.Format("{0} : 'Property' is empty", mToolType);
            }

            return bResult;
        }

        private void radButtonSave_Click(object sender, EventArgs e)
        {
            try
            {
                string sMessage = string.Empty;

                bool bResult = SaveTagRule(ref sMessage);

                if (!string.IsNullOrEmpty(sMessage))
                    MessageBox.Show(sMessage);
                else
                    MessageBox.Show(this, Resources.Message10000, this.Text);
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
