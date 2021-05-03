// QCCheck.cpp: implementation of the CQCCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>

#include <string>
#include <iostream>
#include <boost/regex.hpp> 

#include "QCCheck.h"
#include "SmartISO_ntr.h"

#include <IsPoly2d.h>
#include <DgnTCB.h>
#include <Tokenizer.h>

#include <algorithm>
#include <functional>

#include "isstring.h"
#include <fstream>


#include <string>
#include <vector>
#include "IdmsNtrSetting.h"
#include "DEPFile.h"
#include "IMPFile.h"
#include "dgnenum.h"

#include <set>
#include <map>
#include <sstream>

extern ofstream ofile;
int CIsoBMData::m_nFieldPos[7];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQCCheck::CQCCheck()
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
	}
	catch(...)
	{
		
	}
}

namespace
{
	enum
	{
		FIND_MATCH_CASE		= 0x0001,
		FIND_WHOLE_WORD		= 0x0002,
		FIND_DIRECTION_UP	= 0x0010,
		REPLACE_SELECTION	= 0x0100
	};

	bool __SortByTextPosY(const CDgnText* lhs , const CDgnText* rhs)
	{
		return (lhs->origin().y > rhs->origin().y);
	}


	struct IsMatchString : public binary_function<CDgnText*, string, bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CDgnText* a, const string& b ) const
		{
			string str = a->textString();
			IsString::TrimWhiteSpace(str);
			return (str == b);
		}
	};

	struct IsMatchData : public binary_function<CIsoBMData , string , bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CIsoBMData& lhs, const string& rhs ) const
		{
			string str = lhs.dataString();
			IsString::TrimWhiteSpace(str);
			return (str == rhs);
		}
	};

	struct IsInnerPt : public binary_function<CIsPoly2d , CIsPoint2d , bool>
	{
		bool operator()(CIsPoly2d lhs , const CIsPoint2d& rhs) const
		{
			return lhs.IsInnerPt(rhs);
		}
	};
};


bool FileExist(const string& p)
{
	OFSTRUCT of;
	if(HFILE_ERROR != OpenFile(p.c_str() , &of , OF_EXIST)) return TRUE;

	return FALSE;
}

/**	
	@brief	CIsoBMData�� �������� �� container�� �����Ѵ�.

	@author	humkyung

	@return	void	
*/
void CQCCheck::ExtractTextData(CDgnDocument* pDgnDoc , const string &rIniFilePath)
{	
	InitInternalVariables();
	//////////////////////////////////////////////////////////////////////////
	/// check
	vector<CDgnText*> systemBMTextEntry;
	vector<CIsPoly2d> poly2dEntry;
	m_graphicAreaDatas.clear();

	CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();
	const size_t nCount = pDgnDoc->GetEntityCount();
	for(int i = 0;i < int(nCount);i++)
	{
		CDgnElement* pEnt = pDgnDoc->GetEntityAt(i);
		if(pEnt->IsKindOf(CDgnText::TypeString()))
		{
			CDgnText* pDGNText2d = static_cast<CDgnText*>(pEnt);
			DPoint3d origin = pDGNText2d->origin();
			origin.z = 0.0;

			CDgnVolume vol;
			vol.Add( origin );
			
			if(vol.CollideWith(setting.m_bm_volume))
			{
				systemBMTextEntry.push_back(pDGNText2d);
			}
			else if(vol.CollideWith(setting.m_graphic_volume))
			{
				CIsoBMData isobmdata(CIsPoint2d(pDGNText2d->origin().x , pDGNText2d->origin().y) , pDGNText2d->textString());
				isobmdata.SetDGNText2D(pDGNText2d);
				m_graphicAreaDatas.push_back(isobmdata);
			}
			else if(vol.CollideWith(setting.m_WeldNoVolume))
			{
				CIsoBMData isobmdata(CIsPoint2d(pDGNText2d->origin().x , pDGNText2d->origin().y) , pDGNText2d->textString());
				isobmdata.SetDGNText2D(pDGNText2d);
				m_WeldNoAreaDatas.push_back(isobmdata);
			}
		}
		else if(pEnt->IsKindOf(CDgnLineString::TypeString()))
		{
			CDgnVolume vol = pEnt->volume();
			if(vol.CollideWith(setting.m_graphic_volume))
			{
				CDgnLineString* p = static_cast<CDgnLineString*>(pEnt);
				if(p)
				{
					if((5 == p->GetVertexCount()) || (35 == p->GetVertexCount()))	//! BOX TYPE or ELLIPSE TYPE
					{
						CIsPoly2d poly2d;
						
						DPoint3d unnamed;
						const size_t nCount = p->GetVertexCount();
						for(size_t i = 0;i < nCount;++i)
						{
							unnamed = p->GetVertexAt(i);
							poly2d.AddVertex(CIsPoint2d(unnamed.x , unnamed.y));
						}
						poly2dEntry.push_back(poly2d);
					}
				}
			}
		}
	}
	ParseBMTexts(systemBMTextEntry);

	//////////////////////////////////////////////////////////////////////////
	/// find text inside line string
	map<CIsPoly2d* , CBoxTextString*> unnamed;
	for(vector<CIsoBMData>::iterator jtr = m_graphicAreaDatas.begin();jtr != m_graphicAreaDatas.end();++jtr)
	{
		CIsoBMData* data = &(*jtr);
		vector<CIsPoly2d>::iterator where = find_if(poly2dEntry.begin() , poly2dEntry.end() , bind2nd(IsInnerPt() , jtr->origin()));
		if(where != poly2dEntry.end())
		{
			if(unnamed.end() != unnamed.find(&(*where)))
			{
				///unnamed[where]->m_textStringEntry.push_back(jtr->dataString());
				unnamed[&(*where)]->m_textStringEntry.push_back(jtr->GetDGNText2D());
			}
			else
			{
				unnamed[&(*where)] = new CBoxTextString;
				///unnamed[where]->m_textStringEntry.push_back(jtr->dataString());
				unnamed[&(*where)]->m_textStringEntry.push_back(jtr->GetDGNText2D());
				m_boxTextStringEntry.push_back(unnamed[&(*where)]);
			}
		}
	}
}

