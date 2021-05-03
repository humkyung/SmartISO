using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace IsoDwg.NET
{
	public class CutPipeLengthData
	{
		public string PieceNo { get; set; }
		public string Length { get; set; }
		public string Size { get; set; }

		public CutPipeLengthData(){}

		/// <summary>
		/// parse given text list
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.06.03</date>
		/// <param name="TextList"></param>
		/// <returns></returns>
        private bool Parse(string str)
        {
            if (!string.IsNullOrEmpty(str))
            {
                Regex Pattern = new Regex("[0-9]+");

                Match match = Pattern.Match(str);
                PieceNo = match.Value;

                match = match.NextMatch();
                Length = match.Value;

                match = match.NextMatch();
                Size = match.Value;

                return true;
            }

            return false;
        }
        static private string GetPipeCutLengthDataText(ref string str)
        {
            string res = string.Empty;

            Regex CutPipeLengthDataPattern = new Regex("<[0-9]+>");
            Match match = CutPipeLengthDataPattern.Match(str);
            if ((match.Index >= 0) && (match.Length > 0))
            {
                Match next = match.NextMatch();
                res = (0 == next.Length) ?  str : str.Substring(match.Index , next.Index - match.Index);
                str = (0 == next.Length) ? string.Empty : str.Substring(next.Index);
            }

            return res;
        }

		/// <summary>
		/// parse given text list and return list of cut pipe length data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.06.03</date>
		/// <param name="TextList"></param>
		/// <returns></returns>
		static public List<CutPipeLengthData> Generate(List<IsoDwgParser.DBTextIdPair> TextList)
		{
            Regex CutPipeLengthDataPattern = new Regex("<[0-9]+>");

			List<CutPipeLengthData> res = new List<CutPipeLengthData>();

			List<IsoDwgParser.DBTextIdPair> TempTextList = new List<IsoDwgParser.DBTextIdPair>();
			foreach (IsoDwgParser.DBTextIdPair pair in TextList)
			{
                string str = pair.text.TextString;
                while (!string.IsNullOrEmpty(str))
                {
                    string sPipeCutLengthDataText = GetPipeCutLengthDataText(ref str);
                    if (string.IsNullOrEmpty(sPipeCutLengthDataText)) break;
                    {
                        CutPipeLengthData data = new CutPipeLengthData();
                        data.Parse(sPipeCutLengthDataText);
                        res.Add(data);
                    }
                }
			}

			return res;
		}
	}
}
