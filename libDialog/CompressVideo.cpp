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
    Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CompressVideo::OnSetValueMaxProgressBar));
    Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CompressVideo::OnSetValueProgressBar));
    Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CompressVideo::OnSetText));

    Connect(wxEVENT_UPDATEBITMAP, wxCommandEventHandler(CompressVideo::OnSetBitmap));
#ifndef __APPLE__
	bitmap->Show(false);
#endif
}

void CompressVideo::OnSetBitmap(wxCommandEvent& event)
{
	wxImage * bmp = (wxImage *)event.GetClientData();
	scale = bmp->Scale(344, 200).Mirror(false);
#ifdef __APPLE__
    bitmap->SetBitmap(scale);
#else 
	wxPoint pt = bitmap->GetPosition();
	wxClientDC dc(this);
	dc.DrawBitmap(scale, pt.x, pt.y);
#endif
    delete bmp;
}


void CompressVideo::OnSetValueMaxProgressBar(wxCommandEvent& event)
{
	int position = event.GetInt();
	//cout << "OnSetValueProgressBar Pos : " << position << endl;
	if (ggProgress != nullptr)
	{
        if (ggProgress->GetRange() != position)
        {
       		ggProgress->SetRange(position);
            ggProgress->Refresh();  
        }
	}
}

void CompressVideo::OnSetValueProgressBar(wxCommandEvent& event)
{
	int position = event.GetInt();
	//cout << "OnSetValueProgressBar Pos : " << position << endl;
	if (ggProgress != nullptr)
	{
		if (ggProgress->GetRange() > 0)
		{
			if (position >= ggProgress->GetRange())
				ggProgress->SetValue(ggProgress->GetRange() - 1);
			else
				ggProgress->SetValue(position);

			ggProgress->Refresh();
		}
	}
}

CompressVideo::~CompressVideo()
{

}

void CompressVideo::OnSetText(wxCommandEvent& event)
{
    wxString * statusText = (wxString *)event.GetClientData();
    if(statusText != nullptr)
    {
        labelProgression->SetLabel(*statusText);
        delete statusText;      
    }

}

void CompressVideo::SetTextProgression(const wxString &texte)
{
	//labelProgression->SetLabel(texte);
	wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
	wxString * statusText = new wxString(texte);
	event->SetClientData(statusText);
	wxQueueEvent(this, event);
}

void CompressVideo::SetPos(const int &max, const int &pos)
{
    if (ggProgress->GetRange() != max)
    {
        wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
        event->SetInt(max);
        wxQueueEvent(this, event);    
    }

    
    wxCommandEvent* event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
	event->SetInt(pos);
	wxQueueEvent(this, event);
    
	//ggProgress->SetRange(max);
	//ggProgress->SetValue(pos);
}

void CompressVideo::SetBitmap(wxImage * bmp)
{
//#ifdef __APPLE__

    wxCommandEvent* event = new wxCommandEvent(wxEVENT_UPDATEBITMAP);
	event->SetClientData(bmp);
	wxQueueEvent(this, event);
	/*
#else
	scale = bmp->Scale(344, 200).Mirror(false);
	wxPoint pt = bitmap->GetPosition();
	wxClientDC dc(this);
	dc.DrawBitmap(scale, pt.x, pt.y);
    delete bmp;
#endif
*/
	//bitmap->SetBitmap(scale);

	
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

