#include "header.h"
#include "MediaInfo.h"
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
#include <list>
#include <algorithm>
#include <cctype>
#include <locale>
#include <regex>
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


// Taille maximale du cache de fichiers media ouverts simultanement.
// Au-dela, l'entree la moins recemment utilisee est fermee et liberee.
static const size_t kMaxCacheSize = 1024;


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

    // Trim des espaces (et autres caracteres "espace" : tab, CR, LF, etc.)
    // en debut et fin de chaine. Retourne une copie, ne modifie pas l'original.
    static std::wstring Trim(const std::wstring& s)
    {
        auto is_space = [](wchar_t ch) -> bool
            {
                return std::isspace<wchar_t>(ch, std::locale::classic());
            };

        auto first = std::find_if(s.begin(), s.end(), [&](wchar_t ch) { return !is_space(ch); });
        auto last = std::find_if(s.rbegin(), s.rend(), [&](wchar_t ch) { return !is_space(ch); }).base();

        if (first >= last)
            return std::wstring();

        return std::wstring(first, last);
    }

    std::vector<CMetadata> SplitByLine(const wstring& value)
    {
        wstring categorie;
        wstring data = value;
        std::vector<CMetadata> listOfLine;
        size_t position;
        do
        {
            position = data.find(L"\r\n");
            size_t lineLen;
            size_t advance;

            if (position != std::wstring::npos)
            {
                lineLen = position;
                advance = position + 2; // saute "\r\n"
            }
            else
            {
                position = data.find(L"\n");
                if (position == std::wstring::npos)
                    break;

                lineLen = position;
                advance = position + 1; // saute "\n"
            }

            wstring line = data.substr(0, lineLen);
            if (!line.empty())
            {
                size_t pos = line.find(L":");
                if (pos != std::wstring::npos)
                {
                    CMetadata metadata;
                    wstring key = Trim(line.substr(0, pos));
                    wstring val = Trim(line.substr(pos + 1));

                    metadata.key = categorie + L"." + key;
                    metadata.value = val;
                    listOfLine.push_back(metadata);
                }
                else
                {
                    categorie = Trim(line);
                }
            }

            data = data.substr(advance);

        } while (true);

        return listOfLine;
    }

    void OpenFile(const wxString& fileName)
    {
        try
        {
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
                            MI.Open_Buffer_Init(F_Size, file.Tell()); //Informing MediaInfo we have seek
                        }
                    } while (From_Buffer_Size > 0);

                    //Finalizing
                    MI.Open_Buffer_Finalize(); //This is the end of the stream, MediaInfo must finnish some work

                    file.Close();

                    isOk = true;
                }
                else
                {
                    isOk = true;
                }
            }
        }
        catch (...)
        {
            isOk = false;
        }

        if (!isOk)
        {
            wxLogDebug("CMediaRetrieve::OpenFile: failed to open '%s'", fileName);
        }
    }

    std::vector<CMetadata> GetMetadata()
    {
        std::wstring value = MI.Inform();
        return SplitByLine(value);
    }

    int64 GetDuration()
    {
        int64 duration = -1000;
        std::wstring To_Display = MI.Get(Stream_General, 0, __T("Duration"), Info_Text, Info_Name).c_str();
        if (To_Display != L"")
        {
            try
            {
                duration = std::stoll(To_Display);
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
        width = 0;
        height = 0;

        wstring to_width = MI.Get(Stream_Video, 0, __T("Width"), Info_Text, Info_Name).c_str();
        wstring to_height = MI.Get(Stream_Video, 0, __T("Height"), Info_Text, Info_Name).c_str();

        if (to_width != L"")
        {
            try
            {
                width = std::stoi(to_width);
            }
            catch (...)
            {
            }
        }

        if (to_height != L"")
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

    // Retourne un AspectRatio. En cas d'echec de parsing, "den" vaut 0 :
    // l'appelant DOIT verifier den != 0 avant toute division.
    AspectRatio GetVideoAspectRatio()
    {
        AspectRatio aspectRatio{ 0, 0 };
        wstring To_Display = MI.Get(Stream_Video, 0, __T("Display_aspect_ratio"), Info_Text, Info_Name).c_str();

        if (To_Display == L"")
        {
            std::vector<CMetadata> vectorMeta = GetMetadata();
            auto it = std::find_if(vectorMeta.begin(), vectorMeta.end(),
                [](const CMetadata& val) { return val.key == L"Video.Display aspect ratio"; });
            if (it != vectorMeta.end())
            {
                To_Display = it->value;
            }
        }

        if (To_Display == L"")
            return aspectRatio;

        try
        {
            std::vector<wxString> list = CConvertUtility::split(To_Display, ':');
            if (list.size() == 2)
            {
                aspectRatio.num = std::stof(list[0].utf8_string());
                aspectRatio.den = std::stoi(list[1].utf8_string());
            }
            else
            {
                // Pas de format "num:den" -> on tente de lire une valeur decimale
                // (ex: "1.78") et on la convertit en ratio sur 1000 pour eviter
                // une mauvaise interpretation comme "num:den".
                float decimalValue = std::stof(To_Display.c_str());
                if (decimalValue > 0)
                {
                    aspectRatio.num = static_cast<int>(decimalValue * 1000.0f + 0.5f);
                    aspectRatio.den = 1000;
                }
            }
        }
        catch (...)
        {
        }

        return aspectRatio;
    }

    int GetVideoRotation()
    {
        wstring To_Display = MI.Get(Stream_Video, 0, __T("Rotation"), Info_Text, Info_Name).c_str();

        if (To_Display != L"")
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
        wstring To_Display = MI.Get(Stream_Video, 0, __T("colour_range"), Info_Text, Info_Name).c_str();
        return To_Display;
    }

    wxString GetColorSpace()
    {
        wstring To_Display = MI.Get(Stream_Video, 0, __T("matrix_coefficients"), Info_Text, Info_Name).c_str();
        return To_Display;
    }

};


// ----------------------------------------------------------------------------
// Cache LRU borne des fichiers media ouverts.
//
// - "movieList" : map cle -> iterateur dans "lruList" pour acces O(1)
// - "lruList"   : liste ordonnee du plus recemment au moins recemment utilise
//                 (front = le plus recent)
//
// Toute methode qui accede a une entree la deplace en tete de "lruList".
// Quand la taille depasse kMaxCacheSize, l'entree en queue (la moins
// recemment utilisee) est fermee et detruite.
// ----------------------------------------------------------------------------

namespace
{
    struct CacheEntry
    {
        wxString filename;
        CMediaRetrieve* media;
    };

    std::list<CacheEntry> lruList;
    std::map<wxString, std::list<CacheEntry>::iterator> movieList;
    std::mutex muMovie;

    // Doit etre appele avec muMovie deja verrouille.
    // Retourne le CMediaRetrieve associe a "filename", en le creant/l'ouvrant
    // si necessaire, et en le faisant remonter en tete de la LRU.
    CMediaRetrieve* AcquireLocked(const wxString& filename)
    {
        auto it = movieList.find(filename);
        if (it != movieList.end())
        {
            // Deplace l'entree en tete (plus recemment utilisee)
            lruList.splice(lruList.begin(), lruList, it->second);
            return it->second->media;
        }

        CMediaRetrieve* mediaRetrieve = new CMediaRetrieve();
        mediaRetrieve->OpenFile(filename);

        lruList.push_front({ filename, mediaRetrieve });
        movieList[filename] = lruList.begin();

        // Eviction si on depasse la taille max
        while (lruList.size() > kMaxCacheSize)
        {
            CacheEntry& evicted = lruList.back();
            movieList.erase(evicted.filename);
            delete evicted.media;
            lruList.pop_back();
        }

        return mediaRetrieve;
    }

    // Execute "func" sur le CMediaRetrieve correspondant a "filename",
    // en gerant le cache et le verrouillage.
    template <typename Func>
    auto WithMediaRetrieve(const wxString& filename, Func&& func) -> decltype(func(std::declval<CMediaRetrieve&>()))
    {
        std::lock_guard<std::mutex> lock(muMovie);
        CMediaRetrieve* mediaRetrieve = AcquireLocked(filename);
        return func(*mediaRetrieve);
    }
}


std::vector<CMetadata> CMediaInfo::ReadMetadata(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetMetadata(); });
}

