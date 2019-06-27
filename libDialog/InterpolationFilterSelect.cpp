#include <header.h>
#include "InterpolationFilterSelect.h"
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CInterpolationFilterSelect)
	//*)
#endif
//(*InternalHeaders(CInterpolationFilterSelect)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(CInterpolationFilterSelect)
//*)

BEGIN_EVENT_TABLE(CInterpolationFilterSelect,wxDialog)
	//(*EventTable(CInterpolationFilterSelect)
	//*)
END_EVENT_TABLE()

CInterpolationFilterSelect::CInterpolationFilterSelect(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(CInterpolationFilterSelect)
	wxXmlResource::Get()->LoadObject(this,parent,_T("InterpolationOption"),_T("wxDialog"));
	rbCodec = (wxRadioBox*)FindWindow(XRCID("ID_RBCODEC"));
	btnOK = (wxButton*)FindWindow(XRCID("ID_OK"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));

	Connect(XRCID("ID_OK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CInterpolationFilterSelect::OnbtnOKClick);
	Connect(XRCID("ID_CANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CInterpolationFilterSelect::OnbtnCancelClick);
	//*)
    Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&CInterpolationFilterSelect::OnInit);
	
    SetTitle("Filter Interpolation Selection");
}

void CInterpolationFilterSelect::OnInit(wxInitDialogEvent& event)
{
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
	{ 
		numFilter = config->GetInterpolationType();
	}
    
	rbCodec->SetSelection(numFilter);
}

CInterpolationFilterSelect::~CInterpolationFilterSelect()
{
	//(*Destroy(CInterpolationFilterSelect)
	//*)
}

bool CInterpolationFilterSelect::IsOk()
{
    return isOk;
}
void CInterpolationFilterSelect::SetFilterIndex(const int &numFilter)
{
    this->numFilter = numFilter;
}
int CInterpolationFilterSelect::GetFilterIndex()
{
    return numFilter;
}

void CInterpolationFilterSelect::OnbtnOKClick(wxCommandEvent& event)
{
    isOk = true;
	numFilter = rbCodec->GetSelection();
    this->Close();
}

void CInterpolationFilterSelect::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    this->Close();
}