/**	@brief	���� ���� �ʱ�ȭ.

	@return	void
*/
void CQCCheck::InitInternalVariables()
{
	CIdmsNtrSetting pSetting = CIdmsNtrSetting::GetInstance();
	string rDGNPath = pSetting.m_rDGNFilePath;
	m_pipeDatas.clear();
	m_generalDatas.clear();
	m_supportDatas.clear();
	if(!rDGNPath.empty() && m_dgnFile.Open(rDGNPath.c_str(),CDgnFile::modeRead))
	{		
		CDgnParser parser(NULL);
		parser.AddParsingType(CELL_HEADER_ELM);
		
		/// TCB���� �Ľ��� �Ѵ�.
		for(;m_dgnFile.ReadElement();)
		{
			CDgnElement* pEnt = parser.ParseElement(&m_dgnFile);
			if(NULL == pEnt) break;
			if(pEnt->IsKindOf(CDgnTCB::TypeString())) break;
		}
		m_DgnOpen = TRUE;
	}
	m_boxTextStringEntry.clear();
}


/**	@brief	BM Text�� ���� CIsoBMData�� �����Ѵ�.
	
	@param	systemBMTextEntry	a parameter of type vector<CDgnText*>&

	@remarks
	<li> data position�� ���õ� field�� �д´�.

	@return	void
*/
void CQCCheck::ParseBMTexts(vector<CDgnText*>& systemBMTextEntry)
{
	CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();

	stable_sort(systemBMTextEntry.begin() , systemBMTextEntry.end() , __SortByTextPosY);
	vector<CDgnText*>::iterator where = find_if(systemBMTextEntry.begin() , systemBMTextEntry.end() , bind2nd(IsMatchString() , "PIPE SUPPORTS"));
	vector<CDgnText*>::iterator whereMISCELL = find_if(systemBMTextEntry.begin(), systemBMTextEntry.end() , bind2nd(IsMatchString(), "MISCELLANEOUS COMPONENTS")); ///< MISCELLANEOUS COMPONENTS�� where�� ã�´�.
		
	static const boost::regex num_pattern("^[0-9]+");
	static const boost::regex cut_pipe_length_pattern("CUT PIPE LENGTH");
	static const boost::regex num_pattern_with_bracket("<[0-9]+>");

	for(vector<CDgnText*>::iterator itr = systemBMTextEntry.begin();itr != systemBMTextEntry.end();++itr)
	{
		string rTextString = (*itr)->textString();
		
		/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
		/// find field position
		string::size_type at = rTextString.find("MATERIALS DESCRIPTION");
		if(string::npos != at)
		{
			CIsoBMData::m_nFieldPos[0] = 0;
			at = rTextString.find("NO");
			CIsoBMData::m_nFieldPos[1] = at + 2;
			if("INCH" == setting.m_rUnit)
			{
				at = rTextString.find("(IN)");
				CIsoBMData::m_nFieldPos[2] = at;
			}
			else
			{
				at = rTextString.find("(MM)");
				CIsoBMData::m_nFieldPos[2] = at;
			}
			at = rTextString.find("SPEC");
			CIsoBMData::m_nFieldPos[3] = at;
			at = rTextString.find("SYM");
			CIsoBMData::m_nFieldPos[4] = at;
			at = rTextString.find("CODE");
			CIsoBMData::m_nFieldPos[5] = at;
			at = rTextString.find("QTY");
			CIsoBMData::m_nFieldPos[6] = at;

			continue;
		}
		rTextString.assign(rTextString.substr(0 , 5)); 
		IsString::TrimWhiteSpace(rTextString);
		//////////////////////////////////////////////////////////////////////////
		/// regular expression
		try
		{
			///boost::cmatch results;
			boost::match_results<std::string::const_iterator>   results;
			if(boost::regex_search(rTextString, results, num_pattern))
			{
				CDgnText* pDGNText2d = (*itr);
				if(itr < where) ///< PIPE SUPPORT ���� �κ�
				{
					/// TODO : �α׸� �����
					CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr)->textString());
					/// �ϳ��� �׸� ���õ� �ؽ�Ʈ�� ã�´�.
					StoreBMDataText(bmData , itr , systemBMTextEntry); //! iterator�� ���������� iterator�� check�ؾ� �Ѵ�.
					bmData.Parse();
					{
						string::size_type nFindPos = bmData.m_rMatlDesc.find("PIPE");
						if(string::npos != nFindPos)
						{
							bmData.SetDGNText2D(pDGNText2d);
							m_pipeDatas.push_back(bmData);
						}
						else
						{
							bmData.SetDGNText2D(pDGNText2d);
							m_generalDatas.push_back(bmData);
						}
					}
				}
				/// PIPE SUPPORT �κ�( MISCELLANEOUS DATA ���� �κ� )
				else if( (where < itr) && (itr < whereMISCELL) )
				{
					CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr)->textString());
					StoreBMDataText(bmData , itr , systemBMTextEntry); //! iterator�� ���������� iterator�� check�ؾ� �Ѵ�.
					bmData.Parse();
					bmData.SetDGNText2D(pDGNText2d);
					m_supportDatas.push_back(bmData);
				}
				else if( itr > whereMISCELL ) ///< MISCELLANEOUS DATA �κ�
				{
					CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr)->textString());
					StoreBMDataText(bmData , itr , systemBMTextEntry); //! iterator�� ���������� iterator�� check�ؾ� �Ѵ�.
					bmData.Parse();
					bmData.SetDGNText2D(pDGNText2d);
					m_miscellDatas.push_back(bmData);
				}
			}

			//! iterator�� container�� ���� ���������� loop�� �����Ѵ�.
			if(itr == systemBMTextEntry.end()) break;
		}
		catch(...)
		{
			
		}
	}
}

/**
	@brief	CIsoBMData�� ���� �ؽ�Ʈ���� ���Ѵ�.

	@author	�����
**/
void CQCCheck::StoreBMDataText(CIsoBMData& bmData , vector<CDgnText*>::iterator& itr , vector<CDgnText*>& systemBMTextEntry)
{
	static const boost::regex num_pattern("^[0-9]+");
	static const boost::regex cut_pipe_length_pattern("CUT PIPE LENGTH");
	static const boost::regex num_pattern_with_bracket("<[0-9]+>");

	int nLastFindWhatLen = 0;
	//match_results results;
	for(++itr;itr != systemBMTextEntry.end();++itr)
	{
		string rTextString = (*itr)->textString();
		if(rTextString.empty()) continue;
		string rNumString;
		string::size_type at = rTextString.find("CUT PIPE LENGTH");
		if(string::npos != at)
			rNumString = rTextString;
		else	rNumString = rTextString.substr(0 , 5);
		IsString::TrimWhiteSpace(rNumString);
		///boost::cmatch results;
		boost::match_results<std::string::const_iterator> results;
		if(boost::regex_search(rNumString, results, num_pattern))
		{
			--itr;
			break;
		}
		
		if(boost::regex_search(rNumString , results , cut_pipe_length_pattern))
		{
			--itr;
			break;
		}
		
		if(boost::regex_search(rNumString , results , num_pattern_with_bracket))
		{
			--itr;
			break;
		}
		
		bmData.AddSubText(*itr , rTextString);
	}
}

