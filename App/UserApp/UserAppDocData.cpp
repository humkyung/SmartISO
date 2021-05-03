// UserAppDocData.cpp: implementation of the CAppDocData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <util/FileTools.h>
#include "UserApp.h"
#include <Util/FileVersion.h>
#include "tokenizer.h"
#include "UserAppDocData.h"
#include "BMReportSettingPage.h"
#include "IsoEditSettingPage.h"
#include "ProjectData.h"
#include "StringTable.h"

#include "QCCheck.h"	/// 2011.08.28 added by humkyung

#include "Splash/SplashScreenFx.h"

/*
#include "Socket/CommandObject.h"
#include "Socket/ClientSocket.h"
#include "soapLicWebServiceSoapProxy.h"
extern "C" __declspec(dllimport) int __stdcall Encode(LMS_ENCODE* param);
extern "C" __declspec(dllimport) int __stdcall Decode(LMS_ENCODE* param);
*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/******************************************************************************
	@brief
    @author     humkyung
    @date       2011-08-31
    @return     int
******************************************************************************/
int CBMOutputForamt::Read()
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	map<STRING_T,STRING_T>* pDrawingSetup = pProjectData->GetDrawingSetup();
	map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("BM Field Format"));
	m_oFieldFormatList.clear();
	if(pDrawingSetup->end() != where)
	{
		CTokenizer<CIsComma>::Tokenize(m_oFieldFormatList,where->second, CIsComma());
	}

	return ERROR_SUCCESS;
}

TCHAR* CAppDocData::m_pCheckMessages[]=
{
	_T("(1)  CAN NOT FIND ISO DWG NAME "),
	_T("(2)  ERROR READ                "),
	_T("(3)  PIPE CUTTING LENGTH       "),
	_T("(4)  NOT FOUND ERROR           "),
	_T("(5)  FAIL ISO ERROR            "),
	_T("(6)  PCD SYSMBOL ERROR         "),
	_T("(7)  HOLD ITEM ERROR           "),
	_T("(8)  SYMBOL CODE NOT FOUND     "),
	_T("(9)  SUPPORT B/M ERROR         "),
	_T("(10) TEXT MODIFIED ERROR       "),
	_T("(11) F-G-B ERROR               "),
	_T("(12) SPEC BREAK ERROR          "),
	_T("(13) B/M NOT FOUNED            "),
	_T("(14) B/M CONSISTENCY ERROR     "),
	_T("(15) B/M OVER                  ")
};

int CAppDocData::m_nIsoEditErrorCount[CHECK_ERROR_COUNT];
vector<CString> CAppDocData::ERROR_STRING_ENTRY;	///< PROCESS중 발생한 에러 내용을 담는다.
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAppDocData::CAppDocData() : m_pIssueDateRecordSet(NULL)
{
	m_borderSetting.TextColorNo = 0;
	m_borderSetting.FontNo = -1;
	m_borderSetting.widthMulti  = -1;
	m_borderSetting.heightMulti = -1;
	m_borderSetting.iTextWeight = 1;
	m_pProject = NULL;

	m_headerImgList.Create(IDB_LIST_HEADER , 8 , 16 , RGB(255 , 0 , 255));

	//////////////////////////////////////////////////////////////////////////
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	m_rExecPath = szPathName;
	const int it = m_rExecPath.ReverseFind('\\');
	if(it)
	{
		m_rExecPath = m_rExecPath.Left(it);
	}
	else
	{
		m_rExecPath.Empty();
	}
	m_RemoveAreaEntry.clear();

	m_rUnit = _T("INCH");	/// 기본값

	////////////////////////////////////////////////////////////////////////////////////////////////////
	m_sWorkingUnit = _T("0");

	m_sDVCS[0] = _T("0");
	m_sDVCS[1] = _T("0");
	m_sDVCS[2] = _T("0");

	m_pRevisionStageMap  = new map<int,RevisionStage>;
	m_pGradeDefineEntry  = new vector<GradeDefine>;

	/// 2011.11.04 added by humkyung
	m_oTextModifiedErrorException.iLevel = -1;
	m_oTextModifiedErrorException.iColor = -1;
	m_oTextModifiedErrorException.iStyle = -1;
	m_oTextModifiedErrorException.iWeight= -1;
	/// up to here
	
	m_rSiteName = _T("ALL");	/// 2012.08.13 added by humkyung
}

/**	
	@brief
	@author	humkyung
*/
CAppDocData::~CAppDocData()
{
	try
	{
		for(map<CString , CPCDFile*>::const_iterator itr = m_pcdFileMap.begin();itr != m_pcdFileMap.end();++itr)
		{
			CPCDFile::DeleteInstance((*itr).second);
		}
		m_pcdFileMap.clear();

		if(NULL != m_pRevisionStageMap) delete m_pRevisionStageMap;
		m_pRevisionStageMap = NULL;

		if(NULL != m_pGradeDefineEntry) delete m_pGradeDefineEntry;
		m_pGradeDefineEntry = NULL;

		if(NULL != m_pProject) CProject::DeleteInstance(m_pProject);

		/// logout - 2016.10.06 added by humkyung
		/*
		Packet packet;
		InitializePacket(&packet);
		strcpy((char*)(packet.ID) , m_sLicenseId.c_str());
		strcpy((char*)(packet.Password) , m_sLicensePassword.c_str());
		strcpy((char*)(packet.ActivateCode),m_sToken.c_str());

		LMS_ENCODE lms;
		lms.size = sizeof(Packet);
		memcpy(lms.buf,&packet,sizeof(Packet));
		Encode(&lms);

		_ns1__Logout request;
		{
			request.oLMS = new ns1__LMSStuctType();
			request.oLMS->buf = new xsd__base64Binary();
			request.oLMS->buf->__ptr = new unsigned char[LMS_BUF_SIZE];
			memcpy(request.oLMS->buf->__ptr,lms.buf,lms.size);
			request.oLMS->buf->__size = LMS_BUF_SIZE;
			request.oLMS->size = lms.size;
		}
		_ns1__LogoutResponse response;

		LicWebServiceSoapProxy soapProxy;
		if(SOAP_OK == soapProxy.Logout(LICENDPOINT,LICENDPOINT"/Logout",&request,response))
		{
		}
		delete request.oLMS->buf->__ptr;
		delete request.oLMS->buf;
		delete request.oLMS;
		/// up to here
		*/
	}
	catch(...)
	{
	}
}

