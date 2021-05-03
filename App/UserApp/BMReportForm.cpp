// BMReportForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "UserAppDocData.h"
#include "BMReportForm.h"
#include "WorkStatusDlg.h"
#include "tokenizer.h"
#include "BMLogFileForm.h"
#include "WeldDetailBMFile.h"
#include "IsString.h"
#include "WorkspaceWnd.h"
#include "IsoEditSettingPage.h"
#include "IssueDlg.h"
#include <util/FileTools.h>
#include <util/Path.h>
#include <SplitPath.h>

#include <excel9/ExcelAutomation.h>
#include <extend/libcsv/inc/csv_writer.hpp>
#include <IsPoly2d.h>
#include <DgnDocument.h>
#include <DgnText.h>

#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILE_NAME_COL	1
#define	FILE_EXIST		11
#define	FILE_NOT_FOUND	12

CString INFDataStruct::m_rHeaderItems[] = 
{
	_T("Project No.")  , _T("Class Name")     , _T("Symbol")       , _T("Main Size")  , _T("Sub Size")    , _T("Quantity")    , _T("Commodity Code") ,
	_T("Line No.")     , _T("Drawing Number") , _T("Plant No.")    , _T("Area No.")   , _T("Unit No.")    , _T("Section No.") , _T("Sheet No.") , 
	_T("Revision No.") , _T("Serial No.")     , _T("Field / Shop") , _T("Fluid Code") , _T("Insul. Type") , _T("Temperature") , _T("Paint") ,
	_T("Insul. Thk")
};

INFDataStruct::INFDataStruct() : m_rFormatString("%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s")
{
}

/**
	@brief	copy constructor
	@author	humkyung
	@date	2011.09.24
	@param	rhs;
*/
INFDataStruct::INFDataStruct( const INFDataStruct& rhs )
{
	m_rProjectNo = rhs.m_rProjectNo;
	m_sMatlSpec = rhs.m_sMatlSpec;
	m_rSymbol = rhs.m_rSymbol;
	m_rQuantity = rhs.m_rQuantity;
	m_sMatlCode = rhs.m_sMatlCode;
	m_rLineNo = rhs.m_rLineNo;
	m_rDwgNo = rhs.m_rDwgNo;
	m_rPlantNo = rhs.m_rPlantNo;
	m_rAreaNo = rhs.m_rAreaNo;
	m_rUnitNo = rhs.m_rUnitNo;
	m_rSectionNo = rhs.m_rSectionNo;
	m_rSheetNo = rhs.m_rSheetNo;
	m_rRevNo = rhs.m_rRevNo;
	m_rSerialNo = rhs.m_rSerialNo;
	m_rFieldShop = rhs.m_rFieldShop;
	m_rFluid = rhs.m_rFluid;
	m_rInsulType = rhs.m_rInsulType;
	m_rTemper = rhs.m_rTemper;
	m_rPaint = rhs.m_rPaint;
	m_rInsulThk = rhs.m_rInsulThk;
	m_sMatlDesc = rhs.m_sMatlDesc;

	m_rFormatString = rhs.m_rFormatString;
	m_rMainSize = rhs.m_rMainSize;
	m_rSubSize = rhs.m_rSubSize;
	m_sBoltLength = rhs.m_sBoltLength;
}

CString DBSTRING(const CString& str)
{
	CString res(str);
	if(-1 != res.Find(_T("'")))
	{
		res.Replace(_T("'") , _T("''"));
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-31
    @class      INFDataStruct
    @function   GetQueryClause
    @return     STRING_T
    @brief
******************************************************************************/
STRING_T INFDataStruct::GetQueryClause() const
{
	CString sSql(_T("INSERT INTO T_ISO_BOM_DATA(KEY,DRAW_NO,MATL_SPEC,MATL_CODE,MAIN_SIZE,SUB_SIZE,QTY,BOLT_LENGTH,MATL_DESC,SYMBOL,REV_NO) VALUES("));
	sSql += _T("'") + m_sKey + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rDwgNo) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_sMatlSpec) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_sMatlCode) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rMainSize) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rSubSize) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rQuantity) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_sBoltLength) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_sMatlDesc) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rSymbol) + _T("',");
	sSql += _T("'") +  DBSTRING(this->m_rRevNo) + _T("'");
	sSql += _T(")");

	return STRING_T(sSql.operator LPCTSTR());
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      INFDataStruct
    @function   GetHeaderString
    @return     int
    @param      vector<STRING_T>&   csv_row
    @brief