/**
	@brief	Main or Sub Size�� �Ľ��Ѵ�. �м��� ��쿡 �Ҽ����� �ִ� ���� ��м��̴�.
	�м� ������ �Ҽ��� �������� ��ȯ�Ͽ� �����Ѵ�.

	@author	humkyung
*/
string CIsoBMData::ParseSizeNum( const STRING_T& rString )
{
	string res(rString);

	int at = rString.find("/");
	if(-1 != at)
	{
		string numerator[2];
		numerator[0]       = rString.substr(0 , at);
		string denominator = rString.substr(at + 1 , rString.length() - at - 1);
		
		at = numerator[0].find(".");
		if( -1 != at )
		{
			numerator[1] = numerator[0].substr(at + 1 , numerator[0].length() - at - 1);
			numerator[0] = numerator[0].substr(0 , at);
		}
		
		at = denominator.find(".");
		if( -1 != at ) IsString::TrimRight(denominator , "0");
		
		double value = 0.f;
		if(!denominator.empty() && !numerator[1].empty())
		{
			value = atof(numerator[0].c_str()) + (atof(numerator[1].c_str()) / atof(denominator.c_str()));
		}
		else if(!denominator.empty() && numerator[1].empty())
		{
			value = atof(numerator[0].c_str()) / atof(denominator.c_str());
		}
		else
		{
			value = atof(numerator[0].c_str()) + atof(numerator[1].c_str());
		}
		
		stringstream oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.find(".");
		if( -1 != at ) IsString::TrimRight(res , "0");

		return res;
	}

	return string("");
}

//////////////////////////////////////////////////////////////////////////
/**	
	@brief	BM Text Line�� �Ľ��Ѵ�.

	@author	BHK

	@return	bool
*/
bool CIsoBMData::Parse()
{
	if(int(m_rDataString.length()) < CIsoBMData::m_nFieldPos[5]) return false;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// PT                                       NPD          MATL    BMCS      MATL
	/// NO         MATERIALS DESCRIPTION         (IN) or (MM)         SPEC    SYM       CODE                  QTY
	string rString(m_rDataString);
	int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

	m_rMatlDesc = m_rDataString.substr(CIsoBMData::m_nFieldPos[1] , CIsoBMData::m_nFieldPos[2] - CIsoBMData::m_nFieldPos[1]);
	IsString::TrimWhiteSpace(m_rMatlDesc);

	rString = m_rDataString.substr(CIsoBMData::m_nFieldPos[2] , CIsoBMData::m_nFieldPos[3] - CIsoBMData::m_nFieldPos[2]);
	IsString::TrimWhiteSpace(rString);

	string::size_type nFindPos = rString.find('X');
	if(string::npos != nFindPos)
	{
		m_rNPD[0] = ParseSizeNum(rString.substr(0 , nFindPos).c_str());
		m_rNPD[1] = ParseSizeNum(rString.substr(rString.length() - (rString.length() - nFindPos - 1) , rString.length() - nFindPos - 1).c_str());
	}
	else
	{
		m_rNPD[0] = ParseSizeNum(rString);
	}

	m_rMatlSpec = m_rDataString.substr(CIsoBMData::m_nFieldPos[3] , CIsoBMData::m_nFieldPos[4] - CIsoBMData::m_nFieldPos[3]);
	IsString::TrimWhiteSpace(m_rMatlSpec);

	m_rBMCSSYM = m_rDataString.substr(CIsoBMData::m_nFieldPos[4] , CIsoBMData::m_nFieldPos[5] - CIsoBMData::m_nFieldPos[4]);
	IsString::TrimWhiteSpace(m_rBMCSSYM);
	
	int i = 0;
	/// �߰��� ������ ������ �������� �κ��� MAT'L CODE�� �Ѵ�.
	m_rMatlCode = m_rDataString.substr(CIsoBMData::m_nFieldPos[5] , CIsoBMData::m_nFieldPos[6] - CIsoBMData::m_nFieldPos[5]);
	IsString::TrimWhiteSpace(m_rMatlCode);
	for(i = 0;i < int(m_rMatlCode.length());++i)
	{
		if(' ' == m_rMatlCode[i])
		{
			m_rMatlCode = m_rMatlCode.substr(0 , i);
			break;
		}
	}
	
	//! find QTY code
	for(i = CIsoBMData::m_nFieldPos[5];i < int(m_rDataString.length());++i)
	{
		if(' ' == m_rDataString[i]) break;
	}
	//! variable i contains start of QTY code.
	///m_rQTY = m_rDataString.substr(m_rDataString.length() - (m_rDataString.length() - CIsoBMData::m_nFieldPos[6]) , (m_rDataString.length() - CIsoBMData::m_nFieldPos[6]));
	m_rQTY = m_rDataString.substr(m_rDataString.length() - (m_rDataString.length() - i) , (m_rDataString.length() - i));
	IsString::TrimWhiteSpace(m_rQTY);

	
	/// BOLT TYPE
	if("BT" == m_rBMCSSYM)
	{
		///boost::cmatch results;
		boost::match_results<std::string::const_iterator> results;
		for(map<CDgnText*,string>::iterator itr = m_subTextEntry.begin();itr != m_subTextEntry.end();++itr)
		{
			string line(itr->second);
			static const boost::regex what("[0-9]+(\\.[0-9]+)?MM");
			if( boost::regex_search( line , results , what ))
			{
				m_rNPD[1] = results.str(0);
				m_rNPD[1] = m_rNPD[1].substr(m_rNPD[1].length() - 2);
				break;
			}
		}
	}

	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	if(!m_rNPD[0].empty())
	{
		string::size_type at = 0;
		if(string::npos != (at = m_rNPD[0].find("/")))
		{
			double a = atof(m_rNPD[0].substr(0,at).c_str());
			double b = atof(m_rNPD[0].substr(m_rNPD[0].length() - ((m_rNPD[0].length() - at - 1)) , (m_rNPD[0].length() - at - 1)).c_str());
			::sprintf(szBuf , "%lf" , a / b);
			m_rNPD[0] = szBuf;
		}
	}

	if(!m_rNPD[1].empty())
	{
		string::size_type at = 0;
		if(string::npos != (at = m_rNPD[1].find("/")))
		{
			double a = atof(m_rNPD[1].substr(at).c_str());
			double b = atof(m_rNPD[1].substr(m_rNPD[1].length() - ((m_rNPD[1].length() - at - 1)) , (m_rNPD[1].length() - at - 1)).c_str());
			::sprintf(szBuf , "%lf" , a / b);
			m_rNPD[1] = szBuf;
		}
	}

	return (7 == nCount);
}

