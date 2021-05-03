using System;
using System.Windows.Forms;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Data;
using System.Data.OleDb;  /// for ADO.NET OLEDb provider
using System.Data.OracleClient;
using framework.core;
using System.Runtime.InteropServices;

using GemBox.Spreadsheet;
using GenericParsing;

namespace SmartISOLayer
{
	[ComVisible(true)]
	[Guid("B815898D-B555-4570-ABF6-5A39217E1638")]
	public interface ISmartISOLayer
	{
		bool ConnectOracle(string host, string service, string id, string password);
		bool DisconnectOracle();
		int CreateView(string sPrjName);
		int UpdateOracleWith(string sPrjName,string sInnerCaluse,string sOuterClause);
		bool DumpPrjDatabase(string sOutputFilePath, string sHost, string sService, string sUserName, string sPassword);
	}

	[ComVisible(true)]
	[Guid("1EDCABB5-7E0D-4FD3-AE20-DA885A41BEC3")]
	public class SmartISOLayer : ISmartISOLayer
	{
		AppOracleDatabase _database = null;
		DataTable _TableOfPDTABLE_12 = new DataTable();

		/// <summary>
		/// oracle connection test
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.29</date>
		/// <param name="host"></param>
		/// <param name="service"></param>
		/// <param name="id"></param>
		/// <param name="password"></param>
		/// <returns></returns>
		public bool ConnectOracle(string host, string service , string id, string password)
		{
			bool res = false;

			if (null == _database)
			{
				_database.Connection.Close();
				_database.Dispose();
				_database = null;
			}

			try
			{
				_database = new AppOracleDatabase() { Host = host, Service = service, Id = id, Password = password };
				_database.Connection.Open();
				res = true;
			} catch { }

			return res;
		}

		/// <summary>
		/// disconnect oracle
		/// </summary>
		/// <author>humkyung</author>
		/// <returns></returns>
		public bool DisconnectOracle()
		{
			bool res = false;
			try
			{
				if (null != _database)
				{
					_database.Connection.Close();
					_database.Dispose();
				}
				_database = null;
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}

			return res;
		}

		/// <summary>
		/// get list of pdtable_12_%
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.30</date>
		/// <returns></returns>
		private DataTable GetListOfPDTABLE_12(string projectname)
		{
			if ((null != _database) && (null != _database.Connection) && (ConnectionState.Open == _database.Connection.State))
			{
				if ((null == _TableOfPDTABLE_12.Rows) || (0 == _TableOfPDTABLE_12.Rows.Count))
				{
					string sSql = string.Format("SELECT TABLE_NAME FROM DBA_TABLES WHERE OWNER=upper('DD_{0}') AND TABLE_NAME LIKE 'PDTABLE_12_%'", projectname);

					OracleDataAdapter oDataAdapter = new OracleDataAdapter(sSql, _database.Connection as OracleConnection);
					OracleCommandBuilder builder = new OracleCommandBuilder(oDataAdapter);

					_TableOfPDTABLE_12.Locale = System.Globalization.CultureInfo.InvariantCulture;
					oDataAdapter.Fill(_TableOfPDTABLE_12);
				}
			}

			return _TableOfPDTABLE_12;
		}

		/// <summary>
		/// update oracle with given clause
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.30</date>
		/// <param name="clause"></param>
		/// <returns></returns>
		public int UpdateOracleWith(string sPrjName,string sInnerCaluse,string sOuterClause)
		{
			int res = 0;
			if ((null != _database) && (null != _database.Connection) && (ConnectionState.Open == _database.Connection.State))
			{
				try
				{
					//#region TEST
					//string sFilePath = @"c:\Temp\Update.sql";
					//StreamWriter oStreamWriter = new StreamWriter(sFilePath, false, System.Text.Encoding.UTF8);
					//#endregion

					string sSql = string.Format("SELECT DISTINCT 'UPDATE DD_{0}.PDTABLE_12_'||PART|| ' SET {1}' FROM DD_{0}.SMARTISO_PDTABLE_VIEW WHERE {2}", sPrjName.ToUpper(), sInnerCaluse, sOuterClause);
					//oStreamWriter.WriteLine(sSql);
					using (OracleCommand comm = new OracleCommand(sSql, _database.Connection as OracleConnection))
					{
						OracleDataAdapter oDataAdapter = new OracleDataAdapter(sSql, _database.Connection as OracleConnection);
						OracleCommandBuilder builder = new OracleCommandBuilder(oDataAdapter);
						DataTable oDataTable = new DataTable();
						oDataTable.Locale = System.Globalization.CultureInfo.InvariantCulture;
						oDataAdapter.Fill(oDataTable);
						foreach (DataRow oRow in oDataTable.Rows)
						{
							object oValue = oRow[0];
							string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
							if (string.Empty != sValue)
							{
								//oStreamWriter.WriteLine(sValue);

								comm.CommandText = sValue;
								comm.ExecuteNonQuery();
							}
						}

						//#region TEST
						//{
						//    oStreamWriter.Close();

						//    System.Diagnostics.Process proc = new System.Diagnostics.Process();
						//    proc.StartInfo.FileName = "notepad.exe";
						//    proc.StartInfo.Arguments = sFilePath;
						//    proc.Start();
						//}
						//#endregion
					}
				}
				catch (OracleException ex)
				{
					MessageBox.Show(ex.Message);
					res = ex.ErrorCode;
				}
				catch (Exception ex1)
				{
					MessageBox.Show(ex1.Message);
					res = -1;
				}
			}
			else
			{
				MessageBox.Show("Oracle is not connected");
				res = -1;
			}

			return res;
		}