******************************************************************************/
int INFDataStruct::GetHeaderString(vector<STRING_T>& csv_row) const
{
	csv_row.clear();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const int iWidth = pDocData->m_oBMOutputFormat.m_oFieldFormatList.size();
		for(vector<STRING_T>::iterator itr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();itr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++itr)
		{
			if(_T("DRAWING NO") == (*itr))
			{
				csv_row.push_back(_T("DRAWING NO"));
			}
			else if(_T("SHT NO") == (*itr))
			{
				csv_row.push_back(_T("SHT NO"));
			}
			else if(_T("PTNO") == (*itr))
			{
				csv_row.push_back(_T("PTNO"));
			}
			else if(_T("MATL SPEC") == (*itr))
			{
				csv_row.push_back(_T("MATL SPEC"));
			}
			else if(_T("MATL CODE") == (*itr))
			{
				csv_row.push_back(_T("MATL CODE"));
			}
			else if(_T("SIZE1") == (*itr))
			{
				csv_row.push_back(_T("SIZE1"));
			}
			else if(_T("SIZE2") == (*itr))
			{
				csv_row.push_back(_T("SIZE2"));
			}
			else if(_T("QTY") == (*itr))
			{
				csv_row.push_back(_T("QTY"));
			}
			else if(_T("BOLT LEN") == (*itr))
			{
				csv_row.push_back(_T("BOLT LEN"));
			}
			else if(_T("MATL DESC") == (*itr))
			{
				csv_row.push_back(_T("MATL DESC"));
			}
			else if(_T("SYM") == (*itr))
			{
				csv_row.push_back(_T("SYM"));
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-24
    @class      INFDataStruct
    @function   GetHeaderString4Sum
    @return     int
    @param      vector<STRING_T>&   csv_row
    @brief
******************************************************************************/
int INFDataStruct::GetHeaderString4Sum(vector<STRING_T>& csv_row) const
{
	csv_row.clear();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		for(vector<STRING_T>::iterator itr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();itr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++itr)
		{
			if(_T("SHT NO") == (*itr))
			{
				csv_row.push_back(_T("SHT NO"));
			}
			else if(_T("PTNO") == (*itr))
			{
				csv_row.push_back(_T("PTNO"));
			}
			else if(_T("MATL SPEC") == (*itr))
			{
				csv_row.push_back(_T("MATL SPEC"));
			}
			else if(_T("MATL CODE") == (*itr))
			{
				csv_row.push_back(_T("MATL CODE"));
			}
			else if(_T("SIZE1") == (*itr))
			{
				csv_row.push_back(_T("SIZE1"));
			}
			else if(_T("SIZE2") == (*itr))
			{
				csv_row.push_back(_T("SIZE2"));
			}
			else if(_T("QTY") == (*itr))
			{
				csv_row.push_back(_T("QTY"));
			}
			else if(_T("BOLT LEN") == (*itr))
			{
				csv_row.push_back(_T("BOLT LEN"));
			}
			else if(_T("MATL DESC") == (*itr))
			{
				csv_row.push_back(_T("MATL DESC"));
			}
			else if(_T("SYM") == (*itr))
			{
				csv_row.push_back(_T("SYM"));
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	K5,KB로 시작되면 SUB SIZE로 MAIN SIZE를 대체한다.
	@author	HumKyung
	@date	????.??.??
	@param
	@return
**/
CString INFDataStruct::GetMainSize() const
{
	CString rMainSize = m_rMainSize;
	CString rPreMatlCode = m_sMatlCode.Mid(0 , 2);
	if((_T("K5") == rPreMatlCode) || (_T("KB") == rPreMatlCode))
	{
		rMainSize = m_rSubSize;
	}
	
	return rMainSize;
}

/**
	@brief	K5,KB,KT,KL,KI로 시작되면 SUB SIZE는 뿌리지 않는다.
	@author	humkyung
**/
CString INFDataStruct::GetSubSize() const
{
	CString rSubSize = m_rSubSize;
	CString rPreMatlCode = m_sMatlCode.Mid(0 , 2);
	if((_T("K5") == rPreMatlCode) || (_T("KB") == rPreMatlCode) || (_T("KT") == rPreMatlCode) || (_T("KL") == rPreMatlCode) || (_T("KI") == rPreMatlCode))
	{
		rSubSize = _T("");
	}

	return rSubSize;
}

/**
	@breif	
	
	@author	humkyung
**/
void INFDataStruct::SetMainSize(const CString& rSize)
{
	if(!rSize.IsEmpty())
	{
		const double nSize = atof(rSize);
		stringstream oss;
		oss << nSize;

		m_rMainSize = oss.str().c_str();
	}
	else
	{
		m_rMainSize = rSize;
	}
}

/**
	@brief	
	@author	humkyung
**/
void INFDataStruct::SetSubSize(const CString& rSize)
{
	if(!rSize.IsEmpty())
	{
		const double nSize = atof(rSize);
		OSTRINGSTREAM_T oss;
		oss << nSize;

		m_rSubSize = oss.str().c_str();
	}
	else
	{
		m_rSubSize = rSize;
	}
}

/**	
	@brief	B/M Report용 문자열을 생성하여 리턴한다.(Section No 포함)
	@author	humkyung
	@return	B/M Report용 문자열
*/
CString INFDataStruct::GetBMReportString4TextFile() const
{
	CString rString;
	rString.Format( m_rFormatString , 
		(_T("-") == m_rProjectNo) ? _T(" ") : m_rProjectNo ,	/// PROJECT NO
		(_T("-") == m_sMatlSpec) ? _T(" ") : m_sMatlSpec ,		/// CLASS
		(_T("-") == m_rSymbol)    ? _T(" ") : m_rSymbol ,		/// SYMBOL
		(_T("-") == GetMainSize())? _T(" ") : GetMainSize() ,	/// MAIN SIZE
		(_T("-") == GetSubSize()) ? _T(" ") : GetSubSize() ,	/// SUB SIZE
		(_T("-") == m_rQuantity)  ? _T(" ") : m_rQuantity ,		/// QUANTITY
		(_T("-") == m_sMatlCode) ? _T(" ") : m_sMatlCode ,		/// MATL CODE
		(_T("-") == m_rLineNo)    ? _T(" ") : m_rLineNo ,		/// LINE NO
		(_T("-") == m_rDwgNo)     ? _T(" ") : m_rDwgNo ,		/// DRAWING NUMBER
		(_T("-") == m_rPlantNo)   ? _T(" ") : m_rPlantNo ,		/// PLANT
		(_T("-") == m_rAreaNo)    ? _T(" ") : m_rAreaNo ,		/// AREA
		(_T("-") == m_rUnitNo)    ? _T(" ") : m_rUnitNo ,		/// UNIT
		(_T("-") == m_rSectionNo) ? _T(" ") : m_rSectionNo,		/// SECTION NO
		(_T("-") == m_rSheetNo)   ? _T(" ") : m_rSheetNo ,		/// SHEET NO
		(_T("-") == m_rRevNo)     ? _T(" ") : m_rRevNo ,		/// REV NO
		(_T("-") == m_rSerialNo)  ? _T(" ") : m_rSerialNo ,		/// SERIAL NO
		(_T("-") == m_rFieldShop) ? _T(" ") : m_rFieldShop ,	/// FIELD
		(_T("-") == m_rFluid)     ? _T(" ") : m_rFluid ,		/// FLUID
		(_T("-") == m_rInsulType) ? _T(" ") : m_rInsulType ,	/// INSUL.
		(_T("-") == m_rTemper)    ? _T(" ") : m_rTemper ,		/// TEMPERATURE
		(_T("-") == m_rPaint)     ? _T(" ") : m_rPaint ,		/// PAINT
		(_T("-") == m_rInsulThk)  ? _T(" ") : m_rInsulThk);	

	return rString;
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-31
    @class      INFDataStruct
    @function   GetBMReportString4CSV
    @return     int
    @param      vector<STRING_T>&  csv_row
    @brief
******************************************************************************/
int INFDataStruct::GetBMReportString4CSV(vector<STRING_T>& csv_row) const
{
	csv_row.clear();

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const int iWidth = pDocData->m_oBMOutputFormat.m_oFieldFormatList.size();
		for(vector<STRING_T>::iterator itr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();itr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++itr)
		{
			if(_T("DRAWING NO") == (*itr))
			{
				csv_row.push_back(m_rDwgNo.operator LPCTSTR());
			}
			else if(_T("SHT NO") == (*itr))
			{
				csv_row.push_back(m_rSheetNo.operator LPCTSTR());
			}
			else if(_T("PTNO") == (*itr))
			{
				csv_row.push_back(_T(""));
			}
			else if(_T("MATL SPEC") == (*itr))
			{
				csv_row.push_back( m_sMatlSpec.operator LPCTSTR() );
			}
			else if(_T("MATL CODE") == (*itr))
			{
				csv_row.push_back( m_sMatlCode.operator LPCTSTR() );
			}
			else if(_T("SIZE1") == (*itr))
			{
				csv_row.push_back(m_rMainSize.operator LPCTSTR());
			}
			else if(_T("SIZE2") == (*itr))
			{
				csv_row.push_back(m_rSubSize.operator LPCTSTR());
			}
			else if(_T("QTY") == (*itr))
			{
				csv_row.push_back(m_rQuantity.operator LPCTSTR());
			}
			else if(_T("BOLT LEN") == (*itr))
			{
				csv_row.push_back(m_sBoltLength.operator LPCTSTR());
			}
			else if(_T("MATL DESC") == (*itr))
			{
				csv_row.push_back(m_sMatlDesc.operator LPCTSTR());
			}
			else if(_T("SYM") == (*itr))
			{
				csv_row.push_back(m_rSymbol.operator LPCTSTR());
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-24
    @class      INFDataStruct
    @function   GetBMReportString4SumCSV
    @return     int
    @param      vector<STRING_T>&  csv_row
    @brief
******************************************************************************/
int INFDataStruct::GetBMReportString4SumCSV(vector<STRING_T>& csv_row) const
{
	csv_row.clear();

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const int iWidth = pDocData->m_oBMOutputFormat.m_oFieldFormatList.size();
		for(vector<STRING_T>::iterator itr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();itr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++itr)
		{
			if(_T("SHT NO") == (*itr))
			{
				csv_row.push_back(m_rSheetNo.operator LPCTSTR());
			}
			else if(_T("PTNO") == (*itr))
			{
				csv_row.push_back(_T(""));
			}
			else if(_T("MATL SPEC") == (*itr))
			{
				csv_row.push_back( m_sMatlSpec.operator LPCTSTR() );
			}
			else if(_T("MATL CODE") == (*itr))
			{
				csv_row.push_back( m_sMatlCode.operator LPCTSTR() );
			}
			else if(_T("SIZE1") == (*itr))
			{
				csv_row.push_back(m_rMainSize.operator LPCTSTR());
			}
			else if(_T("SIZE2") == (*itr))
			{
				csv_row.push_back(m_rSubSize.operator LPCTSTR());
			}
			else if(_T("QTY") == (*itr))
			{
				csv_row.push_back(m_rQuantity.operator LPCTSTR());
			}
			else if(_T("BOLT LEN") == (*itr))
			{
				csv_row.push_back(m_sBoltLength.operator LPCTSTR());
			}
			else if(_T("MATL DESC") == (*itr))
			{
				csv_row.push_back(m_sMatlDesc.operator LPCTSTR());
			}
			/*else if(_T("DRAWING NO") == (*itr))
			{
				csv_row.push_back( _T("") );///m_rDwgNo.operator LPCTSTR() );
			}*/
			else if(_T("SYM") == (*itr))
			{
				csv_row.push_back( m_rSymbol.operator LPCTSTR() );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-31
    @class      INFDataStruct
    @function   RetrieveDataFromIsoBMData
    @return     int
    @param      CIsoBMData& oIsoBMData
    @brief
******************************************************************************/
int INFDataStruct::RetrieveDataFromIsoBMData(CNtrData* pNtrData , CIsoBMData& oIsoBMData)
{
	/// clear variables
	m_rLineNo.Empty();
	m_rSerialNo.Empty();
	m_rFieldShop.Empty();
	m_rFluid.Empty();
	
	m_sMatlSpec.Empty();
	m_rSymbol.Empty();
	m_rMainSize.Empty();
	m_rSubSize.Empty();

	m_rQuantity.Empty();
	m_sMatlCode.Empty();
	m_sMatlDesc.Empty();
	m_sBoltLength.Empty();
	/// up to here

	if(pNtrData)
	{
		m_rLineNo    = pNtrData->GetLineNo();
		m_rSerialNo  = pNtrData->GetSerialNo();
		m_rFieldShop = pNtrData->GetFieldShop();
		m_rFluid     = pNtrData->GetFluidCode();
	}

	m_sMatlSpec = oIsoBMData.m_rMatlSpec;
	m_rSymbol    = oIsoBMData.m_rBMCSSYM;
	SetMainSize(oIsoBMData.GetMainSize());
	SetSubSize(oIsoBMData.GetSubSize());

	m_rQuantity  = oIsoBMData.QTY();				
	m_sMatlCode = oIsoBMData.m_rMatlCode;

	m_sBoltLength = oIsoBMData.m_sBoltLen;	/// 2011.09.23 added by humkyung
	if(_T("BT") == oIsoBMData.m_rBMCSSYM)
	{
		m_sBoltLength = oIsoBMData.GetSubSize();
	}

	m_sMatlDesc = oIsoBMData.matlDesc();
	const size_t iSubStringCount = oIsoBMData.GetSubStringCount();
	for(int i = 0;i < int(iSubStringCount);++i)
	{
		m_sMatlDesc += _T(" ") + oIsoBMData.GetSubStringAt(i);
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-24
    @class      INFDataStruct
    @function   HasSameValueExceptQty
    @return     bool
    @param      INFDataStruct& rhs
    @brief
******************************************************************************/
bool INFDataStruct::HasSameValueExceptQty( const INFDataStruct& rhs ) const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		for(vector<STRING_T>::iterator itr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();itr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++itr)
		{
			if(_T("MATL SPEC") == (*itr))
			{
				if(m_sMatlSpec != rhs.m_sMatlSpec) return false;
			}
			else if(_T("MATL CODE") == (*itr))
			{
				if(m_sMatlCode != rhs.m_sMatlCode) return false;
			}
			else if(_T("SIZE1") == (*itr))
			{
				if(m_rMainSize != rhs.m_rMainSize) return false;
			}
			else if(_T("SIZE2") == (*itr))
			{
				if(m_rSubSize != rhs.m_rSubSize) return false;
			}
			else if(_T("BOLT LEN") == (*itr))
			{
				if(m_sBoltLength != rhs.m_sBoltLength) return false;
			}
			else if(_T("MATL DESC") == (*itr))
			{
				if(m_sMatlDesc != rhs.m_sMatlDesc) return false;
			}
			else if(_T("SYM") == (*itr))
			{
				if(m_rSymbol != rhs.m_rSymbol) return false;
			}
		}

		return true;
	}

	return false;
}

static CLineDataRecord* pCurProcessLineDataRecord__ = NULL;
/////////////////////////////////////////////////////////////////////////////
// CBMReportForm

IMPLEMENT_DYNCREATE(CBMReportForm, CFormView)

CBMReportForm::CBMReportForm()
	: CFormView(CBMReportForm::IDD) , m_pLineDataRecordSet(NULL) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CBMReportForm)
	//}}AFX_DATA_INIT
}

CBMReportForm::~CBMReportForm()
{
	try
	{
		if(m_pLineDataRecordSet != NULL) SAFE_DELETE(m_pLineDataRecordSet);
		if(NULL != m_pBoldFont) SAFE_DELETE(m_pBoldFont);
	}
	catch(...)
	{
		
	}
}

void CBMReportForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMReportForm)
	DDX_Text(pDX , IDC_STATIC_STATUS , m_rOutputDrawingFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMReportForm, CFormView)
	//{{AFX_MSG_MAP(CBMReportForm)
	ON_WM_SIZE()
	//ON_UPDATE_COMMAND_UI(IDC_BUTTON_START, OnUpdateButtonStart)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_LINE_DATA, OnKeydownListLineData)
	ON_BN_CLICKED(IDC_BUTTON_ISSUE, OnButtonIssue)
	//}}AFX_MSG_MAP
	/*ON_COMMAND(ID_HORZ_RESIZE, OnHorzResize)*/
	ON_WM_HELPINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINE_DATA, &CBMReportForm::OnNMDblclkListLineData)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINE_DATA, &CBMReportForm::OnLvnItemchangedListLineData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMReportForm diagnostics

#ifdef _DEBUG
void CBMReportForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CBMReportForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBMReportForm message handlers

BOOL CBMReportForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CBMReportForm::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init)
	{
		m_ImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));
		
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		CWnd* pWnd = GetDlgItem(IDC_STATIC_1);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Output Iso Drawing Folder") + _T(" : "));

		if(m_wndLineDataReport.SubclassWindow(GetDlgItem(IDC_LIST_LINE_DATA)->GetSafeHwnd()))
		{
			m_wndLineDataReport.SetWindowPos(NULL , CLineListSelectOption1::WIDTH , 30 , 0 , 0 , SWP_NOZORDER);
			LONG style = GetWindowLong(m_wndLineDataReport.m_hWnd , GWL_STYLE);
			style |= WS_EX_CLIENTEDGE;
			SetWindowLong(m_wndLineDataReport.m_hWnd , GWL_EXSTYLE , style );
			m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
			m_wndLineDataReport.SetHeaderImageList(&m_ImageList);
			m_wndLineDataReport.SetHeadings( _T("[],30 ;") + _TR("File Name") + _T(",300;Rev No,50;Status,70") );
			m_wndLineDataReport.SetGridLines();
			m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
			m_wndLineDataReport.EnableHeaderCheckBox(TRUE);
		}

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,_T("Arial"));

		__init = true;
	}
}

void CBMReportForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	static bool __init_size = true;
	
	DetermineLayout(cx,cy);
}

/**	
	@brief	The CBMReportForm::OnNotify function
	@author	humkyung
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@return	BOOL	
*/
BOOL CBMReportForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQuery4.GetDlgCtrlID() == pNMHDR->idFrom))
	{	
		CString rQuertCondition = pWorkspaceBar->m_wndQuery4.GetInAreaQueryCondition();
		vector< string > issueDates;
		LoadLineDataRecordSetFromDatabase( rQuertCondition , issueDates);
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQuery4.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = pWorkspaceBar->m_wndQuery4.GetQueryCondition();
		vector< string > issueDates;
		pWorkspaceBar->m_wndQuery4.GetSelectedIssueDate( issueDates );
		LoadLineDataRecordSetFromDatabase(rQueryCondition , issueDates );
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**	
	@brief	database에서 line data record들을 읽는다.
	@param	rQueryCondition	a parameter of type const CString &
	@param	pIssueDate	a parameter of type const char*
	@return	void	
*/
void CBMReportForm::LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition,const vector<STRING_T>& issueDates)
{	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		
		BeginWaitCursor();
		
		CString rSelRevNo = pWorkspaceBar->m_wndQuery4.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || ( _T("All") == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates );
		}
		else
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates );
		}
		if(NULL != m_pLineDataRecordSet)
		{
			int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				CString rMsg;
				rMsg.LoadString(IDS_NO_RECORD);
				AfxMessageBox(rMsg);
				EndWaitCursor();
				return;
			}
			SyncOutputFileWithDataRecord();
		}
		EndWaitCursor();
	}
}

