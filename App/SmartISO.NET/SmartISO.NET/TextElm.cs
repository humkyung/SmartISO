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
    class TextElm : WriteElm
    {
        public Point3d Location{get;set;}
        public double Height {get;set;}
        public double Angle {get;set;}
		public double Oblique { get; set; }
        public string TextString{get;set;}
        public string Justify {get;set;}
		public string TextStyle { get; set; }
		public double WidthFactor { get; set; }

		public TextElm()
		{
			WidthFactor = 1.0;
		}

		/// <summary>
		/// create a text instance
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.12.12</date>
		/// <param name="bt"></param>
		/// <param name="btr"></param>
		/// <param name="tm"></param>
		/// <returns></returns>
		public override List<ObjectId> CreateEntity(BlockTableRecord btr, Transaction tr)
        {
            Document doc = Application.DocumentManager.MdiActiveDocument;
            Database db = doc.Database;
            Editor ed = doc.Editor;
            List<ObjectId> retIds = new List<ObjectId>();

			try
			{
				using(DBText oTxt = new DBText())
				{
					oTxt.TextString = TextString;
					oTxt.Position = Location;
					oTxt.Height = Height;
					oTxt.Rotation = Angle;
					oTxt.Oblique = Oblique;
					oTxt.Normal = ed.CurrentUserCoordinateSystem.CoordinateSystem3d.Zaxis;
					oTxt.Layer = (null != this.Layer) ? Layer : "0";
					oTxt.Color = WriteElm.ParseColor(mColor);
					oTxt.LineWeight = WriteElm.ParseLineWeight(mLweight);
					oTxt.Justify = this.ParseJustify(Justify);
					//oTxt.AlignmentPoint = Location;

					using (TextStyleTable tblStyle = (TextStyleTable)db.TextStyleTableId.GetObject(OpenMode.ForWrite))
					{
						if (tblStyle.Has(TextStyle))
						{
							ObjectId oIdStyle = tblStyle[TextStyle];
							oTxt.TextStyleId = oIdStyle;
							oTxt.WidthFactor = WidthFactor;
						}
					}

					retIds.Add(btr.AppendEntity(oTxt));
					tr.AddNewlyCreatedDBObject(oTxt, true);
				}
			}
			catch (Autodesk.AutoCAD.Runtime.Exception ex)
			{
				ed.WriteMessage("\nTextElm Exception: {0}", ex.Message);
			}

            return retIds;
        }

        /// <summary>
        /// get justify from string
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.10.23</date>
        /// <param name="sJustify"></param>
        /// <returns></returns>
        private AttachmentPoint ParseJustify(string sJustify)
        {
            AttachmentPoint res = AttachmentPoint.BottomLeft;

            switch (sJustify)
            {
				case "Left":
					res = AttachmentPoint.BaseLeft;
					break;
                case "Left Top":
                    res = AttachmentPoint.TopLeft;
                    break;
                case "Left Center":
                    res = AttachmentPoint.MiddleLeft;
                    break;
                case "Left Bottom":
                    res = AttachmentPoint.BottomLeft;
                    break;
				case "Center":
					res = AttachmentPoint.BaseCenter;
					break;
                case "Center Top":
                    res = AttachmentPoint.TopMid;
                    break;
                case "Center Center":
                    res = AttachmentPoint.MiddleMid;
                    break;
                case "Center Bottom":
                    res = AttachmentPoint.BottomMid;
                    break;
				case "Right":
					res = AttachmentPoint.BaseRight;
					break;
                case "Right Top":
                    res = AttachmentPoint.TopRight;
                    break;
                case "Right Center":
                    res = AttachmentPoint.MiddleRight;
                    break;
                case "Right Bottom":
                    res = AttachmentPoint.BottomRight;
                    break;
            }

            return res;
        }
    }
}
