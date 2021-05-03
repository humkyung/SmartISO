using System;
using System.Reflection;
using System.Data;
using System.Data.Common;
using System.Windows.Forms;
using System.Data.OleDb;
using System.IO;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.GraphicsInterface;
using Autodesk.AutoCAD.Colors;

using GemBox.Spreadsheet;

namespace IsoDwg.NET
{
	class AppDocData
	{
		public static Dictionary<string, LineDataPos> LineDataPosMap = new Dictionary<string, LineDataPos>();	

		public class ReferenceData
		{
			public string MatlClass = string.Empty;
			public double dFrom = 0.0;
			public double dTo = 0.0;
			public string Desc = string.Empty;
			public string BMCS = string.Empty;
			public string MatlCode = string.Empty;
		}
		public static List<ReferenceData> RefDataList = new List<ReferenceData>();

		///<date>2014.03.18</date>
		///<author>humkyung</author>
		public class TextStyle
		{
			public string Layer { get; set; }
			public string TextStyleName { get; set; }
			public double TextHeight { get; set; }
			public string Justify { get; set; }
			public double Rotation { get; set; }
			public string TextColor { get; set; }
		};

		public class OracleInfo
		{
			public string Host { get; set; }
			public string Service { get; set; }
			public string Id { get; set; }
			public string Password { get; set; }
		};
		public static OracleInfo IDCSOracleInfo = new OracleInfo();
		/// up to here
		static public readonly string GemBoxLicense = "EXK0-347T-N5GE-K8Y1";
		
		static public string JobNo { get; set; }
		static public string IniFilePath { get; set; }

		/// 2014.01.24 added by humkyung
		public class ActionItem
		{
			public bool InsertSupportDetail { get; set; }
			public bool WriteBMCSCode { get; set; }
			public bool WriteRevisionData { get; set; }	/// 2014.03.19 added by humkyung
			public bool RunQCCheck { get; set; }

			/// <summary>
			/// load settings
			/// </summary>
			/// <author>humkyung</author>
			/// <date>2014.01.24</date>
			public void LoadSettings()
			{
				string sApp = "Action";
				InsertSupportDetail = ("True" == AppDocData.GetIniValue(sApp, "Insert Support Detail", 256, AppDocData.IniFilePath));
				WriteBMCSCode = ("True" == AppDocData.GetIniValue(sApp, "Write BMCS Code", 256, AppDocData.IniFilePath));
				RunQCCheck = ("True" == AppDocData.GetIniValue(sApp, "Run Q/C Check", 256, AppDocData.IniFilePath));
				WriteRevisionData = ("True" == AppDocData.GetIniValue(sApp, "Write Revision Data", 256, AppDocData.IniFilePath));	/// 2014.03.19 added by humkyung
			}
		};
		public static ActionItem Action = new AppDocData.ActionItem();
		public static QCCheckCommand.CheckItem QCCheckItem = new QCCheckCommand.CheckItem();
		/// up to here

		public class IsoOutputPath
		{
			public string IsoDwgFilePath { get; set; }
			public string ProductIsoDwgFilePath { get; set; }
			public string TempIsoDwgFilePath { get; set; }
		};

		static public Dictionary<string, IsoOutputPath> IsoDwgFileList = new Dictionary<string, IsoOutputPath>();
		static public Dictionary<string, Area> IsoAreaMap = new Dictionary<string , Area>();
		static public Dictionary<string, Area> RemoveAreaMap = new Dictionary<string, Area>();
		static public Dictionary<string, Area> LineDataArea = new Dictionary<string, Area>();
		
		public class SupportDetailRow
		{
			public string TagNo { get; set; }
			public string ApplySize { get; set; }
			public double FromSize { get; set; }
			public double ToSize { get; set; }
			public string FileName { get; set; }
		}
		//static public DataTable SupportDetailTable = new DataTable();
		static public List<SupportDetailRow> SupportDetailTable = new List<SupportDetailRow>();

		static public string AppDataPath { get; set; }
		static public string TempFolder { get; set; }
		static public string SizeUnit = string.Empty;
		static public string BoltSizeUnit = string.Empty;
		static public int WorkingUnit { get; set; }
		static public string PlanNoFilePath { get; set; }

