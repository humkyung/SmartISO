using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;

namespace framework.autocad
{
	public class AcEx
	{
		public static Database Database
		{
			get
			{
				return HostApplicationServices.WorkingDatabase;
			}
		}

		public static Document Document
		{
			get
			{
				return Application.DocumentManager.MdiActiveDocument;
			}
		}

		public static Editor Editor
		{
			get
			{
				return Application.DocumentManager.MdiActiveDocument.Editor;
			}
		}

		/// <summary>
		/// create application name
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.12.15</date>
		/// <param name="regAppName"></param>
		public static void AddRegAppTableRecord(Database db , string regAppName)
		{
			using(Transaction tr = db.TransactionManager.StartTransaction())
			{
				RegAppTable rat = (RegAppTable)tr.GetObject(db.RegAppTableId, OpenMode.ForRead, false);
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

		private AcEx()
		{
		}
	}
}
