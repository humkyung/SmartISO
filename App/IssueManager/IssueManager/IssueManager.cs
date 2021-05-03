using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.OleDb;
using System.IO; // Add
using System.Reflection;
using System.Data.OracleClient;
using DgvFilterPopup;

using framework.core;
using framework.core.forms;

using EnterpriseDT.Util.Debug;
using EnterpriseDT.Net.Ftp;

using GemBox.Spreadsheet;

namespace SmartISO
{
	public partial class IssueManager : UserControl
	{
		private ADODB.Connection m_connection = new ADODB.Connection();
		private ADODB.Recordset m_recordset = new ADODB.Recordset();
		private OleDbDataAdapter m_dataAdapter = null;
        DataTable m_table = null;
		
		private int m_iCurRowIndex = -1;

		/// <summary>
		/// get/set revision data dictionary
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.08.02</date>
		private Dictionary<string, List<string>> m_oRevDataDic = new Dictionary<string,List<string>>();
		public Dictionary<string, List<string>> RevDataDic
		{
			get { return m_oRevDataDic; }
			set { m_oRevDataDic = value; }
		}

		/// <summary>
		/// get/set line status
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.08.02</date>
		private Dictionary<string, List<string>> m_oLineStatusDic = new Dictionary<string, List<string>>();
		public Dictionary<string, List<string>> LineStatusDic
		{
			get { return m_oLineStatusDic; }
			set { m_oLineStatusDic = value; }
		}

		private CBusinessLogic BusinessLogic = null;
		string AppPath = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf("\\"));

		/// <summary>
		/// constructure
		/// </summary>
		/// <param name="title"></param>
		/// <param name="sDatabaseFilePath"></param>
		public IssueManager(string sProjectNo, string sProjectPath , CBusinessLogic oBusinessLogic)
		{
			InitializeComponent();

			/// 2011.2.22 added by KM
			lvwGRADE.Columns.Add("Grade");
			lvwGRADE.Columns.Add("Desc.");
			lvwGRADE.Columns[0].Width = 90;
			lvwGRADE.Columns[1].Width = 180;
			lvwREV.Columns.Add("Rev No.");
			lvwREV.Columns[0].Width = 270;

			listISSUE_DATE.Columns.Add("Date");
			listView_GRADE.Columns.Add("Grade");
			listView_GRADE.Columns.Add("Desc.");
			listView_GRADE.Columns[0].Width = 50;
			listView_GRADE.Columns[1].Width = 100;
			listView_ISSUE_DATE.Columns.Add("Date");
			listView_ISSUE_DATE.Columns.Add("Control Date");
			listView_ISSUE_DATE.Columns[0].Width = 100;
			listView_ISSUE_DATE.Columns[1].Width = 100;

			this.Text = "Issue Manager - " + sProjectNo;
			this.BusinessLogic = oBusinessLogic;

            /// Create cancellation and delete folder - 2012.05.16 added by humkyung
			if (!System.IO.Directory.Exists(BusinessLogic.IssuePath + @"\Latest\Cancellation"))
            {
				System.IO.Directory.CreateDirectory(BusinessLogic.IssuePath + @"\Latest\Cancellation");
            }
			if (!System.IO.Directory.Exists(BusinessLogic.IssuePath + @"\Latest\Delete"))
            {
				System.IO.Directory.CreateDirectory(BusinessLogic.IssuePath + @"\Latest\Delete");
            }
            /// up to here

            /// show help for issue manager - 2012.05.29 added by humkyung
            IssueManagerHelpProvider.HelpNamespace = AppPath + @"\Help\SmartISO.chm";
            IssueManagerHelpProvider.SetHelpKeyword(this, "Issue Manager.htm");
            IssueManagerHelpProvider.SetHelpNavigator(this, System.Windows.Forms.HelpNavigator.Topic);
            /// up to here
		}
        
        #region Methods

		/// <summary>
		/// get dataset from database
		/// </summary>
		/// <param name="selectCommand"></param>
		private void GetDataSetFrom(string selectCommand , bool bAddCancellation)
		{
			try
			{
				// Specify a connection string. Replace the given value with a 
				// valid connection string for a Northwind SQL Server sample
				// database accessible to your system.
				string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
				///String connectionString =
				/// "Integrated Security=SSPI;Persist Security Info=False;" +
				///"Initial Catalog=Northwind;Data Source=localhost";

				// Create a new data adapter based on the specified query.
				m_dataAdapter = new OleDbDataAdapter(selectCommand, sConnection);

				// Create a command builder to generate SQL update, insert, and
				// delete commands based on selectCommand. These are used to
				// update the database.
				OleDbCommandBuilder commandBuilder = new OleDbCommandBuilder(m_dataAdapter);

				// Populate a new data table and bind it to the BindingSource.
				m_table = new DataTable();
				m_table.Locale = System.Globalization.CultureInfo.InvariantCulture;
				///m_table.Clear();
				if (true == bAddCancellation)
				{
					DataColumn oColumn = m_table.Columns.Add("C_CANCELLATION", typeof(System.Boolean));
				}
				m_dataAdapter.Fill(m_table);
			}
			catch (OleDbException ex)
			{
				MessageBox.Show(selectCommand + " : " + ex.Message);
			}
		}