/**
	@brief	SINGLETON INSTANCE POINTER를 구한다.
	@author	HumKyung.BAEK
*/
CAppDocData* CAppDocData::GetInstance()
{
	static CAppDocData __instance;

	return &(__instance);
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CAppDocData
    @function   ReadIsoEditSetting
    @return     int
    @param      const           CString&
    @param      sIniFilePath
    @brief		read iso edit setting from ini file
******************************************************************************/
int CAppDocData::ReadIsoEditSetting( const CString& sIniFilePath )
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};

	/// Iso Edit Setting
	const CString sApp(_T("Iso Edit Setting"));
	m_rUnit = _T("INCH");
	if(GetPrivateProfileString(sApp , _T("UNIT") , NULL , szBuf , MAX_PATH , sIniFilePath))
	{
		m_rUnit = szBuf;
	}
	/// read header information - 2011.08.28 added by humkyung
	CProjectData* pProjectData = CProjectData::GetInstance();
	map<STRING_T,STRING_T>* pDrawingSetup = pProjectData->GetDrawingSetup();

	CIsoBMData::m_oBMAttrPosMap.clear();
	CString HeaderTextArray[] = {_T("NO") , _T("MATL DESC") , _T("SIZE") , _T("MATL SPEC") , _T("SYM") , _T("MATL CODE") , _T("BOLT LEN") , _T("QTY")};
	for(int i = 0;i < SIZE_OF_ARRAY(HeaderTextArray);++i)
	{
		if(_T("BOLT LEN") == HeaderTextArray[i])
		{
			if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("BOLT_LENGTH_LOCATION")))
			{
				CString rString = (*pProjectData->m_pProjectSettingMap)[_T("BOLT_LENGTH_LOCATION")];
				if(_T("2") == rString) continue;
			}
		}

		map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find((HeaderTextArray[i] + _T(" COL DEF")).operator LPCTSTR());
		if(where != pDrawingSetup->end())
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult ,where->second.c_str(), CIsComma());
			if(2 == oResult.size())
			{
				const int iStart = ATOI_T(oResult[0].c_str());
				const int iWidth = ATOI_T(oResult[1].c_str());
				CIsoBMData::m_oBMAttrPosMap.insert(make_pair(HeaderTextArray[i] , make_pair(iStart , iWidth)));
			}
		}
	}
	/// up to here

	/// 2011.11.04 added by humkyung
	if(GetPrivateProfileString(sApp , _T("Text Modified Error(Exception)") , NULL , szBuf , MAX_PATH , sIniFilePath))
	{
		vector<STRING_T> oResult;
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			vector<STRING_T> oTokens;
			CTokenizer<CIsFromString>::Tokenize(oTokens , (*itr) , CIsFromString(_T(":")));
			if(2 == oTokens.size())
			{
				if(_T("LEVEL") == oTokens[0])
				{
					m_oTextModifiedErrorException.iLevel = ATOI_T(oTokens[1].c_str());
				}
				else if(_T("COLOR") == oTokens[0])
				{
					m_oTextModifiedErrorException.iColor = ATOI_T(oTokens[1].c_str());
				}
				else if(_T("STYLE") == oTokens[0])
				{
					m_oTextModifiedErrorException.iStyle = ATOI_T(oTokens[1].c_str());
				}
				else if(_T("WEIGHT") == oTokens[0])
				{
					m_oTextModifiedErrorException.iWeight = ATOI_T(oTokens[1].c_str());
				}
			}
		}
	}
	/// up to here

	return ERROR_SUCCESS;
}

/**
	@brief	convert given column name consists of alphabet to number
	@author	humkyung
	@date	2015.10.28
	@return	long
*/
long CAppDocData::FieldName2Long(const CString &rFieldName)
{          
	long ret = 0L;  
	for(int i = rFieldName.GetLength() - 1;i >= 0 ;i--)   
	{         
		ret += (rFieldName[i] - 'A' + 1) + i * 26;   
	}         
	return ret;        
}

