// IsoFileListCtrl.h: interface for the CIsoFileListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISOFILELISTCTRL_H__EEAD9F01_F638_4239_BB5B_9E5AF3B55822__INCLUDED_)
#define AFX_ISOFILELISTCTRL_H__EEAD9F01_F638_4239_BB5B_9E5AF3B55822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileListCtrl.h"
#include <map>

class CIsoFileListCtrl : public CFileListCtrl  
{
public:
	int GetIndex(const CString& rFileName) const
	{
		map<CString,int>::const_iterator where = m_DisplayFileMap.find(rFileName);
		if(where != m_DisplayFileMap.end())
		{
			return where->second;
		}

		return -1;
	}

	int GetDisplayItemCount() const;
	void RemoveDisplayItem( const CString& rItem );
	void ClearDisplayFileList();
	void AddToDisplayFileList(const CString& rFileName , const int& nIndex=-1);
	virtual void  SetFolder(CString folder);
	CIsoFileListCtrl(CWnd *pOwner);
	virtual ~CIsoFileListCtrl();
protected:
	map<CString , int> m_DisplayFileMap;
	std::vector<CString> m_RevNoData;

	//{{AFX_MSG(CFileListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_ISOFILELISTCTRL_H__EEAD9F01_F638_4239_BB5B_9E5AF3B55822__INCLUDED_)
