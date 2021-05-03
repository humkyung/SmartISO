using System;
using System.Text;
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

[assembly: CommandClass(typeof(SmartISO.NET.IsoDwg))]

namespace SmartISO.NET
{
	public class IsoDwg
	{
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

		static public readonly string GemBoxLicense = "EXK0-347T-N5GE-K8Y1";

		/// <summary>
		/// setting file
		/// </summary>
		public static string DatabaseFilePath { get; set; }

		/// <summary>
		/// get ini value
		/// </summary>
		/// <param name="section"></param>
		/// <param name="key"></param>
		/// <param name="filePath"></param>
		/// <returns></returns>
		static public string GetIniValue(string section, string key, int iSize, string filePath)
		{
			StringBuilder temp = new StringBuilder(iSize);
			int i = GetPrivateProfileString(section, key, string.Empty , temp, iSize, filePath);
			return temp.ToString();
		}

		private static Matrix3d UCS
		{
			get
			{
				return Application.DocumentManager.MdiActiveDocument.Editor.CurrentUserCoordinateSystem;
			}
		}

		private static void ConvertWork(object sender, EventArgs e)
		{
			int d = 1;
			int c = d;
		}

		[CommandMethod("DrawBorderSize" , CommandFlags.Modal | CommandFlags.Session)]
		static public void DrawBorderSize()
		{
			var doc = Application.DocumentManager.MdiActiveDocument;
			var db_ = doc.Database;
			var ed = doc.Editor;

			PromptStringOptions opts = new PromptStringOptions("\nEnter Border Size:");
			PromptResult res = ed.GetString(opts);
			if (PromptStatus.OK == res.Status)
			{
				string[] aTokens = res.StringResult.Split(",".ToCharArray());
				if(4 == aTokens.Length)
				{
					using(Transaction tr = doc.Database.TransactionManager.StartTransaction())
					{
						BlockTable bt = (BlockTable)tr.GetObject(doc.Database.BlockTableId, OpenMode.ForRead, false);
						BlockTableRecord btr = (BlockTableRecord)tr.GetObject(bt[BlockTableRecord.ModelSpace], OpenMode.ForWrite, false);

						LineElm aLineElm = new LineElm()
						{
							Start = new Point3d(Convert.ToDouble(aTokens[0]),Convert.ToDouble(aTokens[1]),0),
							End = new Point3d(Convert.ToDouble(aTokens[2]) , Convert.ToDouble(aTokens[3]),0)
						};
						aLineElm.CreateEntity(bt , btr,tr);

						tr.Commit();
					}
				}
			}
		}

		/// <summary>
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.03.31</date>
		[CommandMethod("ExtractDataFromDwg", CommandFlags.Modal | CommandFlags.Session)]
		static public void DoIt()
		{
			var doc = Application.DocumentManager.MdiActiveDocument;
			var db_ = doc.Database;
			var ed = doc.Editor;

			PromptStringOptions opts = new PromptStringOptions("\nEnter Setting File Path:");
			PromptResult res = ed.GetString(opts);
			if (PromptStatus.OK == res.Status)
			{
				DatabaseFilePath = res.StringResult;
				{
					ProgressLogDlg.Instance.StartWork += new EventHandler(ConvertWork);
					using (AppSQLiteDatabase sqlite = new AppSQLiteDatabase() { FilePath = DatabaseFilePath })
					{
						Dictionary<string, Extents3d> DataAreaList = new Dictionary<string, Extents3d>();
						{
							using (System.Data.DataSet ds = sqlite.ExecuteDataSet(sqlite.GetSqlStringCommand("SELECT * FROM Setting")))
							{
								foreach (System.Data.DataRow aRow in ds.Tables[0].Rows)
								{
									if (("Data1Area" == aRow["Name"].ToString()) || ("Data2Area" == aRow["Name"].ToString()) || ("Data3Area" == aRow["Name"].ToString()) ||
										("Data4Area" == aRow["Name"].ToString()) || ("Data5Area" == aRow["Name"].ToString()) || ("Data6Area" == aRow["Name"].ToString()) ||
										("Data7Area" == aRow["Name"].ToString()) || ("Data8Area" == aRow["Name"].ToString()) || ("Data9Area" == aRow["Name"].ToString()))
									{
										string[] aTokens = aRow["Value"].ToString().Split(",".ToCharArray());
										if (4 == aTokens.Length)
										{
											double x1 = Convert.ToDouble(aTokens[0]);
											double y1 = Convert.ToDouble(aTokens[1]);
											double x2 = Convert.ToDouble(aTokens[2]);
											double y2 = Convert.ToDouble(aTokens[3]);
											DataAreaList.Add(aRow["Name"].ToString() , 
												new Extents3d(
												new Point3d(Math.Min(x1, x2), Math.Min(y1, y2), 0),
												new Point3d(Math.Max(x1, x2), Math.Max(y1, y2), 0)
												));
										}
									}
								}
							}
						}

						using (System.Data.DataSet ds = sqlite.ExecuteDataSet(sqlite.GetSqlStringCommand("SELECT * FROM DwgFileList")))
						{
							/// scale = pJ-gen circle radius / ipJ-gen circle radius(0.35 / 8.25)
							Application.ShowModelessDialog(Application.MainWindow.Handle, (System.Windows.Forms.Form)ProgressLogDlg.Instance, false);
							ProgressLogDlg.Instance.listViewLog.Items.Clear();
							ProgressLogDlg.Instance.buttonClose.Enabled = false;

							ProgressLogDlg.Instance.progressBarStatus.Maximum = ds.Tables[0].Rows.Count;
							ProgressLogDlg.Instance.progressBarStatus.Minimum = 0;
							ProgressLogDlg.Instance.progressBarStatus.Step = 1;
							foreach (System.Data.DataRow aRow in ds.Tables[0].Rows)
							{
								ProgressLogDlg.Instance.progressBarStatus.PerformStep();
								string sFilePath = aRow[0].ToString();
								ProgressLogDlg.Instance.textBoxFileName.Text = Path.GetFileName(sFilePath);
								System.Windows.Forms.Application.DoEvents();
								if (!File.Exists(sFilePath)) continue;

								Database oldDb = HostApplicationServices.WorkingDatabase;	/// save current database
								using (Database database = new Database(true, true))
								{
									database.ReadDwgFile(sFilePath, FileOpenMode.OpenForReadAndReadShare, true, string.Empty);
									ExtractDataComandHandler aCommand = new ExtractDataComandHandler()
									{
										DataAreaList = DataAreaList
									};
									aCommand.Execute(database);

									string sFileName = Path.GetFileNameWithoutExtension(sFilePath);
									System.Windows.Forms.ListViewGroup oGroup = ProgressLogDlg.Instance.listViewLog.Groups.Add(sFileName, sFileName);
									if (null != aCommand.DataValueList)
									{
										foreach (string sKey in aCommand.DataValueList.Keys)
										{
											foreach (string sValue in aCommand.DataValueList[sKey])
											{
												System.Windows.Forms.ListViewItem oItem = ProgressLogDlg.Instance.listViewLog.Items.Add(sKey);
												oItem.SubItems.Add(sValue);
												oItem.Group = oGroup;
											}
										}
									}
								}
							}
								
							ProgressLogDlg.Instance.buttonClose.Enabled = true;
						}
					}
				}
			}
		}
	}
}
