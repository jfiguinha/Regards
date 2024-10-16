#include "header.h"
#include "MediaInfo.h"
#include <picture_utility.h>
#include <wx/wfstream.h>
#include <wx/file.h>


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

#if defined(__WXGTK__) || defined(__ARM64__) || defined(__APPLE__)

#if defined(__ARM64__)

#include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
#define MediaInfoNameSpace MediaInfoDLL;

#else

#include "MediaInfo/MediaInfo.h" //Staticly-loaded library (.lib or .a or .so)
#define MediaInfoNameSpace MediaInfoLib;

#endif 



#else //MEDIAINFO_LIBRARY

#include "MediaInfoDLL/MediaInfoDLL.h" //Dynamicly-loaded library (.dll or .so)
#define MediaInfoNameSpace MediaInfoDLL;

#endif //MEDIAINFO_LIBRARY
#include <string>
#include <vector>
#include <algorithm>
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




class CMediaRetrieve
{
public:
    CMediaRetrieve()
    {
       
    }
    
    ~CMediaRetrieve()
    {
        MI.Close();
    }
    
    MediaInfo MI;
    bool isOk = false;
    
    std::wstring rtrim(std::wstring& s)
    {
        auto no_space = [](char ch) -> bool
        {
            return !std::isspace<char>(ch, std::locale::classic());
        };
        s.erase(std::find_if(s.rbegin(), s.rend(), no_space).base(), s.end());
        return s;
    }


    std::wstring delUnnecessary(std::wstring& str)
    {
        size_t size = str.length();
        for (int j = 0; j <= size; j++)
        {
            for (int i = 0; i <= j; i++)
            {
                if (str[i] == ' ' && str[i + 1] == ' ')
                {
                    str.erase(str.begin() + i);
                }
                else if (str[0] == ' ')
                {
                    str.erase(str.begin());
                }
                else if (str[i] == '\0' && str[i - 1] == ' ')
                {
                    str.erase(str.end() - 1);
                }
            }
        }
        return str;
    }


    tbb::concurrent_vector<CMetadata> SplitByLine(const wstring& value)
    {
        wstring categorie;
        wstring data = value;
        tbb::concurrent_vector<CMetadata> listOfLine;
        size_t position;
        do
        {
    #ifdef WIN32
            position = data.find(L"\r\n");
    #else
            position = data.find(L"\n");
    #endif
            if (position != std::string::npos)
            {
                wstring line = data.substr(0, position);
    #ifdef WIN32
                position += 2;
    #else
                position++;
    #endif
                if (line != L"")
                {
                    size_t pos = line.find(L":");
                    if (pos != std::string::npos)
                    {
                        CMetadata metadata;
                        wstring key = line.substr(0, pos);
                        pos += 2;
                        key = categorie + L"." + rtrim(key);
                        metadata.key = key;
                        metadata.value = line.substr(pos, line.length() - pos);
                        listOfLine.push_back(metadata);
                    }
                    else
                        categorie = line;
                }

                data = data.substr(position, value.length() - position);
            }
        }
        while (position != std::string::npos);
        return listOfLine;
    }
    
    void OpenFile(wxString fileName)
    {
        printf("MediaInfo OpenFile \n");
        if (wxFile::Exists(fileName))
        {
            size_t taille = MI.Open(CConvertUtility::ConvertToStdWstring(fileName));
            if (taille == 0)
            {
                //From: preparing an example file for reading
                wxFile file(fileName);

                //From: preparing a memory buffer for reading
                uchar From_Buffer[4096]; //Note: you can do your own buffer
                size_t From_Buffer_Size; //The size of the read file buffer

                int64 F_Size = file.Length();
                int64 posSeek = 0;
                //Preparing to fill MediaInfo with a buffer
                MI.Open_Buffer_Init(F_Size, 0);

                //The parsing loop
                do
                {
                    //Reading data somewhere, do what you want for this.
                    From_Buffer_Size = file.Read(From_Buffer, 4096);

                    //Sending the buffer to MediaInfo
                    size_t Status = MI.Open_Buffer_Continue(From_Buffer, From_Buffer_Size);
                    if (Status & 0x08) //Bit3=Finished
                        break;

                    //Testing if there is a MediaInfo request to go elsewhere
                    if (MI.Open_Buffer_Continue_GoTo_Get() != -1)
                    {
                        posSeek = MI.Open_Buffer_Continue_GoTo_Get();
                        file.Seek(posSeek);   //Position the file
                        MI.Open_Buffer_Init(F_Size, file.Tell());                          //Informing MediaInfo we have seek
                    }
                } while (From_Buffer_Size > 0);

                //Finalizing
                MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

                //delete[] From_Buffer;
                file.Close();

                isOk = true;
            }
        }
    }
        
    tbb::concurrent_vector<CMetadata> GetMetadata()
    {
        std:wstring value = MI.Inform();
        return SplitByLine(value);       
    }
    
    int64 GetDuration()
    {
        int64 duration = -1000;
        std:wstring To_Display = MI.Get(Stream_General, 0, __T("Duration"), Info_Text, Info_Name).c_str();
        if (To_Display != "")
        {
            try
            {
                duration =  std::stoull(To_Display);
                duration = duration / 1000;
            }
            catch (...)
            {
            }
        }
        return duration;
    }