		/*
			@brief	fill data
			
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *	
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void IssueManager_Load(object sender, EventArgs e)
		{
			string sQuery = string.Empty;

			try
			{
				int iCol = 0;
				GetDataSetFrom("SELECT * FROM T_PROJECT_SETTING", false);
				foreach (DataColumn dc in m_table.Columns)
				{
					string sValue = m_table.Rows[0].ItemArray[iCol++].ToString();
					BusinessLogic.ProjectSetting[dc.ColumnName] = sValue;

				}

				listPLANT.Items.Add("[all]");
				listAREA.Items.Add("[all]");
				listUNIT.Items.Add("[all]");
				listSECTION.Items.Add("[all]");
				GetDataSetFrom("SELECT DISTINCT C_PLANT_NO,C_AREA_NO,C_UNIT_NO,C_SECTION_NO FROM T_ISSUED_LINE_DATA", false);
				foreach (DataRow dr in m_table.Rows)
				{
					if(null != dr[0]) listPLANT.Items.Add(dr[0]);
					if(null != dr[1]) listAREA.Items.Add(dr[1]);
					if(null != dr[2]) listUNIT.Items.Add(dr[2]);
					if(null != dr[3]) listSECTION.Items.Add(dr[3]);
				}

				/// 2011.2.22 added by KM
				GetDataSetFrom("SELECT C_GRADE_PREFIX,C_GRADE_DESC FROM T_GRADE_DEFINE", false);
				foreach (DataRow dr in m_table.Rows)
				{
					lvwGRADE.Items.Add(dr[0].ToString()).SubItems.Add(dr[1].ToString());
				}

				/// 2011.2.22 added by KM
				string sRevNoList = string.Empty;
				GetDataSetFrom("SELECT DISTINCT C_REV_NO FROM T_ISSUED_REVISION", false);
				foreach (DataRow dr in m_table.Rows)
				{
					sRevNoList += dr[0].ToString() + ",";
				}
				if (string.Empty != sRevNoList)	/// 2011.06.23 added by humkyung
				{
					sRevNoList = sRevNoList.Substring(0, sRevNoList.Length - 1);

					GetDataSetFrom(string.Format("SELECT DISTINCT C_REVISION_NO,C_REVISION_TEXT FROM T_REVISION_STAGE WHERE C_REVISION_NO IN({0})", sRevNoList)
						, false);
					foreach (DataRow dr in m_table.Rows)
					{
						ListViewItem oLVItem = lvwREV.Items.Add(dr[1].ToString());
						if (null != oLVItem)
						{
							oLVItem.Tag = dr[0].ToString();
						}
					}
				}

				GetDataSetFrom("SELECT DISTINCT C_ISSUE_DATE FROM T_ISSUE_DATE", false);
				foreach (DataRow dr in m_table.Rows)
				{
					string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(dr[0].ToString());
					listISSUE_DATE.Items.Add(sDisplayIssueDate);
				}

				//! get revision no from T_ISSUED_REVISION table
				List<string> RevNoList = new List<string> { };
				GetDataSetFrom("SELECT DISTINCT C_REV_NO FROM T_ISSUED_REVISION", false);
				foreach (DataRow dr in m_table.Rows)
				{
					RevNoList.Add(dr[0].ToString());
				}

				comboBox_REVISION_OPTION.Items.Add("LATEST");
				foreach (string s in RevNoList)
				{
					sQuery = "SELECT DISTINCT C_FROM_GRADE_DEFINE,C_REVISION_TEXT FROM T_REVISION_STAGE WHERE C_REVISION_NO=" + s;
					GetDataSetFrom(sQuery, false);
					foreach (DataRow dr in m_table.Rows)
					{
						string sRevNo = dr[0].ToString() + dr[1].ToString();
						comboBox_REVISION_OPTION.Items.Add(sRevNo);
					}
				}

				listISSUE_DATE.Columns[0].Width = 200;
				comboBox_REVISION_OPTION.SelectedIndex = 0;
				listISSUE_DATE.Enabled = checkBox_ISSUE_DATE.Checked;
				lvwGRADE.Enabled = chkGrade.Checked;
				lvwREV.Enabled = chkREV.Checked;
				comboBox_REVISION_OPTION.Enabled = checkBox_ISSUE_DATE.Checked;
				if (chkGrade.Checked && chkREV.Checked && checkBox_ISSUE_DATE.Checked)
				{
					chkALL.Enabled = true;
				}
				else
				{
					chkALL.Enabled = false;
				}

				GetDataSetFrom("SELECT C_GRADE_PREFIX,C_GRADE_DESC FROM T_GRADE_DEFINE", false);
				foreach (DataRow dr in m_table.Rows)
				{
					listView_GRADE.Items.Add(dr[0].ToString()).SubItems.Add(dr[1].ToString());
				}

				GetDataSetFrom("SELECT C_ISSUE_DATE,C_REAL_ISSUE_DATE FROM T_ISSUE_DATE", false);
				foreach (DataRow dr in m_table.Rows)
				{
					string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(dr[0].ToString());
					string sDisplayControlDate = BusinessLogic.GetDisplayIssueDateFrom(dr[1].ToString());
					listView_ISSUE_DATE.Items.Add(sDisplayIssueDate).SubItems.Add(sDisplayControlDate);
				}

				/// 2011.02.19 added by HumKyung
				GetDataSetFrom("SELECT C_REVISION_NO,C_FROM_GRADE_DEFINE,C_REVISION_TEXT FROM T_REVISION_STAGE", false);
				foreach (DataRow dr in m_table.Rows)
				{
					RevisionStage stage = new RevisionStage();
					stage.m_lNo = Convert.ToInt32(dr[0].ToString());
					stage.m_sDefine = dr[1].ToString();
					stage.m_sText = dr[2].ToString();

					BusinessLogic.m_RevisionStageList.Add(stage);
				}

                new DgvFilterPopup.DgvFilterManager(dataGridView_LineData); /// 2013.02.16 added by humkyung
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.StackTrace.ToString());
			}
		}

		/*
		 *	@brief	query issued table with condition
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
			string sGrade = GetSelected_Grade();
			if (string.Empty != sGrade)
			{
				/// 2011.06.13 added by humkyung
				string[] oTokens = sGrade.Split(",".ToCharArray());
				for(int i = 0;i < oTokens.Length;++i)
				{
					oTokens[i] = "'" + ((Convert.ToInt32(oTokens[i])) * 100).ToString() + "'";	//! 2011.02.24 added by HumKyung
				}
				sGrade = string.Join(",", oTokens);
				///
			}
			string sRevNo = GetSelected_RevNo();

			if (string.Empty != sIssueDate || string.Empty != sGrade || string.Empty != sRevNo)
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
					if (string.Empty != sGrade)
					{
						if (0 == chkEmpty)
						{
							sQuery += " C_GRADE IN (" + sGrade + ")";
						}
						else
						{
							sQuery += " AND C_GRADE IN (" + sGrade + ")";
						}
						chkEmpty++;
					}
					if (string.Empty != sGrade)
					{
						if (0 == chkEmpty)
						{
							sQuery += " C_REV_NO IN (" + sRevNo + ")";
						}
						else
						{
							sQuery += " AND C_REV_NO IN (" + sRevNo + ")";
						}
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

				GetDataSetFrom(sQuery, true);
				///bindingSource1.DataSource = m_table;
				///! DataColumn oColumn = new DataColumn();
				///! m_table.Columns.Add("C", typeof(System.Boolean));
				dataGridView_LineData.DataSource = m_table;
				///! Resize the DataGridView columns to fit the newly loaded content.
				dataGridView_LineData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

				for (int i = 1; i < dataGridView_LineData.Columns.Count; ++i)
				{
					dataGridView_LineData.Columns[i].ReadOnly = true;
				}

				int iCancellationCount = 0;
				for (int i = 0; i < dataGridView_LineData.Rows.Count; ++i)
				{
					dataGridView_LineData.Rows[i].Cells[0].Value = dataGridView_LineData.Rows[i].Cells["C_CANCELLATION"].Value;
					object oValue = dataGridView_LineData.Rows[i].Cells[0].Value;
					if ((null != oValue) && (string.Empty != oValue.ToString()) && (true == Convert.ToBoolean(oValue)))
					{
						Font oFont = new Font(dataGridView_LineData.Font, FontStyle.Strikeout);
						for (int j = 1; j < dataGridView_LineData.Columns.Count; ++j)
						{
							dataGridView_LineData.Rows[i].Cells[j].Style.ForeColor = Color.FromArgb(255, 0, 0);
							dataGridView_LineData.Rows[i].Cells[j].Style.Font = oFont;
						}
						++iCancellationCount;
					}
				}
				dataGridView_LineData.Columns[0].Width = 20;

				///
				if (iCancellationCount > 0)
				{
					MessageBox.Show("Cancellation된 Issue 기록이 있습니다", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
			}
		}

		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.17
		 *
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void btnCommit_Click(object sender, EventArgs e)
		{
			string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
            sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
			try
			{
				List<string> columnNameList = new List<string> { };

				object obj = new object { };

				//! dataGridView_LineData Update
				foreach (DataGridViewColumn dc in dataGridView_LineData.Columns)
				{
					columnNameList.Add(dc.Name.ToString());
				}

				m_connection.Open(sConnection, null, null, 0);
				string sCondition = string.Empty;
				foreach (DataGridViewRow dr in dataGridView_LineData.Rows)
				{
					int i = 0;
					string sValueList = string.Empty;
					foreach (DataGridViewCell dc in dr.Cells)
					{
						if ("C_KEY" == columnNameList[i])
						{
							sCondition = "C_KEY='" + dc.Value + "'";
						}
						else
						{
							string sValue = dc.Value.ToString();
							if (sValue.Contains("'"))
							{
								sValue = sValue.Replace("'", "''");
							}

							sValueList += columnNameList[i] + "='" + sValue + "',";
						}
						i++;
					}
					sValueList = sValueList.TrimEnd(",".ToCharArray());

					string sCommand = "UPDATE T_ISSUED_LINE_DATA SET " + sValueList + " WHERE " + sCondition;
					try
					{
						m_connection.Execute(sCommand, out obj, 0);
					}
					catch (OleDbException ex)
					{
						MessageBox.Show(ex.Errors.ToString());
					}
				}

				//! dataGridView_RevisionData Update
				columnNameList.Clear();

				foreach (DataGridViewColumn dc in dataGridView_RevisionData.Columns)
				{
					columnNameList.Add(dc.Name.ToString());
				}

				sCondition = string.Empty;
				string sConditionRevNo = string.Empty;

				foreach (DataGridViewRow dr in dataGridView_RevisionData.Rows)
				{
					int i = 0;
					string sValueList = string.Empty;
					foreach (DataGridViewCell dc in dr.Cells)
					{
						if ("C_KEY" == columnNameList[i])
						{
							sCondition = "C_KEY='" + dc.Value + "'";
						}
						else if ("C_REV_NO" == columnNameList[i])
						{
							sConditionRevNo = "C_REV_NO='" + dc.Value + "'";
						}
						else
						{
							string sValue = dc.Value.ToString();
							if (sValue.Contains("'"))
							{
								sValue = sValue.Replace("'", "''");
							}

							sValueList += columnNameList[i] + "='" + sValue + "',";
						}
						i++;
					}
					sValueList = sValueList.TrimEnd(",".ToCharArray());

					string sCommand = "UPDATE T_ISSUED_REVISION SET " + sValueList + " WHERE " + sCondition + " AND " + sConditionRevNo;
					try
					{
						m_connection.Execute(sCommand, out obj, 0);
					}
					catch (OleDbException ex)
					{
						MessageBox.Show(ex.Errors.ToString());
					}
				}

				m_connection.Close();
			}
			catch (OleDbException ex)
			{
				MessageBox.Show(ex.Errors.ToString());
				m_connection.Close();
			}

		}

		private void btnCancel_Click(object sender, EventArgs e)
		{
			///this.Close();
		}

		private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
		{

		}

		private void checkBox_ISSUE_DATE_CheckedChanged(object sender, EventArgs e)
		{
			listISSUE_DATE.Enabled = checkBox_ISSUE_DATE.Checked;
			comboBox_REVISION_OPTION.Enabled = checkBox_ISSUE_DATE.Checked;
			if (chkGrade.Checked && chkREV.Checked && checkBox_ISSUE_DATE.Checked)
			{
				chkALL.Enabled = true;
			}
			else
			{
				chkALL.Enabled = false;
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
			else if(sString.Length > 0)
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
				if(string.Empty == sItemText) continue;

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
				GetDataSetFrom(sQuery, false);
			}
			else if (string.Empty != sCondition)
			{
				sQuery += " WHERE C_PLANT_NO IN (" + sCondition + ")";
				GetDataSetFrom(sQuery, false);
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
		private void listAREA_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listAREA.SelectedIndex)
			{
				OnDblclkQueryItem(ref listAREA, listAREA.SelectedIndex);
				UpdateUnitNoListBox();
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

			GetDataSetFrom(sQuery, false);
			for (int i = listUNIT.Items.Count - 1; i >= 1; --i)
			{
				listUNIT.Items.RemoveAt(i);
			}
			foreach (DataRow dr in m_table.Rows)
			{
				listUNIT.Items.Add(dr[0].ToString());
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
		private void listUNIT_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listUNIT.SelectedIndex)
			{
				OnDblclkQueryItem(ref listUNIT, listUNIT.SelectedIndex);
				UpdateSectionNoListBox();
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

			GetDataSetFrom(sQuery, false);
			for (int i = listSECTION.Items.Count - 1; i >= 1; --i)
			{
				listSECTION.Items.RemoveAt(i);
			}
			foreach (DataRow dr in m_table.Rows)
			{
				listSECTION.Items.Add(dr[0].ToString());
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
		private void listSECTION_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (-1 != listSECTION.SelectedIndex)
			{
				OnDblclkQueryItem(ref listSECTION, listSECTION.SelectedIndex);
			}
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
						BusinessLogic.ParseIssueDate(ref dYear, ref dMonth, ref dDay, listISSUE_DATE.Items[i].SubItems[0].Text);
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
		private string GetSelected_Grade()
		{
			string sIssueGrade = string.Empty;

			if (chkGrade.Checked)
			{
				for (int i = 0; i < lvwGRADE.Items.Count; ++i)
				{
					if (lvwGRADE.Items[i].Checked)
					{
						sIssueGrade += lvwGRADE.Items[i].SubItems[0].Text + ",";
					}
				}

				if ((string.Empty != sIssueGrade) && ("," == sIssueGrade.Substring(sIssueGrade.Length - 1, 1)))
				{
					sIssueGrade = sIssueGrade.Substring(0, sIssueGrade.Length - 1);
				}
			}

			return sIssueGrade;
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
		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *
		 *	@return
		 */
		private string GetSelectedRevNo()
		{
			string sSelectedRevNo = string.Empty;

			if (checkBox_ISSUE_DATE.Checked)
			{
			}

			return sSelectedRevNo;
		}

