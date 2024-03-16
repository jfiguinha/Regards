#pragma once
#include <GeoPluginValue.h>
#include <wx/progdlg.h>

namespace Regards::Internet
{
	struct url_data
	{
		size_t size;
		char* data;
	};

	class CCheckVersion
	{
	public:
		CCheckVersion(const wxString& server);
		~CCheckVersion();
		wxString GetLastVersion();

	private:
#ifdef USECURL
		static size_t write_data(void* ptr, size_t size, size_t nmemb, struct url_data* data);
#endif
		GeoPluginVector geoPluginVector;
		//int error;
		wxString serverHttp;
	};

	class CDownloadFile
	{
	public:
		CDownloadFile(const wxString& server);
		~CDownloadFile();
		void DownloadFile(wxProgressDialog* dlg, const wxString & outputFile);
		bool ExtractZipFiles(const wxString& aZipFile, const wxString& aTargetDir, wxWindow* parent);
	private:
#ifdef USECURL
		static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);
#endif
		//int error;
		wxString serverHttp;
	};
}
