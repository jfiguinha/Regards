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
    source->VertFlipBuf();

    float new_ratio = 1;
    int pictureWidth = source->GetBitmapWidth();
    int pictureHeight = source->GetBitmapHeight();
    int width = 256;
    int height = 256;

    //int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

    if (pictureWidth > pictureHeight)
        new_ratio = static_cast<float>(width) / static_cast<float>(pictureWidth);
    else
        new_ratio = static_cast<float>(height) / static_cast<float>(pictureHeight);

    if ((pictureHeight * new_ratio) > height)
    {
        new_ratio = static_cast<float>(height) / static_cast<float>(pictureHeight);
    }
    if ((pictureWidth * new_ratio) > width)
    {
        new_ratio = static_cast<float>(width) / static_cast<float>(pictureWidth);
    }

    CRegardsBitmap* out = new CRegardsBitmap(source->GetBitmapWidth() * new_ratio, source->GetBitmapHeight() * new_ratio);

    cv::resize(source->GetMatrix(), out->GetMatrix(), cv::Size(out->GetBitmapWidth(), out->GetBitmapHeight()), cv::INTER_CUBIC);

    //CInterpolation interpolation;
    //interpolation.Execute(source, out);

   	std::ofstream outfile(filenameOut);
    if( outfile.fail() )
    {
        cerr << "!Error opening " << filenameOut << endl;
        return;
    }
    
    for(int y = 0;y < out->GetBitmapHeight();y++)
    {
         for(int x = 0; x < out->GetBitmapWidth(); x++)
         {
             CRgbaquad rgba = out->GetColorValue(x,y);
             outfile << getDensity(rgba.GetIntensity());    
         }
          outfile << endl;
    }     
	outfile.close(); 
    delete out;
}