		private void IssueManager_ResizeEnd(object sender, EventArgs e)
		{
			dataGridView_LineData.Width = this.Size.Width - dataGridView_LineData.Left - 20;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IssueManager_SizeChanged(object sender, EventArgs e)
		{
            ///groupBox_Select.Height = this.ClientSize.Height - groupBox_Select.Top - 10;

            groupBox_RevisionData.Top = this.ClientSize.Height - groupBox_RevisionData.Height + 10;
            groupBox_RevisionData.Height = groupBox_RevisionData.Height - 15;
            groupBox_NextGrade.Top = this.ClientSize.Height - groupBox_NextGrade.Height + 10;
            groupBox_NextGrade.Height = groupBox_NextGrade.Height - 15;
            groupBox_IssueDate.Top = this.ClientSize.Height - groupBox_IssueDate.Height + 10;
            groupBox_IssueDate.Height = groupBox_IssueDate.Height - 15;
            groupBox_IssueDate.Left = this.ClientSize.Width - groupBox_IssueDate.Width - 10;
            groupBox_NextGrade.Left = groupBox_IssueDate.Left - groupBox_NextGrade.Width - 10;
            groupBox_RevisionData.Width = groupBox_NextGrade.Left - groupBox_RevisionData.Left - 10;
            dataGridView_RevisionData.Width = groupBox_RevisionData.Width - 15;
            
		}

		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void listView_GRADE_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (1 == listView_GRADE.SelectedIndices.Count)
			{
				string sSelItemText = listView_GRADE.Items[listView_GRADE.SelectedIndices[0]].SubItems[0].Text;
				groupBox_NextGrade.Text = "Next grade(" + sSelItemText + ")";
			}
		}

