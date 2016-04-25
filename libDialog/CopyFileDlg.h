#ifndef COPYFILEDLG_H
#define COPYFILEDLG_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(CopyFileDlg)
	#include <wx/stattext.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/gauge.h>
	//*)
#endif
//(*Headers(CopyFileDlg)
//*)
#ifdef __APPLE__
#include <AppleReadExif.h>
#endif
#include <vector>
#include <ThumbnailData.h>
#include <libPicture.h>
#include "ExportFile.h"
using namespace std;

class CopyFileDlg: public wxDialog
{
	public:

		CopyFileDlg(wxWindow* parent);
		virtual ~CopyFileDlg();
		void SetSelectItem(vector<CThumbnailData *> * listItem);
		void SetLibelle(const wxString &caption, const wxString &text, const wxString &message
			, const wxString &finalMessage, const wxString &informaitons);
		void SetMode(const int &mode);
		void Start();
		void SetDestinationFolder(const wxString &folder);
		void SetInfosFile(const InfoExportFile & infoFile);
        void SetNewGeoInfos(const float &latitude, const float &longitude,const wxString &lat, const wxString &lng, const wxString &geoInfos);
        void SetNewDate(const wxDateTime &newDate, const wxString &selectDate);
		//(*Declarations(CopyFileDlg)
		wxStaticText* StaticText2;
		wxButton* btOK;
		wxGauge* Gauge1;
		wxStaticText* StaticText1;
		//*)

	protected:

		//(*Identifiers(CopyFileDlg)
		//*)

	private:

		//(*Handlers(CopyFileDlg)
		void OnInit(wxInitDialogEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnIdle(wxIdleEvent& evt);
		void OnSize(wxSizeEvent& evt);
		//*)

		void CopyFile(const wxString & filename, CThumbnailData * data);
		void DeleteFile(const wxString & filename, CThumbnailData * data);
		void ExportFile(const wxString & filename, CThumbnailData * data);
        void GeolocalizeFile(const wxString & filename);
        void ChangeDateFile(const wxString & filename);
    
		void CreateFolder(const wxString &newFolder);
		wxString CreateExportFolder(const InfoExportFile & infoFile, const wxString &folderDestination, const wxString &dateFile, const wxString &gpsFile);
		wxString GenerateFileName(const InfoExportFile & infoFile, const wxString &dateFile, const wxString &gpsFile);

		int progress = 0;
		int width = 0;
		int height = 0;
		bool start = false;
		int mode = 0;

        wxString selectDate = "";
        wxString lat = "";
        wxString lng = "";
        wxString geoInfos = "";
		wxString caption;
		wxString text;
		wxString message;
		wxString finalMessage;
		wxString informations;
		wxString destinationFolder;

#ifdef __APPLE__
        CAppleReadExif appleReadExif;
#endif
		vector<CThumbnailData *> * listItem = nullptr;
		CLibPicture libPicture;
		InfoExportFile infoFile;
		int optionPicture = 0;
		int qualityPicture = 0;
        float latitude = 0.0;
        float longitude = 0.0;
        wxDateTime newDate;
		DECLARE_EVENT_TABLE()
};

#endif
