#include "header.h"
#include "CheckVersion.h"
#include <ConvertUtility.h>
#include <curl/curl.h>

#include <wx/sstream.h>
using namespace Regards::Internet;

CCheckVersion::CCheckVersion(const wxString &server)
{
	serverHttp = server;
}


CCheckVersion::~CCheckVersion()
{

}

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
	curl_easy_setopt(curl, CURLOPT_URL, CConvertUtility::ConvertToUTF8(httpAdress));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_perform(curl); /* ignores error */
	curl_easy_cleanup(curl);

	wxString xml(data.data, wxConvUTF8);

	free(data.data);

	return xml;
}