/**	
	@brief	프로그램에 필요한 세팅을 읽는다.
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CAppDocData::InitContents()
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	m_ExCount = 0;
	CString	rApp( _T("Iso Edit Setting") );

	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	
	map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("Standard Note File"));
	if(where != pDrawingSetup->end())
	{
		if(CFileTools::DoesFileExist(where->second.c_str()))
		{
			m_rStdNoteFilePath = where->second.c_str();
		}
		else if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText( _T("Can't find standard note file. - click to continue") );
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
	}
	else if(NULL != pSplash)
	{
		pSplash->SetTextColor(RGB(255,0,0));
		pSplash->SetText( _T("Please, set standard note file. - click to continue") );
		MessageBeep(MB_ICONHAND);
		pSplash->WaitUntilUserConfirm();
	}
	
	if(GetPrivateProfileString(rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rIsoDwgFileFolder = szBuf;
	}

	where = pDrawingSetup->find(_T("Spec File Folder"));
	if(where != pDrawingSetup->end())
	{
		m_rSpecFileFolder = where->second.c_str();
	}

	where = pDrawingSetup->find(_T("Drawing Setup"));
	if(where != pDrawingSetup->end())
	{
		m_rDrawingType = where->second.c_str();
	}else	m_rDrawingType = _T("665");

	this->ReadSettingFromDatabase();
	InitBorderSettings(m_rIniFilePath);
	//////////////////////////////////////////////////////////////////////////
	if(NULL != m_pProject) m_pIssueDateRecordSet = m_pProject->LoadIssueDateRecordSet();
	
	rApp = _T("Line Data Field");
	if(GetPrivateProfileString(rApp , _T("Primary Key") , _T("") , szBuf , MAX_PATH , m_rIniFilePath))
	{
		CTokenizer<CIsFromString>::Tokenize(m_primaryKeySetting , szBuf , CIsFromString(":"));
	}

	///
	CString rString , rKey;
	LoadBorderSetting(m_rIniFilePath);

	rApp = _T("Oracle Setting");
	{
		const int nCount = GetPrivateProfileInt(rApp , _T("Count") , 0 , m_rIniFilePath);
		if(GetPrivateProfileString(rApp, _T("SmartISOKey") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(m_oOracleSetting.m_IsoManKeySetting , szBuf , CIsFromString(":"));		
		}
		
		if(GetPrivateProfileString(rApp, _T("OraKey") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_OracleKey = szBuf;
		}

		/// read oracle setting - 2012.08.29 added by humkyung
		if(GetPrivateProfileString(rApp, _T("Host") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sHost = szBuf;
		}
		if(GetPrivateProfileString(rApp, _T("Service Name") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sService = szBuf;
		}
		if(GetPrivateProfileString(rApp, _T("User Name") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sPrjUser = szBuf;
		}
		if(GetPrivateProfileString(rApp, _T("Password") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sPrjPassword = szBuf;
		}
		if(GetPrivateProfileString(rApp, _T("Design User Name") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sDesUser = szBuf;
		}
		if(GetPrivateProfileString(rApp, _T("Design Password") , NULL, szBuf, MAX_PATH, m_rIniFilePath))
		{
			m_oOracleSetting.m_sDesPassword = szBuf;
		}
		/// up to here
	}
	
	CBMReportSettingPage::LoadData( m_rIniFilePath );
	CIsoEditSettingPage::LoadData( m_rIniFilePath );

	/// Iso Edit Setting
	ReadIsoEditSetting( m_rIniFilePath );

	/// 2011.08.31 added by humkyung
	m_oBMOutputFormat.Read();
	/// up to here

	rApp = _T("Drawing Excel Matching");
	m_Drawing_Excel_Matching.ExCount = GetPrivateProfileInt(rApp,_T("Count"),0,m_rIniFilePath);
	m_Drawing_Excel_Matching.nExlStartRow = GetPrivateProfileInt(rApp,_T("Row"),1,m_rIniFilePath);

	if(GetPrivateProfileString(rApp, _T("Column") , _T("") ,szBuf, MAX_PATH,m_rIniFilePath))
	{
		m_Drawing_Excel_Matching.nNextColumn = CAppDocData::FieldName2Long(szBuf)-1;
	}else	m_Drawing_Excel_Matching.nNextColumn = 0;
	/// up to here

	rApp = _T("General");
	if(GetPrivateProfileString(rApp , _T("Path") , _T("") , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rProjectPath = szBuf;
	}else	m_rProjectPath = _T("");

	//! 2011.06.22 - added by humkyung
	rApp = _T("InstOrSpecialItem");
	if(GetPrivateProfileString(rApp , _T("FullText") , _T("") , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_oDefineInstOrSpecialItem.insert(make_pair(CString(_T("FullText")) , CString(szBuf)));
	}
	if(GetPrivateProfileString(rApp , _T("MatlCode") , _T("") , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_oDefineInstOrSpecialItem.insert(make_pair(CString(_T("MatlCode")) , CString(szBuf)));
	}
	//!

	rApp = _T("Revision Automation");
	if(GetPrivateProfileString(rApp , _T("Rev Iso Folder") , _T("") , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_sIsoFolder4RevAutomation = szBuf;
	}
}

/******************************************************************************
	@brief		read setting from database
    @author     humkyung
    @date       2012-08-14
    @function   ReadSettingFromDatabase
    @return     int
******************************************************************************/
int CAppDocData::ReadSettingFromDatabase()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	
	const CProject::DatabaseProp prop = GetProject()->GetDatabaseProp();
	try
	{
		{
			CAppPostgreSQL database(this->GetProject()->name() , prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			CString rOpenString = _T("SELECT * FROM T_REVISION_STAGE");
			{
				soci::rowset<soci::row> rs(database.session()->prepare << (rOpenString.operator LPCTSTR()));
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					STRING_T rString;

					try
					{
						RevisionStage _rs;
						_rs.rDefine = itr->get<STRING_T>(_T("from_grade_define"));
						_rs.nNo = itr->get<int>(_T("revision_no"));
						_rs.rText = itr->get<STRING_T>(_T("revision_text"));
						m_pRevisionStageMap->insert(make_pair(_rs.nNo , _rs));
					}
					catch(...)
					{
					}
				}
			}
			
			{
				rOpenString = _T("SELECT * FROM T_GRADE_DEFINE");
				soci::rowset<soci::row> rs(database.session()->prepare << (rOpenString.operator LPCTSTR()));
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					STRING_T rString;
					try
					{
						GradeDefine gd;
						rString = itr->get<STRING_T>(_T("grade_prefix"));
						gd.rPrefix = rString.c_str();
						rString = itr->get<STRING_T>(_T("grade_desc"));
						(gd.rDesc) = rString.c_str();
						gd.ShowOnRevTable = itr->get<int>(_T("show_on_revision_table"));
						m_pGradeDefineEntry->push_back(gd);
					}
					catch(...){}
				}
			}
		}

		{
			CProjectData* pPrjData = CProjectData::GetInstance();
			pPrjData->Load(GetProject()->name(),prop);
			map<CString,CString>::const_iterator where = pPrjData->m_pProjectSettingMap->find(_T("working_unit"));
			if(where != pPrjData->m_pProjectSettingMap->end())
			{
				m_sWorkingUnit = where->second.operator LPCSTR();
			}else	m_sWorkingUnit = _T("80");

			where = pPrjData->m_pProjectSettingMap->find(_T("dvcs_e"));
			if(where != pPrjData->m_pProjectSettingMap->end())
			{
				m_sDVCS[0] = where->second.operator LPCSTR();
			}else	m_sDVCS[0] = _T("0");
			where = pPrjData->m_pProjectSettingMap->find(_T("dvcs_n"));
			if(where != pPrjData->m_pProjectSettingMap->end())
			{
				m_sDVCS[1] = where->second.operator LPCSTR();
			}else	m_sDVCS[1] = _T("0");
			where = pPrjData->m_pProjectSettingMap->find(_T("dvcs_el"));
			if(where != pPrjData->m_pProjectSettingMap->end())
			{
				m_sDVCS[2] = where->second.operator LPCSTR();
			}else	m_sDVCS[2] = _T("0");
		}

		return ERROR_SUCCESS;
	}
	catch(std::exception& ex)
	{
	}

	return ERROR_BAD_ENVIRONMENT;
}

