#pragma once
#ifdef USECURL
#include <curl/curl.h>
#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION
#else
#include <wx/url.h>
#endif

class wxProgressDialog;

namespace Regards::Internet
{
    struct HttpResponse
    {
        long httpCode = 0;
        CURLcode curlCode = CURLE_OK;

        wxString body;
        wxString errorMessage;

        bool IsSuccess() const
        {
            return curlCode == CURLE_OK &&
                httpCode >= 200 &&
                httpCode < 300;
        }
    };

    struct DownloadContext
    {
        wxProgressDialog* dialog = nullptr;
        FILE* file = nullptr;
    };

    class CHttpRequest
    {
    public:

        static HttpResponse Get(const wxString& url);
        static bool DownloadFile(const wxString& url, const wxString& outputFile, wxProgressDialog* dlg = nullptr);

    private:

        static void ConfigureCurl(CURL* curl);

    };
}