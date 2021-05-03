// IsConvexHull.cpp: implementation of the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConvexHull.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvexHull::CConvexHull()
{

}

CConvexHull::~CConvexHull()
{

}

/**	
	@brief	2s 폴리라인에 대한 컨벡스홀을 구한다.

	@param	poly2d	a parameter of type CIsPoly2d

	@return	CIsPoly2d	
*/
vector<DPoint3d> CConvexHull::Create(const vector<DPoint3d>& poly2d )
{
	vector<DPoint3d> _poly2d_;
	_poly2d_.insert(_poly2d_.begin() , poly2d.begin() , poly2d.end());

	AngularSort(_poly2d_);
	GrahamScan(_poly2d_);

	return m_convexhull;
}

static double LEFTSIDE(DPoint3d P1, DPoint3d P2, DPoint3d P3) 
{
	return ((P2.x - P1.x) * (P3.y - P1.y) - (P3.x - P1.x) * (P2.y - P1.y)); 
}

/**	
	@brief	The CConvexHull::AngularSort function

	\param	poly2d	a parameter of type CIsPoly2d&

	\return	void
*/
void CConvexHull::AngularSort(vector<DPoint3d>& poly2d)
{
	int vertices = 0;
	if((vertices = poly2d.size()) < 2) return ;

	DPoint3d *Q = new DPoint3d[vertices];
	
	int id = 0 , i = 0;
	
	DPoint3d pt = poly2d[0];
	for(i = 1; i < vertices; i++)
	{
		if(pt.y > poly2d[i].y)
		{
			pt = poly2d[i];
			id = i;
		}//lowest--y-value;
	}
	
	for(i = 0; i < vertices; i++)
	{
		if((poly2d[i].y == pt.y) && (poly2d[i].x > pt.x))
		{
			pt = poly2d[i];
			id = i;
		}//largest--x-value;
	}

	//swap;
	poly2d[id] = poly2d[0];
	poly2d[0]  = pt;
	//done! finding lowest y and largest x;
	//angular sorting...
	Q[0] = poly2d[0] ;
	Q[1] = poly2d[1] ;
	int k= 2; 
	for(i= 2; i < vertices; i++)
	{
		double ccw = LEFTSIDE(Q[0],Q[k-1], poly2d[i]);
		if(ccw>0)
		{ //LEFT-side;
			Q[k]=poly2d[i]; k++;
		}
		else if (ccw==0) 
		{ //on the line==>먼점을 선택;
			if(::DistanceBetween(poly2d[i] , Q[0]) > ::DistanceBetween(Q[k-1] , Q[0]))
			{
				Q[k-1] = poly2d[i];
			}
			//만약 일직선상의 점들을 모두 보전하고자 하면, 거리가 먼순으로 정렬하고.
			//scan시 ccw==0인 경우도 포함해야 한다.
		}
		else
		{
			int j = 0;
			for(j = k - 2; j > 0; j--)
			{
				ccw = LEFTSIDE(Q[0],Q[j],poly2d[i]);
				if(ccw>0)
				{
					for(int l = k; l > (j + 1) ; l--)
					{
						Q[l] = Q[l-1];
					}
					Q[j+1] = poly2d[i];
					k++;
					break;
				}
				else if(0 == ccw)
				{
					if(::DistanceBetween(Q[0] , poly2d[i]) > ::DistanceBetween(Q[0] , Q[j]))
					{
						Q[j] = poly2d[i];
						break;
					}
				}
			}

			if(0 == j && ccw < 0)
			{
				for(int l = k; l > 1; l--)
				{
					Q[l] = Q[l-1];
				}
				Q[1] = poly2d[i];
				k++;
			}
		}
	}
	
	//ASSERT(k==N);
	m_pts1.clear();
	for(i = 0; i < k; i++)
	{
		m_pts1.push_back(Q[i]);
	}

	delete []Q; 
}

/**	\brief	The CConvexHull::GrahamScan function

	\param	poly2d	a parameter of type CIsPoly2d&

	\return	void

	
*/
void CConvexHull::GrahamScan(vector<DPoint3d>& poly2d)
{
	if(poly2d.size() < 2) return;
	m_convexhull.clear();		//remove relics;
	
	int N = m_pts1.size();
	//CPoint *P=m_vecPts1.GetData();
	DPoint3d *Q = new DPoint3d[N] ;
	
	Q[0] = m_pts1[0];
	Q[1] = m_pts1[1];
	int k=2;
	int i=2;
	while(i < N) 
	{
		double ccw = LEFTSIDE(Q[k-2],Q[k-1],m_pts1[i]) ;
		if(ccw > 0)
		{
			//일직선상의 점들을 모두 보전하고 싶으면, 일직선상에 있는 경우에 기점에서
			//거리 순서대로 정렬을 하고, ccw==0인경우도 넣어야한다.
			Q[k]=m_pts1[i]; k++ ; i++;
		}
		else
		{
			k--;
		}
	}
	
	for(i=0; i < k; i++)
	{
		m_convexhull.push_back(Q[i]);
	}

	delete[] Q; 
}
