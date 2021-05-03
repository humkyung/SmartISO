using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;

namespace framework.autocad
{
	public class AcExDb
	{
		private AcExDb()
		{
		}

		public static T GetDBObjectAs<T>(ObjectId id) where T : DBObject
		{
			return AcExDb.Convert<T>(id, false, false);
		}

		public static T GetDBObjectAs<T>(ObjectId id, bool openErased) where T : DBObject
		{
			return AcExDb.Convert<T>(id, openErased, false);
		}

		public static T GetDBObjectAs<T>(ObjectId id, bool openErased, bool forceOpenLockedLayer) where T : DBObject
		{
			return AcExDb.Convert<T>(id, openErased, forceOpenLockedLayer);
		}

		private static T Convert<T>(ObjectId param0, bool param1, bool param2) where T : DBObject
		{
			T obj = null;
			using (Transaction transaction = ((ObjectId) param0).Database.TransactionManager.StartTransaction())
			{
				obj = transaction.GetObject(param0, OpenMode.ForRead , param1, param2) as T;
				transaction.Commit();
			}
			return obj;
		}

		public static List<T> GetDBObjectsAs<T>(List<ObjectId> ids) where T : DBObject
		{
			return AcExDb.Convert<T>(ids, false, false);
		}

		public static List<T> GetDBObjectsAs<T>(List<ObjectId> ids, bool openErased) where T : DBObject
		{
			return AcExDb.Convert<T>(ids, openErased, false);
		}

		private static List<T> Convert<T>(List<ObjectId> param0, bool param1, bool param2) where T : DBObject
		{
			List<T> list = new List<T>();
			ObjectId objectId = param0[0];
			using(Transaction transaction = ((ObjectId) @objectId).Database.TransactionManager.StartTransaction())
			{
				try
				{
					using (List<ObjectId>.Enumerator enumerator = param0.GetEnumerator())
					{
						while (enumerator.MoveNext())
						{
							ObjectId current = enumerator.Current;
							T obj = transaction.GetObject(current, OpenMode.ForRead, param1, param2) as T;
							if(null != obj) list.Add(obj);
						}
					}
					transaction.Commit();
				}
				catch{}
			}
			return list;
		}

		public static T GetSymbolTableAs<T>(Database db = null) where T : SymbolTable
		{
			Type type = typeof(T);
			if (null == db) db = AcEx.Database;
			ObjectId objectId = ObjectId.Null;
			T obj = null;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				if (type == typeof(BlockTable))
				{
					objectId = db.BlockTableId;
				}
				else
				{
					if (type == typeof(DimStyleTable))
					{
						objectId = db.DimStyleTableId;
					}
					else if (type == typeof(LayerTable))
					{
						objectId = db.LayerTableId;
					}
					else if (type == typeof(LinetypeTable))
					{
						objectId = db.LinetypeTableId;
					}
					else if (type == typeof(TextStyleTable))
					{
						objectId = db.TextStyleTableId;
					}
					else if (type == typeof(RegAppTable))
					{
						objectId = db.RegAppTableId;
					}
					else if (type == typeof(UcsTable))
					{
						objectId = db.UcsTableId;
					}
					else if (type == typeof(ViewTable))
					{
						objectId = db.ViewTableId;
					}
					else if(type == typeof(ViewportTable))
					{
						objectId = db.ViewportTableId;
					}

					if (((ObjectId)@objectId).IsNull)
					{
						obj = default(T);
					}
					else
					{
						obj = transaction.GetObject(objectId, OpenMode.ForRead) as T;
					}
				}
			}

			return obj;
		}

		public static LineWeight LineWeightFromDouble(double lineWeight)
		{
			if (lineWeight >= 2.11)
				return LineWeight.LineWeight211;
			if (lineWeight >= 2.0)
				return LineWeight.LineWeight200;
			if (lineWeight >= 1.58)
				return LineWeight.LineWeight158;
			if (lineWeight >= 1.4)
				return LineWeight.LineWeight140;
			if (lineWeight >= 1.2)
				return LineWeight.LineWeight120;
			if (lineWeight >= 1.06)
				return LineWeight.LineWeight106;
			if (lineWeight >= 1.0)
				return LineWeight.LineWeight100;
			if (lineWeight >= 0.9)
				return LineWeight.LineWeight090;
			if (lineWeight >= 0.8)
				return LineWeight.LineWeight080;
			if (lineWeight >= 0.7)
				return LineWeight.LineWeight070;
			if (lineWeight >= 0.6)
				return LineWeight.LineWeight060;
			if (lineWeight >= 0.53)
				return LineWeight.LineWeight053;
			if (lineWeight >= 0.5)
				return LineWeight.LineWeight050;
			if (lineWeight >= 0.4)
				return LineWeight.LineWeight040;
			if (lineWeight >= 0.35)
				return LineWeight.LineWeight035;
			if (lineWeight >= 0.3)
				return LineWeight.LineWeight030;
			if (lineWeight >= 0.25)
				return LineWeight.LineWeight025;
			if (lineWeight >= 0.2)
				return LineWeight.LineWeight020;
			if (lineWeight >= 0.18)
				return LineWeight.LineWeight018;
			if (lineWeight >= 0.15)
				return LineWeight.LineWeight018;
			if (lineWeight >= 0.13)
				return LineWeight.LineWeight013;
			if (lineWeight >= 0.05)
				return LineWeight.LineWeight005;

			return LineWeight.LineWeight000;
		}
	}
}
