using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.Common;

namespace IsoDwg.NET
{
	public class Area
	{
		public double MinX { get; set; }
		public double MinY { get; set; }
		public double MaxX { get; set; }
		public double MaxY { get; set; }

		/// <summary>
		/// check given point is located in area
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <param name="Pnt"></param>
		/// <returns></returns>
		public bool Contains(Autodesk.AutoCAD.Geometry.Point3d Pnt)
		{
			if (MaxX < Pnt.X) return false;
			if (MaxY < Pnt.Y) return false;
			if (MinX > Pnt.X) return false;
			if (MinY > Pnt.Y) return false;

			return true;
		}

		/// <summary>
		/// parse area by given value
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		/// <param name="oValue"></param>
		/// <returns></returns>
		public bool Parse(object oValue)
		{
			try
			{
				string str = (null != oValue) ? oValue.ToString() : string.Empty;
				if (!string.IsNullOrEmpty(str))
				{
					string[] oTokens = str.Split(",".ToCharArray());
					if (4 == oTokens.Length)
					{
						double x1 = Convert.ToDouble(oTokens[0]);
						double y1 = Convert.ToDouble(oTokens[1]);
						double x2 = Convert.ToDouble(oTokens[2]);
						double y2 = Convert.ToDouble(oTokens[3]);

						MinX = Math.Min(x1, x2);
						MinY = Math.Min(y1, y2);
						MaxX = Math.Max(x1, x2);
						MaxY = Math.Max(y1, y2);

						return true;
					}
				}
			}
			catch { }

			return false;
		}

		/// <summary>
		/// read area setting from oracle
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		public static void LoadSetting()
		{
			string sValue = string.Empty;

			using (AppOracleDatabase databaes = new AppOracleDatabase()
			{
				Host = AppDocData.IDCSOracleInfo.Host,
				Service = AppDocData.IDCSOracleInfo.Service,
				Id = AppDocData.IDCSOracleInfo.Id,
				Password = AppDocData.IDCSOracleInfo.Password
			})
			{
				AppDocData.IsoAreaMap.Clear();
				AppDocData.LineDataArea.Clear();

				using (DataSet ds = databaes.ExecuteDataSet(databaes.GetSqlStringCommand("SELECT * FROM T_BORDER_SETTING")))
				{
					foreach (DataColumn oCol in ds.Tables[0].Columns)
					{
						if (true == oCol.ColumnName.Contains("REMOVE_AREA"))
						{
							object oValue = ds.Tables[0].Rows[0][oCol.ColumnName];
							{
								Area oArea = new Area();
								if (true == oArea.Parse(oValue))
								{
									AppDocData.IsoAreaMap.Add(oCol.ColumnName, oArea);
								}
							}
						}
					}
					{
						object oValue = ds.Tables[0].Rows[0]["DRAWING_AREA"];
						{
							Area oArea = new Area();
							if (true == oArea.Parse(oValue))
							{
								AppDocData.IsoAreaMap.Add("DRAWING_AREA", oArea);
							}
						}

						oValue = ds.Tables[0].Rows[0]["BM_AREA"];
						{
							Area oArea = new Area();
							if (true == oArea.Parse(oValue))
							{
								AppDocData.IsoAreaMap.Add("BM_AREA", oArea);
							}
						}

						oValue = ds.Tables[0].Rows[0]["BORDER_AREA"];
						{
							Area oArea = new Area();
							if (true == oArea.Parse(oValue))
							{
								AppDocData.IsoAreaMap.Add("BORDER_AREA", oArea);
							}
						}
					}
				}

				/// read line data area - 2014.05.20 added by humkyung
				using (DataSet ds = databaes.ExecuteDataSet(databaes.GetSqlStringCommand("SELECT * FROM T_LINE_DATA_AREA")))
				{
					foreach (DataColumn oCol in ds.Tables[0].Columns)
					{
						object oValue = ds.Tables[0].Rows[0][oCol];
						{
							Area oArea = new Area();
							if (true == oArea.Parse(oValue))
							{
								AppDocData.LineDataArea.Add(oCol.ColumnName, oArea);
							}
						}
					}
				}
				/// up to here
			}
		}
	}
}
