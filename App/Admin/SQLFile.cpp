#include "StdAfx.h"
#include <assert.h>
#include "SQLFile.h"
#include "Tokenizer.h"

#include <fstream>

CSQLFile::CSQLFile(const string& rFilePath) : m_rFilePath(rFilePath)
{
}

CSQLFile::~CSQLFile(void)
{
}

/**
	@brief	SQL ������ �Ľ��Ѵ�.

	@author	BHK
*/
int CSQLFile::Parse(const string& rFilePath)
{
	if(!rFilePath.empty()) m_rFilePath = rFilePath;
	ifstream ifile(m_rFilePath.c_str());
	if(ifile.is_open())
	{
		bool bFieldSection = false , bPrimaryKeySection = false;
		long nLineNo = 0;
		string aLine;
		while(!ifile.eof())
		{
			getline(ifile , aLine);
			if(ifile.eof()) break;
			if(aLine.empty() || (0 == aLine.find(_T("--")))) continue;	//! �����̳� �ּ��� ����...

			if(0 == nLineNo) m_rTableName = aLine;
			if(('[' == aLine[0]) && (']' == aLine[aLine.length() - 1]))
			{
				if(_T("[FIELDS]") == aLine) 
				{
					bFieldSection = true;
					bPrimaryKeySection = false;
				}
				else if("[PRIMARY KEY]" == aLine)
				{
					bFieldSection = false;
					bPrimaryKeySection = true;
				}
				else
				{
					bFieldSection = false;
					bPrimaryKeySection = false;
				}
				continue;
			}

			if(bFieldSection) m_FieldEntry.push_back(aLine);
			else if(bPrimaryKeySection) m_PrimaryKeyEntry.push_back(aLine);
			
			++nLineNo;
		}
		ifile.close();

		return ERROR_SUCCESS;
	}

	return ERROR_FILE_NOT_FOUND;
}

/**
	@brief	���̺� ���� SQL ���� �����Ѵ�.

	@author	BHK
*/
string CSQLFile::GetCreateString(void) const
{
	string rSql=_T("CREATE TABLE ") + m_rTableName + _T("(");
	for(vector<string>::const_iterator itr = m_FieldEntry.begin();itr != m_FieldEntry.end();++itr)
	{
		rSql += (*itr) + _T(",");
	}
	
	if(!m_PrimaryKeyEntry.empty())
	{
		rSql += _T(" PRIMARY KEY (");
		for(vector<string>::const_iterator jtr = m_PrimaryKeyEntry.begin(); jtr != m_PrimaryKeyEntry.end();++jtr)
		{
			rSql += (*jtr) + _T(",");
		}
		if(',' == rSql[rSql.length() - 1]) rSql = rSql.substr(0 , rSql.length() - 1);
		rSql += _T("))");
	}
	else
	{
		if(',' == rSql[rSql.length() - 1]) rSql = rSql.substr(0 , rSql.length() - 1);
		rSql += _T(")");
	}

	///if(',' == rSql[rSql.length() - 1]) rSql = rSql.substr(0 , rSql.length() - 1);
	///rSql += _T(")");
	
	return rSql;
}

/**
	@brief	

	@author	BHK
*/
int CSQLFile::GetFieldCount(void) const
{
	return (int)(m_FieldEntry.size());
}

const static CString __MSAccessDataTypes__[] =
{
	_T("IDENTITY"),
	_T("LONGBINARY"),	//! BIGBINARY[1],LONG BINARY,OLE
	_T("BINARY"),		//! BINARY
	_T("BIT"),		//! BIT
	_T("COUNTER"),		//! COUNTER
	_T("CURRENCY"),		//! CURRENCY
	_T("DATETIME"),		//! DATE/TIME
	_T("GUID"),		//! GUID
	_T("LONGTEXT"),		//! LONG TEXT
	_T("SINGLE"),		//! NUMBER (FieldSize= SINGLE)
	_T("DOUBLE"),		//! NUMBER (FieldSize= DOUBLE)
	_T("UNSIGNEDBYTE"),	//! NUMBER (FieldSize= BYTE)
	_T("SHORT"),		//! NUMBER (FieldSize= INTEGER)
	_T("NUMERIC"),		//! NUMERIC
	_T("VARCHAR"),		//! TEXT
	_T("VARBINARY")		//! VARBINARY
};

/**
	@brief	�ʵ� �̸��� ���մϴ�.

	@author	HumKyung.BAEK
*/
string CSQLFile::GetFieldNameAt(const int& at) const
{
	assert(at < m_FieldEntry.size() && _T("range error"));

	if(at < (int)(m_FieldEntry.size()))
	{
		CString rFieldName;
		CString rFieldData = m_FieldEntry[at].c_str();
		rFieldData.TrimLeft();
		rFieldData.TrimRight();
		rFieldData.MakeUpper();
		if('[' == rFieldData[0])
		{
			const int nFound = rFieldData.Find(_T("]"));
			if(-1 != nFound) rFieldName = rFieldData.Left(nFound + 1);
		}
		else
		{
			vector<STRING> oResult;
			CTokenizer<CIsSpace>::Tokenize(oResult , m_FieldEntry[at] , CIsSpace());
			if(!oResult.empty()) rFieldName = oResult[0].c_str();
		}

		if(('[' == rFieldName[0]) && (']' == rFieldName[rFieldName.GetLength() - 1]))
		{
			rFieldName = rFieldName.Mid(1 , rFieldName.GetLength() - 2);
		}
#ifdef	_UNICODE
		return rFieldName.operator LPCTSTR();
#else
		return rFieldName.operator LPCSTR();
#endif
	}

	return _T("");
}

