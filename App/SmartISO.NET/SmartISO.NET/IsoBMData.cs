using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

using Autodesk.AutoCAD.DatabaseServices;

namespace IsoDwg.NET
{
	public class IsoBMData
	{
		public class ColInfo
		{
			public int index { get; set; }
			public double X { get; set; }
		}

		static public Dictionary<string, ColInfo> ColumnLocList = new Dictionary<string, ColInfo>();

		public string ShopField { get; set; }
		public string PtNo { get; set; }
		public string Desc { get; set; }
		public string Size { get; set; }
		public double MainSize
		{
			get 
			{
				double dMainSize = 0, dSubSize = 0;
				GetSizeFrom(ref dMainSize, ref dSubSize, Size);
				
				return dMainSize;
			}
		}
		public double SubSize
		{
			get 
			{
				double dMainSize = 0, dSubSize = 0;
				GetSizeFrom(ref dMainSize, ref dSubSize, Size);

				return dSubSize;
			}
		}
		public ObjectId SizeObjId { get; set; }	/// used for Size object id - 2013.12.11 added by humkyung 
		public string Spec { get; set; }
		public ObjectId SpecObjId { get; set; }	/// used for Spec object id - 2013.12.11 added by humkyung 
		public string Symbol { get; set; }
		public string MatlCode { get; set; }
		public ObjectId CodeObjId { get; set; }	/// used for Matl code object id - 2013.10.21 added by humkyung 
		public bool IsPipe
		{
			get { return MatlCode.StartsWith("PP"); }
		}
		public string Qty { get; set; }
		public double Y { get; set; }
		public bool IsSupport { get; set; }		/// flag for support
		public string Str { get; set; }			/// 2014.05.26 added by humkyung

		private static Regex BoltLengthPattern = new Regex("[0-9]+ mm[ ]*Length$");
		private static Regex SizeDelimiterPattern = new Regex("[xX]");
		private static Regex QtyPattern = new Regex("[0-9.]+");

		public IsoBMData(string ShopOrField)
		{
			ShopField = ShopOrField;
		}

		/// <summary>
		/// convert size string to double
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.07.23</date>
		/// <param name="sSize"></param>
		/// <returns></returns>
		private double ConvertSizeFrom(string sSize)
		{
			double res = 0.0;

			sSize = sSize.Trim();
			if (string.IsNullOrEmpty(sSize)) return res;

			try
			{
				int at = sSize.IndexOf('.');
				if (-1 != at)
				{
					string tmp = sSize.Substring(0, at);
					res = Convert.ToDouble(tmp);
					sSize = sSize.Substring(at + 1);
				}

				at = sSize.IndexOf('/');
				if (-1 != at)
				{
					string sNumerator = sSize.Substring(0, at);
					string sDenominator = sSize.Substring(at + 1);
					res += Convert.ToDouble(sNumerator) / Convert.ToDouble(sDenominator);
				}
				else
				{
					res += Convert.ToDouble(sSize);
				}
			}
			catch (Exception ex)
			{
#if DEBUG
				System.Windows.Forms.MessageBox.Show(ex.Message, "Error", System.Windows.Forms.MessageBoxButtons.OK, System.Windows.Forms.MessageBoxIcon.Error);
#endif
			}

			return res;
		}

		/// <summary>
		/// get main and sub size from given size string
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.07.23</date>
		/// <param name="dMainSize"></param>
		/// <param name="dSubSize"></param>
		/// <param name="sSize"></param>
		/// <returns></returns>
		private bool GetSizeFrom(ref double dMainSize, ref double dSubSize, string sSize)
		{
			bool res = false;

			Match SizeMatch = SizeDelimiterPattern.Match(sSize);
			if (SizeMatch.Length > 0)
			{
				string[] oTokens = sSize.Split(SizeMatch.Value.ToCharArray());

				string sMainSize = oTokens[0];
				dMainSize = ConvertSizeFrom(sMainSize);
				string sSubSize = oTokens[1];
				dSubSize = ConvertSizeFrom(sSubSize);
			}
			else
			{
				dMainSize = ConvertSizeFrom(sSize);
				/// #12: get bolt length and then set it as sub size of bolt(http://172.16.9.162:8080/projects/IDCS4SP3D/ticket/12) - 2013.09.23 added by humkyung
				if ((null != MatlCode) && ((MatlCode.Length > 0) && ('B' == MatlCode[0])) && !string.IsNullOrEmpty(Desc))
				{
					Match match = BoltLengthPattern.Match(Desc);
					if ((null != match) && (match.Length > 0))
					{
						string sValue = Desc.Substring(match.Index, Desc.IndexOf("mm", match.Index) - match.Index);
						Double.TryParse(sValue, out dSubSize);
					}
				}
				/// up to here
			}

			return res;
		}

