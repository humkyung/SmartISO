using System;
using System.Data;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.OleDb;
using System.IO;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	class RevisionDataTable
	{
		public enum WriteOrder{ASC = 1, DESC=2};
		public class RevisionTableSetting
		{
			public int DisplayCount { get; set; }
			public WriteOrder Order {get;set;}
			public double Dist { get; set; }
			public AppDocData.TextStyle RevisionDataTextStyle = new AppDocData.TextStyle();	/// 2014.03.18 added by humkyung
			public Dictionary<string, Autodesk.AutoCAD.Geometry.Point3d> DataLocList = new Dictionary<string, Autodesk.AutoCAD.Geometry.Point3d>();
			public Dictionary<string, AppDocData.TextStyle> RevDataStyle = new Dictionary<string, AppDocData.TextStyle>();	/// 2014.07.25 added by humkyung

			/// <summary>
			/// read revision data setting
			/// </summary>
			/// <author>humkyung</author>
			/// <date>2013.12.25</date>
			public void Read(AppOracleDatabase oracle)
			{
				using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand("SELECT * FROM T_REVISION_SETTING")))
				{
					double dValue = 0;
					int iValue = 0;
					if (Double.TryParse(ds.Tables[0].Rows[0]["REV_DATA_DISTANCE"].ToString(), out dValue))
					{
						Dist = dValue;
					}

					if (Int32.TryParse(ds.Tables[0].Rows[0]["REV_DATA_DISPLAY_COUNT"].ToString(), out iValue))
					{
						DisplayCount = iValue;
					}

					string sValue = ds.Tables[0].Rows[0]["REV_DATA_SORT"].ToString();
					Order = ("DESC" != sValue) ? WriteOrder.ASC : WriteOrder.DESC;

					string[] oTokens = ds.Tables[0].Rows[0]["REV_DATA_STYLE"].ToString().Split(",".ToCharArray());
					if (4 == oTokens.Length)
					{
						RevisionDataTextStyle.Layer = "0";
						RevisionDataTextStyle.TextStyleName = oTokens[0];
						RevisionDataTextStyle.TextHeight = Convert.ToDouble(oTokens[2]);
						RevisionDataTextStyle.Justify = "Ceneter";
						RevisionDataTextStyle.Rotation = 0;
						RevisionDataTextStyle.TextColor = oTokens[3];
					}
				}

				using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand("SELECT * FROM T_REVISION_POS")))
				{
					foreach (System.Data.DataColumn oDataCol in ds.Tables[0].Columns)
					{
						string[] oTokens = ds.Tables[0].Rows[0][oDataCol].ToString().Split(",".ToCharArray());
						if (2 == oTokens.Length)
						{
							Autodesk.AutoCAD.Geometry.Point3d ptLoc = new Autodesk.AutoCAD.Geometry.Point3d(Convert.ToDouble(oTokens[0]), Convert.ToDouble(oTokens[1]), 0);
							DataLocList.Add(oDataCol.ColumnName, ptLoc);
						}
					}
				}

				/// get revision data style form database - 2014.07.25 added by humkyung
				using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand("SELECT * FROM T_REVISION_STYLE")))
				{
					foreach (System.Data.DataColumn oDataCol in ds.Tables[0].Columns)
					{
						string[] oTokens = ds.Tables[0].Rows[0][oDataCol].ToString().Split(",".ToCharArray());
						if (4 == oTokens.Length)
						{
							AppDocData.TextStyle oTextStyle = new AppDocData.TextStyle()
							{ 
								TextStyleName = oTokens[0] , TextHeight=Convert.ToDouble(oTokens[1]) , TextColor = oTokens[3]
							};
							RevDataStyle.Add(oDataCol.ColumnName, oTextStyle);
						}
					}
				}
				/// up to here
			}
		};
		public static RevisionTableSetting Setting { get; set; }

		public RevisionDataTable()
		{ 
		}

		/// <summary>
		/// write revision data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.12.25</date>
		/// <returns></returns>
		public static int Write(AppOracleDatabase oracle , Transaction tr , BlockTableRecord btr , string sDrawingNo)
		{
			int res = 0;

			if (null == RevisionDataTable.Setting)
			{
				RevisionDataTable.Setting = new RevisionDataTable.RevisionTableSetting() { DisplayCount = 5, Order = WriteOrder.ASC };
				RevisionDataTable.Setting.Read(oracle);
			}

			string sSql = string.Format("SELECT * FROM T_REVISION WHERE KEY='{0}' ORDER BY IDX ASC" , sDrawingNo);
			using (DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand(sSql)))
			{
				Int32 iCount = ds.Tables[0].Rows.Count;
				int iStart = (iCount < Setting.DisplayCount) ? 0 : iCount - Setting.DisplayCount, iEnd = iCount;
				for (int i = iStart; i < iEnd; ++i)
				{
					foreach (System.Data.DataColumn oCol in ds.Tables[0].Columns)
					{
						object oValue = ds.Tables[0].Rows[i][oCol];
						if (null == oValue) continue;
						
						if (Setting.DataLocList.ContainsKey(oCol.ColumnName))
						{
							Autodesk.AutoCAD.Geometry.Point3d ptLoc = Setting.DataLocList[oCol.ColumnName];
							Autodesk.AutoCAD.Geometry.Vector3d oMove = new Autodesk.AutoCAD.Geometry.Vector3d(
								0,
								(WriteOrder.ASC == Setting.Order) ? (Setting.Dist * (i - iStart)) : -(Setting.Dist * (i - iStart)),
								0);
							ptLoc = ptLoc.Add(oMove);

							AppDocData.TextStyle oTextStyle = null;
							oTextStyle = (true == RevisionDataTable.Setting.RevDataStyle.ContainsKey(oCol.ColumnName)) ? RevisionDataTable.Setting.RevDataStyle[oCol.ColumnName] : RevisionDataTable.Setting.RevisionDataTextStyle;

							TextElm oText = new TextElm()
							{
								TextString = oValue.ToString(),
								Height = oTextStyle.TextHeight,
								Location = ptLoc,
								Justify = oTextStyle.Justify,
								TextStyle = oTextStyle.TextStyleName,
								Layer = oTextStyle.Layer
							};
							oText.CreateEntity(btr, tr);
						}
					}
				}
			}
            return res;
		}
	}
}
