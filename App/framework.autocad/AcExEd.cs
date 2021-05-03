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
	public class AcExEd
	{
		private AcExEd()
		{
		}

		public static void DrawExtents(Extents3d ext3d, int color, bool drawHighlighted)
		{
			AcExEd.DrawVectors(AcExDbExtents.GetCornerPoints(ext3d), color, drawHighlighted, true);
		}

		public static void DrawVectors(List<Point3d> points, int color, bool drawHighlighted, bool closed)
		{
			if ((points == null || points.Count <= 1)) return;
			for (int index = 0; index < points.Count - 1; ++index)
			{
				AcEx.Editor.DrawVector(points[index], points[index + 1], color, drawHighlighted);
			}
			if (true == closed)
			{
				AcEx.Editor.DrawVector(points[points.Count - 1], points[0], color, drawHighlighted);
			}
		}
	}
}
