#pragma once

class CImageLoadingFormat;

class CRaw
{
public:

    static void GetDimensions(const wxString & fileName, int & width, int & height);
    static CImageLoadingFormat * GetThumbnail(const wxString & fileName, const bool &thumbnail);

};
