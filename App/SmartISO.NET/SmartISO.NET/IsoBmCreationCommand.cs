using System;
using System.Data.Common;
using System.Data.OleDb;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;
using System.Reflection;

using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.GraphicsInterface;
using Autodesk.AutoCAD.Colors;

using GemBox.Spreadsheet;

namespace IsoDwg.NET
{
	class IsoBmCreationCommand : AbstractCommand
	{
		private const int GENERAL = 0;
		private const int INSTRUMENT = 1;
		private const int SPECIALTY = 2;

        public string Key { get; set; }
		public static Dictionary<string, string> LineDataValue = new Dictionary<string, string>();
        
		private const string GemBoxLicense = "EXK0-347T-N5GE-K8Y1";
        public AppOracleDatabase oracle { get; set; }
        public OleDbConnection OleDbConn{ get; set; }

		class WBSItem
		{
			public string WBSCode { get; set; }
			public string Plant { get; set; }
			public string Area { get; set; }
			public string Unit { get; set; }
		}

		/// <summary>
		/// return empty string if given value is '-'
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.09.26</date>
		/// <param name="sValue"></param>
		/// <returns></returns>
		private string DBValueString(string sValue)
		{
			sValue = sValue.Replace("'", "''");
			string res = ("-" == sValue) ? string.Empty : sValue;
			
			return res;
		}

		/// <summary>
		/// load line data value from ini file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.22</date>
		private void LoadLineDataValue()
		{
			IsoBmCreationCommand.LineDataValue.Clear();
            using (System.Data.DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand(string.Format("SELECT * FROM T_LINE_DATA WHERE KEY='{0}'", Key))))
            {
				if ((1 == ds.Tables.Count) && (ds.Tables[0].Rows.Count > 0))
				{
					foreach (System.Data.DataColumn oCol in ds.Tables[0].Columns)
					{
						IsoBmCreationCommand.LineDataValue.Add(oCol.ColumnName, ds.Tables[0].Rows[0][oCol].ToString());
					}
				}
            }

			{
				string sValue = AppDocData.GetIniValue("ISO Product", "Part", 256, AppDocData.IniFilePath);
				if (IsoBmCreationCommand.LineDataValue.ContainsKey("PART"))
				{
					IsoBmCreationCommand.LineDataValue["PART"] = sValue;
				}
				else
				{
					IsoBmCreationCommand.LineDataValue.Add("PART", sValue);
				}

				AppDocData.JobNo = AppDocData.GetIniValue("Project Setting", "Job No", 256, AppDocData.IniFilePath);
			}
		}