void CIsoBMData::AddSubText(CDgnText* pDgnText2d , const string &rString)
{
	assert(pDgnText2d && "pDgnText2d is NULL");

	if(pDgnText2d) m_subTextEntry.insert(make_pair(pDgnText2d,rString));
}


/**	
	@brief	Iso Drawing�� BMCS code�� �����Ѵ�.

	@author	BHK

	@date	?

	@param	rIniFilePath	a parameter of type const string &

	@return	void	
*/
void CQCCheck::UpdateIsoBMCSCode(const string &rIniFilePath)
{
	CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();

	int size = m_pipeDatas.size();
	vector<CIsoBMData>::iterator itr = m_pipeDatas.begin();
	for(itr = m_pipeDatas.begin(); itr != m_pipeDatas.end() ; ++itr)
	{
		string rSpecName = itr->m_rMatlSpec, rMatlCode = itr->m_rMatlCode, rBMCSCode;
		
		try
		{
			bool bFoundCommodityCode = false;
			if(setting.UseDependentFile())
			{
				ofile << "get commodity code of pipe using dependent file..." << std::endl;

				string rCommodityCode;
				CDEPFile& depfile = CDEPFile::GetInstance();
				depfile.Read(setting.GetDependentFilePath());
				if(depfile.FindSysCommodityCodeWithDependtionCode(rCommodityCode , rMatlCode))
				{
					rMatlCode = rCommodityCode;
					bFoundCommodityCode = true;
				}
				ofile << "matl code is " << rMatlCode << std::endl;
			}

			if(setting.UseImpliedDataFile() && (false == bFoundCommodityCode))
			{
				ofile << "get commodity code of pipe using implied data file..." << std::endl;

				CIMPFile& impfile = CIMPFile::GetInstance();
				impfile.Read(setting.GetImpliedDataFilePath());
				impfile.FindBMCSCodeWithDependtionCode(rBMCSCode , rMatlCode);
			}

			if(!rMatlCode.empty() && rBMCSCode.empty())
			{
				string rItem;
				try
				{
					CPCDFile* pPcdFile =  GetPCDFile(rSpecName , rIniFilePath);
					if(pPcdFile && pPcdFile->FindItemWithMatlCode(rItem , rMatlCode))
					{
						rBMCSCode = rItem;
					}
				}
				catch(const exception& ex)
				{
					::OutputDebugString(ex.what());
				}
			}

			if(!rBMCSCode.empty())
			{
				ofile << "BMCS code is " << rBMCSCode << std::endl;
				itr->m_rBMCSSYM = rBMCSCode;
				itr->UpdateBMCSForDGNText2d();
			}
			else
			{
				ofile << "can't find BMCS symbol" << std::endl;
			}
		}
		catch(const exception& ex)
		{
			ofile << ex.what() << std::endl;
		}
	}
	for(itr = m_generalDatas.begin(); itr != m_generalDatas.end() ; ++itr)
	{
		string rSpecName = itr->m_rMatlSpec, rMatlCode = itr->m_rMatlCode, rBMCSCode;
		try
		{
			bool bFoundCommodityCode = false;
			if(setting.UseDependentFile())
			{
				ofile << "get commodity code of general using dependent file..." << std::endl;

				string rCommodityCode;
				CDEPFile& depfile = CDEPFile::GetInstance();
				depfile.Read(setting.GetDependentFilePath());
				if(depfile.FindSysCommodityCodeWithDependtionCode(rCommodityCode , rMatlCode))
				{
					rMatlCode = rCommodityCode;
					bFoundCommodityCode = true;
				}
				ofile << "matl code is " << rMatlCode << std::endl;
			}

			if(setting.UseImpliedDataFile() && (false == bFoundCommodityCode))
			{
				ofile << "get commodity code of general using implied data file..." << std::endl;

				string rCommodityCode;
				CIMPFile& impfile = CIMPFile::GetInstance();
				impfile.Read(setting.GetImpliedDataFilePath());
				impfile.FindBMCSCodeWithDependtionCode(rBMCSCode , rMatlCode);
			}

			if(!rMatlCode.empty() && rBMCSCode.empty())
			{
				string rItem;
				try
				{
					CPCDFile* pPcdFile =  GetPCDFile(rSpecName,rIniFilePath);
					if(pPcdFile && pPcdFile->FindItemWithMatlCode(rItem , rMatlCode))
					{
						rBMCSCode = rItem;
					}
				}
				catch(const exception& ex)
				{
					::OutputDebugString(ex.what());
				}
			}

			if(!rBMCSCode.empty())
			{
				ofile << "BMCS code is " << rBMCSCode << std::endl;
				itr->m_rBMCSSYM = rBMCSCode;
				itr->UpdateBMCSForDGNText2d();
			}
		}
		catch(const exception& ex)
		{
			::OutputDebugString(ex.what());
		}
	}

	//! ������ �ڵ�. �׽�Ʈ ���
	for(itr = m_miscellDatas.begin(); itr != m_miscellDatas.end() ; ++itr)
	{
		string rSpecName = itr->m_rMatlSpec, rMatlCode = itr->m_rMatlCode, rItem;
		try
		{
			if(setting.UseDependentFile())
			{
				string rCommodityCode;
				CDEPFile& depfile = CDEPFile::GetInstance();
				depfile.Read(setting.GetDependentFilePath());
				if(depfile.FindSysCommodityCodeWithDependtionCode(rCommodityCode , rMatlCode))
				{
					rMatlCode = rCommodityCode;
				}
			}

			CPCDFile* pPcdFile =  GetPCDFile(rSpecName,rIniFilePath);
			if(pPcdFile && pPcdFile->FindItemWithMatlCode(rItem , rMatlCode))
			{
				itr->m_rBMCSSYM = rItem;
				itr->UpdateBMCSForDGNText2d();
			}
		}
		catch(const exception& ex)
		{
			::OutputDebugString(ex.what());
		}
	}
}

