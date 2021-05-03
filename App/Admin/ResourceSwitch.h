#ifndef	__RESOURCE_SWITCH_H__
#define	__RESOURCE_SWITCH_H__

class CResourceSwitch
{
	CResourceSwitch& operator=(const CResourceSwitch&){ return (*this); }
	CResourceSwitch(const CResourceSwitch&){}
public:
	CResourceSwitch(HINSTANCE hRes)
	{
		m_hOldRes = AfxGetResourceHandle();
		if(NULL != hRes) AfxSetResourceHandle(hRes);
	}

	~CResourceSwitch()
	{
		AfxSetResourceHandle(m_hOldRes);
	}
private:
	HINSTANCE m_hOldRes;
};

#endif