/**	
	@brief	line data record에 대응하는 output file을 리스트한다.
	@author	HumKyung.BAEK
	@param	nRecordCount int
	@remarks	BM summary는 항상 LATEST를 기준으로 하게 된다.
	@return	void	
*/
void CBMReportForm::SyncOutputFileWithDataRecord()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	m_wndLineDataReport.SetRedraw(FALSE);
	m_wndLineDataReport.DeleteAllItems();
	m_IsoFileMap.clear();
	
	TCHAR szBuf[MAX_PATH + 1]={'\0',};
	vector<STRING_T> oFileName , oFileExt;
	if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Output File Name Format") , _T("") , szBuf , MAX_PATH , rIniFilePath))
	{
		CTokenizer<CIsFromString>::Tokenize(oFileName , szBuf , CIsFromString( _T(":") ));
	}
	/// 2011.08.20 added by humkyung
	if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Output File Ext Format") , _T("") , szBuf , MAX_PATH , rIniFilePath))
	{
		CTokenizer<CIsFromString>::Tokenize(oFileExt , szBuf , CIsFromString( _T(":") ));
	}
	/// up to here
	CString rDesc, rFluid, rLineNo, rSheet,rLineDataValue;
	
	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvitem.iImage = FILE_NOT_FOUND;
	lvitem.cchTextMax = 32;

	int nFileCount = 0;
	const int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
	CString rSelRevNo = pWorkspaceBar->m_wndQuery4.GetSelectedRevNo();
	for(int j=0 ; j < nRecordCount ; j++)
	{
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(j);
		CString rIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
		CString sIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord);

		if(pLineDataRecord)
		{
			CString rOutputFilePath = m_rOutputDrawingFolder;
			if( _T("\\") != rOutputFilePath.Right( 1 ) ) rOutputFilePath += _T("\\");
			rOutputFilePath += rIsoFileName + _T(".") + sIsoFileExt;
			const BOOL bExist = CFileTools::DoesFileExist( rOutputFilePath );

			CString rOutputFileName = rIsoFileName + _T(".") + sIsoFileExt;
			lvitem.iItem = m_wndLineDataReport.GetItemCount();
			lvitem.iSubItem = 0;
			( bExist ) ? lvitem.iImage = FILE_EXIST : lvitem.iImage = FILE_NOT_FOUND;
			CString rString;
			rString.Format(_T("%d") , lvitem.iItem + 1);
			lvitem.pszText = (char*)(rString).operator LPCSTR();
			const int at = m_wndLineDataReport.InsertItem(&lvitem);
			if(-1 != at)
			{
				m_wndLineDataReport.SetItemText(at , 1 , rOutputFileName );
				CDataField* pField = pLineDataRecord->FindWithFieldName(_T("REV_NO"));
				if(pField) m_wndLineDataReport.SetItemText(at,2,pField->value());
				m_wndLineDataReport.SetItemText(at , 3 , ( bExist ) ? _T("Found") : _T("Not Found") );
				m_wndLineDataReport.SetItemData(at , (DWORD) pLineDataRecord ); /// line data record 저장.
			}

			++nFileCount;
		}
	}
	m_wndLineDataReport.GroupByColumn(2);
	m_wndLineDataReport.SetRedraw( TRUE );
	
	ShowRecordCount(nFileCount);
}

