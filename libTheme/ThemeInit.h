#pragma once

class CThemeParam;

class CThemeInit
{
public:
	CThemeInit();
	~CThemeInit();

	static CThemeParam * getInstance();
	static void Initialize(CThemeParam * param);

private:

	static CThemeParam *_singleton;
};

