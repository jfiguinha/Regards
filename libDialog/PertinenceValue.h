#ifndef PERTINENCEVALUE_H
#define PERTINENCEVALUE_H

//(*Headers(PertinenceValue)
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/dialog.h>
//*)

class PertinenceValue : public wxDialog
{
public:
	PertinenceValue(wxWindow* parent);
	~PertinenceValue() override;

	//(*Declarations(PertinenceValue)
	wxButton* btOK;
	wxSpinCtrl* spValue;
	wxButton* btCancel;
	wxStaticBox* stBox;
	//*)
	bool IsOk();
	void SetValue(const double& pertinence);
	double GetValue();

protected:
	//(*Identifiers(PertinenceValue)
	//*)
	void OnbtnOkClick(wxCommandEvent& event);
	void OnBtnCancelClick(wxCommandEvent& event);

private:
	//(*Handlers(PertinenceValue)
	//*)
	bool isOk;
DECLARE_EVENT_TABLE()
};

#endif
