#pragma once

class CIsoEditError
{
public:
	CIsoEditError()
	{
		memset(m_nError , 0 , sizeof(int) * CHECK_ERROR_COUNT);
	}
	CIsoEditError(const CIsoEditError& rhs)
	{
		(*this) = rhs;
	}

	CIsoEditError& operator=(const CIsoEditError& rhs)
	{
		if(this != &rhs)
		{
			m_rFileName = rhs.m_rFileName;
			memcpy(m_nError , rhs.m_nError , sizeof(int) * CHECK_ERROR_COUNT);
		}

		return (*this);
	}

	CString OutputErrorString()
	{
		CString res;
		res.Format("%-40s" , m_rFileName);
		int nCount = sizeof(m_nError) / sizeof(m_nError[0]);
		for(int i = 0;i < nCount;++i)
		{
			 CString rTemp;
			 rTemp.Format("%d: %-4d", i + 1 , m_nError[i]);
			 res += rTemp;
		}

		return res;
	}

	CString m_rFileName;
	int m_nError[CHECK_ERROR_COUNT];
};