// WeldDetailBMFile.cpp: implementation of the CWeldDetailBMFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "UserApp.h"
#include "UserAppDocData.h"
#include <MaterialCodeTable.h>
#include "WeldDetailBMFile.h"
#include "WeldDiaCheck.h"
#include "BMReportSettingPage.h"

#include <algorithm>
#include <functional>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	FLANGE_ITEM_GROUP_STRING		"2"
#define	FITTING_ITEM_GROUP_STRING		"4"
#define	PIPE_ITEM_GROUP_STRING			"5"
#define	REINFORCING_PAD_ITEM_GROUP_STRING	"6"
#define	TUBE_TUBE_FITTING_ITEM_GROUP_STRING	"7"
#define	VALVE_ITEM_GROUP_STRING			"8"
#define	MISC_ITEM_GROUP_STRING			"9"

/// check length of string - 2011.09.28 added by humkyung
CString MATL_CODE_LOC_4(const CString CodeString)
{
	return ((CodeString.GetLength() >= 4) ? CString(CodeString.GetAt(3)) : _T(""));
}
/// up to here

#define	BW	0
#define	SW	1
#define	TW	2
#define	FW	3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeldDetailBMFile::CWeldDetailBMFile()
{
}

CWeldDetailBMFile::~CWeldDetailBMFile()
{

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
	@brief	

	@author	조재호
*/
void CWeldDetailBMFile::Add_Welding_SpecSize_Total(const int& weld_type , const double& f , const double& qty , const double& size)
{
	if( 3 != weld_type )
	{
		vector<SpecSizeTotal>::iterator where = find_if(m_specSizeTotalEntry[0].begin() , m_specSizeTotalEntry[0].end() , bind2nd(IsSpecSizeTotal() , size));
		if(m_specSizeTotalEntry[0].end() != where)
		{
			where->total += f * qty * 0.5;
		}
		else
		{
			SpecSizeTotal spec_size_total;
			spec_size_total.size = size;
			spec_size_total.total= f * qty * 0.5;
				
			m_specSizeTotalEntry[0].push_back(spec_size_total);
		}
	}
	else
	{
		vector<SpecSizeTotal>::iterator where = find_if(m_specSizeTotalEntry[1].begin() , m_specSizeTotalEntry[1].end() , bind2nd(IsSpecSizeTotal() , size));
		if(m_specSizeTotalEntry[1].end() != where)
		{
			where->total += f * qty * 0.5;
		}
		else
		{
			SpecSizeTotal spec_size_total;
			spec_size_total.size = size;
			spec_size_total.total= f * qty * 0.5;
				
			m_specSizeTotalEntry[1].push_back(spec_size_total);
		}
	}
}

/**
	@brief	
	@author	백흠경
*/
void CWeldDetailBMFile::Add(const CString& rHeader , CQCCheck& qcCheck)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CMaterialCodeTable* pMaterialCodeTable = pDocData->GetProject()->GetMaterialCodeTable();
	CWeldDiaTable* pWeldDiaTable = pDocData->GetProject()->GetWeldDiaTable();
	const size_t nWeldDiaCount = pWeldDiaTable->GetWeldDiaCount();
	
	InitInternalVariables();
	
	/// collect all iso bm data
	vector<CIsoBMData> isoBMDataEntry;
	isoBMDataEntry.insert(isoBMDataEntry.end() , qcCheck.m_pipeDatas.begin() , qcCheck.m_pipeDatas.end());
	isoBMDataEntry.insert(isoBMDataEntry.end() , qcCheck.m_generalDatas.begin() , qcCheck.m_generalDatas.end());
	/// isoBMDataEntry.insert(isoBMDataEntry.end() , qcCheck.m_supportDatas.begin() , qcCheck.m_supportDatas.end());

	const long nTotalCount = isoBMDataEntry.size();
	long nIndex = 0;

	CString rString(rHeader);
	m_rFileName = rHeader;
	m_weldDetailEntry.push_back(rString);
	rString.Format("%s" , "          1) WELDING DIA CALCULATION                                                                                                            ");
	m_weldDetailEntry.push_back(rString);
	rString.Format("%s" , "             BW, SW, TW'S WELDING DIA CALCULATION = SIZE * (MAIN OR SUB)* QTY / 2                                                                           ");
	m_weldDetailEntry.push_back(rString);
	rString.Format("                 %-16s %-6s %-16s %-16s %-6s %-6s %-16s %-16s %-16s %-16s" , "MATERIAL CODE" , "SPEC" , "SIZE" , "MATRIAL TYPE" , "WT" , "Qty" , "TOTAL" , "MWP" , "SWP" , "CALCULATION");
	m_weldDetailEntry.push_back(rString);
	
	int nCount = 0;
	for(vector<CIsoBMData>::iterator itr = isoBMDataEntry.begin();itr != isoBMDataEntry.end();++itr)
	{
		try
		{
			m_rSpec = itr->matlSpec();
			CString rCode1 , rCode2;
			const CString rMatlCode = itr->matlCode();

			double size_11 = atof(itr->GetMainSize());
			double size_12 = (!itr->GetSubSize().IsEmpty()) ? atof(itr->GetSubSize()) : 0.f;

			if(0 == size_12) size_12 = size_11;
			const double Qty = atof(itr->QTY());

			/// 먼저 GALV인지를 위해 MAT'L NUM을 구하도록 한다.
			int nMatlCodeNum = -1;
			int checkCodeNum = CheckMatlCodeNumForGALV(rCode1 , rCode2 , rMatlCode);
			if( -1 == checkCodeNum )
			{
				nMatlCodeNum = pMaterialCodeTable->FindMatlNumForGALV( rCode1 , rCode2 );
			}

			if( -1 == nMatlCodeNum )
			{
				/// GALV가 아닐때...
				/// MAT'L CODE STRING에서 ITEM GROUP과 MAT'L CODE STRING에서 4번째에 위치한 CODE STRING을 구한다.
				checkCodeNum = CheckMatlCodeNum(rCode1 , rCode2 , rMatlCode);
				if(-1 == checkCodeNum )
				{
					nMatlCodeNum = pMaterialCodeTable->FindMatlNumWithGroupAndCodeLoc4(rCode1 , rCode2);
				}
			}		

			if ( -1 == checkCodeNum ) /// IF NOT THE CASE 'A' 'B' 'G'
			{
				if(-1 == nMatlCodeNum) continue;	///< MAT'L CODE NUM를 구할 수 없을 때

				/// T_WELD_DIA TABLE에서 WELD DIA RECORD를 구한다.
				WeldDia* pWeldDia = FindWeldDia(rMatlCode);
				if(NULL == pWeldDia) continue;

				/// MATL CODE NUM에서 MATL TYPE 문자열을 구한다.
				CString rMatlType = GetMatlType(nMatlCodeNum);
				if( rMatlType.Find( _T("ETC") ) > -1 ) nMatlCodeNum = 4;

				CString rSizeString;
				/// PCD 파일에서 WELD TYPE을 구한다.

				CString rWeldType = pDocData->GetWeldType(itr->matlSpec() , rMatlCode , itr->bmcsSYM());

				if( _T("NO") == rWeldType )
				{
					if( !itr->GetMainSize().IsEmpty() && !itr->GetSubSize().IsEmpty() )
						rSizeString.Format( _T("%sX%s") ,  itr->GetMainSize() , itr->GetSubSize());
					else    rSizeString.Format( _T("%s") , itr->GetMainSize());
					
					rString.Format(_T("          1-%-2d ) %-16s %-6s %-16s %-16s %-6s %-6s") , ++nCount , itr->matlCode() , itr->matlSpec() , rSizeString , rMatlType , rWeldType , itr->bmcsSYM() + " WELDING TYPE NOT DEFINED");
					m_weldDetailEntry.push_back(rString);
					continue;
				}
				else if( _T("TF") == rWeldType )
				{
					if( !itr->GetMainSize().IsEmpty() && !itr->GetSubSize().IsEmpty() ) 
						rSizeString.Format( _T("%sX%s") , itr->GetMainSize() , itr->GetSubSize() );
					else    rSizeString.Format( _T("%s") ,    itr->GetMainSize() );
					
					rString.Format(_T("          1-%-2d ) %-16s %-6s %-16s %-16s %-6s %-6s") , ++nCount, itr->matlCode() , itr->matlSpec() , rSizeString , rMatlType , rWeldType , itr->QTY() + " WELDING OR BOLTING NOT REQUIRED [THRU BOLT TYPE] ");
					m_weldDetailEntry.push_back(rString);
					continue;
				}

				if ( rMatlCode.GetAt(0) != 'P' )	/// 파이프가 아닐때...
				{
					CString rWeldingType1 , rWeldingType2;
					if( rWeldType.Find(_T(",")) < 0 )
					{
						rWeldingType1 = rWeldType;
						rWeldingType2 = rWeldType;
					}
					else
					{
						rWeldingType1 = rWeldType.Mid( 0, rWeldType.Find(_T(","))  );
						rWeldingType2 = rWeldType.Mid( rWeldType.Find(_T(",")) + 1 );
					}
					
					int nWTSize1 = 0, nWTSize2 = 0;
					if     ( rWeldingType1 == _T("BW") ) nWTSize1 = BW;
					else if( rWeldingType1 == _T("SW") ) nWTSize1 = SW;
					else if( rWeldingType1 == _T("TW") ) nWTSize1 = TW;
					else if( rWeldingType1 == _T("FW") ) nWTSize1 = FW;
					
					if     ( rWeldingType2 == _T("BW") ) nWTSize2 = BW;
					else if( rWeldingType2 == _T("SW") ) nWTSize2 = SW;
					else if( rWeldingType2 == _T("TW") ) nWTSize2 = TW;
					else if( rWeldingType2 == _T("FW") ) nWTSize2 = FW;
										
					CString CF1 , CF2;
					float f1 = 0.f , f2 = 0.f;
					if( BW == nWTSize1 || SW == nWTSize1 || TW == nWTSize1 )
					{
						f1 = (float)(size_11 * pWeldDia->main_size); // 2003.10.6
						CF1.Format( _T("%s:%-6.3f X %-6.3f X %-6.3f / 2 = %-6.3f     ") , rWeldingType1, size_11, (float)pWeldDia->main_size, Qty, f1 * Qty * 0.5 );
					}
					/*	/// FW일 때는 계산을 하지 않는다.
					else
					{
						f1 = (float)1.0;
						// f1 =size_11  ; // 2003.10.6
						CF1.Format( "%s:1.0[%-6.3f] X %-6.3f / 2 =     %-6.3f     ", rWeldingType1, size_11, Qty, Qty / 2 );
					}
					*/
					
					if( BW == nWTSize2 || SW == nWTSize2 || TW == nWTSize2 )
					{
						f2 = (float)(size_12 * pWeldDia->sub_size );// 2003.10.6
						// f2 = size_12; 
						CF2.Format( _T("%s:%-6.3f X %-6.3f X %-6.3f / 2 = %-6.3f      ") , rWeldingType2, size_12, (float)pWeldDia->sub_size, Qty, f2*Qty * 0.5 );
					}
					/*else	//! FW일 때는 계산을 하지 않는다.
					{
						f2 = (float)1.0;// 2003.10.6
						// f2 = size_12 ; 
						CF2.Format( "%s:1.0[%-6.3f] X %-6.3f / 2 =    %-6.3f", rWeldingType2, size_12, Qty, Qty * 0.5 );
					}
					*/
					
					CString TTemp_2;
					if( size_11 == size_12 && nWTSize1 == nWTSize2 ) TTemp_2.Format( _T("%-6d %-16.3f %-16.3f %-16.3f ") , (int)Qty , f1*Qty * 0.5 + f2 * Qty * 0.5, pWeldDia->main_size, pWeldDia->sub_size );
					else
					{
						CString rString;
						rString.Format( _T("%-5.3f,%-5.3f") , f1*Qty * 0.5 ,  f2 * Qty * 0.5);
						TTemp_2.Format( _T("%-6d %-16s %-16.3f %-16.3f ") ,   (int)Qty , rString.operator LPCTSTR() , pWeldDia->main_size, pWeldDia->sub_size );
					}
				
					if( size_11 != size_12 ) 
						rSizeString.Format(_T("%sX%s") , itr->GetMainSize() , itr->GetSubSize() );
					else    rSizeString.Format(_T("%s") ,    itr->GetMainSize() );
					
					rString.Format(_T("          1-%-2d ) %-16s %-6s %-16s %-16s %-6s %s") , ++nCount , itr->matlCode() , itr->matlSpec() , rSizeString , rMatlType , rWeldType , TTemp_2 + CF1 + CF2);
					m_weldDetailEntry.push_back(rString);
					
					/// f Sub_Sum[MN] += ( f1 + f2 ) * Qty;
					m_nSub_Summary_By_Material[nMatlCodeNum][nWTSize1] += f1 * Qty * 0.5;
					m_nSub_Summary_By_Material[nMatlCodeNum][nWTSize2] += f2 * Qty * 0.5;
					/// FOR EXAMPLE, THE TOTAL OF Sub_Sum[1] MEANS THE AMOUNT OF CARBON STEEL IN NEED
					
					/// INSERT WELD_DIA INCH BY SPEC AND SIZE
					Add_Welding_SpecSize_Total(nWTSize1 , f1 , Qty , size_11);
					Add_Welding_SpecSize_Total(nWTSize2 , f2 , Qty , size_12);
				}
				else /// IF IT IS PIPE CUTTING LENGTH
				{
					rSizeString.Format(_T("%s") , itr->GetMainSize());
					rString.Format(_T("          1-%-2d ) %-16s %-6s %-16s %-16s %-6s %-6s") , ++nCount , itr->matlCode() , itr->matlSpec() , rSizeString , rMatlType , rWeldType , "                 SEE PIPE CUTTING LENGTH..");
					m_weldDetailEntry.push_back(rString);
				} 
			}
		}
		catch(const exception& ex)
		{
			AfxMessageBox(ex.what());
		}
	}

	PIPE_CUTTING_LENGTH_CALCULATION_Func(qcCheck);
	SUB_SUMMARY_BY_MAT_Func();
}

