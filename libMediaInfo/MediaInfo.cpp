#include "MediaInfo.h"
/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Example for MediaInfoLib
// Command line version
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef WIN32
#define MEDIAINFO_LIBRARY
#endif

#ifdef MEDIAINFO_LIBRARY
#include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
#define MediaInfoNameSpace MediaInfoLib;
#else //MEDIAINFO_LIBRARY
#include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
#define MediaInfoNameSpace MediaInfoDLL;
#endif //MEDIAINFO_LIBRARY
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <ConvertUtility.h>
using namespace std;
using namespace MediaInfoNameSpace;

#ifdef __MINGW32__
#ifdef _UNICODE
#define _itot _itow
#else //_UNICODE
#define _itot itoa
#endif //_UNICODE
#endif //__MINGW32

// trim from end
static inline std::wstring &rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::wstring CMediaInfo::delUnnecessary(std::wstring &str)
{
    size_t size = str.length();
    for(int j = 0; j<=size; j++)
    {
        for(int i = 0; i <=j; i++)
        {
            if(str[i] == ' ' && str[i+1] == ' ')
            {
                str.erase(str.begin() + i);
            }
            else if(str[0]== ' ')
            {
                str.erase(str.begin());
            }
            else if(str[i] == '\0' && str[i-1]== ' ')
            {
                str.erase(str.end() - 1);
            }
        }
    }
    return str;
}

vector<CMetadata> CMediaInfo::SplitByLine(const wstring &value)
{
    wstring categorie;
    wstring data = value;
    vector<CMetadata> listOfLine;
    size_t position = 0;
    do
    {
#ifdef WIN32
		position = data.find(L"\r\n");
#else
        position = data.find(L"\n");
#endif
        if(position != std::string::npos)
        {
            wstring line = data.substr(0,position);
#ifdef WIN32			
			position+=2;
#else
			position++;
#endif
            if(line != L"")
            {
                size_t pos = line.find(L":");
                if(pos != std::string::npos)
                {
                    CMetadata metadata;
                    wstring key = line.substr(0,pos);pos+=2;
					key = categorie + L"." + rtrim(key);
                    metadata.key = key;
                    metadata.value = line.substr(pos,line.length() - pos);
                    listOfLine.push_back(metadata);
                }
                else
                    categorie = line;
            }
            
            data = data.substr(position, value.length() - position);
        }
    }while(position != std::string::npos);
    return listOfLine;
}


vector<CMetadata> CMediaInfo::ReadMetadata(const wxString &filename)
{
    vector<CMetadata> metadata;
    //Information about MediaInfo
    MediaInfo MI;
    String To_Display=MI.Option(__T("Info_Version"), __T("0.7.13;MediaInfoDLL_Example_MSVC;0.7.13")).c_str();
    MI.Open(CConvertUtility::ConvertToStdWstring(filename));
    std:wstring value = MI.Inform();
    metadata = SplitByLine(value);
    MI.Close();
    return metadata;
}

int CMediaInfo::GetVideoRotation(const wxString &filename)
{
	MediaInfo MI;
	wstring To_Display;
	MI.Open(CConvertUtility::ConvertToStdWstring(filename));
	To_Display = MI.Get(Stream_Video, 0, __T("Rotation"), Info_Text, Info_Name).c_str();
	MI.Close();
	if (To_Display != "")
		return std::stoi(To_Display);
	return 0;
}