        /// <summary>
        /// find next column name of given column
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2014.07.08</date>
        /// <param name="sColName"></param>
        /// <returns></returns>
        private string FindNextColName(string sColName)
        {
            string res = string.Empty;

            int iStartIndex = IsoBMData.ColumnLocList[sColName].index;
            int iMin = -1;
            foreach (string str in IsoBMData.ColumnLocList.Keys)
            {
                if ((str == sColName) || (IsoBMData.ColumnLocList[str].index < iStartIndex)) continue;
                if (-1 == iMin)
                {
                    res = str;
                    iMin = IsoBMData.ColumnLocList[str].index - iStartIndex;
                }
                else if ((IsoBMData.ColumnLocList[str].index - iStartIndex) < iMin)
                {
                    iMin = IsoBMData.ColumnLocList[str].index - iStartIndex;
                    res = str;
                }
            }

            return res;
        }

		/// <summary>
		/// replace AutoCAD Special Chars
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.09</date>
		/// <param name="str"></param>
		private void ReplaceAutoCADSpecialChars(ref string str)
		{
			Dictionary<string,string> oAutoCADSpecialChars = new Dictionary<string,string>()
			{
				{"%%037","%"} , {"%%177" , "±"} , {"%%215" , "×"} , {"%%247" , "÷"}
			};

			foreach (string sKey in oAutoCADSpecialChars.Keys)
			{
				str = str.Replace(sKey, oAutoCADSpecialChars[sKey]);
			}
		}

		/// <summary>
		/// parse text list
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2013.08.19</date>
		/// <param name="TextList"></param>
		public void Parse(List<IsoDwgParser.DBTextIdPair> TextList)
		{
			Dictionary<string, double> ValueOffset = new Dictionary<string, double>();

			int iLastIndex = -1;
			string sTextString = TextList[0].text.TextString;
			ReplaceAutoCADSpecialChars(ref sTextString);
            foreach (string sColName in IsoBMData.ColumnLocList.Keys)
            {
				/// get value corresponding to column
                string sNextColName = FindNextColName(sColName) , sString = string.Empty;
				if(!string.IsNullOrEmpty(sNextColName))
				{
					int iLength = IsoBMData.ColumnLocList[sNextColName].index - IsoBMData.ColumnLocList[sColName].index;
					sString = sTextString.Substring(IsoBMData.ColumnLocList[sColName].index, iLength);
				}
				else
				{
					int iLength = sTextString.Length - iLastIndex;
					sString = sTextString.Substring(iLastIndex , iLength);
				}
				/// up to here

				if (("NO" == sColName))
				{
					PtNo = sString.Trim();
					Y = TextList[0].text.Position.Y;
				}
				else if (("DESC" == sColName))
				{
					Desc = sString;
				}
				else if (("SIZE" == sColName))
				{
					Size = sString.Trim();
					SizeObjId = TextList[0].text.ObjectId;	/// set object id of DBText - 2013.11.11 added by humkyung
				}
				else if (("SPEC" == sColName))
				{
					Spec = sString.Trim();
					SpecObjId = TextList[0].text.ObjectId;	/// set object id of DBText - 2013.11.11 added by humkyung
				}
				else if (("SYM" == sColName))
				{
					Symbol = sString.Trim();
				}
				else if (("CODE" == sColName))
				{
					sString = sString.Trim();
					int i = 0;
					for (i = 0; i < sString.Length; ++i)
					{
						if (' ' == sString[i]) break;
					}
					sString = sString.Substring(0, i);
					MatlCode = sString;
					CodeObjId = TextList[0].text.ObjectId;	/// set object id of DBText - 2013.10.21 added by humkyung
				}
				else if (("QTY" == sColName))
				{
					Qty = sString.Trim();
					Match res = QtyPattern.Match(Qty);
					Qty = Qty.Substring(0, res.Length);
				}

				iLastIndex = IsoBMData.ColumnLocList[sColName].index + sString.Length;
            }
		}

		/// <summary>
		/// parse column location
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.07.08</date>
		/// <param name="TextList"></param>
		static public void ParseColumnLoc(List<IsoDwgParser.DBTextIdPair> TextList)
		{
            List<string> oIndicators = new List<string>()
            {
                "NO" , "MATERIAL DESCRIPTION" , "(MM)" , "(IN)" , "SPEC" , "SYM" , "CODE" , "QTY"
            };
			int col = 0;
            foreach (string sIndicator in oIndicators)
            {
                int at = TextList[0].text.TextString.IndexOf(sIndicator);
                if(-1 == at) continue;

                ColInfo colInfo = new ColInfo() { index = at , X = TextList[0].text.Position.X };
                if ("NO" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("NO", colInfo);
                }
                else if ("MATERIAL DESCRIPTION" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("DESC", colInfo);
                }
                else if (("(IN)" == sIndicator) || ("(MM)" == sIndicator))
                {
                    IsoBMData.ColumnLocList.Add("SIZE", colInfo);
                }
                else if ("SPEC" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("SPEC", colInfo);
                }
                else if ("SYM" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("SYM", colInfo);
                }
                else if ("CODE" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("CODE", colInfo);
                }
                else if ("QTY" == sIndicator)
                {
                    IsoBMData.ColumnLocList.Add("QTY", colInfo);
                }
                ++col;
            }
		}
	}
}
