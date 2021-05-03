#include "StdAfx.h"
#include <DgnLineString.h>
#include "IsoModelDocData.h"
#include "DimExtLineElement.h"

using namespace IsoElement;

CDimExtLineElement::CDimExtLineElement()
{
}

CDimExtLineElement::~CDimExtLineElement(void)
{
}

void CDimExtLineElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	CIsoElement::Check( pDgnDoc , sColor );

	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		if((*itr)->IsKindOf(CDgnLineString::TypeString()))
		{
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
			const int iVertexCount = pLineStringElm->GetVertexCount();
			
			vector<DPoint3d> pts;
			DPoint3d pt = pLineStringElm->GetVertexAt( iVertexCount - 1 );
			pt.x += 0.01;
			pt.y += 0.01;
			pts.push_back( pt );

			pt = pLineStringElm->GetVertexAt( iVertexCount - 1 );
			pt.x -= 0.01;
			pt.y -= 0.01;
			pts.push_back( pt );

			pts.push_back( pLineStringElm->GetVertexAt( iVertexCount - 1 ) );

			pt = pLineStringElm->GetVertexAt( iVertexCount - 1 );
			pt.x -= 0.01;
			pt.y += 0.01;
			pts.push_back( pt );

			pt = pLineStringElm->GetVertexAt( iVertexCount - 1 );
			pt.x += 0.01;
			pt.y -= 0.01;
			pts.push_back( pt );

			::DrawPolyline( pDgnDoc , pts , sColor);
		}
	}
}