// Mapi.h: interface for the CMapi class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#if defined(WIN32) || defined(__MINGW32__)

#include <windows.h>

class CMapi  
{
public:
	CMapi();
	virtual ~CMapi();
	static HRESULT SendEmail(const string &m_szBody,const vector<string> & attachment);
	
};

#endif


