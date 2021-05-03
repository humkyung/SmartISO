using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.Common;

namespace IsoDwg.NET
{
	public class LineDataPos
	{
		public LineDataPos() { factor = 1; }

		public double X { get; set; }
		public double Y { get; set; }
		public double factor { get; set; }
		public string FontName { get; set; }
		public double Width { get; set; }
		public double Height { get; set; }
		public string Color { get; set; }

		/// <summary>
		/// parse given tokens
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		/// <param name="oTokens"></param>
		/// <returns></returns>
		public bool Parse(string[] oTokens)
		{
			try
			{
				X = Convert.ToDouble(oTokens[0]);
				Y = Convert.ToDouble(oTokens[1]);
				factor = (3 == oTokens.Length) ? Convert.ToDouble(oTokens[2]) : 1.0;

				return true;
			}
			catch { }

			return false;
		}

		/// <summary>
		/// parse style by given value
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		/// <param name="oValue"></param>
		/// <returns></returns>
		public bool ParseStyle(object oValue)
		{
			try
			{
				string[] oTokens = oValue.ToString().Split(",".ToCharArray());
				if (4 == oTokens.Length)
				{
					FontName = oTokens[0];
					Width = Convert.ToDouble(oTokens[1]);
					Height = Convert.ToDouble(oTokens[2]);
					Color = oTokens[3];

					return true;
				}
			}
			catch { }

			return false;
		}

		/// <summary>
		/// read line data pos from oracle
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		public static void LoadSetting()
		{
			using (AppOracleDatabase databaes = new AppOracleDatabase()
			{
				Host = AppDocData.IDCSOracleInfo.Host,
				Service = AppDocData.IDCSOracleInfo.Service,
				Id = AppDocData.IDCSOracleInfo.Id,
				Password = AppDocData.IDCSOracleInfo.Password
			})
			{
				using (DataSet ds = databaes.ExecuteDataSet(databaes.GetSqlStringCommand("SELECT * FROM T_LINE_DATA_POS")))
				{
					AppDocData.LineDataPosMap.Clear();
					foreach (DataColumn oCol in ds.Tables[0].Columns)
					{
						object oValue = ds.Tables[0].Rows[0][oCol];
						string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						string[] oTokens = sValue.Split(",".ToCharArray());
						if ((2 == oTokens.Length) || (3 == oTokens.Length))
						{
							LineDataPos oPos = new LineDataPos();
							if (true == oPos.Parse(oTokens))
							{
								AppDocData.LineDataPosMap.Add(oCol.ColumnName, oPos);
							}
						}
					}
				}

				using (DataSet ds = databaes.ExecuteDataSet(databaes.GetSqlStringCommand("SELECT * FROM T_LINE_DATA_POS_STYLE")))
				{
					foreach (DataColumn oCol in ds.Tables[0].Columns)
					{
						if (false == AppDocData.LineDataPosMap.ContainsKey(oCol.ColumnName)) continue;
						AppDocData.LineDataPosMap[oCol.ColumnName].ParseStyle(ds.Tables[0].Rows[0][oCol.ColumnName]);
					}
				}
			}
		}
	}
}
