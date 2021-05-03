using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	class DBTextRemoveCommand : AbstractCommand
	{
		private ObjectId BlockTableRcdId;
		private static Regex DimTextPattern = new Regex("Dimension [0-9]+");
		public IsoDwgParser parser { get; set; }

		public DBTextRemoveCommand(ObjectId objId) { BlockTableRcdId = objId; }

		/// <summary>
		/// delete text when pattern is matching
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.11.29</date>
		/// <param name="parser"></param>
		/// <returns></returns>
		public List<QCCheckCommand.Result> Execute(Autodesk.AutoCAD.ApplicationServices.Document doc)
		{
			using (DocumentLock acLckDoc = doc.LockDocument())
			{
				using (Transaction tr = doc.TransactionManager.StartTransaction())
				{
					using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(BlockTableRcdId, OpenMode.ForWrite))
					{
						/// remove text in remove area
						foreach (ObjectId entId in btr)
						{
							try
							{
								Entity ent = tr.GetObject(entId, OpenMode.ForWrite) as Entity;
								DBText txt = ent as DBText;
								if ((null == txt) || string.IsNullOrEmpty(txt.TextString)) continue;

								foreach (string sKey in AppDocData.RemoveAreaMap.Keys)
								{
									if (AppDocData.RemoveAreaMap[sKey].Contains(txt.Position))
									{
										txt.Erase(true);
									}
								}
							}
							catch (Autodesk.AutoCAD.Runtime.Exception ex)
							{
								doc.Editor.WriteMessage(ex.Message);
							}
						}
						/// up to here
					}
					tr.Commit();
				}
			}

			return null;
		}

		public List<QCCheckCommand.Result> Execute(IsoDwgParser parser) { return null; }
	}
}