/**
	@brief	

	@author	BHK
*/
string CSQLFile::GetFieldDataAt(const int& at) const
{
	assert(at < m_FieldEntry.size() && _T("range error"));

	if(at < (int)(m_FieldEntry.size())) return m_FieldEntry[at];

	return _T("");
}

/**
	@brief	

	@author	BHK
*/
bool CSQLFile::ExistFieldName(const string& rFieldName)
{
	const int nFieldCount = GetFieldCount();
	for(int i = 0;i < nFieldCount;++i)
	{
		if(rFieldName == GetFieldNameAt(i)) return true;
	}

	return false;
}

/**
	@brief	SQL������ ����� ���̺��� ������ ��ġ��Ų��.

	@author BHK	

	@date 2009-08-26 ���� 10:22:20	

	@param	

	@return		
*/
int CSQLFile::SyncToTable(CADODB& adoDB, const string& rTableName) const
{
	CStringList TableNameList;
	adoDB.GetTableNameList(TableNameList);

	BOOL bTableExists = FALSE;
	CString _rTableName(rTableName.c_str());
	_rTableName.MakeUpper();
	POSITION pos = TableNameList.GetHeadPosition();
	while(NULL != pos)
	{
		CString rValue = TableNameList.GetNext(pos);
		rValue.MakeUpper();
		if(_rTableName == rValue)
		{
			bTableExists = TRUE;
			break;
		}
	}

	if(FALSE == bTableExists)
	{
		//! ���̺��� �����Ƿ� ���̺��� �����Ѵ�.	
		CString rSql = CString(GetCreateString().c_str());
		if(!rSql.IsEmpty())
		{
			rSql.Replace(_T("?") , CString(string(_T("[") + rTableName + _T("]")).c_str()));
			adoDB.ExecuteQuery(rSql);
		}
	}
	else
	{
		CStringList ColumnNameList;
		adoDB.GetColumnNameListOfTable(ColumnNameList , _rTableName);

		//! database�� �ʵ���� �̸� ������ �д�.
		vector<CString> FieldNameEntry , ShouldBeAddedFieldNameEntry , TryToModifyFieldNameEntry;
		pos = ColumnNameList.GetHeadPosition();
		while(NULL != pos)
		{
			CString rValue = ColumnNameList.GetNext(pos);
			rValue.MakeUpper();
			FieldNameEntry.push_back(rValue);
			
		}

		{
			const int nSqlFieldCount = GetFieldCount();
			for(int i = 0;i < nSqlFieldCount;++i)
			{
				//! ���̺��� �ʵ��� SQL FILE������ �ʵ���� Ȯ���ϴ� ��ƾ.
				const string rFieldName = GetFieldNameAt(i);
				CString rString(rFieldName.c_str());
				rString.MakeUpper();
				vector<CString>::iterator where = find(FieldNameEntry.begin(),FieldNameEntry.end() , rString);
				if(where != FieldNameEntry.end())
				{
					TryToModifyFieldNameEntry.push_back(GetFieldDataAt(i).c_str());
					//! ���������� FieldNameEntry�� ���� �ʵ�� �����Ǿ�� �� �ʵ��̴�.
					FieldNameEntry.erase(where);
				}else	ShouldBeAddedFieldNameEntry.push_back(GetFieldDataAt(i).c_str());
				//!
			}

			//! ���̺��� SQL FILE�� �ִ� ������ �����.
			{
				//! �ʵ� �߰� �κ�
				for(vector<CString>::const_iterator itr  = ShouldBeAddedFieldNameEntry.begin();itr != ShouldBeAddedFieldNameEntry.end();++itr)
				{
					adoDB.ExecuteQuery(_T("ALTER TABLE [") + CString(rTableName.c_str()) + _T("] ADD ") + *itr);
				}

				//! �ʵ� ���� �κ�
				for(itr  = FieldNameEntry.begin();itr != FieldNameEntry.end();++itr)
				{
					//! DO DOT DELETE INDEX COLUMN.
					if(_T("C_IDX") == (*itr)) continue;

					adoDB.ExecuteQuery(_T("ALTER TABLE [") + CString(rTableName.c_str()) + _T("] DROP [") + *itr + ("]"));
				}

				//! modify field information.
				for(itr  = TryToModifyFieldNameEntry.begin();itr != TryToModifyFieldNameEntry.end();++itr)
				{
					adoDB.ExecuteQuery(_T("ALTER TABLE ") + CString(rTableName.c_str()) + _T(" ALTER COLUMN ") + *itr);
				}
			}
		}
	}

	return ERROR_SUCCESS;
}
