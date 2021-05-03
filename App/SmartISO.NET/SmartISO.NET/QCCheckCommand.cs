using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace IsoDwg.NET
{
	public class QCCheckCommand : AbstractCommand
	{
		enum eCheckType
		{
			BM_NOT_FOUND = 0,
			BM_CONSISTENCY_ERROR = 1,
			CAN_NOT_FIND_ISO_DWG_NAME = 2,
			TEXT_MODIFIED = 3,
			ERROR_READ = 4,
			F_G_B_ERROR = 5,
			PIPE_CUTTING_LENGTH = 6,
			NOT_FOUND_ERROR = 7,
			FAIL_ISO_ERROR = 8,
			PCD_SYSMBOL_ERROR = 9,
			HOLD_ITEM_ERROR = 10,
			SPEC_BREAK_ERROR = 11,
			SYMBOL_CODE_NOT_FOUND = 12,
			SUPPORT_BM_ERROR = 13,
			BOLT_LENGTH_ERROR = 14
		};
		public static readonly int CHECK_ERROR_COUNT = 15;

		public class CheckItem
		{
			public bool CheckDwgNo { get; set; }
			public bool CheckErrRead { get; set; }
			public bool CheckPipeCutErr { get; set; }
			public bool CheckNotFoundErr { get; set; }
			public bool CheckFailIsoErr { get; set; }
			public bool CheckPcdSymErr { get; set; }
			public bool CheckHoldItemErr { get; set; }
			public bool CheckSymCodeErr { get; set; }
			public bool CheckSupportBMErr { get; set; }
			public bool CheckTextModify { get; set; }
			public bool CheckFGBErr { get; set; }
			public bool CheckSpecBreakErr { get; set; }
			public bool CheckBoltLength { get; set; }

			/// <summary>
			/// load Q/C Check settings
			/// </summary>
			/// <author>humkyung</author>
			/// <date>2014.05.20</date>
			public void LoadSettings()
			{
				string sApp = "Q/C Check";
				
				CheckDwgNo = ("TRUE" == AppDocData.GetIniValue(sApp , "CAN NOT FIND ISO DWG FILE" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckErrRead = ("TRUE" == AppDocData.GetIniValue(sApp , "ERROR READ" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckPipeCutErr = ("TRUE" == AppDocData.GetIniValue(sApp , "PIPE CUTTING LENGTH" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckNotFoundErr = ("TRUE" == AppDocData.GetIniValue(sApp , "NOT FOUND ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckFailIsoErr = ("TRUE" == AppDocData.GetIniValue(sApp , "FAIL ISO ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckPcdSymErr = ("TRUE" == AppDocData.GetIniValue(sApp , "PCD SYMBOL ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckHoldItemErr = ("TRUE" == AppDocData.GetIniValue(sApp , "HOLD ITEM ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckSymCodeErr = ("TRUE" == AppDocData.GetIniValue(sApp , "SYMBOL CODE NOT FOUND" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckSupportBMErr = ("TRUE" == AppDocData.GetIniValue(sApp , "SUPPORT B/M ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckTextModify = ("TRUE" == AppDocData.GetIniValue(sApp , "TEXT MODIFIED ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckFGBErr = ("TRUE" == AppDocData.GetIniValue(sApp , "F-G-B ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckSpecBreakErr = ("TRUE" == AppDocData.GetIniValue(sApp , "SPEC BREAK ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				CheckBoltLength = ("TRUE" == AppDocData.GetIniValue(sApp , "BOLT LENGTH ERROR" , 256 , AppDocData.IniFilePath).ToUpper());
				QCCheckCommand.BoltLengthTableFilePath = AppDocData.GetIniValue(sApp, "Bolt Length Table", 256, AppDocData.IniFilePath);
			}
		};
		public static int[] TotalErrCount = new int[CHECK_ERROR_COUNT];
		public static string BoltLengthTableFilePath { get; set; }

		public class Result
		{
			public string FileName { get; set; }
			public string Item { get; set; }
			public string ErrString { get; set; }
		};

		public IsoDwgParser parser { get; set; }

		/// <summary>
		/// Quality control check
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sShaFilePath"></param>
		/// <param name="sPrjIniFilePath"></param>
		public List<QCCheckCommand.Result> Execute(Autodesk.AutoCAD.ApplicationServices.Document doc)
		{
			List<QCCheckCommand.Result> res = new List<QCCheckCommand.Result>();

			if (AppDocData.QCCheckItem.CheckDwgNo)
			{
			}

			if (AppDocData.QCCheckItem.CheckTextModify)
			{
			}

			int iErrCount = CheckErrorReadError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.ERROR_READ)]);
			}

			iErrCount = CheckF_G_BError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.F_G_B_ERROR)]);
			}

			iErrCount = CheckPipeCuttingLengthError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.PIPE_CUTTING_LENGTH)]);
			}

			iErrCount = CheckNotFoundError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.NOT_FOUND_ERROR)]);
			}

			iErrCount = CheckIsoFailError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.FAIL_ISO_ERROR)]);
			}

			iErrCount = CheckPCDSymbolError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.PCD_SYSMBOL_ERROR)]);
			}

			iErrCount = CheckHoldItemError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.PCD_SYSMBOL_ERROR)]);
			}

			iErrCount = CheckSpeckBreakError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.SPEC_BREAK_ERROR)]);
			}

			iErrCount = CheckSymbolCodeNotFoundError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.SYMBOL_CODE_NOT_FOUND)]);
			}

			iErrCount = CheckSupportBMError(res);
			if (iErrCount > 0)
			{
				++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.SUPPORT_BM_ERROR)]);
			}

			if (AppDocData.QCCheckItem.CheckBoltLength)
			{
				BoltLengthCheckCommand oCmd = new BoltLengthCheckCommand();
				List<QCCheckCommand.Result> oErrors = oCmd.Execute(parser);
				if (oErrors.Count > 0)
				{
					res.AddRange(oErrors);
					++(QCCheckCommand.TotalErrCount[Convert.ToInt32(eCheckType.BOLT_LENGTH_ERROR)]);
				}
			}

			return res;
		}

		#region CheckErrRead
		/// <summary>
		/// check if there is ERROR READING text in B/M
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.10</date>
		private int CheckErrorReadError(List<QCCheckCommand.Result> oErrRes)
		{
			if (!AppDocData.QCCheckItem.CheckErrRead) return 0;

			const string sErrorRead = "ERROR READING";
			int iErrCount = 0;
			foreach (IsoBMData oBMData in parser.BMDataList)
			{
				if (oBMData.ToString().Contains(sErrorRead)) ++iErrCount;
			}

			if (iErrCount > 0)
			{
				QCCheckCommand.Result oRes = new QCCheckCommand.Result()
				{
					ErrString = string.Format("{0}{1:2}", "(5)  ERROR READ                ", iErrCount)
				};

				oErrRes.Add(oRes);
			}

			return iErrCount;
		}
		#endregion
		#region CheckFGBErr
		/// <summary>
		/// F , G , B가 모두 들어있는지 검사.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.20</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckF_G_BError(List<QCCheckCommand.Result> oErrRes)
		{
			if (false == AppDocData.QCCheckItem.CheckFGBErr) return 0;
			
			int iErrCount = 0;
			Regex FGBPattern = new Regex("^F[0-9]+");
			foreach (IsoDwgParser.DBTextIdPair pair in parser.DrawingAreaTextList)
			{
				if (0 == FGBPattern.Match(pair.text.TextString).Index)
				{
					FGBPattern = new Regex("[?]+G[0-9]+[?]*");
					if (-1 == FGBPattern.Match(pair.text.TextString).Index)
					{
						QCCheckCommand.Result oRes = new QCCheckCommand.Result()
						{
							ErrString = string.Format("{0}{1,-2}{2}", "(6)  F-G-B ERROR               ", ++iErrCount, pair.text.TextString)
						};
						oErrRes.Add(oRes);
						continue;
					}

					FGBPattern = new Regex("[?]+B[0-9]+$");
					if (-1 == FGBPattern.Match(pair.text.TextString).Index)
					{
						QCCheckCommand.Result oRes = new QCCheckCommand.Result()
						{
							ErrString = string.Format("{0}{1,-2}{2}", "(6)  F-G-B ERROR               ", ++iErrCount, pair.text.TextString)
						};
						oErrRes.Add(oRes);
						continue;
					}
				}
			}

			return iErrCount;
		}
		#endregion
		#region CheckPipeCuttingLengthError
		/// <summary>
		/// PIPE CUT ERROR 여부를 검사한다.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.10</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckPipeCuttingLengthError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckPipeCutErr) return 0;

			Dictionary<string, double> PipeLengthMap = new Dictionary<string, double>();
			foreach (IsoBMData data in parser.BMDataList)
			{
				if (data.IsPipe)
				{
					if (true == PipeLengthMap.ContainsKey(data.Size))
					{
						PipeLengthMap[data.Size] += Convert.ToDouble(data.Qty) * 1000;	/// convert M to MM
					}
					else
					{
						PipeLengthMap[data.Size] = Convert.ToDouble(data.Qty) * 1000;	/// convert M to MM
					}
				}
			}

			foreach (string sSize in PipeLengthMap.Keys)
			{
				double dLength = 0;
				foreach (CutPipeLengthData data in parser.CutPipeLengthDataList)
				{
					if (sSize != data.Size) continue;
					dLength += Convert.ToDouble(data.Length);
				}

				if (Math.Abs(PipeLengthMap[sSize] - dLength) > 1)
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("              ### %{0}(NPD) : Pipe Length Error({1} : {2})", sSize, PipeLengthMap[sSize], dLength)
					};
					oErrRes.Add(oRes);
					++iErrCount;
				}
			}

			return iErrCount;
		}
		#endregion
		#region CheckNotFoundError
		/// <summary>
		/// TEXT중 NOT FOUND라는 글자를 포함하는지를 검사한다.
		/// 포함하는 에러
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.10</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckNotFoundError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckNotFoundErr) return 0;
			foreach (IsoDwgParser.DBTextIdPair pair in parser.DrawingAreaTextList)
			{
				if ("NOT FOUND" == pair.text.TextString)
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2}", "(8)  NOT FOUND ERROR           ", ++iErrCount)
					};
					oErrRes.Add(oRes);
				}
			}
			return iErrCount;
		}
		#endregion
		#region CheckIsoFailError
		/// <summary>
		/// TEXT중 FAIL라는 글자를 포함하는지를 검사한다.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.10</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckIsoFailError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckFailIsoErr) return 0;
			foreach (IsoDwgParser.DBTextIdPair pair in parser.DrawingAreaTextList)
			{
				if ("FAIL" == pair.text.TextString)
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2}", "(9)  FAIL ISO ERROR            ", ++iErrCount)
					};
					oErrRes.Add(oRes);
				}
			}
			return iErrCount;
		}
		#endregion
		#region CheckPCDSymbolError
		private int CheckPCDSymbolError(List<QCCheckCommand.Result> oErrRes)
		{
			if (false == AppDocData.QCCheckItem.CheckPcdSymErr) return 0;
			return 0;
		}
		#endregion
		#region CheckHoldItemError
		/// <summary>
		/// TEXT중 HOLD라는 글자를 포함하는지를 검사한다.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.28</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckHoldItemError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckHoldItemErr) return 0;
			foreach (IsoDwgParser.DBTextIdPair pair in parser.DrawingAreaTextList)
			{
				if ("HOLD" == pair.text.TextString)
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2}", "(11) HOLD ITEM ERROR           ", ++iErrCount)
					};
					oErrRes.Add(oRes);
				}
			}
			return iErrCount;
		}
		#endregion
		#region CheckSpecBreakError
		/// <summary>
		/// SPEC BREAK가 일어나는지 검사한다.(SUPPORT의 SPEC은 포함하지 않는다.)
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.10</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckSpeckBreakError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckSpecBreakErr) return 0;
			
			HashSet<string> set = new HashSet<string>();
			foreach (IsoBMData bm in parser.BMDataList)
			{
				if (bm.IsSupport) continue;
				set.Add(bm.Spec);
			}

			if (set.Count > 1)
			{
				foreach (string value in set)
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2} : {3}", "(12) SPEC BREAK ERROR          ", ++iErrCount, value)
					};
					oErrRes.Add(oRes);
				}
			}

			return iErrCount;
		}
		#endregion
		#region CheckSymbolCodeNotFoundError
		/// <summary>
		/// check bm symbol exists or not
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.26</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckSymbolCodeNotFoundError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;

			if (false == AppDocData.QCCheckItem.CheckSymCodeErr) return 0;
			foreach(IsoBMData bm in parser.BMDataList)
			{
				if (string.IsNullOrEmpty(bm.Symbol))
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2} : {3}", "(13) SYMBOL CODE NOT FOUND     ", ++iErrCount, bm.ToString())
					};
					oErrRes.Add(oRes);
				}
			}

			return iErrCount;
		}
		#endregion
		#region CheckSupportBMError
		/// <summary>
		/// B/M란의 support 데이터가 graphic 란에 없으면 error.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.26</date>
		/// <param name="oErrRes"></param>
		/// <returns></returns>
		private int CheckSupportBMError(List<QCCheckCommand.Result> oErrRes)
		{
			int iErrCount = 0;
			if (false == AppDocData.QCCheckItem.CheckSupportBMErr) return 0;
			foreach (IsoBMData bm in parser.BMDataList)
			{
				if (!bm.IsSupport) continue;
				if (null == parser.DrawingAreaTextList.Find(param => param.text.TextString == bm.MatlCode))
				{
					QCCheckCommand.Result oRes = new QCCheckCommand.Result()
					{
						ErrString = string.Format("{0}{1:2} : Support Not Found({2}).", "              ### ", ++iErrCount, bm.MatlCode)
					};
					oErrRes.Add(oRes);
				}
			}

			return iErrCount;
		}
		#endregion

		public List<QCCheckCommand.Result> Execute(IsoDwgParser parser) { return null; }
	}
}
