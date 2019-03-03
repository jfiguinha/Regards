#pragma once

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

