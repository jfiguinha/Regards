#pragma once
#ifdef WIN32

#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/checklst.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(TiffOption)
//*)

class Association : public wxDialog
{
public:
	Association(wxWindow* parent);
	~Association() override;

	wxButton* btnCancel;
	wxButton* btnOK;
	wxCheckListBox* checkListAssociation;
	wxCheckBox* checkSelect;
	//*)
	bool IsOk();
	bool AssociateExtension();

protected:
	//(*Identifiers(TiffOption)
	//*)

private:
	BOOL IsAppRunningAsAdminMode();
	//(*Handlers(TiffOption)
	void OnbtnOKClick(wxCommandEvent& event);
	void OnbtnCancelClick(wxCommandEvent& event);
	void OnbtnCheckClick(wxCommandEvent& event);
	int AssociateRegards();
	int AssociateExtension(LPCWSTR szFileExtension);
	int GetWindowsVersionAvance();
	void AssociateApplication(HKEY classesDefined);
	//*)
	bool isOk;
DECLARE_EVENT_TABLE()
};

#endif
