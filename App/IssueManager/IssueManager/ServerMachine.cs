using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

using framework.core;
using framework.core.forms;
using System.Data.OracleClient;

using GemBox.Spreadsheet;

namespace SmartISO
{
    public partial class ServerMachine : UserControl
    {
		private CBusinessLogic BusinessLogic = null;
		private OracleDataAdapter m_dataAdapter = null;
		private DataTable m_table = null;
		private int m_iCurRowIndex = -1;

		/// <summary>
		/// get/set revision data dictionary
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.08.02</date>
		private Dictionary<string, List<string>> m_oRevDataDic = new Dictionary<string, List<string>>();
		public Dictionary<string, List<string>> RevDataDic
		{
			get { return m_oRevDataDic; }
			set { m_oRevDataDic = value; }
		}

        public ServerMachine(CBusinessLogic oBusinessLogic)
        {
            InitializeComponent();

			this.BusinessLogic = oBusinessLogic;
        }

		/// <summary>
		/// form load
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void ServerMachine_Load(object sender, EventArgs e)
		{
			lvwREV.Columns.Add("Rev No.");
			lvwREV.Columns[0].Width = 270;

			listISSUE_DATE.Columns.Add("Date");
			listISSUE_DATE.Columns[0].Width = 100;

			listPLANT.Items.Add("[all]");
			listAREA.Items.Add("[all]");
			listUNIT.Items.Add("[all]");
			listSECTION.Items.Add("[all]");
			GetDataSetFrom("SELECT DISTINCT C_PLANT_NO,C_AREA_NO,C_UNIT_NO,C_SECTION_NO FROM T_ISSUED_LINE_DATA");
			if (null != m_table)
			{
				foreach (DataRow dr in m_table.Rows)
				{
					if (null != dr[0]) listPLANT.Items.Add(dr[0]);
					if (null != dr[1]) listAREA.Items.Add(dr[1]);
					if (null != dr[2]) listUNIT.Items.Add(dr[2]);
					if (null != dr[3]) listSECTION.Items.Add(dr[3]);
				}
			}

			string sRevNoList = string.Empty;
			GetDataSetFrom("SELECT DISTINCT C_REV_NO FROM T_ISSUED_REVISION");
			if (null != m_table)
			{
				foreach (DataRow dr in m_table.Rows)
				{
					ListViewItem oLVItem = lvwREV.Items.Add(dr[0].ToString());
				}
			}

			GetDataSetFrom("SELECT DISTINCT C_ISSUE_DATE FROM T_ISSUE_DATE");
			if (null != m_table)
			{
				foreach (DataRow dr in m_table.Rows)
				{
					string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(dr[0].ToString());
					listISSUE_DATE.Items.Add(sDisplayIssueDate);
				}
			}

			//! get revision no from T_ISSUED_REVISION table
			List<string> RevNoList = new List<string> { };
			GetDataSetFrom("SELECT DISTINCT C_REV_NO FROM T_ISSUED_REVISION");
			if (null != m_table)
			{
				foreach (DataRow dr in m_table.Rows)
				{
					RevNoList.Add(dr[0].ToString());
				}
			}

			comboBox_REVISION_OPTION.Items.Add("LATEST");
			foreach (string s in RevNoList)
			{
				comboBox_REVISION_OPTION.Items.Add(s);
			}
		}

		/// <summary>
		/// size changed
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void ServerMachine_SizeChanged(object sender, EventArgs e)
		{
			//groupBox_RevisionData.Top = this.ClientSize.Height - groupBox_RevisionData.Height + 10;
			//groupBox_RevisionData.Height = groupBox_RevisionData.Height - 15;
			//groupBox_NextGrade.Top = this.ClientSize.Height - groupBox_NextGrade.Height + 10;
			//groupBox_NextGrade.Height = groupBox_NextGrade.Height - 15;
			//groupBox_IssueDate.Top = this.ClientSize.Height - groupBox_IssueDate.Height + 10;
			//groupBox_IssueDate.Height = groupBox_IssueDate.Height - 15;
			//groupBox_IssueDate.Left = this.ClientSize.Width - groupBox_IssueDate.Width - 10;
			//groupBox_NextGrade.Left = groupBox_IssueDate.Left - groupBox_NextGrade.Width - 10;
			///groupBox_RevisionData.Width = groupBox_NextGrade.Left - groupBox_RevisionData.Left - 10;
			///dataGridView_RevisionData.Width = groupBox_RevisionData.Width - 15;
		}

