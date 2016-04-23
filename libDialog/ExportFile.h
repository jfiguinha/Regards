#ifndef EXPORTFILE_H
#define EXPORTFILE_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ExportFile)
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/combobox.h>
	//*)
#endif
//(*Headers(ExportFile)
//*)
struct InfoExportFile
{
	int dateInfoSelection = 0;
	int geoInfoSelection = 0;
	int priority = 0;
	int outputFormat = 0;
	int changeFilename = 0;
	bool isOk = false;
};


class CExportFile: public wxDialog
{
	public:

		CExportFile(wxWindow* parent);
		virtual ~CExportFile();

		//(*Declarations(ExportFile)
		wxButton* btOK;
		wxComboBox* cbFileSaveOption;
		wxStaticBox* sbOutputFormat;
		wxRadioBox* rbGeographicalInformation;
		wxRadioBox* rbDateInformation;
		wxButton* btCancel;
		wxRadioBox* rbChangeFilename;
		wxRadioBox* rbPrioriry;
		//*)

		bool GetIsOK();
		InfoExportFile GetInfoExportFile();

	protected:

		//(*Identifiers(ExportFile)
		//*)

	private:

		//(*Handlers(ExportFile)
		void OnRadioBox1Select(wxCommandEvent& event);
		void OnRadioBox2Select(wxCommandEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnbtCancelClick(wxCommandEvent& event);
		//*)

		InfoExportFile infoExportFile;
		bool isOk = false;


		DECLARE_EVENT_TABLE()
};

#endif
