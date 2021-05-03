using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.DSL;
using System.IO;
using Configuration.Properties;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using Telerik.WinControls.UI;
using EDCS.WF.BSL;
using EDCS.WF.DSL.Entities;
using CommonLibrary;

namespace Configuration
{
    public partial class UserControlSPPIDInstrumentCompareItemSetting : UserControlBase
    {
        public Information mInformation = null;

        private enum menColumn { Check, InstrumentCompareID, FullPath, Path, SymbolName, Image, Count, Last };
        private string[] marColumnName = { "Check", "InstrumentCompareID", "FullPath", "Path", "Symbol Name", "Image", "Count", };
        private int[] marColumnWidth = { 90, 0, 0, 300, 220, 90, 90, };
        private DataTable mdtSymbol = new DataTable("Table1");
        private DataTable mdtPipeComp = new DataTable("Table1");
        private List<EntitySPPID_InstrumentCompareItem> mlstInstrumentCompareItem = new List<EntitySPPID_InstrumentCompareItem>();

        private List<string> mlstCompare_Deny = new List<string>();

        public UserControlSPPIDInstrumentCompareItemSetting()
        {
            InitializeComponent();
        }

        private List<string> SetDefaultCompareItem_Deny()
        {
            // 요청
            //1. Tag 가 없는 Symbol
            //2. Function
            //3. System Function 

            List<string> lstSymbol = new List<string>();

            lstSymbol.Add(@"Instrumentation\Gaps");
            lstSymbol.Add(@"Instrumentation\Instrument OPC's");
            lstSymbol.Add(@"Instrumentation\Labels - General Instrument");
            lstSymbol.Add(@"Instrumentation\Labels - In Line Instruments");
            lstSymbol.Add(@"Instrumentation\Labels - Off Line Instruments");
            lstSymbol.Add(@"Instrumentation\Off-Line\Functions");
            lstSymbol.Add(@"Instrumentation\Signal Line");
            lstSymbol.Add(@"Instrumentation\System Functions\Computer Functions");
            lstSymbol.Add(@"Instrumentation\System Functions\D C S");
            lstSymbol.Add(@"Instrumentation\System Functions\Interlocks");
            lstSymbol.Add(@"Instrumentation\System Functions\P L C");

            return lstSymbol;
        }

        public override void LoadData()
        {
            mlstCompare_Deny = SetDefaultCompareItem_Deny();

            this.radDropDownListProjectList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);

