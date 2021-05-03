using System;
using System.Data;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	class BMCSCodeInserter
	{
		private ObjectId BlockTableRcdId;

		public BMCSCodeInserter(ObjectId objId) { BlockTableRcdId = objId; }

		/// <summary>
		/// insert bmcs code when item is gasket or bolt and bmcs code is missing
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.20</date>
		/// <param name="parser"></param>
		/// <returns></returns>
		public int Insert(IsoDwgParser parser)
		{
			Document doc = Application.DocumentManager.MdiActiveDocument;

			using (DocumentLock acLckDoc = doc.LockDocument())
			{
				using (Transaction tr = doc.TransactionManager.StartTransaction())
				{
					using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(BlockTableRcdId, OpenMode.ForWrite))
					{
						foreach (IsoBMData data in parser.BMDataList)
						{
							if (!string.IsNullOrEmpty(data.Desc) && ((-1 != data.Desc.IndexOf("GASKET")) || (-1 != data.Desc.IndexOf("BOLT"))) && string.IsNullOrEmpty(data.Symbol))
							{
								if (-1 != data.Desc.IndexOf("GASKET"))
								{
									data.Symbol = "G";
								}
								else if (-1 != data.Desc.IndexOf("BOLT"))
								{
									data.Symbol = "BT";
								}

								TextElm text = new TextElm()
								{
									TextString = data.Symbol,
									Height = 2.7,
									Location = new Autodesk.AutoCAD.Geometry.Point3d(IsoBMData.ColumnLocList["SYM"].loc + 2.3, data.Y, 0.0),
									Justify = "Left",
									TextStyle = "Arial",
									Layer = "MATLIST"
								};
								text.CreateEntity(btr, tr);
							}

							/// remove / and next one char from Spec if spec start with "/" - 2013.12.11 added by humkyung
							if (!string.IsNullOrEmpty(data.Spec) && (0 == data.Spec.IndexOf("/")))
							{
								string sRemovedStr = string.Empty;

								Entity ent = tr.GetObject(data.SpecObjId, OpenMode.ForWrite, false) as Entity;
								DBText txt = ent as DBText;
								if (null != txt)
								{
									sRemovedStr = txt.TextString.Substring(0, 2);
									string sUpdatedSpec = txt.TextString.Substring(2);
									txt.TextString = sUpdatedSpec;
									txt.Position = new Autodesk.AutoCAD.Geometry.Point3d(IsoBMData.ColumnLocList["SPEC"].loc , txt.Position.Y , txt.Position.Z);
								}

								ent = tr.GetObject(data.SizeObjId, OpenMode.ForWrite, false) as Entity;
								txt = ent as DBText;
								if (null != txt)
								{
									string sUpdatedSize = txt.TextString + sRemovedStr;
									txt.TextString = sUpdatedSize;
								}
							}
							/// up to here
						}
					}
					tr.Commit();
				}
			}

			return 0;
		}
	}
}
