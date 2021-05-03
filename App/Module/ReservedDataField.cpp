// ReservedDataField.cpp: implementation of the CReservedDataField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "ReservedDataField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReservedDataField::CReservedDataField()
{
	m_name = new string;
	m_value = new string;
	m_desc = new string;
}

CReservedDataField::~CReservedDataField()
{
	try
	{
		if(NULL != m_name) delete m_name;
		if(NULL != m_value)delete m_value;
		if(NULL != m_desc) delete m_desc;
	}
	catch(...)
	{
		
	}
}

CReservedDataField* CReservedDataField::CreateInstance()
{
	return new CReservedDataField;
}

const char* CReservedDataField::name() const
{
	return m_name->c_str();
}

const char* CReservedDataField::value() const
{
	return m_value->c_str();
}

void CReservedDataField::Set(const char* name , const char* desc , const char *value)
{
	assert(name && desc && value && "name or desc or value is NULL");

	if(name && desc && value)
	{
		(*m_name)  = name;
		(*m_desc)  = desc;
		(*m_value) = value;
	}
}

const char* CReservedDataField::desc() const
{
	return m_desc->c_str();
}
