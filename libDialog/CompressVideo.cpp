#include <header.h>
#include "CompressVideo.h"
#include <window_id.h>
#include <ImageLoadingFormat.h>
#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CompressVideo)
	//*)
#endif
//(*InternalHeaders(CompressVideo)
#include <wx/xrc/xmlres.h>
#include <wx/statbmp.h>
//*)

//(*IdInit(CompressVideo)
//*)

BEGIN_EVENT_TABLE(CompressVideo,wxDialog)
	//(*EventTable(CompressVideo)
	//*)
END_EVENT_TABLE()

CompressVideo::CompressVideo(wxWindow* parent)
{
    isOk = true;
	//(*Initialize(CompressVideo)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CompressVideo"),_T("wxDialog"));
	ggProgress = (wxGauge*)FindWindow(XRCID("ID_GAUGEPRG"));
	//panel = (wxPanel*)FindWindow(XRCID("ID_PANELVIDEO"));
	labelProgression = (wxStaticText*)FindWindow(XRCID("ID_STPROGRESS"));
	bitmap = (wxStaticBitmap*)FindWindow(XRCID("ID_BITMAPVIDEO"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_BUTTON1"));
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressVideo::OnbtnCancelClick);
	//Connect(wxEVT_PAINT, wxPaintEventHandler(CompressVideo::OnPaint));
	bitmap->Show(false);
}

CompressVideo::~CompressVideo()
{

}


void CompressVideo::SetTextProgression(const wxString &texte)
{
	labelProgression->SetLabel(texte);
}

void CompressVideo::SetPos(const int &max, const int &pos)
{
	ggProgress->SetRange(max);
	ggProgress->SetValue(pos);
}

void CompressVideo::SetBitmap(wxImage * bmp)
{
	scale = bmp->Scale(344, 200).Mirror(false);

	wxPoint pt = bitmap->GetPosition();
	wxClientDC dc(this);
	dc.DrawBitmap(scale, pt.x, pt.y);

	//bitmap->SetBitmap(scale);
	delete bmp;
}


bool CompressVideo::IsOk()
{
    return isOk;
}


void CompressVideo::OnbtnCancelClick(wxCommandEvent& event)
{
    isOk = false;
    //this->Close();
}

