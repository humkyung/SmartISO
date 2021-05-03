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
	public class AcExDbBounds
	{
		public Point3d MinPoint { get; set; }
		public Point3d MaxPoint { get; set; }

		public AcExDbBounds(double x1, double y1, double x2, double y2)
		{
			MinPoint = new Point3d(Math.Min(x1, x2), Math.Min(y1, y2), 0);
			MaxPoint = new Point3d(Math.Max(x1, x2), Math.Max(y1, y2), 0);
		}

		public double GetWidth(Extents3d ext)
		{
			return (MaxPoint.X - MinPoint.X);
		}

		public double GetHeight(Extents3d ext)
		{
			return (MaxPoint.Y - MinPoint.Y);
		}

		public static AcExDbBounds FromString(string str)
		{
			AcExDbBounds res = null;

			if (!string.IsNullOrEmpty(str) && (str.Length > 2))
			{
				string[] oTokens = str.Substring(1, str.Length - 2).Split(',');
				if (4 == oTokens.Length)
				{
					res = new AcExDbBounds(Convert.ToDouble(oTokens[0]), Convert.ToDouble(oTokens[1]), Convert.ToDouble(oTokens[2]), Convert.ToDouble(oTokens[3]));
				}
			}

			return res;
		}

		public override string ToString()
		{
			return string.Format("({0:F3},{1:F3},{2:F3},{3:F3})", MinPoint.X, MinPoint.Y, MaxPoint.X, MaxPoint.Y);
		}
	}
}