		#region Helper
		/// <summary>
		/// get dataset from database
		/// </summary>
		/// <param name="selectCommand"></param>
		private void GetDataSetFrom(string selectCommand)
		{
			if ((string.Empty == this.BusinessLogic.RemoteDatabaseSetting.sHost) || (string.Empty == this.BusinessLogic.RemoteDatabaseSetting.sService) ||
				(string.Empty == this.BusinessLogic.RemoteDatabaseSetting.sId) || (string.Empty == this.BusinessLogic.RemoteDatabaseSetting.sPassword))
				return;

			using (MyOracleConnection conn = new MyOracleConnection())
			{
				if (true == conn.Connect(this.BusinessLogic.RemoteDatabaseSetting.sHost, this.BusinessLogic.RemoteDatabaseSetting.sService,
					this.BusinessLogic.RemoteDatabaseSetting.sId, this.BusinessLogic.RemoteDatabaseSetting.sPassword))
				{
					m_dataAdapter = new OracleDataAdapter(selectCommand , conn.Connection);
					OracleCommandBuilder oCommandBuilder = new OracleCommandBuilder(m_dataAdapter);
										
					/// Populate a new data table and bind it to the BindingSource.
					m_table = new DataTable();
					m_table.Locale = System.Globalization.CultureInfo.InvariantCulture;
					m_dataAdapter.Fill(m_table);

					if (m_table.Columns.Contains("C_CANCELLATION"))
					{
						m_table.Columns.Remove("C_CANCELLATION");
					}
				}
			}
		}

