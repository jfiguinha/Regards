// Mapi.h: interface for the CMapi class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
//#include <mapix.h>
#include <vector>
#include <string>

using namespace std;

class CMapi  
{
public:
	CMapi();
	virtual ~CMapi();
	static HRESULT SendEmail(const string &m_szBody,const vector<string> & attachment);
	
};


