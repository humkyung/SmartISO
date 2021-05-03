using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IsoDwg.NET
{
	public class NtrData
	{
		static private int[] FieldPos = new int[13];
		static public int NtrTextLength = 70;

		public string Item { get; set; }
		public string Spec { get; set; }
		public string MatlCode { get; set; }
		public string FieldShop { get; set; }
		public string MainSize { get; set; }
		public string SubSize { get; set; }
		public string BoltSize { get; set; }
		public string BoltLength { get; set; }
		public string Qty { get; set; }
		public string Length { get; set; }
		public string OptionCode { get; set; }

		/// <summary>
		/// parse given text
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2014.05.28</date>
		/// <param name="oTextPair"></param>
		public bool Parse(IsoDwgParser.DBTextIdPair oTextPair)
		{
			ParseColumnIndex();
			if (!string.IsNullOrEmpty(oTextPair.text.TextString) && oTextPair.text.TextString.Length > NtrTextLength)
			{
				string sTextString = oTextPair.text.TextString;
				Item = sTextString.Substring(FieldPos[0], FieldPos[1] - 1).Trim();
				if (7 != Item.Length) return false;

				Spec = sTextString.Substring(FieldPos[2], FieldPos[3] - FieldPos[2]).Trim();
				MatlCode = sTextString.Substring(FieldPos[4], FieldPos[5] - FieldPos[4]).Trim();
				FieldShop = sTextString.Substring(FieldPos[5], FieldPos[6] - FieldPos[5]).Trim();
				MainSize = sTextString.Substring(FieldPos[6], FieldPos[7] - FieldPos[6]).Trim();
				SubSize = sTextString.Substring(FieldPos[7], FieldPos[8] - FieldPos[7]).Trim();
				BoltSize = sTextString.Substring(FieldPos[8], FieldPos[9] - FieldPos[8]).Trim();
				BoltLength = sTextString.Substring(FieldPos[9], FieldPos[10] - FieldPos[9]).Trim();
				Qty = sTextString.Substring(FieldPos[10], FieldPos[11] - FieldPos[10]).Trim();
				Length = sTextString.Substring(FieldPos[11], FieldPos[12] - FieldPos[11]).Trim();
				OptionCode = sTextString.Substring(FieldPos[12], sTextString.Length - FieldPos[12]).Trim();
				
				return true;
			}
			
			return false;
		}

		private void ParseColumnIndex()
		{
			/*
			rec   mat     1st     2nd     qty             commodity                                   [ wbs ]                                          insul   option
			type  class   size    size    (pipe,bolt)     code (bmcs code)      line id               p    a    u    s    sht seq no            f/s fluid   type    code
			----  ------  ------  ------  --------------  --------------------  --------------------  ---  ---  ---  ---  --  ----------------  --  ------  ------  ------
			*/

			FieldPos[0] = 0;  // item
			FieldPos[1] = 19;
			FieldPos[2] = 25; // spec 
			FieldPos[3] = 35; // insul type
			FieldPos[4] = 45; // matl code
			FieldPos[5] = 65; //s/f
			FieldPos[6] = 68; // from size
			FieldPos[7] = 76; // sub_size
			FieldPos[8] = 84; //bolt_size
			FieldPos[9] = 92; //bolt_length
			FieldPos[10] = 100; //qty 
			FieldPos[11] = 110; //length
			FieldPos[12] = 130; //option code
		}
	}
}
