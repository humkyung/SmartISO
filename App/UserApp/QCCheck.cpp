// QCCheck.cpp: implementation of the CQCCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <functional>
#include <IsPoly2d.h>
#include <DgnDocument.h>
#include <DgnText.h>
#include <DgnLineString.h>
#include <IsRegExp.h>
#include "UserApp.h"
#include "UserAppDocData.h"
#include "QCCheck.h"
#include "SmartISOLog.h"
#include <Tokenizer.h>
#include <SplitPath.h>
#include <ProjectData.h>
#include <util/FileTools.h>

#include <set>
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IsRegExp::RxNode __declspec(dllimport) *IsRxCompile(LPCTSTR Regexp);
int __declspec(dllimport) IsRxExec(IsRegExp::RxNode *Regexp, LPCTSTR Data, int Len, LPCTSTR Start, IsRegExp::RxMatchRes *Match, unsigned int RxOpt);
int __declspec(dllimport) IsRxReplace(LPCTSTR rep, LPCTSTR Src, int len, IsRegExp::RxMatchRes match, LPTSTR *Dest, int *Dlen);
void __declspec(dllimport) IsRxFree(IsRegExp::RxNode *Node);

int CIsoBMData::m_nFieldPos[7];
bool CQCCheck::m_DRWNO, CQCCheck::m_ERRREAD, CQCCheck::m_FGBERR, CQCCheck::m_PIPECUTERR, CQCCheck::m_NOTFOUNERR,
	CQCCheck::m_FAILISOERR,CQCCheck::m_PCDSYMERR,CQCCheck::m_HOLDITEMERR,CQCCheck::m_SPECBREAKERR,
	CQCCheck::m_SYMCODEERR,CQCCheck::m_bCheckSupportBMErr, CQCCheck::m_bCheckTextModify;
/// 2013.06.12 added by humkyung
bool CQCCheck::m_BOMOVER;
/// up to here
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQCCheck::CQCCheck() : m_rxnode(NULL) , m_nModifiedText(0)
{
}

CQCCheck::~CQCCheck()
{
	try
	{
		for(vector<CBoxTextString*>::iterator itr = m_boxTextStringEntry.begin();itr != m_boxTextStringEntry.end();++itr)
		{
			delete (*itr);
		}
		m_boxTextStringEntry.clear();

		if (m_rxnode)
		{
			IsRxFree (m_rxnode);
			m_rxnode = NULL;
		}
	}
	catch(...)
	{
		
	}
}

namespace
{
	enum
	{
		FIND_MATCH_CASE	= 0x0001,
		FIND_WHOLE_WORD	= 0x0002,
		FIND_DIRECTION_UP	= 0x0010,
		REPLACE_SELECTION	= 0x0100
	};

	bool __SortByTextPosY(const CDgnText* lhs , const CDgnText* rhs)
	{
		return (lhs->origin().y > rhs->origin().y);
	}


	struct IsMatchString : public binary_function<std::tr1::shared_ptr<CBMLine> , CString, bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const std::tr1::shared_ptr<CBMLine> a, const CString& b ) const
		{
			CString str = a->textString();
			str.TrimLeft();
			str.TrimRight();
			return (str == b);
		}
	};

	struct IsMatchData : public binary_function<CIsoBMData , CString , bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CIsoBMData& lhs, const CString& rhs ) const
		{
			CString str = lhs.dataString();
			str.TrimLeft();
			str.TrimRight();
			return (str == rhs);
		}
	};

	struct IsInnerPt : public binary_function<CIsPoly2d , CIsPoint2d , bool>
	{
		bool operator()(const CIsPoly2d& lhs , const CIsPoint2d& rhs) const
		{
			return lhs.IsInnerPt(rhs);
		}
	};

	struct HasSameyCoord : public binary_function<std::tr1::shared_ptr<CBMLine> , DPoint3d , bool>
	{
		bool operator()(const std::tr1::shared_ptr<CBMLine> rhs , const DPoint3d& lhs) const
		{
			return (lhs.y == rhs->yCoord());
		}
	};
};

/**	
	@brief	Q/C check를 수행한다. Check 결과를 CAppDocData의 m_IsoEditErrorEntry에 등록한다.
	
	@author	humkyung

	@param	rCheckMsg	a parameter of type CString&
	@param	rFilePath	a parameter of type const CString&

	@return	bool	
*/
bool CQCCheck::Check(CString& rCheckMsg , const CString& rFilePath , const bool& bCheckIsoDwg)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	rCheckMsg.Empty();
	ExtractTextData(rFilePath);

	if(false == bCheckIsoDwg) return true;

	CSplitPath splitter;
	splitter.Split(rFilePath);
	CIsoEditError IsoEditError;
	IsoEditError.m_rFileName = splitter.GetFileName() + splitter.GetExtension();
