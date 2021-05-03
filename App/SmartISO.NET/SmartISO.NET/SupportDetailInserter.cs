using System;
using System.Data;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.ApplicationServices;
using Autodesk.AutoCAD.EditorInput;

namespace IsoDwg.NET
{
	class SupportDetailInserter
	{
		private ObjectId BlockTableRcdId;
		public class SupportTag
		{
			public string TagNo { get; set; }
			public string PtNo { get; set; }
		}
		public List<SupportTag> SupportTagList = new List<SupportTag>();

		public SupportDetailInserter(ObjectId objId) { BlockTableRcdId = objId; }

		/// <summary>
		/// find support tag
		/// </summary>
		/// <author>humkyung</author>
		/// <returns></returns>
		private int Find()
		{
			Document doc = Application.DocumentManager.MdiActiveDocument;
			Database db = doc.Database;
			Editor ed = doc.Editor;

			if (false == AppDocData.IsoAreaMap.Keys.Contains("DRAWING_AREA")) return -1;

			using (Transaction tr = doc.TransactionManager.StartTransaction())
			{
				Regex objNaturalPattern = new Regex("^S[0-9]+$");

				using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(BlockTableRcdId, OpenMode.ForRead))
				{
					foreach (ObjectId entId in btr)
					{
						Entity ent = tr.GetObject(entId, OpenMode.ForRead) as Entity;
						DBText txt = ent as DBText;
						if ((null != txt) && AppDocData.IsoAreaMap["DRAWING_AREA"].Contains(txt.Position))
						{
							if (!string.IsNullOrEmpty(txt.TextString) && objNaturalPattern.IsMatch(txt.TextString))
							{
								DBText txtTag = FindUsing(tr, btr, txt);
								if (null != txtTag)
								{
									string[] Tokens = null;
									/// check if support tag name has space - 2014.01.14 added by humkyung
									if (txtTag.TextString.Contains(' '))
									{
										string str = txtTag.TextString;
										str = str.Replace(" ", string.Empty);
										Tokens = new string[1] { str };
									}
									/// up to here
									else
									{
										Tokens = txtTag.TextString.Split("-".ToCharArray());
									}

									if (null != Tokens)
									{
										SupportTag tag = SupportTagList.Find(param => param.TagNo == Tokens[0]);
										if (null == tag)
										{
											tag = new SupportTag() { TagNo = Tokens[0], PtNo = txt.TextString.Substring(1) };
											SupportTagList.Add(tag);
										}
									}
								}
							}
						}
					}
				}
				tr.Commit();
			}

			return 0;
		}

		/// <summary>
		/// get support block file name
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <returns></returns>
		private string GetSupportBlockFileName(string sTagNo , double dSize)
		{
			string res = string.Empty;
			foreach (AppDocData.SupportDetailRow oRow in AppDocData.SupportDetailTable)
			{
				if (sTagNo == oRow.TagNo)
				{
					if ((oRow.FromSize - dSize) * (oRow.ToSize - dSize) <= 0.0)
					{
						res = oRow.FileName;
						break;
					}
				}
			}
			//foreach (DataRow oRow in AppDocData.SupportDetailTable.Rows)
			//{
			//    if (sTagNo == oRow["TagNo"].ToString())
			//    {
			//        if((Convert.ToDouble(oRow["From_Size"]) - dSize)*(Convert.ToDouble(oRow["To_Size"]) - dSize) < 0.0)
			//        {
			//            res = oRow["FileName"].ToString();
			//            break;
			//        }
			//    }
			//}

			return res;
		}

		/// <summary>
		/// place support detail
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <returns></returns>
		public int PlaceSupportDetail(IsoDwgParser parser, List<QCCheckCommand.Result> oErrList)
		{
			Document doc = Application.DocumentManager.MdiActiveDocument;
			Database db = doc.Database;
			Editor ed = doc.Editor;

			Find();
			using (DocumentLock acLckDoc = doc.LockDocument())
			{
				using (Transaction tr = doc.TransactionManager.StartTransaction())
				{
					/// Open the Block table for read
					BlockTable acBlkTbl;
					acBlkTbl = tr.GetObject(db.BlockTableId, OpenMode.ForRead) as BlockTable;

					using (BlockTableRecord btr = (BlockTableRecord)tr.GetObject(acBlkTbl[BlockTableRecord.ModelSpace], OpenMode.ForWrite) as BlockTableRecord)
					{
						int index = 0;
						foreach (SupportTag tag in SupportTagList)
						{
							try
							{
								IsoBMData BmData = parser.GetIsoBMDataWith(tag.PtNo);
								if (null != BmData)
								{
									string sFileName = GetSupportBlockFileName(tag.TagNo, BmData.MainSize);
									string sFilePath = Path.Combine(Path.Combine(AppDocData.AppDataPath, "Support_Detail"), sFileName + ".dwg");
									if (File.Exists(sFilePath))
									{
										SupportDetail insert = new SupportDetail()
										{
											InsertPnt = new Autodesk.AutoCAD.Geometry.Point3d(AppDocData.IsoAreaMap["DRAWING_AREA"].MinX + index * SupportDetail.Size, AppDocData.IsoAreaMap["DRAWING_AREA"].MinY - SupportDetail.Size, 0.0),
											FilePath = sFilePath,
											XScale = SupportDetail.Scale,
											YScale = SupportDetail.Scale
										};
										insert.CreateEntity(tr, btr);

										++index;
									}
								}
							}
							catch (Exception ex)
							{
								oErrList.Add(new QCCheckCommand.Result() { Item = tag.PtNo, ErrString = ex.StackTrace });
							}
						}
					}
					tr.Commit();
				}
			}
			
			return 0;
		}

		/// <summary>
		/// find support tag using given text
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <param name="txt"></param>
		/// <returns></returns>
		public DBText FindUsing(Transaction tr , BlockTableRecord btr , DBText txt)
		{
			DBText res = null;
			
			double min = Double.MaxValue;
			foreach (ObjectId entId in btr)
			{
				Entity ent = tr.GetObject(entId, OpenMode.ForRead) as Entity;
				DBText tag = ent as DBText;
				if ((null != tag) && (tag.Position.X == txt.Position.X) && (tag.Position.Y < txt.Position.Y))
				{
					double dist = txt.Position.DistanceTo(tag.Position);
					if (dist < min)
					{
						min = dist;
						res = tag;
					}
				}
			}
			
			return res;
		}
	}
}