            InitDropdownListProject();
            SetDropdownListPlantLIst();

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
        }

        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetDropdownListPlantLIst();
        }

        private void SetDropdownListPlantLIst()
        {
            mInformation = new Information();

            string sError = string.Empty;

            mInformation.Project = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

            mInformation.Plant = null;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, mInformation.Project, GeneralUtil.InitConnection(mInformation));
                foreach (InfoPlant data in lstPlant)
                {
                    mInformation.Plant = data;
                    mInformation.InfoDB = new InfoDB();
                    mInformation.InfoDB.DBPath = Path.Combine(Paths.PATH_DATA, mInformation.Project.ProjectNo, mInformation.Plant.SiteName);
                    break;
                }

                InitControl();

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
            }
        }

        private void radBrowseEditorBackupPath_ValueChanged(object sender, EventArgs e)
        {
            if (Directory.Exists(radBrowseEditorBackupPath.Value))
            {
                if (mInformation.Plant.SiteSymbolFolderPath.Contains(radBrowseEditorBackupPath.Value))
                {
                    InitControl();
                }
                else
                {
                    MessageBox.Show(string.Format(Resources.Message10011, "Symbol root"));
                }
            }
        }

        private void InitControl()
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Loading data.  Please wait!";

            try
            {
                string sPath = GetInstrumentSymbolPath(mInformation.Plant.SiteSymbolFolderPath);
                radBrowseEditorBackupPath.Value = sPath;

                InitSpread();
                SetSpread(sPath);
                FrmProgressStatus.FormClose();
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
            }
        }

        private void InitSpread()
        {
            fpSpreadData.Reset();

            fpSpreadData.RowSplitBoxPolicy = SplitBoxPolicy.Never;
            fpSpreadData.ColumnSplitBoxPolicy = SplitBoxPolicy.Never;
            fpSpreadData.VerticalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            fpSpreadData.HorizontalScrollBarPolicy = ScrollBarPolicy.AsNeeded;

            fpSpreadData.ActiveSheet.ColumnCount = (int)menColumn.Last;
            fpSpreadData.ActiveSheet.RowCount = 0;

            for (int i = 0; i < (int)menColumn.Last; i++)
            {
                Column col = fpSpreadData.ActiveSheet.Columns[i];
                col.Label = marColumnName[i];
                col.Width = marColumnWidth[i];

                if (col.Label == marColumnName[(int)menColumn.Check])
                {
                    col.HorizontalAlignment = CellHorizontalAlignment.Center;
                    col.CellType = getCheckCellType();
                }
                else
                {
                    TextCellType ctText = new TextCellType();
                    col.CellType = ctText;
                }
            }

            fpSpreadData.ActiveSheet.GrayAreaBackColor = Color.Transparent;
        }

        private void SetSpread(string sPath)
        {
            fpSpreadData.RowSplitBoxPolicy = SplitBoxPolicy.Never;
            fpSpreadData.ColumnSplitBoxPolicy = SplitBoxPolicy.Never;
            fpSpreadData.VerticalScrollBarPolicy = ScrollBarPolicy.AsNeeded;
            fpSpreadData.HorizontalScrollBarPolicy = ScrollBarPolicy.AsNeeded;

            SheetView sheetView = fpSpreadData.ActiveSheet;
            sheetView.RowCount = 0;

            if (mInformation != null && mInformation.Project != null && mInformation.Plant != null)
            {
                if (mdtPipeComp != null)
                    mdtPipeComp.Clear();

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    mdtPipeComp = proxy.GetItemTypeData(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information.INSTRUMENT, mInformation.InfoDB.DBPath, false);

                    mdtSymbol = null;

                    mlstInstrumentCompareItem = proxy.ReadSPPID_InstrumentCompareItem(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID);

                    SetSymbolTable(mInformation.Plant.SiteSymbolFolderPath, sPath, mlstInstrumentCompareItem, mdtPipeComp);
                    sheetView.DataSource = mdtSymbol;
                    for (int i = 0; i < (int)menColumn.Last; i++)
                    {
                        Column col = fpSpreadData.ActiveSheet.Columns[i];
                        col.Label = marColumnName[i];
                        col.Width = marColumnWidth[i];

                        if (col.Label == marColumnName[(int)menColumn.Check])
                        {
                            col.HorizontalAlignment = CellHorizontalAlignment.Center;
                            col.CellType = getCheckCellType();
                        }
                        else if (col.Label == marColumnName[(int)menColumn.Image])
                        {
                            col.HorizontalAlignment = CellHorizontalAlignment.Center;
                            col.CellType = new ImageCellType();
                        }
                        else
                        {
                            TextCellType ctText = new TextCellType();
                            col.CellType = ctText;
                        }
                    }

                    if (mlstInstrumentCompareItem.Count == 0)
                    {
                        // 세팅 정보가 없으면 Default 세팅으로 설정
                        SetDefaultInstrumentCompareSetting();
                    }
                }

                SpreadUtil.SetColumnWidth(sheetView, marColumnWidth);
                SpreadUtil.SetColumnName(sheetView, marColumnName);
                SpreadUtil.SetAllColumnAutoFilterAndSort(sheetView);
            }
        }



        private void SetSymbolTable(string sSymbolFolderRootPath, string directoryPath, List<EntitySPPID_InstrumentCompareItem> lstSPPID_InstrumentCompareItem, DataTable dtInstrument)
        {
            DirectoryInfo dInfo = new DirectoryInfo(directoryPath);
            DirectoryInfo[] dInfoList = dInfo.GetDirectories();
            FileInfo[] fInfoList = dInfo.GetFiles();

            if (mdtSymbol == null)
            {
                mdtSymbol = new DataTable("Table1");
                mdtSymbol.Columns.Add(menColumn.Check.ToString(), typeof(bool));
                mdtSymbol.Columns.Add(menColumn.InstrumentCompareID.ToString());
                mdtSymbol.Columns.Add(menColumn.FullPath.ToString());
                mdtSymbol.Columns.Add(menColumn.Path.ToString());
                mdtSymbol.Columns.Add(menColumn.SymbolName.ToString());
                mdtSymbol.Columns.Add(menColumn.Image.ToString(), typeof(Image));
                mdtSymbol.Columns.Add(menColumn.Count.ToString());
            }

            foreach (FileInfo fInfo in fInfoList)
            {
                if (fInfo.Extension.ToUpper().Equals(".SYM"))
                {
                    DataRow dRow = mdtSymbol.NewRow();
                    string sDirectoryPath = string.Empty;
                    string sFullPath = fInfo.FullName.Replace(mInformation.Plant.SiteSymbolFolderPath, string.Empty);

                    sDirectoryPath = fInfo.DirectoryName.Replace(sSymbolFolderRootPath + "\\", "").Replace(sSymbolFolderRootPath, "");

                    var spSetting = lstSPPID_InstrumentCompareItem.Where(w => w.SymbolPath == sFullPath).FirstOrDefault();

                    DataRow[] drComp = dtInstrument.Select(string.Format("Representation.FileName = '{0}'", sFullPath.Replace("'", "''")));

                    int iCompCount = 0;
                    if (drComp != null && drComp.Length > 0)
                    {
                        iCompCount = drComp.Length;
                    }

                    bool bCheck = false;

                    if (spSetting != null)
                    {
                        bCheck = spSetting.FiledFlag;
                    }

                    dRow[menColumn.InstrumentCompareID.ToString()] = "";
                    dRow[menColumn.FullPath.ToString()] = sFullPath;
                    dRow[menColumn.Path.ToString()] = sDirectoryPath;
                    dRow[menColumn.SymbolName.ToString()] = fInfo.Name;

                    if (File.Exists(fInfo.FullName))
                    {
                        try
                        {
                            dRow[menColumn.Image.ToString()] = HelperFileIcon.GetLargeIcon(fInfo.FullName).ToBitmap();
                        }
                        catch (Exception)
                        {
                            dRow[menColumn.Image.ToString()] = null;
                        }
                    }


                    dRow[menColumn.Count.ToString()] = iCompCount.ToString();
                    dRow[menColumn.Check.ToString()] = bCheck;

                    mdtSymbol.Rows.Add(dRow);
                }
            }

            foreach (DirectoryInfo dInfo1 in dInfoList)
            {
                SetSymbolTable(sSymbolFolderRootPath, dInfo1.FullName, lstSPPID_InstrumentCompareItem, dtInstrument);
            }
        }

        private string GetInstrumentSymbolPath(string sSymbolFolderRootPath)
        {
            string sPath = sSymbolFolderRootPath;

            DirectoryInfo dInfo = new DirectoryInfo(sSymbolFolderRootPath);
            DirectoryInfo[] dInfoList = dInfo.GetDirectories();

            foreach (DirectoryInfo dInfo1 in dInfoList)
            {
                if (dInfo1.Name.ToUpper() == "INSTRUMENTATION")
                {
                    sPath = dInfo1.FullName;
                    break;
                }
            }

            return sPath;
        }

        private CheckBoxCellType getCheckCellType()
        {
            CheckBoxCellType cbbCheck = new CheckBoxCellType();
            return cbbCheck;
        }

        public override void SaveData()
        {
            string sMessage = string.Empty;
            SaveInstrumentSetting(ref sMessage);
            MessageBox.Show(sMessage);
        }

        public void SaveInstrumentSetting(ref string sMessage)
        {
            sMessage = string.Empty;

            if (mInformation != null && mInformation.Project != null && mInformation.Plant != null)
            {
                DataTable dt = new DataTable("Table1");

                dt.Columns.Add("InstrumentCompareID", typeof(long));
                dt.Columns.Add("ProjectID", typeof(long));
                dt.Columns.Add("SymbolPath");
                dt.Columns.Add("SymbolName");
                dt.Columns.Add("FiledFlag", typeof(bool));

                for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
                {
                    string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Check].Text;

                    DataRow drNew = dt.NewRow();
                    drNew["ProjectID"] = mInformation.Project.ProjectID;
                    drNew["SymbolPath"] = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.FullPath].Text;
                    drNew["SymbolName"] = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SymbolName].Text;

                    if (sCheck.ToUpper() == "TRUE")
                        drNew["FiledFlag"] = true;
                    else
                        drNew["FiledFlag"] = false;

                    dt.Rows.Add(drNew);
                }

                if (dt.Rows.Count > 0)
                {
                    // 저장
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        proxy.SaveSPPID_InstrumentCompareItem(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, dt, ref sMessage);
                    }
                }
                else
                {
                    sMessage = "Instrument symbol check";
                }
            }
        }

        private void SetDefaultInstrumentCompareSetting()
        {
            DataTable dt = new DataTable("Table1");

            dt.Columns.Add("InstrumentCompareID", typeof(long));
            dt.Columns.Add("ProjectID", typeof(long));
            dt.Columns.Add("SymbolPath");
            dt.Columns.Add("SymbolName");
            dt.Columns.Add("FiledFlag", typeof(bool));

            for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
            {
                string sFullPath = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.FullPath].Text;

                if (mlstCompare_Deny.Where(w => sFullPath.Contains(w)).Count() > 0)
                {
                    fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Check].Value = false;
                }
                else
                {
                    fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Check].Value = true;
                }

                string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Check].Text;

                // True인 항목만 저장
                DataRow drNew = dt.NewRow();
                drNew["ProjectID"] = mInformation.Project.ProjectID;
                drNew["SymbolPath"] = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.FullPath].Text;
                drNew["SymbolName"] = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SymbolName].Text;

                if (sCheck.ToUpper() == "TRUE")
                    drNew["FiledFlag"] = true;
                else
                    drNew["FiledFlag"] = false;

                dt.Rows.Add(drNew);
            }
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // 저장
                if (dt.Rows.Count > 0)
                {
                    string sMessage = string.Empty;
                    // 저장


                    proxy.SaveSPPID_InstrumentCompareItem(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, dt, ref sMessage);
                }

                mlstInstrumentCompareItem = proxy.ReadSPPID_InstrumentCompareItem(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID);
            }

        }

        private void radButtonCheck_Click(object sender, EventArgs e)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Default Setting.  Please wait!";
            try
            {
                SetDefaultInstrumentCompareSetting();
                FrmProgressStatus.FormClose();
            }
            catch (Exception ex)
            {
                FrmProgressStatus.FormClose();
            }
        }

    }
}