/**	
	@brief	The CBMReportForm::InitContents function
	@return	void	
*/
void CBMReportForm::InitContents()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	pWorkspaceBar->m_wndQuery4.InitContents( this );

	CUserAppView* pView = static_cast<CUserAppView*>(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rIniFilePath= pDocData->GetProject()->GetIniFilePath();

	TCHAR szBuf[MAX_PATH] = {'\0' ,};
	if(GetPrivateProfileString( _T("Iso Edit Setting") , _T("Output Iso Dwg File Folder") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rOutputDrawingFolder = szBuf;
		if(_T("\\") != m_rOutputDrawingFolder.Right(1)) m_rOutputDrawingFolder += _T("\\");
		UpdateData(FALSE);
	}
}

/**	
	@brief	recalculate sub window's position and size.
	@author	humkyung
	@param	cx	a parameter of type int
	@param	cy	a parameter of type int
	@return	void
*/
void CBMReportForm::DetermineLayout(int cx, int cy)
{
	const int gap = 3 , LeftX = 2 , RIGHT_MARGIN = 20;
	CWnd* pWnd = NULL;

	CRect rect[3] , rectScheduleJobButton , rectBMButton;
	{
	}
	
	if( m_wndLineDataReport.GetSafeHwnd())
	{	
		pWnd = GetDlgItem(IDC_STATIC_1);
		if(pWnd)
		{			
			pWnd->SetWindowPos(NULL , LeftX , gap , 0 , 0 , SWP_NOSIZE);
		}

		pWnd = GetDlgItem(IDC_STATIC_STATUS);
		if(pWnd)
		{
			CWnd* pWnd1 = GetDlgItem(IDC_STATIC_1);
			pWnd1->GetWindowRect(&rect[0]);
			ScreenToClient(&rect[0]);
						
			pWnd->SetWindowPos(NULL , rect[0].right + 3 , rect[0].top , cx - rect[0].right - 6 , rect[0].Height() , SWP_NOZORDER);
			pWnd->GetWindowRect(&rect[1]);
			ScreenToClient(&rect[1]);
		}

		{
			m_wndLineDataReport.GetWindowRect(&rect[0]);
			ScreenToClient(&rect[0]);
			m_wndLineDataReport.SetWindowPos(NULL , LeftX , rect[1].bottom + 5 , (cx - LeftX) , cy - rect[1].bottom - 10 , SWP_NOZORDER);
		}
	}

	if(::IsWindow(m_wndLineDataReport.GetSafeHwnd()))
	{
		CRect rect;
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

/**	
	@brief	BOM Report 부분 시작
	@author	HumKyung.BAEK
	@date	????.??.??
	@return	void	
*/
int CBMReportForm::Generate()
{
	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox( _T("Can't create thread!!!") );
	}
	else
	{
		CAppDocData::ERROR_STRING_ENTRY.clear();
		m_OutputFileFullPathEntry.clear();
		dlg.DoModal();
		
		vector<LineDataRecordMap> aLineDataRecordEntry;
		this->GetSelectedLineDataRecord(aLineDataRecordEntry);
		CIssueDlg aIssueDlg(m_rOutputDrawingFolder,aLineDataRecordEntry);
		if(IDOK == aIssueDlg.DoModal())
		{
			/*const int at = AfxMessageBox( _T("Do you want to copy BOM file to server?") ,MB_YESNO | MB_ICONQUESTION);
			if(at == IDYES)
			{
				/// TEMP FOLDER에서 PRODUCT FOLDER로 복사한다.
				for(vector<CBMOutputPath>::iterator itr = m_OutputFileFullPathEntry.begin(); itr != m_OutputFileFullPathEntry.end(); ++itr)
				{
					CopyFile((itr)->m_rTempFilePath , (itr)->m_rProductFilePath , FALSE);
				}
			}
			*/
		}

		SaveErrorSumFile();	/// ERROR SUMMARY FILE을 저장한다.
		/// LOG 파일을 저장한다.
		if(!CAppDocData::ERROR_STRING_ENTRY.empty())
		{
			CAppDocData* pDocData = CAppDocData::GetInstance();
			CString rTempFolder = pDocData->GetTempFolder();
			if(_T("\\") != rTempFolder) rTempFolder += _T("\\");
			ofstream ofile(rTempFolder + _T("BMReport.log"));
			if(ofile.is_open())
			{
				for(vector<CString>::iterator itr = CAppDocData::ERROR_STRING_ENTRY.begin();itr != CAppDocData::ERROR_STRING_ENTRY.end();++itr)
				{
					ofile << itr->operator LPCTSTR() << std::endl;
				}
				ofile.close();
				
				/// 2011.01.24 - added by HumKyung
				///::ShellExecute(NULL, "open", "notepad.exe", rTempFolder + "BMReport.log" , NULL, SW_SHOW);
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-07
    @function   OnUpdateButtonStart
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CBMReportForm::OnUpdateButtonStart(CCmdUI* pCmdUI) 
{
	/*if(NULL != m_pQualifiedFileList)
	{
		pCmdUI->Enable(m_pQualifiedFileList->GetItemCount() > 1);
	}*/
}

/**
	@author	humkyung
**/
UINT CBMReportForm::StatusThreadEntry(LPVOID pVoid)
{
	return ((CBMReportForm*)pVoid)->StatusThread();
}

/**	
	@brief	쓰레드 진입부.
	@author	humkyung	
	@date	????.??.??
	@return	IDOK = success , IDCANCEL = fail	
*/
UINT CBMReportForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running
		
		CAppDocData*  pDocData  = CAppDocData::GetInstance();	
		if(pDocData)
		{
			TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
			const CString rIniFilePath = pDocData->GetPrjIniFilePath();
			CString rProductBOMFolder = pDocData->GetProductBOMFolder();
			if(_T("\\") != rProductBOMFolder.Right(1)) rProductBOMFolder += _T("\\");
			 
			SYSTEMTIME stStart , stEnd;
			GetLocalTime(&stStart);

			/// Iso Edit 에러 개수를 초기화 한다.
			pDocData->ClearIsoEditError();

			/// 내부 변수들 초기화...
			InitVariables();

			CString rTitle( _T("B/M Reporting...") );
			double dProgress = 0.f;

			CString rOutputFolder = m_rOutputDrawingFolder , rOutputFilePath;
			if(_T("\\") != rOutputFolder.Right(1)) rOutputFolder += _T("\\");

			vector<LineDataRecordMap> aLineDataRecordEntry;
			this->GetSelectedLineDataRecord(aLineDataRecordEntry);
			const int nItemCount = aLineDataRecordEntry.size();
			for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin();itr != aLineDataRecordEntry.end();++itr)
			{
				CString sFileName = m_wndLineDataReport.GetItemText(itr->nItem , FILE_NAME_COL );
				sFileName.MakeUpper();		/// 대문자로 변경
				pCurProcessLineDataRecord__ = itr->pLineDataRecord;
				
				if( NULL != pCurProcessLineDataRecord__ )
				{
					rOutputFilePath = rOutputFolder + sFileName;
					ProcessIsoFile(itr->nItem , rOutputFilePath , rIniFilePath);
				}
				else
				{
					pCurProcessLineDataRecord__ = NULL;
				}

				int progress = (int)((++dProgress) / ( nItemCount - 1 )* 100);
				CWorkStatusDlg::UpdateWorkStatus(rTitle , progress);
			}

			GetLocalTime(&stEnd);
			
			CString rSumFileNameFormat , sINFAllSumFileName , rPipingFileNameFormat , rOthersFileNameFormat;
			rSumFileNameFormat.LoadString(IDS_INF_SUM_FILE_NAME_FORMAT);
			rPipingFileNameFormat.LoadString(IDS_INF_PIPING_FILE_NAME_FORMAT);
			rOthersFileNameFormat.LoadString(IDS_INF_OTHERS_FILE_NAME_FORMAT);
			CString sINFAllFilePath , rPipingINFFilePath , rOthersINFFilePath , sBMOutputFileName;
			sINFAllFilePath.Format(rSumFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			/// 2011.09.24 added by humkyung
			sINFAllSumFileName.Format(_T("%d-%02d-%02d-%d-%d.INF.ALL.SUM.CSV") , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			/// up to here
			/// 2013.01.26 added by humkyung
			sBMOutputFileName.Format(_T("%d-%02d-%02d-%d-%d.xls") , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			/// up to here
			rPipingINFFilePath.Format(rPipingFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			rOthersINFFilePath.Format(rOthersFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);

			CBMOutputPath oINFSumFilePath;
			oINFSumFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + sINFAllFilePath;
			oINFSumFilePath.m_rProductFilePath = rProductBOMFolder + sINFAllFilePath;

			/// 2011.09.24 added by humkyung
			CBMOutputPath oINFAllSumFilePath;
			{
				oINFAllSumFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + sINFAllSumFileName;
				oINFAllSumFilePath.m_rProductFilePath = rProductBOMFolder + sINFAllSumFileName;
			}
			/// up to here

			CBMOutputPath oPipingINFFilePath;
			oPipingINFFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rPipingINFFilePath;
			oPipingINFFilePath.m_rProductFilePath = rProductBOMFolder + rPipingINFFilePath;

			CBMOutputPath oOthersINFFilePath;
			oOthersINFFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rOthersINFFilePath;
			oOthersINFFilePath.m_rProductFilePath = rProductBOMFolder + rOthersINFFilePath;

			/// 2013.01.26 added by humkyung
			CBMOutputPath oBMFilePath;
			oBMFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + sBMOutputFileName;
			oBMFilePath.m_rProductFilePath = rProductBOMFolder + sBMOutputFileName;
			::CopyFile(CAppDocData::GetExecPath() + _T("\\Backup\\BMPrintTemp.xls") , oBMFilePath.m_rTempFilePath , FALSE);
			/// up to here

			GenerateINFSumFile( oBMFilePath , oINFSumFilePath , oINFAllSumFilePath , oPipingINFFilePath , oOthersINFFilePath );

			CString rFileNameFormat;
			rFileNameFormat.LoadString(IDS_PAINT_FILE_NAME_FORMAT);
			CString rPaintCodeFilePath;
			rPaintCodeFilePath.Format(rFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			CBMOutputPath oPaintCodeFilePath;
			oPaintCodeFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rPaintCodeFilePath;
			oPaintCodeFilePath.m_rProductFilePath = rProductBOMFolder + rPaintCodeFilePath;
			if(GeneratePaintCode(oPaintCodeFilePath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oPaintCodeFilePath);
			}

			rFileNameFormat.LoadString(IDS_INS_FILE_NAME_FORMAT);
			CString rInsBMFilePath;
			rInsBMFilePath.Format(rFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			CBMOutputPath oInsBMFilePath;
			oInsBMFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + "\\" + rInsBMFilePath;
			oInsBMFilePath.m_rProductFilePath = rProductBOMFolder + rInsBMFilePath;
			if(GenerateInsulationBM(oInsBMFilePath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oInsBMFilePath);
			}

			rFileNameFormat.LoadString(IDS_WELD_DETAIL_FILE_NAME_FORMAT);
			CString rWeldDetailBMFilePath;
			rWeldDetailBMFilePath.Format(rFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			CBMOutputPath oWeldDetailBMFilePath;
			oWeldDetailBMFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + "\\" + rWeldDetailBMFilePath;
			oWeldDetailBMFilePath.m_rProductFilePath = rProductBOMFolder + rWeldDetailBMFilePath;
			if(MakeWeldDetailFile(oWeldDetailBMFilePath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oWeldDetailBMFilePath);
			}
			
			rFileNameFormat.LoadString(IDS_WELD_DIA_CHECK);
			CString rWeldDiaCheckPath;
			rWeldDiaCheckPath.Format(rFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			CBMOutputPath oWeldDiaCheckPath;
			oWeldDiaCheckPath.m_rTempFilePath    = pDocData->GetTempFolder() + "\\" + rWeldDiaCheckPath;
			oWeldDiaCheckPath.m_rProductFilePath = rProductBOMFolder + rWeldDiaCheckPath;
			if(MakeWeldDiaCheck(oWeldDiaCheckPath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oWeldDiaCheckPath);
			}
			
			rFileNameFormat.LoadString(IDS_PIPE_SUPPORT_CHECK);
			CString rPipeSupportCheckPath;
			rPipeSupportCheckPath.Format(rFileNameFormat , stEnd.wYear , stEnd.wMonth , stEnd.wDay , stEnd.wHour , stEnd.wMinute);
			CBMOutputPath oPipeSupportCheckPath;
			oPipeSupportCheckPath.m_rTempFilePath    = pDocData->GetTempFolder() + "\\" + rPipeSupportCheckPath;
			oPipeSupportCheckPath.m_rProductFilePath = rProductBOMFolder + rPipeSupportCheckPath;
			if(GeneratePipeSupportCheck(oPipeSupportCheckPath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oPipeSupportCheckPath);
			}

			GenerateLogFile();	/// LOG 파일을 생성한다.

			long nElapsedTime = (stEnd.wMinute - stStart.wMinute) * 60 + (stEnd.wSecond - stStart.wSecond);
			CString rString;
			rString.Format("elapsed time : %d" , nElapsedTime);
		}

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox( _T("Unknown error") );
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

/**	
	@brief	NTR의 내용을 파일로 출력한다.

	@author	humkyung
**/
bool CBMReportForm::MakeNTRFile(const CString& rNtrFilePath)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakeNtrFileCheck())
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		OFSTREAM_T ofile(rNtrFilePath);
		if(pDocData->m_borderSetting.borderArea.volume.Empty())
		{
			ofile << _T("Border Setting을 먼저 해주세요.") << std::endl;
		}
		for(vector<CNtrData>::iterator itr = m_qcCheck.m_ntrTextEntry.begin();itr != m_qcCheck.m_ntrTextEntry.end();++itr)
		{
			ofile << itr->GetTextString() << std::endl;
		}
		ofile.close();

		return true;
	}

	return false;
}

/**
	@brief	

	@author	humkyung
**/
bool CBMReportForm::MakePRTFile(const CString& rPrtFilePath)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakePrtFileCheck())
	{
		OFSTREAM_T ofile(rPrtFilePath);
		for(vector<CString>::iterator itr = m_qcCheck.m_prtTextEntry.begin();itr != m_qcCheck.m_prtTextEntry.end();++itr)
		{
			ofile << (*itr).operator const char*() << std::endl;
		}
		ofile.close();

		return true;
	}

	return false;
}

/**	
	@brief	Masterial Control System의 interface 파일을 생성한다.
	@author	HumKyung
	@date	????.??.??
	@remarks\n
	1. CSV 파일 버전을 생성.
	@param	rINFFilePath
	@return	void	
*/
bool CBMReportForm::GenerateINFFile(CAppPostgreSQL& database, const CString& rINFFilePath )
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	/// default inf format string
	CString rINFFormatString( "%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s" );

	INFDataStruct INFStruct;	
	if( pWorkspaceBar->m_BMReportOptionDlg.GetGenerateINFFileCheck() )
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
		
		CString rPlantNo , rAreaNo , rUnitNo , rSectionNo;
		CString rFluidCode , rPaintCode , rSheetNo, rLineNo, rInsType, rInsTemp;
		CString sDrawingNo , rRevNo , rSerialNo = _T(" ") , rFieldShop = _T(" ");
		CString rInsulThk;
		if( !m_qcCheck.m_drawingNoDatas.empty() ) sDrawingNo = m_qcCheck.m_drawingNoDatas.front();
		
		if( NULL != pCurProcessLineDataRecord__ )
		{
			try
			{
				CDataField* pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("PLANT_NO"));
				if( pField ) rPlantNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("AREA_NO"));
				if( pField ) rAreaNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("UNIT_NO"));
				if( pField ) rUnitNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("SECTION_NO"));
				if( pField ) rSectionNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("SHEET_NO"));
				if( pField ) rSheetNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("LINE_NO"));
				if( pField ) rLineNo = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("FLUID_CODE"));
				if( pField ) rFluidCode = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("PAINT_CODE"));
				if( pField ) rPaintCode = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("INS_TYPE"));
				if( pField ) rInsType = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("INS_TEMP"));
				if( pField ) rInsTemp = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("INS_THICKNESS"));
				if( pField ) rInsulThk = pField->value();
				/// 2011.01.26 - added by HumKyung
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("DRAW_NO"));
				if( pField ) sDrawingNo = pField->value();
				///
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("KEY"));
				if( pField ) INFStruct.m_sKey = pField->value();
				pField = pCurProcessLineDataRecord__->FindWithFieldName(_T("REV_NO"));
				if( pField ) INFStruct.m_rRevNo = pField->value();

				INFStruct.m_rDwgNo     = sDrawingNo;
				INFStruct.m_rPlantNo   = rPlantNo;
				INFStruct.m_rAreaNo    = rAreaNo;
				INFStruct.m_rUnitNo    = rUnitNo;
				INFStruct.m_rSectionNo = rSectionNo;
				INFStruct.m_rSheetNo   = rSheetNo;
				INFStruct.m_rInsulType = rInsType;
				INFStruct.m_rTemper    = rInsTemp;
				INFStruct.m_rPaint     = rPaintCode;
				INFStruct.m_rInsulThk  = rInsulThk;
			}
			catch(const exception& ex)
			{
				AfxMessageBox( ex.what() );
			}
		}

		TCHAR szBuf[MAX_PATH + 1]={'\0',};
		CString rProjectNo;
		if(GetPrivateProfileString( _T("General") , _T("Project No") ,NULL,szBuf,MAX_PATH,rIniFilePath))
		{
			rProjectNo = szBuf;
		}

		csv_writer csv;
		vector<STRING_T> csv_row;
		if(1)///0 == STRICMP_T(_T("Text File") , pDocData->m_oBMOutputFormat.m_sOutputStyle.c_str()))
		{
			OFSTREAM_T ofile(rINFFilePath);
			if( ofile.good() )
			{
				INFStruct.GetHeaderString(csv_row);
				csv.write_line(ofile , csv_row);

				for(vector< CIsoBMData >::iterator itr = m_qcCheck.m_pipeDatas.begin(); itr != m_qcCheck.m_pipeDatas.end(); ++itr)
				{
					CNtrData* pNtrData = m_qcCheck.FindNtrData(*itr);
					INFStruct.RetrieveDataFromIsoBMData(pNtrData , *itr);
					
					INFStruct.GetBMReportString4CSV(csv_row);
					STRING_T aRow = csv.write_line(ofile , csv_row);
					{
						(*database.session()) << INFStruct.GetQueryClause();
					}

					m_oINFExlEntry.push_back(INFStruct);
					m_InfAllItemEntry.push_back( aRow );
					m_PipingBulkItemEntry.push_back( aRow );
				}

				for(vector< CIsoBMData >::iterator itr = m_qcCheck.m_generalDatas.begin(); itr != m_qcCheck.m_generalDatas.end(); ++itr)
				{
					CNtrData* pNtrData = m_qcCheck.FindNtrData(*itr);
					INFStruct.RetrieveDataFromIsoBMData(pNtrData , *itr);
									
					INFStruct.GetBMReportString4CSV(csv_row);
					STRING_T aRow = csv.write_line(ofile , csv_row);
					{
						(*database.session()) << INFStruct.GetQueryClause();
					}

					m_oINFExlEntry.push_back(INFStruct);
					m_InfAllItemEntry.push_back( aRow );
					m_PipingBulkItemEntry.push_back( aRow );
				}

				for(vector< CIsoBMData >::iterator itr = m_qcCheck.m_miscellDatas.begin(); itr != m_qcCheck.m_miscellDatas.end(); ++itr)
				{
					CNtrData* pNtrData = m_qcCheck.FindNtrData(*itr);
					INFStruct.RetrieveDataFromIsoBMData(pNtrData , *itr);
									
					INFStruct.GetBMReportString4CSV(csv_row);
					STRING_T aRow = csv.write_line(ofile , csv_row);
					{
						(*database.session()) << INFStruct.GetQueryClause();
					}

					m_oINFExlEntry.push_back(INFStruct);
					m_InfAllItemEntry.push_back( aRow );
					m_PipingBulkItemEntry.push_back( aRow );
				}

				ofile.close();
			}
		}
		
		/// INSTRUMENT , SPECIAL ITEM , SUPPORT는 INF 파일로 뽑지 않는다.
		/// OHTER ITEM 컨테이너에 저장한다.
		for( vector< CIsoBMData >::iterator itr = m_qcCheck.m_InstOrSpecialDatas.begin(); itr != m_qcCheck.m_InstOrSpecialDatas.end(); ++itr)
		{
			CNtrData* pNtrData = m_qcCheck.FindNtrData(*itr);
			INFStruct.RetrieveDataFromIsoBMData(pNtrData , *itr);
							
			INFStruct.GetBMReportString4CSV(csv_row);
			STRING_T aRow = csv.retrieve_string(csv_row);
			{
				(*database.session()) << INFStruct.GetQueryClause();
			}

			m_InfAllItemEntry.push_back( aRow );
			m_OthersItemEntry.push_back( aRow );
		}
		
		for( vector< CIsoBMData >::iterator itr = m_qcCheck.m_supportDatas.begin(); itr != m_qcCheck.m_supportDatas.end(); ++itr)
		{
			CNtrData* pNtrData = m_qcCheck.FindNtrData(*itr);
			INFStruct.RetrieveDataFromIsoBMData(pNtrData , *itr);
							
			INFStruct.GetBMReportString4CSV(csv_row);
			STRING_T aRow = csv.retrieve_string(csv_row);
			{
				(*database.session()) << INFStruct.GetQueryClause();
			}

			m_InfAllItemEntry.push_back( aRow );
			m_OthersItemEntry.push_back( aRow );
		}

		return true;
	}

	return false;
}

