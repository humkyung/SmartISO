using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Data.OracleClient;

/*-------------------------------------------------------------------------------
* 
* *****************  Version 1  *****************
* author: HumKyung Baek Date: 2012.07.17 - access oracle
--------------------------------------------------------------------------------*/
namespace framework.core
{
	public class MyOracleConnection: IDisposable
	{
		private string _connString = @"
			(DESCRIPTION =
				(ADDRESS_LIST =
					(ADDRESS = (PROTOCOL = TCP)(HOST ={0}" + @")(PORT = 1521))
				)
				(CONNECT_DATA =
					(SERVICE_NAME = {1})
				)
			)";

		/// <summary>
		/// property of OracleConnection
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.17</date>
		private OracleConnection _conn = null;
		public OracleConnection Connection
		{
			get { return _conn; }
		}

		//Implement IDisposable.
		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		public MyOracleConnection()
		{
		}

		/// <summary>
		/// connect oralce with host,service,id,password
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.17</date>
		/// <param name="sHost"></param>
		/// <param name="sService"></param>
		/// <param name="sId"></param>
		/// <param name="sPassword"></param>
		/// <returns></returns>
		public bool Connect(string sHost, string sService, string sId, string sPassword)
		{
			string connString = string.Format(_connString, sHost, sService);
			connString = "Data Source=" + connString + ";User Id=" + sId + ";Password=" + sPassword;

			try
			{
				Environment.SetEnvironmentVariable("NLS_LANG", "KOREAN_KOREA.KO16MSWIN949"); //오라클 NLS에러 처리

				_conn = new OracleConnection(connString);
				{
					_conn.Open();

					return true;
				}
			}
			catch (OracleException ex)
			{
				MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}

			return false;
		}

		/// <summary>
		/// get table name list
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.20</date>
		/// <param name="sTableName"></param>
		/// <returns></returns>
		public List<string> GetTableNameList(string sTableName)
		{
			List<string> res = new List<string>{};
			if ((null != _conn) && (System.Data.ConnectionState.Open == _conn.State))
			{
				string sSql = "SELECT OBJECT_NAME FROM USER_OBJECTS WHERE OBJECT_TYPE='TABLE'";
				if (null != sTableName) sSql += " AND OBJECT_NAME='" + sTableName + "'";
				using (OracleCommand aCommand = new OracleCommand(sSql, _conn))
				{
					using (OracleDataReader reader = aCommand.ExecuteReader())
					{
						while (reader.Read())
						{
							res.Add(reader.GetString(0));
						}
					}
				}
			}

			return res;
		}

		/// <summary>
		/// get column list of table
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.17</date>
		/// <param name="oColumnList"></param>
		/// <param name="sTableName"></param>
		/// <returns></returns>
		public List<string> GetColumnNameListOf(string sTableName)
		{
			List<string> res = new List<string>{};
			if ((null != _conn) && (System.Data.ConnectionState.Open == _conn.State))
			{
				using (OracleCommand aCommand = new OracleCommand("SELECT column_name FROM USER_TAB_COLUMNS WHERE table_name='" + sTableName + "'", _conn))
				{
					using (OracleDataReader reader = aCommand.ExecuteReader())
					{
						while (reader.Read())
						{
							res.Add(reader.GetString(0));
						}
					}
				}
			}

			return res;
		}

		/// <summary>
		/// return number of records in table
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.20</date>
		/// <param name="sTableName"></param>
		/// <returns></returns>
		public int GetRecordCountOf(string sTableName)
		{
			int res = 0;
			if ((null != _conn) && (System.Data.ConnectionState.Open == _conn.State))
			{
				using (OracleCommand aCommand = new OracleCommand("SELECT COUNT(*) FROM " + sTableName, _conn))
				{
					using (OracleDataReader reader = aCommand.ExecuteReader())
					{
						while (reader.Read())
						{
							res = Convert.ToInt32(reader.GetValue(0).ToString());
						}
					}
				}
			}

			return res;
		}

		/// <summary>
		/// execute non-query
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.17</date>
		/// <param name="sSql"></param>
		/// <returns>int</returns>
		public int ExecuteNonQuery(string sSql)
		{
			int row = 0;
			if ((null != _conn) && (System.Data.ConnectionState.Open == _conn.State))
			{
				using (OracleCommand aCommand = new OracleCommand(sSql, _conn))
				{
					row = aCommand.ExecuteNonQuery();
				}
			}

			return row;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="disposing"></param>
		protected virtual void Dispose(bool disposing)
		{
			if (disposing)
			{
				// Free other state (managed objects).
			}

			if (null != _conn)
			{
				_conn.Close();
				_conn = null;
			}
		}

		/// <summary>
		/// destructor
		/// </summary>
		~MyOracleConnection()
		{
			this.Dispose(false);
		}
	}
}
