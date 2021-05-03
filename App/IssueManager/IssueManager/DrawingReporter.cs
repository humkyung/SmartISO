using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartISO
{
	class ExlStruct
	{
		public string ExlColumn = string.Empty;
		public string ExlDesc = string.Empty;
	}

	class DrawingReporter
	{
		/// <summary>
		/// 
		/// </summary>
		private List<ExlStruct> m_oExlResultEntry = new List<ExlStruct>();
		public List<ExlStruct> ExlResultEntry
		{
			get { return m_oExlResultEntry; }
		}

		/// <summary>
		/// read drawing excel matching from ini file
		/// </summary>
		public void ReadDrawingExcelMatchingEntry(CBusinessLogic oBusinessLogic)
		{
			int ExCount = oBusinessLogic.DrawingExcelMatching.ExCount;

			for (int k = 0; k < ExCount; ++k)
			{
				ExlStruct oExlStruct = new ExlStruct();

				string sValue = AppDocData.frameworkCore.Ini.GetIniValue("Drawing Excel Matching", string.Format("Name{0}", k), 256 , oBusinessLogic.IniFilePath);
				string[] oTokens = sValue.Split(":".ToCharArray());
				if (oTokens.Length > 1)
				{
					oExlStruct.ExlColumn = oTokens[0];
					oExlStruct.ExlDesc = oTokens[1];
					ExlResultEntry.Add(oExlStruct);
				}
			}
		}
	}
}