		/*
		 *	@brief
		 *	
		 *	@author	HumKyung
		 *	
		 *	@date	2011.02.15
		 *
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void listView_ISSUE_DATE_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (chkINSERT.Checked == false) return;
			if ((1 == listView_GRADE.SelectedItems.Count) && (1 == listView_ISSUE_DATE.SelectedItems.Count))
			{
				Insert_RevisionData(chkALL.Checked);
			}
		}

		private void dataGridView_LineData_CellContentDoubleClick(object sender, DataGridViewCellEventArgs e)
		{
			string sKey = dataGridView_LineData.Rows[e.RowIndex].Cells[0].Value.ToString();
			string sQuery = "SELECT * FROM T_ISSUED_REVISION WHERE C_KEY='" + sKey + "' ORDER BY C_REV_NO";
			GetDataSetFrom(sQuery, false);
			///bindingSource1.DataSource = m_table;
			dataGridView_RevisionData.DataSource = m_table;
			// Resize the DataGridView columns to fit the newly loaded content.
			dataGridView_RevisionData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
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
            GetDataSetFrom(sQuery, false);
            ///bindingSource1.DataSource = m_table;
            dataGridView_RevisionData.DataSource = m_table;
            // Resize the DataGridView columns to fit the newly loaded content.
            dataGridView_RevisionData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

            //! 2011.02.24 added by HumKyung
            foreach (DataGridViewRow row in dataGridView_RevisionData.Rows)
            {
                int iGrade = Convert.ToInt32(row.Cells["C_GRADE"].Value.ToString()) / 100;
                row.Cells["C_GRADE"].Value = iGrade.ToString();
                int iRevNo = Convert.ToInt32(row.Cells["C_REV_NO"].Value.ToString());
                foreach (RevisionStage rs in BusinessLogic.m_RevisionStageList)
                {
                    if (iRevNo == rs.m_lNo)
                    {
                        row.Cells["C_REV_NO"].Tag = row.Cells["C_REV_NO"].Value;
                        row.Cells["C_REV_NO"].Value = rs.m_sDefine + rs.m_sText;
                        break;
                    }
                }

                /// 2011.08.06 added by humkyung
                string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(row.Cells["C_REV_DATE"].Value.ToString());
                row.Cells["C_REV_DATE"].Value = sDisplayIssueDate;
                sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(row.Cells["C_REV_CONTROL_WORKING_DATE"].Value.ToString());
                row.Cells["C_REV_CONTROL_WORKING_DATE"].Value = sDisplayIssueDate;
                /// up to here

            }

            //! 2011.02.19 added by HumKyung
            for (int i = 0; i < dataGridView_RevisionData.Columns.Count; i++)
            {
                dataGridView_RevisionData.Columns[i].ReadOnly = true;
                dataGridView_RevisionData.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable; /// 2012.05.18 added by humkyung
            }
        }

		///<summary>
		///KEY값을 가진 ISSUED REVISION DATA를 표시한다.
		///</summary>
		///<date>2011.02.18</date>
		///<param name="sender" />
		///<param name="e" />
		private void dataGridView_LineData_CurrentCellChanged(object sender, EventArgs e)
		{
			if (null != dataGridView_LineData.CurrentCell)
			{
				int iRowIndex = dataGridView_LineData.CurrentCell.RowIndex;
				if (m_iCurRowIndex != iRowIndex)
				{
                    m_iCurRowIndex = iRowIndex;

					BusinessLogic.m_sKey = dataGridView_LineData.Rows[iRowIndex].Cells["C_KEY"].Value.ToString();
                    DisplayRevisionDataWith(BusinessLogic.m_sKey);
				}
			}
		}

		/*
		 *	@brief Selected Rows Delete from T_ISSUED_LINE_DATA
		 *	
		 *	@author	Kyoungmin
		 *	
		 *	@date	2011.02.18
		 *
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void btnDelete_Click(object sender, EventArgs e)
		{
			int nCnt = dataGridView_LineData.SelectedRows.Count;
			if (0 == nCnt)
			{
				MessageBox.Show("First of all, select datas to delete.");
				return;
			}

			if (DialogResult.No == MessageBox.Show("Do you want to delete?", "Issue Manager", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
			{
				return;
			}

			List<string> columnNameList = new List<string> { };

			object obj = new object { };
			foreach (DataGridViewColumn dc in dataGridView_LineData.Columns)
			{
				columnNameList.Add(dc.Name.ToString());
			}

			try
			{
				string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
				m_connection.Open(sConnection, null, null, 0);

				string sCondition = string.Empty;

				foreach (DataGridViewRow dr in dataGridView_LineData.SelectedRows)
				{
					int i = 0;
					foreach (DataGridViewCell dc in dr.Cells)
					{
						if ("C_KEY" == columnNameList[i])
						{
							sCondition = dc.Value.ToString();
							break;
						}
						i++;
					}

					/// T_ISSUED_LINE_DATA Delete
					string sCommand = "DELETE * FROM T_ISSUED_LINE_DATA " + " WHERE C_KEY='" + sCondition + "'";
					try
					{
						m_connection.Execute(sCommand, out obj, 0);

                        /// move a file to delete folder - 2012.05.16 added by humkyung
                        string sSrcIsoFilePath = BusinessLogic.IssuePath;
                        string sDestIsoFilePath = string.Empty;
                        string sIsoFileName = GetIsoOutputFileName(dr.Index);
                        string sIsoFileExt = GetIsoOutputFileExt(dr.Index);
                        sSrcIsoFilePath += (@"\Latest\" + sIsoFileName + "." + sIsoFileExt);
                        /// up to here
						/// delete source iso file - 2012.09.25 added by humkyung
						if (System.IO.File.Exists(sSrcIsoFilePath))
						{
							System.IO.File.Delete(sSrcIsoFilePath);
						}
					}
					catch (OleDbException ex)
					{
						MessageBox.Show(ex.Errors.ToString());
					}

					///! T_ISSUED_REVISION Delete
					sCommand = "DELETE * FROM T_ISSUED_REVISION " + " WHERE C_KEY='" + sCondition + "'";
					try
					{
						m_connection.Execute(sCommand, out obj, 0);
					}
					catch (OleDbException ex)
					{
						MessageBox.Show(ex.Errors.ToString());
					}
				}
				m_connection.Close();

				///! display data set
				{
					GetDataSetFrom("SELECT * FROM T_ISSUED_LINE_DATA", true);
					///bindingSource1.DataSource = m_table;
					///! DataColumn oColumn = new DataColumn();
					///! m_table.Columns.Add("C", typeof(System.Boolean));
					dataGridView_LineData.DataSource = m_table;
					///! Resize the DataGridView columns to fit the newly loaded content.
					dataGridView_LineData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

					for (int i = 1; i < dataGridView_LineData.Columns.Count; ++i)
					{
						dataGridView_LineData.Columns[i].ReadOnly = true;
					}

					int iCancellationCount = 0;
					for (int i = 0; i < dataGridView_LineData.Rows.Count; ++i)
					{
						dataGridView_LineData.Rows[i].Cells[0].Value = dataGridView_LineData.Rows[i].Cells["C_CANCELLATION"].Value;
						object oValue = dataGridView_LineData.Rows[i].Cells[0].Value;
						if ((null != oValue) && (string.Empty != oValue.ToString()) && (true == Convert.ToBoolean(oValue)))
						{
							Font oFont = new Font(dataGridView_LineData.Font, FontStyle.Strikeout);
							for (int j = 1; j < dataGridView_LineData.Columns.Count; ++j)
							{
								dataGridView_LineData.Rows[i].Cells[j].Style.ForeColor = Color.FromArgb(255, 0, 0);
								dataGridView_LineData.Rows[i].Cells[j].Style.Font = oFont;
							}
							++iCancellationCount;
						}
					}
					dataGridView_LineData.Columns[0].Width = 20;
				}
			}
			catch(Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
		}

		/*
		 *	@brief Selected Rows Delete from T_ISSUED_REVISION
		 *	
		 *	@author	Kyoungmin
		 *	
		 *	@date	2011.02.18
		 *
		 *	@param	sender
		 *	@param	e
		 *	
		 *	@return
		 */
		private void btnRevDelete_Click(object sender, EventArgs e)
		{
			string sRes = MessageBox.Show("Do you want to delete?", "Revision Data", MessageBoxButtons.YesNo, MessageBoxIcon.Question).ToString();
			if ("No" == sRes) return;

			int nCnt = dataGridView_RevisionData.SelectedRows.Count;
			if (0 == nCnt)
			{
				MessageBox.Show("At least select 1 Row.");
				return;
			}

			List<string> columnNameList = new List<string> { };
			object obj = new object { };

			foreach (DataGridViewColumn dc in dataGridView_RevisionData.Columns)
			{
				columnNameList.Add(dc.Name.ToString());
			}

			string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
            sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
			string sCondition = string.Empty;
			string sKey = string.Empty;

			m_connection.Open(sConnection, null, null, 0);
			foreach (DataGridViewRow dr in dataGridView_RevisionData.SelectedRows)
			{
				int i = 0;
				foreach (DataGridViewCell dc in dr.Cells)
				{
					if ("C_KEY" == columnNameList[i])
					{
						sKey = dc.Value.ToString();
					}
					if ("C_REV_NO" == columnNameList[i])
					{
						sCondition = "C_REV_NO='" + dc.Tag + "'";
					}
					i++;
				}

				string sCommand = "DELETE * FROM T_ISSUED_REVISION " + " WHERE " + sCondition + " AND C_KEY='" + sKey + "'";
				try
				{
					m_connection.Execute(sCommand, out obj, 0);
				}
				catch (OleDbException ex)
				{
					MessageBox.Show(ex.Errors.ToString());
				}
			}
			m_connection.Close();

            DisplayRevisionDataWith(sKey);
		}

