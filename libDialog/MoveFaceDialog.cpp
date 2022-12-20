#include <header.h>
#include "MoveFaceDialog.h"
#include <SqlFindFacePhoto.h>
using namespace Regards::Sqlite;

#ifndef WX_PRECOMP
//(*InternalHeadersPCH(MoveFaceDialog)
//*)
#endif
//(*InternalHeaders(MoveFaceDialog)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(MoveFaceDialog)
//*)

BEGIN_EVENT_TABLE(MoveFaceDialog, wxDialog)
	//(*EventTable(MoveFaceDialog)
	//*)
END_EVENT_TABLE()

MoveFaceDialog::MoveFaceDialog(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(MoveFaceDialog)
	wxXmlResource::Get()->LoadObject(this, parent,_T("MoveFaceDialog"),_T("wxDialog"));
	cbFaceLabel = static_cast<wxComboBox*>(FindWindow(XRCID("ID_COMBOBOX2")));
	deviceLabel = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STATICTEXT1")));
	btnOk = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON1")));
	BtnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON2")));

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&MoveFaceDialog::OnbtnOkClick);
	Connect(XRCID("ID_BUTTON2"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&MoveFaceDialog::OnBtnCancelClick);
	Connect(wxID_ANY, wxEVT_INIT_DIALOG, (wxObjectEventFunction)&MoveFaceDialog::OnInit);
	//*)
}

MoveFaceDialog::~MoveFaceDialog()
{
	//(*Destroy(MoveFaceDialog)
	//*)
}

void MoveFaceDialog::OnInit(wxInitDialogEvent& event)
{
	//int i = 0;
	wxString platform = "";
	wxString device = "";

	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceName> listFaceName = sqlFindFacePhoto.GetListFaceName();

	for (CFaceName faceName : listFaceName)
	{
		cbFaceLabel->Append(faceName.faceName);
	}

	if (listFaceName.size() > 0)
	{
		CFaceName faceName = listFaceName[0];
		cbFaceLabel->SetStringSelection(faceName.faceName);
	}
}


wxString MoveFaceDialog::GetFaceNameSelected()
{
	return cbFaceLabel->GetStringSelection();
}

bool MoveFaceDialog::IsOk()
{
	return isOk;
}


void MoveFaceDialog::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	this->Close();
}

void MoveFaceDialog::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

