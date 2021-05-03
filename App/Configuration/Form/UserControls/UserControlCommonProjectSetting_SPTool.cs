using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;

using Configuration.Properties;
using EDCS.WF.DSL.Entities;
using CommonLibrary.CL;

namespace Configuration
{
    public partial class UserControlCommonProjectSetting_SPTool : UserControl
    {


        private long mProjectMasterID = -1;
        private string mToolType = string.Empty;

        private enum menColumn { Select, ProjectID, ProjectMasterID, DefaultProjectFlag, ProjectNo, ProjectName, SPToolType, DBHost, DBPort, DBService, DBSID, DBUserID, DBUserPW, DBVersion, DBSchema, IniPath, SPPIDVersion, BorderSize, SeedDate, Description, SPIUserID, SPIUserPW, SPIVersion, PDSType, IdealOutputPath, SPIPBSDefaultUseFlag, Status, DeleteFlag, Last };
        private string[] marColumnName = { " ", "ProjectID", "ProjectMasterID", "Default Project", "Project No", "Project Name", "Tool Type", "DB Host", "DB Port", "DB Service", "DB SID", "DB ID", "DB PW", "DB Version", "DB Schema", "IniPath", "SPPID Version", "Border Size", "Seed Date", "Description", "SPI UserID", "SPI UserPW", "SPI Version", "PDSType", "Ideal Output Path", "PBS Default", "Status", "Delete" };

        private int[] marColumnWidth_SPPID = { 40, 0, 0, 80, 80, 200, 0, 80, 60, 80, 0, 80, 80, 80, 0, 300, 80, 110, 80, 200, 0, 0, 0, 0, 0, 0, 0, 0 };

        private int[] marColumnWidth_NOT_SPPID = { 40, 0, 0, 80, 80, 200, 0, 80, 60, 120, 80, 80, 80, 80, 80, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0 };

        private int[] marColumnWidth_SPI = { 40, 0, 0, 80, 80, 200, 0, 80, 60, 120, 80, 80, 80, 80, 80, 300, 0, 0, 0, 200, 80, 80, 80, 0, 200, 100, 0, 0 };

        private int[] marColumnWidth_PDS = { 40, 0, 0, 80, 80, 200, 0, 80, 60, 120, 80, 80, 80, 80, 80, 0, 0, 0, 0, 100, 0, 0, 0, 100, 120, 0, 0, 0 };

        public UserControlCommonProjectSetting_SPTool(bool isIE, long lProjectMasterID, string sToolType)
        {
            InitializeComponent();

            mProjectMasterID = lProjectMasterID;
            mToolType = sToolType;

            labelTools.Text = sToolType;
            SetSpread(lProjectMasterID, sToolType);
        }