/**	
	@brief
	@author	humkyung
**/
void CBMReportForm::LogFileSynchronizing(const vector<CString>& LogTextEntry)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	if(m_pViewLogFileForm && pWorkspaceBar->m_BMReportOptionDlg.GetLogFileSynchronizingCheck())
	{
		for(vector<CString>::const_iterator itr = LogTextEntry.begin();itr != LogTextEntry.end();++itr)
		{
			m_pViewLogFileForm->AddLog(*itr);
		}
	}
}

/**	
	@brief	log 파일을 작성합니다.
	@author	humkyung
**/
bool CBMReportForm::GenerateLogFile()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();	
	if( pWorkspaceBar->m_BMReportOptionDlg.GetSaveLogFileCheck() )
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		CString rProductBOMFolder = pDocData->GetProductBOMFolder();
		if(_T("\\") != rProductBOMFolder.Right(1)) rProductBOMFolder += _T("\\");
		
		SYSTEMTIME st;
		GetLocalTime(&st);
		
		CString rFileNameFormat;
		rFileNameFormat.LoadString(IDS_LOG_FILE_NAME_FORMAT);
		CString rFileName;
		rFileName.Format(rFileNameFormat , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
		
		CBMOutputPath oOutputFilePath;
		oOutputFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rFileName;
		oOutputFilePath.m_rProductFilePath = rProductBOMFolder + rFileName;
		
		OFSTREAM_T ofile(oOutputFilePath.m_rTempFilePath);
		if(ofile.is_open())
		{
			for(vector<CString>::iterator itr = m_LogTextEntry.begin();itr != m_LogTextEntry.end();++itr)
			{
				ofile << itr->operator LPCTSTR() << std::endl;
			}
			ofile.close();

			m_OutputFileFullPathEntry.push_back(oOutputFilePath);
		}

		return true;
	}

	return false;
}

/**
	@brief	weld detail 파일을 생성합니다.
	@author	humkyung
	@param	rOutputFilePath = 생성할 weld detail 파일 경로
*/
bool CBMReportForm::MakeWeldDetailFile(const CString& rOutputFilePath)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakeWeldDetailCheck())
	{
		return m_weldDetailBMFile.Write(rOutputFilePath);
	}

	return false;
}

/**	
	@brief	Generate PAINT CODE TEXT FILE
	@author	humkyung
	@return	void
*/
bool CBMReportForm::GeneratePaintCode(const CString& rOutputFilePath)
{	
	static const CString rFormatString("%-20s %-16s %-30s %-10s %-10s %-10s %-16s %-10s %-10s %-10s %-10s %-10s");
	static const CString rHeaderString[]=
	{
		_T("+MATL CODE") , _T("+BMCS SYMBOL") , _T("+DRAW NO.") , _T("+MATL SPEC") , _T("+SIZE1") , _T("+SIZE2") , _T("+PAINT CODE") , _T("+LENGTH") , 
		_T("+PLANT NO")  , _T("+AREA NO")     , _T("+UNIT NO")  , _T("+SECTION NO")
	};

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakePaintCodeCheck())
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		ofstream ofile(rOutputFilePath);

		CString rString;
		rString.Format(rFormatString , rHeaderString[0] , rHeaderString[1] , rHeaderString[2] , rHeaderString[3] , rHeaderString[4] , rHeaderString[5] , 
			rHeaderString[6] , rHeaderString[7] , rHeaderString[8] , rHeaderString[9] , rHeaderString[10] , rHeaderString[11]);
		ofile << rString.operator const char*() << std::endl;
		rString.Format(rFormatString , " " , " " , " " , " " , " " , " " , " " , " " , " " , " " , " " , " ");
		rString.Replace(" " , "=");
		ofile << rString.operator const char*() << std::endl;

		for(vector<CIsoBMData>::iterator itr = m_paintCodeEntry.begin();itr != m_paintCodeEntry.end();++itr)
		{
			rString.Format(rFormatString,
				itr->matlCode() , itr->bmcsSYM() , itr->m_rDrawingNo , 
				itr->matlSpec() , itr->GetMainSize() , itr->GetSubSize() , itr->m_rPaintCode , itr->QTY(),
				itr->plantNo()  , itr->areaNo()      , itr->unitNo()     , itr->sectionNo());
			ofile << rString.operator const char*() << std::endl;
		}

		
		ofile.close();

		return true;
	}

	return false;
}

/**	
	@brief	insulation b/m text file을 생성한다.
	@author	humkyung
**/
bool CBMReportForm::GenerateInsulationBM(const CString& rOutputFilePath)
{
	static const CString rFormatString("%-20s %-16s %-30s %-10s %-10s %-10s %-16s %-16s %-10s %-10s %-10s %-10s %-10s");
	static const CString rHeaderString[]=
	{
		"+MATL CODE" , "+BMCS SYMBOl" , "+DRAW NO." , "+MATL SPEC" , "+SIZE1" , "+SIZE2" , "+INS TYPE" , "+INS THK" , "+LENGTH" , 
		"+PLANT NO" , "+AREA NO" , "+UNIT NO" , "+SECTION NO"
	};

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakeInsBMCheck())
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		ofstream ofile(rOutputFilePath);
		
		CString rString;
		rString.Format(rFormatString , rHeaderString[0] , rHeaderString[1] , rHeaderString[2] , rHeaderString[3] , rHeaderString[4] , rHeaderString[5] ,
			 rHeaderString[6] , rHeaderString[7] , rHeaderString[8] , rHeaderString[9] , rHeaderString[10] , rHeaderString[11] , rHeaderString[12]);
		ofile << rString.operator const char*() << std::endl;
		rString.Format(rFormatString , " " , " " , " " , " " , " " , " " , " " , " " , " " , " " , " " , " " , " ");
		rString.Replace(" " , "=");
		ofile << rString.operator const char*() << std::endl;
		
		for(vector<CIsoBMData>::iterator itr = m_paintCodeEntry.begin();itr != m_paintCodeEntry.end();++itr)
		{
			rString.Format(rFormatString,
				itr->matlCode() , itr->bmcsSYM() , itr->m_rDrawingNo , 
				itr->matlSpec() , itr->GetMainSize() , itr->GetSubSize() , itr->m_rInsType , itr->m_rInsThickness , itr->QTY() ,
				itr->plantNo()  , itr->areaNo()      , itr->unitNo()     , itr->sectionNo());
			ofile << rString.operator const char*() << std::endl;
		}
		
		ofile.close();

		return true;
	}

	return false;
}

/**	
	@brief	create bm text files against a iso drawing file.
	@author	humkyung
	@param	nItem	a parameter of type const int&
	@param	rOutputFilePath	iso drawing file path
	@param	rIniFilePath	ini 파일 경로
	@return	void
*/
void CBMReportForm::ProcessIsoFile(const int& nItem , const CString &rOutputFilePath , const CString &rIniFilePath)
{
	CString rString;
	try
	{
		CString rCheckMsg;
		m_qcCheck.Check(rCheckMsg , rOutputFilePath , false);

		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		const CString sIniFilePath= pDocData->GetProject()->GetIniFilePath();

		try
		{
			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			if(NULL != pCurProcessLineDataRecord__)
			{
				CDataField* pField = pCurProcessLineDataRecord__->FindWithFieldName( _T("KEY") );
				if( pField )
				{
					CString sKey(pField->value());
					(*database.session()) << CString(_T("DELETE FROM T_ISO_BOM_DATA WHERE KEY='") + sKey + _T("'")).operator LPCTSTR();
				}
			}

			CString rProductBOMFolder = pDocData->GetProductBOMFolder();
			if(_T("\\") != rProductBOMFolder.Right(1)) rProductBOMFolder += _T("\\");

			CString rINFFilePath( rOutputFilePath );
			CBMOutputPath oOutputFilePath;

			CSplitPath path(rINFFilePath);
			int at = rINFFilePath.ReverseFind('\\');
			rINFFilePath = rINFFilePath.Right(rINFFilePath.GetLength() - at - 1);
			rINFFilePath = path.GetFileName() + _T(".CSV");
			oOutputFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rINFFilePath;
			oOutputFilePath.m_rProductFilePath = rProductBOMFolder + rINFFilePath;
			if(GenerateINFFile(database , oOutputFilePath.m_rTempFilePath ))
			{
				m_OutputFileFullPathEntry.push_back(oOutputFilePath);
			}
			
			CString rNtrFilePath(rOutputFilePath);
			
			at = rNtrFilePath.ReverseFind('\\');
			rNtrFilePath = rNtrFilePath.Right(rNtrFilePath.GetLength() - at - 1);
			rNtrFilePath+= _T(".NTR");
			oOutputFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + "\\" + rNtrFilePath;
			oOutputFilePath.m_rProductFilePath = rProductBOMFolder + rNtrFilePath;
			if(MakeNTRFile( oOutputFilePath.m_rTempFilePath ))
			{
				m_OutputFileFullPathEntry.push_back(oOutputFilePath);
			}

			CString rPrtFilePath(rOutputFilePath);
			
			at = rPrtFilePath.ReverseFind('\\');
			rPrtFilePath = rPrtFilePath.Right(rPrtFilePath.GetLength() - at - 1);
			rPrtFilePath+= _T(".PRT");
			oOutputFilePath.m_rTempFilePath    = pDocData->GetTempFolder() + _T("\\") + rPrtFilePath;
			oOutputFilePath.m_rProductFilePath = rProductBOMFolder + rPrtFilePath;
			if(MakePRTFile(oOutputFilePath.m_rTempFilePath))
			{
				m_OutputFileFullPathEntry.push_back(oOutputFilePath);
			}
			
			vector<CIsoBMData> IsoBMDataEntry;
			IsoBMDataEntry.insert(IsoBMDataEntry.end() , m_qcCheck.m_pipeDatas.begin()    , m_qcCheck.m_pipeDatas.end());
			IsoBMDataEntry.insert(IsoBMDataEntry.end() , m_qcCheck.m_generalDatas.begin() , m_qcCheck.m_generalDatas.end());
			const CString rDrawingNo(_T("DRAWING NO"));
			const CString rInsType(_T("INS TYPE"));
			const CString rInsThickness(_T("INS THICKNESS"));
			const CString rPaintCode(_T("PAINT CODE"));
			vector<CIsoBMData>::iterator itr = IsoBMDataEntry.begin();
			for(itr = IsoBMDataEntry.begin();itr != IsoBMDataEntry.end();++itr)
			{
				if( !m_qcCheck.m_drawingNoDatas.empty() ) itr->m_rDrawingNo = m_qcCheck.m_drawingNoDatas.front();
				if( NULL != pCurProcessLineDataRecord__ )
				{
					try
					{
						itr->GetDataFromLineDataRecord(pCurProcessLineDataRecord__);
					}
					catch(...)
					{
						AfxMessageBox( _T("unknown error in query line record!!!") );
					}
				}
			}
			
			vector<CIsoBMData> SupportBMEntry;
			SupportBMEntry.insert(SupportBMEntry.end() , m_qcCheck.m_supportDatas.begin() , m_qcCheck.m_supportDatas.end());
			for(itr = SupportBMEntry.begin();itr != SupportBMEntry.end();++itr)
			{
				if( !m_qcCheck.m_drawingNoDatas.empty() ) itr->m_rDrawingNo = m_qcCheck.m_drawingNoDatas.front();
				if( NULL != pCurProcessLineDataRecord__ )
				{
					try
					{
						itr->GetDataFromLineDataRecord(pCurProcessLineDataRecord__);
					}
					catch(...)
					{
						AfxMessageBox( _T("unknown error in query support data!!!") );
					}
				}
			}
			
			m_paintCodeEntry.insert(m_paintCodeEntry.end() , IsoBMDataEntry.begin() , IsoBMDataEntry.end());
			m_paintCodeEntry.insert(m_paintCodeEntry.end() , SupportBMEntry.begin() , SupportBMEntry.end());
			m_SupportBMEntry.insert(m_SupportBMEntry.end() , SupportBMEntry.begin() , SupportBMEntry.end());
			
			CString rFileName(rOutputFilePath);
			at = rOutputFilePath.ReverseFind('\\');
			if(-1 != at)
			{
				rFileName = rOutputFilePath.Right(rOutputFilePath.GetLength() - at - 1);
			}
			rString.Format(_T(" [%-4d / %-4d] %s") , nItem , this->m_wndLineDataReport.GetCheckedItemCount() - 1, rFileName);

			m_weldDetailBMFile.Add(rString , m_qcCheck);
			m_weldDiaCheck.Add(m_weldDetailBMFile);

			vector<CString> LogTextEntry;
			if(GenerateLogText(LogTextEntry , nItem))
			{
				m_LogTextEntry.insert(m_LogTextEntry.end() , LogTextEntry.begin() , LogTextEntry.end());
			}
			LogFileSynchronizing(LogTextEntry);
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
}

void CBMReportForm::SetViewLogFileForm(CBMLogFileForm *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		m_pViewLogFileForm = p;
	}
}

/**	
	@brief
	@author	humkyung
**/
bool CBMReportForm::MakeWeldDiaCheck(const CString& rOutputFilePath)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetMakeWeldDiaCheck())
	{
		return m_weldDiaCheck.Write(rOutputFilePath);
	}

	return false;
}

