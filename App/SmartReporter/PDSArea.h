// PDSArea.h: interface for the CPDSArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDSAREA_H__ECB8C69D_C33D_49C2_B66C_4455D672DB6B__INCLUDED_)
#define AFX_PDSAREA_H__ECB8C69D_C33D_49C2_B66C_4455D672DB6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ado\AdoRemoteDB.h>

#include <vector>
#include <string>
using namespace std;

class CPDSArea;
class CPDSModel
{
public:
	CPDSModel() : m_bSelected(TRUE)
	{}
	virtual ~CPDSModel(){}

	CPDSModel(const CPDSModel& rhs)
	{
		(*this) = rhs;
	}

	CPDSModel& operator=(const CPDSModel& rhs)
	{
		if(this != &rhs)
		{
			m_bSelected = rhs.m_bSelected;
			m_rName = rhs.m_rName;
		}
		
		return (*this);
	}

	const CHAR_T* name() const
	{
		return m_rName.operator LPCTSTR();
	}

	void Select(const BOOL& bSelect)
	{
		m_bSelected = bSelect;
	}

	BOOL IsSelected() const
	{
		return m_bSelected;
	}
private:
	BOOL m_bSelected;
	CString m_rName;

	friend class CPDSArea;
};

class CPDSArea  
{
public:
	BOOL IsModelSelected(const CString& rModel);
	const CHAR_T* name() const;
	static void QueryAreaName(vector<CPDSArea>& AreaNameEntry , CAdoRemoteDB& RemoteDB , const CString& rProjectName , const CString& rDiscipline);

	CPDSModel* GetModelAt(const size_t& at);
	size_t GetModelCount() const;
	void QueryModelName(CAdoRemoteDB& RemoteDB , const CString& rProjectName , const CString& rDiscipline);
	CPDSArea();
	virtual ~CPDSArea();
	CPDSArea(const CPDSArea&);
	CPDSArea& operator=(const CPDSArea&);
private:
	CString m_rName;
	vector<CPDSModel> m_ModelEntry;
};

#endif // !defined(AFX_PDSAREA_H__ECB8C69D_C33D_49C2_B66C_4455D672DB6B__INCLUDED_)
