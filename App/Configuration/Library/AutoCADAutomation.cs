using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using AutoCAD;
//using AutoCAD.Interop;
//using AutoCAD.Interop.Common;

namespace CADAutomation
{
    public static class PF
    {
		[DllImport("user32.dll")]
		static extern bool SetForegroundWindow(IntPtr hWnd);

        public static AcadApplication gbl_app;
		public static AcadDocument gbl_doc;
        public static double gbl_pi = 3.14159;
		private static string sAcadID = "AutoCAD.Application.19";

        #region PublicFunctions
        public static void CloseAllInstance()
        {
            Process[] aCAD = Process.GetProcessesByName("acad");

            foreach (Process aCADPro in aCAD)
            {
                aCADPro.CloseMainWindow();
            }
        }

        private static void CreateAutoCADObject()
        {
			try
			{
				gbl_app = (AcadApplication)Marshal.GetActiveObject(sAcadID);
				gbl_app.Application.Visible = true;
			}
			catch (Exception)
			{
				try
				{
					/// CloseAllInstance();
					gbl_app = new AcadApplication();
					gbl_app.Application.Visible = true;
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
        }

		/// <summary>
		/// open AutoCAD file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.09.05</date>
		/// <param name="sFilePath"></param>
		public static void Open(string sFilePath)
		{
			try
			{
				CreateAutoCADObject();
				if ((null != gbl_app) && !string.IsNullOrEmpty(sFilePath) && File.Exists(sFilePath))
				{
					if (sFilePath != gbl_app.ActiveDocument.FullName)
					{
						gbl_app.Documents.Open(sFilePath);
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		/// <summary>
		/// get color corresponding to given color name
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.03.12</date>
		/// <param name="sColor"></param>
		/// <returns></returns>
		public static AcadAcCmColor SetColorWith(AcadAcCmColor color, string sColor)
		{
			color.ColorMethod = AcColorMethod.acColorMethodByRGB;
			switch (sColor.ToUpper())
			{
				case "RED":
					color.SetRGB(255, 0, 0);
					break;
				case "GREEN":
					color.SetRGB(0, 255, 0);
					break;
				case "BLUE":
					color.SetRGB(0, 0, 255);
					break;
			}

			return color;
		}

		/// <summary>
		/// get AutoCAD version number
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.06.12</date>
		/// <returns></returns>
		public static string GetVersionNumber()
		{
			string res = string.Empty;

			Regex regex = new Regex("[0-9]+");
			res = gbl_app.ActiveDocument.Application.Version;
			Match match = regex.Match(res);
			res = res.Substring(match.Index, match.Length);

			return res;
		}

		/// <summary>
		/// draw line
		/// </summary>
		/// <param name="StartX1"></param>
		/// <param name="StartY1"></param>
		/// <param name="EndX2"></param>
		/// <param name="EndY2"></param>
		/// <param name="LineType"></param>
		/// <param name="DrawDonutsOnLineStart"></param>
		/// <param name="DrawDonutsOnLineEnds"></param>
		public static void DrawLine(double StartX1,
	                                    double StartY1,
	                                    double EndX2,
	                                    double EndY2,
	                                    string LineType,
	                                    bool DrawDonutsOnLineStart,
	                                    bool DrawDonutsOnLineEnds,
										string sColor)
		{
			AcadLine lineObj;
			double[] startPoint = new double[3];
			double[] endPoint = new double[3]; ;
	
			startPoint[0] = StartX1;
			startPoint[1] = StartY1;
			startPoint[2] = 0.0;
			endPoint[0] = EndX2;
			endPoint[1] = EndY2;
			endPoint[2] = 0.01;
			lineObj = gbl_doc.ModelSpace.AddLine(startPoint, endPoint);
			SetColorWith(lineObj.TrueColor , sColor);

			if (LineType.Length > 0)
			{
				lineObj.Linetype = LineType; //'"HIDDEN"
				lineObj.LinetypeScale = 10;
				lineObj.Update();
			}
	
			if (DrawDonutsOnLineStart == true)
			{
				DrawDonut((AcadBlock)gbl_doc.ModelSpace, 0, 3.0, StartX1, StartY1);
			}
	
			if (DrawDonutsOnLineEnds == true)
			{
				DrawDonut((AcadBlock)gbl_doc.ModelSpace, 0, 3.0, EndX2, EndY2);
			}
		}

		/// <summary>
		/// draw line
		/// </summary>
		/// <param name="StartX1"></param>
		/// <param name="StartY1"></param>
		/// <param name="EndX2"></param>
		/// <param name="EndY2"></param>
		public static void DrawLine(double StartX1,
	                                    double StartY1,
	                                    double EndX2,
	                                    double EndY2,
										string sColor)
		{

			DrawLine(StartX1, StartY1, EndX2, EndY2, "", false, false, sColor);
		}

		/// <summary>
		/// draw a donut
		/// </summary>
		/// <param name="space"></param>
		/// <param name="inRad"></param>
		/// <param name="outRad"></param>
		/// <param name="cenPt1"></param>
		/// <param name="cenPt2"></param>
		/// <returns></returns>
		public static AcadLWPolyline DrawDonut(AcadBlock space,
	                                                double inRad,
	                                                double outRad,
	                                                double cenPt1,
	                                                double cenPt2)
		{
			double width, radius, PI;
			var tmp = new double[2];
			double[] v = new double[4];
			AcadLWPolyline pl;
			double[] basePnt = new double[3];
	
			try
			{
				/// Switch to terminals layer
				//gbl_doc.ActiveLayer = TerminalsLayer;
	
				basePnt[0] = cenPt1;
				basePnt[1] = cenPt2;
				basePnt[2] = 0.0;
				PI = Math.Atan(1) * 4;
				width = (outRad - inRad) / 2;
				radius = (inRad + width) / 2;
				tmp = gbl_doc.Utility.PolarPoint(basePnt, PI, radius);
				v[0] = tmp[0];
				v[1] = tmp[1];
				tmp = gbl_doc.Utility.PolarPoint(basePnt, 0, radius);
				v[2] = tmp[0];
				v[3] = tmp[1];
				pl = space.AddLightWeightPolyline(v);
	
				pl.Closed = true;
				pl.SetWidth(0, width, width);
				pl.SetBulge(0, -1);
				pl.SetWidth(1, width, width);
				pl.SetBulge(1, -1);
	
				//Switch to other layer
				//gbl_doc.ActiveLayer = SwitchLayer;
	
				return pl;
	
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);
				return null;
			}
		}

		/// <summary>
		/// draw a text
		/// </summary>
		/// <param name="StartingXPoint"></param>
		/// <param name="StartingYPoint"></param>
		/// <param name="textString"></param>
		/// <param name="Height"></param>
		/// <param name="Rotation"></param>
		public static void DrawText(double StartingXPoint,
	                                    double StartingYPoint,
	                                    string textString,
	                                    double Height,
	                                    double Rotation,
										string sColor)
		{
			AcadText textObj;
			double[] insertionPoint = new double[3];
	
			insertionPoint[0] = StartingXPoint;
			insertionPoint[1] = StartingYPoint;
			insertionPoint[2] = 0.0;
			textObj = gbl_doc.ModelSpace.AddText(textString, insertionPoint, Height);
			textObj.Alignment = AcAlignment.acAlignmentLeft;
			textObj.Backward = false;
			textObj.Rotation = Rotation;
			SetColorWith(textObj.TrueColor , sColor);
		}

		/// <summary>
		/// get autocad point
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.03.11</date>
		/// <returns></returns>
		public static double[] GetCADPoint()
		{
			double[] dValue = null;
			
			CreateAutoCADObject();
			gbl_doc = gbl_app.ActiveDocument;
			SetForegroundWindow((IntPtr)gbl_doc.HWND32);

			try
			{
				object oValue = gbl_doc.Utility.GetPoint(Type.Missing, "Pick Point: ");
				dValue = (double[])oValue;
				return dValue;
			}
			catch
			{
			}

			return dValue;
		}

		/// <summary>
		/// get autocad point
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.03.11</date>
		/// <returns></returns>
		public static object[] GetCADFence()
		{
			object[] oValue = new object[2];

			CreateAutoCADObject();
			gbl_doc = gbl_app.ActiveDocument;
			string sName = gbl_doc.Name;
			SetForegroundWindow((IntPtr)gbl_doc.HWND32);

			try
			{
				oValue[0] = gbl_doc.Utility.GetPoint(Type.Missing, "Pick Point: ");
				oValue[1] = gbl_doc.Utility.GetCorner(oValue[0]);
			}
			catch
			{
				oValue = null;
			}

			return oValue;
		}

		/// <summary>
		/// get distanace between two points
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.03.19</date>
		/// <returns></returns>
		public static double GetDistance()
		{
			CreateAutoCADObject();
			gbl_doc = gbl_app.ActiveDocument;
			SetForegroundWindow((IntPtr)gbl_doc.HWND32);

			try
			{
				return gbl_doc.Utility.GetDistance(Type.Missing, "Pick Point: ");
			}
			catch
			{
			}

			return 0;
		}
        #endregion
    }
}
