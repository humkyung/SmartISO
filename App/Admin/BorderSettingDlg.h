#if !defined(AFX_BORDERSETTINGDLG_H__474E63EE_299C_40B3_BD2D_979FF22C4E54__INCLUDED_)
#define AFX_BORDERSETTINGDLG_H__474E63EE_299C_40B3_BD2D_979FF22C4E54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BorderSettingDlg.h : header file
//
#include "AdminDoc.h"
#include <PropTree/PropTree.h>
#include <PropTree/proptreeitemEditButton.h>
#include "QGLCanvas.h"

#include <DgnDocument.h>

#include <map>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CBorderSettingDlg dialog

class CBorderSettingDlg : public CDialog
{
// Construction
public:
	static CBorderSettingDlg* GetInstance();
	CDgnVolume GetBOMArea() const;
	CDgnDocument* GetDgnDocument();
	void Save();
	void UpdateContents();
	CBorderSettingDlg(CWnd* pParent = NULL);   // standard constructor
	~CBorderSettingDlg();

// Dialog Data
	//{{AFX_DATA(CBorderSettingDlg)
	enum { IDD = IDD_BORDER_SETTING };
		// NOTE: the ClassWizard will add data members here
	CButtonST m_wndBorderFileButton;
	//}}AFX_DATA
	CAdminDoc* m_pAdminDoc;

	CPropTree* m_ctrlPropTree;
	CPropTreeItem* m_pGeneralDataRoot;
	CPropTreeItem* m_pLineDataRoot;
	CPropTreeItem* m_pReservedDataRoot;
	CPropTreeItem* m_pRevisionDataRoot;
	CPropTreeItemEdit* m_pTextWidthItem;
	CPropTreeItemEdit* m_pTextHeightItem;
	CPropTreeItemEdit* m_pTextWeightItem;	/// 2011.09.21 added by humkyung
	CPropTreeItem* m_pIsoDwgAreaRoot;
	CPropTreeItem* m_pLinkKeyDataAreaRoot;	/// 2012.08.26 added by humkyung
	CPropTreeItem* m_pQCCheckAreaRoot;
	CPropTreeItem* m_pIsoDwgRemoveAreaRoot;
	map<CString , CPropTreeItem*> m_oPropTreeItemMap;	/// 2011.10.24 added by humkyung

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBorderSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	auto_ptr<CDgnDocument> m_DgnDoc;
	// Generated message map functions
	//{{AFX_MSG(CBorderSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonBorderFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString GetLineDataPos( const CString& rLineDataName , const CString& rIniFilePath);
	void InitLineDataTree(CAppPostgreSQL&);
	void InitLatestRevDataTree(CAppPostgreSQL&);
	void SaveRemoveAreaData(CAppPostgreSQL&) const;
	void InitReservDataTree(CAppPostgreSQL&);
	void InitRemoveAreaTree(CAppPostgreSQL&);
	void InitAreaDataTree(CAppPostgreSQL&);
	void SaveCheckAreaData(CAppPostgreSQL&,const CString& rIniFilePath);
	void InitCheckAreaDataTree(CAppPostgreSQL&);
	void InitRevDataTree(CAppPostgreSQL&);

	typedef struct
	{
		CPropTreeItemEditButton* pTreeItem;
		STRING_T sUserData;
	}TreeItemValue;

	map<STRING_T,TreeItemValue*> m_oTreeItemValueMap;	//! 2011.07.08 - added by humkyung
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BORDERSETTINGDLG_H__474E63EE_299C_40B3_BD2D_979FF22C4E54__INCLUDED_)
