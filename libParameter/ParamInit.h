#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
using namespace std;

class CRegardsConfigParam;

class CParamInit
{
public:
	CParamInit();
	~CParamInit();

	static CRegardsConfigParam * getInstance();
	static void Initialize(CRegardsConfigParam * param);

private:

	static CRegardsConfigParam *_singleton;
};

