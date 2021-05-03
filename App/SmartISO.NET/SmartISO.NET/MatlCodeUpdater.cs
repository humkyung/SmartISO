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
	class MatlCodeUpdater
	{
		private ObjectId BlockTableRcdId;

		public MatlCodeUpdater(ObjectId objId) { BlockTableRcdId = objId; }

		/// <summary>
		/// update matl code(remove last 4 characters from 12 characters matl code)
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.10.21</date>
		/// <param name="parser"></param>
		/// <returns></returns>
		public int Update(IsoDwgParser parser)
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
							if ((null == data.MatlCode) || data.IsSupport) continue;	/// refer #94(http://172.19.253.16:9001/issues/94) - 2014.04.17 modified by humkyung
							if (12 == data.MatlCode.Length)
							{
								string sUpdateCode = data.MatlCode.Substring(0, 8);
								
								Entity ent = tr.GetObject(data.CodeObjId, OpenMode.ForWrite , false) as Entity;
								DBText txt = ent as DBText;
								if (null != txt)
								{
									txt.TextString = sUpdateCode;
								}
							}
						}
					}
					tr.Commit();
				}
			}

			return 0;
		}
	}
}