		/*
		 *	@brief	get selected item text
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *	
		 *	@param	listbox
		 *	@param	iSel
		 *	
		 *	@return
		 */
		private string GetSelItemText(ref ListBox listbox)
		{
			string sSelItemText = string.Empty;
			for (int i = 0; i < listbox.Items.Count; ++i)
			{
				string sItemText = listbox.Items[i].ToString();
				if (string.Empty == sItemText) continue;

				if ("@" == sItemText.Substring(0, 1))
				{
					if (sItemText.Contains("[all]"))
					{
						sSelItemText = "ALL";
						break;
					}
					else
					{
						sSelItemText += "'" + sItemText.Substring(2) + "',";
					}
				}
			}
			if ((string.Empty != sSelItemText) && ("," == sSelItemText.Substring(sSelItemText.Length - 1, 1)))
			{
				sSelItemText = sSelItemText.Substring(0, sSelItemText.Length - 1);
			}

			return sSelItemText;
		}

		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *
		 *	@return
		 */
		private string GetSelectedIssueDate()
		{
			string sIssueDate = string.Empty;

			if (checkBox_ISSUE_DATE.Checked)
			{
				for (int i = 0; i < listISSUE_DATE.Items.Count; ++i)
				{
					if (listISSUE_DATE.Items[i].Checked)
					{
						/// 2011.08.06 added by humkyung
						Int32 dYear = 0, dMonth = 0, dDay = 0;
						this.BusinessLogic.ParseIssueDate(ref dYear, ref dMonth, ref dDay, listISSUE_DATE.Items[i].SubItems[0].Text);
						string _sIssueDate = string.Format("{0:0000}-{1:00}-{2:00}", dYear, dMonth, dDay);
						/// up to here

						sIssueDate += "'" + _sIssueDate + "',";
					}
				}

				if ((string.Empty != sIssueDate) && ("," == sIssueDate.Substring(sIssueDate.Length - 1, 1)))
				{
					sIssueDate = sIssueDate.Substring(0, sIssueDate.Length - 1);
				}
			}

			return sIssueDate;
		}
		/*
		 *	@brief
		 *	
		 *	@author	KyoungMin
		 *	
		 *	@date	2011.02.23
		 *
		 *	@return
		 */
		private string GetSelected_RevNo()
		{
			string sIssueRevNo = string.Empty;

			if (chkREV.Checked)
			{
				for (int i = 0; i < lvwREV.Items.Count; ++i)
				{
					if (lvwREV.Items[i].Checked)
					{
						sIssueRevNo += "'" + lvwREV.Items[i].Tag.ToString() + "',";
					}
				}

				if ((string.Empty != sIssueRevNo) && ("," == sIssueRevNo.Substring(sIssueRevNo.Length - 1, 1)))
				{
					sIssueRevNo = sIssueRevNo.Substring(0, sIssueRevNo.Length - 1);
				}
			}

			return sIssueRevNo;
		}
		/// <summary>
		/// get issue date
		/// </summary>
		/// <param name="sKey"></param>
		/// <param name="iRevNo">get the last revision number if iRevNo is -1</param>
		/// <returns></returns>
		private string GetRevIssueDateAtIndex(string sKey, int iRevNo)
		{
			string sIssueDate = string.Empty;

			/// store all issued revision data - 2011.08.02 added by humkyung
			if (0 == RevDataDic.Count)
			{
				string sCommand = "SELECT C_KEY,C_REV_DATE FROM T_ISSUED_REVISION";
				GetDataSetFrom(sCommand);
				for (int i = 0; i < m_table.Rows.Count; ++i)
				{
					string _sKey = m_table.Rows[i].ItemArray[0].ToString();
					if (false == RevDataDic.ContainsKey(_sKey))
					{
						RevDataDic[_sKey] = new List<string>();
					}

					string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(m_table.Rows[i].ItemArray[1].ToString());
					RevDataDic[_sKey].Add(sDisplayIssueDate);
				}
			}

			if (true == RevDataDic.ContainsKey(sKey))
			{
				if (-1 != iRevNo)
				{
					if (RevDataDic[sKey].Count > iRevNo)
					{
						sIssueDate = RevDataDic[sKey][iRevNo];
					}
				}
				else
				{
					sIssueDate = RevDataDic[sKey][RevDataDic[sKey].Count - 1].ToString();
				}
			}

			return sIssueDate;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="sFieldName"></param>
		/// <returns></returns>
		private long FieldName2Long(string sFieldName)
		{
			long ret = 0L;

			for (int i = sFieldName.Length - 1; i >= 0; i--)
			{
				ret += (sFieldName[i] - 'A' + 1) + i * 26;
			}

			return ret;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="sFirstCol"></param>
		/// <param name="nCol"></param>
		/// <returns></returns>
		private string SetNextColumn(string sFirstCol, int nCol)
		{
			string res = string.Empty;
			long nFirstCol = FieldName2Long(sFirstCol);
			long nNextCol = nFirstCol + nCol;

			string sString = string.Empty;
			do
			{
				long nDenom = nNextCol % 26;

				sString = string.Format("{0}", (char)('A' + nDenom - 1));

				res = sString + res;

				nNextCol /= 26;

			} while (nNextCol > 0);

			return res;
		}
		#endregion

		private void btnQuery_Click(object sender, EventArgs e)
		{
			string sQuery = "SELECT * FROM T_ISSUED_LINE_DATA";

			string sSelPlantNo = GetSelItemText(ref listPLANT);
			string sSelAreaNo = GetSelItemText(ref listAREA);
			string sSelUnitNo = GetSelItemText(ref listUNIT);
			string sSelSectionNo = GetSelItemText(ref listSECTION);

			if (((string.Empty != sSelPlantNo) && ("ALL" != sSelPlantNo)) ||
				((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo)) ||
				((string.Empty != sSelUnitNo) && ("ALL" != sSelUnitNo)) ||
				((string.Empty != sSelSectionNo) && ("ALL" != sSelSectionNo)))
			{
				sQuery += " WHERE";
			}

			if ((string.Empty != sSelPlantNo) && ("ALL" != sSelPlantNo))
			{
				sQuery += " C_PLANT_NO IN (" + sSelPlantNo + ") AND";
			}
			if ((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo))
			{
				sQuery += " C_AREA_NO IN (" + sSelAreaNo + ") AND";
			}
			if ((string.Empty != sSelUnitNo) && ("ALL" != sSelUnitNo))
			{
				sQuery += " C_UNIT_NO IN (" + sSelUnitNo + ") AND";
			}
			if ((string.Empty != sSelSectionNo) && ("ALL" != sSelSectionNo))
			{
				sQuery += " C_SECTION_NO IN (" + sSelSectionNo + ") AND";
			}

			string sIssueDate = GetSelectedIssueDate();
			string sRevNo = GetSelected_RevNo();

			if (string.Empty != sIssueDate || string.Empty != sRevNo)
			{
				int chkEmpty = 0;

				if (sQuery.Contains(" WHERE "))
				{
					sQuery += " C_KEY IN (SELECT DISTINCT C_KEY FROM T_ISSUED_REVISION WHERE C_REV_DATE IN (" + sIssueDate + "))";
				}
				else
				{
					sQuery += " WHERE C_KEY IN (SELECT C_KEY FROM T_ISSUED_REVISION WHERE";
					if (string.Empty != sIssueDate)
					{
						sQuery += " C_REV_DATE IN (" + sIssueDate + ")";
						chkEmpty++;
					}
					sQuery += ")";
				}
			}
			if (" AND" == sQuery.Substring(sQuery.Length - 4, 4))
			{
				sQuery = sQuery.Substring(0, sQuery.Length - 4);
			}

			using (LoadingIndicator oIndicator = new LoadingIndicator(this))
			{
				oIndicator.Start();

				GetDataSetFrom(sQuery);
				dataGridView_LineData.DataSource = m_table;
				///! Resize the DataGridView columns to fit the newly loaded content.
				dataGridView_LineData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

				for (int i = 0; i < dataGridView_LineData.Columns.Count; ++i)
				{
					dataGridView_LineData.Columns[i].ReadOnly = true;
				}

				///dataGridView_LineData.Columns[0].Width = 20;
			}
		}

		private void dataGridView_LineData_CurrentCellDirtyStateChanged(object sender, EventArgs e)
		{
			if (dataGridView_LineData.IsCurrentCellDirty)
			{
				dataGridView_LineData.CommitEdit(DataGridViewDataErrorContexts.Commit);
			}
		}

		private void dataGridView_LineData_CurrentCellChanged(object sender, EventArgs e)
		{
			if (null != dataGridView_LineData.CurrentCell)
			{
				int iRowIndex = dataGridView_LineData.CurrentCell.RowIndex;
				if (m_iCurRowIndex != iRowIndex)
				{
					m_iCurRowIndex = iRowIndex;

					this.BusinessLogic.m_sKey = dataGridView_LineData.Rows[iRowIndex].Cells["C_KEY"].Value.ToString();
					DisplayRevisionDataWith(this.BusinessLogic.m_sKey);
				}
			}
		}

		/// <summary>
		/// display revision data according to key
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.10.26</date>
		/// <param name="sKey"></param>
		private void DisplayRevisionDataWith(string sKey)
		{
			string sQuery = "SELECT * FROM T_ISSUED_REVISION WHERE C_KEY='" + sKey + "' ORDER BY C_REV_NO";
			GetDataSetFrom(sQuery);
			dataGridView_RevisionData.DataSource = m_table;
			// Resize the DataGridView columns to fit the newly loaded content.
			dataGridView_RevisionData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

			//! 2011.02.24 added by HumKyung
			foreach (DataGridViewRow row in dataGridView_RevisionData.Rows)
			{
				int iGrade = Convert.ToInt32(row.Cells["C_GRADE"].Value.ToString()) / 100;
				row.Cells["C_GRADE"].Value = iGrade.ToString();
				int iRevNo = Convert.ToInt32(row.Cells["C_REV_NO"].Value.ToString());
				foreach (RevisionStage rs in this.BusinessLogic.m_RevisionStageList)
				{
					if (iRevNo == rs.m_lNo)
					{
						row.Cells["C_REV_NO"].Tag = row.Cells["C_REV_NO"].Value;
						row.Cells["C_REV_NO"].Value = rs.m_sDefine + rs.m_sText;
						break;
					}
				}

				/// 2011.08.06 added by humkyung
				string sDisplayIssueDate = this.BusinessLogic.GetDisplayIssueDateFrom(row.Cells["C_REV_DATE"].Value.ToString());
				row.Cells["C_REV_DATE"].Value = sDisplayIssueDate;
				sDisplayIssueDate = this.BusinessLogic.GetDisplayIssueDateFrom(row.Cells["C_REV_CONTROL_WORKING_DATE"].Value.ToString());
				row.Cells["C_REV_CONTROL_WORKING_DATE"].Value = sDisplayIssueDate;
				/// up to here

				for (int i = 0; i < dataGridView_LineData.Columns.Count; ++i)
				{
					dataGridView_LineData.Columns[i].ReadOnly = true;
				}

			}

			//! 2011.02.19 added by HumKyung
			for (int i = 0; i < dataGridView_RevisionData.Columns.Count; i++)
			{
				dataGridView_RevisionData.Columns[i].ReadOnly = true;
				dataGridView_RevisionData.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable; /// 2012.05.18 added by humkyung
			}
		}

        /// <summary>
        /// CLIENT NAME,PROJECT NAME 과 JOB NO.를 기입한다.
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.09.03</date>
        /// <param name="xlApp"></param>
        private void ReplaceUserInput(ExcelWorksheet oWorksheet)
        {
            CellRange oRange = oWorksheet.GetUsedCellRange(true);
            for (int i = 0; i < oRange.LastColumnIndex; ++i)
            {
                for (int j = 0; j < oRange.LastRowIndex; ++j)
                {
                    object oValue = oWorksheet.Cells[j, i].Value;
                    if (null == oValue) continue;
                    if ("$CLIENT" == oValue.ToString())
                    {
                        oWorksheet.Cells[j, i].Value = "'" + BusinessLogic.ClientName;
                    }
                    else if ("$PROJECT" == oValue.ToString())
                    {
                        oWorksheet.Cells[j, i].Value = "'" + BusinessLogic.ProjectDesc;
                    }
                    else if ("$JOB" == oValue.ToString())
                    {
                        oWorksheet.Cells[j, i].Value = "'" + BusinessLogic.JobNo;
                    }
                }
            }
        }

        /// <summary>
        /// set cancellation cell
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2013.02.16</date>
        /// <param name="oWorksheet"></param>
        /// <param name="row"></param>
        /// <param name="col"></param>
        private void SetCancellationCell(ExcelWorksheet oWorksheet, int row, int col)
        {
            oWorksheet.Cells[row, col].Style.Font.Color = Color.Red;
            oWorksheet.Cells[row, col].Style.Font.Strikeout = true;
        }

		/// <summary>
		/// export issued line data in database server to excel file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.22</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void btnExport_Click(object sender, EventArgs e)
		{
            AppDocData.frameworkCore.Logger.Reset();

            string sExportFilePath = string.Empty;
            using (LoadingIndicator oIndicator = new LoadingIndicator(this))
            {
                oIndicator.Start();
                int iNextColumn = BusinessLogic.DrawingExcelMatching.iNextColumn;
                try
                {
                    int ExCount = BusinessLogic.DrawingExcelMatching.ExCount;
                    int iExlStartRow = BusinessLogic.DrawingExcelMatching.iExlStartRow;

                    ///! 
                    RevDataDic.Clear();	///! 2011.08.02 added by humkyung

                    DrawingReporter oDrawingReporter = new DrawingReporter();
                    oDrawingReporter.ReadDrawingExcelMatchingEntry(BusinessLogic);

                    SpreadsheetInfo.SetLicense(AppDocData.GemBoxLicense);
                    ExcelFile ef = new ExcelFile();
                    try
                    {
                        string sXlsFilePath = Path.Combine(BusinessLogic.ProjectPath, @"Setting\ISOPrintTemp.xls");
                        ef.LoadXls(sXlsFilePath, XlsOptions.PreserveAll);

                        object[] parameter = new object[1];
                        parameter[0] = 1;
                        ExcelWorksheet oWorksheet = ef.Worksheets[0];
                        this.ReplaceUserInput(oWorksheet);

                        /// copy sheets
                        int iSheetCount = dataGridView_LineData.Rows.Count / BusinessLogic.ITEM_COUNT_PER_SHEET;
                        if (-1 != iNextColumn)
                        {
                            int iRemain = (dataGridView_LineData.Rows.Count % BusinessLogic.ITEM_COUNT_PER_SHEET);
                            if (iRemain > 0) ++iSheetCount;
                            for (int i = 0; i < iSheetCount - 1; ++i)
                            {
                                ExcelWorksheet ws = ef.Worksheets.AddCopy(string.Format("Sheet{0}", (i + 2)), oWorksheet);
                            }
                        }
                        else
                        {
                            /// 2011.08.02 added by humkyung
                            iSheetCount = 1;
                            BusinessLogic.ITEM_COUNT_PER_SHEET = BusinessLogic.ITEM_HALF_COUNT_PER_SHEET = dataGridView_LineData.Rows.Count;
                        }

                        using (LoadingIndicator oLoadingIndicator = new LoadingIndicator(this))
                        {
                            oLoadingIndicator.Start();

                            for (int iSheet = 1; iSheet <= iSheetCount; ++iSheet)
                            {
                                oWorksheet = ef.Worksheets[iSheet - 1];
                                int iRow = iExlStartRow, iNo = 0;
                                for (int k = 0; k < BusinessLogic.ITEM_COUNT_PER_SHEET; ++k)
                                {
                                    int i = (iSheet - 1) * BusinessLogic.ITEM_COUNT_PER_SHEET + k + 1;
                                    if (i > dataGridView_LineData.Rows.Count) break;

                                    int row = iExlStartRow + k - 1;
                                    if (k >= BusinessLogic.ITEM_HALF_COUNT_PER_SHEET)
                                    {
                                        iNo = iNextColumn;
                                        row = row - BusinessLogic.ITEM_HALF_COUNT_PER_SHEET;
                                    }

                                    object oValue = dataGridView_LineData.Rows[i - 1].Cells[0].Value;
                                    bool bCancellation = ((string.Empty != oValue.ToString()) && (true == (bool)oValue));
                                    if (bCancellation) SetCancellationCell(oWorksheet, row, iNo);
                                    oWorksheet.Cells[row, iNo].Value = string.Format("{0}", i); /// write index

                                    string sKey = dataGridView_LineData.Rows[i - 1].Cells["C_KEY"].Value.ToString();
                                    foreach (ExlStruct oExlStruct in oDrawingReporter.ExlResultEntry)
                                    {
                                        System.Text.RegularExpressions.Regex oRegex = new System.Text.RegularExpressions.Regex("REV [0-9]+ DATE");
                                        if (true == oRegex.IsMatch(oExlStruct.ExlDesc))
                                        {
                                            int iRevNo = 0;
                                            string[] oTokens = oExlStruct.ExlDesc.Split(" ".ToCharArray());
                                            Int32.TryParse(oTokens[1], out iRevNo);
                                            string sIssueDate = GetRevIssueDateAtIndex(sKey, iRevNo);

                                            int col = BusinessLogic.FieldName2Int(oExlStruct.ExlColumn);
                                            string sLast = string.Format("{0}", row);
                                            if (k >= BusinessLogic.ITEM_HALF_COUNT_PER_SHEET)
                                            {
                                                col = BusinessLogic.SetNextColumn(col, iNextColumn);
                                            }

                                            if (bCancellation) SetCancellationCell(oWorksheet, row, col);
                                            oWorksheet.Cells[row, col].Value = sIssueDate;
                                        }
                                        else if ("LAST REV NO" == oExlStruct.ExlDesc)
                                        {
                                            string sIssueDate = GetRevIssueDateAtIndex(sKey, -1);
                                            int col = BusinessLogic.FieldName2Int(oExlStruct.ExlColumn);
                                            string sLast = string.Format("{0}", row);
                                            if (k >= BusinessLogic.ITEM_HALF_COUNT_PER_SHEET)
                                            {
                                                col = BusinessLogic.SetNextColumn(col, iNextColumn);
                                            }

                                            if (bCancellation) SetCancellationCell(oWorksheet, row, col);
                                            oWorksheet.Cells[row, col].Value = sIssueDate;
                                        }
                                        else
                                        {
                                            /// 2011.08.02 - added by humkyung
                                            string sValue = string.Empty;
                                            for (int column = 0; column < dataGridView_LineData.Columns.Count; ++column)
                                            {
                                                if (oExlStruct.ExlDesc == dataGridView_LineData.Columns[column].Name)
                                                {
                                                    sValue = dataGridView_LineData.Rows[i - 1].Cells[column].Value.ToString();
                                                    break;
                                                }
                                            }
                                            /// up to here

                                            if (string.Empty != sValue)
                                            {
                                                if ("LINE STATUS" == oExlStruct.ExlDesc)
                                                {
                                                    if ("LIVE" == sValue) continue;
                                                    int col = BusinessLogic.FieldName2Int(oExlStruct.ExlColumn);
                                                    string sLast = string.Format("{0}", row);
                                                    if (k >= BusinessLogic.ITEM_HALF_COUNT_PER_SHEET)
                                                    {
                                                        col = BusinessLogic.SetNextColumn(col, iNextColumn);
                                                    }

                                                    if (bCancellation) SetCancellationCell(oWorksheet, row, col);
                                                    oWorksheet.Cells[row, col].Value = sValue;
                                                }
                                                else
                                                {
                                                    int col = BusinessLogic.FieldName2Int(oExlStruct.ExlColumn);
                                                    string sLast = string.Format("{0}", row);
                                                    if (k >= BusinessLogic.ITEM_HALF_COUNT_PER_SHEET)
                                                    {
                                                        col = BusinessLogic.SetNextColumn(col, iNextColumn);
                                                    }

                                                    if (bCancellation) SetCancellationCell(oWorksheet, row, col);
                                                    oWorksheet.Cells[row, col].Value = sValue;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        /// 2011.06.13 added by humkyung
                        DateTime today = DateTime.Today;
                        string sFileName = string.Format("HISOTRY_ISSUE_{0:D4}-{1:D2}-{2:D2}", today.Year, today.Month, today.Day);
                        ///
                        sExportFilePath = BusinessLogic.ProjectPath + @"\Report\" + sFileName + ".xls";
                        ef.SaveXls(sExportFilePath);    /// 2013.02.16 added by humkyung
                    }
                    catch (GemBox.Spreadsheet.SpreadsheetException ex)
                    {
                        AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR, ex.Message);
                    }
                }
                catch (Exception ex)
                {
                    AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR, ex.Message);
                }
            }

            /// open execl file when user click 'Yes' - 2011.06.13 added by humkyung
            if (string.Empty != sExportFilePath)
            {
                if (DialogResult.Yes == MessageBox.Show("Export is complete\n Will you open excel file?", "Message", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
                {
                    System.Diagnostics.Process.Start(sExportFilePath);
                }
            }

            AppDocData.frameworkCore.Logger.ShowErrorResult(this);
		}

		/*
			@brief
			
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *	
		 *	@param	listbox
		 *	@param	iSel
		 *	
		 *	@return
		 */
		private void OnDblclkQueryItem(ref ListBox listbox, int iSel)
		{
			string sString = listbox.Items[iSel].ToString();
			if (sString.Contains("[all]"))
			{
				if ("@" == sString.Substring(0, 1))
				{
					for (int i = 0; i < listbox.Items.Count; ++i)
					{
						string sItemText = listbox.Items[i].ToString();
						if (string.Empty != sItemText && ("@ " == sItemText.Substring(0, 2)))
						{
							listbox.Items.RemoveAt(i);
							sItemText = sItemText.Substring(2);
							listbox.Items.Insert(i, sItemText);
						}
					}
				}
				else
				{
					for (int i = 0; i < listbox.Items.Count; ++i)
					{
						string sItemText = listbox.Items[i].ToString();
						if ((sItemText.Length > 0) && ("@" != sItemText.Substring(0, 1)))
						{
							listbox.Items.RemoveAt(i);
							sItemText = "@ " + sItemText;
							listbox.Items.Insert(i, sItemText);
						}
					}
				}
			}
			else
			{
				if ("@" == sString.Substring(0, 1))
				{
					sString = sString.Replace("@ ", "");
					listbox.Items.RemoveAt(iSel);
					listbox.Items.Insert(iSel, sString);

					if ("@" == listbox.Items[0].ToString())
					{
						sString = listbox.Items[0].ToString().Replace("@ ", "");
						listbox.Items.RemoveAt(0);
						listbox.Items.Insert(0, sString);
					}
				}
				else
				{
					sString = "@ " + sString;
					listbox.Items.RemoveAt(iSel);
					listbox.Items.Insert(iSel, sString);
				}
			}
		}

		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *	
		 *	@param	listbox
		 *	@param	iSel
		 *	
		 *	@return
		 */
		private void UpdateAreaNoListBox()
		{
			string sQuery = "SELECT DISTINCT C_AREA_NO FROM T_ISSUED_LINE_DATA";
			string sCondition = GetSelItemText(ref listPLANT);

			if ("ALL" == sCondition)
			{
				GetDataSetFrom(sQuery);
			}
			else if (string.Empty != sCondition)
			{
				sQuery += " WHERE C_PLANT_NO IN (" + sCondition + ")";
				GetDataSetFrom(sQuery);
			}

			for (int i = listAREA.Items.Count - 1; i >= 1; --i)
			{
				listAREA.Items.RemoveAt(i);
			}
			foreach (DataRow dr in m_table.Rows)
			{
				listAREA.Items.Add(dr[0].ToString());
			}
		}

		private void listPLANT_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listPLANT.SelectedIndex)
			{
				OnDblclkQueryItem(ref listPLANT, listPLANT.SelectedIndex);
				UpdateAreaNoListBox();

				List<string> stringList = new List<string> { };
				for (int i = 0; i < listAREA.Items.Count; ++i)
				{
					stringList.Add(listAREA.Items[i].ToString());
				}
				listAREA.Items.Clear();
				for (int i = 0; i < stringList.Count; ++i)
				{
					string sTemp = stringList[i].Replace("@", "");
					listAREA.Items.Add(sTemp);
				}
			}
		}

		private void UpdateUnitNoListBox()
		{
			string sQuery = "SELECT DISTINCT C_UNIT_NO FROM T_ISSUED_LINE_DATA";
			string sSelPlantNo = GetSelItemText(ref listPLANT);
			string sSelAreaNo = GetSelItemText(ref listAREA);

			if ((string.Empty != sSelPlantNo) && ("ALL" != sSelPlantNo))
			{
				sQuery += " WHERE C_PLANT_NO IN (" + sSelPlantNo + ")";

				if ((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo))
				{
					sQuery += " AND C_AREA_NO IN (" + sSelAreaNo + ")";
				}
			}
			else
			{
				if ((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo))
				{
					sQuery += " WHERE C_AREA_NO IN (" + sSelAreaNo + ")";
				}
			}

			GetDataSetFrom(sQuery);
			for (int i = listUNIT.Items.Count - 1; i >= 1; --i)
			{
				listUNIT.Items.RemoveAt(i);
			}
			foreach (DataRow dr in m_table.Rows)
			{
				listUNIT.Items.Add(dr[0].ToString());
			}
		}

		private void listAREA_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listAREA.SelectedIndex)
			{
				OnDblclkQueryItem(ref listAREA, listAREA.SelectedIndex);
				UpdateUnitNoListBox();
			}
		}

		private void UpdateSectionNoListBox()
		{
			string sQuery = "SELECT DISTINCT C_SECTION_NO FROM T_ISSUED_LINE_DATA";
			string sSelPlantNo = GetSelItemText(ref listPLANT);
			string sSelAreaNo = GetSelItemText(ref listAREA);
			string sSelUnitNo = GetSelItemText(ref listUNIT);

			if ((string.Empty != sSelPlantNo) && ("ALL" != sSelPlantNo))
			{
				sQuery += " WHERE C_PLANT_NO IN (" + sSelPlantNo + ")";

				if ((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo))
				{
					sQuery += " AND C_AREA_NO IN (" + sSelAreaNo + ")";
				}

				if ((string.Empty != sSelUnitNo) && ("ALL" != sSelUnitNo))
				{
					sQuery += " AND C_UNIT_NO IN (" + sSelUnitNo + ")";
				}
			}
			else
			{
				if ((string.Empty != sSelAreaNo) && ("ALL" != sSelAreaNo))
				{
					sQuery += " WHERE C_AREA_NO IN (" + sSelAreaNo + ")";

					if ((string.Empty != sSelUnitNo) && ("ALL" != sSelUnitNo))
					{
						sQuery += " AND C_UNIT_NO IN (" + sSelUnitNo + ")";
					}
				}
				else
				{
					if ((string.Empty != sSelUnitNo) && ("ALL" != sSelUnitNo))
					{
						sQuery += " WHERE C_UNIT_NO IN (" + sSelUnitNo + ")";
					}
				}
			}

			GetDataSetFrom(sQuery);
			for (int i = listSECTION.Items.Count - 1; i >= 1; --i)
			{
				listSECTION.Items.RemoveAt(i);
			}
			foreach (DataRow dr in m_table.Rows)
			{
				listSECTION.Items.Add(dr[0].ToString());
			}
		}

		private void listUNIT_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listUNIT.SelectedIndex)
			{
				OnDblclkQueryItem(ref listUNIT, listUNIT.SelectedIndex);
				UpdateSectionNoListBox();
			}
		}

		private void listSECTION_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listSECTION.SelectedIndex)
			{
				OnDblclkQueryItem(ref listSECTION, listSECTION.SelectedIndex);
			}
		}
	}
}
