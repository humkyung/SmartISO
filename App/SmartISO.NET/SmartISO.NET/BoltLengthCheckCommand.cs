using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Geometry;
using Autodesk.AutoCAD.ApplicationServices;
using System.Reflection;

using System.IO;
using System.Collections;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.GraphicsInterface;
using Autodesk.AutoCAD.Colors;

namespace IsoDwg.NET
{
	public class BoltLengthCheckCommand : AbstractCommand
	{
		/// <summary>
		/// check bolt length error
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.10.01</date>
		/// <param name="parser"></param>
		/// <returns></returns>
		public List<QCCheckCommand.Result> Execute(IsoDwgParser parser)
		{
			List<QCCheckCommand.Result> res = new List<QCCheckCommand.Result>();
			if (!File.Exists(QCCheckCommand.BoltLengthTableFilePath)) return res;

			using (AppOleDatabase database = new AppOleDatabase() { AccessFilePath = QCCheckCommand.BoltLengthTableFilePath })
			{
				foreach (IsoBMData bm in parser.BMDataList)
				{
					if (string.IsNullOrEmpty(bm.MatlCode) || ('B' != bm.MatlCode[0])) continue;
					string sSpec = bm.Spec, sSymbol = bm.Symbol, sMainSize = bm.MainSize.ToString(), sSubSize = bm.SubSize.ToString();

					string sSql;
					sSql = string.Format("SELECT COUNT(*) FROM BoltTable WHERE [CD_CLS]='{0}' AND [CD_SYMB]='{1}' AND [BOLT_DMT]='U {2}' AND [BOLT_LNTH]='{3} mm'",
					sSpec, sSymbol, sMainSize, sSubSize);
					System.Data.DataSet ds = database.ExecuteDataSet(database.GetSqlStringCommand(sSql));
					if (0 == Convert.ToInt32(ds.Tables[0].Rows[0][0]))
					{
						sSql = string.Format("SELECT COUNT(*) FROM BoltTable WHERE [CD_CLS]='{0}' AND [CD_SYMB]='{1}' AND [BOLT_DMT]='U {2}'", sSpec, sSymbol, sMainSize);
						ds = database.ExecuteDataSet(database.GetSqlStringCommand(sSql));
						if (0 == Convert.ToInt32(ds.Tables[0].Rows[0][0]))
						{
							sSql = string.Format("SELECT COUNT(*) FROM BoltTable WHERE [CD_CLS]='{0}' AND [CD_SYMB]='{1}'", sSpec, sSymbol);
							if (0 == Convert.ToInt32(ds.Tables[0].Rows[0][0]))
							{
								sSql = string.Format("SELECT COUNT(*) FROM BoltTable WHERE [CD_CLS]={0}'", sSpec);
								if (0 == Convert.ToInt32(ds.Tables[0].Rows[0][0]))
								{
									QCCheckCommand.Result oRes = new QCCheckCommand.Result()
									{
										ErrString = string.Format("{0}{1,-2} : {2}", "(15) BOLT LENGTH ERROR         ", res.Count + 1, bm.ToString(), "(Bolt Spec Error)")
									};
									res.Add(oRes);
								}
								else
								{
									QCCheckCommand.Result oRes = new QCCheckCommand.Result()
									{
										ErrString = string.Format("{0}{1,-2} : {2}", "(15) BOLT LENGTH ERROR         ", res.Count + 1, bm.ToString(), "(Bolt Symbol Error)")
									};
									res.Add(oRes);
								}
							}
							else
							{
								QCCheckCommand.Result oRes = new QCCheckCommand.Result()
								{
									ErrString = string.Format("{0}{1,-2} : {2}", "(15) BOLT LENGTH ERROR         ", res.Count + 1, bm.ToString(), "(Bolt Size Error)")
								};
								res.Add(oRes);
							}
						}
						else
						{
							QCCheckCommand.Result oRes = new QCCheckCommand.Result()
							{
								ErrString = string.Format("{0}{1,-2} : {2}", "(15) BOLT LENGTH ERROR         ", res.Count + 1, bm.ToString(), "(Bolt Length Error)")
							};
							res.Add(oRes);
						}
					}
				}
			}

			return res;
		}

		public List<QCCheckCommand.Result> Execute(Autodesk.AutoCAD.ApplicationServices.Document doc) { return null; }
	}
}
