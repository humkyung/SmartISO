using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Geometry;

namespace ExtractDataFromDwg.NET
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

        public static bool IsActiveDocumentQuiescent
        {
            get
            {
                if (null == AcEx.Document) return false;
                if (string.IsNullOrEmpty(AcEx.Document.CommandInProgress))
                {
                    return AcEx.Editor.IsQuiescent;
                }

                return false;
            }
        }

        /// <summary>
        /// create application name
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2014.12.15</date>
        /// <param name="regAppName"></param>
        public static void AddRegAppTableRecord(Database db, string regAppName)
        {
            using (Transaction txn = db.TransactionManager.StartTransaction())
            {
                RegAppTable acRegAppTbl = (RegAppTable)txn.GetObject(db.RegAppTableId, OpenMode.ForRead, false);
                if (!acRegAppTbl.Has(regAppName))
                {
                    acRegAppTbl.UpgradeOpen();
                    RegAppTableRecord acRegAppTblRcd = new RegAppTableRecord();
                    acRegAppTblRcd.Name = regAppName;
                    acRegAppTbl.Add(acRegAppTblRcd);
                    txn.AddNewlyCreatedDBObject(acRegAppTblRcd, true);
                }
                txn.Commit();
            }
        }

		public static Bounds SelectBounds()
		{
			Autodesk.AutoCAD.Internal.Utils.SetFocusToDwgView();
			PromptPointResult point = AcEx.Editor.GetPoint(new PromptPointOptions("Pick a first corner :"));
			if (((PromptResult)point).Status != PromptStatus.OK)
			{
				System.Threading.Thread.Sleep(500);
				return (Bounds)null;
			}
			Point3d point3d1 = point.Value;
			System.Threading.Thread.Sleep(500);
			PromptPointResult corner = AcEx.Editor.GetCorner(new PromptCornerOptions("Piack a second corner :", point.Value));
			if (((PromptResult)corner).Status != PromptStatus.OK)
			{
				System.Threading.Thread.Sleep(500);
				return (Bounds)null;
			}
			Point3d point3d2 = corner.Value;
			return new Bounds(((Point3d)@point3d1).X, ((Point3d)@point3d1).Y, ((Point3d)@point3d2).X, ((Point3d)@point3d2).Y);
		}

        private AcEx()
        {
        }
    }
}
