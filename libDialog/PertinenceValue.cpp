#include <header.h>
#include "PertinenceValue.h"

//(*InternalHeaders(PertinenceValue)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(PertinenceValue)
//*)

BEGIN_EVENT_TABLE(PertinenceValue, wxDialog)
		//(*EventTable(PertinenceValue)
		//*)
END_EVENT_TABLE()

PertinenceValue::PertinenceValue(wxWindow* parent)
{
	//(*Initialize(PertinenceValue)
	wxXmlResource::Get()->LoadObject(this, parent,_T("PertinenceValue"),_T("wxDialog"));
	stBox = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_STBOX")));
	spValue = static_cast<wxSpinCtrl*>(FindWindow(XRCID("ID_SPVALUE")));
	btOK = static_cast<wxButton*>(FindWindow(XRCID("ID_BTOK")));
	btCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BTCANCEL")));
	//*)

	Connect(XRCID("ID_BTOK"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&PertinenceValue::OnbtnOkClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&PertinenceValue::OnBtnCancelClick);
}

PertinenceValue::~PertinenceValue()
{
	//(*Destroy(PertinenceValue)
	//*)
}

void PertinenceValue::SetValue(const double& pertinence)
{
	int value = pertinence * 100;
	spValue->SetValue(value);
}

double PertinenceValue::GetValue()
{
	return static_cast<double>(spValue->GetValue()) / 100.0f;
}

bool PertinenceValue::IsOk()
{
	return isOk;
}

void PertinenceValue::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	this->Close();
}

void PertinenceValue::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