/**	
	@brief	The CQCCheck::GetPCDFile function

	@author	BHK

	@date	?

	@param	matl_spec	a parameter of type const string &
	@param	rIniFilePath	a parameter of type const string &

	@return	CPCDFile*
*/
CPCDFile* CQCCheck::GetPCDFile(const string &matl_spec , const string &rIniFilePath)
{
	TCHAR szBuf[MAX_PATH + 1]={'\0',};
	STRING_T rSpecFolder;
	if(GetPrivateProfileString( "SmartISO_ntr" , "SpecFileFolder" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		rSpecFolder = szBuf;
	}

	STRING_T rSpecFilePath = rSpecFolder + matl_spec + string(".pcd");

	map<string , CPCDFile*>::iterator where = m_pcdFileMap.find(rSpecFilePath);
	if(m_pcdFileMap.end() != where)
	{
		return ((*where).second);
	}

	if(FileExist(rSpecFilePath))
	{
		CPCDFile* pPCDFile = CPCDFile::CreateInstance();
		if(NULL != pPCDFile)
		{
			m_pcdFileMap[rSpecFilePath] = pPCDFile;
			try
			{
				pPCDFile->Read(rSpecFilePath);
			}
			catch(const exception& ex)
			{
				::OutputDebugString(ex.what());
				///UNUSED_ALWAYS(ex);
			}

			return (pPCDFile);
		}
	}

	throw exception("GetPCDFile function error");
}

/**	
	@brief	The CIsoBMData::UpdateBMCSForDGNText2d function


	\return	void	
*/
void CIsoBMData::UpdateBMCSForDGNText2d()
{
	if(NULL != m_pDGNText2d)
	{
		/// update BMCS SYM
		string textString = m_pDGNText2d->textString();
		int at = m_nFieldPos[4];
		//char szBuf[MAX_PATH]={'\0',};
		//*szBuf = textString[at];
		string temp = textString.substr(m_nFieldPos[4] , m_nFieldPos[5] - m_nFieldPos[4]);
		IsString::TrimWhiteSpace(temp);
		int last = temp.length();
		int nBMlast = m_rBMCSSYM.length();
		
		string rBMCS = m_rBMCSSYM;

		if(last < nBMlast) last = nBMlast;

		/// ��ü�� ���ڿ��� ���̸� �����.
		for(int i =0; i< (last-nBMlast);++i)
		{
			rBMCS += " ";
		}
		
		if(at != NULL)
		{
			textString.replace(at,last,rBMCS);
			m_pDGNText2d->SetTextString(textString.c_str());	
		}

		///< commodity code�� ������ �ʿ䰡 �ִ°�?
	}
}

void CIsoBMData::SetDGNText2D(CDgnText *p)
{
	m_pDGNText2d = p;
}

void CQCCheck::CellMapping(CDgnDocument* pDgnDoc,const string &rCellName, const int &oriX, const int &oriY)
{
	
	/*
	string line, rTemp,rReplaceName,rOldCellName=rCellName,rFilePos;
	ifstream ifile, icellfile;
	char szBuf[MAX_PATH] ={'\0',};

	//////////////////////////////////////////////////////////////////////////
	/// Mapping Table���� ��ġ�ϴ� ���� �̸� �����´�.
	ifile.open("D:\\������Ʈ\\��ȭ�Ǽ�\\Bin\\CELL_MAPPING.TBL",ios::in);
	if(ifile)
	{
		while(!ifile.eof())
		{
			getline(ifile,line);
			int at = line.find("=");
			rTemp = line.substr(0,at-1);
			IsString::TrimWhiteSpace(rTemp);
			IsString::TrimWhiteSpace(rOldCellName);
			if(rOldCellName == rTemp)
			{
				rReplaceName = line.substr(at+1);
				IsString::TrimWhiteSpace(rReplaceName);
				break;
			}
		}
	}
	ifile.close();

	//////////////////////////////////////////////////////////////////////////
	///  ��ġ�ϴ� �����̸����� cell.txt���� FilePos�� ���Ѵ�.
	icellfile.open("D:\\������Ʈ\\��ȭ�Ǽ�\\CELL\\cell_����\\cell.txt",ios::in);
	if(icellfile)
	{
		while(!icellfile.eof())
		{
			getline(icellfile, line);
			int atEql = line.find("=");
			rTemp = line.substr(0,atEql-1);
			IsString::TrimWhiteSpace(rTemp);
			if(rReplaceName == rTemp)
			{
				int atComma = line.find(",");
				int n = atComma - atEql;
				rFilePos = line.substr(atEql+1,n-1);
				break;
			}
		}
	}
	icellfile.close();

	//////////////////////////////////////////////////////////////////////////
	/// 
	
	if(m_DgnOpen)
	{
		unsigned long nFilePos = atol(rFilePos.c_str());
		CIsDGNParser parser(NULL);
		parser.AddParsingType(DGNT_CELL_HEADER);
		parser.AddParsingType(DGNT_SHAPE);
		if(m_dgnFile.GotoFilePos(nFilePos))
		{
			m_dgnFile.ReadElement(); ///< �ϳ��� element�� ���Ѵ�.
			CDgnElement* pEnt = parser.ParseElement(&m_dgnFile);
			if(pEnt->IsKindOf(CDgnCellHeader::TypeString()))
			{
				CDgnCellHeader* pCellHeader2d = (CDgnCellHeader*)(pEnt);
				DGNPoint2d origin;
				origin.x = oriX;
				origin.y = oriY;
				pCellHeader2d->Offset(oriX - pCellHeader2d->origin().x , oriY - pCellHeader2d->origin().y);
				pDgnDoc->Add(pCellHeader2d);
			}
		}
	}
	*/
}

struct compareFnc : binary_function<mapping , mapping , bool>
{
	bool operator()(const mapping& lhs , const mapping& rhs) const
	{
		if(lhs.rSupportName == rhs.rSupportName)
		{
			if( (0 == lhs.low) && (0 == lhs.high) ) return true;
			
			return ((lhs.low - rhs.low) * (lhs.high - rhs.high/*rhs.low*/) <= 0);
		}

		return false;
	}
};

struct compareName : binary_function<cellInfo, cellInfo, bool>
{
	bool operator()(const cellInfo& lhs, const cellInfo& rhs) const
	{
		if(lhs.rCellName == rhs.rCellName)
		{
			return true;
		}

		return false;
	}
};

extern vector<string> __DELETED_CELL_ENTRY__;

/**	
	@brief	Support���� Cell������ �����ؼ� DGN�� �׷��ش�.

	@author	HumKyung.BAEK

	@param	pDgnDoc	a parameter of type CDgnDocument*
  
	@return	void	
*/
void CQCCheck::ReplaceSupport(CDgnDocument* pDgnDoc)
{
	CIdmsNtrSetting& pSetting = CIdmsNtrSetting::GetInstance();
	long oriX = pSetting.m_CellX, oriY = pSetting.m_CellY , CellDistance = pSetting.m_Celldistance;
	string rCellInfoPath = pSetting.m_rCellInfoPath, rMapPath = pSetting.m_rMapTablePath;
	string rCelldirection = pSetting.m_Celldirection;
	IsString::ToUpper(rCelldirection);
	ifstream ifile, icellfile;
	string line;
	int count = 0;

	char szBuf[MAX_PATH] = {'\0' ,};
	vector<string> oResult;
	vector<mapping> Map;
	CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();
	const double nCellHalfWidth = setting.GetCellDistance() * 0.5;
	vector<cellInfo> CellInfo;
	if(!rCellInfoPath.empty())
	{
		icellfile.open(rCellInfoPath.c_str(),ios::in);
		if(icellfile.is_open())
		{
			//////////////////////////////////////////////////////////////////////////
			/// type�� ��ġ�ϴ°��� name�� ���Ѵ�.
			ifile.open(rMapPath.c_str(),ios::in);
			mapping Maptemp;
			if(ifile)
			{
				while(!ifile.eof())
				{
					getline(ifile,line);
					if(ifile.eof()) break;

					CTokenizer<CIsComma>::Tokenize(oResult , line , CIsComma());
					if(4 == oResult.size())
					{
						Maptemp.rCellName = oResult[0];
						Maptemp.low = atof(oResult[1].c_str()); Maptemp.high = atof(oResult[2].c_str());
						Maptemp.rSupportName = oResult[3];
						Map.push_back(Maptemp);
					}
				}
			}
			ifile.close();
		
			//////////////////////////////////////////////////////////////////////////
			///  cell.txt���� FilePos�� ���Ѵ�.
			cellInfo CellTemp;
			if(icellfile)
			{
				while(!icellfile.eof())
				{
					getline(icellfile, line);
					CTokenizer<CIsFromString>::Tokenize(oResult,line,CIsFromString("="));
					IsString::TrimWhiteSpace(oResult[0]);
					CellTemp.rCellName = oResult[0];
					vector<string> oFilepos;
					CTokenizer<CIsComma>::Tokenize(oFilepos,oResult[1],CIsComma());
					CellTemp.pos = atof(oFilepos[0].c_str());
					CellInfo.push_back(CellTemp);
				}
			}

			ifile.close();
			icellfile.close();
		}
	}


	vector<mapping> already_inserted;
	for(vector<CIsoBMData>::iterator itr = m_supportDatas.begin() ; itr != m_supportDatas.end() ; ++itr,++count)
	{
		CTokenizer<CIsFromString>::Tokenize(oResult,itr->matlCode(),CIsFromString("-")); ///< '-' ���� ���ڸ� support type���� �����Ѵ�.
		string rTextString = oResult[0];
		string rNPD1 = itr->NPD1(), rNPD2 = itr->NPD2();
		double nNPD1 = atof(rNPD1.c_str()) , nNPD2 = 0.f;
		nNPD2 = ( rNPD2.empty() ) ? nNPD1 : atof( rNPD2.c_str() );
		
		CDgnText* pDGNText2d = itr->GetDGNText2D();
		if(NULL != pDGNText2d)
		{
			//! SUPPORT �̸��� �� ��쿡�� 2������ �������� �ִ�.
			const string rTextString = pDGNText2d->textString();
			string rNewTextString;
			string::size_type at = rTextString.find("SUPPORT DATA: ");
			if((string::npos != at) && itr->m_subTextEntry.empty())
			{
				int nUnderbars = 0 , i = 0;
				const int nStart = at + ::strlen("SUPPORT DATA: ");
				rNewTextString = rTextString.substr(0 , nStart);
				for(i = nStart;i < int(rTextString.length());++i)
				{
					if(' ' == rTextString[i]) break;
					if('_' != rTextString[i]) 
						rNewTextString += rTextString[i];
					else	++nUnderbars;
				}

				for(int j = 0;j < nUnderbars;++j) rNewTextString += ' ';

				rNewTextString += rTextString.substr(i);
				pDGNText2d->SetTextString(rNewTextString.c_str());
			}
			else if((string::npos != at) && (1 == itr->m_subTextEntry.size()))
			{
				//! 2������ ������ ���.
				CDgnText* pDGNSubText2d = itr->m_subTextEntry.begin()->first;
				const string rTextString = pDGNSubText2d->textString();
				string rNewTextString;
				int nUnderbars = 0 , i = 0;
				int nStart = 0;
				for(nStart = 0;(' ' == rTextString[nStart]) && (nStart < int(rTextString.length()));++nStart);
				rNewTextString = rTextString.substr(0 , nStart);
				for(i = nStart;i < int(rTextString.length());++i)
				{
					if(' ' == rTextString[i]) break;
					if('_' != rTextString[i]) 
						rNewTextString += rTextString[i];
					else	++nUnderbars;
				}
				
				for(int j = 0;j < nUnderbars;++j) rNewTextString += ' ';
				
				rNewTextString += rTextString.substr(i);
				pDGNSubText2d->SetTextString(rNewTextString.c_str());
			}
		}

		/*
		//! special�� ���� CELL�� �Ѹ��� �ʵ��� �Ѵ�.
		if((rTextString.length() >= 3) && (0 == strncmp(rTextString.c_str() , "SPS" , 3)))
			continue;
		*/

		//! ���������� ������ CELL�� �ƴ� ��쿡�� OVERWRITE���� �ʴ´�.
		///vector<string>::iterator where = find(__DELETED_CELL_ENTRY__.begin() , __DELETED_CELL_ENTRY__.end() , rTextString);
		///if(where == __DELETED_CELL_ENTRY__.end()) continue;
		
		mapping key;
		key.rSupportName = rTextString;
		key.low = nNPD1;
		key.high= nNPD2;

		/// CELL�� �̹� ���ԵǾ����� �˻��Ѵ�. ���ԵǾ��ٸ� �ǳ� �ڴ�.
		vector<mapping>::iterator jtr = find_if(already_inserted.begin() , already_inserted.end() , bind2nd(compareFnc(), key));
		if(jtr != already_inserted.end())
		{
			//! SPECIAL�� ���� SEE DETAIL ? �� �κ��� SEE DETAIL 0���� �����ϱ� ����...
			FindSupportDetailNoAndReplaceSpecialDetailNo( m_graphicAreaDatas , itr->matlCode() );
			continue;
		}

		/// �����ϴ� CELL�� ������ �ǳʶڴ�.
		jtr = find_if(Map.begin(),Map.end(),bind2nd(compareFnc() , key));
		///if(jtr == Map.end()) continue;
		string rTemp;
		if( jtr != Map.end() ) 
			rTemp = jtr->rCellName;
		else	rTemp = "ERROR";

		cellInfo cKey;
		cKey.rCellName = rTemp;

		vector<cellInfo>::iterator ktr = find_if(CellInfo.begin(),CellInfo.end(),bind2nd(compareName(), cKey));
		if(ktr == CellInfo.end()) continue;
		already_inserted.push_back(key);

		double nFilePos = ktr->pos;
		
		//////////////////////////////////////////////////////////////////////////
		/// DGN���Ͽ� �׸���
		if(m_DgnOpen)
		{
			CDgnParser parser(NULL);
			parser.AddParsingType(CELL_HEADER_ELM);
			parser.AddParsingType(DGNT_SHAPE);
			if(m_dgnFile.GotoFilePos(long(nFilePos)))
			{
				m_dgnFile.ReadElement(); ///< �ϳ��� element�� ���Ѵ�.
				CDgnElement* pEnt = parser.ParseElement(&m_dgnFile);
				if(pEnt->IsKindOf(CDgnCellHeader::TypeString()))
				{
					CDgnCellHeader* pCellHeader2d = (CDgnCellHeader*)(pEnt);
					DGNPoint2d origin;
					if(0 != count) GetNextCellPos( oriX , oriY );
					origin.x = oriX;
					origin.y = oriY;

					if( rTemp != "ERROR" )
					{
						pCellHeader2d->Offset(oriX - pCellHeader2d->origin().x , oriY - pCellHeader2d->origin().y);
					}
					else
					{
						double x = (setting.m_graphic_volume.minx() + setting.m_graphic_volume.maxx()) * 0.5 - 3055;
						double y = (setting.m_graphic_volume.miny() + setting.m_graphic_volume.maxy()) * 0.5 - 3055;
						pCellHeader2d->Offset( x - pCellHeader2d->origin().x ,  y - pCellHeader2d->origin().y);
					}
					pDgnDoc->Add(pCellHeader2d);

					if( rTemp != "ERROR" )
					{
						int nDetailNo = FindSupportDetailNoAndReplaceSpecialDetailNo( m_graphicAreaDatas , itr->matlCode() );
						CDgnText* pText2ad = static_cast<CDgnText*>(CDgnText::CreateInstance());
						if(pText2ad)
						{
							DPoint3d pt;
							pt.x = origin.x + nCellHalfWidth;
							pt.y = origin.y + 12;
							pt.z = 0.0;

							::sprintf( szBuf , _T("%d") , nDetailNo );
							pText2ad->SetLineWeight(1);	
							pText2ad->Set( setting.m_font , pt , szBuf );
							pText2ad->SetTextWidth( 18 * 2);
							pText2ad->SetTextHeight( 24 * 2 );
							pDgnDoc->Add(pText2ad);
						}
					}
				}
			}
		}
	}
}

/**	
	@brief	SUPPORT�� DETAIL NUMBER�� ���ϰ� SPECIAL SUPPORT�� ��쿡�� DETAIL NO�� 0���� �ٲ۴�.

	@author	HumKyung.Baek

	@param	graphicAreaTextEntry	a parameter of type vector<CIsoBMData*>&
	@param	rSupportNo	a parameter of type const string&

	@return	void	
*/
int CQCCheck::FindSupportDetailNoAndReplaceSpecialDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const string& rSupportNo )
{
	static const string SEE_DETAIL("SEE DETAIL");

	CIsPoint2d origin;
	vector<CIsoBMData>::iterator itr = graphicAreaTextEntry.begin() ;
	for(itr = graphicAreaTextEntry.begin() ; itr != graphicAreaTextEntry.end() ; ++itr)
	{
		string rText = (itr)->dataString();
		if( rText == rSupportNo )
		{
			origin = (itr)->origin();
			break;
		}
	}

	CIsoBMData* pIsoBMData = NULL;
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
				pIsoBMData = &(*itr);
			}
		}
	}

	if( !rLastSeeDetailText.empty() && (nMinDist < 100.f))
	{
		if(0 == strncmp(rSupportNo.c_str() , ("SPS") , 3))
		{
			//! this is special support so change detail text.
			if(pIsoBMData) pIsoBMData->SetDGNTextString( ("SEE DETAIL 0") );
			return 0;
		}
		else
		{
			string rDetailNo = rLastSeeDetailText.substr( 11 ); 
			return atoi( rDetailNo.c_str() );
		}
	}

	return (0 == strncmp(rSupportNo.c_str() , "SPS" , 3)) ? 0 : 1;
}