/**
	@brief	PIPE CUT LENGTH에 대한 BOM을 산출한다.

	@author	백흠경
*/
void CWeldDetailBMFile::PIPE_CUTTING_LENGTH_CALCULATION_Func(CQCCheck& qcCheck)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CMaterialCodeTable* pMaterialCodeTable = pDocData->GetProject()->GetMaterialCodeTable();
	CWeldDiaTable* pWeldDiaTable = pDocData->GetProject()->GetWeldDiaTable();
	const size_t nWeldDiaCount = pWeldDiaTable->GetWeldDiaCount();
	
	CString rString;

	rString = "  ";
	m_weldDetailEntry.push_back(rString);
	rString = ("          2) PIPE CUTTING LENGTH CALCULATION ");
	m_weldDetailEntry.push_back(rString);
	rString.Format("%s %-16s %-16s %-8s %-5s %-8s %-6s" ,"                ","WELD_TYPE" , "LENGTH" , "PIPE" , "REM." , "TOTAL" , "CALCULATION");
	m_weldDetailEntry.push_back(rString);

	int nCount = 0;
	for(vector<CCutPipeLengthData>::iterator itr = qcCheck.m_cutPipeLengths.begin();itr != qcCheck.m_cutPipeLengths.end();++itr)
	{
		const CIsoBMData* pIsoBMData = qcCheck.FindPipe(itr->NPD());
		if(NULL == pIsoBMData) continue;
		
		CString rMatlCode = pIsoBMData->matlCode();

		CString rCode1;
		rCode1.Format("5");	///< PIPE CODE_1
		CString rCode2 = MATL_CODE_LOC_4(rMatlCode);
		
		const int nMatlCodeNum = pMaterialCodeTable->FindMatlNumWithGroupAndCodeLoc4(rCode1 , rCode2);
		if(-1 == nMatlCodeNum) continue;	///< MAT'L CODE NUM를 구할 수 없을 때
		///if( MATL_CODE(rMatlCode) == '2' ) nMatlCodeNum = 5;	
		
		/// MATL CODE NUM에서 MATL TYPE 문자열을 구한다.
		CString rMatlType = GetMatlType(nMatlCodeNum);

		int WT = 0;
		CString rWeldType = pDocData->GetWeldType(pIsoBMData->matlSpec() , rMatlCode , pIsoBMData->bmcsSYM());

		if(rWeldType.IsEmpty()) continue; /// PCD File이 없으면 WeldType이 Empty라 에러처리.
		if     ( rWeldType == "BW" ) WT = BW;
		else if( rWeldType == "SW" ) WT = SW;
		else if( rWeldType == "TW" ) WT = TW;
		else if( rWeldType == "FW" ) WT = FW;
		
		if( "NO" == rWeldType )
		{
			rString.Format("          2-%-2d ) %-16s %-10s %-15s %-15s %-6s %-6s %s" , 
				++nCount , rMatlCode , pIsoBMData->matlSpec() , pIsoBMData->GetMainSize() , rMatlType , rWeldType , pIsoBMData->bmcsSYM() , "WELDING TYPE NOT DEFINED");
			m_weldDetailEntry.push_back(rString);
			continue;
		}
		
		if( rWeldType == "TF" )
		{
			rString.Format("          2-%-2d ) %-16s %-10s %-15s %-15s %-6s %-6s %s" , 
				++nCount , rMatlCode , pIsoBMData->matlSpec() , pIsoBMData->GetMainSize() , rMatlType , rWeldType , pIsoBMData->bmcsSYM() , "WELDING OR BOLTING NOT REQUIRED [THRU BOLT TYPE] ");
			m_weldDetailEntry.push_back(rString);
			continue;
		}
		
		CString TTemp_1;
		long l = GetWeldingLength( atof(itr->length()) );
		double size = atof(pIsoBMData->GetMainSize());
		
		CString TTemp_2;
		TTemp_2.Format( "          2-%-2d ) %-16s %-16s %-8d %-5d %-8d", ++nCount , rWeldType, itr->length() , l - 1, 1 , l);

		if ( (size < 1) && ( BW == WT || SW == WT ) )
		{ 
			TTemp_1.Format( " %-3d X 1.00[%-7.3f] = %-7.3f", l, size , (float)l ); 
			// Sub_Sum[MN][WT] += (float)l; //2003.10.6 for spec total 
			m_nSub_Summary_By_Material[nMatlCodeNum][WT] += (float)size;
		}
		else if( size < 1 && ( BW != WT && SW != WT ) )
		{
			TTemp_1.Format( " %-3d X 1.00[%-7.3f] = %-7.3f", l, size , (float)l ); 
			// Sub_Sum[MN][WT] += (float)l; //2003.10.6 for spec total 
			m_nSub_Summary_By_Material[nMatlCodeNum][WT] += (float)size;
		}
		else if( size >= 1 && ( BW == WT  || SW == WT ) )
		{
			TTemp_1.Format( " %-3d X %-7.3f = %-7.3f", l, size , (float)l * size );
			// Sub_Sum[MN][WT] += (float)l * (float)PIP[i].Size; //2003.10.6 for spec total 
			m_nSub_Summary_By_Material[nMatlCodeNum][WT] += (float)(l * size);
		}
		else
		{
			TTemp_1.Format( " %-3d X %-7.3f = %-7.3f", l, size , (float)l * size );
			// Sub_Sum[MN][WT] += (float)l;
			m_nSub_Summary_By_Material[nMatlCodeNum][WT] += (float)(l* size);  //2003.10.6 for spec total 
		}
		
		Add_Welding_SpecSize_Total(WT , size , l * 2 , size);
		
		rString = TTemp_2 + TTemp_1;
		m_weldDetailEntry.push_back(rString);
	}
}