///----------------------------------------------------------------------///
	bool error = false;
	//////////////////////////////////////////////////////////////////////////
	/// (1) Drawing No Check
	if( TRUE == m_DRWNO )
	{
		error = CheckDrawingNoError(rCheckMsg);
		if(error)
		{
			++(IsoEditError.m_nError[CAN_NOT_FIND_ISO_DWG_NAME]);
			++(CAppDocData::m_nIsoEditErrorCount[CAN_NOT_FIND_ISO_DWG_NAME]);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (2) Error Read
	if( TRUE == m_ERRREAD )		
	{
		int nError = CheckErrorReadError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[ERROR_READ]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[ERROR_READ]) += nError;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (3) Pipe Cutting Error
	if( TRUE == m_PIPECUTERR )
	{
		int nError = CheckPipeCuttingLengthError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[PIPE_CUTTING_LENGTH]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[PIPE_CUTTING_LENGTH]) += nError;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (4) Not Found Error
	if( TRUE == m_NOTFOUNERR )
	{
		int nError = CheckNotFoundError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[NOT_FOUND_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[NOT_FOUND_ERROR]) += nError;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (5) Fail Iso Error
	if( TRUE == m_FAILISOERR )
	{
		int nError = CheckIsoFailError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[FAIL_ISO_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[FAIL_ISO_ERROR]) += nError;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (6) Pcd Sysmbol Error
	if( TRUE == m_PCDSYMERR )
	{
		int nError = CheckPCDSymbolError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[PCD_SYSMBOL_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[PCD_SYSMBOL_ERROR]) += nError;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	/// (7) Hold Item Error
	if( TRUE == m_HOLDITEMERR )
	{
		int nError = CheckHoldItemError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[HOLD_ITEM_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[HOLD_ITEM_ERROR]) += nError;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/// (8) SymBol Code Not Found Error
	if( TRUE == m_SYMCODEERR )
	{
		int nError = CheckSymbolCodeNotFoundError(rCheckMsg);
		if( nError )
		{
			(IsoEditError.m_nError[SYMBOL_CODE_NOT_FOUND]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[SYMBOL_CODE_NOT_FOUND]) += nError;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/// (9) Support B/M Error 
	if( TRUE == m_bCheckSupportBMErr )
	{
		int nError = CheckSupportBMError(rCheckMsg);
		if( nError )
		{
			(IsoEditError.m_nError[SUPPORT_BM_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[SUPPORT_BM_ERROR]) += nError;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/// (10) Text Modified Error
	if( TRUE == m_bCheckTextModify )
	{
		int nError = CheckTextModifiedError(rCheckMsg);
		if( nError > 0 )
		{
			(IsoEditError.m_nError[TEXT_MODIFIED]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[TEXT_MODIFIED]) += nError;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/// (11) F G B Error
	if( TRUE == m_FGBERR )
	{
		int nError = CheckF_G_BError(rCheckMsg);
		if(nError > 0)
		{
			(IsoEditError.m_nError[F_G_B_ERROR]) += nError;
			(CAppDocData::m_nIsoEditErrorCount[F_G_B_ERROR]) += nError;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	/// (12) Spec Break Error
	if( TRUE == m_SPECBREAKERR )
	{
		bool bError = CheckSpeckBreakError(rCheckMsg);
		if(bError)
		{
			++(IsoEditError.m_nError[SPEC_BREAK_ERROR]);
			++(CAppDocData::m_nIsoEditErrorCount[SPEC_BREAK_ERROR]);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	/// (13) BOM OVER - 2013.06.12 added by humkyung
	if(m_BOMOVER)
	{
		if(CheckBOMOver(rCheckMsg))
		{
			++(IsoEditError.m_nError[BOM_OVER]);
			++(CAppDocData::m_nIsoEditErrorCount[BOM_OVER]);
		}
	}
	/// up to here

	if(m_bBOMNotFound)
	{
		rCheckMsg.Format(_T("%40s") , CAppDocData::m_pCheckMessages[1]);
		rCheckMsg += _T(" ===> _ERROR_\n");

		++(IsoEditError.m_nError[BM_NOT_FOUND]);
		++(CAppDocData::m_nIsoEditErrorCount[BM_NOT_FOUND]);
	}

	if(!m_generalDatas.empty() && !m_supportDatas.empty())
	{
		bool error = false;
	}
	
	/// IsoEditError 변수를 리스트에 추가한다.
	pDocData->m_IsoEditErrorEntry.push_back(IsoEditError);

	return false;
}

/**	
	@brief	extract dgn element.
		CIsoBMData를 생성한후 각 container에 저장한다.

	@author	humkyung

	@param	rFilePath	a parameter of type const CString &

	@return	void	
*/
void CQCCheck::ExtractTextData(const CString &rFilePath)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	
	InitInternalVariables();
	//////////////////////////////////////////////////////////////////////////
	/// check
	if(CFileTools::DoesFileExist(rFilePath))
	{		
		vector<CDgnText*> systemBMTextEntry;
		vector<CDgnText*> graphicAreaTextEntry;
		vector<CIsPoly2d> poly2dEntry;

		IsoDwgDataArea* pDrawNoArea = pDocData->FindIsoDwgDataAreaWithFieldName(_T("DRAW_NO"));	/// 2013.01.03 added by humkyung
		
		CDgnDocument* pDgnDoc = CDgnDocument::CreateInstance();
		pDgnDoc->Read(rFilePath);
		const size_t nCount = pDgnDoc->GetEntityCount();
		for(int i = 0;i < int(nCount);++i)
		{
			CDgnElement* pEnt = pDgnDoc->GetEntityAt(i);
			if(pEnt->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* pDgnText = static_cast<CDgnText*>(pEnt);
				DPoint3d origin = pDgnText->origin();
				
				CIsVolume vol;
				vol.Add(CIsPoint3d(origin.x , origin.y , 0.0));
				if(!vol.CollideWith(pDocData->m_borderSetting.borderArea.volume))
				{
					bool bCollideWithKeyData = false;
					for(list<IsoDwgDataArea>::iterator itr = pDocData->m_borderSetting.KeyDataAreaList.begin();itr != pDocData->m_borderSetting.KeyDataAreaList.end();++itr)
					{
						if(vol.CollideWith(itr->volume))
						{
							bCollideWithKeyData = true;
							break;
						}
					}

					if(false == bCollideWithKeyData)
					{
						CString str(pDgnText->textString());
						if(!str.IsEmpty())
						{
							/// check str is part of bom - 2013.06.12 added by humkyung
							if(CQCCheck::HasHeaderText(str))
							{
								m_bBOMOver = true;
							}
							/// up to here
							else
							{
								/// NTR Data 분석하는 부분.
								CNtrData NtrData(pDgnText->textString());
								if(NtrData.ParseText(pDgnText->textString()))
									m_ntrTextEntry.push_back(NtrData);
								else
								{
									CAppDocData::ERROR_STRING_ENTRY.push_back(_T("NTR Data가 아닙니다. = ") + CString(pDgnText->textString()) + _T(" in ") + rFilePath);
								}
							}
						}
					}
				}
				
				if(vol.CollideWith(pDocData->m_borderSetting.SystemBMArea.volume)) 
				{
					if(pDgnText->IsModified() && !pDocData->IsTextModifiedErrorException(pDgnText))
					{
						++m_nModifiedText;
						m_modifiedTextEntry.push_back(pDgnText->textString());
					}

					systemBMTextEntry.push_back(pDgnText);
				}
				else if(vol.CollideWith(pDocData->m_borderSetting.IsoGraphicArea.volume))
				{
					if(pDgnText->IsModified() && !pDocData->IsTextModifiedErrorException(pDgnText))
					{
						++m_nModifiedText;
						m_modifiedTextEntry.push_back(pDgnText->textString());
					}
					m_bBOMNotFound = false;

					m_graphicAreaDatas.push_back(CIsoBMData(CIsPoint2d(pDgnText->origin().x , pDgnText->origin().y ) , pDgnText->textString()));
				}
				else if(pDrawNoArea && vol.CollideWith(pDrawNoArea->volume))	/// 2013.01.03 modified by humkyung
				{
					if(pDgnText->IsModified() &&  !pDocData->IsTextModifiedErrorException(pDgnText))
					{
						++m_nModifiedText;
						m_modifiedTextEntry.push_back(pDgnText->textString());
					}
					
					m_drawingNoDatas.push_back(CString(pDgnText->textString()));
				}
			}
			else if(pEnt->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnVolume vol = pEnt->volume();
				if(vol.maxx() < pDocData->m_borderSetting.IsoGraphicArea.volume.maxx() && 
				   vol.minx() > pDocData->m_borderSetting.IsoGraphicArea.volume.minx() && 
				   vol.maxy() < pDocData->m_borderSetting.IsoGraphicArea.volume.maxy() && 
				   vol.miny() > pDocData->m_borderSetting.IsoGraphicArea.volume.miny())
				{
					CDgnLineString* p = static_cast<CDgnLineString*>(pEnt);
					if(p)
					{
						if(5 == p->GetVertexCount())
						{
							CIsPoly2d poly2d;

							const size_t nCount = p->GetVertexCount();
							for(int i = 0;i < int(nCount);++i)
							{
								const DPoint3d unnamed = (p->GetVertexAt(i));
								poly2d.AddVertex(CIsPoint2d(unnamed.x , unnamed.y));
							}
							poly2dEntry.push_back(poly2d);
						}
					}
				}
			}
		}

		ParseBMTexts(systemBMTextEntry);
		/// 2011.06.21 added by humkyung
		for(vector<CDgnText*>::iterator itr = systemBMTextEntry.begin();itr != systemBMTextEntry.end();++itr)
		{
			m_prtTextEntry.push_back((*itr)->textString());
		}
		/// up to here
		
		//////////////////////////////////////////////////////////////////////////
		/// find text inside line string
		map<CIsPoly2d* , CBoxTextString*> unnamed;
		for(vector<CIsoBMData>::iterator jtr = m_graphicAreaDatas.begin();jtr != m_graphicAreaDatas.end();++jtr)
		{
			CIsoBMData* data = &(*jtr);
			vector<CIsPoly2d>::iterator where = find_if(poly2dEntry.begin() , poly2dEntry.end() , bind2nd(IsInnerPt() , jtr->origin()));
			if(where != poly2dEntry.end())
			{
				CIsPoly2d* p = &(*where);
				if(unnamed.end() != unnamed.find(p))
				{
					unnamed[p]->m_textStringEntry.push_back(jtr->dataString());
				}
				else
				{
					unnamed[p] = new CBoxTextString(jtr->origin());
					unnamed[p]->m_textStringEntry.push_back(jtr->dataString());
					m_boxTextStringEntry.push_back(unnamed[p]);
				}
			}
		}

		/// get pipe cut length no - 2012.04.25 added by humkyung
		for(vector<CBoxTextString*>::iterator ktr = m_boxTextStringEntry.begin();ktr != m_boxTextStringEntry.end();++ktr)
		{
			if(1 == (*ktr)->m_textStringEntry.size())
			{
				int nLastFindWhatLen = 0;
				int nPos = ::FindStringHelper((*ktr)->m_textStringEntry[0] , "[0-9]+" , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
				if(-1 != nPos)
				{
					double dMinDist = DBL_MAX;
					int iPipeCutLengthNo = -1;

					const int iPipeNo = atoi((*ktr)->m_textStringEntry[0]);
					for(vector<CIsoBMData>::iterator jtr = m_graphicAreaDatas.begin();jtr != m_graphicAreaDatas.end();++jtr)
					{
						if(("<" == jtr->dataString().Left(1)) && (">" == jtr->dataString().Right(1)))
						{
							const double d = jtr->origin().DistanceTo((*ktr)->origin());
							if(d < dMinDist)
							{
								dMinDist = d;
								iPipeCutLengthNo = atoi(jtr->dataString().Mid(1,jtr->dataString().GetLength() - 2));
							}
						}
					}

					if(-1 != iPipeCutLengthNo)
					{
						(*ktr)->pipeNo() = iPipeNo;
						(*ktr)->pipeCutLengthNo() = iPipeCutLengthNo;
					}
				}
			}
		}
		/// up to here

		CDgnDocument::DeleteInstance(pDgnDoc);
	}
}

/**	
	@brief	내부 변수 초기화.

	@author	humkyung

	@return	void
*/
void CQCCheck::InitInternalVariables()
{
	m_bBOMNotFound = true;
	/// 2013.06.12 added by humkyung
	m_bBOMOver = false;
	/// up to here

	m_nModifiedText = 0;
	m_modifiedTextEntry.clear();

	m_ntrTextEntry.clear();
	m_prtTextEntry.clear();

	m_boxTextStringEntry.clear();
	m_drawingNoDatas.clear();
	m_graphicAreaDatas.clear();
	
	m_pipeDatas.clear();
	m_generalDatas.clear();
	m_supportDatas.clear();
	m_InstOrSpecialDatas.clear();

	m_cutPipeLengths.clear();
}

/**
	@brief	PIPE CUT ERROR 여부를 검사한다.
	
	@author	humkyung

	@param	rResMsg	a parameter of type CString &

	@return	PIPE CUT ERROR의 발생 갯수 
*/
int CQCCheck::CheckPipeCuttingLengthError(CString &rResMsg)
{
	int nError = 0;

	CString rErrorMsg;
	for(vector<CIsoBMData>::iterator itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		CString rNPD = (*itr).GetMainSize();
		double nCutLength = 0.f;
		for(vector<CCutPipeLengthData>::iterator jtr = m_cutPipeLengths.begin();jtr != m_cutPipeLengths.end();++jtr)
		{
			if(rNPD == (*jtr).NPD())
			{
				nCutLength += atof((*jtr).length());
			}
		}
		
		int nLen = 0;
		CString strRegEx (_T("\\d*(\\.?\\d*)")) , rQTY((*itr).QTY());
		int nPos = ::FindStringHelper(rQTY , strRegEx , 0 , nLen , m_rxnode , &m_rxmatch);
		if(-1 != nPos)
		{
			rQTY = rQTY.Left(nLen);
			nCutLength = ceil(nCutLength * 0.01) * 0.1;
			double nQTY= atof(rQTY);
			if(fabs(nQTY - nCutLength) > 0.001)
			{
				++nError;

				CString rMsg;
				rMsg.Format( _T("%lf") , nCutLength);
				rErrorMsg += _T("              ### ") + rNPD + _T("(NPD) : Pipe Length Error(") + rQTY + _T(" : ") + rMsg + _T(")\n");
			}
		}
	}

	if(!rErrorMsg.IsEmpty())
	{
		CString rErrorString;
		rErrorString.Format( _T("%40s(%d)") , CAppDocData::m_pCheckMessages[PIPE_CUTTING_LENGTH] , nError);
		rResMsg += rErrorString;
		rResMsg += _T(" ===> _ERROR_\n");
		rResMsg += rErrorMsg;
		
		return nError;
	}

	return nError;

}

/**
	@brief	텍스트의 문자열이 NOT FOUND 인지를 검사한다.
	
	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	텍스트의 문자열이 NOT FOUND 인 갯수 
*/
int CQCCheck::CheckNotFoundError(CString &rResMsg)
{
	int nError = 0;

	CString rErrorString;
	for(vector<CIsoBMData>::iterator itr = m_graphicAreaDatas.begin();itr != m_graphicAreaDatas.end();++itr)
	{
		if(CString("NOT FOUND") == (*itr).dataString())
		{
			++nError;

			rErrorString.Format("%40s(%d)" , CAppDocData::m_pCheckMessages[NOT_FOUND_ERROR] , nError);
			rResMsg += rErrorString;
			rResMsg +=  " ===> _ERROR_\n";
		}
	}

	return nError;
}

/**	@brief	The CQCCheck::CheckIsoFailError function

	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	bool	  
*/
int CQCCheck::CheckIsoFailError(CString &rResMsg)
{
	int nError = 0;

	CString rErrorString;
	for(vector<CIsoBMData>::iterator itr = m_graphicAreaDatas.begin();itr != m_graphicAreaDatas.end();++itr)
	{
		if(CString("FAIL") == (*itr).dataString())
		{
			++nError;

			rErrorString.Format("%40s" , CAppDocData::m_pCheckMessages[FAIL_ISO_ERROR]);
			rResMsg += rErrorString;
			rResMsg += " ===> _ERROR_\n";
		}
	}

	return nError;
}

/**
	@brief	TEXT중 HOLE라는 글자를 포함하는지를 검사한다.
	
	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	HOLE 텍스를 포함하는 텍스트의 발생 갯수 
*/
int CQCCheck::CheckHoldItemError(CString &rResMsg)
{
	int nError = 0;
	
	for(vector<CIsoBMData>::iterator itr = m_graphicAreaDatas.begin();itr != m_graphicAreaDatas.end();++itr)
	{
		if(CString("HOLD") == (*itr).dataString())
		{
			++nError;
		}
	}
	
	if(nError > 0)
	{
		CString rErrorString;
		rErrorString.Format("%40s(%d)" , CAppDocData::m_pCheckMessages[HOLD_ITEM_ERROR] , nError);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
	}

	return nError;
}

/**
	@brief	TEXT가 수정되었는지를 검사한다.

	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	수정된 텍스트의 발생 갯수 
*/
int CQCCheck::CheckTextModifiedError(CString &rResMsg)
{
	if(m_nModifiedText > 0)
	{
		CString rErrorString;
		rErrorString.Format("%40s(%d)" , CAppDocData::m_pCheckMessages[TEXT_MODIFIED] , m_nModifiedText);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
		for(vector<CString>::iterator itr = m_modifiedTextEntry.begin();itr != m_modifiedTextEntry.end();++itr)
		{
			rResMsg += "              ### <" + (*itr) + " > is modified.\n";
		}
		
		return m_nModifiedText;
	}
	
	return m_nModifiedText;
}

/**	\brief	Drawing Number 유무를 검사한다.

	\param	rResMsg	a parameter of type CString &

	\return	bool	
*/
bool CQCCheck::CheckDrawingNoError(CString &rResMsg)
{
	if(m_drawingNoDatas.empty())
	{
		CString rErrorString;
		rErrorString.Format("%40s(1)" , CAppDocData::m_pCheckMessages[CAN_NOT_FIND_ISO_DWG_NAME]);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
		return true;
	}

	return false;
}

/**
	@brief	TEXT중 ERROR READING라는 글자를 포함하는지를 검사한다.

	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	ERROR READING를 포함하는 텍스트의 발생 갯수 
*/
int CQCCheck::CheckErrorReadError(CString &rResMsg)
{
	static const CString __ERROR_READING__("ERROR READING");
	int nError = 0;

	vector<CIsoBMData>::iterator itr = m_pipeDatas.begin();
	for(itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		CString rString((*itr).dataString());
		int nPos = rString.Find(__ERROR_READING__);
		if(-1 != nPos) ++nError;
	}

	for(itr = m_generalDatas.begin();itr != m_generalDatas.end();++itr)
	{
		CString rString((*itr).dataString());
		int nPos = rString.Find(__ERROR_READING__);
		if(-1 != nPos) ++nError;
	}

	for(itr = m_supportDatas.begin();itr != m_supportDatas.end();++itr)
	{
		CString rString((*itr).dataString());
		int nPos = rString.Find(__ERROR_READING__);
		if(-1 != nPos) ++nError;
	}

	if( nError > 0 )
	{
		CString rErrorString;
		rErrorString.Format("%40s(%d)" , CAppDocData::m_pCheckMessages[ERROR_READ] , nError);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
	}

	return nError;
}


/**	@brief	SPEC BREAK가 일어나는지 검사한다.(SUPPORT의 SPEC은 포함하지 않는다.)

	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	true when 1 above material spec kinds exist
*/

bool CQCCheck::CheckSpeckBreakError(CString &rResMsg)
{
	bool bError = false;
	set<CString> matlSpecSet;
	vector<CIsoBMData>::iterator itr = m_pipeDatas.begin();
	for(itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		matlSpecSet.insert((*itr).matlSpec());
	}

	for(itr = m_generalDatas.begin();itr != m_generalDatas.end();++itr)
	{
		matlSpecSet.insert((*itr).matlSpec());
	}

	/*
	for(itr = m_supportDatas.begin();itr != m_supportDatas.end();++itr)
	{
		matlSpecSet.insert((*itr).matlSpec());
	}
	*/

	if(matlSpecSet.size() > 1)
	{
		CString rErrorString;
		rErrorString.Format("%40s(1)" , CAppDocData::m_pCheckMessages[SPEC_BREAK_ERROR]);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
		return true;
	}

	return false;
}

/**
	@brief	SYMBOL CODE가 비어 있는지를  검사한다. SUPPORT는 SYMBOL CODE 검사를 하지 않는다.

	@author	백흠경

	@param	rResMsg	a parameter of type CString &

	@return	SYMBOL CODE 텍스트가 비어 있는 텍스트의 갯수 
*/
int CQCCheck::CheckSymbolCodeNotFoundError(CString &rResMsg)
{
	int  nError = 0;

	vector<CIsoBMData>::iterator itr = m_pipeDatas.begin();
	for(itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		if((*itr).bmcsSYM().IsEmpty()) ++nError;
	}

	for(itr = m_generalDatas.begin();itr != m_generalDatas.end();++itr)
	{
		if((*itr).bmcsSYM().IsEmpty()) ++nError;
	}

	/*
	for(itr = m_supportDatas.begin();itr != m_supportDatas.end();++itr)
	{
		if((*itr).bmcsSYM().IsEmpty()) ++nError;
	}
	*/

	if( nError > 0 )
	{
		CString rErrorString;
		rErrorString.Format("%40s(%d)" , CAppDocData::m_pCheckMessages[SYMBOL_CODE_NOT_FOUND] , nError);
		rResMsg += rErrorString;
		rResMsg += " ===> _ERROR_\n";
	}

	return nError;
}

/**	
	@brief	check bom is over the area

	@author	humkyung
	
	@param	rResMsg	a parameter of type CString &

	@return	bool
*/
bool CQCCheck::CheckBOMOver(CString &rResMsg) const
{
	return (true == m_bBOMOver);
}

/**	@brief	B/M란의 support 데이터가 graphic 란에 없으면 error.

	@author	humkyung
	
	@param	rResMsg	a parameter of type CString &

	@return	bool
*/
int CQCCheck::CheckSupportBMError(CString &rResMsg)
{
	int nError = 0;

	CString rErrorMsg;
	for(vector<CIsoBMData>::iterator itr = m_supportDatas.begin();itr != m_supportDatas.end();++itr)
	{
		CString rMatlCode = itr->matlCode();
		rMatlCode.TrimRight();
		
		vector<CIsoBMData>::iterator where = find_if(m_graphicAreaDatas.begin() , m_graphicAreaDatas.end() , bind2nd(IsMatchData() , rMatlCode));
		if(m_graphicAreaDatas.end() == where)
		{
			++nError;

			CString rString;
			rString.Format(_T("%s%8s : Support Not Found.\n") , _T("              ### ") , rMatlCode);
			rErrorMsg += rString;
		}
	}

	if(!rErrorMsg.IsEmpty())
	{
		CString rErrorString;
		rErrorString.Format( _T("%40s(%d)") , CAppDocData::m_pCheckMessages[SUPPORT_BM_ERROR] , nError);
		rResMsg += rErrorString;
		rResMsg += _T(" ===> _ERROR_\n");
		rResMsg += rErrorMsg;
	}

	return nError;
}

/**	
	@brief	F , G , B가 모두 들어있는지 검사.
	
	@author	humkyung

	@param	rResMsg	a parameter of type CString &

	@return	number of F-G-B Error
*/
int CQCCheck::CheckF_G_BError(CString &rResMsg)
{
	int nError = 0;
	//////////////////////////////////////////////////////////////////////////
	int nLastFindWhatLen = 0;
	for(vector<CBoxTextString*>::iterator itr = m_boxTextStringEntry.begin();itr != m_boxTextStringEntry.end();++itr)
	{
		if(1 == (*itr)->m_textStringEntry.size())
		{
			int nPos = ::FindStringHelper((*itr)->m_textStringEntry[0] ,  _T("F[0-9]+")  , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
			if(0 == nPos)	/// check if F{number} start at first column - 2012.06.07 added by humkyung
			{
				nPos = ::FindStringHelper((*itr)->m_textStringEntry[0] ,  _T("G[0-9]+")  , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
				if(-1 == nPos)
				{
					++nError;
					continue;
				}
				nPos = ::FindStringHelper((*itr)->m_textStringEntry[0] ,  _T("B[0-9]+") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
				if(-1 == nPos)
				{
					++nError;
					continue;
				}
			}
		}
	}

	if( nError > 0)
	{
		CString rErrorString;
		rErrorString.Format( _T("%40s(%d)") , CAppDocData::m_pCheckMessages[F_G_B_ERROR] , nError);
		rResMsg += rErrorString;
		rResMsg += _T(" ===> _ERROR_\n");
	}

	return nError;
}

/**	
	@brief	The CQCCheck::CheckPCDSymbolError function
	@author	humkyung
	@param	rResMsg	a parameter of type CString&
	@return	bool	
*/
int CQCCheck::CheckPCDSymbolError(CString& rResMsg)
{
	int nError = 0;
	CAppDocData* pDocData = CAppDocData::GetInstance();

	CString rErrorMsg;
	for(vector<CIsoBMData>::const_iterator itr =  m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		try
		{
			STRING_T rmatalSpec = itr->matlSpec();
			STRING_T rmatelCode = itr->matlCode();
			CPCDFile& pcd_file = pDocData->GetPCDFile(rmatalSpec.c_str());
			if(!pcd_file.FindMatlCode(rmatelCode.c_str()))
			{
				++nError;
				rErrorMsg += _T("              ### ") + itr->matlCode() + _T(" not found.\n");
			}
		}
		catch(const exception& ex)
		{
			WRITE_LOG(ex.what());
		}
	}

	for(vector<CIsoBMData>::const_iterator itr =  m_generalDatas.begin();itr != m_generalDatas.end();++itr)
	{
		try
		{
			STRING_T rmatalSpec = itr->matlSpec();
			STRING_T rmatelCode = itr->matlCode();
			CPCDFile& pcd_file = pDocData->GetPCDFile(rmatalSpec.c_str());
			if(!pcd_file.FindMatlCode(rmatelCode.c_str()))
			{
				++nError;
				rErrorMsg += _T("              ### ") + itr->matlCode() + _T(" not found.\n");
			}
		}
		catch(const exception& ex)
		{
			WRITE_LOG(ex.what());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if( nError > 0 )
	{
		CString rErrorString;
		rErrorString.Format(_T("%40s(%d)") , CAppDocData::m_pCheckMessages[PCD_SYSMBOL_ERROR] , nError);
		rResMsg += rErrorString;
		rResMsg += _T(" ===> _ERROR_\n");
		rResMsg += rErrorMsg;
	}
	
	return nError;
}

/**
	@brief	check whether it is speical or instrument item

	@author	humkyung

	@date	????.??.??
*/
bool CQCCheck::IsInstOrSpecialItem( const CString& rTextString )
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	
	map<CString,CString>::iterator where = pDocData->m_oDefineInstOrSpecialItem.find(_T("FullText"));
	if(where != pDocData->m_oDefineInstOrSpecialItem.end())
	{
		CString sRegExp = where->second;
		int nLastFindWhatLen = 0;
		int nPos = ::FindStringHelper(rTextString , sRegExp , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
		if(nPos > 0)
		{
			return true;
		}
	}
	
	return false;
}

/**
	@brief	check whether it is speical or instrument item

	@author	humkyung

	@date	2011.06.22
*/
bool CQCCheck::IsInstOrSpecialItem( const CIsoBMData& bmData )
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	for(map<CString,CString>::iterator itr = pDocData->m_oDefineInstOrSpecialItem.begin();itr != pDocData->m_oDefineInstOrSpecialItem.end();++itr)
	{
		if(0 == itr->first.CompareNoCase(_T("MatlCode")))
		{
			int nLastFindWhatLen = 0;
			int nPos = ::FindStringHelper(bmData.matlCode() , itr->second , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
			if(nPos > 0)
			{
				return true;
			}
		}
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-31
    @class      CQCCheck
    @function   IsSkipLine
    @return     bool
    @param      const   CString&
    @param      sLine
    @brief
******************************************************************************/
bool CQCCheck::IsSkipLine(const CString& sLine) const
{
	int at = sLine.Find( _T("PT") );
	if(0 == at) return true;
	at = sLine.Find( _T("MATERIALS DESCRIPTION") );
	if(-1 != at) return true;
	at = sLine.Find( _T("___") );
	if(0 == at) return true;
	at = sLine.Find( _T("PIPE SUPPORTS") );
	if(-1 != at) return true;
	at = sLine.Find( _T("CUT PIPE LENGTH") );
	if(-1 != at) return true;
	
	return false;
}

/**	
	@brief	create bm line list with dgn text
	
	@author	humkyung
	
	@param	oBMLineList			store bm line
	@param	systemBMTextEntry

	@return	int
*/
int CQCCheck::SetupBMLine(vector<std::tr1::shared_ptr<CBMLine>>& oBMLineList , vector<CDgnText*>& systemBMTextEntry)
{
	stable_sort(systemBMTextEntry.begin() , systemBMTextEntry.end() , __SortByTextPosY);

	for(vector<CDgnText*>::iterator itr = systemBMTextEntry.begin();itr != systemBMTextEntry.end();++itr)
	{
		DPoint3d origin = (*itr)->origin();

		CBMLine* pNewBMLine = NULL;
		vector<std::tr1::shared_ptr<CBMLine>>::iterator where = find_if(oBMLineList.begin() , oBMLineList.end() , bind2nd(HasSameyCoord() , origin));
		if(where == oBMLineList.end())
		{
			std::tr1::shared_ptr<CBMLine> oNewBMLine(new CBMLine((*itr)));
			oBMLineList.push_back( oNewBMLine );
		}
		else
		{
			pNewBMLine = (*where).get();
			pNewBMLine->Add(*itr);
		}
	}

	return ERROR_SUCCESS;
}

static bool HasPipeSupportPrefix(const CString& str)
{
	const CString aryPrefix[]={_T("SUPPORT DATA:"),_T("PLAN:")};
	for(int i = 0;i < SIZE_OF_ARRAY(aryPrefix);++i)
	{
		if(-1 != str.Find(aryPrefix[i])) return true;
	}

	return false;
}

/**
	@brief	check if str is header text

	@author	humkyung
*/
bool CQCCheck::HasHeaderText( const CString& str )
{
	const CString aryPrefix[]={_T("MATERIALS DESCRIPTION") , _T("COMPONENT DESCRIPTION") , _T("MATERIAL DESCRIPTION")};
	for(int i = 0;i < SIZE_OF_ARRAY(aryPrefix);++i)
	{
		if(-1 != str.Find(aryPrefix[i])) return true;
	}

	return false;
}

/**	
	@brief	BM Text에 대한 CIsoBMData를 생성한다.
	
	@author	humkyugn
	
	@param	systemBMTextEntry	a parameter of type vector<CDgnText*>&

	@return	void
*/
void CQCCheck::ParseBMTexts(vector<CDgnText*>& systemBMTextEntry)
{
	/// setup bm line list - 2012.03.14 added by humkyung
	vector<std::tr1::shared_ptr<CBMLine>> oBMLineList;
	this->SetupBMLine(oBMLineList , systemBMTextEntry);
	/// up to here

	vector<std::tr1::shared_ptr<CBMLine>>::iterator wherePipeSupport = find_if(oBMLineList.begin() , oBMLineList.end() , bind2nd(IsMatchString() , _T("PIPE SUPPORTS")));
	vector<std::tr1::shared_ptr<CBMLine>>::iterator whereMISCELL = find_if(oBMLineList.begin(), oBMLineList.end() , bind2nd(IsMatchString(), _T("MISCELLANEOUS COMPONENTS"))); ///< MISCELLANEOUS COMPONENTS의 where를 찾는다.

	for(vector<std::tr1::shared_ptr<CBMLine>>::iterator itr = oBMLineList.begin();itr != oBMLineList.end();++itr)
	{
		CString rTextString = (*itr)->textString();
		if(rTextString.IsEmpty()) continue;

		/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
		/// find field position
		if(HasHeaderText(rTextString))
		{
			ParseColumnIndex(rTextString);
			continue;
		}
		if(this->IsSkipLine(rTextString)) continue;
		
		rTextString = rTextString.Left(5);
		rTextString.TrimLeft();
		//////////////////////////////////////////////////////////////////////////
		/// regular expression
		try
		{
			int nLastFindWhatLen = 0;
			int nPos = ::FindStringHelper(rTextString , _T("[0-9]+") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
			if(0 == nPos)
			{
				bool bInstOrSpecialItem = false;
				/// PIPE 부분 - PIPE SUPPORTS 부분이 둘다 있을 경우
				if(oBMLineList.end() != wherePipeSupport)
				{
					if(itr < wherePipeSupport)
					{
						/// 인스트루먼트 파이핑 스페셜리을 체크한다.
						rTextString = (*itr)->textString();
						rTextString.TrimRight();
						rTextString.TrimLeft();
						if(IsInstOrSpecialItem(rTextString)) bInstOrSpecialItem = true;

						CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
						for(++itr;itr != oBMLineList.end();++itr)
						{
							/// 어디까지가 하나의 ITEM DATA되는지 확인
							rTextString = (*itr)->textString();
							if(rTextString.IsEmpty()) continue;
							rTextString.TrimLeft();rTextString.TrimRight();
							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}
							nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
							if(-1 != nPos)
							{
								--itr;
								break;
							}

							if(this->IsSkipLine(rTextString))
							{
								--itr;
								break;
							}

							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							bmData.AddSubText(rTextString);
						}

						if(true == bmData.Parse())
						{
							const int nFindPos = bmData.m_rMatlDesc.Find( _T("PIPE") );
							if(-1 != nFindPos)
								m_pipeDatas.push_back(bmData);
							else if((true == bInstOrSpecialItem) || IsInstOrSpecialItem(bmData))	//! 2011.06.22 modified by humkyung
								m_InstOrSpecialDatas.push_back(bmData);
							else	m_generalDatas.push_back(bmData);
						}
					}
					else if( (wherePipeSupport < itr) && (itr < whereMISCELL) ) /// 서포트 부분
					{
						rTextString = (*itr)->textString();
						rTextString.TrimLeft();
						rTextString.TrimRight();
						bool has = HasPipeSupportPrefix(rTextString);
						if(false == has) continue;
						/// 
						CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
						for(++itr;itr != oBMLineList.end();++itr)
						{
							rTextString = (*itr)->textString();
							if(rTextString.IsEmpty()) continue;
							rTextString.TrimLeft(); rTextString.TrimRight();						

							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ SUPPORT DATA:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}
							nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
							if(-1 != nPos)
							{
								--itr;
								break;
							}

							if(this->IsSkipLine(rTextString))
							{
								--itr;
								break;
							}

							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							bmData.AddSubText(rTextString);
						}
						if(true == bmData.Parse())
						{
							m_supportDatas.push_back(bmData);
						}
					}
					else	/// MISCELLANEOUS 부분
					{
						rTextString = (*itr)->textString();
						rTextString.TrimLeft();
						rTextString.TrimRight();
						/// 
						CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
						for(++itr;itr != oBMLineList.end();++itr)
						{
							rTextString = (*itr)->textString();
							if(rTextString.IsEmpty()) continue;
							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}
							nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
							if(-1 != nPos)
							{
								--itr;
								break;
							}

							rTextString.TrimLeft(); rTextString.TrimRight();
							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ SUPPORT DATA:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}
							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ PLAN:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}

							if(this->IsSkipLine(rTextString))
							{
								--itr;
								break;
							}

							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							bmData.AddSubText(rTextString);
						}
						if(true == bmData.Parse())
						{
							if(IsInstOrSpecialItem(bmData))	/// 2011.06.22 added by humkyung
								m_InstOrSpecialDatas.push_back(bmData);
							else	m_miscellDatas.push_back(bmData);
						}
					}
				}
				else
				{
					/// 인스트루먼트 파이핑 스페셜리을 체크한다.
					rTextString = (*itr)->textString();
					rTextString.TrimRight();
					rTextString.TrimLeft();
					if(IsInstOrSpecialItem(rTextString)) bInstOrSpecialItem = true;
					/// PIPE SUPPORTS 부분이 없을 경우.
					
					CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
					for(++itr;itr != oBMLineList.end();++itr)
					{
						rTextString = (*itr)->textString();
						if(rTextString.IsEmpty()) continue;
						rTextString = rTextString.Trim();
						nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
						if(0 == nPos)
						{
							--itr;
							break;
						}
						nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
						if(-1 != nPos)
						{
							--itr;
							break;
						}

						if(this->IsSkipLine(rTextString))
						{
							--itr;
							break;
						}

						rTextString = (*itr)->textString();
						rTextString.TrimLeft();
						rTextString.TrimRight();
						bmData.AddSubText(rTextString);
					}
					//if(!ExtractPipeCompTextData(*itr)) continue;
					if(true == bmData.Parse())
					{
						/// 'TUBE' is added - 2012.04.20 added by humkyung
						const bool isPipe = (-1 != bmData.m_rMatlDesc.Find( _T("PIPE") )) || (-1 != bmData.m_rMatlDesc.Find( _T("TUBE") ));
						/// up to here
						if(true == isPipe)
							m_pipeDatas.push_back(bmData);
						else if((true == bInstOrSpecialItem) || IsInstOrSpecialItem(bmData))	//! 2011.06.22 modified by humkyung
							m_InstOrSpecialDatas.push_back(bmData);
						else	m_generalDatas.push_back(bmData);
					}
				}
			}
			else
			{
				/// PIPE CUT LENGTH 부분
				CString line((*itr)->textString());
				CString what( _T("\\<[0-9]+\\>") );
				int nFoundPos = -1;
				int nMatchLen = what.GetLength();
				int nLineLen = line.GetLength();
				int nPos = -1 , nFoundCount = 0;
				do
				{
					nPos = ::FindStringHelper(line , what , 0 , nLastFindWhatLen , m_rxnode, &m_rxmatch);
					if( nPos >= 0 )
					{
						nFoundPos = (nFoundPos == -1)? nPos : nFoundPos + nPos;
						nFoundPos+= nMatchLen;
						line = line.Right( nLineLen - (nMatchLen + nPos) );
						nLineLen = line.GetLength();
						++nFoundCount;
					}
				}
				while( nPos >= 0 );
				
				line = (*itr)->textString();
				if( 2 == nFoundCount)
				{
					m_cutPipeLengths.push_back(CCutPipeLengthData(line.Left(42)));
					m_cutPipeLengths.push_back(CCutPipeLengthData(line.Right(line.GetLength() - 42)));
				}
				else if(1 == nFoundCount)
				{
					m_cutPipeLengths.push_back(CCutPipeLengthData(line));
				}
			}
		}
		catch(...)
		{
			
		}
		if(oBMLineList.end() == itr) break;	/// ENTRY의 맨 마지막이면 종료한다.
	}
}

map<STRING_T , pair<int,int>> CIsoBMData::m_oBMAttrPosMap;	/// 2011.08.28 added by humkyung

/**
	@brief	return "NO"

	@author	humkyung

	@date	2012.04.25

	@return	 int
*/
int CIsoBMData::no() const
{
	return m_iNo;
}

/**
	@brief	Main or Sub Size를 파싱한다. 분수의 경우에 소수점이 있는 것은 대분수이다.
	분수 형식을 소수점 형식으로 변환하여 리턴한다.

	@author	humkyung
*/
CString CIsoBMData::ParseSizeNum( const CString& rString )
{
	CString res(rString);

	int at = rString.Find( _T("/") );
	if(-1 != at)
	{
		CString numerator[2];
		numerator[0]        = rString.Left(at);
		CString denominator = rString.Right(rString.GetLength() - at - 1);
		
		at = numerator[0].Find(".");
		if( -1 != at )
		{
			numerator[1] = numerator[0].Right(numerator[0].GetLength() - at - 1);
			numerator[0] = numerator[0].Left(at);
		}
		
		at = denominator.Find(".");
		if( -1 != at ) denominator.TrimRight("0");
		
		double value = 0.f;
		if(!denominator.IsEmpty() && !numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) + (atof(numerator[1]) / atof(denominator));
		}
		else if(!denominator.IsEmpty() && numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) / atof(denominator);
		}
		else
		{
			value = atof(numerator[0]) + atof(numerator[1]);
		}
		
		STRINGSTREAM_T oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.Find( _T(".") );
		if( (-1 != at ) && ((res.GetLength() - at - 1) > 1) ) res.TrimRight( _T("0") );

		return res;
	}

	return CString( _T("") );
}

/**	
	@brief	하나의 BM Text Line을 파싱한다.
	@author	humkyung
	@date	2013.02.14
	@return	bool
*/
bool CIsoBMData::ParseType1()
{
	if(CIsoBMData::m_oBMAttrPosMap.empty()) return false;	/// 2013.05.03 added by humkyung

	//////////////////////////////////////////////////////////////////////////
	/// PT                                           NPS          MATL    BMCS      MATL 
	/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
	CString sString(m_rDataString);
	int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

	for(map<STRING_T , pair<int,int> >::const_iterator itr = CIsoBMData::m_oBMAttrPosMap.begin();itr != CIsoBMData::m_oBMAttrPosMap.end();++itr)
	{
		if(-1 == itr->second.second)
			sString = m_rDataString.Mid(itr->second.first , m_rDataString.GetLength() - itr->second.first);
		else	sString = m_rDataString.Mid(itr->second.first , itr->second.second);
		if(_T("MATL DESC") == itr->first)
		{
			m_rMatlDesc = sString;
			m_rMatlDesc.TrimLeft();
			m_rMatlDesc.TrimRight();
		}
		else if(_T("SIZE") == itr->first)
		{
			sString.TrimLeft();
			sString.TrimRight();
			const int nFindPos = sString.Find('X');
			if(-1 != nFindPos)
			{
				m_rNPD[0] = ParseSizeNum(sString.Left(nFindPos));
				m_rNPD[1] = ParseSizeNum(sString.Right(sString.GetLength() - nFindPos - 1));
			}
			else
			{
				m_rNPD[0] = ParseSizeNum(sString);
			}
		}
		else if(_T("MATL SPEC") == itr->first)
		{
			m_rMatlSpec = sString;
			m_rMatlSpec.TrimLeft();
			m_rMatlSpec.TrimRight();
			/// 중간에 공백이 나오기 전까지의 부분을 SPEC로 한다.
			const int at = m_rMatlSpec.Find(' ' , 0);
			if(-1 != at) m_rMatlSpec = m_rMatlSpec.Left(at);
		}
		else if(_T("SYM") == itr->first)
		{
			m_rBMCSSYM = sString;
			m_rBMCSSYM.TrimLeft();
			m_rBMCSSYM.TrimRight();
		}
		else if(_T("MATL CODE") == itr->first)
		{
			m_rMatlCode = sString;
			m_rMatlCode.TrimLeft();
			m_rMatlCode.TrimRight();
			/// 중간에 공백이 나오기 전까지의 부분을 MAT'L CODE로 한다.
			const int at = m_rMatlCode.Find(' ' , 0);
			if(-1 != at) m_rMatlCode = m_rMatlCode.Left(at);
		}
		/// extract bolt length - 2011.09.23 added by humkyung
		else if(_T("BOLT LEN") == itr->first)
		{
			sString.TrimLeft();
			sString.TrimRight();
			m_sBoltLen = ParseSizeNum(sString);
		}
		/// up to here
		else if(_T("QTY") == itr->first)
		{
			m_rQTY = sString;
			m_rQTY.TrimLeft();
			m_rQTY.TrimRight();
		}
	}

	return true;
}

/**	
	@brief	여러개로 이루어진 BM Text Line을 파싱한다.
	@author	humkyung
	@date	2013.02.14
	@return	bool
*/
bool CIsoBMData::ParseType2()
{
	//////////////////////////////////////////////////////////////////////////
	/// PT                                           NPS          MATL    BMCS      MATL 
	/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
	CString sString(m_rDataString);
	int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

	int iIndex = 0 , iTextCount = m_pBMLine->GetTextCount();
	for(map<STRING_T , pair<int,int> >::const_iterator itr = CIsoBMData::m_oBMAttrPosMap.begin();(itr != CIsoBMData::m_oBMAttrPosMap.end()) && (iIndex < iTextCount);++itr,++iIndex)
	{
		sString = m_pBMLine->GetTextAt(iIndex);
		if(_T("MATL DESC") == itr->first)
		{
			m_rMatlDesc = sString;
			m_rMatlDesc.TrimLeft();
			m_rMatlDesc.TrimRight();
		}
		else if(_T("SIZE") == itr->first)
		{
			sString.TrimLeft();
			sString.TrimRight();
			const int nFindPos = sString.Find('X');
			if(-1 != nFindPos)
			{
				m_rNPD[0] = ParseSizeNum(sString.Left(nFindPos));
				m_rNPD[1] = ParseSizeNum(sString.Right(sString.GetLength() - nFindPos - 1));
			}
			else
			{
				m_rNPD[0] = ParseSizeNum(sString);
			}
		}
		else if(_T("MATL SPEC") == itr->first)
		{
			m_rMatlSpec = sString;
			m_rMatlSpec.TrimLeft();
			m_rMatlSpec.TrimRight();
			/// 중간에 공백이 나오기 전까지의 부분을 SPEC로 한다.
			const int at = m_rMatlSpec.Find(' ' , 0);
			if(-1 != at) m_rMatlSpec = m_rMatlSpec.Left(at);
		}
		else if(_T("SYM") == itr->first)
		{
			m_rBMCSSYM = sString;
			m_rBMCSSYM.TrimLeft();
			m_rBMCSSYM.TrimRight();
		}
		else if(_T("MATL CODE") == itr->first)
		{
			m_rMatlCode = sString;
			m_rMatlCode.TrimLeft();
			m_rMatlCode.TrimRight();
			
			/// 중간에 공백이 나오기 전까지의 부분을 MAT'L CODE로 한다.
			const int at = m_rMatlCode.Find(' ' , 0);
			if(-1 != at) m_rMatlCode = m_rMatlCode.Left(at);
		}
		/// extract bolt length - 2011.09.23 added by humkyung
		else if(_T("BOLT LEN") == itr->first)
		{
			sString.TrimLeft();
			sString.TrimRight();
			m_sBoltLen = ParseSizeNum(sString);
		}
		/// up to here
		else if(_T("QTY") == itr->first)
		{
			m_rQTY = sString;
			m_rQTY.TrimLeft();
			m_rQTY.TrimRight();
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**	
	@brief	BM Text Line을 파싱한다.
	
	@author	humkyung

	@date	????.??.??

	@return	bool
*/
bool CIsoBMData::Parse()
{
	bool res = false;

	IsRegExp::RxNode *_rxnode = NULL;
	IsRegExp::RxMatchRes _rxmatch;

	if((NULL != m_pBMLine) && (m_pBMLine->GetTextCount() > 1))
	{
		res = ParseType2();
	}
	else
	{
		res = ParseType1();
	}
	if(false == res) return false;

	int nPos = 0 , nLastFindWhatLen = 0;
	/// find bolt length in matl desc if it is bolt material and bolt len position is not set
	CProjectData* pProjectData = CProjectData::GetInstance();
	if((pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("BOLT_LENGTH_LOCATION"))))
	{
		if(_T("2") == (*pProjectData->m_pProjectSettingMap)[_T("BOLT_LENGTH_LOCATION")])
		{
			/// get bolt length keyword - 2012.08.27 added by humkyung
			CString sBoltLengthKeyword(_T("BOLT LENGTH"));
			if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("BOLT_LENGTH_KEYWORD")))
			{
				sBoltLengthKeyword = (*pProjectData->m_pProjectSettingMap)[_T("BOLT_LENGTH_KEYWORD")];
			}
			/// up to here

			CString rSubString(m_rDataString);
			for(vector<CString>::iterator itr = m_subTextEntry.begin();itr != m_subTextEntry.end();++itr)
			{
				rSubString += (*itr);
			}
			rSubString.MakeUpper();
			const int at = rSubString.Find(sBoltLengthKeyword);
			if(at >= 0)
			{
				CString what(_T("[0-9]+(\\.[0-9]+)?(MM|M)"));	/// 숫자M 혹은 MM
				nPos = ::FindStringHelper(rSubString , what , 0 , nLastFindWhatLen , _rxnode, &_rxmatch);
				if( nPos >= 0 )
				{
					rSubString = rSubString.Mid(_rxmatch.Open[0] , _rxmatch.Close[0] - _rxmatch.Open[0]);
					rSubString = rSubString.TrimRight('M');
					m_sBoltLen = this->ParseSizeNum(rSubString);
					//m_rNPD[1] = m_sBoltLen;
				}
			}
		}
	}

	/// 분수 형식의 숫자를 사이언스 형식으로 바꾼다.
	if(!m_rNPD[0].IsEmpty())
	{
		int at = 0;
		if(-1 != (at = m_rNPD[0].Find(_T("/"))))
		{
			const double b = atof(m_rNPD[0].Right(m_rNPD[0].GetLength() - at - 1));
			
			double a = 0.f , c = 0.f;
			CString rQuotient = m_rNPD[0].Left(at);
			at = rQuotient.Find(_T("."));	/// 여기서 .은 소수점을 표시하는 것이 아니다.
			if(-1 != at)
			{
				a = atof(rQuotient.Left(at));
				c = atof(rQuotient.Right(rQuotient.GetLength() - at - 1));
			}
			else
			{
				c = atof(rQuotient);
			}

			m_rNPD[0].Format(_T("%lf") , a + (c / b));
		}
	}

	if(!m_rNPD[1].IsEmpty())
	{
		int at = 0;
		if(-1 != (at = m_rNPD[1].Find(_T("/"))))
		{
			const double b = atof(m_rNPD[1].Right(m_rNPD[1].GetLength() - at - 1));

			double a = 0.f , c = 0.f;
			CString rQuotient = m_rNPD[1].Left(at);
			at = rQuotient.Find(".");
			if(-1 != at)
			{
				a = atof(rQuotient.Left(at));
				c = atof(rQuotient.Right(rQuotient.GetLength() - at - 1));
			}
			else
			{
				c = atof(rQuotient);
			}
			m_rNPD[1].Format("%lf" , a + (c / b));
		}
	}

	if (_rxnode)
	{
		IsRxFree (_rxnode);
		_rxnode = NULL;
	}

	return true;
}

/**
	@brief	add sub text string

	@author	humkyung
**/
void CIsoBMData::AddSubText(const CString &rString)
{
	m_subTextEntry.push_back(rString);
}

/**
**/
const CIsoBMData* CQCCheck::FindPipe(const CString &rSize) const
{
	for(vector<CIsoBMData>::const_iterator itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		if(rSize == itr->GetMainSize())
		{
			return &(*itr);
		}
	}

	return NULL;
}

/**	
	@brief	칼럼의 index위치를 분류한다.
	
	@author	백흠경

	@param	rTextString	a parameter of type const CString &

	@return	void	
*/
void CQCCheck::ParseColumnIndex(const CString &rTextString)
{
}

/**
	@brief	MainSize,SubSize and MatlCode가 같은지 비교한다.
	
	@author	백흠경
*/
struct MatchSizeAndMatlCode : public binary_function<CNtrData , CIsoBMData , bool>
{
	bool operator()(const CNtrData& lhs , const CIsoBMData& rhs) const
	{
		CString _ntrMain = lhs.GetMainSize();
		CString _ntrSub = lhs.GetSubSize();
		if(_ntrSub.IsEmpty()) _ntrSub = _ntrMain;

		CString _bmMain = rhs.GetMainSize();
		CString _bmSub = rhs.GetSubSize();
		if(_bmSub.IsEmpty()) _bmSub = _bmMain;

		CString _ntrMatlCode = lhs.GetMatlCode();
		CString _bmMatlCode = rhs.m_rMatlCode;

		return ((_ntrMain == _bmMain) && (_ntrSub == _bmSub) && (_ntrMatlCode == _bmMatlCode));
	}
};

/*
	@brief	

	@author	BHK
*/
CNtrData* CQCCheck::FindNtrData(CIsoBMData& IsoBMData)
{
	vector<CNtrData>::iterator where = find_if(m_ntrTextEntry.begin(), m_ntrTextEntry.end(), bind2nd(MatchSizeAndMatlCode(), IsoBMData));
	return ((where != m_ntrTextEntry.end()) ? &(*where) : NULL);
}

/**	
	@brief	SUPPORT의 DETAIL NUMBER를 구한다.

	@param	graphicAreaTextEntry	a parameter of type vector<CIsoBMData*>&
	@param	rSupportNo	a parameter of type const string&

	@return	void	
*/
int CQCCheck::FindSupportDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const string& rSupportNo )
{
	static const STRING_T SEE_DETAIL("SEE DETAIL");

	CIsPoint2d origin;
	vector<CIsoBMData>::iterator itr = graphicAreaTextEntry.begin();
	for(itr = graphicAreaTextEntry.begin() ; itr != graphicAreaTextEntry.end() ; ++itr)
	{
		string rText = (itr)->dataString();
		if( rText == rSupportNo )
		{
			origin = (itr)->origin();
			break;
		}
	}

	string rLastSeeDetailText;
	double nMinDist = -1;
	for( itr = graphicAreaTextEntry.begin() ; itr != graphicAreaTextEntry.end() ; ++itr )
	{
		string rText = (itr)->dataString() , rTemp;
		
		if( 0 == strncmp( rText.c_str() , SEE_DETAIL.c_str() , SEE_DETAIL.length()) )
		{
			CIsPoint2d dest = (itr)->origin();
			double dist = origin.DistanceTo( dest );

			if( (-1 == nMinDist) || (dist < nMinDist) )
			{
				nMinDist = dist;
				rLastSeeDetailText = rText;
			}
		}
	}

	if( !rLastSeeDetailText.empty() )
	{
		string rDetailNo = rLastSeeDetailText.substr( 11 ); 
		return atoi( rDetailNo.c_str() );
	}

	return 0;
}

/**
	@brief	constructor

	@author	humkyung
*/
CIsoBMData::CIsoBMData(const CIsPoint2d& origin , LPCTSTR p)
{
	m_origin = origin;
	m_pBMLine = NULL;
	m_rDataString = p;
}

/**
	@brief	constructor

	@author	humkyung

	@date	2013.02.26
*/
CIsoBMData::CIsoBMData(const CIsPoint2d& origin , CBMLine* p)
{
	m_origin = origin;
	m_pBMLine = p;
	m_rDataString = p->textString();
}

/**
**/
CString CIsoBMData::GetAllSubString() const
{
	CString res;
	for(vector<CString>::const_iterator itr = m_subTextEntry.begin();itr != m_subTextEntry.end();++itr)
	{
		if(!itr->IsEmpty()) res += (*itr);
	}

	return res;
}

/**
**/
CString CIsoBMData::GetSubStringAt(const size_t& at) const
{
	if(at < m_subTextEntry.size()) return m_subTextEntry[at];
	throw exception("range error in CIsoBMData::GetSubStringAt");
}

/**
**/
size_t CIsoBMData::GetSubStringCount() const
{
	return m_subTextEntry.size();
}

/**
**/
CString CCutPipeLengthData::dataString() const
{
	return m_rDataString;
}

/**
	@brief	LineDataRecord에서 필요한 DATA를 구한다.

	@author	BHK
**/
bool CIsoBMData::GetDataFromLineDataRecord(CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if(pLineDataRecord)
	{
		CDataField* pField = pLineDataRecord->FindWithFieldName( "INS_TYPE" );
		if( pField ) m_rInsType = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "INS_THICKNESS" );
		if( pField ) m_rInsThickness = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "PAINT_CODE" );
		if( pField ) m_rPaintCode = pField->value();

		pField = pLineDataRecord->FindWithFieldName( "PLANT_NO" );
		if( pField ) m_rPlantNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "AREA_NO" );
		if( pField ) m_rAreaNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "UNIT_NO" );
		if( pField ) m_rUnitNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "SECTION_NO" );
		if( pField ) m_rSectionNo = pField->value();
		
		return true;
	}

	return false;
}

/**
	@brief	return no

	@author	humkyung

	@date	2012.04.25

	@return	int
*/
int CCutPipeLengthData::no() const
{
	return m_iNo;
}

/**
	@brief	get no

	@author	humkyung

	@date	2012.04.25

	@return	int
*/
int CCutPipeLengthData::ParseNo()
{
	int at = m_rDataString.Find("<");
	if(-1 != at)
	{
		m_iNo = atoi(m_rDataString.Mid(at + 1));
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	Main or Sub Size를 파싱한다. 분수의 경우에 소수점이 있는 것은 대분수이다.
	분수 형식을 소수점 형식으로 변환하여 리턴한다.

	@author	BHK
*/
CString CCutPipeLengthData::ParseSizeNum( const CString& rString )
{
	CString res(rString);

	int at = rString.Find("/");
	if(-1 != at)
	{
		CString numerator[2];
		numerator[0]        = rString.Left(at);
		CString denominator = rString.Right(rString.GetLength() - at - 1);
		
		at = numerator[0].Find(".");
		if( -1 != at )
		{
			numerator[1] = numerator[0].Right(numerator[0].GetLength() - at - 1);
			numerator[0] = numerator[0].Left(at);
			///numerator[1].TrimRight("0");
		}
		
		at = denominator.Find(".");
		if( -1 != at ) denominator.TrimRight("0");
		
		double value = 0.f;
		if(!denominator.IsEmpty() && !numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) + (atof(numerator[1]) / atof(denominator));
		}
		else if(!denominator.IsEmpty() && numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) / atof(denominator);
		}
		else
		{
			value = atof(numerator[0]) + atof(numerator[1]);
		}
		
		stringstream oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.Find(".");
		if( -1 != at ) res.TrimRight("0");

		return res;
	}

	return CString("");
}