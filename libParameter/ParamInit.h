#pragma once

class CRegardsConfigParam;

class CParamInit
{
public:
	CParamInit();
	~CParamInit();

	static CRegardsConfigParam* getInstance();
	static void Initialize(CRegardsConfigParam* param);
	static bool IsConfigFileExist();

private:
    static wxString ConfigPath();
	static CRegardsConfigParam* _singleton;
};