		/// <summary>
		/// execute iso bm creation
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.19</date>
		/// <param name="tm"></param>
		public List<QCCheckCommand.Result> Execute(Document doc)
		{
			List<QCCheckCommand.Result> res = new List<QCCheckCommand.Result>();

			/// get option for check fixisodwg(refer to : http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/20) - 2013.10.22 added by humkyung
			//if ((true == AppDocData.QCCheck.CheckIfQCCheckExecution) && (false == IsoProductCommand.HasRegAppTableRecord("IDCS4PDMS")))
			//{
			//    QCCheckCommand.Result oErr = new QCCheckCommand.Result() { Item = "-1", ErrString = "fix iso dwg is not executed!!!" };
			//    res.Add(oErr);
			//    return res;
			//}
			/// up to here

			Database db = doc.Database;
			Editor ed = doc.Editor;
			Autodesk.AutoCAD.DatabaseServices.TransactionManager tm = doc.TransactionManager;

			ObjectId TableRcdId;
			using (Transaction tr = tm.StartTransaction())
			{
				/// read entity.
				BlockTable bt = (BlockTable)tr.GetObject(db.BlockTableId, OpenMode.ForRead);
				TableRcdId = bt[BlockTableRecord.ModelSpace];
				tr.Commit();
			}

			IsoDwgParser parser = new IsoDwgParser(TableRcdId);
			parser.Parse();
			{
				LoadLineDataValue();
				/// update line data with data in iso dwg
				foreach (string str in IsoDwgParser.LineDataValue.Keys)
				{
					if (string.IsNullOrEmpty(IsoDwgParser.LineDataValue[str])) continue;
					LineDataValue[str] = IsoDwgParser.LineDataValue[str];
				}

				/// check value exist for bm field, if not insert empty string
				List<string> BMFieldNameList = new List<string>()
				{
					"DRAW_NO" , "PLANT"      , "AREA"       , "UNIT"          , "FLUID"     , "SYSTEM_NO", "SHEET" , "DWG_REV"   , "PLAN_DWG" , "PID_NO" ,
					"LINE_NO" , "PWHT"       , "PAINT_CODE" , "OPER_TEMP"     , "OPER_PRES" , "DGN_TEMP" , "DGN_PRES" , "TEST_TEMP" , "TEST_PRES", "TEST_FLUID" ,
					"CLASS"   , "INSU_SYMBO" , "INSU_TEMP"  , "INS_THICKNESS" , "DWG_TYPE"  , "RADIOGRAPHY"
				};
				foreach (string sFieldName in BMFieldNameList)
				{
					if (false == LineDataValue.ContainsKey(sFieldName))
					{
						string sValue = string.Empty;
						if ("DRAW_NO" == sFieldName)
						{
							sValue = Path.GetFileNameWithoutExtension(db.Filename);
						}
						/// #10:B/M 생성시 DWG_TYPE 값에 3을 입력(http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/10#ticket) - 2013.09.23 added by humkyung
						else if ("DWG_TYPE" == sFieldName)
						{
							sValue = "3";
						}
						/// up to here
						LineDataValue.Add(sFieldName, sValue);
					}
				}
				/// up to here

				string sSql = string.Empty;
				string Part = string.Empty, WBS = string.Empty;
				/// write header
				try
				{
					Part = IsoBmCreationCommand.LineDataValue["PART"];
					if (IsoBmCreationCommand.LineDataValue.ContainsKey("WBS") && !string.IsNullOrEmpty(IsoBmCreationCommand.LineDataValue["WBS"]))	/// 2014.03.12 added by humkyung
					{
						WBS = IsoBmCreationCommand.LineDataValue["WBS"];
					}
					else
					{
						WBS = IsoBmCreationCommand.LineDataValue["PLANT"] + IsoBmCreationCommand.LineDataValue["AREA"] + IsoBmCreationCommand.LineDataValue["UNIT"];
					}
					sSql = string.Format(@"INSERT INTO HEADER
								(
								JOB_NO     , PART      , DWG_NO     , DWG_PLANT  , DWG_AREA   , DWG_UNIT   , DWG_FLUID  , DWG_SERIAL , DWG_SHEET  , DWG_REV    ,
								PLAN_DWG   , DWG_TYPE  , PID_NO     , LINE_NO    , PWHT       , RADIO_GRAP , PAINT_SYMB , OPER_TEMP  , OPER_PRESS , DESIGN_TEM ,
								DESIGN_PRE , TEST_TEMP , TEST_PRESS , TEST_FLUID , LINE_CLASS , INSU_SYMBO , INSU_TEMP  , INSU_THICK , THICK_UNIT , PC_WBS_CD
								) 
								VALUES 
								(
								'{0}' ,'{1}' ,'{2}' ,'{3}' ,'{4}' ,'{5}' ,'{6}' ,'{7}' ,'{8}' ,'{9}',
								'{10}','{11}','{12}','{13}','{14}','{15}','{16}','{17}','{18}','{19}',
								'{20}','{21}','{22}','{23}','{24}','{25}','{26}','{27}','{28}','{29}'
								)",
									DBValueString(AppDocData.JobNo),
									DBValueString(Part),
									DBValueString(IsoBmCreationCommand.LineDataValue["DRAW_NO"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["PLANT"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["AREA"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["UNIT"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["FLUID"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["SYSTEM_NO"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["SHEET"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["DWG_REV"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["PLAN_DWG"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["DWG_TYPE"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["PID_NO"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["LINE_NO"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["PWHT"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["RADIOGRAPHY"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["PAINT_CODE"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["OPER_TEMP"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["OPER_PRES"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["DGN_TEMP"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["DGN_PRES"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["TEST_TEMP"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["TEST_PRES"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["TEST_FLUID"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["CLASS"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["INSU_SYMBO"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["INSU_TEMP"]),
									DBValueString(IsoBmCreationCommand.LineDataValue["INS_THICKNESS"]),
									"mm",
									DBValueString(WBS)
									);
					if (!string.IsNullOrEmpty(sSql))
					{
						using (OleDbCommand comm = new OleDbCommand(sSql, OleDbConn))
						{
							comm.ExecuteNonQuery();
						}
					}
				}
				catch
				{
					sSql = string.Format(@"INSERT INTO HEADER(JOB_NO,PART,DWG_NO) VALUES ('{0}','{1}','{2}')",
						DBValueString(AppDocData.JobNo),
						DBValueString(Part),
						DBValueString(IsoBmCreationCommand.LineDataValue["DRAW_NO"]));
					using (OleDbCommand comm = new OleDbCommand(sSql, OleDbConn))
					{
						comm.ExecuteNonQuery();
					}
				}

				/// write bm data
				foreach (IsoBMData data in parser.BMDataList)
				{
					sSql = string.Empty;

					/// skip this if desc is null or empty(http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/34) - 2014.01.08 added by humkyung
					//if ((true == AppDocData.QCCheck.CheckBMDescription) && string.IsNullOrEmpty(data.Desc))
					//{
					//    QCCheckCommand.Result oErr = new QCCheckCommand.Result() { Item = data.PtNo, ErrString = "Description is null or empty" };
					//    res.Add(oErr);
					//    continue;
					//}
					/// up to here

					int iType = BmItemType(data.Desc);
					if (data.IsSupport && (SPECIALTY != iType)) continue;	/// skip support data except SPECIAL ITEM

					string MainThk = string.Empty, SubThk = string.Empty, MatlCode = string.Empty;
					if ((null != data.MatlCode) && (12 == data.MatlCode.Length))
					{
						MainThk = data.MatlCode.Substring(8, 2);
						SubThk = data.MatlCode.Substring(10, 2);
						MatlCode = data.MatlCode.Substring(0, 8);
					}
					else if ((null != data.MatlCode) && (8 == data.MatlCode.Length))
					{
						MatlCode = data.MatlCode;
					}
					
					double dSubSize = data.SubSize;
					string sSizeUnit = (!string.IsNullOrEmpty(data.MatlCode) && ('B' == data.MatlCode[0])) ? AppDocData.BoltSizeUnit : AppDocData.SizeUnit;
					switch (iType)
					{
						case GENERAL:
							{
								double dMainSize = data.MainSize;

								/// exchange main size and sub size and then set sub size to empty when item is HALF COUPLING or BOSS - 2014.01.22 added by humkyung
								if ((MatlCode.Length > 1) && (("KW" == MatlCode.Substring(0,2)) || ("KT" == MatlCode.Substring(0,2))))
								{
								    dMainSize = dSubSize;
								    dSubSize = 0;
								}
								//else if ((true == AppDocData.QCCheck.CheckMatlCode) && (8 != MatlCode.Length))
								//{
								//    res.Add(new QCCheckCommand.Result() { Item = data.PtNo, ErrString = "MatlCode Error" });
								//}
								/// up to here
								sSql = @"INSERT INTO ITEM( JOB_NO, PART, PC_WBS_CD, DWG_NO, ISO_SEQ, CLASS_CD, SYMBOL_CD, BM_CD, MAIN_THK, SUB_THK, SIZE_UNIT, MAIN_SIZE,
														SUB_SIZE, DWG_BM_QTY, FS_GUBN)";
								sSql += string.Format(" VALUES( '{0}','{1}','{2}','{3}','{4}','{5}','{6}','{7}','{8}','{9}','{10}','{11}','{12}','{13}','{14}')",
									AppDocData.JobNo, Part, WBS, IsoBmCreationCommand.LineDataValue["DRAW_NO"], data.PtNo, data.Spec, data.Symbol, MatlCode, MainThk, SubThk,
									sSizeUnit, dMainSize, (((0 == dSubSize) || (dMainSize == dSubSize)) ? string.Empty : dSubSize.ToString()), data.Qty, data.ShopField);
							}
							break;
						case INSTRUMENT:
							{
								string sTagNo = string.Empty;
								string[] Tokens = data.Desc.Split(";".ToCharArray());
								if (2 == Tokens.Length)
								{
									sTagNo = Tokens[1];
								}
								sSql = "INSERT INTO INSTRUMENT( JOB_NO, PART, PC_WBS_CD, DWG_NO, CLASS_CD, TAG_NO, MAIN_SIZE, SUB_SIZE, DWG_BM_QTY )";
								sSql += string.Format(" VALUES( '{0}','{1}','{2}','{3}','{4}','{5}','{6}','{7}','{8}')",
									AppDocData.JobNo, Part, WBS, IsoBmCreationCommand.LineDataValue["DRAW_NO"], data.Spec, sTagNo, data.MainSize, ((0 == dSubSize) ? string.Empty : dSubSize.ToString()), data.Qty);
							}
							break;
						case SPECIALTY:
							{
								string sTagNo = string.Empty;
								string[] Tokens = data.Desc.Split(";".ToCharArray());
								if (2 == Tokens.Length)
								{
									sTagNo = Tokens[1];
								}
								sSql = "INSERT INTO SPECIAL( JOB_NO, PART, PC_WBS_CD, DWG_NO, CLASS_CD, TAG_NO, MAIN_SIZE, SUB_SIZE, DWG_BM_QTY )";
								sSql += string.Format(" VALUES( '{0}','{1}','{2}','{3}','{4}','{5}','{6}','{7}','{8}')",
									AppDocData.JobNo, Part, WBS, IsoBmCreationCommand.LineDataValue["DRAW_NO"], data.Spec, sTagNo, data.MainSize, ((0 == dSubSize) ? string.Empty : dSubSize.ToString()), data.Qty);
							}
							break;
					}

					if (!string.IsNullOrEmpty(sSql))
					{
						try
						{
							using (OleDbCommand comm = new OleDbCommand(sSql, OleDbConn))
							{
								comm.ExecuteNonQuery();
							}
						}
						catch (System.Exception ex)
						{
							ed.WriteMessage(ex.Message);
						}
					}
				}
			}

			/// remove the last 4 charcters from Matl Code - 2013.10.21 added by humkyung
			{
				MatlCodeUpdater updater = new MatlCodeUpdater(TableRcdId);
				updater.Update(parser);
			}
			/// up to here

			return res;
		}

		/// <summary>
		/// get bm data type
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.22</date>
		/// <param name="sDesc"></param>
		/// <returns></returns>
		private int BmItemType(string sDesc)
		{
			int res = GENERAL;

			if((0 == sDesc.IndexOf("INSTRUMENT")) || (0 == sDesc.IndexOf("INSTRUMENT;")))
			{
				res = INSTRUMENT;
				return res;
			}
			else if ((-1 != sDesc.IndexOf("PIPING SPECIALTY COMPONENT")) || (-1 != sDesc.IndexOf("SPECIALTY;")))
			{
				res = SPECIALTY;
			}

			return res;
		}

		/// <summary>
		/// get line class from line no. size should be located at start.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.05.28</date>
		/// <param name="sLineNo"></param>
		/// <returns></returns>
		private string GetLineClassFrom(string sLineNo)
		{
			string res = string.Empty;

			int at = sLineNo.IndexOf("\"");
			if (-1 != at)
			{
				sLineNo = sLineNo.Substring(at);
			}
			string[] oTokens = sLineNo.Split("-".ToCharArray());
			if (oTokens.Length >= 4)
			{
				res = oTokens[3];
			}

			return res;
		}

		/// <summary>
		/// write plant,area,wbs code according to unit
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.05.15</date>
		public void DoWBSMapping(OleDbConnection conn)
		{
			string sWBSMappingFile = AppDocData.GetIniValue("App Setting", "WBS Mapping", 256, AppDocData.IniFilePath);
			if (!string.IsNullOrEmpty(sWBSMappingFile) && File.Exists(sWBSMappingFile))
			{
				SpreadsheetInfo.SetLicense(GemBoxLicense);
				ExcelFile ef = new ExcelFile();
				string sExt = Path.GetExtension(sWBSMappingFile);
				if (".XLSX" == sExt.ToUpper())
				{
					ef.LoadXlsx(sWBSMappingFile, XlsxOptions.PreserveMakeCopy);
				}
				else
				{
					ef.LoadXls(sWBSMappingFile, XlsOptions.PreserveAll);
				}

				Dictionary<string, WBSItem> oWBSItemList = new Dictionary<string, WBSItem>();
				/// 2014.03.12 added by humkyung
				string sWBS = (IsoDwgParser.LineDataValue.ContainsKey("WBS") && !string.IsNullOrEmpty(IsoDwgParser.LineDataValue["WBS"])) ? IsoDwgParser.LineDataValue["WBS"] : string.Empty;
				string sPLANT = (IsoDwgParser.LineDataValue.ContainsKey("PLANT") && !string.IsNullOrEmpty(IsoDwgParser.LineDataValue["PLANT"])) ? IsoDwgParser.LineDataValue["PLANT"] : string.Empty;
				string sAREA = (IsoDwgParser.LineDataValue.ContainsKey("AREA") && !string.IsNullOrEmpty(IsoDwgParser.LineDataValue["AREA"])) ? IsoDwgParser.LineDataValue["AREA"] : string.Empty;
				/// up to here

				ExcelWorksheet oSheet = ef.Worksheets[0];
				GemBox.Spreadsheet.CellRange oRange = oSheet.GetUsedCellRange(true);
				int iRowCount = oRange.LastRowIndex - oRange.FirstRowIndex;
				for (int iRow = 1; iRow <= oRange.LastRowIndex; ++iRow)
				{
					WBSItem oItem = new WBSItem()
					{
						/// 2014.03.12 modified by humkyung
						WBSCode = !string.IsNullOrEmpty(sWBS) ? sWBS : oRange[iRow, 1].Value.ToString(),
						Plant = !string.IsNullOrEmpty(sPLANT) ? sPLANT : oRange[iRow, 4].Value.ToString(),
						Area = !string.IsNullOrEmpty(sAREA) ? sAREA : oRange[iRow, 5].Value.ToString(),
						/// up to here
						Unit = oRange[iRow, 6].Value.ToString()
					};
					if (false == oWBSItemList.ContainsKey(oItem.Unit))
					{
						oWBSItemList.Add(oItem.Unit, oItem);
					}
				}

				try
				{
					string[] oTokens = null;

					System.Data.DataTable oHeaderTable = new System.Data.DataTable();
					string sSql = "SELECT DWG_NO,LINE_NO FROM Header";
					OleDbDataAdapter oHeaderAdapter = new OleDbDataAdapter(sSql, conn);
					oHeaderAdapter.Fill(oHeaderTable);
					foreach (System.Data.DataRow oHeaderRow in oHeaderTable.Rows)
					{
						oTokens = oHeaderRow[0].ToString().Split("-".ToCharArray());
						if ((oTokens.Length > 0) && (oWBSItemList.ContainsKey(oTokens[0])))
						{
							/// 2013.05.28 addd by humkyung
							string sFluid = string.Empty;
							string sSerial = string.Empty;
							if (oTokens.Length > 1) sFluid = oTokens[1];
							if (oTokens.Length > 2) sSerial = oTokens[2];
							string sLineClass = GetLineClassFrom(oHeaderRow[1].ToString());
							/// up to here

							//using (OleDbTransaction trans = conn.BeginTransaction())
							{
								sSql = string.Format("UPDATE Header SET PC_WBS_CD='{0}',DWG_PLANT='{1}',DWG_AREA='{2}',DWG_UNIT='{3}',DWG_FLUID='{4}',DWG_SERIAL='{5}',LINE_CLASS='{6}' WHERE DWG_NO='{7}'",
									oWBSItemList[oTokens[0]].WBSCode, oWBSItemList[oTokens[0]].Plant, oWBSItemList[oTokens[0]].Area, oWBSItemList[oTokens[0]].Unit,
									sFluid, sSerial, sLineClass, oHeaderRow[0].ToString());
								OleDbCommand cmd = new OleDbCommand(sSql, conn);
								cmd.ExecuteNonQuery();

								sSql = string.Format("UPDATE Item SET PC_WBS_CD='{0}' WHERE DWG_NO='{1}'", oWBSItemList[oTokens[0]].WBSCode, oHeaderRow[0].ToString());
								cmd.CommandText = sSql;
								cmd.ExecuteNonQuery();

								sSql = string.Format("UPDATE Instrument SET PC_WBS_CD='{0}' WHERE DWG_NO='{1}'", oWBSItemList[oTokens[0]].WBSCode, oHeaderRow[0].ToString());
								cmd.CommandText = sSql;
								cmd.ExecuteNonQuery();

								sSql = string.Format("UPDATE Special SET PC_WBS_CD='{0}' WHERE DWG_NO='{1}'", oWBSItemList[oTokens[0]].WBSCode, oHeaderRow[0].ToString());
								cmd.CommandText = sSql;
								cmd.ExecuteNonQuery();

								//trans.Commit();
							}
						}
					}
					conn.Close();
				}
				catch
				{
					//ed.WirteMessage(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}

		/// <summary>
		/// get fluid and serial from dwg no
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.05.10</date>
		/// <param name="sFluid"></param>
		/// <param name="sSerial"></param>
		/// <param name="sDWGNO"></param>
		/// <returns></returns>
		private bool ParseDWGNO(ref string sFluid, ref string sSerial, string sDWGNO)
		{
			bool res = false;
			string[] oTokens = null;

			oTokens = sDWGNO.Split("-".ToCharArray());
			if (oTokens.Length >= 4)
			{
				sFluid = oTokens[1];
				sSerial = (oTokens[2].Length > 4) ? oTokens[2].Substring(oTokens[2].Length - 4, 4) : string.Empty;
				res = true;
			}

			return res;
		}

		/// <summary>
		/// write B/M data to excel
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.05.09</date>
		/// <param name="ws"></param>
		private void WriteExcel(ExcelWorksheet ws , OleDbConnection conn)
		{
			int row = 3;

			try
			{
				System.Data.DataTable oHeaderTable = new System.Data.DataTable();
				string sSql = "SELECT * FROM Header";
				OleDbDataAdapter oHeaderAdapter = new OleDbDataAdapter(sSql, conn);
				oHeaderAdapter.Fill(oHeaderTable);
				foreach (System.Data.DataRow oHeaderRow in oHeaderTable.Rows)
				{
					ws.Cells[row, 0].Value = string.Format("@{0} {1}", AppDocData.JobNo , oHeaderRow["DWG_NO"].ToString());
					ws.Cells[row, 7].Value = oHeaderRow["PART"].ToString();
					/// write line conditions(#27:http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/27) - 2013.11.26 added by humkyung
					object oValue = oHeaderRow["DWG_PLANT"];
					ws.Cells[row, 8].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_AREA"];
					ws.Cells[row, 9].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_UNIT"];
					ws.Cells[row, 10].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_FLUID"];
					ws.Cells[row, 11].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_SERIAL"];
					ws.Cells[row, 12].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_SHEET"];
					ws.Cells[row, 13].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["DWG_REV"];
					ws.Cells[row, 14].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["INSU_SYMBO"];
					ws.Cells[row, 15].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["INSU_TEMP"];
					ws.Cells[row, 16].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					oValue = oHeaderRow["PAINT_SYMB"];
					ws.Cells[row, 17].Value = (null != oValue) ? oValue.ToString() : string.Empty;
					/// up to here
					++row;

					System.Data.DataTable oTable = new System.Data.DataTable();
					sSql = string.Format("SELECT CLASS_CD,SYMBOL_CD,MAIN_SIZE,SUB_SIZE,DWG_BM_QTY,FS_GUBN,DWG_NO FROM Item WHERE DWG_NO='{0}'", oHeaderRow["DWG_NO"].ToString());
					OleDbDataAdapter oAdapter = new OleDbDataAdapter(sSql, conn);
					oAdapter.Fill(oTable);
					foreach (System.Data.DataRow oRow in oTable.Rows)
					{
						ws.Cells[row, 1].Value = oRow[0].ToString();
						ws.Cells[row, 2].Value = oRow[1].ToString();
						ws.Cells[row, 3].Value = oRow[2].ToString();
						ws.Cells[row, 4].Value = oRow[3].ToString();
						ws.Cells[row, 5].Value = oRow[4].ToString();
						ws.Cells[row, 6].Value = oRow[5].ToString();	/// FIELD / SHOP

						string sFluid = string.Empty, sSerial = string.Empty;
						if (ParseDWGNO(ref sFluid, ref sSerial, oRow[6].ToString()))
						{
							ws.Cells[row, 11].Value = sFluid;
							ws.Cells[row, 12].Value = sSerial;
						}

						++row;
					}
				}

				//using (OleDbTransaction trans = conn.BeginTransaction())
				{
					/// change JOB_NO
					OleDbCommand oCmd = new OleDbCommand(string.Format("UPDATE Item SET JOB_NO='{0}'", AppDocData.JobNo), conn);
					oCmd.ExecuteNonQuery();
					oCmd.CommandText = string.Format("UPDATE Instrument SET JOB_NO='{0}'", AppDocData.JobNo);
					//oCmd.Connection = conn;
					oCmd.ExecuteNonQuery();
					oCmd.CommandText = string.Format("UPDATE Special SET JOB_NO='{0}'", AppDocData.JobNo);
					//oCmd.Connection = conn;
					oCmd.ExecuteNonQuery();
					oCmd.CommandText = string.Format("UPDATE Header SET JOB_NO='{0}'", AppDocData.JobNo);
					//oCmd.Connection = conn;
					oCmd.ExecuteNonQuery();
					/// up to here

					//trans.Commit();
				}
			}
			catch/// (System.Exception ex)
			{
				//MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/// <summary>
		/// create bom summary
		/// </summary>
		/// <param name="sExcelFilePath"></param>
		/// <param name="conn"></param>
		public void CreateBmSummary(string sExcelFilePath, OleDbConnection conn)
		{
			/// 2013.05.09 added by humkyung
			SpreadsheetInfo.SetLicense(GemBoxLicense);
			ExcelFile xls = new ExcelFile();
			xls.LoadXlsx(sExcelFilePath, XlsxOptions.PreserveMakeCopy);
			ExcelWorksheet ws = xls.Worksheets.ActiveWorksheet;
			WriteExcel(ws , conn);
			xls.SaveXlsx(sExcelFilePath);
			/// up to here
		}

		public List<QCCheckCommand.Result> Execute(IsoDwgParser parser) { return null; }
	}
}
