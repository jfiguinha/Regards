#pragma once

class CImageLoadingFormat;

class CBitmapToAscii
{
public:
	CBitmapToAscii()
	{
	};

	~CBitmapToAscii()
	{
	};
	static void SaveToAscii(CImageLoadingFormat* source, const string& filenameOut);

private:
	static char getDensity(int value);
};