        private void InitSpread(string sToolType)
        {
            if (sToolType == ToolType.SPPID)
                SpreadUtil.InitSpread(fpSpreadData, marColumnWidth_SPPID, marColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            else if (sToolType == ToolType.SPI)
                SpreadUtil.InitSpread(fpSpreadData, marColumnWidth_SPI, marColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            else if (sToolType == ToolType.PDS)
                SpreadUtil.InitSpread(fpSpreadData, marColumnWidth_PDS, marColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            else
            {
                // SP3D , pdms
                SpreadUtil.InitSpread(fpSpreadData, marColumnWidth_NOT_SPPID, marColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            }

            fpSpreadData.ActiveSheet.ColumnHeader.Rows[0].Height = 30;

            // 컬럼 세팅
            string[] arItems = { };
            string[] arItemsData = { };

            for (int i = 0; i < fpSpreadData.ActiveSheet.Columns.Count; i++)
            {
                arItems = null;
                arItemsData = null;

                ComboBoxCellType cbtColumn = new ComboBoxCellType();

                switch (i)
                {
                    case (int)menColumn.Select:
                    case (int)menColumn.DefaultProjectFlag:
                    case (int)menColumn.DeleteFlag:
                    case (int)menColumn.SPIPBSDefaultUseFlag:
                        CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                        fpSpreadData.ActiveSheet.Columns[i].CellType = ctCheckBox;
                        fpSpreadData.ActiveSheet.Columns[i].HorizontalAlignment = CellHorizontalAlignment.Center;
                        break;
                    case (int)menColumn.SeedDate:
                        DateTimeCellType dateTimeCellType = new DateTimeCellType();
                        dateTimeCellType.DropDownButton = true;
                        fpSpreadData.ActiveSheet.Columns[i].CellType = dateTimeCellType;
                        break;
                    case (int)menColumn.BorderSize:
                        arItems = SPPIDToolInfo.BorderSize.ToArray();
                        arItemsData = SPPIDToolInfo.BorderSize.ToArray();
                        break;
                    case (int)menColumn.SPPIDVersion:
                        arItems = SPPIDVersion.Version.ToArray();
                        arItemsData = SPPIDVersion.Version.ToArray();
                        break;
                    case (int)menColumn.SPIVersion:
                        arItems = SPIVersion.Version.ToArray();
                        arItemsData = SPIVersion.Version.ToArray();
                        break;
                    case (int)menColumn.PDSType:
                        arItems = PDSType.GetItem().ToArray();
                        arItemsData = PDSType.GetItem().ToArray();
                        break;
                    default:
                        fpSpreadData.ActiveSheet.Columns[i].CellType = new TextCellType();
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
        }

        private void SetSpread(long lProjectMasterID, string sToolType)
        {
            InitSpread(sToolType);

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var lstProject = proxy.ReadSPToolProjectDetailInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectMasterID, sToolType).OrderByDescending(w => w.DefaultProjectFlag).ToList();

                EncryptDecryptQueryString eq = new EncryptDecryptQueryString();

                foreach (var spProject in lstProject)
                {
                    int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectID].Value = spProject.ProjectID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectMasterID].Value = lProjectMasterID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DefaultProjectFlag].Value = spProject.DefaultProjectFlag;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectNo].Value = spProject.ProjectNo;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectName].Value = spProject.ProjectName;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPToolType].Value = mToolType;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBHost].Value = spProject.DBHost;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBPort].Value = spProject.DBPort;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBService].Value = spProject.DBService;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBSID].Value = spProject.DBSID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBUserID].Value = spProject.DBUserID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBUserPW].Value = eq.Decrypt(spProject.DBUserPW);
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBVersion].Value = spProject.DBVersion;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DBSchema].Value = spProject.DBSchema;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.IniPath].Value = spProject.IniPath;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPPIDVersion].Value = spProject.SPPIDVersion;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.BorderSize].Value = spProject.BorderSize;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SeedDate].Value = spProject.SeedDate;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DeleteFlag].Value = spProject.DeleteFlag;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.Description].Value = spProject.Description;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.Status].Text = SpreadFlag.None;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPIUserID].Value = spProject.SPIUserID;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPIUserPW].Value = spProject.SPIUserPW;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPIVersion].Value = spProject.SPIVersion;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.PDSType].Value = spProject.PDSType;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.IdealOutputPath].Value = spProject.IdealOutputPath;
                    fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPIPBSDefaultUseFlag].Value = spProject.SPIPBSDefaultUseFlag;
                }
            }
        }

        private void fpSpreadData_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.Range.Row < 0)
                return;

            string strValue = string.Empty;
            OpenFileDialog ofd = null;

            if (e.Range.Column == (int)menColumn.IniPath)
            {
                ofd = new OpenFileDialog();
                if (fpSpreadData.ActiveSheet.Cells[e.Range.Row, e.Range.Column].Value != null)
                {
                    strValue = fpSpreadData.ActiveSheet.Cells[e.Range.Row, e.Range.Column].Value.ToString();
                }

                if (e.Range.Column == (int)menColumn.IniPath)
                {
                    ofd.Filter = "ini File(*.ini)|*.ini";
                }

                if (!string.IsNullOrEmpty(strValue))
                {
                    ofd.InitialDirectory = Path.GetDirectoryName(strValue);
                    ofd.FileName = Path.GetFileName(strValue);
                }

                if (ofd.ShowDialog(this) == DialogResult.OK)
                {
                    fpSpreadData.ActiveSheet.Cells[e.Range.Row, e.Range.Column].Value = ofd.FileName;

                    if (!fpSpreadData.ActiveSheet.Cells[e.Range.Row, (int)menColumn.Status].Text.Equals(SpreadFlag.Add))
                        fpSpreadData.ActiveSheet.Cells[e.Range.Row, (int)menColumn.Status].Text = SpreadFlag.Update;
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            if (fpSpreadData.ActiveSheet != null)
            {
                int iRow = fpSpreadData.ActiveSheet.Rows.Count++;

                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectID].Value = "-1";
                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.ProjectMasterID].Value = mProjectMasterID;
                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPToolType].Value = mToolType;

                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DefaultProjectFlag].Value = false;
                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.DeleteFlag].Value = false;
                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.SPIPBSDefaultUseFlag].Value = false;

                fpSpreadData.ActiveSheet.Cells[iRow, (int)menColumn.Status].Value = SpreadFlag.Add;

                fpSpreadData.ActiveSheet.SetActiveCell(fpSpreadData.ActiveSheet.Rows.Count - 1, (int)menColumn.ProjectNo, true);
                fpSpreadData.ShowRow(0, fpSpreadData.ActiveSheet.Rows.Count, FarPoint.Win.Spread.VerticalPosition.Bottom);

            }
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            for (int i = fpSpreadData.ActiveSheet.Rows.Count - 1; i >= 0; i--)
            {
                string sCheck = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Select].Text;
                string sStatus = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Status].Text;

                if (sCheck.ToUpper().Equals("TRUE") && sStatus.Equals(SpreadFlag.Add))
                {
                    fpSpreadData.ActiveSheet.Rows.Remove(i, 1);
                }
                else if (sCheck.ToUpper().Equals("TRUE"))
                {
                    fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DeleteFlag].Value = true;
                    fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Status].Text = SpreadFlag.Delete;
                }
            }

            SaveDetail(true);
        }

        private void radButtonAttributeSave_Click(object sender, EventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    SaveDetail(false);
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

        private void SaveDetail(bool bDeleteFlag)
        {
            if (fpSpreadData.ActiveSheet != null)
            {
                StringBuilder sbError = new StringBuilder();

                if (!bDeleteFlag)
                {
                    List<string> lstDefaultCount = new List<string>();
                    for (int i = 0; i < fpSpreadData.ActiveSheet.Rows.Count; i++)
                    {
                        string sDefaultProjectFlag = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DefaultProjectFlag].Text.ToString();
                        lstDefaultCount.Add(sDefaultProjectFlag);
                    }

                    int iCount = (from num in lstDefaultCount
                                  where num == "True"
                                  select num).Count();

                    if (iCount > 1)
                    {
                        MessageBox.Show(this, Resources.Message10002, this.Text);
                        return;
                    }

                    lstDefaultCount.Clear();
                }
                bool bUpdateFlag = false;

                EncryptDecryptQueryString cq = new EncryptDecryptQueryString();

                for (int i = 0; i < fpSpreadData.ActiveSheet.Rows.Count; i++)
                {
                    string sProjectID = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.ProjectID].Text.ToString();
                    string sProjectMasterID = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.ProjectMasterID].Value.ToString();
                    string sProjectNo = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.ProjectNo].Text.ToString();
                    string sProjectName = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.ProjectName].Text.ToString();
                    string sSPToolType = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPToolType].Text.ToString();
                    string sDBHost = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBHost].Text.ToString();
                    string sDBPort = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBPort].Text.ToString();
                    string sDBService = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBService].Text.ToString();
                    string sDBSID = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBSID].Text.ToString();
                    string sDBUserID = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBUserID].Text.ToString();
                    string sDBUserPW = cq.Encrypt(fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBUserPW].Text.ToString().Trim());
                    string sDBVersion = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBVersion].Text.ToString();
                    string sDBSchema = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DBSchema].Text.ToString();
                    string sIniPath = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.IniPath].Text.ToString();
                    string sSPPIDVersion = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPPIDVersion].Text.ToString();
                    string sBorderSize = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.BorderSize].Text.ToString();
                    string sSeedDate = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SeedDate].Text.ToString();
                    string sDeleteFlag = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DeleteFlag].Text.ToString();
                    string sDefaultProjectFlag = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.DefaultProjectFlag].Text.ToString();
                    string sDescription = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Description].Text.ToString();
                    string sStatus = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.Status].Text.ToString();

                    // SPI Data
                    string sSPIUserID = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPIUserID].Text.ToString();
                    string sSPIUserPW = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPIUserPW].Text.ToString();
                    string sSPIVersion = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPIVersion].Text.ToString();

                    string sPDSType = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.PDSType].Text.ToString();

                    string sIdealOutputPath = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.IdealOutputPath].Text.ToString();

                    string sSPIPBSDefaultUseFlag = fpSpreadData.ActiveSheet.Cells[i, (int)menColumn.SPIPBSDefaultUseFlag].Text.ToString();

                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        string sError = string.Empty;

                        if (sStatus != SpreadFlag.None)
                        {
                            if (bDeleteFlag)
                            {
                                if (sStatus == SpreadFlag.Delete)
                                {
                                    if (!proxy.SaveCommon_SPTool_Project(EDCS.WF.BSL.Variable.SharedVariable.IsIE,
                                                                 long.Parse(sProjectID),
                                                                 long.Parse(sProjectMasterID),
                                                                 sProjectNo,
                                                                 sProjectName,
                                                                 mToolType,
                                                                 sDBHost,
                                                                 sDBPort,
                                                                 sDBService,
                                                                 sDBSID,
                                                                 sDBUserID,
                                                                 sDBUserPW,
                                                                 sDBVersion,
                                                                 sDBSchema,
                                                                 sIniPath,
                                                                 sSPPIDVersion,
                                                                 sBorderSize,
                                                                 sSeedDate,
                                                                 bool.Parse(sDeleteFlag),
                                                                 bool.Parse(sDefaultProjectFlag),
                                                                 sDescription,
                                                                 sStatus,
                                                                 sSPIUserID,
                                                                 sSPIUserPW,
                                                                 sSPIVersion,
                                                                 sPDSType,
                                                                 sIdealOutputPath,
                                                                 bool.Parse(sSPIPBSDefaultUseFlag),
                                                                ref  sError))
                                    {
                                        sbError.AppendLine(string.Format("{0} : {1}", sProjectNo, sError));
                                    }
                                    else
                                    {
                                        bUpdateFlag = true;
                                    }
                                }
                            }
                            else
                            {
                                if (!proxy.SaveCommon_SPTool_Project(EDCS.WF.BSL.Variable.SharedVariable.IsIE,
                                                           long.Parse(sProjectID),
                                                           long.Parse(sProjectMasterID),
                                                           sProjectNo,
                                                           sProjectName,
                                                           mToolType,
                                                           sDBHost,
                                                           sDBPort,
                                                           sDBService,
                                                           sDBSID,
                                                           sDBUserID,
                                                           sDBUserPW,
                                                           sDBVersion,
                                                           sDBSchema,
                                                           sIniPath,
                                                           sSPPIDVersion,
                                                           sBorderSize,
                                                           sSeedDate,
                                                           bool.Parse(sDeleteFlag),
                                                            bool.Parse(sDefaultProjectFlag),
                                                           sDescription,
                                                           sStatus,
                                                           sSPIUserID,
                                                           sSPIUserPW,
                                                           sSPIVersion,
                                                           sPDSType,
                                                           sIdealOutputPath,
                                                           bool.Parse(sSPIPBSDefaultUseFlag),
                                                          ref  sError))
                                {
                                    sbError.AppendLine(string.Format("{0} : {1}", sProjectNo, sError));
                                }
                                else
                                {
                                    bUpdateFlag = true;
                                }
                            }
                        }
                    }
                }

                SetSpread(mProjectMasterID, mToolType);

                if (sbError.Length > 0)
                {
                    MessageBox.Show(sbError.ToString());
                }
                else if (bUpdateFlag)
                {
                    MessageBox.Show(this, Resources.Message10000, this.Text);
                }
            }
        }

        private void fpSpreadData_Change(object sender, ChangeEventArgs e)
        {
            //this.fpSpreadData.Change -= new ChangeEventHandler(this.fpSpreadData_Change);
            SetChangeFlag();
            //this.fpSpreadData.Change += new ChangeEventHandler(this.fpSpreadData_Change);
        }
        private void fpSpreadData_ClipboardPasted(object sender, ClipboardPastedEventArgs e)
        {
            SetChangeFlag();
        }

        public void SetChangeFlag()
        {
            if (fpSpreadData.ActiveSheet != null && !fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Text.Equals(SpreadFlag.Add))
            {
                fpSpreadData.ActiveSheet.Cells[fpSpreadData.ActiveSheet.ActiveRowIndex, (int)menColumn.Status].Text = SpreadFlag.Update;
            }
        }

    }
}