/**
	@brief	

	@author	백흠경
**/
bool CWeldDetailBMFile::Write(const CString& rOuputFilePath)
{
	ofstream ofile(rOuputFilePath);
	if(ofile.good())
	{
		for(vector<CString>::iterator itr = m_weldDetailEntry.begin();itr != m_weldDetailEntry.end();++itr)
		{
			ofile << itr->operator const char*() << std::endl;
		}
		ofile.close();

		return true;
	}

	return false;
}

/**
	@brief	

	@author	백흠경
**/
void CWeldDetailBMFile::Clear()
{
	m_weldDetailEntry.clear();
}

/**
	@brief	mat'l code에서 item group와 matl code loc 8~9 혹은 10~11 의 값을 구한다.

	@author	humkyung
*/
int CWeldDetailBMFile::CheckMatlCodeNumForGALV(CString &rCode1, CString &rCode2, const CString &rMatlCode) const
{
	int ret = 0;

	const int nLength = rMatlCode.GetLength();
	rCode1 = GetMatlItemGroup( rMatlCode );
	if( (FLANGE_ITEM_GROUP_STRING == rCode1) || (FITTING_ITEM_GROUP_STRING == rCode1) )
	{
		if(nLength > 9)
		{
			rCode2 = rMatlCode.Mid( 7 , 2 );
			return -1;
		}
	}
	else if( PIPE_ITEM_GROUP_STRING == rCode1 )
	{
		if(nLength > 11)
		{
			rCode2 = rMatlCode.Mid( 9 , 2 );
			return -1;
		}
	}

	return ret;
}