		/// change to use unicode version - 2011.06.27 modified by humkyung
		[DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
		private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);
		[DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
		private static extern int GetPrivateProfileString(string section, string key, string def, StringBuilder retVal, int size, string filePath);
		/// <summary>
		/// INI 파일에 섹션과 키로 검색하여 값을 Integer형으로 읽어 옵니다.
		/// </summary>
		/// <param name="lpAppname">섹션명입니다.</param>
		/// <param name="lpKeyName">키값명입니다.</param>
		/// <param name="nDefalut">기본값입니다.</param>
		/// <param name="lpFileName">파일 이름입니다.</param>
		/// <returns>입력된 값입니다. 만약 해당 키로 검색 실패시 기본 값으로 대체 됩니다.</returns>
		[DllImport("kernel32.dll", CharSet = CharSet.Unicode)]
		public static extern uint GetPrivateProfileInt(string lpAppName, string lpKeyName, int nDefalut, string lpFileName);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="section"></param>
		/// <param name="key"></param>
		/// <param name="filePath"></param>
		/// <returns></returns>
		static public string GetIniValue(string section, string key, int iSize, string filePath)
		{
			StringBuilder temp = new StringBuilder(iSize);
			int i = GetPrivateProfileString(section, key, "", temp, iSize, filePath);
			return temp.ToString();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="section"></param>
		/// <param name="key"></param>
		/// <param name="val"></param>
		/// <param name="filePath"></param>
		static  public void SetIniValue(string section, string key, string val, string filePath)
		{
			WritePrivateProfileString(section, key, val, filePath);
		}

		/// <summary>
		/// return access provider depending on operationg system
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.12.31</date>
		/// <returns></returns>
		static public string GetAccessProvider()
		{
			string res = "Microsoft.Jet.OLEDB.4.0";
			string sValue = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE").ToString();
			if(0 != sValue.IndexOf("64"))
			{
				return "Microsoft.ACE.OLEDB.12.0";
			}
			
			return res;
		}

		/// <summary>
		/// load reference data for Q/C Check
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.07.23</date>
		/// <param name="sPrjIniFilePath"></param>
		static private void LoadRefData(string sPrjIniFilePath)
		{
			if (0 != RefDataList.Count) return;

			StringBuilder oStrBuilder = new StringBuilder(256);
			string sExlFilePath = GetIniValue("App Setting", "Q/C Check" , 256 , sPrjIniFilePath);
			if (File.Exists(sExlFilePath))
			{
				SpreadsheetInfo.SetLicense(GemBoxLicense);
				ExcelFile ef = new ExcelFile();
				string sExt = Path.GetExtension(sExlFilePath);
				if (".XLSX" == sExt.ToUpper())
				{
					ef.LoadXlsx(sExlFilePath, XlsxOptions.PreserveMakeCopy);
				}
				else
				{
					ef.LoadXls(sExlFilePath, XlsOptions.PreserveAll);
				}

				ExcelWorksheet oSheet = ef.Worksheets[0];
				CellRange oRange = oSheet.GetUsedCellRange(true);
				int iRowCount = oRange.LastRowIndex - oRange.FirstRowIndex;
				for (int iRow = 2; iRow <= oRange.LastRowIndex; ++iRow)
				{
					ReferenceData oRefData = new ReferenceData()
					{
						MatlClass = oRange[iRow, 0].Value.ToString(),
						Desc = oRange[iRow, 1].Value.ToString(),
						dFrom = Convert.ToDouble(oRange[iRow, 3].Value.ToString()),
						dTo = Convert.ToDouble(oRange[iRow, 4].Value.ToString()),
						BMCS = oRange[iRow, 13].Value.ToString(),
						MatlCode = oRange[iRow, 14].Value.ToString()
					};

					RefDataList.Add(oRefData);
				}
			}
			else if (!string.IsNullOrEmpty(sExlFilePath))
			{
				Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
				Editor ed = doc.Editor;
				ed.WriteMessage(System.Environment.NewLine + string.Format("fixisodwg >> Ref data file is not found({0})" , sExlFilePath));
			}
		}

		/// <summary>
		/// load setting from ini file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <param name="sIniFilePath"></param>
		static public void LoadSetting(string sIniFilePath)
		{
			string sApp = "Border Setting";
						
			AppDocData.IniFilePath = sIniFilePath;

			AppDocData.AppDataPath = GetIniValue("General", "AppDataPath", 256, sIniFilePath);
			AppDocData.TempFolder = GetIniValue("General", "Temp Folder", 256, sIniFilePath);
			AppDocData.WorkingUnit = (int)AppDocData.GetPrivateProfileInt("General", "Working Unit", 100000 , sIniFilePath);
			double divider = 1.0 / (double)(AppDocData.WorkingUnit);

			AppDocData.SizeUnit = GetIniValue("ISO Product", "Size Unit", 256, sIniFilePath);
			AppDocData.BoltSizeUnit = GetIniValue("ISO Product", "Bolt Size Unit", 256, sIniFilePath);

			/// read iso dwg file list which has key and file path
			string sValue = string.Empty;
			IsoDwgFileList.Clear();
			uint iCount = GetPrivateProfileInt("Iso Dwg File List", "Count", 256, sIniFilePath);
			for (int i = 0; i < iCount; ++i)
			{
				string[] oTokens = GetIniValue("Iso Dwg File List", i.ToString() , 256, sIniFilePath).Split(",".ToCharArray());
				if (2 == oTokens.Length)
				{
					IsoOutputPath oIsoOutputPath = new IsoOutputPath() { IsoDwgFilePath = oTokens[1] };
					oIsoOutputPath.ProductIsoDwgFilePath = GetIniValue("Product Iso Dwg File List", oTokens[0], 256, sIniFilePath);
					oIsoOutputPath.TempIsoDwgFilePath = GetIniValue("Temp Iso Dwg File List", oTokens[0], 256, sIniFilePath);
					IsoDwgFileList.Add(oTokens[0], oIsoOutputPath);
				}
				else if(1 == oTokens.Length)	/// it has only file path
				{
					IsoOutputPath oIsoOutputPath = new IsoOutputPath() { IsoDwgFilePath = oTokens[0] };
					oIsoOutputPath.ProductIsoDwgFilePath = GetIniValue("Product Iso Dwg File List", i.ToString() , 256, sIniFilePath);
					oIsoOutputPath.TempIsoDwgFilePath = GetIniValue("Temp Iso Dwg File List", i.ToString() , 256, sIniFilePath);
					IsoDwgFileList.Add(i.ToString(), oIsoOutputPath);
				}
			}
			/// up to here
			
			/// get oracle information - 2015.05.20 added by humkyung
			sApp = "Site Oracle Setting";
			AppDocData.IDCSOracleInfo.Host = AppDocData.GetIniValue(sApp, "Host", 256, AppDocData.IniFilePath);
			AppDocData.IDCSOracleInfo.Service = AppDocData.GetIniValue(sApp, "Service", 256, AppDocData.IniFilePath);
			AppDocData.IDCSOracleInfo.Id = AppDocData.GetIniValue(sApp, "Id", 256, AppDocData.IniFilePath);
			AppDocData.IDCSOracleInfo.Password = AppDocData.GetIniValue(sApp, "Password", 256, AppDocData.IniFilePath);
			/// up to here
			LineDataPos.LoadSetting();
			Area.LoadSetting();
			
			/// 2013.08.30 added by humkyung
			PlanNoFilePath = GetIniValue("App Setting", "Plan No File Path", 256, AppDocData.IniFilePath);
			/// up to here
			
			AppDocData.JobNo = AppDocData.GetIniValue("Project Setting", "Job No", 256, AppDocData.IniFilePath);
			
			/// load settings from ini file - 2014.01.24 added by humkyung
			Action.LoadSettings();
			QCCheckItem.LoadSettings();
			/// up to here
		}

		/// <summary>
		/// load reference datas for support detail and qc check
		/// </summary>
		/// <param name="sIniFilePath"></param>
		static public void LoadRefDataForSupportDetail(string sIniFilePath)
		{
			/// load support detail table
			string sSuppDetailTablePath = Path.Combine(AppDocData.AppDataPath, "SupportDetailTable.txt");
			if ((0 == SupportDetailTable.Count) && File.Exists(sSuppDetailTablePath))
			{
				using (StreamReader sr = new StreamReader(sSuppDetailTablePath))
				{
					while (!sr.EndOfStream)
					{
						string oLine = sr.ReadLine();
						string[] Tokens = oLine.Split(",".ToCharArray());
						if (5 == Tokens.Length)
						{
							SupportDetailTable.Add(
								new SupportDetailRow()
									{
										TagNo = Tokens[0],
										ApplySize = Tokens[1],
										FromSize = Convert.ToDouble(Tokens[2]),
										ToSize = Convert.ToDouble(Tokens[3]),
										FileName = Tokens[4]
									}
							);
						}
					}
				}
			}

			//string sSuppDetailTablePath = Path.Combine(AppDocData.AppDataPath, "SupportDetailTable.mdb");
			//if ((0 == SupportDetailTable.Rows.Count) && File.Exists(sSuppDetailTablePath))
			//{
			//    string sPlatform = System.Environment.OSVersion.Platform.ToString();
			//    //string sConnString = string.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source={0}", sSuppDetailTablePath);
			//    string sConnString = string.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source={0}", sSuppDetailTablePath);
			//    using (OleDbConnection conn = new OleDbConnection(sConnString))
			//    {
			//        conn.Open();
			//        OleDbDataAdapter adpater = new OleDbDataAdapter("SELECT * FROM SupportDetailTable", conn);
			//        SupportDetailTable.Locale = System.Globalization.CultureInfo.InvariantCulture;
			//        adpater.Fill(SupportDetailTable);
			//    }
			//}
			//else if (!File.Exists(sSuppDetailTablePath))
			//{
			//    Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
			//    Editor ed = doc.Editor;
			//    ed.WriteMessage(System.Environment.NewLine + "IsoDwg.NET >> Support Detail Table is not found");
			//}

			LoadRefData(sIniFilePath);
		}
	}
}
