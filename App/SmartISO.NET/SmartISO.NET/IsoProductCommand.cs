using System;
using System.Windows.Forms;

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
	class IsoProductCommand : AbstractCommand
	{
		public string Key { get; set; }
		public AppOracleDatabase oracle { get; set; }

		static Dictionary<string, string> PlanNoList = new Dictionary<string, string>();

		/// <summary>
		/// write line data and Revision Data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.30</date>
		/// <param name="db"></param>
		private void WriteLineData(IsoDwgParser parser, Document doc, ObjectId BlockTableRcdId, System.Data.DataSet ds , List<QCCheckCommand.Result> oErrList)
		{
			using (DocumentLock acLckDoc = doc.LockDocument())
			{
				using (Transaction tr = doc.TransactionManager.StartTransaction())
				{
					Database db = doc.Database;
					using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(BlockTableRcdId, OpenMode.ForWrite))
					{
						try
						{
                            foreach(System.Data.DataColumn oDataCol in ds.Tables[0].Columns)
                            {
                                string sValue = ds.Tables[0].Rows[0][oDataCol].ToString();
                                if (string.IsNullOrEmpty(sValue)) continue;
                                if (IsoDwgParser.LineDataValue.ContainsKey(oDataCol.ColumnName))
                                {
                                    if (!string.IsNullOrEmpty(IsoDwgParser.LineDataValue[oDataCol.ColumnName])) continue;
                                    IsoDwgParser.LineDataValue[oDataCol.ColumnName] = sValue;
                                }
                                else
                                {
                                    IsoDwgParser.LineDataValue.Add(oDataCol.ColumnName, sValue);
                                }
                            }

							foreach(string sKey in IsoDwgParser.LineDataValue.Keys)
							{
								if(false == AppDocData.LineDataPosMap.ContainsKey(sKey)) continue;
								LineDataPos oLineDataPos = AppDocData.LineDataPosMap[sKey];
								TextElm text = new TextElm()
								{
									TextString = IsoDwgParser.LineDataValue[sKey],
										Height = oLineDataPos.Height,
										Location = new Autodesk.AutoCAD.Geometry.Point3d(oLineDataPos.X, oLineDataPos.Y, 0.0),
										Justify = "Left",
										Layer = "0",
										TextStyle = oLineDataPos.FontName
								};
								text.CreateEntity(btr, tr);
							}
							/// write reivion data - 2013.12.25 added by humkyung
							string sDrawingKey = ds.Tables[0].Rows[0]["KEY"].ToString();
							if(AppDocData.Action.WriteRevisionData) RevisionDataTable.Write(oracle , tr, btr, sDrawingKey );
							/// up to here
						}
						catch (Autodesk.AutoCAD.Runtime.Exception ex)
						{
							doc.Editor.WriteMessage("\nTextElm Exception: {0}", ex.Message);
						}
						catch (System.Exception ex)
						{
							Autodesk.AutoCAD.ApplicationServices.Application.ShowAlertDialog(ex.StackTrace);
						}
					}
					tr.Commit();
				}
			}
		}

		/// <summary>
		/// do it
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="tm"></param>
		public List<QCCheckCommand.Result> Execute(Document doc)
		{
			List<QCCheckCommand.Result> res = new List<QCCheckCommand.Result>();

			try
			{
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

				/// run qc check
				{
					QCCheckCommand oCommand = new QCCheckCommand() { parser = parser };
					res.AddRange(oCommand.Execute(doc));
				}

				/// insert support detail block
				if (true == AppDocData.Action.InsertSupportDetail)
				{
					SupportDetailInserter inserter = new SupportDetailInserter(TableRcdId);
					inserter.PlaceSupportDetail(parser, res);
				}
				
				/// remove text in remove area
				{
					DBTextRemoveCommand remover = new DBTextRemoveCommand(TableRcdId);
					remover.Execute(doc);
				}
				/// up to here

				/// write line data
				using (System.Data.DataSet ds = oracle.ExecuteDataSet(oracle.GetSqlStringCommand(string.Format("SELECT * FROM T_LINE_DATA WHERE KEY='{0}'", Key))))
				{
					WriteLineData(parser, doc, TableRcdId, ds , res);
				}

				using (DocumentLock acLckDoc = doc.LockDocument())
				{
					IsoProductCommand.AddRegAppTableRecord("IDCS4PDMS");
				}
			}
			catch (Autodesk.AutoCAD.Runtime.Exception ex)
			{
				QCCheckCommand.Result oResult = new QCCheckCommand.Result() { Item = "-1", FileName = doc.Database.Filename, ErrString = ex.Message };
				res.Add(oResult);
			}

			return res;
		}

		/// <summary>
		/// check if application table record exists
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.09.27</date>
		/// <param name="regAppName"></param>
		public static bool HasRegAppTableRecord(string regAppName)
		{
			bool res = false;

			Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
			Editor ed = doc.Editor;
			Database db = doc.Database;

			Transaction tr = doc.TransactionManager.StartTransaction();
			using (tr)
			{
				RegAppTable rat = (RegAppTable)tr.GetObject(db.RegAppTableId, OpenMode.ForRead, false);
				res = rat.Has(regAppName);
				tr.Commit();
			}

			return res;
		}

		/// <summary>
		/// add application table record
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.09.27</date>
		/// <param name="regAppName"></param>
		static void AddRegAppTableRecord(string regAppName)
		{
			Document doc = Autodesk.AutoCAD.ApplicationServices.Application.DocumentManager.MdiActiveDocument;
			Editor ed = doc.Editor;
			Database db = doc.Database;

			Transaction tr = doc.TransactionManager.StartTransaction();
			using (tr)
			{
				RegAppTable rat = (RegAppTable)tr.GetObject(db.RegAppTableId,OpenMode.ForRead,false);
				if (!rat.Has(regAppName))
				{
					rat.UpgradeOpen();
					RegAppTableRecord ratr = new RegAppTableRecord();
					ratr.Name = regAppName;
					rat.Add(ratr);
					tr.AddNewlyCreatedDBObject(ratr, true);
				}
				tr.Commit();
			}
		}

		/// <summary>
		/// do something after doing iso product
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.30</date>
		public static void DoProstProcess()
		{
			/// ask user to send producted iso files to server or not - 2014.07.30 added by humkyung
			if (DialogResult.Yes == MessageBox.Show("Do you want to send iso files to server?", "Question", MessageBoxButtons.YesNo, MessageBoxIcon.Question))
			{
				foreach (string sKey in AppDocData.IsoDwgFileList.Keys)
				{
                    File.Copy(AppDocData.IsoDwgFileList[sKey].TempIsoDwgFilePath + ".dwg", AppDocData.IsoDwgFileList[sKey].ProductIsoDwgFilePath + ".dwg" , true);
				}
			}
		}

		public List<QCCheckCommand.Result> Execute(IsoDwgParser parser) { return null; }
	}
}
