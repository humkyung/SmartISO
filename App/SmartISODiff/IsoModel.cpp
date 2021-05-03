#include "StdAfx.h"
#include <assert.h>
#include <Tokenizer.h>
#include <util/FileTools.h>

#include <DgnEnum.h>
#include <DgnLine.h>
#include <DgnArc.h>
#include <DgnUnknownElement.h>

#include "IsoModel.h"
#include "IsoModelDocData.h"

#include "PipeElement.h"
#include "TeeElement.h"
#include "DimExtLineElement.h"
#include "DimElement.h"
#include "LeaderElement.h"

CIsoModel::CIsoModel(CString sFilePath) : m_bOpenForWrite(false) , m_sFilePath(sFilePath)
{
	m_pDgnDocument = NULL;
}

CIsoModel::~CIsoModel(void)
{
	try
	{
		if(true == m_bOpenForWrite)
		{
			m_pDgnDocument->Write();
			::DeleteFile(m_sFilePath + _T(".dgn"));
		}

		CDgnDocument::DeleteInstance(m_pDgnDocument);
		m_oDgnElementList.clear();

		for(list<CDgnElement*>::iterator itr = m_oDummyDgnElementList.begin();itr != m_oDummyDgnElementList.end();++itr)
		{
			CDgnElement::DeleteInstance( *itr );
		}
		m_oDummyDgnElementList.clear();
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-05
    @class      CIsoModel
    @function   ReadVolume
    @return     int
    @param      const           CString&
    @param      sIniFilePath
    @brief
******************************************************************************/
int CIsoModel::ReadVolume(const CString& sIniFilePath)
{
	CString sAppName = _T("Border Setting");
	TCHAR szBuf[MAX_PATH + 1] = {'\0',};

	vector<STRING_T> oResult;
	if(GetPrivateProfileString(sAppName ,  _T("Iso Graphic Area") , NULL , szBuf , MAX_PATH , sIniFilePath))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			const double minx = ATOF_T(oResult[0].c_str());
			const double miny = ATOF_T(oResult[1].c_str());
			const double maxx = ATOF_T(oResult[2].c_str());
			const double maxy = ATOF_T(oResult[3].c_str());
	
			m_oIsoAreaMap.insert(make_pair(_T("Main_Drawing_Area") , new CIsoArea(_T("Main_Drawing_Area") , minx , miny , maxx , maxy , this)));
		}
	}

	if(GetPrivateProfileString(sAppName ,  _T("PRT BM Area") , NULL , szBuf , MAX_PATH , sIniFilePath))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			const double minx = ATOF_T(oResult[0].c_str());
			const double miny = ATOF_T(oResult[1].c_str());
			const double maxx = ATOF_T(oResult[2].c_str());
			const double maxy = ATOF_T(oResult[3].c_str());

			m_oIsoAreaMap.insert(make_pair(_T("BM_Area") , new CIsoBMArea(_T("BM_Area") , minx , miny , maxx , maxy , this)));
		}
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-01
    @class      CIsoModel
    @function   Read
    @return     int
    @param      void
    @brief		read iso model