		private void chkModify_CheckedChanged(object sender, EventArgs e)
		{
			if (chkModify.Checked == true)
			{
				for (int i = 1; i < dataGridView_LineData.Columns.Count; i++)
				{
					dataGridView_LineData.Columns[i].ReadOnly = false;
				}
			}
			else
			{
				for (int i = 1; i < dataGridView_LineData.Columns.Count; i++)
				{
					dataGridView_LineData.Columns[i].ReadOnly = true;
				}
			}
		}

		private void chkGrade_CheckedChanged(object sender, EventArgs e)
		{
			lvwGRADE.Enabled = chkGrade.Checked;
			if (chkGrade.Checked && chkREV.Checked && checkBox_ISSUE_DATE.Checked)
			{
				chkALL.Enabled = true;
			}
			else
			{
				chkALL.Enabled = false;
			}
		}

		private void chkREV_CheckedChanged(object sender, EventArgs e)
		{
			lvwREV.Enabled = chkREV.Checked;
			if (chkGrade.Checked && chkREV.Checked && checkBox_ISSUE_DATE.Checked)
			{
				chkALL.Enabled = true;
			}
			else
			{
				chkALL.Enabled = false;
			}
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="bCheckAll"></param>
		private void Insert_RevisionData(bool bCheckAll)
		{
			int index = Application.ExecutablePath.LastIndexOf("\\");
			string sLogPath = Application.ExecutablePath.Substring(0, index) + "\\Loglist.txt";
			string[] sLogList;
			string sLogTmp = "";
			int nLogCnt = 0;

			string[] sKey;
			if (bCheckAll && chkALL.Enabled)
			{
				sKey = new string[dataGridView_LineData.Rows.Count];
				for (int i = 0; i < dataGridView_LineData.Rows.Count; i++)
				{
					sKey[i] = dataGridView_LineData[0, i].Value.ToString();
				}
			}
			else
			{
				sKey = new string[1];
				sKey[0] = BusinessLogic.m_sKey;
			}

			for (int i = 0; i < sKey.Length; i++)
			{
				string sSelControlDate = listView_ISSUE_DATE.SelectedItems[0].SubItems[1].Text;
				if (BusinessLogic.IsBeforeCurrentDate(sSelControlDate))
				{
					sLogTmp += sKey[i] + " 날짜가 지났습니다." + "^";
					nLogCnt++;
					continue;
				}

				string sSelGrade = listView_GRADE.SelectedItems[0].SubItems[0].Text;
				string sSelGradeDesc = listView_GRADE.SelectedItems[0].SubItems[1].Text;

				string sQuery = "SELECT C_REV_NO, C_REV_DATE FROM T_ISSUED_REVISION WHERE C_KEY='" + sKey[i] + "'";
				GetDataSetFrom(sQuery, false);

				string sLastRevNo = "0";
				string sLastRevDate = string.Empty;
				string sSelRevDate = listView_ISSUE_DATE.SelectedItems[0].SubItems[0].Text;

				foreach (DataRow dr in m_table.Rows)
				{
					if (Convert.ToInt32(sLastRevNo) < Convert.ToInt32(dr[0].ToString()))
					{
						sLastRevNo = dr[0].ToString();  //Revision No
						sLastRevDate = dr[1].ToString();//Revision No를 기준으로 Revation Date를 가져옴
					}
				}

				if (BusinessLogic.IsBeforeLastRevDate(sLastRevDate, sSelRevDate))
				{
					//MessageBox.Show("최종 REV. 날짜보다 이전 날짜는 입력할 수 없습니다.", "에러");
					sLogTmp += sKey[i] + " 최종 REV. 날짜보다 이전 날짜는 입력할 수 없습니다." + "^";
					nLogCnt++;
					continue;
				}

				if (!BusinessLogic.CheckSelectedGrade(Convert.ToInt32(sLastRevNo), Convert.ToInt32(sSelGrade)))
				{
					//MessageBox.Show("Select appropriate grade!!!");
					sLogTmp += sKey[i] + " Select appropriate grade!!!" + "^";
					nLogCnt++;
					continue;
				}

				RevisionStage stage = new RevisionStage();
				if (BusinessLogic.GetNextRevisionStage(ref stage, Convert.ToInt32(sLastRevNo), Convert.ToInt32(sSelGrade)))
				{
					string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                    sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
					try
					{
						m_connection.Open(sConnection, null, null, 0);

						string sFields = "C_KEY,C_REV_DRWN_BY,C_REV_CHECKED_BY,C_REV_APPROVED_BY,C_REV_DATE,";
						sFields += "C_GRADE,C_REV_NO,C_REV_CONTROL_WORKING_DATE,C_REV_DESC";

						string sValues = "'" + sKey[i] + "',";
						sValues += "'" + BusinessLogic.ProjectSetting["C_REV_DRWN_BY"] + "',";
						sValues += "'" + BusinessLogic.ProjectSetting["C_REV_CHECKED_BY"] + "',";
						sValues += "'" + BusinessLogic.ProjectSetting["C_REV_APPROVED_BY"] + "',";
						
						/// 2011.08.06 added by humkyung
						Int32 dYear = 0, dMonth = 0, dDay = 0;
						BusinessLogic.ParseIssueDate(ref dYear, ref dMonth, ref dDay , sSelRevDate);
						sSelRevDate = string.Format("{0:0000}-{1:00}-{2:00}" , dYear , dMonth , dDay);
						/// up to here
						sValues += "'" + sSelRevDate + "',";
						sValues += "'" + sSelGrade + "',";
						sValues += "'" + stage.m_lNo.ToString() + "',";
						/// 2011.08.06 added by humkyung
						BusinessLogic.ParseIssueDate(ref dYear, ref dMonth, ref dDay, sSelControlDate);
						sSelControlDate = string.Format("{0:0000}-{1:00}-{2:00}", dYear, dMonth, dDay);
						/// up to here
						sValues += "'" + sSelControlDate + "',";
						sValues += "'" + sSelGradeDesc + "'";

						string sCommand = "INSERT INTO T_ISSUED_REVISION(" + sFields + ") VALUES(" + sValues + ")";
						object obj = new object();
						m_connection.Execute(sCommand, out obj, 0);
						m_connection.Close();
					}
					catch (Exception ex)
					{
						m_connection.Close();
						MessageBox.Show(ex.Message.ToString());
					}
				}
			}
			// Revision Data 창 Refresh
			string sQuery2 = "SELECT * FROM T_ISSUED_REVISION WHERE C_KEY='" + BusinessLogic.m_sKey + "'";
			GetDataSetFrom(sQuery2, false);
			///bindingSource1.DataSource = m_table;
			dataGridView_RevisionData.DataSource = m_table;
			dataGridView_RevisionData.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);

			for (int i = 0; i < dataGridView_RevisionData.Columns.Count; i++)
			{
				dataGridView_RevisionData.Columns[i].ReadOnly = true;
			}

			// Error List 띄우기
			if (string.Empty != sLogTmp.Trim())
			{
				sLogList = new string[nLogCnt];
				sLogList = sLogTmp.Split(new Char[] { '^' });
				File.WriteAllLines(sLogPath, sLogList, Encoding.UTF8);
				System.Diagnostics.Process.Start(sLogPath);
			}
		}

