#include <header.h>
#include "ExportFile.h"

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(ExportFile)
//*)
#endif
//(*InternalHeaders(ExportFile)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ExportFile)
//*)

BEGIN_EVENT_TABLE(CExportFile, wxDialog)
	//(*EventTable(ExportFile)
	//*)
END_EVENT_TABLE()

CExportFile::CExportFile(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(ExportFile)
	wxXmlResource::Get()->LoadObject(this, parent,_T("ExportFile"),_T("wxDialog"));
	rbDateInformation = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBDATEINFORMATION")));
	btOK = static_cast<wxButton*>(FindWindow(XRCID("ID_BTOK")));
	rbGeographicalInformation = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBGEOGRAPHICALINFORMATION")));
	rbPrioriry = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBPRIORITY")));
	sbOutputFormat = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_SBOUTPUTFORMAT")));
	cbFileSaveOption = static_cast<wxComboBox*>(FindWindow(XRCID("ID_cbfilesaveoption")));
	btCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BTCANCEL")));
	rbChangeFilename = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBCHANGEFILENAME")));

	Connect(XRCID("ID_RBDATEINFORMATION"),wxEVT_COMMAND_RADIOBOX_SELECTED,
	        (wxObjectEventFunction)&CExportFile::OnRadioBox1Select);
	Connect(XRCID("ID_BTOK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CExportFile::OnButton1Click);
	Connect(XRCID("ID_RBGEOGRAPHICALINFORMATION"),wxEVT_COMMAND_RADIOBOX_SELECTED,
	        (wxObjectEventFunction)&CExportFile::OnRadioBox2Select);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CExportFile::OnbtCancelClick);
	//*)
}

CExportFile::~CExportFile()
{
	//(*Destroy(ExportFile)
	//*)
}


void CExportFile::OnRadioBox1Select(wxCommandEvent& event)
{
}

void CExportFile::OnRadioBox2Select(wxCommandEvent& event)
{
}

bool CExportFile::GetIsOK()
{
	return isOk;
}

InfoExportFile CExportFile::GetInfoExportFile()
{
	return infoExportFile;
}

void CExportFile::OnButton1Click(wxCommandEvent& event)
{
	//Si Ok
	infoExportFile.dateInfoSelection = rbDateInformation->GetSelection();
	infoExportFile.geoInfoSelection = rbGeographicalInformation->GetSelection();
	infoExportFile.priority = rbPrioriry->GetSelection();
	infoExportFile.outputFormat = cbFileSaveOption->GetSelection();
	infoExportFile.changeFilename = rbChangeFilename->GetSelection();
	isOk = true;
	this->Close();
}

void CExportFile::OnbtCancelClick(wxCommandEvent& event)
{
	//Si Cancel
	isOk = false;
	this->Close();
}
