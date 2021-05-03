using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	public class FGBPolyline : Area
	{
		/// <summary>
		/// check given polyline is located in DRAWING_AREA
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.26</date>
		/// <param name="oPolyline"></param>
		/// <returns></returns>
		private static bool IsLocatedInDrawingArea(Polyline oPolyline)
		{
			Area oArea = AppDocData.IsoAreaMap["DRAWING_AREA"];
			for(int i = 0;i < oPolyline.NumberOfVertices;++i)
			{
				if(false == oArea.Contains(oPolyline.GetPoint3dAt(i))) return false;
			}
			
			return true;
		}

		/// <summary>
		/// check given polyline is FGBPolyline
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.26</date>
		/// <param name="oPolyline"></param>
		/// <returns></returns>
		public static bool CanBe(Polyline oPolyline)
		{
			if (FGBPolyline.IsLocatedInDrawingArea(oPolyline))
			{
				return (oPolyline.GetPoint3dAt(0) == oPolyline.GetPoint3dAt(oPolyline.NumberOfVertices - 1));
			}

			return false;
		}

		/// <summary>
		/// parse given polyline
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.26</date>
		/// <param name="oPolyline"></param>
		/// <returns></returns>
		public bool Parse(Polyline oPolyline)
		{
			MinX = Double.MaxValue;
			MinY = Double.MaxValue;
			MaxX = Double.MinValue;
			MaxY = Double.MinValue;
			for (int i = 0; i < oPolyline.NumberOfVertices; ++i)
			{
				Autodesk.AutoCAD.Geometry.Point3d pt = oPolyline.GetPoint3dAt(i);
				MinX = Math.Min(MinX, pt.X);
				MinY = Math.Min(MinY, pt.Y);
				MaxX = Math.Max(MaxX, pt.X);
				MaxY = Math.Max(MaxY, pt.Y);
			}

			return true;
		}
	}
}
