using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using CommonLibrary;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;

using Telerik.WinControls.UI;
using Configuration.Properties;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlSPPIDPipingCompSetting : UserControlBase
    {
        public Information mInformation = null;

        private enum menColumn { PipingCompSettingID, FullPath, Path, SymbolName, Image, Count, Valve, Specialty, Last };
        private string[] marColumnName = { "PipingCompSettingID", "FullPath", "Path", "Symbol Name", "Image", "Count", "Valve", "Specialty" };
        private int[] marColumnWidth = { 0, 0, 300, 220, 90, 90, 90, 90 };
        private DataTable mdtSymbol = new DataTable("Table1");
        private DataTable mdtPipeComp = new DataTable("Table1");
        private string mRootPath = string.Empty;

        public UserControlSPPIDPipingCompSetting()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            this.radDropDownListProjectList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListPlantList.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListPlantList_SelectedIndexChanged);
            this.radBrowseEditorBackupPath.ValueChanged -= new System.EventHandler(this.radBrowseEditorBackupPath_ValueChanged);

            InitDropdownListProject();
            SetDropdownListPlantLIst();

            this.radDropDownListProjectList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListProjectList_SelectedIndexChanged);
            this.radDropDownListPlantList.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(this.radDropDownListPlantList_SelectedIndexChanged);

            InitControl(string.Empty);

            this.radBrowseEditorBackupPath.ValueChanged += new System.EventHandler(this.radBrowseEditorBackupPath_ValueChanged);
        }

        private void InitDropdownListProject()
        {
            GeneralUtil.SetRadDropDownListProjectNo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);
        }

        private void SetDropdownListPlantLIst()
        {
            mInformation = new Information();

            string sError = string.Empty;

            mInformation.Project = (EntitySPTool_Project)radDropDownListProjectList.SelectedItem.DataBoundItem;

            mInformation.Plant = null;
            radDropDownListPlantList.Items.Clear();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<InfoPlant> lstPlant = proxy.GetPlantNames(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation, mInformation.Project, GeneralUtil.InitConnection(mInformation));
                foreach (InfoPlant data in lstPlant)
                {
                    RadListDataItem item = new RadListDataItem(data.SiteName);
                    item.Tag = data;
                    radDropDownListPlantList.Items.Add(item);
                }

                if (radDropDownListPlantList.Items.Count > 0)
                {
                    radDropDownListPlantList.Items[0].Selected = true;
                    mInformation.Plant = (InfoPlant)radDropDownListPlantList.SelectedItem.Tag;
                    if (mInformation.Plant != null)
                    {
                        mInformation.InfoDB = new InfoDB();
                        mInformation.InfoDB.DBPath = Path.Combine(Paths.PATH_DATA, mInformation.Project.ProjectNo, mInformation.Plant.SiteName);
                    }
                }

                GeneralUtil.SaveRadDropDownListProjectNo(radDropDownListProjectList, this._entityEDBInformation, ToolType.SPPID);

            }
        }

        private void radDropDownListProjectList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            SetDropdownListPlantLIst();
        }

        private void InitControl(string sRootPath)
        {
            FrmProgressStatus.FormShow();
            FrmProgressStatus.SetMarquee();
            FrmProgressStatus.ToolTip = EDCS.WF.BSL.Variable.SharedVariable.ToolTip;
            FrmProgressStatus.ProgressPer = "Loading data.  Please wait!";

            InitSpread();
            SetSpread(sRootPath);

            FrmProgressStatus.FormClose();
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

                if (col.Label == marColumnName[(int)menColumn.Specialty] || col.Label == marColumnName[(int)menColumn.Valve])
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

        private CheckBoxCellType getCheckCellType()
        {
            CheckBoxCellType cbbCheck = new CheckBoxCellType();
            return cbbCheck;
        }

        private void SetSpread(string sRootPath)
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
                    mdtPipeComp = proxy.GetItemTypeData(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Information.PIPINGCOMP, mInformation.InfoDB.DBPath, false);

                    string sPath = string.Empty;
                    if (string.IsNullOrEmpty(sRootPath))
                    {
                        List<EntitySPPID_PipingCompRootPathSetting> lstPipingCompRootPathSetting = proxy.ReadSPPID_PipingCompRootPathSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, mInformation.Plant.SiteName);

                        sPath = GetPipingSymbolPath(mInformation.Plant.SiteSymbolFolderPath, mInformation.Plant.SiteSymbolFolderPath, lstPipingCompRootPathSetting);

                        if (string.IsNullOrEmpty(sPath))
                            sPath = mInformation.Plant.SiteSymbolFolderPath;

                        radBrowseEditorBackupPath.Value = sPath;
                    }
                    else
                    {
                        sPath = sRootPath;
                    }

                    mRootPath = sPath;
                    mdtSymbol = null;

                    List<EntitySPPID_PipingCompSetting> lstPipingCompSetting = proxy.ReadSPPID_PipingCompSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, mInformation.Plant.SiteName);

                    SetSymbolTable(sPath, sPath, lstPipingCompSetting, mdtPipeComp);

                    sheetView.DataSource = mdtSymbol;

                    for (int i = 0; i < (int)menColumn.Last; i++)
                    {
                        Column col = fpSpreadData.ActiveSheet.Columns[i];
                        col.Label = marColumnName[i];
                        col.Width = marColumnWidth[i];

                        if (col.Label == marColumnName[(int)menColumn.Specialty] || col.Label == marColumnName[(int)menColumn.Valve])
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
                }

                SpreadUtil.SetColumnWidth(sheetView, marColumnWidth);
                SpreadUtil.SetColumnName(sheetView, marColumnName);
                SpreadUtil.SetAllColumnAutoFilterAndSort(sheetView);
            }
        }

        private string GetPipingSymbolPath(string sSymbolFolderRootPath, string directoryPath, List<EntitySPPID_PipingCompRootPathSetting> lstPipingCompRootPathSetting)
        {
            string sPath = string.Empty;

            if (lstPipingCompRootPathSetting != null && lstPipingCompRootPathSetting.Count > 0)
            {
                foreach (EntitySPPID_PipingCompRootPathSetting sRoot in lstPipingCompRootPathSetting)
                {
                    sPath = sRoot.RootPath;
                    break;
                }

                return sPath;
            }

            DirectoryInfo dInfo = new DirectoryInfo(directoryPath);
            DirectoryInfo[] dInfoList = dInfo.GetDirectories();

            foreach (DirectoryInfo dInfo1 in dInfoList)
            {
                if (dInfo1.Name.ToUpper() == "PIPING")
                {
                    sPath = dInfo1.FullName;
                    break;
                }
                else
                {
                    GetPipingSymbolPath(sSymbolFolderRootPath, dInfo1.FullName, lstPipingCompRootPathSetting);
                }
            }

            return sPath;
        }

        private void SetSymbolTable(string sSymbolFolderRootPath, string directoryPath, List<EntitySPPID_PipingCompSetting> lstPipingCompSetting, DataTable dtPipeComp)
        {
            DirectoryInfo dInfo = new DirectoryInfo(directoryPath);
            DirectoryInfo[] dInfoList = dInfo.GetDirectories();
            FileInfo[] fInfoList = dInfo.GetFiles();

            if (mdtSymbol == null)
            {
                mdtSymbol = new DataTable("Table1");
                mdtSymbol.Columns.Add(menColumn.PipingCompSettingID.ToString());
                mdtSymbol.Columns.Add(menColumn.FullPath.ToString());
                mdtSymbol.Columns.Add(menColumn.Path.ToString());
                mdtSymbol.Columns.Add(menColumn.SymbolName.ToString());
                mdtSymbol.Columns.Add(menColumn.Image.ToString(), typeof(Image));
                mdtSymbol.Columns.Add(menColumn.Count.ToString());
                mdtSymbol.Columns.Add(menColumn.Valve.ToString(), typeof(bool));
                mdtSymbol.Columns.Add(menColumn.Specialty.ToString(), typeof(bool));
            }

            foreach (FileInfo fInfo in fInfoList)
            {
                if (fInfo.Extension.ToUpper().Equals(".SYM"))
                {
                    DataRow dRow = mdtSymbol.NewRow();
                    string sDirectoryPath = string.Empty;
                    string sFullPath = fInfo.FullName.Replace(mInformation.Plant.SiteSymbolFolderPath, string.Empty);

                    sDirectoryPath = fInfo.DirectoryName.Replace(sSymbolFolderRootPath + "\\", "").Replace(sSymbolFolderRootPath, "");

                    var spSetting = lstPipingCompSetting.Where(w => w.FullPath == sFullPath).FirstOrDefault();

                    DataRow[] drComp = dtPipeComp.Select(string.Format("Representation.FileName = '{0}'", sFullPath.Replace("'", "''")));

                    int iCompCount = 0;
                    if (drComp != null && drComp.Length > 0)
                    {
                        iCompCount = drComp.Length;
                    }

                    bool bValve = false;
                    bool bSpecialty = false;

                    if (spSetting != null)
                    {
                        bValve = spSetting.ValveFlag;
                        bSpecialty = spSetting.SpecialtyFlag;
                    }

                    dRow[menColumn.PipingCompSettingID.ToString()] = "";
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
                    dRow[menColumn.Valve.ToString()] = bValve;
                    dRow[menColumn.Specialty.ToString()] = bSpecialty;

                    mdtSymbol.Rows.Add(dRow);
                }
            }

            foreach (DirectoryInfo dInfo1 in dInfoList)
            {
                SetSymbolTable(sSymbolFolderRootPath, dInfo1.FullName, lstPipingCompSetting, dtPipeComp);
            }
        }

        private void radDropDownListPlantList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this.radBrowseEditorBackupPath.ValueChanged -= new EventHandler(this.radBrowseEditorBackupPath_ValueChanged);

            if (radDropDownListPlantList.SelectedItem != null)
            {
                mInformation.Plant = (InfoPlant)radDropDownListPlantList.SelectedItem.Tag;
                if (mInformation.Plant != null)
                {
                    mInformation.InfoDB = new InfoDB();
                    mInformation.InfoDB.DBPath = Path.Combine(Paths.PATH_DATA, mInformation.Project.ProjectNo, mInformation.Plant.SiteName);
                }

                InitControl(string.Empty);
            }

            this.radBrowseEditorBackupPath.ValueChanged += new EventHandler(this.radBrowseEditorBackupPath_ValueChanged);
        }

        public override void SaveData()
        {
            string sMessage = string.Empty;
            SavePipingCompSetting(ref sMessage);
            MessageBox.Show(sMessage);
        }

        public void SavePipingCompSetting(ref string sMessage)
        {
            sMessage = string.Empty;

            if (mInformation != null && mInformation.Project != null && mInformation.Plant != null)
            {
                DataTable dt = new DataTable("Table1");

                dt.Columns.Add("PipingCompSettingID", typeof(long));
                dt.Columns.Add("ProjectID", typeof(long));
                dt.Columns.Add("PlantName");
                dt.Columns.Add("FullPath");
                dt.Columns.Add("ValveFlag", typeof(bool));
                dt.Columns.Add("SpecialtyFlag", typeof(bool));

                for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
                {
                    string sValve = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Valve].Text;
                    string sSpecialty = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Specialty].Text;

                    if (sValve.ToUpper().Equals("FALSE") && sSpecialty.ToUpper().Equals("FALSE"))
                        continue;

                    string sPath = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.FullPath].Text;

                    DataRow drNew = dt.NewRow();
                    drNew["ProjectID"] = mInformation.Project.ProjectID;
                    drNew["PlantName"] = mInformation.Plant.SiteName;
                    drNew["FullPath"] = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.FullPath].Text;

                    if (sValve.ToUpper() == "TRUE")
                        drNew["ValveFlag"] = true;
                    else
                        drNew["ValveFlag"] = false;

                    if (sSpecialty.ToUpper() == "TRUE")
                        drNew["SpecialtyFlag"] = true;
                    else
                        drNew["SpecialtyFlag"] = false;

                    dt.Rows.Add(drNew);
                }

                if (dt.Rows.Count > 0)
                {
                    // 저장
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        proxy.SaveSPPID_PipingCompSetting(EDCS.WF.BSL.Variable.SharedVariable.IsIE, mInformation.Project.ProjectID, mInformation.Plant.SiteName, mRootPath, dt, ref sMessage);
                    }
                }
                else
                {
                    sMessage = "Item check Valve or Specialty";
                }
            }
        }

        private void radBrowseEditorBackupPath_Click(object sender, EventArgs e)
        {
            if (Directory.Exists(radBrowseEditorBackupPath.Value))
            {
                FolderBrowserDialog dialog = (FolderBrowserDialog)radBrowseEditorBackupPath.Dialog;
                dialog.SelectedPath = radBrowseEditorBackupPath.Value;
            }
        }

        private void radBrowseEditorBackupPath_ValueChanged(object sender, EventArgs e)
        {
            if (Directory.Exists(radBrowseEditorBackupPath.Value))
            {
                if (mInformation.Plant.SiteSymbolFolderPath.Contains(radBrowseEditorBackupPath.Value))
                {
                    InitControl(radBrowseEditorBackupPath.Value);
                }
                else
                {
                    MessageBox.Show(string.Format(Resources.Message10011, "Symbol root"));
                }
            }
        }

        private void radButtonCheck_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
            {
                string sPath = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Path].Text;

                if (sPath.Contains("Specialty"))
                {
                     fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Specialty].Value = true;
                }

                if (sPath.Contains("Valves"))
                {
                    fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Valve].Value = true;
                }
            }
        }

    }
}
