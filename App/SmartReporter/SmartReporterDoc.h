
// LineControlManagerDoc.h : interface of the CSmartReporterDoc class
//


#pragma once

#define	FLUID_CODE_NOTE_NO		125
#define	INSULATION_PURPOSE_NOTE_NO	220
#define	SPECIAL_END_PREP_NOTE_NO	330

class CSmartReporterDoc : public CDocument
{
protected: // create from serialization only
	CSmartReporterDoc();
	DECLARE_DYNCREATE(CSmartReporterDoc)

// Attributes
public:

// Operations
public:
	CString GetProjectFilePath() const;	/// 2012.05.05 added by humkyung
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CSmartReporterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	afx_msg void OnViewOraDataReport();
	DECLARE_MESSAGE_MAP()
private:
	STRING_T m_rExecPath;
	STRING_T m_rProjectFilePath;
public:
	afx_msg void OnFileSave();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


