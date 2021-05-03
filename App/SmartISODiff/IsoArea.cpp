#include "stdafx.h"
#include <assert.h>

#include <limits>
#include <algorithm>

#include <DgnEnum.h>
#include <DgnText.h>
#include <DgnLineString.h>
#include <DgnComplexShapeHeader.h>

#include "PipeSupportElement.h"
#include "IsoArea.h"
#include "PipeElement.h"
#include "DimElement.h"
#include "ElbowElement.h"
#include "TeeElement.h"
#include "OletElement.h"
#include "ReducerElement.h"	/// 2011.10.31 added by humkyung
#include "RWeldElement.h"	/// 2011.11.01 added by humkyung
#include "AttributeElement.h"
#include "WeldMarkElement.h"
#include "SlopeAreaMarkElement.h"
#include "PageConnectionElement.h"
#include "IsoModelDocData.h"
#include "IsoModel.h"
#include "SmartPtr.h"
#include "ArrowElement.h"
#include "IsoElement.h"
#include "LeaderElement.h"
#include "IsoModelDocData.h"
#include "ConvexHull.h"

using namespace std;

CIsoArea::CIsoArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel) : 
m_volume(minx , miny , 0.0 , maxx , maxy , 0.0) , m_rxnode(NULL) , m_pIsoModel(pIsoModel)
{
	m_sName = sName;
}

CIsoArea::~CIsoArea(void)
{
	try
	{
		for(vector<CPipeRoutine*>::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			SAFE_DELETE(*itr);
		}
		m_oPipeRoutineList.clear();

		CleanElementMap();
	}
	catch(...)
	{
	}
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-05
	@class      CIsoArea
	@function   Add
	@return     void
	@param      CIsoElement*    pEle
	@brief
******************************************************************************/
void CIsoArea::Add(CDgnElement* pEle)
{
	assert(pEle && "pEle is NULL");

	if(pEle)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		CString sType = _T("general");
		/// element를 분류하여 저장한다.(분류 예 : pipe , dimension , general)
		const int iEntType = pEle->elementType();
		UInt32 uiLevel = pEle->level();
		CString sColor;
		sColor.Format(_T("%d") , pEle->color());
		UInt32 uiWeight = pEle->weight();
		
		switch(iEntType)
		{	
		case LINE_ELM:
		case LINE_STRING_ELM:
			if(0 != pEle->style()) return;
			if (docData.IsPipeLineLevel(uiLevel) && docData.IsPipeLineColor(sColor) && docData.IsPipeLineWeight(uiWeight))
			{
				sType = _T("pipe");
			}
			break;
		case TEXT_ELM:
			{
				CDgnText* pTextElm = static_cast<CDgnText*>(pEle);
				if(!CString(pTextElm->textString()).IsEmpty())
				{
					/// 2011.11.16 added by humkyung
					if(0.0 == CIsoModelDocData::m_dDrawingToler)
					{
						CIsoModelDocData::m_dDrawingToler = pTextElm->GetTextHeight() * 0.1;
					}
					/// up to here

					/*vector<DPoint3d> oEnclosePolygon = pTextElm->enclosePolygon();
					oEnclosePolygon.push_back(oEnclosePolygon.front());
					::DrawPolyline(m_pIsoModel->GetDgnDocument() , oEnclosePolygon , _T("0"));*/
				}
				else
				{
					sType = _T("general");
				}
			}
			break;
		}

		map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(sType);
		if(where == m_oDgnElementMap.end())
		{
			m_oDgnElementMap.insert(make_pair(sType , new vector<CDgnElement*>));
		}

		m_oDgnElementMap[sType]->push_back(pEle);
	}
}

