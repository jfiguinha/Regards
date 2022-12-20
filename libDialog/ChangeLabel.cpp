#include <header.h>
#include "ChangeLabel.h"
#include <wx/msgdlg.h>
#include <SqlFaceLabel.h>
#include <LibResource.h>
//(*InternalHeaders(ChangeLabel)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ChangeLabel)
//*)
using namespace Regards::Sqlite;

BEGIN_EVENT_TABLE(ChangeLabel, wxDialog)
	//(*EventTable(ChangeLabel)
	//*)
END_EVENT_TABLE()

ChangeLabel::ChangeLabel(wxWindow* parent)
{
	//(*Initialize(ChangeLabel)
	wxXmlResource::Get()->LoadObject(this, parent,_T("ChangeLabel"),_T("wxDialog"));
	txtCtrlFaceName = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTFACENAME")));
	btOK = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON1")));
	btCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BTCANCEL")));
	stFaceName = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STFACENAME")));
	//*)

	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ChangeLabel::OnbtnOkClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ChangeLabel::OnBtnCancelClick);
}

ChangeLabel::~ChangeLabel()
{
	//(*Destroy(ChangeLabel)
	//*)
}

void ChangeLabel::SetLabel(const wxString& labelFaceName)
{
	this->labelFaceName = labelFaceName;
	txtCtrlFaceName->SetValue(labelFaceName);
}

wxString ChangeLabel::GetNewLabel()
{
	return labelFaceName;
}

bool ChangeLabel::IsOk()
{
	return isOk;
}


void ChangeLabel::OnbtnOkClick(wxCommandEvent& event)
{
	wxString wronglabelname = CLibResource::LoadStringFromResource(L"wronglabelname", 1);
	wxString wronglabelsize = CLibResource::LoadStringFromResource(L"wronglabelsize", 1);
	wxString error_labelname = CLibResource::LoadStringFromResource(L"ErrorLabelName", 1);
	wxString error_labelsize = CLibResource::LoadStringFromResource(L"erroronlabelsize", 1);
	labelFaceName = txtCtrlFaceName->GetValue();
	if (labelFaceName.size() > 3)
	{
		//Test si label existe déjà
		CSqlFaceLabel sqlFaceLabel;
		int numFace = sqlFaceLabel.GetNumFace(labelFaceName);
		if (numFace == -1)
		{
			isOk = true;
			this->Close();
		}
		else
		{
			wxMessageBox(wronglabelname, error_labelname, wxICON_ERROR);
		}
	}
	else
	{
		wxMessageBox(wronglabelsize, error_labelsize, wxICON_ERROR);
	}
}

void ChangeLabel::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