/**
	@brief	write csv files to excel file
	@author	humkyung
	@date	2013.01.22
*/
int CBMReportForm::WriteCSVFilesToExcelFile( const CString& sExlFilePath , const CString& sCSVFile1 , const CString& sCSVFile2 )
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	{
		VARIANT varArgs[4];
		varArgs[3].vt = VT_BSTR;
		varArgs[3].bstrVal = sExlFilePath.AllocSysString();
		varArgs[2].vt = VT_BSTR;
		varArgs[2].bstrVal = sCSVFile1.AllocSysString();
		varArgs[1].vt = VT_BSTR;
		varArgs[1].bstrVal = sCSVFile2.AllocSysString();
		varArgs[0].vt = VT_BSTR;
		varArgs[0].bstrVal = pDocData->GetPrjIniFilePath().AllocSysString();

		CUserAppApp* pApp = static_cast<CUserAppApp*>(AfxGetApp());

		VARIANT varRet;
		varRet.vt = VT_BOOL;
		const int iNoOfParams = ARRAYSIZE(varArgs);
		int iRet = pApp->CallManagedFunction(L"WriteCSVToExcel" , iNoOfParams , varArgs , &varRet);
		if(!iRet)
		{
			if(0 != varRet.boolVal)
			{
				return ERROR_SUCCESS;
			}
		}
	}*/

	return ERROR_BAD_ENVIRONMENT;
}

/**	
	@brief	여러개의 INF 파일의 내용을 하나의 SUMMARY 파일로 만든다.
	
	@author	humkyung

	@param	rINFSumFilePath	a parameter of type const CString &

	@remarks
	Piping INF / Others INF / SUMMARY 파일을 뽑는다.

	@return	void	
*/
void CBMReportForm::GenerateINFSumFile(const CBMOutputPath &oBMOutputFilePath , const CBMOutputPath &oINFSumFilePath , const CBMOutputPath &oINFAllSumFilePath , const CBMOutputPath& oPipingINFFilePath , const CBMOutputPath& oOthersINFFilePath)
{	
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	/// default inf format string
	CString rINFFormatString( _T("%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s%-50s") );

	INFDataStruct InfStruct;
	if( pWorkspaceBar->m_BMReportOptionDlg.GetGenerateINFSumFileCheck() )
	{	
		/// write summary csv file
		STRING_T aHeadText;
		{
			OFSTREAM_T ofile( oINFSumFilePath.m_rTempFilePath );
			csv_writer csv;
			vector<STRING_T> csv_row;
			
			vector< STRING_T >::iterator itr = m_InfAllItemEntry.begin();
			for(itr = m_InfAllItemEntry.begin();itr != m_InfAllItemEntry.end();++itr)
			{
				ofile << (*itr) << std::endl;
			}
			ofile.close();
		}
		/// write all summary csv file - 2011.09.24 added by humkyung
		{
			OFSTREAM_T ofile( oINFAllSumFilePath.m_rTempFilePath );
			if(ofile.is_open())
			{
				csv_writer csv;
				vector<STRING_T> csv_row;
				
				for(vector<INFDataStruct>::iterator itr = m_oINFExlEntry.begin();itr != m_oINFExlEntry.end();++itr)
				{
					for(vector<INFDataStruct>::iterator jtr = itr + 1;jtr != m_oINFExlEntry.end();)
					{
						if(itr->HasSameValueExceptQty( *jtr ))
						{
							double dQty[2] = {0.0 , 0.0};
							sscanf_s(itr->m_rQuantity , _T("%lf") , &(dQty[0]));
							sscanf_s(jtr->m_rQuantity , _T("%lf") , &(dQty[1]));
							STRINGSTREAM_T oss;
							oss << (dQty[0] + dQty[1]);

							itr->m_rQuantity = oss.str().c_str();

							jtr = m_oINFExlEntry.erase(jtr);
						}
						else
						{
							++jtr;
						}
					}
				}

				for(vector<INFDataStruct>::iterator itr = m_oINFExlEntry.begin();itr != m_oINFExlEntry.end();++itr)
				{
					itr->GetBMReportString4SumCSV( csv_row );
					csv.write_line( ofile , csv_row );
				}
				ofile.close();
			}
		}
		/// up to here
		this->WriteCSVFilesToExcelFile(oBMOutputFilePath.m_rTempFilePath , oINFSumFilePath.m_rTempFilePath , oINFAllSumFilePath.m_rTempFilePath);
		::DeleteFile(oINFSumFilePath.m_rTempFilePath);
		::DeleteFile(oINFAllSumFilePath.m_rTempFilePath);

		OFSTREAM_T oPipingFile(oPipingINFFilePath.m_rTempFilePath);
		oPipingFile << aHeadText << std::endl;
		for(vector< STRING_T >::iterator itr = m_PipingBulkItemEntry.begin();itr != m_PipingBulkItemEntry.end(); ++itr)
		{
			oPipingFile << (*itr) << std::endl;
		}
		oPipingFile.close();

		OFSTREAM_T oOthersFile(oOthersINFFilePath.m_rTempFilePath);
		oOthersFile << aHeadText << std::endl;
		for(vector< STRING_T >::iterator itr = m_OthersItemEntry.begin();itr != m_OthersItemEntry.end(); ++itr)
		{
			oOthersFile << (*itr) << std::endl;
		}
		oOthersFile.close();

		///
		m_OutputFileFullPathEntry.push_back(oBMOutputFilePath);		/// 2013.01.26 added by humkyung
		m_OutputFileFullPathEntry.push_back(oPipingINFFilePath);
		m_OutputFileFullPathEntry.push_back(oOthersINFFilePath);
	}
}

/**
	@brief	
	@author	humkyung
**/
void CBMReportForm::GenerateInterfaceExcel()
{
}

/**	
	@brief	The CBMReportForm::ShowSelectedCount function
	@author	humkyung
	@param	nCount	a parameter of type int
	@return	void	
*/
void CBMReportForm::ShowSelectedCount()
{
	/*int nRecordCount = m_nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_QU_COUNT);
	if(pWnd)
	{
		int nCount = m_pQualifiedFileList->GetItemCount() - 1; ///< '..'은 개수에서 제외

		CString rCount, rSlash = _T(" / ") , rRecodrCount;
		rCount.Format( _T("%d") ,nCount);
		rRecodrCount.Format(_T("%d"),nRecordCount);
		rCount = rCount + rSlash + rRecodrCount;
		pWnd->SetWindowText((LPCTSTR)rCount);
	}*/
}

void CBMReportForm::ShowRecordCount(int nRecordCount)
{
	m_nRecordCount = nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount;
		rCount.Format( _T("%d") ,m_nRecordCount);
		pWnd->SetWindowText((LPCTSTR)rCount);
	}
}

/**	
	@brief	The CBMReportForm::GetSelectedLineDataRecord function
	@author	HumKyung.BAEK
	@param	IsoEditItemEntry	a parameter of type vector<long>&
	@return	void	
*/
void CBMReportForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& aLineDataRecordEntry)
{
	aLineDataRecordEntry.clear();
	LineDataRecordMap lineDataRecordMap;

	const int iItemCount = m_wndLineDataReport.GetItemCount();
	for(int i = 0;i < iItemCount;++i)
	{
		if(TRUE == m_wndLineDataReport.GetCheck(i))
		{
			lineDataRecordMap.nItem = i;
			lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(lineDataRecordMap.nItem));
			if(lineDataRecordMap.pLineDataRecord) aLineDataRecordEntry.push_back(lineDataRecordMap);		
		}
	}
}

/**	
	@brief	클래스 내부 변들을 초기화 한다.
	@author	humkyung
**/
void CBMReportForm::InitVariables()
{
	m_paintCodeEntry.clear();
	m_insBMEntry.clear();
	m_SupportBMEntry.clear();
	m_weldDetailBMFile.Clear();
	m_weldDiaCheck.Clear();
	
	m_InfAllItemEntry.clear();
	m_PipingBulkItemEntry.clear();
	m_OthersItemEntry.clear();
	m_LogTextEntry.clear();
	
	m_oINFExlEntry.clear();
	m_pViewLogFileForm->ClearAllLog();
}

/**	
	@brief	pipe support data file을 생성한다.
	@author	humkyung
**/
bool CBMReportForm::GeneratePipeSupportCheck(const CString &rOutputFilePath)
{
	static const CString FORMAT   = _T("%-25s %-10s %-20s %-90s %-10s %-10s %-10s %-10s %-10s");
	static const CString HEADERS[]=
	{
		_T("+DRAW NO.") , _T("+PIPE SIZE") , _T("+SUPPORT NO.") , _T("+SUPPORT DESC") , _T("+QT'Y") , _T("+PLANT NO") , 
		_T("+AREA NO") , _T("+UNIT NO") , _T("+SECTION NO")
	};

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(pWorkspaceBar->m_BMReportOptionDlg.GetPipeSupportCheck())
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		OFSTREAM_T ofile(rOutputFilePath);
		CString rString;
		rString.Format(FORMAT , HEADERS[0] , HEADERS[1] , HEADERS[2] , HEADERS[3] , HEADERS[4] , HEADERS[5] , HEADERS[6] , HEADERS[7] , HEADERS[8]);
		ofile << rString.operator LPCSTR() << std::endl;

		for(vector<CIsoBMData>::iterator itr = m_SupportBMEntry.begin();itr != m_SupportBMEntry.end();++itr)
		{
			CString rDescString = itr->GetAllSubString() , rDesc;
			if(rDescString.IsEmpty())
			{
				CString rDataString = itr->dataString();
				const int at = rDataString.Find( _T("SUPPORT DATA:")) ;
				const int nStart = at + CString( _T("SUPPORT DATA:")).GetLength();
				rDescString = rDataString.Mid( nStart , CIsoBMData::m_nFieldPos[2] - nStart);
			}
			rDescString.TrimLeft(); rDescString.TrimRight();
			if(!rDescString.IsEmpty())
			{
				int index = 0;
				vector<string> oResult;
				CTokenizer<CIsFromString>::Tokenize(oResult , rDescString.operator LPCTSTR() , CIsFromString( _T("-") ));
				for(vector<string>::iterator jtr = oResult.begin();jtr != oResult.end();++jtr,++index)
				{
					CString xxx = jtr->c_str();
					xxx.Replace( _T("_") , _T("") );
					xxx.TrimLeft(); xxx.TrimRight();
					CString rField;
					rField.Format( _T("%-10s") , xxx.operator LPCTSTR());
					rDesc += rField.operator LPCTSTR();
				}
			}
			rString.Format(FORMAT , 
				itr->m_rDrawingNo , itr->GetMainSize() , itr->matlCode() , rDesc , itr->QTY() , itr->plantNo() , itr->areaNo() , itr->unitNo() , itr->sectionNo());
			ofile << rString.operator LPCTSTR() << std::endl;
		}
		
		ofile.close();

		return true;
	}

	return false;
}

