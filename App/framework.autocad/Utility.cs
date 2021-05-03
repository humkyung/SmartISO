using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using acApp = Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.GraphicsInterface;
using Autodesk.AutoCAD.Geometry;

namespace framework.autocad
{
	public class Utility
	{
		/// <summary>
		/// pick a point
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.01.28</date>
		/// <returns></returns>
		//public static Point PickPoint()
		//{
		//	CommonHelper.SetFocusDrawingArea();
		//	PromptPointResult point = AcEx.Editor.GetPoint(new PromptPointOptions("Pick a point :"));
		//	if (((PromptResult) point).Status != Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
		//	  return (PCMS.WMM.CADPlugIn.Common.Point) null;
		//	Point3d point3d1 = point.Value;
		//	double x = ((Point3d) @point3d1).X;
		//	Point3d point3d2 = point.Value;
		//	double y = ((Point3d) @point3d2).Y;
		//	return new PCMS.WMM.CADPlugIn.Common.Point(x, y);
		//}

		public static AcExDbBounds SelectBounds()
		{
			Autodesk.AutoCAD.Internal.Utils.SetFocusToDwgView();
			PromptPointResult point = AcEx.Editor.GetPoint(new PromptPointOptions("Pick a first corner :"));
			if (((PromptResult)point).Status != PromptStatus.OK)
			{
				System.Threading.Thread.Sleep(500);
				return (AcExDbBounds)null;
			}
			Point3d point3d1 = point.Value;
			System.Threading.Thread.Sleep(500);
			PromptPointResult corner = AcEx.Editor.GetCorner(new PromptCornerOptions("Piack a second corner :", point.Value));
			if (((PromptResult)corner).Status != PromptStatus.OK)
			{
				System.Threading.Thread.Sleep(500);
				return (AcExDbBounds)null;
			}
			Point3d point3d2 = corner.Value;
			return new AcExDbBounds(((Point3d)point3d1).X, ((Point3d)point3d1).Y, ((Point3d)point3d2).X, ((Point3d)point3d2).Y);
		}

		///// <summary>
		///// select text in autocad
		///// </summary>
		///// <author>humkyung</author>
		///// <date>2015.01.28</date>
		///// <returns></returns>
		//public static string SelectDBText()
		//{
		//	CommonHelper.SetFocusDrawingArea();
		//	PromptEntityOptions promptEntityOptions = new PromptEntityOptions("DrawingSetupForm.\u0002(24456)");
		//	promptEntityOptions.AllowNone = false;
		//	promptEntityOptions.SetRejectMessage("DrawingSetupForm.\u0002(24477)");
		//	promptEntityOptions.AddAllowedClass(typeof (DBText), false);
		//	PromptEntityResult entity = AcEx.Editor.GetEntity(promptEntityOptions);
		//	if (((PromptResult) entity).Status != Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
		//		return (string) null;
		//	DBText dbObjectAs = AcExDb.GetDBObjectAs<DBText>(entity.ObjectId);
		//	if (dbObjectAs == null) return (string) null;
		//	return dbObjectAs.TextString;
		//}

		///// <summary>
		///// select text in autocad and then return text object
		///// </summary>
		///// <author>humkyung</author>
		///// <date>2015.01.28</date>
		///// <returns></returns>
		//public static DBText SelectDBTextObject()
		//{
		//	CommonHelper.SetFocusDrawingArea();
		//	PromptEntityOptions promptEntityOptions = new PromptEntityOptions("Select a DBText");
		//	promptEntityOptions.AllowNone = false;
		//	promptEntityOptions.SetRejectMessage("DrawingSetupForm.\u0002(24477)");
		//	promptEntityOptions.AddAllowedClass(typeof(DBText), false);
		//	PromptEntityResult entity = AcEx.Editor.GetEntity(promptEntityOptions);
		//	if (((PromptResult)entity).Status != Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
		//		return null;
		//	DBText dbObjectAs = AcExDb.GetDBObjectAs<DBText>(entity.ObjectId);
		//	if (dbObjectAs == null) return null;
		//	return dbObjectAs;
		//}

		///// <summary>
		///// select a entity
		///// </summary>
		///// <author>humkyung</author>
		///// <date>2015.03.10</date>
		///// <returns></returns>
		//public static Entity SelectEntity(string msg)
		//{
		//	CommonHelper.SetFocusDrawingArea();
		//	PromptEntityOptions peo = new PromptEntityOptions(msg);
		//	peo.AllowNone = false;
		//	peo.SetRejectMessage("\nYou're missing, try again >>");
		//	peo.AddAllowedClass(typeof(Entity), false);
		//	PromptEntityResult res = AcEx.Editor.GetEntity(peo);
		//	if (((PromptResult)res).Status != Autodesk.AutoCAD.EditorInput.PromptStatus.OK)
		//		return null;
		//	Entity dbObjectAs = AcExDb.GetDBObjectAs<Entity>(res.ObjectId);
		//	if (dbObjectAs == null) return null;
		//	return dbObjectAs;
		//}
	}
}
