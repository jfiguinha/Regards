#include "header.h"
#include "CheckVersion.h"
#include <ConvertUtility.h>
#ifdef USECURL
#include <curl/curl.h>
#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION
#else
#include <wx/url.h>
#endif

#include <wx/sstream.h>
using namespace Regards::Internet;

CCheckVersion::CCheckVersion(const wxString &server)
{
	serverHttp = server;
}


CCheckVersion::~CCheckVersion()
{

}

#ifdef USECURL

size_t CCheckVersion::write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);

#ifdef DEBUG
	fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
	tmp = (char *)realloc(data->data, data->size + 1); /* +1 for '\0' */

	if (tmp != NULL) {
		data->data = tmp;
	}
	else {
		if (data->data) {
			free(data->data);
		}
		fprintf(stderr, "Failed to allocate memory.\n");
		return 0;
	}

	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';

	return size * nmemb;
}


wxString CCheckVersion::GetLastVersion()
{
	CURL *curl;

	//int error = 0;
	bool returnValue = true;
	//wxString xml = L"";
	wxString httpAdress = serverHttp;
	CURLcode res;
	struct url_data data;
	data.size = 0;
	data.data = (char *)malloc(4096); /* reasonable size initial buffer */
	if (NULL == data.data) {
        printf("CCheckVersion Failed to allocate memory \n");
		fprintf(stderr, "Failed to allocate memory.\n");
		return "error";
	}

	data.data[0] = '\0';

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, CConvertUtility::ConvertToUTF8(httpAdress));

#ifdef SKIP_PEER_VERIFICATION
		/*
		 * If you want to connect to a site who isn't using a certificate that is
		 * signed by one of the certs in the CA bundle you have, you can skip the
		 * verification of the server's certificate. This makes the connection
		 * A LOT LESS SECURE.
		 *
		 * If you have a CA cert for the server stored someplace else than in the
		 * default bundle, then the CURLOPT_CAPATH option might come handy for
		 * you.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
		/*
		 * If the site you're connecting to uses a different host name that what
		 * they have mentioned in their server certificate's commonName (or
		 * subjectAltName) fields, libcurl will refuse to connect. You can skip
		 * this check, but this will make the connection less secure.
		 */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		res = curl_easy_perform(curl); /* ignores error */

	/* Check for errors */
		if (res != CURLE_OK)
		{
			wxString error = curl_easy_strerror(res);
			wxMessageBox(error);
			printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}
	wxString xml(data.data, wxConvUTF8);

	free(data.data);

	return xml;
}

#else

wxString CCheckVersion::GetLastVersion()
{
	printf("CCheckVersion::GetLastVersion \n");

	//int error = 0;
	bool returnValue = true;
	wxString xml = L"";
	wxString httpAdress = serverHttp;

	try
	{
		printf("http address : %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
		wxURL url(httpAdress);
		if (url.GetError() == wxURL_NOERR)
		{
			wxInputStream* in_stream = url.GetInputStream();
			if (in_stream != nullptr)
			{
				if (in_stream && in_stream->IsOk()) {
					wxStringOutputStream html_stream(&xml);
					in_stream->Read(html_stream);
				}
			}

			if (in_stream == nullptr)
			{
				return "";
			}
			else
				delete in_stream;
		}
		else
		{
			printf("URL ERROR %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
			return "";
		}
	}
	catch (...)
	{
		printf("wxURL ERROR CRASH %s \n", CConvertUtility::ConvertToUTF8(httpAdress));
	}

	return xml;
}
#endif