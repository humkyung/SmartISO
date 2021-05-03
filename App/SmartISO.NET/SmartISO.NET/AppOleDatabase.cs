using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data;
using System.Data.Common;
using System.Data.OleDb;

/*-------------------------------------------------------------------------------
* 
* *****************  Version 1  *****************
* author: HumKyung Baek Date: 2014.01.15 - access oledatabase
--------------------------------------------------------------------------------*/
namespace IsoDwg.NET
{
	public class AppOleDatabase : AbstractDatabase<OleDbConnection, OleDbCommand, OleDbDataAdapter>
	{
		public string AccessFilePath { get; set; }
		public string Password { get; set; }

		public class ColInfo
		{
			public string Name { get; set; }
			public string DataType { get; set; }
		};

		protected override string GetConnectionString()
		{
			string sConnString = string.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0}", AccessFilePath);
			if (!string.IsNullOrEmpty(Password)) sConnString += string.Format(";Jet OLEDB:Database Password={0}", Password);
			return sConnString;
		}
		

		/// <summary>
		/// get table names from database
		/// </summary>
		/// <returns></returns>
		public List<string> GetTableNameList()
		{
			/*
			String connect = "Provider=Microsoft.JET.OLEDB.4.0;data source=.\\Employee.mdb";
			OleDbConnection con = new OleDbConnection(connect);
			con.Open();  
			Console.WriteLine("Made the connection to the database");

			Console.WriteLine("Information for each table contains:");
			DataTable tables = con.GetOleDbSchemaTable(OleDbSchemaGuid.Tables,new object[]{null,null,null,"TABLE"});

			Console.WriteLine("The tables are:");
			foreach(DataRow row in tables.Rows) 
			Console.Write("  {0}", row[2]);


			con.Close();
			*/
			List<string> NameList = new List<string>();
			try
			{
				if (Connection.State != ConnectionState.Open) Connection.Open();
				DataTable tables = (Connection as OleDbConnection).GetOleDbSchemaTable(OleDbSchemaGuid.Tables,new object[]{null,null,null,"TABLE"});
				foreach(DataRow row in tables.Rows)
				{
					NameList.Add(row[2].ToString());
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine("Error Message : {0}", ex.Message);
			}

			return NameList;
		}

		/// <summary>
		/// get column information from ole database
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sTableName"></param>
		/// <returns></returns>
		public List<ColInfo> GetColumnInfoListOfTable(string sTableName)
		{
			List<ColInfo> ColumnInfoList = new List<ColInfo>();
			try
			{
				DataTable tables = (Connection as OleDbConnection).GetOleDbSchemaTable(OleDbSchemaGuid.Columns,new object[]{null,null,sTableName,null});
				foreach (DataRow row in tables.Rows)
				{
					ColInfo oColInfo = new ColInfo();
					oColInfo.Name = row["COLUMN_NAME"].ToString();
					oColInfo.DataType = row["DATA_TYPE"].ToString();
					ColumnInfoList.Add(oColInfo);
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine("Error Message : {0}", ex.Message);
			}

			return ColumnInfoList;
		}

        /// <summary>
        /// save data table to access database
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2013.04.01</date>
        /// <param name="datTable"></param>
        /// <returns></returns>
        public int SaveDataTable(string sTableName, System.Data.DataTable datTable)
        {
            int res = 0;

            if (Connection.State != ConnectionState.Open) Connection.Open();
            {
                foreach (System.Data.DataRow oRow in datTable.Rows)
                {
                    OleDbCommand oCmd = new OleDbCommand(string.Empty, Connection as OleDbConnection);

                    string sColumns = string.Empty, sValues = string.Empty;
                    byte[] dataBytes = null;
                    foreach (System.Data.DataColumn oCol in datTable.Columns)
                    {
                        string sValue = string.Empty;
                        if (!string.IsNullOrEmpty(sColumns)) sColumns += ",";
                        sColumns += string.Format("[{0}]", oCol.ColumnName);
                        object oValue = oRow[oCol.ColumnName];
                        sValue = (null != oValue) ? oValue.ToString() : "-";

                        if (System.Type.GetType("System.Byte[]") == oCol.DataType)
                        {
                            object val = oRow[oCol.ColumnName];
                            dataBytes = (val == DBNull.Value) ? new byte[1] : (byte[])val;

                            /// add by parameters
                            if (dataBytes.Length > 0)
                            {
                                sValue = string.Format("@{0}", oCol.ColumnName);
                                oCmd.Parameters.AddWithValue(sValue, dataBytes);
                            }
                        }
                        else
                        {
                            sValue = string.Format("'{0}'", sValue);
                        }
                        if (!string.IsNullOrEmpty(sValues)) sValues += ",";

                        sValues += sValue;

                    }

                    string sSql = string.Format("INSERT INTO {0}({1}) VALUES({2})", sTableName, sColumns, sValues);
                    try
                    {
                        oCmd.CommandText = sSql;
                        res += oCmd.ExecuteNonQuery();
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Error Message : {0}", ex.Message);
                    }
                }
            }

            return res;
        }
	}
}
