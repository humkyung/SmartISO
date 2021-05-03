using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SmartISO
{
	public class RevisionStage
	{
		public long m_lNo = 0;
		public string m_sDefine = string.Empty, m_sText = string.Empty;
	};

	public struct tagRemoteDatabaseSetting
	{
		public string sHost;
		public string sService;
		public string sId;
		public string sPassword;
	};

	public class CBusinessLogic
	{
		private string m_sDatabasePath = string.Empty;
		public string DatabasePath
		{
			get { return m_sDatabasePath; }
			set { m_sDatabasePath = value; }
		}

		/// <summary>
		/// 
		/// </summary>
		private string m_sIniFilePath = string.Empty;
		public string IniFilePath
		{
			get { return m_sIniFilePath; }
			set { m_sIniFilePath = value; }
		}

		/// <summary>
		/// 
		/// </summary>
		private string m_sProjectDesc = string.Empty;
		public string ProjectDesc
		{
			get { return m_sProjectDesc; }
			set { m_sProjectDesc = value; }
		}

		/// <summary>
		/// 
		/// </summary>
		private string m_sJobNo = string.Empty;
		public string JobNo
		{
			get { return m_sJobNo; }
			set { m_sJobNo = value; }
		}

		/// <summary>
		/// 
		/// </summary>
		private string m_sClientName = string.Empty;
		public string ClientName
		{
			get { return m_sClientName; }
			set { m_sClientName = value; }
		}

		/// <summary>
		/// get/set project path
		/// <date>2011.06.13</date>
		/// </summary>
		private string m_sProjectPath = string.Empty;
		public string ProjectPath
		{
			get { return m_sProjectPath; }
			set { m_sProjectPath = value; }
		}

        /// <summary>
        /// get/set issue pate
        /// <date>2012.05.16</date>
        /// <autohr>humkyung</autohr>
        /// </summary>
        private string m_sIssuePath = string.Empty;
        public string IssuePath
        {
            get { return m_sIssuePath; }
            set { m_sIssuePath = value; }
        }

		public class CDrawingExcelMatching
		{
			public string ExlPath = string.Empty;
			public int ExCount = 0;
			public int iExlStartRow = 0;
			public int iNextColumn = 0;
		};
		/// <summary>
		/// 
		/// </summary>
		private CDrawingExcelMatching m_oDrawingExcelMatching = new CDrawingExcelMatching();
		public CDrawingExcelMatching DrawingExcelMatching
		{
			get { return m_oDrawingExcelMatching; }
		}

		//! 2011.02.19 added HumKyung
		public string m_sKey = string.Empty;
		private Dictionary<string, string> m_oProjectSetting = new Dictionary<string, string>();
		public Dictionary<string, string> ProjectSetting
		{
			get { return m_oProjectSetting; }
			set { m_oProjectSetting = value; }
		}

		public int ITEM_COUNT_PER_SHEET = 60;
		public int ITEM_HALF_COUNT_PER_SHEET = 30;

		//!
		public List<RevisionStage> m_RevisionStageList = new List<RevisionStage>();
		
		private string _sRemoteDatabaseAppName = "Remote Database";
		private string _sFtpServerAppName = "Ftp Server";
		public tagRemoteDatabaseSetting RemoteDatabaseSetting;
		/// <summary>
		/// read setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		public void ReadSetting()
		{
			ProjectDesc = AppDocData.frameworkCore.Ini.GetIniValue("General", "Project No", 256 , IniFilePath);

			JobNo = AppDocData.frameworkCore.Ini.GetIniValue("General", "Comment", 256 , IniFilePath);

			ClientName = AppDocData.frameworkCore.Ini.GetIniValue("General", "Client Name", 256 , IniFilePath);

			string sApp = "Drawing Excel Matching";
			DrawingExcelMatching.ExlPath = AppDocData.frameworkCore.Ini.GetIniValue(sApp, "ExlPath", 256 , IniFilePath);

			string sValue = AppDocData.frameworkCore.Ini.GetIniValue(sApp, "Count", 256 , IniFilePath);
			try
			{
				DrawingExcelMatching.ExCount = Convert.ToInt32(sValue);
			}
			catch (FormatException)
			{
				DrawingExcelMatching.ExCount = 0;
			}

			sValue = AppDocData.frameworkCore.Ini.GetIniValue(sApp, "Row", 256 , IniFilePath);
			try
			{
				DrawingExcelMatching.iExlStartRow = Convert.ToInt32(sValue);
			}
			catch (FormatException)
			{
				DrawingExcelMatching.iExlStartRow = 0;
			}

			/// 2011.08.02 added by humkyung
			sValue = AppDocData.frameworkCore.Ini.GetIniValue(sApp, "Column", 256 , IniFilePath);
			try
			{
				DrawingExcelMatching.iNextColumn = Convert.ToInt32(sValue);

			}
			catch (FormatException)
			{
				DrawingExcelMatching.iNextColumn = 0;
			}

			RemoteDatabaseSetting.sHost = AppDocData.frameworkCore.Ini.GetIniValue(_sRemoteDatabaseAppName, "Host", 256 , IniFilePath);
			RemoteDatabaseSetting.sService = AppDocData.frameworkCore.Ini.GetIniValue(_sRemoteDatabaseAppName, "Service", 256 , IniFilePath);
			RemoteDatabaseSetting.sId = AppDocData.frameworkCore.Ini.GetIniValue(_sRemoteDatabaseAppName, "Id", 256 , IniFilePath);
			RemoteDatabaseSetting.sPassword = AppDocData.frameworkCore.Ini.GetIniValue(_sRemoteDatabaseAppName, "Password", 256 , IniFilePath);

			AppDocData.FtpServerSetting.sHost = AppDocData.frameworkCore.Ini.GetIniValue(_sFtpServerAppName, "Host", 256, IniFilePath);
			AppDocData.FtpServerSetting.sId = AppDocData.frameworkCore.Ini.GetIniValue(_sFtpServerAppName, "Id", 256, IniFilePath);
			AppDocData.FtpServerSetting.sPassword = AppDocData.frameworkCore.Ini.GetIniValue(_sFtpServerAppName, "Password", 256, IniFilePath);
			AppDocData.FtpServerSetting.sRemoteDirectory= AppDocData.frameworkCore.Ini.GetIniValue(_sFtpServerAppName, "Remote Directory", 256, IniFilePath);
		}

		/// <summary>
		/// save setting
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		public void SaveSetting()
		{
			AppDocData.frameworkCore.Ini.SetIniValue(_sRemoteDatabaseAppName, "Host", RemoteDatabaseSetting.sHost, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sRemoteDatabaseAppName, "Service", RemoteDatabaseSetting.sService, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sRemoteDatabaseAppName, "Id", RemoteDatabaseSetting.sId, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sRemoteDatabaseAppName, "Password", RemoteDatabaseSetting.sPassword, IniFilePath);

			AppDocData.frameworkCore.Ini.SetIniValue(_sFtpServerAppName, "Host", AppDocData.FtpServerSetting.sHost, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sFtpServerAppName, "Id", AppDocData.FtpServerSetting.sId, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sFtpServerAppName, "Password", AppDocData.FtpServerSetting.sPassword, IniFilePath);
			AppDocData.frameworkCore.Ini.SetIniValue(_sFtpServerAppName, "Remote Directory", AppDocData.FtpServerSetting.sRemoteDirectory, IniFilePath);
		}

		/// <summary>
		/// get year,month,day from issue date
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.08.02</date>
		/// <param name="iYear"></param>
		/// <param name="iMonth"></param>
		/// <param name="iDay"></param>
		/// <param name="sIssueDate"></param>
		/// <returns></returns>
		public bool ParseIssueDate(ref Int32 iYear, ref Int32 iMonth, ref Int32 iDay, string sIssueDate)
		{
			string sIssueDateFormatNo = ProjectSetting["C_ISSUE_DATE_FORMAT_NO"];
			string  sIssueDateDelimiter= ProjectSetting["C_ISSUE_DATE_DELIMITER"];
			
			string[] oResult = sIssueDate.Split(sIssueDateDelimiter.ToCharArray());
			if(3 == oResult.Length)
			{
				if(("1") == sIssueDateFormatNo)
				{
					iYear = Convert.ToInt32(oResult[0]);
					iMonth = Convert.ToInt32(oResult[1]);
					iDay = Convert.ToInt32(oResult[2]);

					return true;
				}
				else if(("2") == sIssueDateFormatNo)
				{
					iMonth = Convert.ToInt32(oResult[0]);
					iDay = Convert.ToInt32(oResult[1]);
					iYear = Convert.ToInt32(oResult[2]);

					return true;
				}
				else
				{
					iDay  = Convert.ToInt32(oResult[0]);
					iYear = Convert.ToInt32(oResult[2]);

					string[] oMonthAbbrList = 
					{ 
						("JAN") , ("FEB") , ("MAR") , ("APR") , ("MAY") , 
						("JUN") , ("JUL") , ("AUG") , ("SEP") , ("OCT") , 
						("NOV") , ("DEC")
					};
					iMonth = 1;
					foreach(string sMonth in oMonthAbbrList)
					{
						if(sMonth == oResult[1])
						{
							return true;
						}
						iMonth += 1;
					}

					return false;
				}
			}

			return false;
		}

		/// <summary>
		/// get display issue date from 
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2011.08.06</date>
		/// <param name="sIssueDate"></param>
		/// <returns></returns>
		public string GetDisplayIssueDateFrom(string sIssueDate)
		{
			string res = string.Empty;

			string[] oToken = sIssueDate.Split("-".ToCharArray());
			if(3 == oToken.Length)
			{
				int iYear  = Convert.ToInt32(oToken[0]);
				int iMonth = Convert.ToInt32(oToken[1]);
				int iDay   = Convert.ToInt32(oToken[2]);

				string sIssueDateFormatNo = ProjectSetting["C_ISSUE_DATE_FORMAT_NO"];
				string  sIssueDateDelimiter= ProjectSetting["C_ISSUE_DATE_DELIMITER"];
			
				if(("1") == sIssueDateFormatNo)
				{
					res = string.Format(("{0:0000}{1}{2:00}{3}{4:00}") , iYear , sIssueDateDelimiter , iMonth , sIssueDateDelimiter , iDay);
				}
				else if(("2") == sIssueDateFormatNo)
				{
					res = string.Format(("{0:00}{1}{2:00}{3}{4:0000}") , iMonth , sIssueDateDelimiter , iDay , sIssueDateDelimiter , iYear);
				}
				else
				{
					string[] sMonthAbbrList = 
					{ 
						("JAN") , ("FEB") , ("MAR") , ("APR") , ("MAY") , 
						("JUN") , ("JUL") , ("AUG") , ("SEP") , ("OCT") , 
						("NOV") , ("DEC")
					};
					
					res = string.Format(("{0:00}{1}{2}{3}{4:0000}") , iDay , sIssueDateDelimiter , sMonthAbbrList[iMonth - 1] , sIssueDateDelimiter , iYear);
				}
			}

			return res;
		}

		/*
		 * @brief	주어진 시간이 현재 시간보다 이전인지 검사한다.
		 * 
		 * @author	HumKyung
		 * 
		 * @date	2011.02.18
		 * 
		 * @param	sDate
		 * 
		 * @return	bool
		 */
		public bool IsBeforeCurrentDate(string sDate)
		{
			string sDateTime = string.Empty;
			DateTime today = DateTime.Today;
			string year = today.Year.ToString();
			string month = today.Month.ToString();
			string day = today.Day.ToString();
			double dCurrentDate = Convert.ToDouble(year) * 360 + Convert.ToDouble(month) * 30 + Convert.ToDouble(day);

			Int32 dYear = 0, dMonth = 0, dDay = 0;
			bool res = ParseIssueDate(ref dYear, ref dMonth, ref dDay, sDate);
			double dDate = 0;
			if (true == res)
			{
				dDate = dYear * 360 + dMonth * 30 + dDay;
			}

			return (dDate < dCurrentDate);
		}

		/*
		 * @brief	주어진 날짜가 최종 REV. 날짜보다 이전인지 검사한다.
		 * 
		 * @author	HumKyung
		 * 
		 * @date	2011.02.18
		 * 
		 * @param	sDate
		 * 
		 * @return	bool
		 */
		public bool IsBeforeLastRevDate(string sLastRevDate , string sDate)
		{
			string sDelimiter = ProjectSetting["C_ISSUE_DATE_DELIMITER"];

			string[] oToken = sLastRevDate.Split("-".ToCharArray());
			double dLastRevDate = 0;
			if(3 == oToken.Length)
			{
				dLastRevDate = Convert.ToInt32(oToken[0])*360 + Convert.ToInt32(oToken[1])* 30 + Convert.ToInt32(oToken[2]);
			}

			Int32 dYear = 0, dMonth = 0, dDay = 0;
			double dDate = 0;
			bool res = ParseIssueDate(ref dYear, ref dMonth, ref dDay, sDate);
			if (true == res)
			{
				dDate = dYear * 360 + dMonth * 30 + dDay;
			}

			return (dDate < dLastRevDate);
		}

		/*
		 * @brief	selected grade를 검사한다. 현재의 grade보다 크거나 같아야 한다.
		 * 
		 * @author	HumKyung
		 * 
		 * @date	2011.02.18
		 * 
		 * @param	sDate
		 * 
		 * @return	bool
		 */
		public bool CheckSelectedGrade(Int32 iLastRevNo, Int32 iSelGrade)
		{
			Int32 iCurGrade = iLastRevNo / 100;

			return (iSelGrade >= iCurGrade);
		}

		/*
		 * @brief	사용할 revision stage를 구한다.
		 * 
		 * @author	HumKyung
		 * 
		 * @date	2011.02.19
		 * 
		 * @param	sDate
		 * 
		 * @return	bool
		 */
		public bool GetNextRevisionStage(ref RevisionStage stage, int iRevNo, int iSelGrade)
		{
			foreach (RevisionStage rev in m_RevisionStageList)
			{
				long iGrade = Convert.ToInt32(rev.m_sDefine);
				if ((iGrade >= iSelGrade) && (rev.m_lNo > iRevNo))
				{
					stage = rev;
					return true;
				}
			}
				
			return false;
		}

        private string[] m_oOutputIsoFileNameFormat = null;
        /// <summary>
        /// return output iso file format
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.05.18</date>
        /// <returns>string[]</returns>
        public string[] GetOutputIsoFileNameFormat()
        {
            if (null == m_oOutputIsoFileNameFormat)
            {
				string sOutputFileFormat = AppDocData.frameworkCore.Ini.GetIniValue("Iso Edit Setting", "Output File Name Format", 256 , IniFilePath);
                m_oOutputIsoFileNameFormat = sOutputFileFormat.Split(":".ToCharArray());
            }

            return m_oOutputIsoFileNameFormat;
        }

        private string[] m_oOutputIsoFileExtFormat = null;
        /// <summary>
        /// return output iso file format
        /// </summary>
        /// <author>humkyung</author>
        /// <date>2012.05.18</date>
        /// <returns>string[]</returns>
        public string[] GetOutputIsoFileExtFormat()
        {
            if (null == m_oOutputIsoFileExtFormat)
            {
				string sOutputFileFormat = AppDocData.frameworkCore.Ini.GetIniValue("Iso Edit Setting", "Output File Ext Format", 256, IniFilePath);
                m_oOutputIsoFileExtFormat = sOutputFileFormat.Split(":".ToCharArray());
            }

            return m_oOutputIsoFileExtFormat;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sFieldName"></param>
        /// <returns></returns>
        public int FieldName2Int(string sFieldName)
        {
            int ret = 0;

            for (int i = sFieldName.Length - 1; i >= 0; i--)
            {
                ret += (sFieldName[i] - 'A') + i * 26;
            }

            return ret;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sFirstCol"></param>
        /// <param name="nCol"></param>
        /// <returns></returns>
        public int SetNextColumn(int nFirstCol, int nCol)
        {
            int nNextCol = nFirstCol + nCol;

            return nNextCol;
        }
	}
}
