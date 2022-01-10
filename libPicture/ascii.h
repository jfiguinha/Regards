#pragma once

class CRegardsBitmap;

class CBitmapToAscii
{
public:
    CBitmapToAscii(){};
    ~CBitmapToAscii(){};
    static void SaveToAscii(CRegardsBitmap * source, const string &filenameOut);
    
private:

    static char getDensity(int value);
};