int64_t CMediaInfo::GetVideoDuration(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetDuration(); });
}

AspectRatio CMediaInfo::GetVideoAspectRatio(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetVideoAspectRatio(); });
}

void CMediaInfo::GetVideoDimensions(const wxString& filename, int& width, int& height)
{
    WithMediaRetrieve(filename, [&](CMediaRetrieve& mr) { mr.GetVideoDimensions(width, height); return 0; });
}

int CMediaInfo::GetVideoRotation(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetVideoRotation(); });
}

wxString CMediaInfo::GetColorRange(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetColorRange(); });
}

wxString CMediaInfo::GetColorSpace(const wxString& filename)
{
    return WithMediaRetrieve(filename, [](CMediaRetrieve& mr) { return mr.GetColorSpace(); });
}

// Ferme et libere l'entree de cache associee a "filename", si elle existe.
// A appeler quand l'appelant sait qu'il n'aura plus besoin de ce fichier
// (ex: fichier supprime/deplace, ou fin de traitement d'un lot).
void CMediaInfo::ReleaseFile(const wxString& filename)
{
    std::lock_guard<std::mutex> lock(muMovie);

    auto it = movieList.find(filename);
    if (it != movieList.end())
    {
        delete it->second->media;
        lruList.erase(it->second);
        movieList.erase(it);
    }
}

// Ferme et libere toutes les entrees du cache.
// A appeler par exemple a la fermeture de l'application ou lors d'un
// rafraichissement complet de la photothèque.
void CMediaInfo::ClearCache()
{
    std::lock_guard<std::mutex> lock(muMovie);

    for (auto& entry : lruList)
    {
        delete entry.media;
    }
    lruList.clear();
    movieList.clear();
}