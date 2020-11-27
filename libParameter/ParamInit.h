#pragma once

class CRegardsConfigParam;

class CParamInit
{
public:
	CParamInit();
	~CParamInit();

	static CRegardsConfigParam * getInstance();
	static void Initialize(CRegardsConfigParam * param);
	static bool IsConfigFileExist();
private:

	static CRegardsConfigParam *_singleton;
};