		/// <summary>
		/// get issue date
		/// </summary>
		/// <param name="sKey"></param>
		/// <param name="iRevNo">get the last revision number if iRevNo is -1</param>
		/// <returns></returns>
		private string GetRevIssueDateAtIndex(string sKey , int iRevNo)
		{
			string sIssueDate = string.Empty;

			/// store all issued revision data - 2011.08.02 added by humkyung
			if (0 == RevDataDic.Count)
			{
				string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
				string sCondition = string.Empty;

				string sCommand = "SELECT C_KEY,C_REV_DATE FROM T_ISSUED_REVISION";
				GetDataSetFrom(sCommand, false);
				for(int i = 0;i < m_table.Rows.Count;++i)
				{
					string _sKey = m_table.Rows[i].ItemArray[0].ToString();
					if(false == RevDataDic.ContainsKey(_sKey))
					{
						RevDataDic[_sKey] = new List<string>();
					}

					string sDisplayIssueDate = BusinessLogic.GetDisplayIssueDateFrom(m_table.Rows[i].ItemArray[1].ToString());
					RevDataDic[_sKey].Add(sDisplayIssueDate);
				}	
			}

			if(true == RevDataDic.ContainsKey(sKey))
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
        private void SetCancellationCell(ExcelWorksheet oWorksheet , int row, int col)
        {
            oWorksheet.Cells[row, col].Style.Font.Color = Color.Red;
            oWorksheet.Cells[row, col].Style.Font.Strikeout = true;
        }

		/// <summary>
		/// export data to excel file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.??.??</date>
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
                        string sXlsFilePath = Path.Combine(BusinessLogic.ProjectPath , @"Setting\ISOPrintTemp.xls");
                        ef.LoadXls(sXlsFilePath , XlsOptions.PreserveAll);
					
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
                                    if (bCancellation) SetCancellationCell(oWorksheet , row, iNo);
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
											
                                            if (bCancellation) SetCancellationCell(oWorksheet , row, col);
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
                    catch(GemBox.Spreadsheet.SpreadsheetException ex)
                    {
                        AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR , ex.Message);
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
				if (DialogResult.Yes == MessageBox.Show("Export is complete\n Will you open excel file?", "Message", MessageBoxButtons.YesNo , MessageBoxIcon.Question))
				{
					System.Diagnostics.Process.Start(sExportFilePath);
				}
			}

