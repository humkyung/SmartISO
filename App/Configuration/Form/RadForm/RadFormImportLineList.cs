using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;
using Telerik.WinControls.UI;
using System.Linq;
using System.Configuration;
using GemBox.Spreadsheet;

using SmartISO.Framework.Service;
using SmartISO.Service.Interface;

namespace Configuration.Form.RadForm
{
    public partial class RadFormImportLineList : TechSun.Theme.KRadForm///Telerik.WinControls.UI.RadForm
    {
        public Int64 ProjectUID { get; set; }

        private List<string> KeyCols = new List<string>() { "UID", "PROJECT_UID" };
        BackgroundWorker worker = new BackgroundWorker() { WorkerReportsProgress = true, WorkerSupportsCancellation = true };

        public IEnumerable<string> CheckedSheetName
        {
            get
            {
                var rows = (from GridViewRowInfo row in this.radGridViewSheetName.Rows
                            where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
                            select row.Cells["SheetName"].Value.ToString());
                return rows;
            }
        }

        public RadFormImportLineList()
        {
            InitializeComponent();
            {
                OpenFileDialog dialog = this.radBrowseEditorExcelFile.Dialog as OpenFileDialog;
                dialog.Filter = "(*.xlsx;*.xls)|*.xlsx;*.xls";

                this.radDropDownListStartRowNumber.DataSource = new List<string>() { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

                worker.DoWork += new DoWorkEventHandler(worker_DoWork);
                worker.ProgressChanged += new ProgressChangedEventHandler(worker_ProgressChanged);
                worker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(worker_RunWorkerCompleted);

                this.Disposed += new EventHandler(RadFormImportLineList_Disposed); 
                Program.OnIdleUpdate += new Program.IdleUpdateEvent(OnIdleUpdate);
            }
        }

        void RadFormImportLineList_Disposed(object sender, EventArgs e)
        {
            Program.OnIdleUpdate -= new Program.IdleUpdateEvent(OnIdleUpdate);
        }

        /// <summary>
        /// update enabled property of control
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.15</date>
        public void OnIdleUpdate()
        {
            this.kRadFreeButtonOK.Enabled = (!worker.IsBusy && (CheckedSheetName.Count() > 0));
        }

        private void radBrowseEditorExcelFile_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                SpreadsheetInfo.SetLicense(ConfigurationManager.AppSettings["GemBoxLicense"]);
                ExcelFile oExcelFile = new ExcelFile();
                string sExt = System.IO.Path.GetExtension(this.radBrowseEditorExcelFile.Value);
                if (".XLSX" == sExt.ToUpper())
                {
                    oExcelFile.LoadXlsx(this.radBrowseEditorExcelFile.Value, XlsxOptions.PreserveMakeCopy);
                }
                else
                {
                    oExcelFile.LoadXls(this.radBrowseEditorExcelFile.Value, XlsOptions.PreserveAll);
                }

                foreach (ExcelWorksheet oSheet in oExcelFile.Worksheets)
                {
                    GridViewRowInfo oGridRow = this.radGridViewSheetName.Rows.AddNew();
                    oGridRow.Cells["SheetName"].Value = oSheet.Name;
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        private void RadFormImportLineList_Load(object sender, EventArgs e)
        {
            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
            {
                /// get LineList Key for project - 2017.05.15 added by humkyung
                using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "GetConfigCommand"))
                {
                    DataRow drParm = dsParam.Tables["Params"].NewRow();
                    {
                        drParm["Ref_UID"] = ProjectUID;
                        drParm["Section"] = "LineList";
                        drParm["Key"] = "Key";
                    }
                    dsParam.Tables["Params"].Rows.Add(drParm);

                    using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetConfigCommand", dsParam))
                    {
                        this.radTextBoxLineListKey.Text = ds.Tables[0].Rows[0]["Value"].ToString();
                    }
                }
                /// up to here
            }
        }