/**	
	@brief	LOG TEXT를 생성한다.
	@author	humkyung
**/
bool CBMReportForm::GenerateLogText(vector<CString>& LogTextEntry , const int &nItem)
{
	/*if(m_pQualifiedFileList)
	{
		LogTextEntry.clear();

		const int nItemCount = m_pQualifiedFileList->GetItemCount() - 1;
		
		CString rLog;
		rLog.Format( _T("[ %3d / %3d ] : %s") , nItem , nItemCount , m_pQualifiedFileList->GetItemText(nItem , 0));
		LogTextEntry.push_back(rLog);
		rLog.Format( _T("%15s%s") , _T("1) ") , _T("SCANNING DGN FILE"));
		LogTextEntry.push_back(rLog);
		
		long lCount = 1;
		vector<CIsoBMData>::iterator itr = m_qcCheck.m_pipeDatas.begin();
		for(itr = m_qcCheck.m_pipeDatas.begin();itr != m_qcCheck.m_pipeDatas.end();++itr)
		{
			rLog.Format(_T("            %d-%-3d) SCAN SHOP DATA  : %s") , nItem , lCount++ , itr->dataString());
			LogTextEntry.push_back(rLog);
			
			size_t nCount = itr->GetSubStringCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CString rSubString = itr->GetSubStringAt(i);
				if(!rSubString.IsEmpty())
				{
					rLog.Format(_T("            %d-%-3d) SCAN SHOP DATA  : %s") , nItem , lCount++ , itr->GetSubStringAt(i));
					LogTextEntry.push_back(rLog);
				}
			}
		}
		for(itr = m_qcCheck.m_generalDatas.begin();itr != m_qcCheck.m_generalDatas.end();++itr)
		{
			rLog.Format(_T("            %d-%-3d) SCAN SHOP DATA  : %s") , nItem , lCount++ , itr->dataString());
			LogTextEntry.push_back(rLog);
			
			size_t nCount = itr->GetSubStringCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CString rSubString = itr->GetSubStringAt(i);
				if(!rSubString.IsEmpty())
				{
					rLog.Format(_T("            %d-%-3d) SCAN SHOP DATA  : %s") , nItem , lCount++ , itr->GetSubStringAt(i));
					LogTextEntry.push_back(rLog);
				}
			}
		}
		
		for(vector<CCutPipeLengthData>::iterator jtr = m_qcCheck.m_cutPipeLengths.begin();jtr != m_qcCheck.m_cutPipeLengths.end();++jtr)
		{
			rLog.Format(_T("            %d-%-3d) SCAN PIPE LEN.  : %s") , nItem , lCount++ , jtr->dataString());
			LogTextEntry.push_back(rLog);
		}
		
		for(vector<CNtrData>::iterator ktr = m_qcCheck.m_ntrTextEntry.begin();ktr != m_qcCheck.m_ntrTextEntry.end();++ktr)
		{
			CString rNtrString = ktr->GetTextString();
			if(!rNtrString.IsEmpty())
			{
				rLog.Format(_T("            %d-%-3d) SCAN NTR FILE   : %s") , nItem , lCount++ , ktr->GetTextString());
				LogTextEntry.push_back(rLog);
			}
		}
		
		rLog.Format(_T("%15s%s") , _T("2) ") , _T("ERROR CHECKING"));
		LogTextEntry.push_back(rLog);
		rLog.Format(_T("%15s%s") , _T("3) ") , _T("MAKE ISODWG HEADER"));
		LogTextEntry.push_back(rLog);
		rLog.Format(_T("%15s%s") , _T("3) ") , _T("MAKE ISODWG INFO."));
		LogTextEntry.push_back(rLog);
		rLog.Format(_T("%15s%s") , _T("4) ") , _T("MAKE SUB WELDING FILE --> VIEW WELD DETAIL FILE"));
		LogTextEntry.push_back(rLog);

		return true;
	}*/

	return false;
}

/**	
	@brief	error summary file을 저장한다.
	@author	humkyung
**/
void CBMReportForm::SaveErrorSumFile()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	/// ERROR SUM 로그 파일을 저장한다.
	const BOOL checked = pWorkspaceBar->m_BMReportOptionDlg.GetSaveErrorSumFile( );
	if( checked )
	{
		CAppDocData*  pDocData  = CAppDocData::GetInstance();
		
		SYSTEMTIME st;
		GetLocalTime( &st );
		
		CString rLogFileName;
		rLogFileName.Format( _T("%d-%02d-%02d-%d-%d.ERR_SUM_BOM") , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
		CString rLogFilePath ( pDocData->GetTempFolder() + _T("\\") + rLogFileName );
		
		int nIndex = 1;
		OFSTREAM_T ofile(rLogFilePath);
		for(vector<CIsoEditError>::iterator itr = pDocData->m_IsoEditErrorEntry.begin();itr != pDocData->m_IsoEditErrorEntry.end();++itr , ++nIndex)
		{
			CString rLog;
			rLog.Format(_T("[ %3d / %3d ] : ") , nIndex , pDocData->m_IsoEditErrorEntry.size());
			rLog += itr->OutputErrorString();
			ofile << rLog.operator LPCSTR() << std::endl;
		}
		
		ofile << std::endl;
		ofile << _T("ERROR SUMMARY OF ") << pDocData->m_IsoEditErrorEntry.size() << _T(" DGN FILE(S)") << std::endl;
		ofile << _T("=============================================") << std::endl;
		int nTotalErrorCount = 0;
		for(int i = 0;i < CHECK_ERROR_COUNT ;++i)
		{
			CString rTemp;
			rTemp.Format(_T("%30s : %d") , CAppDocData::m_pCheckMessages[i] , CAppDocData::m_nIsoEditErrorCount[i]);
			ofile << rTemp.operator LPCSTR() << std::endl;
			nTotalErrorCount += CAppDocData::m_nIsoEditErrorCount[i];
		}
		ofile << _T("=============================================") << std::endl;
		CString rString;
		rString.Format(_T("%30s  : %d") , _T("TOTAL ERROR COUNT ") , nTotalErrorCount);
		ofile << rString.operator LPCSTR() << std::endl;
		
		ofile.close();
	}
}

/**
	@brief	delete selected line data when user press VK_DELETE.
	@author	humkyung
	@date	2010.03.31
*/
void CBMReportForm::OnKeydownListLineData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	
	if(VK_DELETE == pLVKeyDow->wVKey)
	{
		list<int> ItemEntry;
		POSITION pos = m_wndLineDataReport.GetFirstSelectedItemPosition();
		while(pos)
		{
			const int nItem = m_wndLineDataReport.GetNextSelectedItem(pos);
			ItemEntry.push_back(nItem);
		}

		if(!ItemEntry.empty() && (IDYES == AfxMessageBox(_T("Do you want to delete?") , MB_YESNO)))
		{
		}
	}
	
	*pResult = 0;
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-08
    @class      CBMReportForm
    @function   CreateIssueFolder
    @return     int
    @param      const           CString&
    @param      sProjectFolder  const
    @param      CString&        sIssueDate
    @brief
******************************************************************************/
int CBMReportForm::CreateIssueFolder(const CString& sProjectFolder , const CString& sIssueDate)
{
	int res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE") , NULL);
	if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE") , FILE_ATTRIBUTE_NORMAL);

		/// create latest folder - 2012.05.08 added by humkyung
		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\Latest") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\Latest") , FILE_ATTRIBUTE_NORMAL);
		/// up to here

		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate , FILE_ATTRIBUTE_NORMAL);

		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\SET") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\SET") , FILE_ATTRIBUTE_NORMAL);

		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\ISOOUT") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\ISOOUT") , FILE_ATTRIBUTE_NORMAL);

		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\DGN") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\DGN") , FILE_ATTRIBUTE_NORMAL);

		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\BOM") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\BOM") , FILE_ATTRIBUTE_NORMAL);
		//! 2011.01.21 - added hy HumKyung
		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\BOM\\temp") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\BOM\\temp") , FILE_ATTRIBUTE_NORMAL);
		//!

		//! 2011.01.26 - added hy HumKyung
		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\ERR") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\ERR") , FILE_ATTRIBUTE_NORMAL);
		//!

		//! 2011.01.31 - added hy HumKyung
		res = ::CreateDirectory(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\INDEX") , NULL);
		if(res) SetFileAttributes(sProjectFolder + _T("\\ISSUE\\") + sIssueDate + _T("\\INDEX") , FILE_ATTRIBUTE_NORMAL);
		//!

	return ERROR_SUCCESS;
}

/**
	@brief	save line data , revision data and BM data to issued table
	@author	humkyung
	@date	2013.02.07
*/
int SaveDataToIssuedTable( const CString& sKey , CAppPostgreSQL& database)
{
	CString rSql = _T("DELETE * FROM T_ISSUED_ISO_LINE_DATA WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	rSql = _T("INSERT INTO T_ISSUED_ISO_LINE_DATA SELECT * FROM T_ISO_LINE_DATA WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	rSql = _T("DELETE * FROM T_ISSUED_ISO_REVISION WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	rSql = _T("INSERT INTO T_ISSUED_ISO_REVISION SELECT * FROM T_ISO_REVISION WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	rSql = _T("DELETE * FROM T_ISSUED_ISO_BOM_DATA WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	rSql = _T("INSERT INTO T_ISSUED_ISO_BOM_DATA SELECT * FROM T_ISO_BOM_DATA WHERE KEY='") + sKey + _T("'");
	(*database.session()) << (rSql.operator LPCTSTR());

	return ERROR_SUCCESS;
}

/**
	@brief	copy all necessary file to issue folder
	@author	HumKyung
	@date	2010.03.31
	@param
	@return
*/
void CBMReportForm::OnButtonIssue() 
{
	if(IDYES != AfxMessageBox(_T("Do you want to issue?") , MB_YESNO | MB_ICONWARNING)) return;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CString rProjectPath = pDocData->GetProjectPath().c_str();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		const CString sIniFilePath= pDocData->GetPrjIniFilePath();

		try
		{
			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

			SYSTEMTIME st;
			GetLocalTime( &st );
			CString rDate;
			rDate.Format(_T("%d-%02d-%02d") , st.wYear , st.wMonth , st.wDay);

			this->CreateIssueFolder(rProjectPath , rDate);
			
			TCHAR szBuf[MAX_PATH + 1]={'\0',};
			CString	rApp(_T("Iso Edit Setting")) , rSourceIsoDwgFileFolder;
			CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
			if(GetPrivateProfileString(rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				rSourceIsoDwgFileFolder = szBuf;
			}

			CString rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\DGN");
			CString rIsogenFolder(rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\ISOOUT"));
			const int nItemCount = this->m_wndLineDataReport.GetCheckedItemCount();///m_pQualifiedFileList->GetItemCount();
			for(int i = 0;i < nItemCount;++i)
			{
				CString rFileName;/// = m_pQualifiedFileList->GetItemText( i , 0 );
				if(_T("..") == rFileName ) continue;
				rFileName.MakeUpper();		/// 대문자로 변경

				const int nItem = -1;///m_pQualifiedFileList->GetIndex(rFileName);
				if(-1 != nItem)
				{
					/// save line data and revision to issued table
					CLineDataRecord* pRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(nItem);
					CString rKey = pRecord->GetKey();

					SaveDataToIssuedTable(rKey , database);

					CString rInputFileName = pDocData->GetProject()->GetInputIsoFileName(pRecord);
					::CopyFile(rSourceIsoDwgFileFolder + _T("\\") + rInputFileName + CIsoEditSettingPage::m_rFileNameExt , rIsogenFolder + _T("\\") + rInputFileName + CIsoEditSettingPage::m_rFileNameExt, FALSE);
				}

				::CopyFile(m_rOutputDrawingFolder + _T("\\") + rFileName , rPath + _T("\\") + rFileName , FALSE);

				/// copy output iso file to ISSUE\Latest folder - 2012.05.08 added by humkyung
				::CopyFile(m_rOutputDrawingFolder + _T("\\") + rFileName , rProjectPath + _T("\\ISSUE\\Latest\\") + rFileName , FALSE);
				/// up to here
			}
			/// copy border file to issue\DGN folder - 2011.02.14 added by HumKyung
			if(GetPrivateProfileString(_T("Border Setting") , _T("Border File Path") , NULL, szBuf, MAX_PATH, sIniFilePath))
			{
				CSplitPath path(szBuf);
				::CopyFile(szBuf , rPath + _T("\\") + path.GetFileName() + path.GetExtension() , FALSE);
			}
			///

			/// delete files in DGN folder 2011.01.20 - added by HumKyung
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\DGN") , _T("*.*"));

			rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\SET");
			::CopyFile(rProjectPath + _T("\\DATABASE\\") + CString(pDocData->GetProject()->name()) + _T(".MDB") , rPath + _T("\\") + CString(pDocData->GetProject()->name()) + _T(".MDB") , FALSE);
			::CopyFile(rProjectPath + _T("\\SETTING\\") + CString(pDocData->GetProject()->name()) + _T(".INI") , rPath + _T("\\") + CString(pDocData->GetProject()->name()) + _T(".INI") , FALSE);

			/// copy .prt,.ntr and .dat files to BOM\\temp folder 2011.01.21 - added by HumKyung
			rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\BOM\\temp");
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\BOM") , rPath , _T("*.PRT"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\BOM") , _T("*.PRT"));
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\BOM") , rPath , _T("*.NTR"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\BOM") , _T("*.NTR"));
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\BOM") , rPath , _T("*.DAT"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\BOM") , _T("*.DAT"));

			/// copy product bom files to issue bom folder 2011.01.19 - added by HumKyung
			rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\BOM");
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\BOM") , rPath , _T("*.*"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\BOM") , _T("*.*"));

			/// copy error files to issue error folder 2011.01.26 - added by HumKyung
			rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\ERR");
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\ERR") , rPath , _T("*.*"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\ERR") , _T("*.*"));

			/// copy index files to issue index folder 2011.01.31 - added by HumKyung
			rPath = rProjectPath + _T("\\ISSUE\\") + rDate + _T("\\INDEX");
			CFileTools::CopyFiles(rProjectPath + _T("\\PRODUCT\\INDEX") , rPath , _T("*.XLS"));
			CFileTools::DeleteFiles(rProjectPath + _T("\\PRODUCT\\INDEX") , _T("*.XLS"));

			/// update PDS database - 2012.05.10 added by humkyung
			CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
			if((NULL != pDoc) && (TRUE == pDoc->m_bIssueAutomation))
			{
				vector<LineDataRecordMap> oLineDataRecordEntry;
				LineDataRecordMap oLineDataRecordMap;
				for(int i = 0;i < nItemCount;++i)
				{
					CString rFileName = this->m_wndLineDataReport.GetItemText( i , 0 );
					if(_T("..") == rFileName ) continue;
					rFileName.MakeUpper();		//! 대문자로 변경

					const int nItem = -1;///m_pQualifiedFileList->GetIndex(rFileName);
					if(-1 != nItem)
					{
						/// save line data and revision to issued table
						oLineDataRecordMap.pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(nItem);
					}
				}
				CUserAppView::GetInstance()->m_pInsertToPDS->StatusThread(oLineDataRecordEntry);
			}
			/// up to here

			AfxMessageBox(_T("Issue is complete") , MB_OK | MB_ICONINFORMATION);
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-07
    @function   PreTranslateMessage
    @return     BOOL
    @param      MSG*    pMsg
    @brief
