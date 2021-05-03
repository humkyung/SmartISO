using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Data.OleDb;  /// for ADO.NET OLEDb provider
using System.Data.OracleClient;

namespace SmartISOLayer
{
	public partial class FormExtractOracle : Form
	{
		private string PrjName{ get; set; }
		private string OutputFilePath { get; set; }
		private string Host { get; set; }
		private string Service { get; set; }
		private string PrjDatabaseUser { get; set; }
		private string PrjDatabasePassword { get; set; }

		private List<string> PartionNoList = new List<string>();

		public FormExtractOracle(string sOutputFilePath , string sHost , string sService , string sPrjDatabaseUser , string sPrjDatabasePassword)
		{
			InitializeComponent();
			OutputFilePath = sOutputFilePath;
			Host = sHost;
			Service = sService;
			PrjDatabaseUser = sPrjDatabaseUser;
			PrjDatabasePassword = sPrjDatabasePassword;
		}

		private void buttonClose_Click(object sender, EventArgs e)
		{
			Close();
		}

		/// <summary>
		/// dump project database(121,122) of pds
		/// </summary>
		/// <param name="host"></param>
		/// <param name="service"></param>
		/// <param name="id"></param>
		/// <param name="password"></param>
		/// <param name="AccessFilePath"></param>
		/// <returns></returns>
		public int DumpProjectDatabase()
		{
			int res = 0;
			try
			{
				using (framework.core.AppOracleDatabase database = new framework.core.AppOracleDatabase()
				{
					Host = this.Host,
					Service = this.Service,
					Id = this.PrjDatabaseUser,
					Password = this.PrjDatabasePassword
				}
				)
				{
					toolStripStatusLabelProgress.ForeColor = Color.Black;
					toolStripStatusLabelProgress.Text = "Dump Project Database";
					res = this.DumpProjectTable(database);
				}

				return res;
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
			return res;
		}

		/// <summary>
		/// dump project table
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.09.17</date>
		/// <param name="oleConn"></param>
		/// <param name="oraConn"></param>
		/// <returns></returns>
		private int DumpProjectTable(framework.core.AppOracleDatabase oracle)
		{
			int res = 0;
			try
			{
				progressBarProjectDatabase.Minimum = 0;
				progressBarProjectDatabase.Maximum = 0;
				progressBarProjectDatabase.Step = 1;
				progressBarProjectDatabase.Value = 0;

				using (framework.core.AppSQLiteDatabase database = new framework.core.AppSQLiteDatabase() { FilePath = OutputFilePath })
				{
					List<string> oTableNames = new List<string>() { "121", "122" };
					/// get count of records
					foreach (string sTableName in oTableNames)
					{
						string sSql = string.Format("SELECT COUNT(*) FROM PDTABLE_{0}", sTableName);
						using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand(sSql)))
						{
							progressBarProjectDatabase.Maximum += Convert.ToInt32(ds.Tables[0].Rows[0][0]);
						}
					}
					/// up to here

					foreach (string sTableName in oTableNames)
					{
						string sSql = string.Format("SELECT * FROM PDTABLE_{0}", sTableName);
						toolStripStatusLabelProgress.Text = sSql;
						using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand(sSql)))
						{
							List<string> oColumns = new List<string>();
							List<string> oValues = new List<string>();

							/// create 121 table
							string sql = string.Format("create table if not exists pdtable_{0}(", sTableName);
							foreach (DataColumn col in ds.Tables[0].Columns)
							{
								oColumns.Add(col.ColumnName);
								sql += col.ColumnName + " varchar(256),";
							}
							sql = sql.TrimEnd(",".ToCharArray());
							sql += ")";
							database.ExecuteNonQuery(database.GetSqlStringCommand(sql));
							/// up to here

							foreach (DataRow oDataRow in ds.Tables[0].Rows)
							{
								oValues.Clear();
								foreach (string sColName in oColumns)
								{
									string sValue = (null != oDataRow[sColName]) ? oDataRow[sColName].ToString() : string.Empty;
									oValues.Add(sValue);
								}

								sql = string.Format("INSERT INTO pdtable_{0}({1}) VALUES({2})", sTableName, string.Join(",", oColumns.ToArray()), "'" + string.Join("','", oValues.ToArray()) + "'");
								toolStripStatusLabelProgress.Text = sql;
								database.ExecuteNonQuery(database.GetSqlStringCommand(sql));

								progressBarProjectDatabase.PerformStep();
								Application.DoEvents();
							}
						}
					}
				}
			}
			catch (Exception ex)
			{
				toolStripStatusLabelProgress.ForeColor = Color.Red;
				toolStripStatusLabelProgress.Text = ex.Message;
			}

			return res;
		}

		

		private void FormExtractOracle_Load(object sender, EventArgs e)
		{
		}

		private void FormExtractOracle_Shown(object sender, EventArgs e)
		{
			buttonClose.Enabled = false;
			try
			{
				DumpProjectDatabase();
				toolStripStatusLabelProgress.ForeColor = Color.Blue;
				toolStripStatusLabelProgress.Text = "Dump is done.";
			}
			catch (Exception ex)
			{
				toolStripStatusLabelProgress.ForeColor = Color.Red;
				toolStripStatusLabelProgress.Text = ex.Message;
			}
			buttonClose.Enabled = true;
			Close();
		}
	}
}
