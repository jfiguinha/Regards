#include <header.h>
#include "ExternalProgram.h"
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <ParamInit.h>
#include "ViewerParamInit.h"
#include "ViewerParam.h"
using namespace Regards::Viewer;

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(ExternalProgram)
	//*)
#endif
//(*InternalHeaders(ExternalProgram)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(ExternalProgram)
//*)

BEGIN_EVENT_TABLE(ExternalProgram,wxDialog)
	//(*EventTable(ExternalProgram)
	//*)
END_EVENT_TABLE()

ExternalProgram::ExternalProgram(wxWindow* parent)
{
	isOk = false;
	//(*Initialize(ExternalProgram)
	SetTitle("Regards Configuration");
	wxXmlResource::Get()->LoadObject(this,parent,_T("ExternalProgram"),_T("wxDialog"));
	txtPicturePath = (wxTextCtrl *)FindWindow(XRCID("ID_TXTPICTUREPATH"));
	btPicturePath = (wxButton*)FindWindow(XRCID("ID_PICTUREPATH"));
	btOk = (wxButton*)FindWindow(XRCID("ID_OK"));
	txtVideoPath = (wxTextCtrl *)FindWindow(XRCID("ID_TXTVIDEOPATH"));
	btVideoPath = (wxButton*)FindWindow(XRCID("ID_VIDEOPATH"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_CANCEL"));
	Connect(XRCID("ID_VIDEOPATH"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExternalProgram::OnbtnPathVideoClick);
	Connect(XRCID("ID_PICTUREPATH"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExternalProgram::OnBtnPathPictureClick);

	Connect(XRCID("ID_OK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExternalProgram::OnbtnOkClick);
	Connect(XRCID("ID_CANCEL"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ExternalProgram::OnBtnCancelClick);
	//*)
    
    Init();

	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		txtVideoPath->SetValue(config->GetPathForVideoEdit());
	if (config != nullptr)
		txtPicturePath->SetValue(config->GetPathForPictureEdit());

    SetAutoLayout( TRUE );
}

ExternalProgram::~ExternalProgram()
{
	//(*Destroy(ExternalProgram)
	//*)
}


void ExternalProgram::Init()
{
	this->SetTitle("Configuration");
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	
}

void ExternalProgram::OnbtnPathVideoClick(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(nullptr, "Select Video Editor Executable", "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK)
		txtVideoPath->SetValue(openFileDialog.GetPath());
}

void ExternalProgram::OnBtnPathPictureClick(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(nullptr, "Select Picture Editor Executable", "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK)
		txtPicturePath->SetValue(openFileDialog.GetPath());
}

void ExternalProgram::OnbtnOkClick(wxCommandEvent& event)
{
	isOk = true;
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
    
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
		config->SetPathForVideoEdit(txtVideoPath->GetValue());
	if (config != nullptr)
		config->SetPathForPictureEdit(txtPicturePath->GetValue());

    this->Close();        
}

void ExternalProgram::OnBtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}

bool ExternalProgram::IsOk()
{
	return isOk;
}