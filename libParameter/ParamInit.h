#pragma once

class CRegardsConfigParam;

class CParamInit
{
public:
	static CRegardsConfigParam* getInstance();
	static bool IsConfigFileExist();

private:
    static wxString ConfigPath();
	static std::unique_ptr<CRegardsConfigParam> _singleton;
};