		/// <summary>
		/// create a view
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.09.05</date>
		/// <param name="projectname"></param>
		/// <returns></returns>
		public int CreateView(string sPrjName)
		{
			int res = 0;
			try
			{
				string sSql = string.Format("DROP VIEW DD_{0}.SmartISO_PDTABLE_VIEW" , sPrjName.ToUpper());
				try
				{
					using (OracleCommand comm = new OracleCommand(sSql, _database.Connection as OracleConnection))
					{
						comm.ExecuteNonQuery();
					}
				}
				catch (OracleException)
				{
				}

				sSql = string.Format("CREATE VIEW DD_{0}.SmartISO_PDTABLE_VIEW AS " , sPrjName.ToUpper());
				string sColumnNames = string.Empty;

				DataTable o12Table = this.GetListOfPDTABLE_12(sPrjName);
				foreach (DataRow oRow in o12Table.Rows)
				{
					string sTableName = oRow[0].ToString();
					if (string.Empty == sColumnNames)
					{
						List<string> oColumnNameList = _database.GetColumnNameListOf(sPrjName, sTableName);
						foreach (string str in oColumnNameList)
						{
							sColumnNames += str + ",";
						}
						sColumnNames = sColumnNames.TrimEnd(",".ToCharArray());
					}
					string sPartNum = sTableName.Substring("PDTABLE_12_".Length);
					sSql += "SELECT " + sColumnNames + "," + sPartNum + " AS PART FROM " + sTableName + " UNION ALL ";
				}
				sSql = sSql.Substring(0, sSql.Length - " UNION ALL ".Length);

				using (OracleCommand comm = new OracleCommand(sSql, _database.Connection as OracleConnection))
				{
					comm.ExecuteNonQuery();
				}
			}
			catch (OracleException ex)
			{
				MessageBox.Show(ex.Message);
				res = ex.ErrorCode;
			}

			return res;
		}

		/// <summary>
		/// create and fill table with given table
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.29</date>
		/// <param name="conn"></param>
		/// <param name="sTableName"></param>
		/// <param name="oDataTable"></param>
		private void CreateAndFillTableWith(OleDbConnection conn , DataTable oDataTable)
		{
			string sSql = "DROP TABLE " + oDataTable.TableName;
			OleDbCommand oCommand = new OleDbCommand(sSql, conn);
			try
			{
				oCommand.ExecuteNonQuery();
			}
			catch(OleDbException)
			{
				/// got error
			}

			sSql = "CREATE TABLE " + oDataTable.TableName + "(";
			foreach (DataColumn oColumn in oDataTable.Columns)
			{
				sSql += oColumn.ColumnName + " VARCHAR(255),";
			}
			sSql = sSql.TrimEnd(",".ToCharArray());
			sSql += ")";
			{
				oCommand.CommandText = sSql;
				oCommand.ExecuteNonQuery();
			}

			foreach (DataRow oRow in oDataTable.Rows)
			{
				sSql = string.Empty;
				for (int i = 0; i < oDataTable.Columns.Count; ++i)
				{
					object oValue = oRow[i];
					string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
					sSql += "'" + sValue + "',";
				}
				sSql = sSql.TrimEnd(",".ToCharArray());
				oCommand.CommandText = "INSERT INTO " + oDataTable.TableName + " VALUES(" + sSql + ")";
				oCommand.ExecuteNonQuery();
			}
		}

		/// <summary>
		/// extract drawing information and then save as mdb file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.29</date>
		/// <param name="output"></param>
		/// <param name="host"></param>
		/// <param name="service"></param>
		/// <param name="id"></param>
		/// <param name="password"></param>
		/// <returns></returns>
		public bool ExtractDrawingInfo(string output , string host, string service, string id, string password , string projectname)
		{
			bool res = false;

			DataSet oDataSet = new DataSet();
			try
			{
				using (AppOracleDatabase database = new AppOracleDatabase() { Host = host, Service = service, Id = id, Password = password })
				{
					database.Connection.Open();
					{
						string sSql = "select * from pd_" + projectname + ".pdtable_121";
						OracleDataAdapter oDataAdapter = new OracleDataAdapter(sSql, database.Connection as OracleConnection);
						OracleCommandBuilder builder = new OracleCommandBuilder(oDataAdapter);

						DataTable oTable = oDataSet.Tables.Add();
						oTable.Locale = System.Globalization.CultureInfo.InvariantCulture;
						oDataAdapter.Fill(oTable);
						oTable.TableName = "pdtable_121";
					}
					{
						string sSql = "SELECT * from pd_" + projectname + ".pdtable_122";
						OracleDataAdapter oDataAdapter = new OracleDataAdapter(sSql, database.Connection as OracleConnection);
						OracleCommandBuilder builder = new OracleCommandBuilder(oDataAdapter);

						DataTable oTable = oDataSet.Tables.Add();
						oTable.Locale = System.Globalization.CultureInfo.InvariantCulture;
						oDataAdapter.Fill(oTable);
						oTable.TableName = "pdtable_122";
					}
				}
			}
			catch (OracleException ex)
			{
				MessageBox.Show(ex.Message);
				return res;
			}

			try
			{
				string strConnection = @"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + output + ";";
				OleDbConnection conn = new OleDbConnection(strConnection);
				conn.Open();
				{
					foreach (DataTable oDataTable in oDataSet.Tables)
					{
						this.CreateAndFillTableWith(conn, oDataTable);
					}
					conn.Close();
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);
				return res;
			}

			return true;
		}

