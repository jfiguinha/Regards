#include "header.h"
#include "DownloadFile.h"
#include <ConvertUtility.h>
#include <wx/zipstrm.h>
#ifdef USECURL
#include <curl/curl.h>
#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION
#else
#include <wx/url.h>
#endif

#include <wx/sstream.h>
#include <wx/wfstream.h>
using namespace Regards::Internet;

struct SDataDownload
{
	wxProgressDialog* dlg;
	FILE* pagefile;
	int updatesize;
};

CDownloadFile::CDownloadFile(const wxString& server)
{
	serverHttp = server;
}


CDownloadFile::~CDownloadFile()
{
}

size_t CDownloadFile::write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	SDataDownload* data = (SDataDownload*)stream;
	size_t written = fwrite(ptr, size, nmemb, (FILE*)data->pagefile);
	//data->dlg->Update(data->updatesize++, "In progress");
	return written;
}

// https://curl.se/libcurl/c/CURLOPT_XFERINFOFUNCTION.html
int download_progress_callback(void* clientp, curl_off_t dltotal,
	curl_off_t dlnow, curl_off_t ultotal,
	curl_off_t ulnow)
{
	SDataDownload* data = (SDataDownload*)clientp;

	if (dltotal == 0)
	{
		data->updatesize = 0;
		data->dlg->Update(data->updatesize++, "In progress");
	}
	else
	{
		data->updatesize =
			static_cast<float>(dlnow) / static_cast<float>(dltotal) * 100;
		data->dlg->Update(data->updatesize, "In progress");
	}

	// If your callback function returns CURL_PROGRESSFUNC_CONTINUE it will
	// cause libcurl to continue executing the default progress function. return
	// CURL_PROGRESSFUNC_CONTINUE;

	return 0;
}


void CDownloadFile::DownloadFile(wxProgressDialog * dlg, const wxString& outputFile, const wxString& crtFile)
{
	CURL* curl;

	//int error = 0;
	//bool returnValue = true;
	//wxString xml = L"";
	wxString httpAdress = serverHttp;
	CURLcode res;

	SDataDownload dataDownload;

	FILE* pagefile;

	curl_version_info_data* vinfo = curl_version_info(CURLVERSION_NOW);

	if (vinfo->features & CURL_VERSION_SSL) {
		printf("CURL: SSL enabled\n");
	}
	else {
		printf("CURL: SSL not enabled\n");
	}

	/* init the curl session */
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, CConvertUtility::ConvertToUTF8(httpAdress));
	//curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
	curl_easy_setopt(curl, CURLOPT_CAINFO, CConvertUtility::ConvertToUTF8(crtFile));
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, true);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	/* send all data to this function  */
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);


	/* open the file */
	pagefile = fopen(outputFile, "wb");
	if (pagefile) {

		dataDownload.dlg = dlg;
		dataDownload.pagefile = pagefile;
		dataDownload.updatesize = 0;

		/* write the page body to this file handle */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &dataDownload);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, download_progress_callback);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &dataDownload);
		/* get it! */
		res = curl_easy_perform(curl); /* ignores error */

		/* Check for errors */
		if (res != CURLE_OK)
		{
			wxString error = curl_easy_strerror(res);
			wxMessageBox(error);
			printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		/* close the header file */
		fclose(pagefile);
	}

	/* cleanup curl stuff */
	curl_easy_cleanup(curl);


}

bool CDownloadFile::ExtractZipFiles(const wxString& aZipFile, const wxString& aTargetDir, wxWindow * parent)
{

	bool ret = true;

	//wxFileSystem fs;
	std::unique_ptr<wxZipEntry> entry(new wxZipEntry());


	do {

		wxFileInputStream in(aZipFile);

		if (!in) {
			wxLogError(_T("Can not open file '") + aZipFile + _T("'."));
			ret = false;
			break;
		}
		wxZipInputStream zip(in);

		int update = 0;
		int maxValue = zip.GetTotalEntries();

		wxProgressDialog dialog("Extracting...", "Please wait...", maxValue, parent, wxPD_APP_MODAL | !wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_SMOOTH);


		while (entry.reset(zip.GetNextEntry()), entry.get() != NULL) {
			// access meta-data
			wxString name = entry->GetInternalName();
			name = aTargetDir + wxFileName::GetPathSeparator() + name;

			// read 'zip' to access the entry's data
			if (entry->IsDir()) {
				int perm = entry->GetMode();
				wxFileName::Mkdir(name, perm, wxPATH_MKDIR_FULL);
			}
			else {
				zip.OpenEntry(*entry.get());
				if (!zip.CanRead()) {
					wxLogError(_T("Can not read zip entry '") + entry->GetName() + _T("'."));
					ret = false;
					break;
				}

				if (wxFileExists(name))
					wxRemoveFile(name);

				wxFileOutputStream file(name);

				if (!file) {
					wxLogError(_T("Can not create file '") + name + _T("'."));
					ret = false;
					break;
				}
				zip.Read(file);

				++update;
				dialog.Update(update, "File Decompression ...");

			}


			dialog.Update(maxValue);
		}


	} while (false);

	

	return ret;
}
