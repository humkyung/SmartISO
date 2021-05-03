using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;

namespace IsoDwg.NET
{
	class SupportDetail
	{
		public string FilePath { get; set; }
		public Point3d InsertPnt { get; set; }
		public double XScale { get; set; }
		public double YScale { get; set; }
		public double Angle {get; set;}

		static public readonly double Size = 50;
		static public readonly double Scale = 5000.0;

		/// <summary>
		/// create a block by using file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.11.05</date>
		/// <param name="bt"></param>
		/// <returns></returns>
		private List<ObjectId> CreateBlockByUsingFile(Transaction tr)
		{
			List<ObjectId> retids = new List<ObjectId>();

			try
			{
				Document doc = Application.DocumentManager.MdiActiveDocument;
				Database db = doc.Database;
				BlockTable bt = (BlockTable)tr.GetObject(db.BlockTableId, OpenMode.ForRead);

				string sBlockName = Path.GetFileNameWithoutExtension(FilePath);
				if (!bt.Has(sBlockName))
				{
					using (Database newdb = new Database(false, true))
					{
						/// Read the DWG into our side database
						newdb.ReadDwgFile(FilePath, FileShare.Read, true, "");
						retids.Add(db.Insert(sBlockName, newdb, true));
					}
				}
				/// 2012.11.29 added by humkyung
				else
				{
					retids.Add(bt[sBlockName]);
				}
				/// up to here
			}
			catch (Autodesk.AutoCAD.Runtime.Exception ex)
			{
				Application.DocumentManager.MdiActiveDocument.Editor.WriteMessage(ex.Message);
			}

			return retids;
		}

		/// <summary>
		/// create insert entity
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.11.05</date>
		/// <param name="bt"></param>
		/// <param name="btr"></param>
		/// <param name="tm"></param>
		/// <returns></returns>
		public List<ObjectId> CreateEntity(Transaction tr , BlockTableRecord btr)
		{
			List<ObjectId> retids = new List<ObjectId>();

			List<ObjectId> ids = CreateBlockByUsingFile(tr);
			if(ids.Count > 0)
			{
				BlockReference br = new BlockReference(InsertPnt , ids[0]);
				if (null != br)
				{
					Document doc = Application.DocumentManager.MdiActiveDocument;

					br.Rotation = Angle;
					br.ScaleFactors = new Scale3d(XScale);	/// 2012.11.29 added by humkyung
					retids.Add(btr.AppendEntity(br));
					tr.AddNewlyCreatedDBObject(br, true);
				}
			}

			return retids;
		}

	}
}
