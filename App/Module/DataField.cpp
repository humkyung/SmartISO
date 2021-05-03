// DataField.cpp: implementation of the CDataField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "DataField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataField::CDataField()
{
	m_rFieldName = new string;
	m_rFieldDesc = new string;
	m_rFieldValue= new string;
	m_pExcelCol  = new string;
}

CDataField::~CDataField()
{
	try
	{
		if(NULL != m_rFieldName) delete m_rFieldName;
		if(NULL != m_rFieldDesc) delete m_rFieldDesc;
		if(NULL != m_rFieldValue)delete m_rFieldValue;
		if(NULL != m_pExcelCol)  delete m_pExcelCol;
	}
	catch(...)
	{

	}
}

/*
*/
const char* CDataField::name() const
{
	return m_rFieldName->c_str();
}

const char* CDataField::desc() const
{
	return m_rFieldDesc->c_str();
}

CDataField* CDataField::CreateInstance()
{
	return new CDataField;
}

void CDataField::DeleteInstance(CDataField *pLineDataField)
{
	delete pLineDataField;
}

/**	\brief	The CDataField::Set function

	\param	name	a parameter of type const char *
	\param	desc	a parameter of type const char *
	\param	value	a parameter of type const char*

	\return	void	
*/
void CDataField::Set(const char *name, const char *desc , const char* value)
{
	assert(name && "name is NULL");
	assert(desc && "desc is NULL");
	assert(value&& "value is NULL");

	(*m_rFieldName) = "";
	(*m_rFieldDesc) = "";
	(*m_rFieldValue)= "";
	if(name && desc && value)
	{
		(*m_rFieldName).assign(name);
		(*m_rFieldDesc).assign(desc);
		(*m_rFieldValue).assign(value);
	}
}

const char* CDataField::value() const
{
	return m_rFieldValue->c_str();
}

/**	\brief	복사품을 생성한다.
	\author	백흠경
**/
CDataField* CDataField::Clone()
{
	CDataField* p = new CDataField;
	if(p)
	{
		p->Copy(this);
		/*
		*(p->m_rFieldName) = (*m_rFieldName);
		*(p->m_rFieldDesc) = (*m_rFieldDesc);
		*(p->m_rFieldValue)= (*m_rFieldValue);
		*/
	}

	return p;
}

void CDataField::SetExcelCol(const char *pExcelCol)
{
	assert(pExcelCol && "pExcelCol is NULL");

	if(pExcelCol) (*m_pExcelCol) = pExcelCol;
}

const char* CDataField::GetExcelCol() const
{
	return m_pExcelCol->c_str();
}

bool CDataField::CoincideWith(CDataField *p)
{
	assert(p && "Null parameter");

	if(p)
	{
		*(m_rFieldValue) = *(p->m_rFieldValue);
		return true;
	}

	return false;
}

/**
	\brief	
	\author	백흠경
**/
void CDataField::Copy(CDataField *rhs)
{
	if(this != rhs)
	{
		m_rFieldName->assign(rhs->m_rFieldName->c_str());
		m_rFieldDesc->assign(rhs->m_rFieldDesc->c_str());
		m_rFieldValue->assign(rhs->m_rFieldValue->c_str());
		
		m_pExcelCol->assign(rhs->m_pExcelCol->c_str());
	}
}
