using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Colors;
using Autodesk.AutoCAD.Geometry;

namespace framework.autocad
{
	public class AcExDbExtents
	{
		private AcExDbExtents()
		{
		}

		public static double GetWidth(Extents3d ext)
		{
			return (ext.MaxPoint.X - ext.MinPoint.X);
		}

		public static double GetHeight(Extents3d ext)
		{
			return (ext.MaxPoint.Y - ext.MinPoint.Y);
		}

		public static List<Point3d> GetCornerPoints(Extents3d ext)
		{
			List<Point3d> list = new List<Point3d>();
			Point3d point3d1 = new Point3d(ext.MinPoint.X , ext.MinPoint.Y, 0);
			list.Add(new Point3d(ext.MinPoint.X, ext.MinPoint.Y, 0));
			list.Add(new Point3d(ext.MaxPoint.X, ext.MinPoint.Y, 0));
			list.Add(new Point3d(ext.MaxPoint.X , ext.MaxPoint.Y , 0));
			list.Add(new Point3d(ext.MinPoint.X , ext.MaxPoint.Y , 0));
			return list;
		}
	}
}