			AppDocData.frameworkCore.Logger.ShowErrorResult(this);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void dataGridView_LineData_CurrentCellDirtyStateChanged(object sender, EventArgs e)
		{
			if (dataGridView_LineData.IsCurrentCellDirty)
			{
				dataGridView_LineData.CommitEdit(DataGridViewDataErrorContexts.Commit);
			}
		}

        /// <summary>
        /// return iso output file name
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.05.18</date>
        /// <param name="row"></param>
        /// <returns></returns>
        private string GetIsoOutputFileName(int row)
        {
            string res = string.Empty;

            string[] oOutputIsoFileNameFormat = BusinessLogic.GetOutputIsoFileNameFormat();
            for (int i = 0; i < oOutputIsoFileNameFormat.Length; ++i)
            {
                bool bFound = false;
                foreach (DataGridViewColumn oColumn in dataGridView_LineData.Columns)
                {
                    if (oOutputIsoFileNameFormat[i] == oColumn.Name)
                    {
                        res += dataGridView_LineData[oColumn.Name, row].Value;
                        bFound = true;
                        break;
                    }
                }

                if (false == bFound) res += oOutputIsoFileNameFormat[i];
            }

            return res;
        }

        /// <summary>
        /// return iso output file ext
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.05.18</date>
        /// <param name="row"></param>
        /// <returns></returns>
        private string GetIsoOutputFileExt(int row)
        {
            string res = string.Empty;

            string[] oOutputIsoFileExtFormat = BusinessLogic.GetOutputIsoFileExtFormat();
            for (int i = 0; i < oOutputIsoFileExtFormat.Length; ++i)
            {
                bool bFound = false;
                foreach(DataGridViewColumn oColumn in dataGridView_RevisionData.Columns)
                {
                    if (oOutputIsoFileExtFormat[i] == oColumn.Name)
                    {
                        res += dataGridView_RevisionData[oColumn.Name, dataGridView_RevisionData.Rows.Count - 1].Value;
                        bFound = true;
                        break;
                    }
                }

                if(false == bFound)
                {
                    foreach (DataGridViewColumn oColumn in dataGridView_LineData.Columns)
                    {
                        if (oOutputIsoFileExtFormat[i] == oColumn.Name)
                        {
                            res += dataGridView_LineData[oColumn.Name, row].Value;
                            bFound = true;
                            break;
                        }
                    }
                }

                if (false == bFound) res += oOutputIsoFileExtFormat[i];
            }

            return res;
        }

