// SmartISO_ntr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include "QCCheck.h"

#include <DgnEnum.h>
#include <DgnV7Lib.h>

#include <fstream>
#include <string>
#include "IdmsNtrSetting.h"

string MakeOutputIsoFilePath( const string& rSourceIsoFilePath , const string& rOutputFolder )
{
	string rOutputFileName = rSourceIsoFilePath;
	string::size_type at = rOutputFileName.find_last_of( "\\" );
	if( string::npos != at )
	{
		rOutputFileName = rOutputFileName.substr( at + 1 );
	}
	string rOutputFilePath = rOutputFolder;
	if( '\\' != rOutputFilePath.at( rOutputFilePath.size() - 1 ) ) rOutputFilePath += "\\";
	rOutputFilePath += rOutputFileName;

	return rOutputFilePath;
}

struct IsInVolume : binary_function< CDgnVolume , DPoint3d , bool >
{
	bool operator()( const CDgnVolume& lhs , const DPoint3d& rhs ) const
	{
		bool res = (rhs.x >= lhs.minx()) && (rhs.x <= lhs.maxx()) && (rhs.y >= lhs.miny()) && (rhs.y <= lhs.maxy());
		return res;
	}
};

vector<string> __DELETED_CELL_ENTRY__;

/**	
	@brief		������ SUPPORT CELL�� �����.

	@remark		SUPPORT CELL�� ������ ����ִ� TEXT�� ���� �����.

	@author		BHK
**/	
void DeleteOriginalCell( CDgnDocument* pDgnDoc )
{
	assert( pDgnDoc && "pDgnDoc is NULL" );

	if( pDgnDoc )
	{
		vector< CDgnVolume > CellVolumeEntry;

		__DELETED_CELL_ENTRY__.clear();
		//////////////////////////////////////////////////////////////////////////
		// Cell Delete�κ� 
		const int nCount = pDgnDoc->GetEntityCount();
		int i = 0;
		for(i = 0;i < nCount;++i)
		{
			CDgnElement* pEnt = pDgnDoc->GetEntityAt(i);
			if( pEnt->IsKindOf(CDgnCellHeader::TypeString() ))
			{
				CDgnCellHeader* pCell = (CDgnCellHeader*)pEnt;
				__DELETED_CELL_ENTRY__.push_back(pCell->name());

				pEnt->Delete();
				CellVolumeEntry.push_back( pEnt->volume() );
			}
		}

		for( i = 0;i < nCount;++i)
		{
			CDgnElement* pEnt = pDgnDoc->GetEntityAt(i);
			if( pEnt->IsKindOf( CDgnText::TypeString() ))
			{
				CDgnText* pText2d = static_cast< CDgnText* >( pEnt );
				vector<CDgnVolume>::iterator where = find_if( CellVolumeEntry.begin() , CellVolumeEntry.end() , bind2nd( IsInVolume() , pText2d->origin() ));
				if( CellVolumeEntry.end() != where )
				{
					pText2d->Delete();
				}
			}
		}
	}
}

/**	
	@brief	NTR ������ �о� DGN ���Ͽ� �����Ѵ�.

	@author	�����
*/
void WriteNTRFile( CDgnDocument* pDgnDoc , const string& rNTRFilePath , ofstream& error_log )
{
	static const int START_OF_MATLCODE_IN_NTR = 46;
	assert( pDgnDoc && "pDgnDoc is NULL" );
	
	if( pDgnDoc )
	{
		CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();

		string line;
		ifstream ifile;
		ifile.open( rNTRFilePath.c_str() );
		if( ifile.good() )
		{
			int x = setting.m_x , y = setting.m_y;
			while(!ifile.eof())
			{
				getline(ifile , line);
				
				error_log << line << std::endl;	///< for debug
				
				CDgnText* pText2ad = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2ad)
				{
					DPoint3d origin;
					origin.x = x; origin.y = y; origin.z = 0.0;
					
					pText2ad->SetLineWeight(1);
					
					if( ('G' == line[START_OF_MATLCODE_IN_NTR]) || ('B' == line[START_OF_MATLCODE_IN_NTR]) ) ///< GASKET�̳� BOLT���� Ȯ���Ѵ�.
					{
						for( int i = START_OF_MATLCODE_IN_NTR;' ' != line[i];++i);	//! 46��° ���� ������ ��Ÿ���������� commodity code�̴�.
						
						//! �븲������ GASKET�̳� BOLT�� ��쿡 commodity code�� �ǵ� 3�ڸ��� �ڸ����� ��ȭ������ �׷��� ���� �ʴ´�.
						///line.replace(i - 3 , 3 , "   ");				//! 
					};
					
					pText2ad->Set(setting.m_font , origin , line.c_str());
					pText2ad->SetTextWidth(18);
					pText2ad->SetTextHeight(24);
					pDgnDoc->Add(pText2ad);
				}
				y -= 40;
			}
		}
		else
		{
			error_log << "NTR ������ ã�� �� �����ϴ�." << std::endl;
		}
		ifile.close();
	}
}

