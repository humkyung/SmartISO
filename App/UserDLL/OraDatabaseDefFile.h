// OraDatabaseDefFile.h: interface for the COraDatabaseDefFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORADATABASEDEFFILE_H__ACA9B191_BF43_4339_B86B_F45E39C52CBF__INCLUDED_)
#define AFX_ORADATABASEDEFFILE_H__ACA9B191_BF43_4339_B86B_F45E39C52CBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDocData.h"

#include <IsTools.h>
#include <vector>
using namespace std;

class USER_EXT_CLASS COraDatabaseDefFile  
{
	COraDatabaseDefFile& operator=(const COraDatabaseDefFile&){ return *this; }
	COraDatabaseDefFile(const COraDatabaseDefFile&){ }
public:
	bool GetAllFieldString(CString& rString) const;
	class USER_EXT_CLASS CFieldDef
	{
	public:
		CFieldDef()
		{
		}
		CFieldDef( const CFieldDef& rhs )
		{
			m_rFieldName = rhs.m_rFieldName;
			m_rFieldType = rhs.m_rFieldType;
			m_rNote      = rhs.m_rNote;
		}

		CFieldDef& operator=(const CFieldDef& def)
		{
			if(this != &def)
			{
				m_rFieldName = def.m_rFieldName;
				m_rFieldType = def.m_rFieldType;
				m_rNote      = def.m_rNote;
			}

			return (*this);
		}

		STRING_T ConvertValue(const STRING_T& sValue);	/// 2013.06.11 added by humkyung

		STRING_T m_rFieldName;
		STRING_T m_rFieldType;
		STRING_T m_rNote;
	private:
		bool HasAlphaCharacter(const STRING_T& str) const;
	};

	bool GetOraDefField(COraDatabaseDefFile::CFieldDef& defField , const STRING_T& rFieldName);
	bool GetFieldAt(CFieldDef& def , const size_t& at);
	size_t GetFieldCount() const;
	bool Read(const STRING_T& rFilePath);
	COraDatabaseDefFile();
	virtual ~COraDatabaseDefFile();
private:
	int ParseTableNumber(int& iTableNumber , int& iColumnCount , const STRING_T& line) const;	/// 2011.01.04 - added by HumKyung

	void ClearContents();
	STRING_T* m_pFilePath;
	vector<CFieldDef*>* m_pFieldEntry;
};

#endif // !defined(AFX_ORADATABASEDEFFILE_H__ACA9B191_BF43_4339_B86B_F45E39C52CBF__INCLUDED_)