/**
	@brief	mat'l code에서 item group와 matl code loc 4의 값을 구한다.

	@author	백흠경
*/
int CWeldDetailBMFile::CheckMatlCodeNum(CString &rCode1, CString &rCode2, const CString &rMatlCode) const
{
	int ret = 0;

	if(!rMatlCode.IsEmpty())
	{
		switch ( rMatlCode.GetAt(0) ) 
		{
			case 'B': { ret = 0; break; }													///< BOLT / NUT	rCode1 ==> 1
			case 'G': { ret = 0; break; }													///< GASKET		rCode1 ==> 3
				
			case 'F': { rCode1.Format("2"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< FLANGE
			case 'K': { rCode1.Format("4"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< FITTING
			case 'P': { rCode1.Format("5"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< PIPE
			case 'S': { rCode1.Format("6"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< REINFORCING PAD
			case 'T': { rCode1.Format("7"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< TUBE & TUBE FITTING
			case 'V': { rCode1.Format("8"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< VALVE
			case 'Y': { rCode1.Format("9"); rCode2 = MATL_CODE_LOC_4(rMatlCode); ret = -1; break; }	///< MISC. ITEM 
		} 
	}

	return ret;
}

/**
	@brief	MATL CODE NUM에 대해서 MATL TYPE STRING을 구한다.

	@author	백흠경
*/
CString CWeldDetailBMFile::GetMatlType(const int &matlCodeNum) const
{
	switch(matlCodeNum)
	{
		case 1:
			return "CARBON STEEL   ";
		case 2:
			return "ALLOY STEEL    ";
		case 3:
			return "STAINLESS STEEL";
		case 4:
			return "ETC               ";
		case 5:
			return "GALV.          ";
	}

	return CString("");
}

/**	@brief	material code에 맞는 weld dia name을 가지는 WeldDia 구조체를 구한다.

	@author	humkyung

	@param	rMatlCode	a parameter of type const CString &

	@return	WeldDia*
*/
WeldDia* CWeldDetailBMFile::FindWeldDia(const CString &rMatlCode)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CWeldDiaTable* pWeldDiaTable = pDocData->GetProject()->GetWeldDiaTable();
	
	if(pWeldDiaTable)
	{
		const size_t nWeldDiaCount = pWeldDiaTable->GetWeldDiaCount();

		WeldDia* pWeldDia = NULL;
		int check = 0;
		for(int i = 0;i < int(nWeldDiaCount);++i)
		{
			pWeldDia = pWeldDiaTable->GetWeldDiaAt(i);
			
			///if ( pWeldDia->range == -1 || pWeldDia->range == 65535 ) break;
			if ( -1 == pWeldDia->name->Find('?') )
			{
				 /// ?를 포함하지 않을 때
				if ( 0 == rMatlCode.Find( *(pWeldDia->name) ) ) /// FOUND IN THE FIRST PLACE
				{
					check = 1;
					break;
				}
			}
			else
			{
				size_t nWeldDiaNameLength = pWeldDia->name->GetLength();
				check = 0;
				for ( int k = 0; k < int(nWeldDiaNameLength) ; k++ )
				{
					if ( pWeldDia->name->GetAt( k ) != '?' )
					{
						if ( pWeldDia->name->GetAt( k ) == rMatlCode.GetAt( k ) ) check++;
					}
					else check++;
				}
				if ( check == int(nWeldDiaNameLength) ) ///< IF ALL MATCHES WITH MC_1 EXCEPT '?'
				{
					check = 1;
					break;
				}
			}
		}

		return pWeldDia;
	}

	return NULL;
}

/**	@brief	WELDING POINT의 개수를 구한다.

	@author	백흠경

	@param	length	a parameter of type const double &

	@remarks	파이프의 길이를 PIPE CUT LENGTH로 나누면 WELDING POINT의 개수가 된다.

	@return	long	
*/
long CWeldDetailBMFile::GetWeldingLength(const double &length) const
{
	const double nPipeCutLength = CBMReportSettingPage::GetPipeCutLength();

	long ret = long(ceil( length / nPipeCutLength ));
	return ret;
}

namespace
{
	bool __SortBySize(const SpecSizeTotal& lhs , const SpecSizeTotal& rhs)
	{
		return (lhs.size < rhs.size);
	}
}

/**
	@brief	BM관련 SUMMARY를 한다.
	
	@author	백흠경
*/
void CWeldDetailBMFile::SUB_SUMMARY_BY_MAT_Func()
{
	CString rString;

	try
	{
		rString = ("  "); m_weldDetailEntry.push_back(rString);
		rString = ("          3 )SUB SUMMARY BY MAT.   BW        SW        TW        TOTAL");
		m_weldDetailEntry.push_back(rString);
		rString.Format("          3-1 ) CARBON STEEL    :  %-10.3f%-10.3f%-10.3f%-10.3f", m_nSub_Summary_By_Material[1][0], m_nSub_Summary_By_Material[1][1], m_nSub_Summary_By_Material[1][2], m_nSub_Summary_By_Material[1][0] + m_nSub_Summary_By_Material[1][1] + m_nSub_Summary_By_Material[1][2] );
		m_weldDetailEntry.push_back(rString);
		rString.Format("          3-2 ) ALLOY STEEL     :  %-10.3f%-10.3f%-10.3f%-10.3f", m_nSub_Summary_By_Material[2][0], m_nSub_Summary_By_Material[2][1], m_nSub_Summary_By_Material[2][2], m_nSub_Summary_By_Material[2][0] + m_nSub_Summary_By_Material[2][1] + m_nSub_Summary_By_Material[2][2] );
		m_weldDetailEntry.push_back(rString);
		rString.Format("          3-3 ) STAINLESS STEEL :  %-10.3f%-10.3f%-10.3f%-10.3f", m_nSub_Summary_By_Material[3][0], m_nSub_Summary_By_Material[3][1], m_nSub_Summary_By_Material[3][2], m_nSub_Summary_By_Material[3][0] + m_nSub_Summary_By_Material[3][1] + m_nSub_Summary_By_Material[3][2] );
		m_weldDetailEntry.push_back(rString);
		rString.Format("          3-4 ) GALV.     STEEL :  %-10.3f%-10.3f%-10.3f%-10.3f", m_nSub_Summary_By_Material[5][0], m_nSub_Summary_By_Material[5][1], m_nSub_Summary_By_Material[5][2], m_nSub_Summary_By_Material[5][0] + m_nSub_Summary_By_Material[5][1] + m_nSub_Summary_By_Material[5][2] );
		m_weldDetailEntry.push_back(rString);
		rString.Format("          3-5 ) ETC             :  %-10.3f%-10.3f%-10.3f%-10.3f", m_nSub_Summary_By_Material[4][0], m_nSub_Summary_By_Material[4][1], m_nSub_Summary_By_Material[4][2], m_nSub_Summary_By_Material[4][0] + m_nSub_Summary_By_Material[4][1] + m_nSub_Summary_By_Material[4][2] );
		m_weldDetailEntry.push_back(rString);
		
		rString.Format("          3-6 ) TOTAL           :  %-10.3f%-10.3f%-10.3f%-10.3f", 
			m_nSub_Summary_By_Material[1][0] + m_nSub_Summary_By_Material[2][0] + m_nSub_Summary_By_Material[3][0] + m_nSub_Summary_By_Material[4][0] + m_nSub_Summary_By_Material[5][0],
			m_nSub_Summary_By_Material[1][1] + m_nSub_Summary_By_Material[2][1] + m_nSub_Summary_By_Material[3][1] + m_nSub_Summary_By_Material[4][1] + m_nSub_Summary_By_Material[5][1],
			m_nSub_Summary_By_Material[1][2] + m_nSub_Summary_By_Material[2][2] + m_nSub_Summary_By_Material[3][2] + m_nSub_Summary_By_Material[4][2] + m_nSub_Summary_By_Material[5][2],
			
			m_nSub_Summary_By_Material[1][0] + m_nSub_Summary_By_Material[1][1] + m_nSub_Summary_By_Material[1][2] + // m_nSub_Summary_By_Material[1][3] +
			m_nSub_Summary_By_Material[2][0] + m_nSub_Summary_By_Material[2][1] + m_nSub_Summary_By_Material[2][2] + // m_nSub_Summary_By_Material[2][3] +
			m_nSub_Summary_By_Material[3][0] + m_nSub_Summary_By_Material[3][1] + m_nSub_Summary_By_Material[3][2] + // m_nSub_Summary_By_Material[3][3] + 
			m_nSub_Summary_By_Material[4][0] + m_nSub_Summary_By_Material[4][1] + m_nSub_Summary_By_Material[4][2] + // m_nSub_Summary_By_Material[4][3] +
			m_nSub_Summary_By_Material[5][0] + m_nSub_Summary_By_Material[5][1] + m_nSub_Summary_By_Material[5][2]   // m_nSub_Summary_By_Material[5][3],
			);
		m_weldDetailEntry.push_back(rString);
		
		static const CString rHeaders[2] = {"          BW, SW, TW WELDING DIA SUMMARY BY SPEC AND SIZE" , "          FW WELDING DIA SUMMARY BY SPEC AND SIZE  "};
		float fTmp = 0, fTmpSum = 0; 
		for( int x = 0 ; x < 1 ; ++x )
		{
			rString = ("  "); m_weldDetailEntry.push_back(rString);
			
			rString = rHeaders[ x ];
			m_weldDetailEntry.push_back(rString);
			
			rString = _T("          SPEC_NAME SIZE ");
			CString rSizeString;
			stable_sort(m_specSizeTotalEntry[x].begin() , m_specSizeTotalEntry[x].end() , __SortBySize);
			vector<SpecSizeTotal>::iterator itr = m_specSizeTotalEntry[x].begin();
			for(itr = m_specSizeTotalEntry[x].begin() ; itr != m_specSizeTotalEntry[x].end() ; ++itr)
			{ 
				rSizeString.Format("%-7.3lf" , itr->size);
				rSizeString.Format("%-10s" , rSizeString);
				rString += rSizeString;
			}
			
			rString += "TOTAL"; 
			m_weldDetailEntry.push_back(rString); 
			
			double nTotal = 0;
			rString.Format("          %-15s" , m_rSpec);
			for( itr = m_specSizeTotalEntry[x].begin() ; itr != m_specSizeTotalEntry[x].end() ; ++itr)
			{ 
				nTotal += itr->total;
				rSizeString.Format("%-7.3lf" , itr->total);
				rSizeString.Format("%-10s" , rSizeString);
				rString += rSizeString;
			}
			rSizeString.Format("%-7.3lf" , nTotal);
			rString += rSizeString;
			m_weldDetailEntry.push_back(rString);
			
			rString = _T("          --------------------------------------------------------------------------");
			m_weldDetailEntry.push_back(rString);
			
			CString rSummaryFormat;
			rSummaryFormat.Format("          TOTAL%%%ds" , (int)(10 + 10 * (m_specSizeTotalEntry[x].size())));
			rSummaryFormat += "%lf";
			rString.Format(rSummaryFormat , " " , nTotal);
			rString = ("  "); m_weldDetailEntry.push_back(rString);
			m_weldDetailEntry.push_back(rString);
		}
	}
	catch(...)
	{
		AfxMessageBox("Error in SUB_SUMMARY_BY_MAT_Func");
	}
}

/**
	@brief	관련 내부 변수들을 초기화한다.

	@author	백흠경
*/
void CWeldDetailBMFile::InitInternalVariables()
{
	memset(m_nSub_Summary_By_Material , 0 , sizeof(double) * 6 * 4);
	memset(m_nTotal_Sum,0,sizeof(double)*6*4);
	m_specSizeTotalEntry[0].clear();
	m_specSizeTotalEntry[1].clear();
}

/**
	@brief	MAT'L CODE STRING에서 ITEM GROUP STRING을 구한다.

	@author	백흠경

	@param	MAT'L CODE STRING

	@reutrn	ITEM GROUP STRING(-1 = 원하지 않는 ITEM GROUP NUMBER)
*/
CString CWeldDetailBMFile::GetMatlItemGroup(const CString &rMatlCode) const
{
	CString res;

	if(!rMatlCode.IsEmpty())
	{
		TCHAR ch = rMatlCode.GetAt(0);

		switch(ch)
		{
			case 'F': 
				res = FLANGE_ITEM_GROUP_STRING;			/// FLANGE
			break;
			case 'K':
				res = FITTING_ITEM_GROUP_STRING;		/// FITTING
			break;
			case 'P':
				res = PIPE_ITEM_GROUP_STRING;			/// PIPE
			break;
			case 'S':
				res = REINFORCING_PAD_ITEM_GROUP_STRING;	/// REINFORCING PAD
			break;
			case 'T':
				res = TUBE_TUBE_FITTING_ITEM_GROUP_STRING;	/// TUBE & TUBE FITTING
			break;
			case 'V':
				res = VALVE_ITEM_GROUP_STRING;			/// VALVE
			break;
			case 'Y':
				res = MISC_ITEM_GROUP_STRING;			/// MISC. ITEM 
			break;
		}
	}

	return res;
}