******************************************************************************/
int CIsoModel::Read(const CString& sIniFilePath , const bool& bOpenForWrite)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

	m_bOpenForWrite = bOpenForWrite;

	///2011.09.05 added by humkyung
	ReadVolume(sIniFilePath);

	m_pDgnDocument = CDgnDocument::CreateInstance();
	if(NULL != m_pDgnDocument)
	{
		/// set font path
		CString sExecPath;
		CFileTools::GetExecutableDirectory(sExecPath);
		const STRING_T sFontFolderPath = (sExecPath + _T("fonts")).operator LPCTSTR();
		m_pDgnDocument->SetFontFolderPath( sFontFolderPath );
		/// up to here

		if(true == m_bOpenForWrite)
		{
			::CopyFile(m_sFilePath , m_sFilePath + _T(".dgn") , TRUE);
			m_pDgnDocument->OpenForWrite(m_sFilePath , CDgnDocument::UOR_BASE , m_sFilePath + _T(".dgn"));
		}
		else
		{
			m_pDgnDocument->Read(m_sFilePath , CDgnDocument::UOR_BASE/* , bChangeColor*/);
		}
		docData.m_dScale = m_pDgnDocument->scale();
		const size_t iEntCount = m_pDgnDocument->GetEntityCount();
		if(iEntCount > 0)
		{
			for(int i = 0;i < int(iEntCount);++i)
			{
				CDgnElement* pDgnEnt = m_pDgnDocument->GetEntityAt(i);
				if(pDgnEnt->IsKindOf(CDgnUnknownElement::TypeString())) continue;

				const UInt32 iEntType = pDgnEnt->elementType();
				UInt32 uiLevel = pDgnEnt->level();
				UInt32 uiWeight= pDgnEnt->weight();
				CString sColor;
				sColor.Format(_T("%d") , pDgnEnt->color());

				switch(iEntType)
				{	
					///convert arc and lin to line string - 2011.11.15 added by humkyung
					case ARC_ELM:
					{
						if (docData.IsPipeLineLevel(uiLevel) && docData.IsPipeLineColor(sColor) && docData.IsPipeLineWeight(uiWeight))
						{
							vector<DPoint3d> oVertexList;
							CDgnArc* pArcElm = static_cast<CDgnArc*>(pDgnEnt);
							pArcElm->CreateSegments(oVertexList , 4);

							CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(CDgnLineString::CreateInstance());
							pLineStringElm->SetElementID( pArcElm->GetElementID() );
							pLineStringElm->SetLevel(pArcElm->level());
							pLineStringElm->SetLineColor(pArcElm->color());
							pLineStringElm->SetLineWeight(pArcElm->weight());
							pLineStringElm->SetVertexList( oVertexList );

							m_oDgnElementList.push_back( pLineStringElm );
							m_oDummyDgnElementList.push_back( pLineStringElm );
						}
					}
						break;
					case LINE_ELM:
					{
						CDgnLine* pLineElm = static_cast<CDgnLine*>(pDgnEnt);
						DPoint3d ptStart = pLineElm->GetStartPoint();
						DPoint3d ptEnd = pLineElm->GetEndPoint();

						CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(CDgnLineString::CreateInstance());
						pLineStringElm->SetLevel(pLineElm->level());
						pLineStringElm->SetLineColor(pLineElm->color());
						pLineStringElm->SetLineWeight( pLineElm->weight() );
						pLineStringElm->SetElementID( pLineElm->GetElementID() );
						vector<DPoint3d> oVertexList;
						oVertexList.push_back(ptStart);
						oVertexList.push_back(ptEnd);
						pLineStringElm->SetVertexList( oVertexList );

						m_oDgnElementList.push_back( pLineStringElm );
						m_oDummyDgnElementList.push_back( pLineStringElm );
					}
						break;
					/// up to here
					case LINE_STRING_ELM:
						m_oDgnElementList.push_back( pDgnEnt );
						break;
					case TEXT_ELM:
						m_oDgnElementList.push_back( pDgnEnt );
						break;
					default:
						m_oDgnElementList.push_back( pDgnEnt );
						break;
				}
			}

			/// add iso element to corresponding area - 2011.09.05 added by humkyung
			for(list<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
			{
				if(0L == (*itr)->filePos())
				{
					int d = 1;
				}

				CDgnVolume volume = (*itr)->volume();
				for(map<CString , SmartPtr<CIsoArea> >::iterator jtr = m_oIsoAreaMap.begin();jtr != m_oIsoAreaMap.end();++jtr)
				{
					if(volume.CollideWith(jtr->second->volume()))
					{
						jtr->second->Add(*itr);
						break;
					}
				}
			}
			/// up to here
		}
	}

	return 0;
}


/******************************************************************************
    @author     humkyung
    @date       2011-09-01
    @class      CIsoModel
    @function   CompareWith
    @return     int
    @param      CIsoModel*  rhs
    @brief		compare two iso models
******************************************************************************/
int CIsoModel::CompareWith(CIsoModel* pOldModel)
{
	assert(pOldModel && "pOldModel is NULL");

	if(pOldModel)
	{
		for(map<CString , SmartPtr<CIsoArea> >::iterator itr = m_oIsoAreaMap.begin();itr != m_oIsoAreaMap.end();++itr)
		{
			map<CString , SmartPtr<CIsoArea> >::iterator where = pOldModel->m_oIsoAreaMap.find(itr->first);
			if(pOldModel->m_oIsoAreaMap.end() != where)
			{
				itr->second->PrepareToCompare();
				where->second->PrepareToCompare();
				
				/// check environment before compare
				/*if((ERROR_SUCCESS != itr->second->CheckEnvironment()) || (ERROR_SUCCESS != where->second->CheckEnvironment()))
				{
					return ERROR_BAD_ENVIRONMENT;
				}*/
				/// up to here

				itr->second->CompareWith(where->second);
			}
		}

		///Check();

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoModel
    @function   GetBMDataCorrespondingToPartNo
    @return     CIsoBMData
    @param      const   CString&
    @param      sPartNo
    @brief
******************************************************************************/
bool CIsoModel::GetBMDataCorrespondingToPartNo(CIsoBMData& oIsoBMData , const CString& sPartNo)
{
	map<CString , SmartPtr<CIsoArea> >::iterator where = m_oIsoAreaMap.find(_T("BM_Area"));
	if(where != m_oIsoAreaMap.end())
	{
		CIsoBMArea* pBMArea = static_cast<CIsoBMArea*>(where->second.GetPtr());
		return pBMArea->GetBMDataCorrespondingToPartNo( oIsoBMData , sPartNo );
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CIsoModel
    @function   GetDgnModel
    @return     GetDgnDocument*
    @brief		return dgn model
******************************************************************************/
CDgnDocument* CIsoModel::GetDgnDocument()
{
	return m_pDgnDocument;
}

int CIsoModel::Check()
{
	for(map<CString , SmartPtr<CIsoArea> >::iterator itr = m_oIsoAreaMap.begin();itr != m_oIsoAreaMap.end();++itr)
	{
		itr->second->Check();
	}

	return 0;
}