        /// <summary>
        /// Import LineList
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.15</date>
        public void SaveData()
        {
            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveConfigsCommand"))
                    {
                        DataRow drParm = dsParam.Tables["Params"].NewRow();
                        {
                            drParm["Ref_UID"] = ProjectUID;
                            drParm["Section"] = "LineList";
                            drParm["Key"] = "Key";
                            drParm["Value"] = this.radTextBoxLineListKey.Text;
                        }
                        dsParam.Tables["Params"].Rows.Add(drParm);

                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveConfigsCommand", dsParam);
                    }
                }
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog();
            }
            finally
            {
            }
        }

        #region Import LineList
       Dictionary<int, string> GetHeaderCell(ExcelWorksheet oSheet, int iRow)
        {
            Dictionary<int, string> res = new Dictionary<int, string>();

            CellRange oCellRng = oSheet.GetUsedCellRange(true);
            for (int col = oCellRng.FirstColumnIndex; col <= oCellRng.LastColumnIndex; ++col)
            {
                object oValue = oSheet.Cells[iRow - 1, col].Value;
                res.Add(col, (null != oValue) ? oValue.ToString() : string.Empty);
            }

            return res;
        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL))
                {
                    using (DataSet dsParam = svcProxy.GetCommandParam("SmartISO.BSL", "SaveLineListCommand"))
                    {
                        SpreadsheetInfo.SetLicense(ConfigurationManager.AppSettings["GemBoxLicense"]);
                        ExcelFile oExcelFile = new ExcelFile();
                        string sExt = System.IO.Path.GetExtension(e.Argument.ToString());
                        if (".XLSX" == sExt.ToUpper())
                        {
                            oExcelFile.LoadXlsx(e.Argument.ToString(), XlsxOptions.PreserveMakeCopy);
                        }
                        else
                        {
                            oExcelFile.LoadXls(e.Argument.ToString(), XlsOptions.PreserveAll);
                        }

                        int iStartRow = Convert.ToInt32(this.radDropDownListStartRowNumber.Items[this.radDropDownListStartRowNumber.SelectedIndex].Text);

                        int iTotalRows = 0, iRows = 0;
                        #region get total row count
                        foreach (string sSheetName in this.CheckedSheetName)
                        {
                            ExcelWorksheet oSheet = oExcelFile.Worksheets[sSheetName];
                            CellRange oCellRng = oSheet.GetUsedCellRange(true);
                            iTotalRows += (oCellRng.LastRowIndex - iStartRow) + 1;
                        }
                        #endregion

                        foreach (string sSheetName in this.CheckedSheetName)
                        {
                            ExcelWorksheet oSheet = oExcelFile.Worksheets[sSheetName];
                            Dictionary<int, string> oHeaderCells = GetHeaderCell(oSheet, iStartRow);
                            CellRange oCellRng = oSheet.GetUsedCellRange(true);
                            for (int row = iStartRow; row <= oCellRng.LastRowIndex; ++row)
                            {
                                DataRow dr = dsParam.Tables[0].NewRow();
                                for (int col = oCellRng.FirstColumnIndex; col <= oCellRng.LastColumnIndex; ++col)
                                {
                                    object oValue = oSheet.Cells[row, col].Value;
                                    if (dr.Table.Columns.Contains(oHeaderCells[col])) dr[oHeaderCells[col]] = oValue;
                                }
                                dr["Project_UID"] = ProjectUID;
                                dr["RefKey"] = this.radTextBoxLineListKey.Text;
                                dsParam.Tables[0].Rows.Add(dr);

                                worker.ReportProgress(Convert.ToInt32(iRows++ / iTotalRows));
                            }
                        }
                        svcProxy.ExecuteCommand("SmartISO.BSL", "SaveLineListCommand", dsParam);
                    }
                }
            }
            catch (Exception ex)
            {
                throw;
            }
            finally
            {
                worker.ReportProgress(100);
            }
        }

        void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            this.radProgressBarWork.Value1 = e.ProgressPercentage;
        }

        void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (null != e.Error)
            {
                new TechSun.Theme.frmWarning(e.Error.Message).ShowDialog(this);
            }
        }
        #endregion

        private void kRadFreeButtonOK_Click(object sender, EventArgs e)
        {
            try
            {
                SaveData();
                worker.RunWorkerAsync(this.radBrowseEditorExcelFile.Value);
            }
            catch (Exception ex)
            {
                new TechSun.Theme.frmWarning(ex.Message).ShowDialog(this);
            }
        }

        /// <summary>
        /// Linelist Key Setting
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2017.05.15</date>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void radButtonLinelistKeySetting_Click(object sender, EventArgs e)
        {
            using (ISmartISOService svcProxy = ServiceFactory.CreateServiceChannel<ISmartISOService>(AppDocData.ServiceURL, "wsHttp"))
            {
                using (DataSet ds = svcProxy.ExecuteCommand("SmartISO.BSL", "GetLineListColInfoCommand", null))
                {
                    List<string> oColList = new List<string>();
                    foreach (DataRow dr in ds.Tables[0].Rows)
                    {
                        string sName = dr["Name"].ToString();
                        if (this.KeyCols.Contains(sName.ToUpper())) continue;
                        oColList.Add(sName);
                    }

                    Configuration.RadFormSmartISOFormFormatting oFrm = new RadFormSmartISOFormFormatting(this.radTextBoxLineListKey.Text, oColList);
                    if (DialogResult.OK == oFrm.ShowDialog(this))
                    {
                        this.radTextBoxLineListKey.Text = oFrm.Result;
                    }
                }
            }
        }

        private void kRadFreeButtonClose_Click(object sender, EventArgs e)
        {
            DialogResult = System.Windows.Forms.DialogResult.Cancel;
        }
    }
}