******************************************************************************/
BOOL CBMReportForm::PreTranslateMessage(MSG* pMsg)
{
	UpdateDialogControls(this, TRUE);

	return CFormView::PreTranslateMessage(pMsg);
}

/**
	@brief	show help

	@author	humkyung
*/
BOOL CBMReportForm::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/BM Report.htm") , HH_DISPLAY_TOPIC , 0);

	return CFormView::OnHelpInfo(pHelpInfo);
}

///******************************************************************************
//    @author     humkyung
//    @date       2012-06-07
//    @class      CBMReportForm
//    @function   OnBegindragLineList
//    @return     void
//    @param      NMHDR*      pNMHDR
//    @param      LRESULT*    pResult
//    @brief		start dragging
//******************************************************************************/
//void CBMReportForm::OnBegindragLineList(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	//This routine sets the parameters for a Drag and Drop operation.
//	//It sets some variables to track the Drag/Drop as well
//	// as creating the drag image to be shown during the drag.
//
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	if(NULL == pNMListView) return;
//
//	//// Save the index of the item being dragged in m_nDragIndex
//	////  This will be used later for retrieving the info dragged
//	m_nDragIndex = pNMListView->iItem;
//
//	//// Create a drag image
//	POINT pt;
//	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
//	if((pNMListView->hdr.hwndFrom == m_wndLineDataReport.m_hWnd) && (m_wndLineDataReport.GetSelectedCount() > 0)) //! more than one item is selected
//	{
//		pt.x = nOffset;
//		pt.y = nOffset;
//
//		m_pDragImage = m_wndLineDataReport.CreateDragImage(m_nDragIndex, &pt);
//		ASSERT(m_pDragImage); //make sure it was created
//		//We will call delete later (in LButtonUp) to clean this up
//
//		CBitmap bitmap;
//		if(m_wndLineDataReport.GetSelectedCount() > 1) //more than 1 item in list is selected
//			//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
//			bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);
//		else
//			bitmap.LoadBitmap(IDB_BITMAP_BOX);
//		m_pDragImage->Replace(0, &bitmap, &bitmap);
//
//		//// Change the cursor to the drag image
//		////	(still must perform DragMove() in OnMouseMove() to show it moving)
//		m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
//		m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
//		
//		//// Set dragging flag and others
//		m_bDragging = TRUE;	//we are in a drag and drop operation
//		m_nDropIndex = -1;	//we don't have a drop index yet
//		m_pDragList = &m_wndLineDataReport; //make note of which list we are dragging from
//		m_pDropWnd =  &m_wndLineDataReport;	//at present the drag list is the drop list
//
//		//// Capture all mouse messages
//		SetCapture ();
//	}
//
//	*pResult = 0;
//}

///******************************************************************************
//    @author     humkyung
//    @date       2012-06-07
//    @class      CBMReportForm
//    @function   OnMouseMove
//    @return     void
//    @param      UINT    nFlags
//    @param      CPoint  point
//    @brief
//******************************************************************************/
//void CBMReportForm::OnMouseMove(UINT nFlags, CPoint point) 
//{
//	if (m_bDragging)
//	{
//		//// Move the drag image
//		CPoint pt(point);	//get our current mouse coordinates
//		ClientToScreen(&pt); //convert to screen coordinates
//		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
//		// Unlock window updates (this allows the dragging image to be shown smoothly)
//		m_pDragImage->DragShowNolock(false);
//		
//		//// Get the CWnd pointer of the window that is under the mouse cursor
//		CWnd* pDropWnd = WindowFromPoint (pt);
//		ASSERT(pDropWnd); //make sure we have a window
//
//		//// If we drag outside current window we need to adjust the highlights displayed
//		if (pDropWnd != m_pDropWnd)
//		{
//			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
//			{
//				TRACE("m_nDropIndex is -1\n");
//				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
//				//VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
//				// redraw item
//				//VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
//				//pList->UpdateWindow ();
//				m_nDropIndex = -1;
//			}
//			else //If we drag out of the CListCtrl altogether
//			{
//				TRACE("m_nDropIndex is not -1\n");
//				//CListCtrl* pList = (CListCtrl*)m_pDropWnd;
//				//int i = 0;
//				//int nCount = pList->GetItemCount();
//				//for(i = 0; i < nCount; i++)
//				//{
//				//	pList->SetItemState(i, 0, LVIS_DROPHILITED);
//				//}
//				//pList->RedrawItems(0, nCount);
//				//pList->UpdateWindow();
//			}
//		}
//
//		// Save current window pointer as the CListCtrl we are dropping onto
//		m_pDropWnd = pDropWnd;
//
//		// Convert from screen coordinates to drop target client coordinates
//		pDropWnd->ScreenToClient(&pt);
//
//		//If we are hovering over a CListCtrl we need to adjust the highlights
//		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))
//		{
//			TRACE("m_nDropIndex is not 100\n");
//			//Note that we can drop here
//			SetCursor(LoadCursor(NULL, IDC_ARROW));
//			UINT uFlags;
//			CListCtrl* pList = (CListCtrl*)pDropWnd;
//
//			// Turn off hilight for previous drop target
//			//pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
//			// Redraw previous item
//			//pList->RedrawItems (m_nDropIndex, m_nDropIndex);
//			
//			// Get the item that is below cursor
//			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
//			// Highlight it
//			//pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
//			// Redraw item
//			//pList->RedrawItems(m_nDropIndex, m_nDropIndex);
//			//pList->UpdateWindow();
//		}
//		else
//		{
//			//If we are not hovering over a CListCtrl, change the cursor
//			// to note that we cannot drop here
//			SetCursor(LoadCursor(NULL, IDC_NO));
//		}
//		// Lock window updates
//		m_pDragImage->DragShowNolock(true);
//	}
//	
//	CFormView::OnMouseMove(nFlags, point);
//}

/******************************************************************************
    @author     humkyung
    @date       2012-06-07
    @class      CBMReportForm
    @function   OnLButtonUp
    @return     void
    @param      UINT    nFlags
    @param      CPoint  point
    @brief
******************************************************************************/
//void CBMReportForm::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//	if (m_bDragging)
//	{
//		// Release mouse capture, so that other controls can get control/messages
//		ReleaseCapture ();
//		
//		// Note that we are NOT in a drag operation
//		m_bDragging = FALSE;
//		
//		// End dragging image
//		m_pDragImage->DragLeave (GetDesktopWindow ());
//		m_pDragImage->EndDrag ();
//		delete m_pDragImage; //must delete it because it was created at the beginning of the drag
//		
//		CPoint pt (point); //Get current mouse coordinates
//		ClientToScreen (&pt); //Convert to screen coordinates
//		// Get the CWnd pointer of the window that is under the mouse cursor
//		CWnd* pDropWnd = WindowFromPoint (pt);
//		ASSERT (pDropWnd); //make sure we have a window pointer
//		// If window is CListCtrl, we perform the drop
//		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))
//		{
//			m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on
//			DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
//		}
//	}
//	
//	CFormView::OnLButtonUp(nFlags, point);
//}

/******************************************************************************
    @author     humkyung
    @date       2012-06-07
    @class      CBMReportForm
    @function   DropItemOnList
    @return     void
    @param      CListCtrl   *pDragList
    @param      CListCtrl   *pDropList
    @brief		called when drop on target window
******************************************************************************/
//void CBMReportForm::DropItemOnList(CListCtrl *pDragList, CListCtrl *pDropList)
//{
//	/*if(pDropList->m_hWnd == m_pQualifiedFileList->m_hWnd)
//	{
//		OnButtonDown();
//	}*/
//}

/**
	@brief	open the dng file

	@author	humkyung

	@date	2013.05.11
*/
void CBMReportForm::OnNMDblclkListLineData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	TCHAR szBuf[MAX_PATH + 1]={'\0',};

	POSITION pos = m_wndLineDataReport.GetFirstSelectedItemPosition();
	int nItem = m_wndLineDataReport.GetNextSelectedItem(pos);
	if(0 != nItem )
	{
		const CString sItemText = m_wndLineDataReport.GetItemText(nItem , 1);
		const CString sIsoFilePath = IsTools::CPath::Combine(m_rOutputDrawingFolder , sItemText , 0);
		pDocData->OpenIsoFile(sIsoFilePath.operator LPCTSTR());
	}

	*pResult = 0;
}

/**
	@brief	show B/M Data of selected item
	@author	humkyung
	@date	2013.05.11
*/
void CBMReportForm::OnLvnItemchangedListLineData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ((pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))
	{
		const int nSelectItem = pNMLV->iItem;
		if (-1 == nSelectItem) return;
		
		CLineDataRecord* pRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(nSelectItem);
		CDataField* pDataField = pRecord->FindWithFieldName(_T("REV_NO"));
		m_pViewLogFileForm->DisplayBOMData(CString(pRecord->GetKey()),pDataField->value());
	}
	else if((pNMLV->uOldState ^ pNMLV->uNewState) & LVIS_STATEIMAGEMASK)
	{
		const CString sStatus = m_wndLineDataReport.GetItemText(pNMLV->iItem,3).MakeUpper();
		if(_T("NOT FOUND") == sStatus) m_wndLineDataReport.UnCheckItem(pNMLV->iItem);
	}

	*pResult = 0;
}