		/// <summary>
		/// run cancellation
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void dataGridView_LineData_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			if (0 == e.ColumnIndex)
			{
				bool bValue = ((bool)dataGridView_LineData[e.ColumnIndex, e.RowIndex].Value);
				if (true == bValue)
				{
					Font oFont = new Font(dataGridView_LineData.Font, FontStyle.Strikeout);
					for (int j = 1; j < dataGridView_LineData.Columns.Count; ++j)
					{
						dataGridView_LineData.Rows[e.RowIndex].Cells[j].Style.ForeColor = Color.FromArgb(255, 0, 0);
						dataGridView_LineData.Rows[e.RowIndex].Cells[j].Style.Font = oFont;
					}
				}
				else
				{
					for (int j = 1; j < dataGridView_LineData.Columns.Count; ++j)
					{
						dataGridView_LineData.Rows[e.RowIndex].Cells[j].Style.ForeColor = Color.FromArgb(0, 0, 0);
						dataGridView_LineData.Rows[e.RowIndex].Cells[j].Style.Font = null;
					}
				}
				dataGridView_LineData.Invalidate();

				///! 
				string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
				string sCondition = string.Empty;
				string sKey = dataGridView_LineData[1, e.RowIndex].Value.ToString();

				try
				{
					m_connection.Open(sConnection, null, null, 0);
					string sCommand = string.Format("UPDATE T_ISSUED_LINE_DATA SET C_CANCELLATION={0} WHERE C_KEY='{1}'", ((true == bValue) ? "1" : "0"), sKey);
					try
					{
						object obj;
						m_connection.Execute(sCommand, out obj, 0);

                        /// copy cancellation file - 2012.05.16 added by humkyung
                        string sSrcIsoFilePath = BusinessLogic.IssuePath;
                        string sDestIsoFilePath= string.Empty;
                        string sIsoFileName = GetIsoOutputFileName(e.RowIndex);
                        string sIsoFileExt = GetIsoOutputFileExt(e.RowIndex);
                        if (true == bValue)
                        {
                            sSrcIsoFilePath += (@"\Latest\" + sIsoFileName + "." + sIsoFileExt);
                            sDestIsoFilePath = BusinessLogic.IssuePath + @"\Latest\Cancellation\" + sIsoFileName + "." + sIsoFileExt;
                        }
                        else
                        {
                            sSrcIsoFilePath += (@"\Latest\Cancellation\" + sIsoFileName + "." + sIsoFileExt);
                            sDestIsoFilePath = BusinessLogic.IssuePath + @"\Latest\" + sIsoFileName + "." + sIsoFileExt;
                        }
                        
                        if (System.IO.File.Exists(sSrcIsoFilePath))
                        {
                            System.IO.File.Move(sSrcIsoFilePath, sDestIsoFilePath);
                        }
                        /// up to here
					}
					catch (OleDbException ex)
					{
						MessageBox.Show(ex.Errors.ToString());
					}
				}
				catch
				{
				}
				m_connection.Close();
			}
		}

		/// <summary>
		/// when issue manager is shown
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.??.??</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void IssueManager_Shown(object sender, EventArgs e)
		{
		}

		private void UpdateRevisionDataInRemoteDatabase(MyOracleConnection conn , string sKey)
		{
			try
			{
				// Specify a connection string. Replace the given value with a 
				// valid connection string for a Northwind SQL Server sample
				// database accessible to your system.
				string sConnection = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + BusinessLogic.DatabasePath;
                sConnection += ";Jet OLEDB:Database Password=gopds";	/// 2012.01.27 added by humkyung
				///String connectionString =
				/// "Integrated Security=SSPI;Persist Security Info=False;" +
				///"Initial Catalog=Northwind;Data Source=localhost";

				string sSql = "SELECT * FROM T_ISSUED_REVISION WHERE C_KEY='" + sKey + "'";
				// Create a new data adapter based on the specified query.
				OleDbDataAdapter oDataAdapter = new OleDbDataAdapter(sSql, sConnection);

				// Create a command builder to generate SQL update, insert, and
				// delete commands based on selectCommand. These are used to
				// update the database.
				OleDbCommandBuilder oCommandBuilder = new OleDbCommandBuilder(oDataAdapter);

				// Populate a new data table and bind it to the BindingSource.
				DataTable oDataTable = new DataTable();
				oDataTable.Locale = System.Globalization.CultureInfo.InvariantCulture;
				oDataAdapter.Fill(oDataTable);
				
				foreach(DataRow oRow in oDataTable.Rows)
				{
					sSql = "UPDATE T_ISSUED_REVISION SET ";
					string sFields = string.Empty, sValues = string.Empty;
					foreach(DataColumn oColumn in oDataTable.Columns)
					{
						object oValue = oRow[oColumn];
						if ("C_KEY" != oColumn.ColumnName)
						{
							string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
							if (string.Empty != sValue)
							{
								sValue = sValue.Replace("'" , "''");
								sSql += oColumn.ColumnName + "='" + sValue + "',";
								sFields += oColumn.ColumnName + ",";
								sValues += "'" + sValue + "',";
							}
						}
						else
						{
							sFields += oColumn.ColumnName + ",";
							sValues += "'" + sKey + "',";
						}
					}

					if (string.Empty != sKey)
					{
						sSql = sSql.TrimEnd(",".ToCharArray());
						sSql += " WHERE C_KEY='" + sKey + "'";
						int rows = 0;
						using (OracleCommand comm = new OracleCommand(sSql, conn.Connection))
						{
							rows = comm.ExecuteNonQuery();
						}

						if (0 == rows)
						{
							sFields = sFields.TrimEnd(",".ToCharArray());
							sValues = sValues.TrimEnd(",".ToCharArray());
							sSql = "INSERT INTO T_ISSUED_REVISION(" + sFields + ") VALUES(" + sValues + ")";
							using (OracleCommand comm = new OracleCommand(sSql, conn.Connection))
							{
								comm.ExecuteNonQuery();
							}
						}
					}
				}
			}
			catch (OleDbException)
			{
				///MessageBox.Show(selectCommand + " : " + ex.Message);
			}
		}

		/// <summary>
		/// update remote database
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		public void UpdateRemoteDatabase()
		{
			using (MyOracleConnection conn = new MyOracleConnection())
			{
				if (true == conn.Connect(BusinessLogic.RemoteDatabaseSetting.sHost , BusinessLogic.RemoteDatabaseSetting.sService, BusinessLogic.RemoteDatabaseSetting.sId , BusinessLogic.RemoteDatabaseSetting.sPassword))
				{
					foreach (DataGridViewRow oRow in this.dataGridView_LineData.Rows)
					{
						string sSql = "UPDATE T_ISSUED_LINE_DATA SET " , sKey = string.Empty;
						string sFields = string.Empty, sValues = string.Empty;
						foreach (DataGridViewCell oCell in oRow.Cells)
						{
							if ("C_KEY" != oCell.OwningColumn.Name)
							{
								object oValue = oCell.Value;
								string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
								if (string.Empty != sValue)
								{
									sValue = sValue.Replace("'" , "''");
									sSql += oCell.OwningColumn.Name + "='" + sValue + "',";
									sFields += oCell.OwningColumn.Name + ",";
									sValues += "'" + sValue + "',";
								}
							}
							else
							{
								object oValue = oCell.Value;
								sKey = (null != oValue) ? oValue.ToString() : string.Empty;
								sFields += oCell.OwningColumn.Name + ",";
								sValues += "'" + sKey + "',";
							}
						}

						if (string.Empty != sKey)
						{
							sSql = sSql.TrimEnd(",".ToCharArray());
							sSql += " WHERE C_KEY='" + sKey + "'";
							int rows = 0;
							using (OracleCommand comm = new OracleCommand(sSql, conn.Connection))
							{
								rows = comm.ExecuteNonQuery();
							}

							if (0 == rows)
							{
								sFields = sFields.TrimEnd(",".ToCharArray());
								sValues = sValues.TrimEnd(",".ToCharArray());
								sSql = "INSERT INTO T_ISSUED_LINE_DATA(" + sFields + ") VALUES(" + sValues + ")";
								using (OracleCommand comm = new OracleCommand(sSql, conn.Connection))
								{
									comm.ExecuteNonQuery();
								}
							}

							this.UpdateRevisionDataInRemoteDatabase(conn, sKey);
						}
					}
				}
			}
		}

		/// <summary>
		/// transfer iso dwg file to ftp server
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.27</date>
		public void TransferIsoDwgToFtpServer()
		{
			FTPClient ftp = null;

			try
			{
				ftp = new FTPClient(AppDocData.FtpServerSetting.sHost);
				ftp.Login(AppDocData.FtpServerSetting.sId, AppDocData.FtpServerSetting.sPassword);
				ftp.ConnectMode = FTPConnectMode.PASV;
				ftp.TransferType = FTPTransferType.BINARY;

				foreach (DataGridViewRow oRow in this.dataGridView_LineData.Rows)
				{
					string sSrcIsoFilePath = BusinessLogic.IssuePath;
					string sIsoFileName = GetIsoOutputFileName(oRow.Index);
					string sIsoFileExt = GetIsoOutputFileExt(oRow.Index);
					sSrcIsoFilePath += (@"\Latest\" + sIsoFileName + "." + sIsoFileExt);

					try
					{
						string sRemoteFilePath = AppDocData.FtpServerSetting.sRemoteDirectory;
						if ((string.Empty != sRemoteFilePath) && ('\\' != sRemoteFilePath[sRemoteFilePath.Length - 1])) sRemoteFilePath += "\\";
						sRemoteFilePath += sIsoFileName + "." + sIsoFileExt;
						if (false == ftp.Exists(sRemoteFilePath))
						{
							ftp.Put(sSrcIsoFilePath, sRemoteFilePath);
						}
						else
						{
							AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR, "Can't transfer " + sSrcIsoFilePath + " because there is same file on ftp server");
						}
					}
					catch (Exception ex)
					{
						AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR, ex.Message);
					}
				}
			}
			catch (Exception ex)
			{
				AppDocData.frameworkCore.Logger.WriteLog(FrameworkLogger.Level.ERROR, ex.Message);
			}
		}
	}

    #endregion
}