bool CAppDocData::IsFixedLineDataFieldName(CString rFieldName)
{
	return ((rFieldName == C_PLANT_NO) || (rFieldName == C_AREA_NO) || 
		(rFieldName == C_UNIT_NO)  || (rFieldName == C_SHEET_NO));
}

/**	
	@brief	ISSUE 날짜는 현재 날짜보다 이후의 날짜인지 확인한다.

	@author	HumKyung.BAEK
**/
bool CAppDocData::IsLastDate(const CString &rIssueDate)
{
	CTime t = CTime::GetCurrentTime();
	CString rCurrentDate = t.Format( _T("%Y-%m-%d") );
	
	vector<STRING_T> oResult;
	const double dCurrentDate = CIssueDateRecord::GetValueFromIssueDate(rCurrentDate.operator LPCTSTR());
	const double dIssueDate = CIssueDateRecord::GetValueFromIssueDate(rIssueDate.operator LPCTSTR());

	return (dIssueDate < dCurrentDate);
}

/**	
	@brief	border setting을 읽는다.
	@author	HumKyung.BAEK
	@param	rIniFilePath	a parameter of type const CString&
	@return	void	
*/
void CAppDocData::LoadBorderSetting(const CString& rIniFilePath)
{
	TCHAR szBuf[MAX_PATH] = {'\0' ,};	
	//////////////////////////////////////////////////////////////////////////
	CString rMsg;
	rMsg.LoadString(IDS_BORDER_SETTING_ERROR);

	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();

	CString rApp = _T("Border Setting");
	map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("Text Color"));
	if(where != pDrawingSetup->end())
	{
		m_borderSetting.TextColorNo = ATOI_T(where->second.c_str());
	}
	else
	{
		m_borderSetting.TextColorNo = 0;
		const CString sMessage = rMsg + " Text Color value.";
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText(sMessage);
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}

	where = pDrawingSetup->find(_T("Font No"));
	if(where != pDrawingSetup->end())
	{
		m_borderSetting.FontNo = ATOI_T(where->second.c_str());
	}
	else
	{
		const CString sMessage = rMsg + " Font No value.";
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText(sMessage);
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}
	
	where = pDrawingSetup->find(_T("Text Width"));
	if(where != pDrawingSetup->end())
	{
		m_borderSetting.widthMulti = ATOF_T(where->second.c_str());
	}
	else
	{
		const CString sMessage = rMsg + " Text Width value.";
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText(sMessage);
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}
	
	where = pDrawingSetup->find(_T("Text Height"));
	if(where != pDrawingSetup->end())
	{
		m_borderSetting.heightMulti = ATOF_T(where->second.c_str());
	}
	else
	{
		const CString sMessage = rMsg + " Text Height value.";
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText(sMessage);
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}

	/// get text weight - 2011.09.21 added by humkyung
	where = pDrawingSetup->find(_T("Text Weight"));
	if(where != pDrawingSetup->end())
	{
		m_borderSetting.iTextWeight = ATOI_T(where->second.c_str());
	}
	/// up to here

	vector<STRING_T> oResult;
	where = pDrawingSetup->find(_T("BOM AREA"));
	if(where != pDrawingSetup->end())
	{
		CTokenizer<CIsComma>::Tokenize(oResult ,where->second, CIsComma());
		if(4 == oResult.size())
		{
			CIsPoint3d pt;
			pt.Set(atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , 0);
			m_borderSetting.SystemBMArea.volume.Add(pt);
			pt.Set(atof(oResult[2].c_str()) , atof(oResult[3].c_str()) , 0);
			m_borderSetting.SystemBMArea.volume.Add(pt);
		}
	}
	else
	{
		const CString sMessage = _T("PRT BM Area를 설정하십시요!");
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText(sMessage);
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}

	where = pDrawingSetup->find(_T("GRAPHIC AREA"));
	if(where != pDrawingSetup->end())
	{
		CTokenizer<CIsComma>::Tokenize(oResult ,where->second, CIsComma());
		if(4 == oResult.size())
		{
			CIsPoint3d pt;
			pt.Set(atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , 0);
			m_borderSetting.IsoGraphicArea.volume.Add(pt);
			pt.Set(atof(oResult[2].c_str()) , atof(oResult[3].c_str()) , 0);
			m_borderSetting.IsoGraphicArea.volume.Add(pt);
		}
	} else { }

	m_borderSetting.KeyDataAreaList.clear();
	const static CString _KEY_AREAS_[] = 
	{
		_T("PLANT_NO") , _T("AREA_NO") , _T("UNIT_NO") , _T("SECTION_NO") , _T("CLASS") , 
		_T("SEQUENCE") , _T("FLUID")   , _T("SHEET_NO"), _T("LINE_ID")
	};
	for(int i = 0;i < sizeof(_KEY_AREAS_) / sizeof(_KEY_AREAS_[0]);++i)
	{
		where = pDrawingSetup->find((_KEY_AREAS_[i] + _T("_area")).operator LPCTSTR());
		if(where != pDrawingSetup->end())
		{
			CTokenizer<CIsComma>::Tokenize(oResult ,where->second, CIsComma());
			if(4 == oResult.size())
			{
				IsoDwgDataArea area;
				area.sFieldName = _KEY_AREAS_[i];

				CIsPoint3d pt;
				pt.Set(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , 0);
				area.volume.Add(pt);
				pt.Set(ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , 0);
				area.volume.Add(pt);
				m_borderSetting.KeyDataAreaList.push_back(area);
			}
		}
	}
}