#include "DEPFile.h"
#include "IMPFile.h"
ofstream ofile;

/**
	@brief	start section

	@author	BHK
*/
int main(int argc, char* argv[])
{
	TCHAR szBuf[MAX_PATH + 1]={'\0',};

	/// �Է� ���� : ISO ���� �̸� , B ���� �̸� , OUTPUT ���� ��� , W ���� �̸�
	if((4 == argc) || (5 == argc))
	{
		//! open log file
		string log_file = argv[0];
		int at = log_file.find(".");
		log_file.replace(at+1,3,"log");

		ofile.open(log_file.c_str() , ios::app); 
		for(int i = 0;i < argc;++i)
		{
			ofile << argv[i] << " - ";
		}
		ofile << std::endl;
		//!

		/// get ini file name from execution file name
		string ini_file = argv[0];
		at = ini_file.find(".");
		ini_file.replace(at+1,3,"ini");
		/// up to here
		
		static const string rApp = "SmartISO_ntr";
		string rPCDFilePath;
		vector<string> oResult;
		
		string rOutputFilePath = MakeOutputIsoFilePath( argv[1] , argv[3] );

		CIdmsNtrSetting& setting = CIdmsNtrSetting::GetInstance();
		if(5 == argc) setting.m_rWFilePath = argv[4];
		setting.Read(ini_file);
		
		ofile << "reading dependent & implied data file..." << std::endl;
		CDEPFile& depfile = CDEPFile::GetInstance();
		depfile.Read(setting.GetDependentFilePath());
		CIMPFile& impfile = CIMPFile::GetInstance();
		
		CDgnDocument* pDgnDoc = CDgnDocument::CreateInstance();
		//////////////////////////////////////////////////////////////////////////
		/// �Ľ��� DGN Ÿ���� �Է��Ѵ�.
		vector<int> parsingTypes;
		parsingTypes.push_back(TEXT_ELM);
		parsingTypes.push_back(CELL_HEADER_ELM);
		parsingTypes.push_back(LINE_STRING_ELM);
		
		/*
		if( FALSE == CopyFile( argv[1] , rOutputFilePath.c_str() , FALSE ) )
		{
			::MessageBox( NULL ,  "������ ������ �� �����ϴ�." , "MSG" , MB_OK );
			return -1;
		}
		*/

		if(pDgnDoc->OpenForWrite( argv[1] , CDgnDocument::UOR_BASE , NULL , &parsingTypes))
		{	
			//! write ntr file
			WriteNTRFile( pDgnDoc , argv[2] , ofile );
			if(GetPrivateProfileString(rApp.c_str() , "REMOVE_ORIGINAL_CELL" , NULL , szBuf , MAX_PATH , ini_file.c_str()))
			{
				if(0 == stricmp("ON" , szBuf))
				{
					DeleteOriginalCell( pDgnDoc );	//! delete cell in iso dgn file.
				}
			}

			try
			{
				CQCCheck vQC;
				vQC.ExtractTextData(pDgnDoc , ini_file);
				vQC.UpdateIsoBMCSCode( ini_file );
				vQC.ReplaceSupport(pDgnDoc);
				if(5 == argc) vQC.WriteWeldNo(pDgnDoc);
			}
			catch(...)
			{
				ofile << "iso file�� �����ϴ� �߿� ������ �߻��߽��ϴ�." << std::endl;
			}

			/// SUPPORT CELL�� ������ ���Ѵ�.
			/// CELL ����Ʈ ���Ͽ��� ������ CELL�� �����͸� ���Ѵ�.

			/// write NTR , update BMCS SYM code
			pDgnDoc->Write();
			
			CDgnDocument::DeleteInstance(pDgnDoc);
		}
		else
		{
			ofile << "ISO ����(" << rOutputFilePath.c_str() << ")�� ������ �����ϴ�." << std::endl;
		}

		ofile.close();
	}	
	else
	{
		printf("usage : SmartISO_ntr.exe <iso file> <b file> <output folder> <w file>\n");
	}

	return 0;
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   DistanceTo
    @return     double
    @param      const       DPoint3d
    @param      pt1         const
    @param      DPoint3d&   pt2
    @brief
******************************************************************************/
double DistanceTo(const DPoint3d& pt1 , const DPoint3d& pt2)
{
	const double dx = pt2.x - pt1.y;
	const double dy = pt2.y - pt1.y;
	const double dz = pt2.z - pt1.z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}