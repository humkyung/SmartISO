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

namespace SmartISO.NET
{
	class ExtractDataComandHandler : System.Windows.Input.ICommand
	{
		public Dictionary<string, Extents3d> DataAreaList { get; set; }
		public SortedDictionary<string, List<string>> DataValueList { get; set; }

		public bool CanExecute(object parameter)
		{
			return true;
		}

		public event EventHandler CanExecuteChanged;
		/// <summary>
		/// execute command
		/// </summary>
		/// <param name="parameter"></param>
		public void Execute(object parameter)
		{
			Database database = (Database)parameter;
			Editor ed = Application.DocumentManager.MdiActiveDocument.Editor;

			using (Transaction tr = database.TransactionManager.StartOpenCloseTransaction())
			{
				BlockTable blockTable = (BlockTable)tr.GetObject(database.BlockTableId, OpenMode.ForRead);
				BlockTableRecord blockTableRecord = (BlockTableRecord)tr.GetObject(blockTable[BlockTableRecord.ModelSpace], OpenMode.ForRead);
				using (BlockTableRecordEnumerator enumerator = blockTableRecord.GetEnumerator())
				{
					while (enumerator.MoveNext())
					{
						ObjectId current = enumerator.Current;
						Entity acEnt = tr.GetObject(current, OpenMode.ForWrite) as Entity;
						if (null != acEnt)
						{
							if (acEnt is DBText)
							{
								DBText acDBTxt = acEnt as DBText;
								foreach (string sKey in DataAreaList.Keys)
								{
									if (acDBTxt.Position.X < DataAreaList[sKey].MinPoint.X) continue;
									if (acDBTxt.Position.X > DataAreaList[sKey].MaxPoint.X) continue;
									if (acDBTxt.Position.Y < DataAreaList[sKey].MinPoint.Y) continue;
									if (acDBTxt.Position.Y > DataAreaList[sKey].MaxPoint.Y) continue;

									if (null == DataValueList) DataValueList = new SortedDictionary<string, List<string>>();
									if (!DataValueList.ContainsKey(sKey)) DataValueList[sKey] = new List<string>();
									DataValueList[sKey].Add(acDBTxt.TextString);
								}
							}
						}
					}
				}
				
				tr.Commit();
			}
		}
	}
}
