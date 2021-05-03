using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Colors;

namespace framework.autocad
{
	public class AcExDbLayer
	{
		public static void Create(string layerName, Database db)
		{
			if (null == db) db = HostApplicationServices.WorkingDatabase;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				try
				{
					LayerTable acLyrTbl = (LayerTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead);
					if (!acLyrTbl.Has(layerName))
					{
						LayerTableRecord acLyrTblRcd = new LayerTableRecord();
						acLyrTblRcd.Name = layerName;
						acLyrTbl.UpgradeOpen();
						acLyrTbl.Add(acLyrTblRcd);
						transaction.AddNewlyCreatedDBObject(acLyrTblRcd, true);
						transaction.Commit();
					}
				}
				finally
				{
				}
			}
		}

		public static void SetColor(string layerName, int colorIndex, Database db = null)
		{
			if (null == db) db = HostApplicationServices.WorkingDatabase;
			using(Transaction transaction = db.TransactionManager.StartTransaction())
			{
				try
				{
					LayerTable acLyrTbl = (LayerTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead);
					if (acLyrTbl.Has(layerName))
					{
						LayerTableRecord acLyrTblRcd = (LayerTableRecord)transaction.GetObject(acLyrTbl[layerName], OpenMode.ForRead);
						((DBObject)acLyrTblRcd).UpgradeOpen();
						acLyrTblRcd.Color = Autodesk.AutoCAD.Colors.Color.FromColorIndex(Autodesk.AutoCAD.Colors.ColorMethod.ByAci, (short)colorIndex);
					}
					transaction.Commit();
				}
				finally
				{
				}
			}
		}

		public static void Create(Database db, string layerName)
		{
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				LayerTable acLyrTbl = (LayerTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead);
				if (!acLyrTbl.Has(layerName))
				{
					LayerTableRecord acLyrTblRcd = new LayerTableRecord();
					((SymbolTableRecord)acLyrTblRcd).Name = layerName;
					((DBObject)acLyrTbl).UpgradeOpen();
					((SymbolTable)acLyrTbl).Add(acLyrTblRcd);
					transaction.AddNewlyCreatedDBObject(acLyrTblRcd, true);
				}
				transaction.Commit();
			}
		}

		public static void SetColor(Database db, string layerName, int colorIndex)
		{
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				try
				{
					LayerTable acLyrTbl= (LayerTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead);
					if (acLyrTbl.Has(layerName))
					{
						LayerTableRecord acLyrTblRcd = (LayerTableRecord)transaction.GetObject(acLyrTbl[layerName], OpenMode.ForRead);
						acLyrTblRcd.UpgradeOpen();
						acLyrTblRcd.Color = Color.FromColorIndex(ColorMethod.ByAci, (short)colorIndex);
					}
					transaction.Commit();
				}
				finally
				{
				}
			}
		}

		public static ObjectId GetLayerReady(string strLayerName, Database db = null)
		{
			if (null == db) db = HostApplicationServices.WorkingDatabase;
			ObjectId objectId = ObjectId.Null;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				LayerTable acLyrTbl = (LayerTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead);
				if (acLyrTbl.Has(strLayerName))
				{
					objectId = acLyrTbl[strLayerName];
				}
				else
				{
					acLyrTbl.UpgradeOpen();
					using (LayerTableRecord acLyrTblRcd = new LayerTableRecord())
					{
						acLyrTblRcd.Name = strLayerName;
						acLyrTblRcd.Color = Color.FromColorIndex(ColorMethod.ByAci, (short)7);
						acLyrTblRcd.IsHidden = false;
						acLyrTblRcd.IsOff = false;
						acLyrTblRcd.IsLocked = true;
						acLyrTblRcd.IsPlottable = true;
						acLyrTblRcd.IsFrozen = false;
						acLyrTbl.Add(acLyrTblRcd);
						transaction.AddNewlyCreatedDBObject(acLyrTblRcd, true);
						objectId = acLyrTblRcd.ObjectId;
					}
					acLyrTbl.DowngradeOpen();
				}
				transaction.Commit();
			}
			return objectId;
		}

		public static HashSet<string> GetLockedLayers(Database db)
		{
			HashSet<string> hashSet = new HashSet<string>();

			if (null == db) db = HostApplicationServices.WorkingDatabase;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				
				using (SymbolTableEnumerator enumerator = ((SymbolTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead)).GetEnumerator())
				{
					while (enumerator.MoveNext())
					{
						ObjectId current = enumerator.Current;
						LayerTableRecord acLyrTblRcd = (LayerTableRecord)transaction.GetObject(current, OpenMode.ForRead);
						if (acLyrTblRcd.IsLocked) hashSet.Add(acLyrTblRcd.Name.ToUpper());
					}
				}
			}
			return hashSet;
		}

		public static void SetUnLockedLayerAll(Database db)
		{
			if (null == db) db = HostApplicationServices.WorkingDatabase;
			using (Transaction transaction = db.TransactionManager.StartTransaction())
			{
				try
				{
					using (SymbolTableEnumerator enumerator = ((SymbolTable)transaction.GetObject(db.LayerTableId, OpenMode.ForRead)).GetEnumerator())
					{
						while (enumerator.MoveNext())
						{
							ObjectId current = enumerator.Current;
							LayerTableRecord acLyrTblRcd = (LayerTableRecord)transaction.GetObject(current, OpenMode.ForRead);
							((DBObject)acLyrTblRcd).UpgradeOpen();
							acLyrTblRcd.IsLocked = false;
						}
					}
					transaction.Commit();
				}
				finally
				{
				}
			}
		}

		public static void On(string layerName, Database db)
		{
		  if (null == db) db = HostApplicationServices.WorkingDatabase;
		  //\u0004.\u0001(layerName, false, db);
		}

		public static void Off(string layerName, Database db)
		{
		  if (null == db) db = HostApplicationServices.WorkingDatabase;
		  //\u0004.\u0001(layerName, true, db);
		}
	}
}
