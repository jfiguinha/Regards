#include "CompressionOption.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CompressionOption)
	//*)
#endif
//(*InternalHeaders(CompressionOption)
#include <wx/xrc/xmlres.h>
//*)

//(*IdInit(CompressionOption)
//*)

BEGIN_EVENT_TABLE(CompressionOption,wxDialog)
	//(*EventTable(CompressionOption)
	//*)
END_EVENT_TABLE()

CompressionOption::CompressionOption(wxWindow* parent)
{
	isOk = false;
	compressLevel = 0;
	//(*Initialize(CompressionOption)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CompressionOption"),_T("wxDialog"));
	StaticBox1 = (wxStaticBox*)FindWindow(XRCID("ID_STATICBOX1"));
	btOK = (wxButton*)FindWindow(XRCID("ID_BTOK"));
	btCancel = (wxButton*)FindWindow(XRCID("ID_BTCANCEL"));
	slCompression = (wxSlider*)FindWindow(XRCID("ID_SLCOMPRESSION"));
	StaticText1 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT1"));
	StaticText2 = (wxStaticText*)FindWindow(XRCID("ID_STATICTEXT2"));

	Connect(XRCID("ID_BTOK"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressionOption::OnButton1Click);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressionOption::OnButton2Click);
	//*)
}

CompressionOption::~CompressionOption()
{
	//(*Destroy(CompressionOption)
	//*)
}


bool CompressionOption::IsOk()
{
	return isOk;
}

int CompressionOption::CompressionLevel()
{
	return compressLevel;
}

void CompressionOption::OnButton1Click(wxCommandEvent& event)
{
	compressLevel = slCompression->GetValue();
	isOk = true;
	this->Close();
}

void CompressionOption::OnButton2Click(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