/**	
	@brief	Spec에 맞는 pcd파일을 로드한다.
	@author	humkyung
	@param	matl_spec	a parameter of type const CString &
	@return	CPCDFile&	
*/
CPCDFile& CAppDocData::GetPCDFile(const CString &matl_spec)
{
	const CString sSpecFilePath = m_rSpecFileFolder + CString(_T("\\")) + matl_spec + CString(_T(".pcd"));

	map<CString , CPCDFile*>::const_iterator where = m_pcdFileMap.find(sSpecFilePath);
	if(m_pcdFileMap.end() != where)
	{
		return *((*where).second);
	}
	if(CFileTools::DoesFileExist(sSpecFilePath))
	{
		CPCDFile* pPCDFile = CPCDFile::CreateInstance();
		if(NULL != pPCDFile)
		{
			m_pcdFileMap[sSpecFilePath] = pPCDFile;
			pPCDFile->Read(sSpecFilePath.operator LPCTSTR());
			
			return (*pPCDFile);
		}
	}

	throw exception("GetPCDFile function error");
}

/**
	@brief	

	@author	humkyung
**/
const CString& CAppDocData::GetTempFolder() const
{
	static CString __rTempFolder;

	if(__rTempFolder.IsEmpty())
	{
		char szBuf[MAX_PATH] = {'\0' ,};
		CString rIniFilePath = this->GetProject()->GetIniFilePath();
		CString	rApp("General");
		if(GetPrivateProfileString(rApp , "Path" , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			__rTempFolder = szBuf;
			if("\\" != __rTempFolder.Right(1)) __rTempFolder += "\\";
			__rTempFolder += "Temp";
		}
	}

	return __rTempFolder;
}

/**
	@brief	PCD 파일에서 WELD TYPE을 구한다.
	@author	humkyung
**/
CString CAppDocData::GetWeldType(const CString &matl_spec , const CString &rsMatlCode, const CString &rsItem)
{
	CString res( _T("NO") );
	try
	{
		const STRING_T sWeldTypeFilePath = this->GetProjectPath() + _T("\\Setting\\WeldType.xml");

		STRING_T rMatlCode = rsMatlCode, rItem = rsItem;
		CPCDFile& pcdFile = GetPCDFile(matl_spec);

		res = pcdFile.GetWeldType(rMatlCode.c_str() , rItem.c_str() , sWeldTypeFilePath);
	}
	catch(const exception&)
	{
	}

	return res;
}

void CAppDocData::SetArea(CString &rArea)
{
	m_ProjectArea = rArea;
}

/**
	@brief	return the first revision no
	@author	humkyung
**/
CString CAppDocData::GetFirstRevNoString() const
{
	if(m_pRevisionStageMap)
	{
		map<int,RevisionStage>::iterator where = m_pRevisionStageMap->find(100);
		if(where != m_pRevisionStageMap->end())
		{
			return (where->second.rText.c_str());
		}
	}

	return CString("");
}

/**	
	@brief	get revision no string
	@author	humkyung
	@date	????.??.??
**/
CString CAppDocData::GetRevNoString(CLineDataRecord* pLineDataRecord , const CString& rRevNo)
{
	CString sIsoFileExt;
	sIsoFileExt = this->GetProject()->GetOutputIsoFileExt(pLineDataRecord , rRevNo);

	return sIsoFileExt;
}

/**	
	@brief	get revision stage from revision no
	@author	humkyung
	@date	2010.05.25
**/
int CAppDocData::GetRevisionStage(RevisionStage& _RevisionStage , const int& iRevNo)
{
	if(m_pRevisionStageMap)
	{
		map<int,RevisionStage>::iterator where = m_pRevisionStageMap->find(iRevNo);
		if(where != m_pRevisionStageMap->end())
		{
			_RevisionStage = where->second;
			return ERROR_SUCCESS;
		}
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	
	@brief	BORDER SETTING 부분을 초기화 시킨다.
	@author	HumKyung
	@date	????.??.??
	@param	rIniFilePath
	@return
**/
void CAppDocData::InitBorderSettings(const CString& rIniFilePath)
{
	char szBuf[MAX_PATH + 1] = {'\0' ,};
	vector<string> oResult;
	static const CString rApp = _T("Border Setting");
	
	/*if(GetPrivateProfileString(rApp , "Area No Area" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			CIsPoint3d pt;
			pt.Set(atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , 0);
			m_areaNoArea.volume.Add(pt);
			pt.Set(atof(oResult[2].c_str()) , atof(oResult[3].c_str()) , 0);
			m_areaNoArea.volume.Add(pt);
		}
	}
	
	if(GetPrivateProfileString(rApp , "Area No Info" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_areaNoArea.sFieldName = szBuf;
	}*/
	
	/*if(GetPrivateProfileString(rApp , "Unit No Area" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			CIsPoint3d pt;
			pt.Set(atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , 0);
			m_unitNoArea.volume.Add(pt);
			pt.Set(atof(oResult[2].c_str()) , atof(oResult[3].c_str()) , 0);
			m_unitNoArea.volume.Add(pt);
		}
	}
	
	if(GetPrivateProfileString(rApp , "Unit No Info" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_unitNoArea.sFieldName = szBuf;
	}*/
	
	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	for(map<STRING_T,STRING_T>::const_iterator itr = pDrawingSetup->begin();itr != pDrawingSetup->end();++itr)
	{
		STRING_T sName(itr->first);
		IsString::ToUpper(sName);
		if(_T("BORDER_AREA") == sName)
		{
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(4 == oResult.size())
			{
				CIsPoint3d pt;
				pt.Set(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , 0);
				m_borderSetting.borderArea.volume.Add(pt);
				pt.Set(ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , 0);
				m_borderSetting.borderArea.volume.Add(pt);
			}
		}
		else if(0 == STRNCMP_T(sName.c_str(),_T("REMOVE AREA"),STRLEN_T(_T("REMOVE AREA"))))
		{
			IsoDwgDataArea Remove;
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(4 == oResult.size())
			{
				CIsPoint3d pt;
				pt.Set(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , 0);
				Remove.volume.Add(pt);
				pt.Set(ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , 0);
				Remove.volume.Add(pt);
				Remove.sFieldName = itr->first.c_str();
				
				m_RemoveAreaEntry.push_back(Remove);
			}
		}
		else if(0 == STRNCMP_T(sName.c_str(),_T("LATEST_"),STRLEN_T(_T("LATEST_"))))
		{
			RevPos oRevPos;

			oRevPos.rDesc = itr->first.c_str();
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(3 == oResult.size())
			{
				oRevPos.PosX = ATOF_T(oResult[0].c_str());
				oRevPos.PosY = ATOF_T(oResult[1].c_str());
				/// 2011.09.22 added by humkyung
				oRevPos.dTextScale = ATOF_T( oResult[2].c_str() );
				/// up to here

				m_oLatestRevData.insert(make_pair(itr->first.c_str() , oRevPos));
			}
		}
		else if(STRING_T::npos != sName.find(_T("_AREA")))
		{
			IsoDwgDataArea area;
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(4 == oResult.size())
			{
				CIsPoint3d pt;
				pt.Set(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , 0);
				area.volume.Add(pt);
				pt.Set(ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , 0);
				area.volume.Add(pt);
				area.sFieldName = sName.c_str();
				area.sFieldName = area.sFieldName.Left(area.sFieldName.GetLength() - 5);	/// remove '_AREA'
				if(!area.volume.IsEmpty()) m_IsoDwgDataAreaEntry.push_back(area);
			}
		}
	}
}

/**
	@brief	vol을 포함하는 DWG AREA를 리턴한다.

	@author	HumKyung

	@date	????.??.??

	@param	vol

	@return	IsoDwgDataArea*
**/
IsoDwgDataArea* CAppDocData::FindIsoDwgDataAreaCollideWith(CIsVolume vol)
{
	for(vector<IsoDwgDataArea>::iterator itr = m_IsoDwgDataAreaEntry.begin();itr != m_IsoDwgDataAreaEntry.end();++itr)
	{
		if(vol.CollideWith(itr->volume)) return (&(*itr));
	}

	return NULL;
}

/**
	@brief	data field인 DWG AREA를 리턴한다.

	@author	HumKyung

	@date	2011.01.20

	@param	sFieldName

	@return	IsoDwgDataArea*
**/
IsoDwgDataArea* CAppDocData::FindIsoDwgDataAreaWithFieldName(const CString& sFieldName)
{
	for(vector<IsoDwgDataArea>::iterator itr = m_IsoDwgDataAreaEntry.begin();itr != m_IsoDwgDataAreaEntry.end();++itr)
	{
		if(itr->sFieldName == sFieldName) return (&(*itr));
	}

	return NULL;
}

/**
	@brief	clear error count

	@author	humkyung
**/
void CAppDocData::ClearIsoEditError()
{
	m_IsoEditErrorEntry.clear();
	memset(CAppDocData::m_nIsoEditErrorCount , 0 , sizeof(int) * CHECK_ERROR_COUNT);
}

/**
	@author	humkyung
**/
CString CAppDocData::GetProductBOMFolder() const
{
	static const CString rApp( _T("Iso Edit Setting") );
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};

	CString rIniFilePath = this->GetProject()->GetIniFilePath();
	if(GetPrivateProfileString(rApp , _T("Product BOM Folder") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		return szBuf;
	}

	return CString( _T("") );
}

/**
	@brief	사용되는 UNIT 문자열을 리턴한다.
	
	@author	humkyung

	@return	UNIT 문자열
*/
STRING_T CAppDocData::GetUnitString() const
{
	return m_rUnit;
}

/**
	@brief	return project path
	
	@author	humkyung
*/
STRING_T CAppDocData::GetProjectPath() const
{
	return m_rProjectPath;
}

/**	
	@brief	return execute path
	
	@author	HumKyung.BAEK

	@date	2010.03.16

	@return	CString	
*/
CString CAppDocData::GetExecPath()
{
	static CString sExecPath;

	if(sExecPath.IsEmpty())
	{
		CFileTools::GetExecutableDirectory( sExecPath );
		sExecPath.TrimRight('\\');
	}
	
	return sExecPath;
}

/**	
	@brief	return next revision stage(increase minor rev. no when nNextGrade is -1, otherwise increase major rev. no)
	@author	humkyung	
	@date	2010.03.25
	@return	int	
*/
int CAppDocData::GetNextRevisionStage(RevisionStage &stage, const int &nRevNo)
{
	assert(m_pRevisionStageMap && "m_pRevisionStageMap is NULL");

	if(m_pRevisionStageMap)
	{
		const static int iFirstRevNo = 100;

		IncreaseRevNo eIncRevNo = CAppDocData::MajorRevNo;
		map<CString,CString>::const_iterator where = CProjectData::GetInstance()->m_pProjectSettingMap->find(_T("select_rev_no"));
		if(CProjectData::GetInstance()->m_pProjectSettingMap->end() != where)
		{
			eIncRevNo = (_T("MANUAL") == where->second) ? CAppDocData::MinorRevNo : CAppDocData::MajorRevNo;
		}
		//if(sSelRevText.IsEmpty())
		{
			/// when it's first revision for a line record - 2011.08.20 added by humkyung
			if(-1 == nRevNo)
			{
				if(m_pRevisionStageMap->end() != m_pRevisionStageMap->find(iFirstRevNo))
				{
					stage = (*m_pRevisionStageMap)[iFirstRevNo];
					return ERROR_SUCCESS;
				}
				return ERROR_BAD_ENVIRONMENT;
			}
			/// up to here
			else if(CAppDocData::MinorRevNo == eIncRevNo)	/// increase minor rev. no
			{
				for(map<int,RevisionStage>::const_iterator itr = m_pRevisionStageMap->begin();itr != m_pRevisionStageMap->end();++itr)
				{
					if(itr->second.nNo > nRevNo)
					{
						stage = itr->second;
						return ERROR_SUCCESS;
					}
				}
			}
			else
			{
				const int iCurGrade = int(nRevNo / 100) * 100;
				const int iNextGrade = iCurGrade + 100;
				if(m_pRevisionStageMap->end() != m_pRevisionStageMap->find(iNextGrade))
				{
					stage = (*m_pRevisionStageMap)[iNextGrade];
					return ERROR_SUCCESS;
				}
			}
		}
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	
	@brief	check grade is show or not
	
	@author	HumKyung.BAEK

	@date	2011.01.19

	@return	bool	
*/
bool CAppDocData::IsThisGradeShow(const int& iPrefixNo) const
{
	assert(m_pGradeDefineEntry && "m_pGradeDefineEntry is NULL");

	if(m_pGradeDefineEntry)
	{
		for(vector<GradeDefine>::iterator itr = m_pGradeDefineEntry->begin();itr != m_pGradeDefineEntry->end();++itr)
		{
			if(iPrefixNo == atoi(itr->rPrefix))
			{
				return (1 == itr->ShowOnRevTable);
			}
		}
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CAppDocData
    @function   IsTextModifiedErrorException
    @return     bool
    @param      CDgnText*   pDgnText2d
    @brief		return true if text's properties are as same as text modified error exception condition
******************************************************************************/
bool CAppDocData::IsTextModifiedErrorException(CDgnText* pDgnText2d) const
{
	assert(pDgnText2d && "pDgnText2d is NULL");

	if(pDgnText2d)
	{
		bool res[4] = {false,false,false,false};

		if(	(-1 == m_oTextModifiedErrorException.iLevel) && (-1 == m_oTextModifiedErrorException.iColor) &&
			(-1 == m_oTextModifiedErrorException.iStyle) && (-1 == m_oTextModifiedErrorException.iWeight))
		{
			return false;
		}

		res[0] = ((-1 == m_oTextModifiedErrorException.iLevel) || (pDgnText2d->level() == m_oTextModifiedErrorException.iLevel));
		res[1] = ((-1 == m_oTextModifiedErrorException.iColor) || (pDgnText2d->color() == m_oTextModifiedErrorException.iColor));
		res[2] = ((-1 == m_oTextModifiedErrorException.iStyle) || (pDgnText2d->style() == m_oTextModifiedErrorException.iStyle));
		res[3] = ((-1 == m_oTextModifiedErrorException.iWeight) || (pDgnText2d->weight()== m_oTextModifiedErrorException.iWeight));

		return ((true == res[0]) && (true == res[1]) && (true == res[2]) && (true == res[3]));
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class      CAppDocData
    @function   GetIsoFolderForRevAutomation
    @return     STRING_T
    @brief
******************************************************************************/
STRING_T CAppDocData::GetIsoFolder4RevAutomation() const
{
	return m_sIsoFolder4RevAutomation;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-31
    @class      CAppDocData
    @function   GetConnectionString
    @return     STRING_T
    @brief
******************************************************************************/
STRING_T CAppDocData::GetConnectionString()
{
	STRING_T res;

	const STRING_T strFilePath = this->GetProject()->path();
	const STRING_T strFileName = this->GetProject()->name();

	res = PROVIDER + strFilePath + _T("\\DataBase\\") + strFileName + _T(".MDB") + STRING_T(DBPASSWORD);
	
	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-26
    @function	Initialize 
    @return     const CString&
    @brief		initialize
******************************************************************************/
int CAppDocData::Initialize(const CString& sPrjIniFilePath)
{
	m_rIniFilePath = sPrjIniFilePath;
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-19
    @class      CAppDocData
    @function   GetPrjIniFilePath
    @return     CString
    @brief		return project ini file path
******************************************************************************/
CString CAppDocData::GetPrjIniFilePath() const
{
	return m_rIniFilePath;
}

/**
	@brief	set token
	@author	humkyung
	@date	2016.10.06
*/
int CAppDocData::SetToken(const STRING_T& sToken)
{
	m_sToken = sToken;
	return ERROR_SUCCESS;
}

/**
	@brief	save license information
	@author	humkyung
	@date	2016.10.07
*/
int CAppDocData::SetLicense(const STRING_T& sId,const STRING_T& sPassword)
{
	m_sLicenseId = sId;
	m_sLicensePassword = sPassword;
	return ERROR_SUCCESS;
}

/**	
	@brief	write header of log file
	@author	HumKyung.BAEK
	@date	2010.03.31
	@return	int	
*/
int CAppDocData::appendHeader(ofstream &ofile)
{
	ofile << _T("<!DOCTYPE HTML PUBLIC ");
	ofile << _T("\"-//W3C//DTD HTML 4.01 Transitional//EN\" ");
	ofile << _T("\"http://www.w3.org/TR/html4/loose.dtd\">") << std::endl;
	ofile << _T("<html>") << std::endl;
	ofile << _T("<head>") << std::endl;
	ofile << _T("<title>SmartISO Log Messages</title>") << std::endl;
	ofile << _T("<style type=\"text/css\">") << std::endl;
	ofile << _T("<!--") << std::endl;
	ofile << _T("body, table {font-family: arial,sans-serif; font-size: x-small;}") << std::endl;
	ofile << _T("th {background: #336699; color: #FFFFFF; text-align: left;}") << std::endl;
	ofile << _T("-->") << std::endl;
	ofile << _T("</style>") << std::endl;
	ofile << _T("</head>") << std::endl;
	ofile << _T("<body bgcolor=\"#FFFFFF\" topmargin=\"6\" leftmargin=\"6\">") << std::endl;
	ofile << _T("<p style=\"font-family:verdana;color:red;font-size:20px\">") << _T("</p>") << std::endl;
	ofile << _T("<hr size=\"1\" noshade>") << std::endl;
	ofile << _T("Log session start time ");
	SYSTEMTIME st;
	GetLocalTime( &st );
	CString strTimeData;
	strTimeData.Format("%d년 %d월 %d일 - %d시 %d분" , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
	ofile << strTimeData.operator LPCTSTR();

	ofile << _T("<br>") << std::endl;
	ofile << _T("<br>") << std::endl;
	ofile << _T("<table cellspacing=\"0\" cellpadding=\"4\" border=\"1\" bordercolor=\"#224466\" width=\"100%\">") << std::endl;
	
	ofile << _T("<tr>") << std::endl;
	
	ofile << _T("<th>NO</th>") << std::endl;
	ofile << _T("<th>desc.</th>") << std::endl;
	ofile << _T("</tr>") << std::endl;

	return ERROR_SUCCESS;
}

/**
	@brief	open iso file given by parameter

	@author	humkyung

	@date	2013.05.11
*/
int CAppDocData::OpenIsoFile(const STRING_T& sIsoFilePath) const
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0',};

	if( GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") , szBuf , MAX_PATH , this->GetPrjIniFilePath()) )
	{
		CString rMSFilePath = szBuf;
		if( !rMSFilePath.IsEmpty() )
		{
			::ShellExecute(NULL, _T("open") , rMSFilePath , sIsoFilePath.c_str() , NULL , SW_SHOW);
		}
		else
		{
			CString rString;
			rString.LoadString( IDS_NO_MSTATION_PATH );
			AfxMessageBox( rString );
		}
	}
	else
	{
		CString rString;
		rString.LoadString( IDS_NO_MSTATION_PATH );
		AfxMessageBox( rString );
	}

	return ERROR_SUCCESS;
}

/**
	@brief  get file version
	@author humkyung
	@date   2013.11.17
*/
CString CAppDocData::GetFileVersion() const
{
	CString res;

	TCHAR szModuleName[MAX_PATH+1];
	(void)GetModuleFileName(AfxGetInstanceHandle(), szModuleName , MAX_PATH);
	CFileVersion oVersion;
	if(TRUE == oVersion.Open(szModuleName))
	{
		res = oVersion.GetFileVersion();
	}
	
	return res;
}

/**
	@brief	set project
	@author	humkyung
	@date	2015.08.20
*/
int CAppDocData::SetProject(CProject* pProject)
{
	assert(pProject && "pProject is NULL");
	if(NULL != pProject) m_pProject = pProject;
	return ERROR_SUCCESS;
}

/**
	@brief	return project instance
	@author	humkyung
	@date	2015.08.20
*/
CProject* CAppDocData::GetProject() const
{
	return m_pProject;
}