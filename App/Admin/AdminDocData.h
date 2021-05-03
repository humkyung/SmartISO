// AdminDocData.h: interface for the CAdminDocData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINDOCDATA_H__7B110BE7_3453_4F39_B594_AB0A32662368__INCLUDED_)
#define AFX_ADMINDOCDATA_H__7B110BE7_3453_4F39_B594_AB0A32662368__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdminDoc.h"
#include <ThemeHelperST.h>

class CAdminDocData  
{
	CAdminDocData();
public:
	static CAdminDocData* GetInstance();
	virtual ~CAdminDocData();
public:
	static CString GetExecPath();
	void LoadData();
	CAdminDoc* GetDocument();
	CString GetIniFilePath() const;			/// 2011.08.29 added by humkyung
	void SetIniFilePath(const CString&);	/// 2011.08.29 added by humkyung

	CString m_rDDLFilePath;
	CString m_ProjectName;
	CString m_ProjectPath;
	CImageList m_headerImgList;

	CUser* m_pSelUser;

	CThemeHelperST m_themeHelperST;
private:
	CString m_rIniFilePath;
public:
	int SaveSettingData();
};

#endif // !defined(AFX_ADMINDOCDATA_H__7B110BE7_3453_4F39_B594_AB0A32662368__INCLUDED_)
