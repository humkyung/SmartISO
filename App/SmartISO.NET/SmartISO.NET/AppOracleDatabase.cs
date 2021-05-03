using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data;
using System.Data.Common;

/*-------------------------------------------------------------------------------
* 
* *****************  Version 1  *****************
* author: HumKyung Baek Date: 2012.07.17 - access oracle
--------------------------------------------------------------------------------*/
namespace IsoDwg.NET
{
	public class AppOracleDatabase : AbstractDatabase<OracleConnection , OracleCommand , OracleDataAdapter>
	{
		private const string oConnString = "(DESCRIPTION = (ADDRESS_LIST = (ADDRESS = (PROTOCOL = TCP)(HOST ={0}" + @")(PORT = 1521)) ) (CONNECT_DATA = (SERVICE_NAME = {1})))";
		
		public string Host{get;set;}
		public string Service{get;set;}
		public string Id{get;set;}
		public string Password{get;set;}

		public class ColInfo
		{
			public string Name { get; set; }
			public string DataType { get; set; }
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
		protected override string  GetConnectionString()
		{
			string connString = string.Format(oConnString, Host, Service);
			connString = "Data Source=" + connString + ";User Id=" + Id + ";Password=" + Password;

			Environment.SetEnvironmentVariable("NLS_LANG", "KOREAN_KOREA.KO16MSWIN949"); ///오라클 NLS에러 처리
			Environment.SetEnvironmentVariable("SQLNET.AUTHENTICATION_SERVICES", "(NONE,NTS)");

			return connString;
		}
		
		/// <summary>
		/// get table name list
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.07.20</date>
		/// <param name="sTableName"></param>
		/// <returns></returns>
		public List<string> GetTableNameList(string Owner , string sTableName)
		{
			List<string> res = new List<string>{};
			//if ((null != Connection) && (System.Data.ConnectionState.Open == Connection.State))
			{
				string sSql = "SELECT OBJECT_NAME FROM USER_OBJECTS WHERE OBJECT_TYPE='TABLE'";
				///if (string.Empty != Owner) sSql += " AND OWNER='" + Owner + "'";
				if (null != sTableName) sSql += " AND OBJECT_NAME='" + sTableName + "'";
				
				using(DbDataReader reader = ExecuteReader(new OracleCommand(sSql , Connection as OracleConnection)))
				{
					while (reader.Read())
					{
						res.Add(reader.GetString(0));
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
		public List<ColInfo> GetColumnInfoListOf(string sDatabaseName, string sTableName)
		{
			List<ColInfo> res = new List<ColInfo> { };

			string sSql = "SELECT column_name,DATA_TYPE FROM all_tab_columns WHERE ";
			if (string.Empty != sDatabaseName) sSql += "OWNER=upper('" + sDatabaseName + "') AND ";
			sSql += "table_name='" + sTableName + "' ORDER BY COLUMN_ID";

			using(DbDataReader reader = ExecuteReader(GetSqlStringCommand(sSql)))
			{
				while (reader.Read())
				{
					res.Add(new ColInfo() { Name = reader.GetString(0), DataType = reader.GetString(1) });
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
			if ((null != Connection) && (System.Data.ConnectionState.Open == Connection.State))
			{
				DbCommand cmd = GetSqlStringCommand(string.Format("SELECT COUNT(*) FROM {0}" , sTableName));
				using(DbDataReader reader = cmd.ExecuteReader())
				{
					while (reader.Read())
					{
						res = Convert.ToInt32(reader.GetValue(0).ToString());
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
			DbCommand cmd = GetSqlStringCommand(sSql);
			return cmd.ExecuteNonQuery();
		}

		/// <summary>
		/// get only first blob data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.08</date>
		/// <param name="sFieldName"></param>
		/// <param name="sTableName"></param>
		/// <param name="sFilePath"></param>
		/// <returns></returns>
		public byte[] GetBlobData(string sFieldName , string sTableName)
		{
			byte[] buffer = null;
			if ((null != Connection) && (System.Data.ConnectionState.Open == Connection.State))
			{
				using (OracleCommand comm = new OracleCommand("SELECT " + sFieldName + " FROM " + sTableName, Connection as OracleConnection))
				{
					using (OracleDataReader reader = comm.ExecuteReader())
					{
						while (reader.Read())
						{
							Oracle.ManagedDataAccess.Types.OracleBlob blob = reader.GetOracleBlob(0);
							buffer = new byte[blob.Length];
							int actual = blob.Read(buffer, 0, Convert.ToInt32(blob.Length));
							break;
						}
					}
				}
			}

			return buffer;
		}
	}
}
