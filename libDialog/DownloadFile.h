#pragma once
#include <wx/progdlg.h>

namespace Regards::Internet
{
	class CDownloadFile
	{
	public:
		CDownloadFile(const wxString& server);
		~CDownloadFile();
		void DownloadFile(wxProgressDialog* dlg, const wxString & outputFile, const wxString& crtFile);
		bool ExtractZipFiles(const wxString& aZipFile, const wxString& aTargetDir, wxWindow* parent);
	private:
#ifdef USECURL
		static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);
#endif
		//int error;
		wxString serverHttp;
	};
}
