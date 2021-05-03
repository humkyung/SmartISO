// IsConvexHull.h: interface for the CConvexHull class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISCONVEXHULL_H__8246B9CF_ACAB_4226_A5D3_2A44520CAD77__INCLUDED_)
#define AFX_ISCONVEXHULL_H__8246B9CF_ACAB_4226_A5D3_2A44520CAD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConvexHull  
{
public:
	vector<DPoint3d> Create(const vector<DPoint3d>& poly2d);
	CConvexHull();
	virtual ~CConvexHull();

private:
	void GrahamScan(vector<DPoint3d>& poly2d);
	void AngularSort(vector<DPoint3d>& poly2d);

	vector<DPoint3d> m_pts1;
	vector<DPoint3d> m_convexhull;
};

#endif // !defined(AFX_ISCONVEXHULL_H__8246B9CF_ACAB_4226_A5D3_2A44520CAD77__INCLUDED_)
