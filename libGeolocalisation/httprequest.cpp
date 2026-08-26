#include <header.h>
#include "httprequest.h"
#include <ConvertUtility.h>
#include <wx/progdlg.h>


using namespace Regards::Internet;

static size_t WriteToString(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    auto* response =
        static_cast<std::string*>(userdata);

    response->append(
        static_cast<char*>(ptr),
        size * nmemb);

    return size * nmemb;
}

static size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    auto* context =
        static_cast<DownloadContext*>(userdata);

    return fwrite(
        ptr,
        size,
        nmemb,
        context->file);
}

static int ProgressCallback(
    void* clientp,
    curl_off_t dltotal,
    curl_off_t dlnow,
    curl_off_t,
    curl_off_t)
{
    auto* context =
        static_cast<DownloadContext*>(clientp);

    if (context->dialog == nullptr)
        return 0;

    int percent = 0;

    if (dltotal > 0)
    {
        percent = static_cast<int>(
            (100.0 * dlnow) / dltotal);
    }

    context->dialog->Update(
        percent,
        wxString::Format("%d %%", percent));

    return 0;
}

void CHttpRequest::ConfigureCurl(CURL* curl)
{
    curl_easy_setopt(
        curl,
        CURLOPT_FOLLOWLOCATION,
        1L);

    curl_easy_setopt(
        curl,
        CURLOPT_CONNECTTIMEOUT,
        10L);

    curl_easy_setopt(
        curl,
        CURLOPT_TIMEOUT,
        60L);

    curl_easy_setopt(
        curl,
        CURLOPT_SSL_VERIFYPEER,
        1L);

    curl_easy_setopt(
        curl,
        CURLOPT_SSL_VERIFYHOST,
        2L);

    curl_easy_setopt(
        curl,
        CURLOPT_USERAGENT,
        "Regards/1.0");
}

bool CHttpRequest::DownloadFile(
    const wxString& url,
    const wxString& outputFile,
    wxProgressDialog* dlg)
{
    using CurlPtr =
        std::unique_ptr<CURL,
        decltype(&curl_easy_cleanup)>;

    CurlPtr curl(
        curl_easy_init(),
        &curl_easy_cleanup);

    if (!curl)
        return false;

    ConfigureCurl(curl.get());

#ifdef _WIN32
    FILE* file = nullptr;

    if (_wfopen_s(
        &file,
        outputFile.wc_str(),
        L"wb") != 0)
    {
        return false;
    }
#else
    FILE* file =
        fopen(
            outputFile.mb_str(),
            "wb");

    if (!file)
        return false;
#endif

    DownloadContext context;
    context.dialog = dlg;
    context.file = file;

    curl_easy_setopt(
        curl.get(),
        CURLOPT_URL,
        url.ToUTF8().data());

    curl_easy_setopt(
        curl.get(),
        CURLOPT_WRITEFUNCTION,
        WriteToFile);

    curl_easy_setopt(
        curl.get(),
        CURLOPT_WRITEDATA,
        &context);

    if (dlg)
    {
        curl_easy_setopt(
            curl.get(),
            CURLOPT_NOPROGRESS,
            0L);

        curl_easy_setopt(
            curl.get(),
            CURLOPT_XFERINFOFUNCTION,
            ProgressCallback);

        curl_easy_setopt(
            curl.get(),
            CURLOPT_XFERINFODATA,
            &context);
    }

    CURLcode result =
        curl_easy_perform(curl.get());

    fclose(file);

    if (result != CURLE_OK)
    {
        wxRemoveFile(outputFile);
        return false;
    }

    long httpCode = 0;

    curl_easy_getinfo(
        curl.get(),
        CURLINFO_RESPONSE_CODE,
        &httpCode);

    if (httpCode < 200 ||
        httpCode >= 300)
    {
        wxRemoveFile(outputFile);
        return false;
    }

    return true;
}


HttpResponse CHttpRequest::Get(const wxString& url)
{
    HttpResponse response;

    using CurlPtr =
        std::unique_ptr<CURL,
        decltype(&curl_easy_cleanup)>;

    CurlPtr curl(
        curl_easy_init(),
        &curl_easy_cleanup);

    if (!curl)
    {
        response.errorMessage =
            "curl_easy_init failed";

        return response;
    }

    ConfigureCurl(curl.get());

    std::string buffer;

    curl_easy_setopt(
        curl.get(),
        CURLOPT_URL,
        url.ToUTF8().data());

    curl_easy_setopt(
        curl.get(),
        CURLOPT_WRITEFUNCTION,
        WriteToString);

    curl_easy_setopt(
        curl.get(),
        CURLOPT_WRITEDATA,
        &buffer);

    response.curlCode =
        curl_easy_perform(curl.get());

    if (response.curlCode != CURLE_OK)
    {
        response.errorMessage =
            curl_easy_strerror(
                response.curlCode);

        return response;
    }

    curl_easy_getinfo(
        curl.get(),
        CURLINFO_RESPONSE_CODE,
        &response.httpCode);

    response.body =
        wxString::FromUTF8(buffer);

    return response;
}