void CQCCheck::GetNextCellPos(long &oriX, long &oriY)
{
	CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();
	string rCelldirection = setting.m_Celldirection;
	long CellDistance = setting.m_Celldistance;

	if( rCelldirection == "R")
	{
		oriX += CellDistance;
	}
	else if(rCelldirection == "L")
	{
		oriX -= CellDistance;
	}
	else if(rCelldirection == "T")
	{
		oriY += CellDistance;
	}
	else if(rCelldirection == "B")
	{
		oriY -= CellDistance;
	}
}

/**
	@brief	

	@author	BHK
*/
int CIsoBMData::SetDGNTextString(const STRING_T &str)
{
	if(m_pDGNText2d) m_pDGNText2d->SetTextString(str.c_str());

	return ERROR_SUCCESS;
}

static bool SortByWeldNo(const WeldNoInfo& lhs , const WeldNoInfo& rhs)
{
	return (atoi(lhs.WeldNo.c_str()) > atoi(rhs.WeldNo.c_str()));
}

/**
	@brief	WELD NO�� DGN���Ͽ� �����մϴ�.

	@author	BHK
*/
int CQCCheck::WriteWeldNo(CDgnDocument *pDgnDoc)
{
	assert(pDgnDoc && "pDgnDoc is NULL");

	if(pDgnDoc)
	{
		const int TEXT_HEIGHT = 60;
		int nMaxY = -1;
		{
			for(vector<CIsoBMData>::iterator itr = m_WeldNoAreaDatas.begin();itr != m_WeldNoAreaDatas.end();++itr)
			{
				if(itr == m_WeldNoAreaDatas.begin())
					nMaxY = int(itr->GetDGNText2D()->origin().y);
				else if(nMaxY < itr->GetDGNText2D()->origin().y)
					nMaxY = int(itr->GetDGNText2D()->origin().y);
			}
		}
		{
			//! WRITE�� ������ �ľ��Ͽ� Y�� ��ġ�� �����Ѵ�.
			for(vector<CBoxTextString*>::iterator itr = m_boxTextStringEntry.begin();itr != m_boxTextStringEntry.end();++itr)
			{
				if(1 == (*itr)->m_textStringEntry.size())
				{
					const string rTextString = (*itr)->m_textStringEntry[0]->textString();
					string::size_type nPos = rTextString.find("WELD NO: ");
					if(0 == nPos)	//! WELD NO:�� �����ϴ� ������ ���....
					{
						nMaxY += TEXT_HEIGHT;
					}
				}
			}
		}

		CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();
		
		vector<WeldNoInfo> weld_no_info_entry;
		int y = nMaxY;
		for(vector<CBoxTextString*>::iterator itr = m_boxTextStringEntry.begin();itr != m_boxTextStringEntry.end();++itr)
		{
			if(1 == (*itr)->m_textStringEntry.size())
			{
				const DPoint3d TextPt2d  = (*itr)->m_textStringEntry[0]->origin();
				const string rTextString = (*itr)->m_textStringEntry[0]->textString();
				string::size_type nPos = rTextString.find("WELD NO: ");
				if(0 == nPos)	//! WELD NO:�� �����ϴ� ������ ���....
				{
					WeldNoInfo weld_no_info;

					vector<string> oResult;
					const string rWeldNo = rTextString.substr(9);
					CTokenizer<CIsFromString>::Tokenize(oResult , rWeldNo , CIsFromString("-"));
					int nCount = 0;
					for(vector<string>::iterator jtr = oResult.begin();(jtr != oResult.end()) && (nCount < 4);++jtr,++nCount)
					{
						CIsPoint2d origin(setting.m_WeldNoXPos[nCount] , y);
												
						weld_no_info.pts[nCount] = origin;
						if(0 == nCount)
						{
							weld_no_info.WeldNo = (*jtr);
						}
						else if(1 == nCount)
						{
							weld_no_info.SF = (*jtr);
						}
						else if(2 == nCount)
						{
							weld_no_info.NPD = (*jtr);
						}
					}
					if(3 == nCount)
					{
						double nMinDist = -1;
						for(vector<CIsoBMData>::iterator jtr = m_graphicAreaDatas.begin();jtr != m_graphicAreaDatas.end();++jtr)
						{
							if(string("RPAD") == string(jtr->GetDGNText2D()->textString()))
							{
								DPoint3d pt2d = jtr->GetDGNText2D()->origin();
								if(-1 == nMinDist)
									nMinDist = ::DistanceTo(TextPt2d , pt2d);
								else if(nMinDist > ::DistanceTo(TextPt2d , pt2d))
									nMinDist = ::DistanceTo(TextPt2d , pt2d);
							}
						}

						if((nMinDist > 0) && (nMinDist < 100))
						{
							CIsPoint2d origin(setting.m_WeldNoXPos[nCount] , y);
									
							weld_no_info.pts[nCount] = origin;
							weld_no_info.WeldType = "SOB";
						}
						else
						{
							CIsPoint2d origin(setting.m_WeldNoXPos[nCount] , y);
														
							weld_no_info.pts[nCount] = origin;
							weld_no_info.WeldType = "BW";
						}
					}
					y -= TEXT_HEIGHT;
					
					weld_no_info_entry.push_back(weld_no_info);
				}
			}
		}

		{
			//! WELD NO.�� ����
			stable_sort(weld_no_info_entry.begin() , weld_no_info_entry.end() , SortByWeldNo);
			
			//! APPEND MODE�� ������ ���ϴ�.
			ofstream ofile(setting.m_rWFilePath.c_str() , ios::app);
			static char buf[64] = {'\0',};
			y = nMaxY;
			for(vector<WeldNoInfo>::iterator itr = weld_no_info_entry.begin();itr != weld_no_info_entry.end();++itr)
			{
				string aLine;

				CDgnText* pText2d = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2d)
				{
					::sprintf(buf , "%-11s" , itr->WeldNo.c_str());
					aLine += buf;

					DPoint3d origin;
					origin.x = itr->pts[0].x();
					origin.y = y;
					origin.z = 0.0;
					
					pText2d->SetLineWeight(1);
					pText2d->Set(setting.m_WeldNoFont , origin , itr->WeldNo.c_str());
					pText2d->SetTextWidth(32);
					pText2d->SetTextHeight(42);
					pDgnDoc->Add(pText2d);
				}

				pText2d = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2d)
				{
					::sprintf(buf , "%-9s" , itr->SF.c_str());
					aLine += buf;
					
					DPoint3d origin;
					origin.x = itr->pts[1].x();
					origin.y = y;
					origin.z = 0.0;
					
					pText2d->SetLineWeight(1);
					pText2d->Set(setting.m_WeldNoFont , origin , itr->SF.c_str());
					pText2d->SetTextWidth(32);
					pText2d->SetTextHeight(42);
					pDgnDoc->Add(pText2d);
				}

				pText2d = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2d)
				{
					::sprintf(buf , "%-10s" , itr->NPD.c_str());
					aLine += buf;

					DPoint3d origin;
					origin.x = itr->pts[2].x();
					origin.y = y;
					origin.z = 0.0;
					
					pText2d->SetLineWeight(1);
					pText2d->Set(setting.m_WeldNoFont , origin , itr->NPD.c_str());
					pText2d->SetTextWidth(32);
					pText2d->SetTextHeight(42);
					pDgnDoc->Add(pText2d);
				}

				pText2d = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2d)
				{
					aLine += itr->WeldType;
					
					DPoint3d origin;
					origin.x = itr->pts[3].x();
					origin.y = y;
					origin.z = 0.0;
					
					pText2d->SetLineWeight(1);
					pText2d->Set(setting.m_WeldNoFont , origin , itr->WeldType.c_str());
					pText2d->SetTextWidth(32);
					pText2d->SetTextHeight(42);
					pDgnDoc->Add(pText2d);
				}
				y -= TEXT_HEIGHT;
				if(ofile.is_open()) ofile << aLine << std::endl;
			}
		}
	}

	return ERROR_SUCCESS;
}