        /// <summary>
        /// write csv files to excel file
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2013.01.22</date>
        /// <param name="sExlFilePath"></param>
        /// <param name="sCSVFilePath1"></param>
        /// <param name="sCSVFilePath2"></param>
        /// <param name="sIniFilePath"></param>
        /// <returns></returns>
        public bool WriteCSVToExcel(string sExlFilePath, string sCSVFilePath1, string sCSVFilePath2 , string sIniFilePath)
        {
            const string GemBoxLicense = "EXK0-347T-N5GE-K8Y1";

            SpreadsheetInfo.SetLicense(GemBoxLicense);
            ExcelFile ef = new ExcelFile();
            ef.LoadXls(sExlFilePath , XlsOptions.PreserveAll);
            if (1 == ef.Worksheets.Count)
            {
                ExcelWorksheet wsTemplate = ef.Worksheets[0];

                /// copy excel sheet
                ExcelWorksheet ws = ef.Worksheets.AddCopy(string.Format("Sheet{0}", 2), wsTemplate);
            }
            
            /// get b/m header - 2013.05.19 added by humkyung
            CIni Ini = new CIni();
            string sValue = Ini.GetIniValue("Iso Edit Setting" , "BM Field Format" , 256 , sIniFilePath);
            string[] oTokens = sValue.Split(",".ToCharArray());
            /// up to here

            using (GenericParserAdapter parser = new GenericParserAdapter())
			{
                ExcelWorksheet ws = ef.Worksheets[0];
                
				parser.SetDataSource(sCSVFilePath1);

				parser.ColumnDelimiter = ',';
				parser.TextQualifier = '\"';

                /// write b/m header - 2013.05.19 added by humkyung
                int col = 0;
                foreach (string str in oTokens)
                {
                    ws.Cells[0, col++].Value = str;
                }
                /// up to here

                int row = 1;
				while (parser.Read())
				{
                    for (col = 0; col < parser.ColumnCount; ++col)
					{
                        string str = parser[col];
                        if (string.IsNullOrEmpty(str)) continue;
                        if ('=' == str[0]) str = str.Substring(1);
                        str = str.Replace("\"\"", "\"");
                        str = str.Trim("\"".ToCharArray());
                        ws.Cells[row, col].Value = str;
					}
                    row++;
				}
			}

            using (GenericParserAdapter parser = new GenericParserAdapter())
			{
                ExcelWorksheet ws = ef.Worksheets[1];
                
				parser.SetDataSource(sCSVFilePath2);

				parser.ColumnDelimiter = ',';
				parser.TextQualifier = '\"';

                /// write b/m header - 2013.05.19 added by humkyung
                int col = 0;
                foreach (string str in oTokens)
                {
                    if ("DRAWING NO" == str) continue;  /// ignore DRAWING NO when it is b/m summary file
                    ws.Cells[0, col++].Value = str;
                }
                /// up to here

                int row = 1;
				while (parser.Read())
				{
                    for (col = 0; col < parser.ColumnCount; ++col)
					{
                        string str = parser[col];
                        if (string.IsNullOrEmpty(str)) continue;
                        if ('=' == str[0]) str = str.Substring(1);
                        str = str.Replace("\"\"", "\"");
                        str = str.Trim("\"".ToCharArray());
                        ws.Cells[row, col].Value = str;
					}
                    row++;
				}
			}

            ef.SaveXls(sExlFilePath);

            return true;
        }

		/// <summary>
		/// dump project database
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.10.30</date>
		/// <param name="sOutputFilePath"></param>
		/// <param name="sHost"></param>
		/// <param name="sService"></param>
		/// <param name="sUserName"></param>
		/// <param name="sPassword"></param>
		/// <returns></returns>
		public bool DumpPrjDatabase(string sOutputFilePath, string sHost, string sService, string sUserName, string sPassword)
		{
			bool res = false;

			FormExtractOracle dlg = new FormExtractOracle(sOutputFilePath, sHost, sService, sUserName, sPassword);
			try
			{
				dlg.ShowDialog();
				res = true;
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			finally
			{
				dlg.Close();
			}

			return res;
		}
	}
}
