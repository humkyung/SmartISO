// ProjectAccess.cpp: implementation of the CProjectAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "ProjectAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectAccess::CProjectAccess() : m_index(-1)
{
	m_userid = new string;
	m_date = new string;
	m_comments = new string;
}

CProjectAccess::CProjectAccess(const char* id , const char* date , const char* comments) : m_index(-1)
{
	m_userid = new string(id);
	m_date = new string(date);
	m_comments = new string(comments);
}

CProjectAccess::~CProjectAccess()
{
	try
	{
		if(NULL != m_userid) delete m_userid;
		if(NULL != m_date) delete m_date;
		if(NULL != m_comments) delete m_comments;
	}
	catch(...)
	{
		
	}
}

const char* CProjectAccess::user_id()
{
	return m_userid->c_str();
}

const char* CProjectAccess::date()
{
	return m_date->c_str();
}

const char* CProjectAccess::comments()
{
	return m_comments->c_str();
}

void CProjectAccess::Set(const char *id, const char *date, const char *comments)
{
	assert(id && date && comments && "Null Pointer!!!");

	if(id && date && comments)
	{
		(*m_userid) = id;
		(*m_date) = date;
		(*m_comments) = comments;
	}
}

CProjectAccess* CProjectAccess::CreateInstance()
{
	return new CProjectAccess;
}

const char* CProjectAccess::comment() const
{
	return m_comments->c_str();
}

long CProjectAccess::index() const
{
	return m_index;
}