    void GetVideoDimensions(int& width, int& height)
    {

        wstring to_width;
        wstring to_height;
        to_width = MI.Get(Stream_Video, 0, __T("Width"), Info_Text, Info_Name).c_str();
        to_height = MI.Get(Stream_Video, 0, __T("Height"), Info_Text, Info_Name).c_str();
      //  MI.Close();
        if (to_width != "")
        {
            try
            {
                width = std::stoi(to_width);
            }
            catch (...)
            {
            }
        }

        if (to_height != "")
        {
            try
            {
                height = std::stoi(to_height);
            }
            catch (...)
            {
            }
        }
    }

    AspectRatio GetVideoAspectRatio()
    {
        AspectRatio aspectRatio;
        wstring To_Display;
        To_Display = MI.Get(Stream_Video, 0, __T("Display_aspect_ratio"), Info_Text, Info_Name).c_str();
        if (To_Display == "")
        {          
            tbb::concurrent_vector<CMetadata> vectorMeta = GetMetadata();
            tbb::concurrent_vector<CMetadata>::iterator it = std::find_if(vectorMeta.begin(), vectorMeta.end(), [&](CMetadata val) -> bool {return val.key == "Video.Display aspect ratio"; });
            if (it != vectorMeta.end())
            {
                To_Display = it->value;
            }
            /*
            for (CMetadata meta : vectorMeta)
            {
                if (meta.key == "Video.Display aspect ratio")
                {
                    To_Display = meta.value;
                    break;
                }
            }*/
        }
       

        if (To_Display != "")
        {
            try
            {
                std::vector<wxString> list = CConvertUtility::split(To_Display, ':');
                if (list.size() == 2)
                {
                    aspectRatio.num = std::stoi(list[0].ToStdString());
                    aspectRatio.den = std::stoi(list[1].ToStdString());
                }
                return aspectRatio;
            }
            catch (...)
            {
            }
        }
        return aspectRatio;
    }


    int GetVideoRotation()
    {
        wstring To_Display;
        To_Display = MI.Get(Stream_Video, 0, __T("Rotation"), Info_Text, Info_Name).c_str();

        if (To_Display != "")
        {
            try
            {
                return std::stoi(To_Display);
            }
            catch (...)
            {
            }
        }
        return 0;
    }

    wxString GetColorRange()
    {
        wstring To_Display = L"";
        To_Display = MI.Get(Stream_Video, 0, __T("colour_range"), Info_Text, Info_Name).c_str();
        return To_Display;
    }

    wxString GetColorSpace()
    {
        wstring To_Display = L"";
        To_Display = MI.Get(Stream_Video, 0, __T("matrix_coefficients"), Info_Text, Info_Name).c_str();
        return To_Display;
    }

};



static map<wxString, CMediaRetrieve*> movieList;
static mutex muMovie;

tbb::concurrent_vector<CMetadata> CMediaInfo::ReadMetadata(const wxString& filename)
{
    tbb::concurrent_vector<CMetadata> metadata;
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        metadata = mediaRetrieve->GetMetadata();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        metadata = mediaRetrieve->GetMetadata();
        muMovie.unlock();
    }

	return metadata;
}

int64_t CMediaInfo::GetVideoDuration(const wxString& filename)
{
    int64_t duration = 0;
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        duration = mediaRetrieve->GetDuration();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        duration = mediaRetrieve->GetDuration();
        muMovie.unlock();
    }


    return duration;

}

AspectRatio CMediaInfo::GetVideoAspectRatio(const wxString& filename)
{
    AspectRatio aspectRatio = { 0,0 };
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        aspectRatio = mediaRetrieve->GetVideoAspectRatio();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        aspectRatio = mediaRetrieve->GetVideoAspectRatio();
        muMovie.unlock();
    }


    return aspectRatio;

}


void CMediaInfo::GetVideoDimensions(const wxString& filename, int& width, int& height)
{
    int64_t duration = 0;
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        mediaRetrieve->GetVideoDimensions(width, height);
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        mediaRetrieve->GetVideoDimensions(width, height);
        muMovie.unlock();
    }

}

int CMediaInfo::GetVideoRotation(const wxString& filename)
{
    int rotation = 0;
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        rotation = mediaRetrieve->GetVideoRotation();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        rotation = mediaRetrieve->GetVideoRotation();
        muMovie.unlock();
    }


    return rotation;
}

wxString CMediaInfo::GetColorRange(const wxString& filename)
{
    wxString colorRange = "";
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        colorRange = mediaRetrieve->GetColorRange();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        colorRange = mediaRetrieve->GetColorRange();
        muMovie.unlock();
    }

    return colorRange;
}

wxString CMediaInfo::GetColorSpace(const wxString& filename)
{
    wxString colorRange = "";
    bool isFind = false;
    std::map<wxString, CMediaRetrieve*>::iterator it;
    muMovie.lock();
    CMediaRetrieve* mediaRetrieve = nullptr;
    it = movieList.find(filename);
    if (it != movieList.end())
    {
        mediaRetrieve = movieList[filename];
        colorRange = mediaRetrieve->GetColorSpace();
        isFind = true;
    }
    muMovie.unlock();
    if (!isFind)
    {
        mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);
        muMovie.lock();
        movieList[filename] = mediaRetrieve;
        colorRange = mediaRetrieve->GetColorSpace();
        muMovie.unlock();
    }

    return colorRange;

}
