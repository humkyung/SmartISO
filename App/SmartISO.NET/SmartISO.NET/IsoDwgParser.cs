using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	public class IsoDwgParser
	{
		private ObjectId BlockTableRcdId;

		public List<IsoBMData> BMDataList = new List<IsoBMData>();
		public List<DBTextIdPair> DrawingAreaTextList = new List<DBTextIdPair>();
		public List<NtrData> NtrDataList = new List<NtrData>();
		public List<FGBPolyline> FGBPolylineList = new List<FGBPolyline>();
		public List<CutPipeLengthData> CutPipeLengthDataList = new List<CutPipeLengthData>();
		public static Dictionary<string, string> LineDataValue = new Dictionary<string,string>();

		public IsoDwgParser(ObjectId objId) { BlockTableRcdId = objId; }
		private static List<string> IndicatorList = new List<string>()
		{
				"SHOP MATERIAL" , "FIELD MATERIAL" , " PT " , " NO " , "CUT PIPE LENGTH" , "PIECE" , "MATERIAL DESCRIPTION" , 
				"PIPE SUPPORTS" , "MISCELLANEOUS COMPONENTS" , "INSTRUMENTS" , "FABRICATION MATERIAL" , "ERECTION MATERIAL"
		};

        enum eCategory
        {
            NONE = 0,
            SHOP = 1,
            FIELD = 2,
            CUT_PIPE_LENGTH = 4
        };

        private static Regex PartNoPattern = new Regex("[0-9]+");

		public class DBTextIdPair
		{
			public DBText text {get; set;}
			public ObjectId objId{get; set;}
		};

		/// <summary>
		/// check given string is indicator
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.08</date>
		/// <param name="str"></param>
		/// <returns></returns>
		private bool IsIndicator(string str)
		{
            foreach (string sIndicator in IndicatorList)
            {
                if (str.Contains(sIndicator)) return true;
            }

            return false;
		}

        /// <summary>
        /// check if given material is shop?
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2014.07.08</date>
        /// <param name="str"></param>
        /// <returns></returns>
        private bool IsShopMaterial(string str)
        {
            return ((str.Contains("SHOP MATERIAL") || str.Contains("FABRICATION MATERIAL")));
        }

        /// <summary>
        /// check if given material is field?
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2014.07.08</date>
        /// <param name="str"></param>
        /// <returns></returns>
        private bool IsFieldMaterial(string str)
        {
            return ((str.Contains("FIELD MATERIAL") || str.Contains("ERECTION MATERIAL")));
        }

        /// <summary>
        /// check if given material is pipe support?
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2014.07.08</date>
        /// <param name="str"></param>
        /// <returns></returns>
        private bool IsPipeSupport(string str)
        {
            return (str.Contains("PIPE SUPPORTS"));
        }

		/// <summary>
		/// check if given str is line seperator(str must be consist of ' ' or '-')
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.08</date>
		/// <param name="str"></param>
		/// <returns></returns>
		private bool IsLineSeperator(string str)
		{
			for (int i = 0; i < str.Length; ++i)
			{
				if ((' ' != str[i]) && ('-' != str[i])) return false;
			}

			return true;
		}

		/// <summary>
		/// parse b/m data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		/// <param name="BMTextList"></param>
		private void ParseBMData(Document doc, List<DBTextIdPair> BMTextList)
		{
			BMTextList.Sort((param1, param2) => (param1.text.Position.Y.CompareTo(param2.text.Position.Y)));
			BMTextList.Reverse();

			/// create bm line with several dbtext has same y coord.
			Dictionary<double, List<DBTextIdPair>> BMLineList = new Dictionary<double, List<DBTextIdPair>>();
			foreach (DBTextIdPair pair in BMTextList)
			{
				List<DBTextIdPair> BMLine = null;
				if (!BMLineList.Keys.Contains(pair.text.Position.Y))
				{
					BMLine = new List<DBTextIdPair>();
					BMLine.Add(pair);
					BMLineList.Add(pair.text.Position.Y, BMLine);
				}
				else
				{
					BMLineList[pair.text.Position.Y].Add(pair);
					BMLineList[pair.text.Position.Y].Sort((param1, param2) => (param1.text.Position.X.CompareTo(param2.text.Position.X)));
				}
			}
			/// up to here

			IsoBMData.ColumnLocList.Clear();
            eCategory category = eCategory.NONE;
			bool isShop = true, isSupport = false;
			foreach (double y in BMLineList.Keys)
			{
				try
				{
					if (IsIndicator(BMLineList[y][0].text.TextString))
					{
						isSupport = false;
                        if (category == eCategory.CUT_PIPE_LENGTH) continue;	/// skip headers below cut pipe length
						if (IsShopMaterial(BMLineList[y][0].text.TextString))
						{
                            category = eCategory.SHOP;
							isShop = true;
							isSupport = false;
						}
						else if (IsFieldMaterial(BMLineList[y][0].text.TextString))
						{
                            category = eCategory.FIELD;
							isShop = false;
							isSupport = false;
						}
						else if (IsPipeSupport(BMLineList[y][0].text.TextString))
						{
							isSupport = true;
						}
						else if (0 == BMLineList[y][0].text.TextString.Trim().IndexOf("NO"))
						{
							if (0 == IsoBMData.ColumnLocList.Count)
							{
								IsoBMData.ParseColumnLoc(BMLineList[y]);
							}
							isSupport = false;
						}
						else if (BMLineList[y][0].text.TextString.Contains("CUT PIPE LENGTH"))
						{
                            category = eCategory.CUT_PIPE_LENGTH;
                            isSupport = false;
//							break;
						}

						continue;
					}

					if (0 == IsoBMData.ColumnLocList.Count) continue;	/// 2014.01.20 added by humkyung
                    if (eCategory.CUT_PIPE_LENGTH == category)
                    {
                        CutPipeLengthDataList.AddRange(CutPipeLengthData.Generate(BMLineList[y]));
                    }
                    else
                    {
                        Match res = PartNoPattern.Match(BMLineList[y][0].text.TextString);
                        double dist = Math.Abs(res.Index - IsoBMData.ColumnLocList["NO"].index);
                        if ((res.Length > 0) && (dist <= "NO".Length))
                        {
                            IsoBMData bm = new IsoBMData((isShop ? "S" : "F")) { IsSupport = isSupport };
                            {
                                bm.Parse(BMLineList[y]);
                                BMDataList.Add(bm);
                            }
                        }
                        else if (!IsLineSeperator(BMLineList[y][0].text.TextString))/// assume this is description!!!
                        {
                            foreach (DBTextIdPair txt in BMLineList[y])
                            {
                                BMDataList[BMDataList.Count - 1].Desc += txt.text.TextString;
                            }
                        }
                    }
				}
				catch (Autodesk.AutoCAD.Runtime.Exception ex)
				{
					doc.Editor.WriteMessage("\nIsoDwgParse Exception: {0}", ex.Message);
				}
			}
		}

		/// <summary>
		/// parse iso bom datas
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <returns></returns>
		public int Parse()
		{
			Document doc = Application.DocumentManager.MdiActiveDocument;
			Database db = doc.Database;
			Editor ed = doc.Editor;

			using (Transaction tr = doc.TransactionManager.StartTransaction())
			{
				List<DBTextIdPair> BMTextList = new List<DBTextIdPair>();
				LineDataValue.Clear();
				DrawingAreaTextList.Clear();
				FGBPolylineList.Clear();

				using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(BlockTableRcdId, OpenMode.ForRead))
				{
					foreach (ObjectId entId in btr)
					{
						Entity ent = tr.GetObject(entId, OpenMode.ForRead) as Entity;
						DBText txt = ent as DBText;
						if ((null != txt) && !string.IsNullOrEmpty(txt.TextString))	
						{
							if(1 == txt.ColorIndex) continue;	/// ignore text when color is red(#http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/19)
							if (AppDocData.IsoAreaMap.Keys.Contains("BM_AREA") && AppDocData.IsoAreaMap["BM_AREA"].Contains(txt.Position))
							{
								BMTextList.Add(new DBTextIdPair(){text = txt,objId = entId});
							}
							else if (AppDocData.IsoAreaMap.Keys.Contains("DRAWING_AREA") && AppDocData.IsoAreaMap["DRAWING_AREA"].Contains(txt.Position))
							{
								DrawingAreaTextList.Add(new DBTextIdPair() { text = txt, objId = entId });
							}
							else if (txt.Position.X < AppDocData.IsoAreaMap["BORDER_AREA"].MinX)
							{
								NtrData oNtrData = new NtrData();
								oNtrData.Parse(new DBTextIdPair() { text = txt, objId = entId });
								NtrDataList.Add(oNtrData);
							}

							foreach (string str in AppDocData.LineDataArea.Keys)
							{
								if (AppDocData.LineDataArea[str].Contains(txt.Position))
								{
									if (!LineDataValue.ContainsKey(str))
									{
										LineDataValue.Add(str, txt.TextString);
									}
									else
									{
										LineDataValue[str] = txt.TextString;
									}
								}
							}
						}
						else if (ent is Polyline3d)
						{
							Polyline oPolyline = ent as Polyline;
							if(FGBPolyline.CanBe(oPolyline))
							{
								FGBPolyline oFGBPolyline = new FGBPolyline();
								oFGBPolyline.Parse(oPolyline);
								FGBPolylineList.Add(oFGBPolyline);
							}
						}
					}
				}
				ParseBMData(doc, BMTextList);

				tr.Commit();
			}

			return 0;
		}

		/// <summary>
		/// get iso bm data has given pt no
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <param name="PtNo"></param>
		/// <returns></returns>
		public IsoBMData GetIsoBMDataWith(string PtNo)
		{
			return BMDataList.Find(param => param.PtNo == PtNo);
		}
	}
}