/******************************************************************************
@author     humkyung
@date       2011-09-05
@class      CIsoArea
@function   volume
@return     CDgnVolume
@brief
******************************************************************************/
CDgnVolume CIsoArea::volume() const
{
	return m_volume;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-24
    @class      CIsoArea
    @function   GetIsoModel
    @return     CIsoModel*
    @brief
******************************************************************************/
CIsoModel* CIsoArea::GetIsoModel() const
{
	return m_pIsoModel;
}

template<typename InputIterator,typename OutputIterator,typename Op>
OutputIterator copy_if(OutputIterator destBegin,
					   InputIterator begin,
					   InputIterator end,
					   Op Pr)
{
	while( begin != end )
	{
		if( Pr (*begin) )
		{
			*destBegin++ = *begin;
		}
		++begin;
	}

	return destBegin;
}

class IsTextElement
{
public:
	bool operator()(const CDgnElement* p) const
	{
		assert(p && "p is NULL");
		if(p) return (p->IsKindOf(CDgnText::TypeString()));

		return false;
	}
};

/// 
/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      
	@function   
	@return     void
	@brief		Patr No 의 Volume 안에 들어 있는 Dimension Line을 찾아서 push_back
******************************************************************************/
void CIsoArea::FindConnectedpartNoDimLine(CAttributeElement* pAttrElm , CDgnVolume volume)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		vector<CDgnElement*>* pDimElmList = m_oDgnElementMap[_T("general")];
		
		for(vector<CDgnElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				if(2 == pLineStringElm->GetVertexCount())
				{
					const double dToler = 0;
					DPoint3d AddMaxToler;
					DPoint3d AddMinToler;
					
					DPoint3d pts[2] = {0,0,};
					pts[0] = pLineStringElm->GetVertexAt(0);
					pts[1] = pLineStringElm->GetVertexAt(1);

					CDgnVolume vol_0 ;
					AddMaxToler.x = pts[0].x + dToler;
					AddMaxToler.y = pts[0].y + dToler;
					AddMaxToler.z = 0;
					vol_0.Add (AddMaxToler);

					AddMinToler.x = pts[0].x - dToler;
					AddMinToler.y = pts[0].y - dToler;
					AddMinToler.z = 0;
					vol_0.Add (AddMinToler);
					
					CDgnVolume vol_1 ;
					AddMaxToler.x = pts[1].x + dToler;
					AddMaxToler.y = pts[1].y + dToler;
					AddMaxToler.z = 0;
					vol_1.Add (AddMaxToler);

					AddMinToler.x = pts[1].x - dToler;
					AddMinToler.y = pts[1].y - dToler;
					AddMinToler.z = 0;
					vol_1.Add (AddMinToler);

					if ((volume.Contains(pts[0])) || volume.Contains(pts[1]))
					{
						pAttrElm->AddLeaderlineElement(*itr); 
						break;
					}	
				}
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-19
    @class      CIsoArea
    @function   SetupElementMap
    @return     int
    @brief
******************************************************************************/
int CIsoArea::SetupElementMap()
{
	{
		map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("dimension"));
		if(m_oDgnElementMap.end() == where)
		{
			m_oDgnElementMap.insert(make_pair(_T("dimension") , new vector<CDgnElement*>));
		}
	}

	map<CString , vector<CIsoElement*>*>::iterator where = m_oIsoElementMap.find(_T("arrow"));
	if(where == m_oIsoElementMap.end())
	{
		m_oIsoElementMap.insert(make_pair(_T("arrow") , new vector<CIsoElement*>));
	}

	where = m_oIsoElementMap.find(_T("attribute"));
	if(where == m_oIsoElementMap.end()) m_oIsoElementMap.insert(make_pair(_T("attribute") , new vector<CIsoElement*>));

	where = m_oIsoElementMap.find(_T("dimension"));
	if(m_oIsoElementMap.end() == where)
	{
		m_oIsoElementMap.insert(make_pair(_T("dimension") , new vector<CIsoElement*>));
	}

	/// Pipe요소를 생성한다.
	where = m_oIsoElementMap.find(_T("pipe"));
	if(where == m_oIsoElementMap.end())
	{
		m_oIsoElementMap.insert(make_pair(_T("pipe") , new vector<CIsoElement*>));
	}
	
	where = m_oIsoElementMap.find(_T("fitting"));
	if(where == m_oIsoElementMap.end())
	{
		m_oIsoElementMap.insert(make_pair(_T("fitting") , new vector<CIsoElement*>));
	}

	where = m_oIsoElementMap.find(_T("weld mark"));
	if(where == m_oIsoElementMap.end())
	{
		m_oIsoElementMap.insert(make_pair(_T("weld mark") , new vector<CIsoElement*>));
	}

	where = m_oIsoElementMap.find(_T("slope area mark"));
	if(where == m_oIsoElementMap.end())
	{
		m_oIsoElementMap.insert(make_pair(_T("slope area mark") , new vector<CIsoElement*>));
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-14
    @class      CIsoArea
    @function   CleanElementMap
    @return     int
    @brief
******************************************************************************/
int CIsoArea::CleanElementMap()
{
	for(map<CString , vector<CDgnElement*>*>::iterator itr = m_oDgnElementMap.begin();itr != m_oDgnElementMap.end();++itr)
	{
		SAFE_DELETE( itr->second );
	}
	m_oDgnElementMap.clear();

	for(map<CString , vector<CIsoElement*>*>::iterator itr = m_oIsoElementMap.begin();itr != m_oIsoElementMap.end();++itr)
	{
		for(vector<CIsoElement*>::iterator jtr = itr->second->begin();jtr != itr->second->end();++jtr)
		{
			SAFE_DELETE( *jtr );
		}
		
		SAFE_DELETE( itr->second );
	}
	m_oIsoElementMap.clear();

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-29
    @class      CIsoArea
    @function   SeperateDimTextFrom
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief		seperate dim. text from general element
******************************************************************************/
int CIsoArea::SeperateDimTextFrom(vector<CDgnElement*>* pDgnElmList)
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	if(pDgnElmList)
	{
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();/*++itr*/)
		{
			if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* pTextElm = static_cast<CDgnText*>(*itr);
				CString sString = pTextElm->textString();
				sString.Trim();
				int iLastFindWhatLen = 0;
				if(0 == ::FindStringHelper(sString , _T("[0-9]+") , 0 , iLastFindWhatLen , m_rxnode , &m_rxmatch))
				{
					if(iLastFindWhatLen == sString.GetLength())
					{
						const DPoint3d origin = pTextElm->origin();

						/// check text is instrument text
						bool isInstText = false;
						for(vector<CDgnElement*>::iterator jtr = pDgnElmList->begin();jtr != pDgnElmList->end();++jtr)
						{
							if(itr == jtr) continue;
							if((*jtr)->IsKindOf(CDgnLineString::TypeString()))
							{
								CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);
								
								DPoint3d center;
								double dRadius = 0.0;
								if(pLineStringElm->IsClosed() && CIsoElement::IsPossibleToBeCircle(center , dRadius , *jtr) && 
									pLineStringElm->volume().Contains(origin))
								{
									isInstText = true;
									break;
								}
							}
							else if((*jtr)->IsKindOf(CDgnComplexShapeHeader::TypeString()))
							{
								DPoint3d center;
								double dRadius = 0.0;
								if(CIsoElement::IsPossibleToBeCircle(center , dRadius , *jtr) && (*jtr)->volume().Contains(origin))
								{
									isInstText = true;
									break;
								}
							}
						}
						/// up to here

						if(false == isInstText)
						{
							m_oDgnElementMap[_T("dimension")]->push_back( *itr );
							itr = pDgnElmList->erase( itr );
							continue;
						}
					}
				}
			}
			++itr;
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-27
	@class      CIsoArea
	@function   PrepareToCompare
	@return     void
	@brief		prepare to compare
******************************************************************************/
void CIsoArea::PrepareToCompare()
{
	SetupElementMap();

	CollectPageConnectionElement();

	/// remove arrow elemnts from "general/dimension" area - 2011.10.06 added by humkyung
	CollectArrows(_T("general"));
	/// up to here

	CollectAttributeElements();
	
	/// seperate dimension text and general text - 2011.11.16 added by humkyung
	map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find( _T("general") );
	if(where != m_oDgnElementMap.end() && (NULL != where->second))
	{
		SeperateDimTextFrom(where->second);
	}
	/// up to here

	CollectDimensions();

	///TODO:need more test - try to get leader line for attribute which has no leader until here - 2011.12.20 added by humkyung
	vector<CIsoElement*>* pAttrElmList = m_oIsoElementMap[_T("attribute")];
	for(vector<CIsoElement*>::iterator itr = pAttrElmList->begin();itr != pAttrElmList->end();++itr)
	{
		CAttributeElement* pAttrElm = static_cast<CAttributeElement*>(*itr);
		const CString sAttrType = pAttrElm->GetAttrType();
		if((_T("PIPE") == sAttrType) || (_T("NONE") == sAttrType)) continue;

		if(0 == pAttrElm->GetLeaderElement()->size())
		{
			vector<CDgnVolume> oVolumeList;

			pAttrElm->CollectLeaderLine(m_oDgnElementMap[_T("general")] , oVolumeList , m_oIsoElementMap[_T("arrow")]);
			pAttrElm->GetLeaderElement()->CollectPartOfLeaderLine (m_oDgnElementMap[_T("general")] , m_oIsoElementMap[_T("arrow")]);
		}
	}
	/// up to here
	CreateConnectedElementWithAttribute();

	/// create pipe elements and connecte pipe
	ConnectPipes();

	AssignDimensionToPipe();
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CIsoArea
    @function   GetPipeRoutineStartWith
    @return     CPipeRoutine*
    @param      CIsoElement*    pStartElm
    @brief		return pipe routine start with pStartElm
******************************************************************************/
CPipeRoutine* CIsoArea::GetPipeRoutineStartWith( CIsoElement* pStartElm )
{
	assert(pStartElm && "pStartElm is NULL");

	if(pStartElm)
	{
		for(vector<CPipeRoutine*>::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			if(pStartElm == (*itr)->GetStartElm()) return (*itr);
		}
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-11
    @class      CIsoArea
    @function   CheckEnvironment
    @return     int
    @brief		check environment before compare
******************************************************************************/
int CIsoArea::CheckEnvironment()
{
	if(!m_oPipeRoutineList.empty())
	{
		int iCandidatedCount = 0;

		for(vector<CPipeRoutine*>::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			CIsoElement* pStartElm = (*itr)->GetStartElm();
			if(NULL == pStartElm)
			{
				++(iCandidatedCount);
			}
		}

		return (1 != iCandidatedCount) ? ERROR_BAD_ENVIRONMENT : ERROR_SUCCESS;
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      CIsoArea
	@function   CompareWith
	@return     int
	@param      list<CString>&  oRemovedElemIDList
	@param      list<CString>&  oAddedElemIDList
	@param      CIsoArea*       rhs
	@brief		compare two area( 복잡하고 어려운 루틴이 필요할것 같음. )
******************************************************************************/
int CIsoArea::CompareWith(CIsoArea* rhs)
{
	assert(rhs && "rhs is NULL");

	if(rhs)
	{
		CPipeRoutine *pMainPipeRoutineA = NULL , *pMainPipeRoutineB = NULL;
		for(vector<CPipeRoutine*>::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			CIsoElement* pStartElm = (*itr)->GetStartElm();
			if(NULL == pStartElm)
			{
				if(NULL == pMainPipeRoutineB)
				{
					pMainPipeRoutineB = *itr;
				}
			}
		}

		for(vector<CPipeRoutine*>::iterator itr = rhs->m_oPipeRoutineList.begin();itr != rhs->m_oPipeRoutineList.end();++itr)
		{
			CIsoElement* pStartElm = (*itr)->GetStartElm();
			if(NULL == pStartElm)
			{
				if(NULL == pMainPipeRoutineA)
				{
					pMainPipeRoutineA = *itr;
				}
			}
		}

		if(NULL != pMainPipeRoutineB)
		{
			pMainPipeRoutineB->CompareWith( pMainPipeRoutineA , this );
		}

		/// compare page connection - 2011.11.09 added by humkyung
		vector<CPageConnectionElement*> oPageConnElmListA , oPageConnElmListB;
		for(vector<CIsoElement*>::iterator itr = m_oIsoElementMap[_T("attribute")]->begin();itr != m_oIsoElementMap[_T("attribute")]->end();++itr)
		{
			if((*itr)->IsKindOf(CPageConnectionElement::TypeString()))
			{
				oPageConnElmListB.push_back( static_cast<CPageConnectionElement*>(*itr) );
			}
		}
		for(vector<CIsoElement*>::iterator itr = rhs->m_oIsoElementMap[_T("attribute")]->begin();itr != rhs->m_oIsoElementMap[_T("attribute")]->end();++itr)
		{
			if((*itr)->IsKindOf(CPageConnectionElement::TypeString()))
			{
				oPageConnElmListA.push_back( static_cast<CPageConnectionElement*>(*itr) );
			}
		}
		
		for(vector<CPageConnectionElement*>::iterator itr = oPageConnElmListB.begin();itr != oPageConnElmListB.end();/*++itr*/)
		{
			bool bFound = false;
			for(vector<CPageConnectionElement*>::iterator jtr = oPageConnElmListA.begin();jtr != oPageConnElmListA.end();++jtr)
			{
				if((*itr)->CompareWith(*jtr))
				{
					bFound = true;
					itr = oPageConnElmListB.erase( itr );
					break;
				}
			}

			if(true == bFound)
			{
				continue;
			}
			++itr;
		}

		for(vector<CPageConnectionElement*>::iterator itr = oPageConnElmListB.begin();itr != oPageConnElmListB.end();++itr)
		{
			(*itr)->DrawCloudMark(m_pIsoModel->GetDgnDocument());
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
    @author     humkyung
    @date       2011-11-03
    @class      CIsoArea
    @function   IsRectangular
    @return     bool
    @param      CDgnLineString* pLineStringElm
    @brief		check if linestring is rectangular
******************************************************************************/
bool CIsoArea::IsRectangular(CDgnLineString* pLineStringElm)
{
	assert(pLineStringElm && "pLineStringElm is NULL");

	if(pLineStringElm)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		vector<DPoint3d> oPointList = pLineStringElm->GetPointStringExceptSamePoint();
		const int iVertexCount = oPointList.size();
		if(4 == iVertexCount)
		{
			DPoint3d pts[3] = {0,0,0};
			for(int i = 0 ;i < iVertexCount;++i)
			{
				pts[0] = pLineStringElm->GetVertexAt((i + iVertexCount - 1) % iVertexCount);
				pts[1] = pLineStringElm->GetVertexAt(i);
				pts[2] = pLineStringElm->GetVertexAt((i + 1) % iVertexCount);

				DVec3d vec[2];
				vec[0].x = pts[0].x - pts[1].x;
				vec[0].y = pts[0].y - pts[1].y;
				vec[0].z = pts[0].z - pts[1].z;
				vec[1].x = pts[2].x - pts[1].x;
				vec[1].y = pts[2].y - pts[1].y;
				vec[1].z = pts[2].z - pts[1].z;

				const double rad = ::AngleBetweenVectors(vec[0] , vec[1]);
				const double angle = RAD2DEG(rad);
				if(fabs(90.0 - angle) >= dAngleToler) return false;
			}

			return true;
		}
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-20
    @class      CIsoArea
    @function   CollectLeaderLineForAttr
    @return     int
    @param      vector<CIsoElement*>*   pAttrElmList
    @brief
******************************************************************************/
int CIsoArea::CollectLeaderLineForAttr(vector<CIsoElement*>* pAttrElmList , vector<CDgnElement*>* pDgnElmList)
{
	assert(pAttrElmList && pDgnElmList && "pAttrElmList or pDgnElmList is NULL");

	if(pAttrElmList && pDgnElmList)
	{
		vector<CIsoElement*>* pArrElmList = m_oIsoElementMap[_T("arrow")];
		map<int , SmartPtr<vector<CIsoElement*>> > oTempAttrElmMap;
		/// Part No Area 안에 있는 Leader 중에 가장 멀리 떨어진 Dim Leader 을 찾음
		for(vector<CIsoElement*>::iterator itr = pAttrElmList->begin();itr != pAttrElmList->end();++itr)
		{
			CAttributeElement* pAttrElm = static_cast<CAttributeElement*>(*itr);
			if(_T("PIPE") == pAttrElm->GetAttrType()) continue;

			vector<CDgnVolume> oVolumeList;
			pAttrElm->CreateVolumeList(oVolumeList);
			for(vector<CDgnVolume>::iterator jtr = oVolumeList.begin();jtr != oVolumeList.end();++jtr)
			{
				pAttrElm->ExpandVolume( *jtr );
			}
			vector<CDgnElement*> oResult = pAttrElm->GetConnectedFirstLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
			if(1 == oResult.size())		// Part No Area 안에 들어 있는 Leader 가 1개 일 때
			{
				pAttrElm->CollectLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
				pAttrElm->GetLeaderElement()->CollectPartOfLeaderLine (pDgnElmList , m_oIsoElementMap[_T("arrow")]);
			}
			else
			{
				// Part No Area 안에 들어 있는 Leader 가 0개 or 2개 이상일 때는 oTempAttElmMap 에 담아두고 이후에 처리
				map<int , SmartPtr<vector<CIsoElement*>> >::iterator where = oTempAttrElmMap.find(oResult.size());
				if(oTempAttrElmMap.end() == where)
				{
					oTempAttrElmMap.insert(make_pair(oResult.size() , new vector<CIsoElement*>));
				}

				oTempAttrElmMap[oResult.size()]->push_back( pAttrElm );	
			}
		}

		///NOTICE: 이부분은 불안정함.. 수정 필요!!!
		for(map<int , SmartPtr<vector<CIsoElement*>> >::iterator itr = oTempAttrElmMap.begin();itr != oTempAttrElmMap.end();++itr)
		{
			for(vector<CIsoElement*>::iterator jtr = itr->second.GetPtr()->begin();jtr != itr->second.GetPtr()->end();++jtr)
			{
				CAttributeElement* pAttrElm = static_cast<CAttributeElement*>(*jtr);
				/// 파이프는 굳이 leader line을 찾을 필요가 없다...
				if(_T("PIPE") == pAttrElm->GetAttrType()) continue;

				const double dTolers[2]={pAttrElm->m_dUsedToler*0.25 , pAttrElm->m_dUsedToler*0.5};
				for(int i = 0;i < SIZE_OF_ARRAY(dTolers);++i)
				{
					vector<CDgnVolume> oVolumeList;
					pAttrElm->CreateVolumeList(oVolumeList);
					if(0 == itr->first)
					{
						pAttrElm->m_dUsedToler += dTolers[i];
					}
					else
					{
						pAttrElm->m_dUsedToler -= dTolers[i];
					}

					for(vector<CDgnVolume>::iterator jtr = oVolumeList.begin();jtr != oVolumeList.end();++jtr)
					{
						pAttrElm->ExpandVolume( *jtr );
					}

					vector<CDgnElement*> oResult = pAttrElm->GetConnectedFirstLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
					if(1 == oResult.size())
					{
						pAttrElm->CollectLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
						pAttrElm->GetLeaderElement()->CollectPartOfLeaderLine (pDgnElmList , pArrElmList);
						if(!pAttrElm->GetLeaderElement()->m_oDgnElementList.empty())
						{
							break;
						}
					}
				}

				if(pAttrElm->GetLeaderElement()->m_oDgnElementList.empty())
				{
					vector<CDgnVolume> oVolumeList;
					pAttrElm->CreateVolumeList(oVolumeList);
					for(vector<CDgnVolume>::iterator jtr = oVolumeList.begin();jtr != oVolumeList.end();++jtr)
					{
						pAttrElm->ExpandVolume( *jtr );
					}

					pAttrElm->CollectLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
					pAttrElm->GetLeaderElement()->CollectPartOfLeaderLine (pDgnElmList , pArrElmList);
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CIsoArea
    @function   GetElevationAttribute
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief
******************************************************************************/
int CIsoArea::GetElevationAttribute(vector<CDgnElement*>* pDgnElmList)
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	if(pDgnElmList)
	{
		list<CAttributeElement*> oElevAttrElmList;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();/*++itr*/)
		{
			if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* pTextElm = static_cast<CDgnText*>(*itr);

				int iLastFindWhatLen = 0;
				if(0 == ::FindStringHelper(pTextElm->textString() , _T("EL (\\+|-)[0-9]+") , 0, iLastFindWhatLen , m_rxnode , &m_rxmatch))
				{
					IsoElement::CAttributeElement* pAttrElm = new IsoElement::CAttributeElement(pTextElm->textString());
					if(NULL != pAttrElm)
					{
						pAttrElm->Add( pTextElm );
						m_oIsoElementMap[_T("attribute")]->push_back(pAttrElm);
						oElevAttrElmList.push_back( pAttrElm );

						itr = pDgnElmList->erase(itr);
						continue;
					}
				}
			}
			++itr;
		}

		for(list<CAttributeElement*>::iterator itr = oElevAttrElmList.begin();itr != oElevAttrElmList.end();++itr)
		{
			CDgnText* pTextElm = (*itr)->GetPartNoTextElement();
			if(NULL != pTextElm)
			{
				DPoint3d origin = pTextElm->origin();
				(*itr)->CollectPartOfAttribute( pDgnElmList , pTextElm , origin);
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-30
	@class      CIsoArea
	@function   CollectAttributeElements
	@return     int
	@param      void
	@brief		get attribute elements which consists of a line string has 5 vertices and contains text
******************************************************************************/
int CIsoArea::CollectAttributeElements(void)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	double dToler = 0.0;

	vector<CDgnElement*>* pGeneralElmList = NULL;
	/// store collected elements to m_oIsoElementMap
	map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("general"));
	if(where != m_oDgnElementMap.end())
	{
		pGeneralElmList = where->second;
		if(NULL == pGeneralElmList) return ERROR_BAD_ENVIRONMENT;

		DPoint3d center;
		double dRadius = 0.0;
		list<CDgnLineString*> oCandidatedLineString;
		for(vector<CDgnElement*>::iterator itr = pGeneralElmList->begin();itr != pGeneralElmList->end();/*++itr*/)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				const int iVertexCount = pLineStringElm->GetVertexCount();
				if(IsRectangular(pLineStringElm))
				{
					oCandidatedLineString.push_back( pLineStringElm );
					itr = pGeneralElmList->erase(itr);
					continue;
				}
				else if(CSlopeAreaMarkElement::IsSlopeAreaMark( pLineStringElm ))
				{
					CSlopeAreaMarkElement* pSlopeAreaMarkElm = new CSlopeAreaMarkElement();
					pSlopeAreaMarkElm->Add( pLineStringElm );
					/// add weld mark
					{
						m_oIsoElementMap[_T("slope area mark")]->push_back( pSlopeAreaMarkElm );
					}

					itr = pGeneralElmList->erase(itr);
					continue;
				}
				else if(CWeldMarkElement::IsWeldMark(center , dRadius , pLineStringElm , pGeneralElmList))
				{
					CWeldMarkElement* pWeldMarkElm = new CWeldMarkElement(center , dRadius);
					pWeldMarkElm->Add( pLineStringElm );
					/// add weld mark
					{
						m_oIsoElementMap[_T("weld mark")]->push_back( pWeldMarkElm );
					}

					itr = pGeneralElmList->erase(itr);
					continue;
				}
			}
			else if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				int iLastFindWhatLen = 0;

				/// create a instance for RWELD - 2011.11.01 added by humkyung
				CDgnText* pTextElm = static_cast<CDgnText*>(*itr);
				if((_T("RWELD") == CString(pTextElm->textString()).Trim()) || (_T("RPAD") == CString(pTextElm->textString()).Trim()))
				{
					IsoElement::CAttributeElement* pAttrElm = new IsoElement::CAttributeElement(CString(pTextElm->textString()).Trim());
					pAttrElm->Add( pTextElm );
					m_oIsoElementMap[_T("attribute")]->push_back( pAttrElm );

					itr = pGeneralElmList->erase(itr);
					continue;
				}
				/// up to here
			}

			++itr;
		}
		
		/// collect size attribute - 2011.12.02 added by humkyung
		for(vector<CDgnElement*>::iterator jtr = pGeneralElmList->begin();jtr != pGeneralElmList->end();/*++jtr*/)
		{
			if((*jtr)->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* pTextElm = static_cast<CDgnText*>(*jtr);
				int nLastFindWhatLen = 0;
				if(0 == ::FindStringHelper(pTextElm->textString() , _T("[0-9]+") , 0, nLastFindWhatLen, m_rxnode , &m_rxmatch))
				{
					const CString sLastString = CString(pTextElm->textString()).Mid(nLastFindWhatLen);
					if((0 == sLastString.Compare(_T("\""))) || (0 == sLastString.Compare(_T(" MM"))))
					{
						IsoElement::CAttributeElement* pAttrElm = new IsoElement::CAttributeElement(pTextElm->textString());
						if(NULL != pAttrElm)
						{
							pAttrElm->Add( *jtr );
							m_oIsoElementMap[_T("attribute")]->push_back(pAttrElm);

							jtr = pGeneralElmList->erase( jtr );
							continue;
						}
					}
				}
			}
			++jtr;
		}
		/// up to here

		for(list<CDgnLineString*>::iterator itr = oCandidatedLineString.begin();itr != oCandidatedLineString.end();++itr)
		{
			for(vector<CDgnElement*>::iterator jtr = pGeneralElmList->begin();jtr != pGeneralElmList->end();/*++jtr*/)
			{
				if((*jtr)->IsKindOf(CDgnText::TypeString()))
				{
					CDgnText* pTextElm = static_cast<CDgnText*>(*jtr);
					/// 숫자이거나 S로 시작되는 숫자여야 한다.
					int nLastFindWhatLen = 0;
					if(0 == ::FindStringHelper(pTextElm->textString() , _T("(S)?[0-9]+") , 0, nLastFindWhatLen, m_rxnode , &m_rxmatch))
					{
						if(nLastFindWhatLen == CString(pTextElm->textString()).GetLength())
						{
							if(CIsoElement::IsInnerPt((*itr)->GetLineStringPointList() , pTextElm->origin()))
							{
								IsoElement::CAttributeElement* pAttrElm = new IsoElement::CAttributeElement(pTextElm->textString());
								if(NULL != pAttrElm)
								{
									/// LineString과 Text를 저장한다.
									pAttrElm->m_oDgnElementList.push_back( *itr );  // <== 네모 박스
									pAttrElm->Add( *jtr );  // <== 네모 박스 안에 있는 Text (Part No)									
									m_oIsoElementMap[_T("attribute")]->push_back(pAttrElm);

									jtr = pGeneralElmList->erase( jtr );
									continue;
								}
							}
						}
					}
					/// F-G-B mark - 2011.12.20 added by humkyung
					else if(0 == ::FindStringHelper(pTextElm->textString() , _T("(F|G|B)[0-9]+") , 0, nLastFindWhatLen, m_rxnode , &m_rxmatch))
					{
						if(CIsoElement::IsInnerPt((*itr)->GetLineStringPointList() , pTextElm->origin()))
						{
							IsoElement::CAttributeElement* pAttrElm = new IsoElement::CAttributeElement(pTextElm->textString());
							if(NULL != pAttrElm)
							{
								/// LineString과 Text를 저장한다.
								pAttrElm->m_oDgnElementList.push_back( *itr );  // <== 네모 박스
								pAttrElm->Add( *jtr );  // <== 네모 박스 안에 있는 Text (Part No)									
								m_oIsoElementMap[_T("attribute")]->push_back(pAttrElm);

								jtr = pGeneralElmList->erase( jtr );
								continue;
							}
						}
					}
					/// up to here
				}
				++jtr;
			}
		}

		///  Part No 과 연관 되어있는 나머지 Text 를 찾음
		{
			vector<CIsoElement*>* pAttrElmList = m_oIsoElementMap[_T("attribute")];
			/// attribute element에 대응하는 bm 데이타를 찾는다.
			for(vector<CIsoElement*>::iterator itr = pAttrElmList->begin();itr != pAttrElmList->end();++itr)
			{
				IsoElement::CAttributeElement* pAttrElm = static_cast<IsoElement::CAttributeElement*>(*itr);
				if(pAttrElm->GetPartNo().IsEmpty()) continue;

				const CString sPartNo = pAttrElm->GetPartNoExceptS();
				DPoint3d origin = {0,};
				CIsoBMData oIsoBMData(origin , _T("") , _T(""));
				if(true == m_pIsoModel->GetBMDataCorrespondingToPartNo(oIsoBMData , sPartNo))
				{
					pAttrElm->SetIsoBMData( oIsoBMData ); 
				}
			}
			/// up to here

			for(vector<CIsoElement*>::iterator itr = pAttrElmList->begin();itr != pAttrElmList->end();++itr)
			{
				CAttributeElement* pAttrElm = static_cast<CAttributeElement*>(*itr);
				CDgnText* pTextElm = pAttrElm->GetPartNoTextElement();
				if(NULL != pTextElm)
				{
					DPoint3d origin = pTextElm->origin();
					pAttrElm->CollectPartOfAttribute( pGeneralElmList , pTextElm , origin);
				}
			}

			this->GetElevationAttribute(pGeneralElmList);
			this->CollectLeaderLineForAttr(pAttrElmList , pGeneralElmList);
		}
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     yjk
    @date       2011-10-10
    @class      CIsoArea
    @function   IsDuplicatedPoint
    @return     bool
    @param      Dpoint3d point
    @brief		pipe 에는 Support type이 가르키는
******************************************************************************/
CPipeSupportElement* CIsoArea::GetPipeSupportAt(const DPoint3d& pt)
{
	map<CString , vector<CIsoElement*>*>::iterator where = m_oIsoElementMap.find(_T("fitting"));
	
	if(where != m_oIsoElementMap.end())
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler;

		vector<CIsoElement*>* pIsoElmList = where->second;
		for(vector<CIsoElement*>::iterator itr = pIsoElmList->begin();itr != pIsoElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CPipeSupportElement::TypeString()))
			{
				CPipeSupportElement* pPipeSuppElm = static_cast<CPipeSupportElement*>(*itr);
				
				const double dist = ::DistanceBetween((pt) , (pPipeSuppElm->GetOrigin()));

				if (dist < dToler) // 이미 같은 좌표값이 들어 있을 경우
				{
					return pPipeSuppElm;
				}
			}
		}
	}

	return NULL;
}

/******************************************************************************
    @author     yjk
    @date       2011-10-10
    @class      CIsoArea
    @function   CreatePipeSupportElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a pipe support
******************************************************************************/
int CIsoArea::CreatePipeSupportElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler*10;	/// tolerance is as same value as text height

		/// support element!!!
		vector<DPoint3d> oConnPointList;
		pAttrElm->GetLeaderElement()->GetConnPointList(oConnPointList);
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{
			DPoint3d ptConn = oConnPointList[iSize - 1];

			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("general"));
			if(where != m_oDgnElementMap.end())
			{
				vector<CDgnElement*>* pDgnElmList = where->second;

				DVector3d l1;
				l1.org = oConnPointList[iSize - 2];
				l1.end = oConnPointList[iSize - 1];

				double dMinDist = DBL_MAX;
				vector<CDgnLineString*> oIntersectWithLineStringList;  /// Pipe Support Leder 와 intersection 이 일어나는 Line 만 모아 놓음
				for(vector<CDgnElement*>::iterator jtr = pDgnElmList->begin();jtr != pDgnElmList->end();++jtr)
				{
					if((*jtr)->IsKindOf(CDgnLineString::TypeString()))
					{
						CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);
						DPoint3d intersect;
						if (SUCCESS == pLineStringElm->IntersectWith(intersect , &l1))
						{
							const double d = ::DistanceBetween(intersect , ptConn);
							if((d < dToler) && (d < dMinDist))
							{
								dMinDist = d;
								oIntersectWithLineStringList.push_back(pLineStringElm);
							}
						}
					}
				}

				CDgnLineString* p = NULL ;
				if(!oIntersectWithLineStringList.empty())
				{
					std::stable_sort(oIntersectWithLineStringList.begin() , oIntersectWithLineStringList.end() , CIsoElement::SortDgnElmByDistance(l1.end));
					p = oIntersectWithLineStringList[0];
				}
				
				if ((NULL != p) && (p->IsKindOf(CDgnLineString::TypeString()))) // Pipe Support Leder 가장 가까운 Support Line
				{
					DPoint3d pts[2] = {0,0,};
					pts[0] = p->GetVertexAt(0);
					pts[1] = p->GetVertexAt(1);
					const double dLength = ::DistanceBetween((pts[0]) , (pts[1]));

					DPoint3d MidPoint; /// Support Line 의 가운데 위치
					MidPoint.x = (pts[0].x + pts[1].x) * 0.5 ;
					MidPoint.y = (pts[0].y + pts[1].y) * 0.5 ;
					MidPoint.z = 0;

					map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("pipe"));
					if(where != m_oDgnElementMap.end())
					{
						vector<CDgnElement*>* pPipeDgnElmList = where->second;
						CDgnElement* pMinDistancePipeLine = NULL ;

						double dMinDist = DBL_MAX;
						/// Pipe 를 돌면서 Support Line과 가장 가까운 Pipe를 찾는다
						for(vector<CDgnElement*>::iterator jtr = pPipeDgnElmList->begin();jtr != pPipeDgnElmList->end();++jtr) 
						{
							CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);

							const double d = pLineStringElm->DistanceTo ( MidPoint );
							if((d < dLength) && (d  < dMinDist) )
							{
								dMinDist = d;
								pMinDistancePipeLine = *jtr;
							}
						}

						if (NULL != pMinDistancePipeLine )
						{
							CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(pMinDistancePipeLine);
							const int iVertexCount = pLineStringElm->GetVertexCount();
							DPoint3d ptStart = pLineStringElm->GetVertexAt(0);
							DPoint3d ptEnd = pLineStringElm->GetVertexAt(iVertexCount - 1);

							CPipeSupportElement* pPipeSuppElm = NULL;
							double dDist[2] = {0,0};
							dDist[0] = ::DistanceBetween(MidPoint , ptStart);
							dDist[1] = ::DistanceBetween(MidPoint , ptEnd);
							if (dDist[0] < dDist[1])
							{
								pPipeSuppElm = GetPipeSupportAt(ptStart);
								if(NULL == pPipeSuppElm)
								{
									pPipeSuppElm = new CPipeSupportElement(ptStart , pAttrElm);
									m_oIsoElementMap[_T("fitting")]->push_back( pPipeSuppElm );
								}
								else
								{
									pPipeSuppElm->AddAttributeElement( pAttrElm );
								}
							}
							else 
							{
								pPipeSuppElm = GetPipeSupportAt(ptEnd);
								if(NULL == pPipeSuppElm)
								{
									pPipeSuppElm = new CPipeSupportElement(ptEnd , pAttrElm);
									m_oIsoElementMap[_T("fitting")]->push_back( pPipeSuppElm );
								}
								else
								{
									pPipeSuppElm->AddAttributeElement( pAttrElm );
								}
							}
						}
						/*else
						{
							DVector3d l1;
							double dx = pts[1].x - pts[0].x;
							double dy = pts[1].y - pts[0].y;
							l1.org = MidPoint;
							l1.end.x = l1.org.x - dy;
							l1.end.y = l1.org.y + dx;

							DPoint3d intersect;
							double dMinDist = DBL_MAX;
							for(vector<CDgnElement*>::iterator jtr = pPipeDgnElmList->begin();jtr != pPipeDgnElmList->end();++jtr) 
							{
								CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);
								if (SUCCESS == pLineStringElm->IntersectWith(intersect , &l1 , CIsoModelDocData::m_dDrawingToler))
								{
									const double d = ::DistanceBetween(intersect , MidPoint);
									if((d < dToler) && (d < dMinDist))
									{
										dMinDist = d;
										pMinDistancePipeLine = *jtr;
									}
								}
							}

							if(NULL != pMinDistancePipeLine)
							{
								if(NULL == this->GetPipeSupportAt(intersect))
								{
									CPipeSupportElement* pPipeSuppElm = new CPipeSupportElement(intersect , pAttrElm);
									m_oIsoElementMap[_T("fitting")]->push_back( pPipeSuppElm );
								}
							}
						}*/
					}
				}
				else if(NULL == p)
				{
					///NOTICE: 가장 가까운 pipe나 fitting을 찾도록 수정요망 - 2011.12.02
					m_oIsoElementMap[_T("fitting")]->push_back( new CPipeSupportElement(ptConn , pAttrElm) );
				}

				return ERROR_SUCCESS;
			}
		}
	}
	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CIsoArea
    @function   CreateElbowElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a elbow element
******************************************************************************/
int CIsoArea::CreateElbowElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler*10;

		vector<DPoint3d> oConnPointList;
		pAttrElm->GetLeaderElement()->GetConnPointList( oConnPointList );
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{
			DPoint3d ptConn = oConnPointList[iSize - 1];
			
			DVector3d l;
			l.org = oConnPointList[iSize - 2];
			l.end = ptConn;

			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("pipe"));
			if((where != m_oDgnElementMap.end()) && (!where->second->empty()))
			{
				vector<CDgnElement*>* pDgnElmList = where->second;
				
				double dMinDist = DBL_MAX;
				CDgnElement* pPartOfElbow = NULL;

				/// get part of elbow has shortest distance between intersect point and ptConn
				for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
				{
					DPoint3d intersect;
					if(SUCCESS == (*itr)->IntersectWith(intersect , &l))
					{
						const double d = ::DistanceBetween(intersect , ptConn);
						if( (d < dToler) && (d < dMinDist) )
						{
							dMinDist = d;
							pPartOfElbow = (*itr);
						}
					}
				}
				/// up to here
				/// get part of elbow has shortest distance between line string point and ptConn if pPartOfElbow is NULL
				if(NULL == pPartOfElbow)
				{
					vector<CDgnElement*> oTempElmList;
					oTempElmList.insert( oTempElmList.begin() , pDgnElmList->begin() , pDgnElmList->end() );
					stable_sort(oTempElmList.begin() , oTempElmList.end() , CIsoElement::SortDgnElmByDistance( ptConn ));
					pPartOfElbow = oTempElmList[0];
				}
				/// up to here

				if(NULL != pPartOfElbow)
				{
					CElbowElement* pElbowElm = new CElbowElement();
					if(pElbowElm)
					{
						pElbowElm->Add( pPartOfElbow );
						/// remove collected element from list
						pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pPartOfElbow) );
						/// up to here

						pElbowElm->CollectElements( pDgnElmList , m_oIsoElementMap[_T("weld mark")]);
						
						m_oIsoElementMap[_T("fitting")]->push_back( pElbowElm );
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CIsoArea
    @function   CreateTeeElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a tee element
******************************************************************************/
int CIsoArea::CreateTeeElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		vector<DPoint3d> oConnPointList;
		pAttrElm->GetLeaderElement()->GetConnPointList( oConnPointList );
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{
			DPoint3d ptConn = oConnPointList[iSize - 1];

			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("pipe"));
			if(where != m_oDgnElementMap.end())
			{
				vector<CDgnElement*>* pDgnElmList = where->second;
				
				vector<CDgnElement*> pDgnElmList_;
				pDgnElmList_.insert(pDgnElmList_.begin() , pDgnElmList->begin() , pDgnElmList->end());
				stable_sort( pDgnElmList_.begin() , pDgnElmList_.end() , CIsoElement::SortDgnElmByDistance(ptConn) );
				if(pDgnElmList_.size() >= 3)
				{
					CTeeElement* pTeeElm = new CTeeElement();
					if(pTeeElm)
					{
						pTeeElm->Add( pDgnElmList_[0] );
						pTeeElm->Add( pDgnElmList_[1] );
						pTeeElm->Add( pDgnElmList_[2] );
						pTeeElm->BuildConnPointList( ptConn );
						m_oIsoElementMap[_T("fitting")]->push_back( pTeeElm );
					}

					/// remove dgn element from list
					{
						vector<CDgnElement*>::iterator where_ = find(pDgnElmList->begin() , pDgnElmList->end() , pDgnElmList_[0]);
						if(pDgnElmList->end() != where_) pDgnElmList->erase( where_ );
						where_ = find(pDgnElmList->begin() , pDgnElmList->end() , pDgnElmList_[1]);
						if(pDgnElmList->end() != where_) pDgnElmList->erase( where_ );
						where_ = find(pDgnElmList->begin() , pDgnElmList->end() , pDgnElmList_[2]);
						if(pDgnElmList->end() != where_) pDgnElmList->erase( where_ );

						/// remove connected weld mark - 2011.11.18 added by humkyung
						vector<CIsoElement*>* pWeldMarkElmList = m_oIsoElementMap[_T("weld mark")];
						vector<DPoint3d> oConnPtList;
						pTeeElm->GetConnPointList(oConnPtList);
						for(vector<DPoint3d>::iterator jtr = oConnPtList.begin();jtr != oConnPtList.end();++jtr)
						{
							for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();/*++itr*/)
							{
								if((*itr)->volume().Contains( *jtr ))
								{
									itr = pWeldMarkElmList->erase( itr );
									continue;
								}
								++itr;
							}
						}
						/// up to here
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CIsoArea
    @function   CreateOletElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a olet element
******************************************************************************/
int CIsoArea::CreateOletElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		vector<DPoint3d> oConnPointList;
		pAttrElm->GetLeaderElement()->GetConnPointList( oConnPointList );
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{
			DPoint3d ptConn = oConnPointList[iSize - 1];
			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("pipe"));
			if(where != m_oDgnElementMap.end())
			{
				vector<CDgnElement*>* pDgnElmList = where->second;

				COletElement* pOletElm = new COletElement();
				if(ERROR_SUCCESS == pOletElm->CollectElements( pDgnElmList , m_oIsoElementMap[_T("weld mark")] , ptConn ))
				{
					m_oIsoElementMap[_T("fitting")]->push_back( pOletElm );
				}
				else
				{
					SAFE_DELETE( pOletElm );
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CIsoArea
    @function   CreateReducerElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a reducer element
******************************************************************************/
int CIsoArea::CreateReducerElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		vector<DPoint3d> oConnPointList;
		CLeaderElement* pLeaderElm = pAttrElm->GetLeaderElement();
		pLeaderElm->GetConnPointList( oConnPointList );
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{			
			const int iSize = oConnPointList.size();
			DPoint3d pts[2]={0,0};
			pts[0] = oConnPointList[iSize - 2];
			pts[1] = oConnPointList[iSize - 1];

			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("general"));
			if(where != m_oDgnElementMap.end())
			{
				vector<CDgnElement*>* pDgnElmList = where->second;

				CReducerElement* pReducerElm = new CReducerElement();
				if(ERROR_SUCCESS == pReducerElm->CollectElements( pDgnElmList , m_oIsoElementMap[_T("weld mark")] , pts ))
				{
					m_oIsoElementMap[_T("fitting")]->push_back( pReducerElm );
				}
				else
				{
					SAFE_DELETE( pReducerElm );
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CIsoArea
    @function   CreateRWeldElementWith
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief		create a olet element
******************************************************************************/
int CIsoArea::CreateRWeldElementWith(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		vector<DPoint3d> oConnPointList;
		pAttrElm->GetLeaderElement()->GetConnPointList( oConnPointList );
		const int iSize = int(oConnPointList.size());
		if(iSize >= 2)
		{
			DPoint3d ptConn = oConnPointList[iSize - 1];
			map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("pipe"));
			if(where != m_oDgnElementMap.end())
			{
				vector<CDgnElement*>* pDgnElmList = where->second;

				CRWeldElement* pRWeldElm = new CRWeldElement();
				if(ERROR_SUCCESS == pRWeldElm->CollectElements( pDgnElmList , m_oIsoElementMap[_T("weld mark")] , ptConn ))
				{
					m_oIsoElementMap[_T("fitting")]->push_back( pRWeldElm );
				}
				else
				{
					SAFE_DELETE( pRWeldElm );
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-30
    @class      CIsoArea
    @function   FindConnectedElementWithAttribute
    @return     int
    @brief		지시선이 가리키는 PipeLine 요소를 찾는다.(pipe support , elbow , tee , pipe)
******************************************************************************/
int CIsoArea::CreateConnectedElementWithAttribute()
{
	/// find elbow , tee , valve , pipe support
	map<CString , vector<CIsoElement*>*>::iterator where = m_oIsoElementMap.find(_T("attribute"));
	if(where != m_oIsoElementMap.end())
	{
		vector<CIsoElement*>* pAttrElmList = where->second;
		for(vector<CIsoElement*>::iterator itr = pAttrElmList->begin();itr != pAttrElmList->end();++itr)
		{
			CAttributeElement* pAttrElm = static_cast<CAttributeElement*>(*itr);
			const CString sAttrType = pAttrElm->GetAttrType();
			if(_T("PIPE SUPPORT") == sAttrType)
			{
				CreatePipeSupportElementWith( pAttrElm );
			}
			else if(_T("ELBOW") == sAttrType)
			{
				CreateElbowElementWith( pAttrElm );
			}
			else if(_T("TEE") == sAttrType)
			{
				CreateTeeElementWith( pAttrElm );
			}
			else if(_T("OLET") == sAttrType)
			{
				CreateOletElementWith( pAttrElm );
			}
			else if(_T("REDUCER") == sAttrType)
			{
				CreateReducerElementWith( pAttrElm );
			}
			else if(_T("RWELD") == sAttrType)
			{
				CreateRWeldElementWith( pAttrElm );
			}
		}
		/// up to here
	}

	/// creat pipe element which has flow mark
	map<CString , vector<CDgnElement*>*>::iterator _where = m_oDgnElementMap.find(_T("pipe"));
	if(_where != m_oDgnElementMap.end())
	{
		vector<CDgnElement*>* pDgnElmList = _where->second;
		CombinePipeElementConnectedToFlowMark( pDgnElmList );
	}
	/// up to here

	_where = m_oDgnElementMap.find(_T("pipe"));
	if(_where != m_oDgnElementMap.end())
	{
		vector<CDgnElement*>* pDgnElmList = _where->second;

		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();/*++itr*/)
		{
			CPipeElement* pPipeElm = new CPipeElement();
			if(ERROR_SUCCESS == pPipeElm->Add( *itr ))
			{
				m_oIsoElementMap[_T("pipe")]->push_back( pPipeElm );

				itr = pDgnElmList->erase( itr );
			}
			else
			{
				SAFE_DELETE( pPipeElm );
				++itr;
			}
		}

		assert( pDgnElmList->empty() );
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-06
    @class      CIsoArea
    @function   CreatePipeElementToConnect
    @return     int
    @param      vector<CIsoElement*>*   pIsoElementList
    @brief
******************************************************************************/
int CIsoArea::CombinePipeElementConnectedToFlowMark(vector<CDgnElement*>* pDgnElmList)
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	if(pDgnElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dOffset = docData.GetOffset();

		vector<CDgnElement*> oFlowMarkElmList , *pGeneralElmList = NULL;
		map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("general"));
		if(m_oDgnElementMap.end() != where)
		{
			pGeneralElmList = where->second;
			for(vector<CDgnElement*>::iterator itr = pGeneralElmList->begin();itr != pGeneralElmList->end();++itr)
			{
				if((*itr)->IsKindOf(CDgnLineString::TypeString()))
				{
					CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
					/// work flow상 FlowMark를 vertex의 갯수로 구분했음...( weld mark는 앞에서 제거 되었음... )
					const int iVertexCount = pLineStringElm->GetVertexCount();
					if(iVertexCount > MIN_VERTEX_COUNT_FOR_FLOW_MARK) oFlowMarkElmList.push_back( *itr );
				}
			}
		}

		for(vector<CDgnElement*>::iterator itr = oFlowMarkElmList.begin();itr != oFlowMarkElmList.end();++itr)
		{
			CDgnVolume volume = (*itr)->volume();
			volume.Expand(dOffset , dOffset);

			vector<CDgnElement*> oConnectedElmList;
			for(vector<CDgnElement*>::iterator jtr = pDgnElmList->begin();jtr != pDgnElmList->end();++jtr)
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);
				if(2 == pLineStringElm->GetVertexCount())
				{
					DPoint3d pts[2] = {pLineStringElm->GetVertexAt(0) , pLineStringElm->GetVertexAt(1)};
					if(volume.Contains( pts[0] ) || volume.Contains( pts[1] ))
					{
						oConnectedElmList.push_back( *jtr );
					}
				}
			}

			if(2 == oConnectedElmList.size())
			{
				CPipeElement* pPipeElm = new CPipeElement( true );
				pPipeElm->m_oDgnElementList.push_back( oConnectedElmList[0] );
				pPipeElm->m_oDgnElementList.push_back( *itr );
				pPipeElm->m_oDgnElementList.push_back( oConnectedElmList[1] );
				pPipeElm->BuildConnPointList( oConnectedElmList[0] , oConnectedElmList[1] );

				m_oIsoElementMap[_T("pipe")]->push_back( pPipeElm );

				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oConnectedElmList[0]) );
				pGeneralElmList->erase( find(pGeneralElmList->begin() , pGeneralElmList->end() , *itr) );
				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oConnectedElmList[1]) );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      CIsoArea
    @function   CombinePipeElements
    @return     int
    @param      vector<CIsoElement*>*   pPipeElmList
    @brief
******************************************************************************/
int CIsoArea::CombinePipeElements(vector<CIsoElement*>* pPipeElmList)
{
	assert(pPipeElmList && "pPipeElmList is NULL");

	if(pPipeElmList)
	{
		vector<CIsoElement*> oPipeChacheList;
		bool bLoop = true;
		while(bLoop && !pPipeElmList->empty())
		{
			bLoop = false;

			for(vector<CIsoElement*>::iterator itr = pPipeElmList->begin();itr != pPipeElmList->end();++itr)
			{
				if((*itr)->IsKindOf(CPipeElement::TypeString()))
				{
					CPipeElement* pPipeElm = static_cast<CPipeElement*>(*itr);

					oPipeChacheList.push_back( *itr );
					pPipeElmList->erase(itr);

					pPipeElm->CollectConnectedPipe( pPipeElmList , m_oIsoElementMap[_T("fitting")] );
					bLoop = true;

					break;
				}
			}
		}

		pPipeElmList->insert(pPipeElmList->begin() , oPipeChacheList.begin() , oPipeChacheList.end());

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-21
    @class      CIsoArea
    @function   ChoosePipeToRoute
    @return     CPipeElement*
    @param      vector<CIsoElement*>*   pPipeElmList
    @brief		choose a pipe element to route(choose a pipe with flowmark if it exists)
******************************************************************************/
CPipeElement* CIsoArea::ChoosePipeToRoute(vector<CIsoElement*>* pPipeElmList)
{
	assert(pPipeElmList && "pPipeElmList is NULL");

	if(pPipeElmList && !pPipeElmList->empty())
	{
		for(vector<CIsoElement*>::iterator itr = pPipeElmList->begin();itr != pPipeElmList->end();++itr)
		{
			CPipeElement* pPipeElm = static_cast<CPipeElement*>(*itr);
			if(pPipeElm->HasFlowMark()) return pPipeElm;
		}

		return (CPipeElement*)(pPipeElmList->front());
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CIsoArea
    @function   CreatePipeRoutineForBranchElm
    @return     int
    @param      vector<CIsoElement*>*   pFittingElmList
    @brief		
******************************************************************************/
int CIsoArea::CreatePipeRoutineForBranchElm(vector<CIsoElement*>* pFittingElmList , vector<CIsoElement*>* pPipeElmList)
{
	assert(pFittingElmList && pPipeElmList && "pFittingElmList or pPipeElmList is NULL");

	if(pFittingElmList && !pFittingElmList->empty() && (NULL != pPipeElmList))
	{
		vector<CPipeRoutine*> oPipeRoutineList;
		vector<CIsoElement*> oBranchElmList;
	
		for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CTeeElement::TypeString()))
			{
				CTeeElement* pTeeElm = static_cast<CTeeElement*>(*itr);

				/// create branch routine for tee
				CPipeRoutine* pBranchRoutine = new CPipeRoutine(this , pTeeElm);
				m_oPipeRoutineList.push_back( pBranchRoutine );
				pTeeElm->SetBranchRoutine( pBranchRoutine );

				/// create main routine for tee
				/*CPipeRoutine* pMainRoutine = new CPipeRoutine(this , NULL);
				m_oPipeRoutineList.push_back( pMainRoutine );
				pTeeElm->SetMainRoutine( pMainRoutine );*/

				oBranchElmList.push_back( *itr );
			}
			else if((*itr)->IsKindOf(COletElement::TypeString()))
			{
				COletElement* pOletElm = static_cast<COletElement*>(*itr);

				CPipeRoutine* pPipeRoutine = new CPipeRoutine(this , pOletElm);
				m_oPipeRoutineList.push_back( pPipeRoutine );
				pOletElm->SetBranch( pPipeRoutine );

				oBranchElmList.push_back( *itr );
			}
			else if((*itr)->IsKindOf(CRWeldElement::TypeString()))
			{
				CRWeldElement* pRWeldElm = static_cast<CRWeldElement*>(*itr);

				CPipeRoutine* pPipeRoutine = new CPipeRoutine(this , pRWeldElm);
				m_oPipeRoutineList.push_back( pPipeRoutine );
				pRWeldElm->SetBranch( pPipeRoutine );

				oBranchElmList.push_back( *itr );
			}
		}

		///NOTICE: loop가 되면 안된다.... - 2011.12.06
		for(vector<CIsoElement*>::iterator itr = oBranchElmList.begin();itr != oBranchElmList.end();++itr)
		{
			if((*itr)->IsKindOf(CTeeElement::TypeString()))
			{
				CTeeElement* pTeeElm = static_cast<CTeeElement*>(*itr);
				CPipeRoutine* pPipeRoutine = pTeeElm->GetBranchRoutine();
				if(pPipeRoutine)
				{
					vector<DPoint3d> ptConnList;
					DPoint3d ptConn = pTeeElm->GetStartPointOfBranch();
					ptConnList.push_back( ptConn );
					pPipeRoutine->CollectConnectedPipe( ptConnList , NULL , pPipeElmList , pFittingElmList );
					if(0 == pPipeRoutine->GetPipeElmCount())
					{
						pPipeRoutine->m_ptConnList.push_back(pTeeElm->center());
						pPipeRoutine->m_ptConnList.push_back(ptConn);
					}
				}
			}
			else if((*itr)->IsKindOf(COletElement::TypeString()))
			{
				COletElement* pOletElm = static_cast<COletElement*>(*itr);
				CPipeRoutine* pPipeRoutine = pOletElm->GetBranch();
				if(pPipeRoutine)
				{
					vector<DPoint3d> ptConnList;
					DPoint3d ptConn = pOletElm->GetStartPointOfBranch();
					ptConnList.push_back( ptConn );
					pPipeRoutine->CollectConnectedPipe( ptConnList , NULL , pPipeElmList , pFittingElmList );
				}
			}
			else if((*itr)->IsKindOf(CRWeldElement::TypeString()))
			{
				CRWeldElement* pRWeldElm = static_cast<CRWeldElement*>(*itr);
				CPipeRoutine* pPipeRoutine = pRWeldElm->GetBranch();
				if(pPipeRoutine)
				{
					CIsoElement* pIsoElm = pRWeldElm->GetStartElmOfBranchFrom(pPipeElmList);
					if(NULL != pIsoElm)
					{
						vector<DPoint3d> oConnPtList;
						pRWeldElm->GetConnPointList(oConnPtList);
						DPoint3d ptCenter = oConnPtList[0];

						pIsoElm->GetConnPointList( oConnPtList );
						stable_sort( oConnPtList.begin() , oConnPtList.end() , CIsoElement::SortDPoint3dFrom( ptCenter ) );
						const DPoint3d ptOrigin = oConnPtList[oConnPtList.size() - 1];

						pPipeRoutine->Add( ptOrigin , pIsoElm );
						pPipeElmList->erase( find(pPipeElmList->begin() , pPipeElmList->end() , pIsoElm) );

						oConnPtList.clear();
						oConnPtList.push_back( ptOrigin );
						pPipeRoutine->CollectConnectedPipe( oConnPtList , NULL , pPipeElmList , pFittingElmList );
					}
				}
			}
		}
		/// up to here

		bool bLoop = true;
		while(bLoop)
		{
			bLoop = false;
			for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
			{
				if((*itr)->IsKindOf(CTeeElement::TypeString()))
				{
					CTeeElement* pTeeElm = static_cast<CTeeElement*>(*itr);

					/// create main routine for tee
					CPipeRoutine* pMainRoutine = new CPipeRoutine(this , NULL);
					if(pMainRoutine)
					{
						m_oPipeRoutineList.push_back( pMainRoutine );
						pTeeElm->SetMainRoutine( pMainRoutine );

						vector<DPoint3d> oConnPtList;
						pTeeElm->GetConnPointList( oConnPtList );
						pMainRoutine->CollectConnectedPipe(oConnPtList , pTeeElm , pPipeElmList , pFittingElmList );
						if(0 == pMainRoutine->GetPipeElmCount())
						{
							m_oPipeRoutineList.erase( find(m_oPipeRoutineList.begin() , m_oPipeRoutineList.end() , pMainRoutine) );
							SAFE_DELETE(pMainRoutine);
							pTeeElm->SetMainRoutine(NULL);
						}
					}

					bLoop = true;
					break;
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-30
    @class      CIsoArea
    @function   ConnectPipes
    @return     int
    @brief		create and connecte pipe
******************************************************************************/
int CIsoArea::ConnectPipes()
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

	vector<CIsoElement*>* pFittingElmList = NULL;
	map<CString , vector<CIsoElement*>*>::iterator where = m_oIsoElementMap.find(_T("pipe"));
	if((where != m_oIsoElementMap.end()) && !where->second->empty())
	{
		vector<CIsoElement*>* pPipeElmList = where->second;
				
		/// 연결할수 있는 pipe들을 연결한다.
		/// 이제 pipe와 tee , elbow , pipe support , olet , inline-component 만 남게된다.
		CombinePipeElements( pPipeElmList );
		
		/// branch쪽으로 먼저 pipe routine을 확장한다.
		pFittingElmList = m_oIsoElementMap[_T("fitting")];
		if(!pFittingElmList->empty())
		{
			CreatePipeRoutineForBranchElm(pFittingElmList , pPipeElmList);
		}
		/// up to here

		/// create pipe routines
		while(!pPipeElmList->empty())
		{
			CPipeRoutine* pPipeRoutine = new CPipeRoutine(this);
			if(pPipeRoutine)
			{
				m_oPipeRoutineList.push_back( pPipeRoutine );

				CPipeElement* pPipeElm = ChoosePipeToRoute(pPipeElmList);
				if(NULL != pPipeElm)
				{
					pPipeElmList->erase( find(pPipeElmList->begin() , pPipeElmList->end() , pPipeElm) );

					vector<DPoint3d> ptConnList;
					pPipeElm->GetConnPointList( ptConnList );
					pPipeRoutine->Add( ptConnList[0] , pPipeElm );

					if(ERROR_BAD_ENVIRONMENT == pPipeRoutine->CollectConnectedPipe( ptConnList , NULL , pPipeElmList , pFittingElmList ))
					{
						m_oPipeRoutineList.erase( find(m_oPipeRoutineList.begin() , m_oPipeRoutineList.end() , pPipeRoutine) );
						SAFE_DELETE( pPipeRoutine );
						break;
					}
				}
			}
		}

		/// create pipe routine for fitting. at this moment, there is no pipe line. - 2011.11.25 added by humkyung
		for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CElbowElement::TypeString()))
			{
				CPipeRoutine* pPipeRoutine = new CPipeRoutine(this);
				if(pPipeRoutine)
				{
					vector<DPoint3d> oConnPtList;
					(*itr)->GetConnPointList( oConnPtList );
					pPipeRoutine->Add(oConnPtList[0] , *itr);

					m_oPipeRoutineList.push_back( pPipeRoutine );
				}
			}
		}
		/// up to here - 2011.11.25
		/// up to here

		///NOTICE: loop되는 것은 없다고 가정...
		vector<CIsoElement*>* pWeldMarkElmList = m_oIsoElementMap[_T("weld mark")];
		while(GetDisconnectedPipeRoutineCount() > 1)
		{
			/// select two pipe routines which the distance is shortest
			double dMinDist = DBL_MAX;
			CPipeRoutine* oPipeRoutine[2] = {NULL,NULL};
			for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
			{
				if(NULL != (*itr)->GetStartElm()) continue;
				for(vector<CPipeRoutine* >::iterator jtr = m_oPipeRoutineList.begin();jtr != m_oPipeRoutineList.end();++jtr)
				{
					if(itr == jtr) continue;
					///if(NULL != (*jtr)->GetStartElm()) continue;
					const double d = (*itr)->DistanceToIfCollinear(*jtr);
					if(d < dMinDist)
					{
						dMinDist = d;
						oPipeRoutine[0] = *itr;
						oPipeRoutine[1] = *jtr;
					}
				}
			}
			/// up to here

			if((NULL != oPipeRoutine[0]) && (NULL != oPipeRoutine[1]))
			{
				oPipeRoutine[0]->Join(oPipeRoutine[1] , pWeldMarkElmList);
				m_oPipeRoutineList.erase( find(m_oPipeRoutineList.begin() , m_oPipeRoutineList.end() , oPipeRoutine[1]) );
				SAFE_DELETE( oPipeRoutine[1] );
			}
			else
			{
				/// maybe, error occurs!!! please don't reach this spot...
				break;
			}
		}

		/// delete pipe routine which has no pipe elment - 2011.12.05 added by humkyung
		//for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();/*++itr*/)
		//{
		//	if(0 == (*itr)->GetPipeElmCount())
		//	{
		//		SAFE_DELETE(*itr);
		//		itr = m_oPipeRoutineList.erase( itr );
		//		continue;
		//	}
		//	++itr;
		//}
		/// up to here

		/// 2011.11.10 added by humkyung
		for(vector<CPipeRoutine*>::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			if((*itr)->GetPipeElmCount() > 0)
			{
				(*itr)->AlignToCompare();
			}
		}
		/// up to here
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-19
    @class      CIsoArea
    @function   CollectPageConnectionElement
    @return     int
    @brief		collect page connection element
******************************************************************************/
int CIsoArea::CollectPageConnectionElement()
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

	/// parse page connection component
	vector<CDgnElement*> oTextElementList;
	vector<CDgnElement*>* pDgnElmList = m_oDgnElementMap[_T("general")];
	if(pDgnElmList)
	{
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				oTextElementList.push_back( *itr );
			}
		}
	}

	vector< CPageConnectionElement* > oPageConnList;
	for(vector<CDgnElement*>::iterator itr = oTextElementList.begin();itr != oTextElementList.end();)
	{
		CDgnText* pTextElm = static_cast<CDgnText*>((*itr));
		if(docData.IsPageConnectionString(pTextElm->textString()))
		{
			CPageConnectionElement* pPageConnElm = new CPageConnectionElement();
			pPageConnElm->Add( *itr );
			oPageConnList.push_back( pPageConnElm );
			
			pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , *itr) );
			itr = oTextElementList.erase(itr);
			continue;
		}
		++itr;
	}

	for(vector< CPageConnectionElement* >::iterator itr = oPageConnList.begin();itr != oPageConnList.end();++itr)
	{
		CDgnElement* pPageConnElm = (*itr)->DgnElementP(0);

		vector<CDgnElement*> _oTextElementList;
		for(vector<CDgnElement*>::iterator jtr = oTextElementList.begin();jtr != oTextElementList.end();++jtr)
		{
			if((pPageConnElm->origin().x == (*jtr)->origin().x) && ((*jtr)->origin().y < pPageConnElm->origin().y))
			{
				_oTextElementList.push_back( *jtr );
			}
		}

		stable_sort(_oTextElementList.begin() , _oTextElementList.end() , CIsoElement::SortDgnElmByDistanceFromDgnElm(pPageConnElm));
		const int iMaxCount = 8;
		for(int i = 0;(i < iMaxCount) && (i < int(_oTextElementList.size()));++i)
		{
			(*itr)->Add( _oTextElementList[i] );
			
			pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , _oTextElementList[i]) );

			CDgnText* pTextElm = static_cast<CDgnText*>(_oTextElementList[i]);
			if((0 == CString(pTextElm->textString()).Find(_T("EL."))) || (0 == CString(pTextElm->textString()).Find(_T("EL ")))) break;
		}

		m_oIsoElementMap[_T("attribute")]->push_back( *itr );
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-06
    @class      CIsoArea
    @function   RemoveDimArrows
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief		general/dimension 요소 리스트에서 arrow을 생성한 뒤 해당 요소들을 리스트에서 제거한다.
******************************************************************************/
int CIsoArea::CollectArrows(const CString& sCategory)
{
	map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find( sCategory );
	if(where != m_oDgnElementMap.end())
	{
		vector<CDgnElement*>* pDgnElmList = where->second;
		if(NULL == pDgnElmList) return ERROR_BAD_ENVIRONMENT;

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler;
		const int iArrowType = docData.GetArrowType();

		vector<CDgnLineString*> oCandidatedElmList;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();/*++itr*/)
		{	
			if(((*itr)->IsKindOf(CDgnLineString::TypeString())))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				vector<DPoint3d> oUniqueVertexList = pLineStringElm->GetPointStringExceptSamePoint();
				if(1 == iArrowType)
				{
					if(pLineStringElm->IsClosed() && (3 == oUniqueVertexList.size()))
					{
						oCandidatedElmList.push_back( pLineStringElm );
						itr = pDgnElmList->erase( itr );
						continue;
					}
				}
				else if(2 == iArrowType)
				{
					if(!pLineStringElm->IsClosed() && (3 == oUniqueVertexList.size()))
					{
						const double d1 = ::DistanceBetween(oUniqueVertexList[0] , oUniqueVertexList[1]);
						const double d2 = ::DistanceBetween(oUniqueVertexList[1] , oUniqueVertexList[2]);
						if(fabs(d1 - d2) < dToler)
						{
							oCandidatedElmList.push_back( pLineStringElm );
							itr = pDgnElmList->erase( itr );
							continue;
						}
					}
				}
			}

			++itr;
		}

		/// create arrow instance
		for(vector<CDgnLineString*>::iterator itr = oCandidatedElmList.begin();itr != oCandidatedElmList.end();++itr)
		{			
			CArrowElement* pArrowElm = new CArrowElement();
			pArrowElm->Add( *itr );
			CDgnVolume _volume1 = pArrowElm->volume();
			
			if(1 == iArrowType)
			{
				vector<DPoint3d> oEnclosePolygon = pArrowElm->GetEnclosePolygon();
				pArrowElm->ExpandEnclosePolygon( oEnclosePolygon);
				for(vector<CDgnElement*>::iterator jtr = pDgnElmList->begin();jtr != pDgnElmList->end();/*++jtr*/)
				{
					if((*jtr)->IsKindOf(CDgnLineString::TypeString()))
					{
						CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*jtr);
						CDgnVolume _volume2 = pLineStringElm->volume();
						if(_volume1.CollideWith( _volume2 ))
						{
							if(true == pArrowElm->IsLocatedInArrow( pLineStringElm , oEnclosePolygon))
							{
								pArrowElm->Add( *jtr );
								jtr = pDgnElmList->erase( jtr);
								continue;
							}
						}
					}
					++jtr;
				}
			}
			
			m_oIsoElementMap[_T("arrow")]->push_back(pArrowElm);
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-30
    @class      CIsoArea
    @function   ConnectPipes
    @return     int
    @brief		collect dimension
******************************************************************************/
int CIsoArea::CollectDimensions()
{
	map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(_T("dimension"));
	if((where != m_oDgnElementMap.end()) && !where->second->empty())
	{
		vector<CIsoElement*>* pDimElmList = m_oIsoElementMap[_T("dimension")];
		
		/// create dimension element
		vector<CDgnElement*>* pDgnElmList = where->second;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();/*++itr*/)
		{
			if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				CDimElement* pDimElm = new CDimElement();
				if(pDimElm)
				{
					pDimElm->Add( *itr );
					pDimElmList->push_back( pDimElm );
				}
				itr = pDgnElmList->erase( itr );
				continue;
			}
			++itr;
		}
		/// up to here

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dOffset = docData.GetOffset();

		vector<CIsoElement*>* pArrElmList = m_oIsoElementMap[_T("arrow")];
		/// get dimension line and extend line for dimension
		where = m_oDgnElementMap.find(_T("general"));
		if((where != m_oDgnElementMap.end()) && !where->second->empty())
		{
			vector<CDgnElement*>* pDgnElmList = where->second;

			vector<CIsoElement*> oTempDimElmList;
			for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
			{
				CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
				vector<CDgnElement*> oConnectedElmList = pDimElm->GetLineListConnectedToDimText( pDgnElmList , dOffset , dOffset*0.5);
				if((1 == oConnectedElmList.size()) || (2 == oConnectedElmList.size()))
				{
					pDimElm->CollectDimLines( pDgnElmList , pArrElmList);
				}
				else
				{
					oTempDimElmList.push_back(*itr);
				}
			}
			{
				for(vector<CIsoElement*>::iterator itr = oTempDimElmList.begin();itr != oTempDimElmList.end();++itr)
				{
					CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
					pDimElm->CollectDimLines( pDgnElmList , pArrElmList);
				}
			}

			for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
			{
				CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
				pDimElm->CollectExtendLines( pDgnElmList , pArrElmList );
			}
		}
		/// up to here

		/// remove dgn element for extend dim line from general dgn element list - 2011.12.22 added by humkyung
		for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
		{
			CDimElement* pDimElm = static_cast<CDimElement*>(*itr);

			CDimExtLineElement* pDimExtLine = pDimElm->GetExtLineElement();
			if(NULL != pDimExtLine)
			{
				vector<CDgnElement*>* pDgnElmList = m_oDgnElementMap[_T("general")];
				for(vector<CDgnElement*>::iterator jtr = pDimExtLine->m_oDgnElementList.begin();jtr != pDimExtLine->m_oDgnElementList.end();++jtr)
				{
					vector<CDgnElement*>::iterator where = find(pDgnElmList->begin() , pDgnElmList->end() , *jtr);
					if(where != pDgnElmList->end())
					{
						pDgnElmList->erase(where);
					}
				}
			}
		}
		/// up to here
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CIsoArea
    @function   GetDisconnectedPipeRoutineCount
    @return     int
    @brief		return the count of disconnected pipe routine
******************************************************************************/
int CIsoArea::GetDisconnectedPipeRoutineCount()
{
	int iCount = 0;
	for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
	{
		if(NULL == (*itr)->GetStartElm()) ++iCount;
	}

	return iCount;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-30
    @class      CIsoArea
    @function   ConnectPipes
    @return     int
    @brief		assign dimension to pipe
******************************************************************************/
int CIsoArea::AssignDimensionToPipe()
{
	vector<CPipeRoutine::CPipeRun*> oPipeRunList;
	
	/// create pipe run for slope area mark - 2011.12.13 added by humkyung
	{
		vector<CIsoElement*>* pSlopeAreaMarkElmList = m_oIsoElementMap[_T("slope area mark")];
		for(vector<CIsoElement*>::iterator itr = pSlopeAreaMarkElmList->begin();itr != pSlopeAreaMarkElmList->end();++itr)
		{
			CSlopeAreaMarkElement* pSloeAreaMarkElm = static_cast<CSlopeAreaMarkElement*>(*itr);

			pSloeAreaMarkElm->CreatePipeRun();
			vector<CPipeRoutine::CPipeRun*> _oPipeRunList;
			pSloeAreaMarkElm->GetPipeRunList( _oPipeRunList );
			oPipeRunList.insert( oPipeRunList.end() , _oPipeRunList.begin() , _oPipeRunList.end() );
		}
	}
	/// up to here

	for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
	{
		(*itr)->CreatePipeRun();
		vector<CPipeRoutine::CPipeRun*> _oPipeRunList;
		(*itr)->GetPipeRunList( _oPipeRunList );
		oPipeRunList.insert( oPipeRunList.end() , _oPipeRunList.begin() , _oPipeRunList.end() );
	}

	map<CString , vector<CIsoElement*>*>::iterator where =  m_oIsoElementMap.find(_T("dimension"));
	if(m_oIsoElementMap.end() != where)
	{
		vector<CIsoElement*>* pDimElmList = where->second;
		for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
		{
			if(!(*itr)->IsKindOf(CDimElement::TypeString())) continue;
			
			CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
			pDimElm->FindConnectedPipeRunFrom( oPipeRunList );
		}

		/// assign dim. to pipe run for not connected dim. - 2011.12.05 added by humkyung
		bool bLoop = true;
		while(bLoop)
		{
			bLoop = false;
			for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
			{
				CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
				if(NULL == pDimElm->GetConnectedPipeRun())
				{
					pDimElm->FindConnectedPipeRunFrom(pDimElmList);
					if(NULL != pDimElm->GetConnectedPipeRun())
					{
						bLoop = true;
						break;
					}
				}
			}
		}
		/// up to here

		for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
		{
			(*itr)->AssignDimension( pDimElmList );
		}
	}

	return ERROR_SUCCESS;
}

int CIsoArea::Check()
{
	int iColor = 2;

	for(map<CString , vector<CIsoElement*>*>::iterator itr = m_oIsoElementMap.begin();itr != m_oIsoElementMap.end();++itr)
	{
		///if((_T("arrow") == itr->first) || (_T("pipe") == itr->first) || (_T("dimension") == itr->first)) continue;
		for(vector<CIsoElement*>::iterator jtr = itr->second->begin();jtr != itr->second->end();++jtr)
		{
			CString sColor;
			sColor.Format (_T("%d") , iColor++);
			(*jtr)->Check(m_pIsoModel->GetDgnDocument() , sColor);
		}
	}

	for(vector<CPipeRoutine* >::iterator itr = m_oPipeRoutineList.begin();itr != m_oPipeRoutineList.end();++itr)
	{
		CString sColor;
		sColor.Format (_T("%d") , iColor++);
		(*itr)->Check( m_pIsoModel->GetDgnDocument() , sColor );
	}

	return 0;
}
