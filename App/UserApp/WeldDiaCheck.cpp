// WeldDiaCheck.cpp: implementation of the CWeldDiaCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "UserApp.h"
#include "WeldDiaCheck.h"

#include <algorithm>
#include <fstream>
#include <functional>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeldDiaCheck::CWeldDiaCheck()
{
}

CWeldDiaCheck::~CWeldDiaCheck()
{

}

/**
**/
void CWeldDiaCheck::InitInternalVariables()
{
	memset(m_nTotal_Sum,'\0',sizeof(double)*6*4);
	m_specSizeTotalEntry[0].clear();
	m_specSizeTotalEntry[1].clear();
	m_TotalSpecTypeEntry.clear();
}

namespace
{

	struct IsSpecSizeTotal : public binary_function<SpecSizeTotal , double , bool>
	{
		bool operator ()(const SpecSizeTotal& lhs , const double& size) const
		{
			return (fabs(lhs.size - size) < 0.001);
		}
	};
}

/**
**/
void CWeldDiaCheck::Add(const CWeldDetailBMFile& weld_detail)
{
	TotalSpecValue TempTotalSpecValue;
	memcpy(TempTotalSpecValue.m_nTotalSpec , weld_detail.m_nSub_Summary_By_Material , sizeof(double) * 6 * 4);
	CString rFileName = weld_detail.m_rFileName;
	int at = rFileName.Find("]");
	rFileName = rFileName.Right(rFileName.GetLength() - at - 2 );
	m_TotalSpecTypeEntry[rFileName] = TempTotalSpecValue;
	
	for(int i = 1 ; i < 6 ; ++i )
	{
		for(int j = 0 ; j < 4 ; ++j )
		{
			m_nTotal_Sum[i][j] += weld_detail.m_nSub_Summary_By_Material[i][j];
		}
	}
		
	string rSpec = weld_detail.m_rSpec.operator const char*();
	
	//////////////////////////////////////////////////////////////////////////
	/// BW, SW, TW'S TOTAL WELDING DIA BY SPEC AND SIZE
	map<string , vector<SpecSizeTotal> >::iterator where = m_specSizeTotalEntry[0].find(rSpec);
	if(m_specSizeTotalEntry[0].end() != where)
	{
		for(vector<SpecSizeTotal>::const_iterator itr = weld_detail.m_specSizeTotalEntry[0].begin();itr != weld_detail.m_specSizeTotalEntry[0].end();++itr)
		{
			vector<SpecSizeTotal>::iterator at = find_if(where->second.begin() , where->second.end() , bind2nd(IsSpecSizeTotal() , itr->size));
			if(where->second.end() != at)
			{
				at->total += itr->total;
			}
			else
			{
				where->second.push_back(*itr);
				m_sizeEntry[0].insert(itr->size);
			}
		}
	}
	else
	{
		
		for(vector<SpecSizeTotal>::const_iterator itr = weld_detail.m_specSizeTotalEntry[0].begin();itr != weld_detail.m_specSizeTotalEntry[0].end();++itr)
		{
			m_specSizeTotalEntry[0][rSpec].push_back(*itr);
			m_sizeEntry[0].insert(itr->size);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	/// FW TOTAL WELDING DIA BY SPEC AND SIZE
	where = m_specSizeTotalEntry[1].find(rSpec);
	if(m_specSizeTotalEntry[1].end() != where)
	{
		for(vector<SpecSizeTotal>::const_iterator itr = weld_detail.m_specSizeTotalEntry[1].begin();itr != weld_detail.m_specSizeTotalEntry[1].end();++itr)
		{
			vector<SpecSizeTotal>::iterator at = find_if(where->second.begin() , where->second.end() , bind2nd(IsSpecSizeTotal() , itr->size));
			if(where->second.end() != at)
			{
				at->total += itr->total;
			}
			else
			{
				where->second.push_back(*itr);
				m_sizeEntry[1].insert(itr->size);
			}
		}
	}
	else
	{
		for(vector<SpecSizeTotal>::const_iterator itr = weld_detail.m_specSizeTotalEntry[1].begin();itr != weld_detail.m_specSizeTotalEntry[1].end();++itr)
		{
			m_specSizeTotalEntry[1][rSpec].push_back(*itr);
			m_sizeEntry[1].insert(itr->size);
		}
	}
}

namespace
{
	bool __SortBySize(const SpecSizeTotal& lhs , const SpecSizeTotal& rhs)
	{
		return (lhs.size < rhs.size);
	}
}

/**	\brief	WELD DIA CHECK 파일을 생성한다.
	\author	백흠경
**/
bool CWeldDiaCheck::Write(const CString &rOuputFilePath)
{
	ofstream ofile(rOuputFilePath);
	if(ofile.is_open() && ofile.good())
	{
		//////////////////////////////////////////////////////////////////////////
		/// TOTAL TYPE
		CString rString;
		ofile << "========================================================================================================================================================================================================================================" << std::endl;
                ofile << "              B/M FILE(S)  DRAWING NO.    CARBON STEEL                        ALLOY STEEL                         STAINLESS STEEL                     GALV.                               OTHER                               TOTAL     " << std::endl;
		ofile << "                                          BW          SW          TW          BW          SW          TW          BW          SW          TW          BW          SW          TW          BW          SW          TW          BW+SW+TW  " << std::endl;
		ofile << "========================================================================================================================================================================================================================================" << std::endl;
		CString rTypeString , rBSTWString, rFWString , rWTotalString;
		int nCount = 1;
		int nTotalCount = m_TotalSpecTypeEntry.size();
		for(map<CString, TotalSpecValue>::iterator itr = m_TotalSpecTypeEntry.begin(); itr != m_TotalSpecTypeEntry.end(); ++itr)
		{
			double nBSTWTOtal = 0.f;
			rString.Format(" [%-4d/%4d] %-28s" , nCount ,nTotalCount, itr->first);		
			for(int i = 1; i < 6 ;++i)
			{	
				for(int j = 0; j < 3; ++j)
				{
					nBSTWTOtal += itr->second.m_nTotalSpec[i][j];
					rTypeString.Format(" %-10.3f " ,itr->second.m_nTotalSpec[i][j]);
					rString += rTypeString;
				}
			}
			rBSTWString.Format(" %-10.3f", nBSTWTOtal);
			rString += rBSTWString;
			ofile << rString.operator const char*() << std::endl;
			nCount++;
		}
		//////////////////////////////////////////////////////////////////////////
		/// TOTAL SUM WELDING DIA BY MAT.'s TYPE
		ofile << std::endl;
		ofile << std::endl;
		ofile << std::endl;
		ofile << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		ofile << "  *** TOTAL WELDING DIA BY MATERIAL TYPE ***             " << std::endl;
		ofile << "                     BW         SW         TW         TOTAL" << std::endl;	                    
		rString.Format(" CARBON STEEL    :   %-10.3f %-10.3f %-10.3f %-10.3f\n", 
			m_nTotal_Sum[1][0], m_nTotal_Sum[1][1], m_nTotal_Sum[1][2], m_nTotal_Sum[1][0] + m_nTotal_Sum[1][1] + m_nTotal_Sum[1][2] );
		ofile << rString.operator const char*() << std::endl;

		rString.Format(" ALLOY STEEL     :   %-10.3f %-10.3f %-10.3f %-10.3f\n", 
			m_nTotal_Sum[2][0], m_nTotal_Sum[2][1], m_nTotal_Sum[2][2], m_nTotal_Sum[2][0] + m_nTotal_Sum[2][1] + m_nTotal_Sum[2][2] );
		ofile << rString.operator const char*() << std::endl;

		rString.Format(" STAINLESS STEEL :   %-10.3f %-10.3f %-10.3f %-10.3f\n", 
			m_nTotal_Sum[3][0], m_nTotal_Sum[3][1], m_nTotal_Sum[3][2], m_nTotal_Sum[3][0] + m_nTotal_Sum[3][1] + m_nTotal_Sum[3][2] );
		ofile << rString.operator const char*() << std::endl;

		rString.Format(" GALV. STEEL     :   %-10.3f %-10.3f %-10.3f %-10.3f\n", 
			m_nTotal_Sum[5][0], m_nTotal_Sum[5][1], m_nTotal_Sum[5][2], m_nTotal_Sum[5][0] + m_nTotal_Sum[5][1] + m_nTotal_Sum[5][2] );
		ofile << rString.operator const char*() << std::endl;

		rString.Format(" ETC             :   %-10.3f %-10.3f %-10.3f %-10.3f\n", 
			m_nTotal_Sum[4][0], m_nTotal_Sum[4][1], m_nTotal_Sum[4][2], m_nTotal_Sum[4][0] + m_nTotal_Sum[4][1] + m_nTotal_Sum[4][2] );
		ofile << rString.operator const char*() << std::endl;

		rString.Format(" TOTAL           :   %-10.3f %-10.3f %-10.3f %-10.3f\n\n", 
			m_nTotal_Sum[1][0] + m_nTotal_Sum[2][0] + m_nTotal_Sum[3][0] + m_nTotal_Sum[4][0] + m_nTotal_Sum[5][0],
			m_nTotal_Sum[1][1] + m_nTotal_Sum[2][1] + m_nTotal_Sum[3][1] + m_nTotal_Sum[4][1] + m_nTotal_Sum[5][1],
			m_nTotal_Sum[1][2] + m_nTotal_Sum[2][2] + m_nTotal_Sum[3][2] + m_nTotal_Sum[4][2] + m_nTotal_Sum[5][2],    
		
			m_nTotal_Sum[1][0] + m_nTotal_Sum[2][0] + m_nTotal_Sum[3][0] + m_nTotal_Sum[4][0] + m_nTotal_Sum[5][0] +
			m_nTotal_Sum[1][1] + m_nTotal_Sum[2][1] + m_nTotal_Sum[3][1] + m_nTotal_Sum[4][1] + m_nTotal_Sum[5][1] +
			m_nTotal_Sum[1][2] + m_nTotal_Sum[2][2] + m_nTotal_Sum[3][2] + m_nTotal_Sum[4][2] + m_nTotal_Sum[5][2]
		);
		ofile << rString.operator const char*() << std::endl;
		
		ofile << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		
		static const CString rHeaders[2] = {"  *** BW, SW, TW'S TOTAL WELDING DIA BY SPEC AND SIZE ***  " , "  *** FW'S TOTAL WELDING DIA BY SPEC AND SIZE ***  "};
		//////////////////////////////////////////////////////////////////////////
		// BW, SW, TW'S TOTAL WELDING DIA BY SPEC AND SIZE
		for(int x = 0;x < 1;++x)
		{
			ofile << std::endl;
		ofile << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
			ofile <<  rHeaders[x].operator LPCTSTR() << std::endl;
		ofile << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
			
			if(!m_specSizeTotalEntry[x].empty())
			{
				int nTNum=0,nVNum=0;;
				rString = " SPEC_NAME SIZE       ";
				CString rSizeString, rTotalString;
				set<double>::iterator itr = m_sizeEntry[x].begin() ;
				for(itr = m_sizeEntry[x].begin() ; itr != m_sizeEntry[x].end() ; ++itr ,++nTNum)
				{ 
					rSizeString.Format("%-7.3lf" , *itr);
					rSizeString.Format("%-10s" , rSizeString);
					rString += rSizeString;
				}
				rString += "TOTAL";
				ofile << rString.operator const char*() << std::endl;
				
				for(map<string , vector<SpecSizeTotal> >::iterator jtr = m_specSizeTotalEntry[x].begin() ; jtr != m_specSizeTotalEntry[x].end() ; ++jtr)
				{
					double nTotal=0;
					rString.Format("       %-15s" , jtr->first.c_str());
					
					stable_sort(jtr->second.begin() , jtr->second.end() , __SortBySize);
					
					int prev_index = 0;
					for(vector<SpecSizeTotal>::iterator ktr = jtr->second.begin() ; ktr != jtr->second.end() ; ++ktr,++nVNum)
					{ 
						nTotal += ktr->total;
						
						int index = 0;
						for( itr = m_sizeEntry[x].begin() ; itr != m_sizeEntry[x].end() ; ++itr,++index)
						{
							if((*itr) == ktr->size) break;
						}
						/// padding
						for(int i = prev_index; i < index ;++i)
						{
							rTotalString.Format("%-10s" , "   ");
							rString += rTotalString;
						}
						prev_index = index + 1;

						rTotalString.Format("%-7.3lf" , ktr->total);
						rTotalString.Format("%-10s" , rTotalString);
						rString += rTotalString;
					}

					/// padding
					for(int k=prev_index;k<nTNum;++k)
					{
						rTotalString.Format("%-10s" , "   ");
						rString += rTotalString;	
					}
					ofile << rString.operator const char*();

					rTotalString.Format("%-7.3lf" , nTotal);
					rString = rTotalString;
					ofile << rString.operator const char*() << std::endl;
					nVNum=0;
				}
			}
			
		ofile << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		}
		ofile.close();

		return true;
	}

	return false;
}

/**
**/
void CWeldDiaCheck::Clear()
{
	InitInternalVariables();
}
