#include <header.h>
#include "ascii.h"
#include <algorithm>    // std::max
#include <fstream>
#include <RegardsBitmap.h>
using namespace std;

const string DENSITY = "@QB#NgWM8RDHdOKq9$6khEPXwmeZaoS2yjufF]}{tx1zv7lciL/\\|?*>r^;:_\"~,'.-`";

char CBitmapToAscii::getDensity(int value) 
{
    
    // Since we don't have 255 characters, we have to use percentages
    int charValue = (int)round(DENSITY.size() / 255.0 * value);
    charValue = max(charValue, 0);
    charValue = min(charValue, (int)DENSITY.size() - 1);
    return DENSITY[charValue];
}

void CBitmapToAscii::SaveToAscii(CRegardsBitmap * source, const string &filenameOut)
{
   	std::ofstream outfile;
	outfile.open(filenameOut);
    if( outfile.fail() )
    {
        cerr << "!Error opening " << filenameOut << endl;
        return;
    }
    
    for(int y = 0;y < source->GetBitmapHeight();y++)
    {
         for(int x = 0; x < source->GetBitmapWidth(); x++)
         {
             CRgbaquad rgba = source->GetColorValue(x,y);
             outfile << getDensity(rgba.GetIntensity());    
         }
          outfile << endl;
    }     
	outfile.close(); 
}