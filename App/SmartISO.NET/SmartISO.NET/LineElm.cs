using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace SmartISO.NET
{
    class LineElm : WriteElm
    {
        public Point3d Start {get;set;}
		public Point3d End{get;set;}

		public List<ObjectId> CreateEntity(BlockTable bt, BlockTableRecord btr, Autodesk.AutoCAD.DatabaseServices.Transaction tr)
        {
            List<ObjectId> res = new List<ObjectId>();

			try
			{
				Polyline acPoly = new Polyline();
				acPoly.SetDatabaseDefaults();
				acPoly.AddVertexAt(0 , new Point2d(Start.X , Start.Y) , 0,0,0);
				acPoly.AddVertexAt(1 , new Point2d(End.X, Start.Y),0,0,0 );
				acPoly.AddVertexAt(2 , new Point2d(End.X, End.Y),0,0,0 );
				acPoly.AddVertexAt(3 , new Point2d(Start.X, End.Y),0,0,0 );
				acPoly.Closed = true;
				//Line oLine = new Line(Start, End);
				{
					acPoly.Layer = "0";
					acPoly.Color = WriteElm.ParseColor("Red");
					acPoly.Linetype = WriteElm.ParseLineType("ByLayer");
					acPoly.LinetypeScale = this.ParseLineScale("ByLayer");
					acPoly.LineWeight = WriteElm.ParseLineWeight("ByLayer");

					res.Add(btr.AppendEntity(acPoly));
					tr.AddNewlyCreatedDBObject(acPoly, true);
				}
			}
			catch (Autodesk.AutoCAD.Runtime.Exception ex)
			{
				
				Document doc = Application.DocumentManager.MdiActiveDocument;
				Editor ed = doc.Editor;

				ed.WriteMessage("\nLineElm Exception: {0}", ex.Message );
			}

            return res;
        }
    }
}
