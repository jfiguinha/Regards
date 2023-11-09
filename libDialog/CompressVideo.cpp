#include <header.h>
#include "CompressVideo.h"
#include <window_id.h>
#include <wx/dcmemory.h>


#ifndef WX_PRECOMP
//(*InternalHeadersPCH(CompressVideo)
//*)
#endif
//(*InternalHeaders(CompressVideo)
#include <wx/xrc/xmlres.h>
#include <wx/statbmp.h>
#include <libPicture.h>
//*)
using namespace Regards::Picture;

//(*IdInit(CompressVideo)
//*)

BEGIN_EVENT_TABLE(CompressVideo, wxDialog)
		//(*EventTable(CompressVideo)
		//*)
END_EVENT_TABLE()

CompressVideo::CompressVideo(wxWindow* parent, int rotation)
{
	this->rotation = rotation;
	isOk = true;
	//(*Initialize(CompressVideo)
	wxXmlResource::Get()->LoadObject(this, parent,_T("CompressVideo"),_T("wxDialog"));
	ggProgress = static_cast<wxGauge*>(FindWindow(XRCID("ID_GAUGEPRG")));
	//panel = (wxPanel*)FindWindow(XRCID("ID_PANELVIDEO"));
	labelProgression = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STPROGRESS")));
	labelFrame = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STFRAME")));
	labelTime = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STTIMEELAPSED")));
	labelTimeMissing = static_cast<wxStaticText*>(FindWindow(XRCID("ID_STTIMEMISSING")));
	bitmap = static_cast<wxStaticBitmap*>(FindWindow(XRCID("ID_BITMAPVIDEO")));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BUTTON1")));
	Connect(XRCID("ID_BUTTON1"),wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CompressVideo::OnbtnCancelClick);
	//Connect(wxEVT_PAINT, wxPaintEventHandler(CompressVideo::OnPaint));
	Connect(wxEVENT_SETRANGEPROGRESSBAR, wxCommandEventHandler(CompressVideo::OnSetValueMaxProgressBar));
	Connect(wxEVENT_SETVALUEPROGRESSBAR, wxCommandEventHandler(CompressVideo::OnSetValueProgressBar));
	Connect(wxEVENT_SETSTATUSTEXT, wxCommandEventHandler(CompressVideo::OnSetText));

	Connect(wxEVENT_UPDATEBITMAP, wxCommandEventHandler(CompressVideo::OnSetBitmap));
#ifndef __APPLE__
	bitmap->Show(false);
#endif
	wxSize size = bitmap->GetSize();
	_localBmp = wxBitmap(size.GetWidth(), size.GetHeight(), -1);
}

void CompressVideo::OnSetBitmap(wxCommandEvent& event)
{
	wxSize size = bitmap->GetSize();
	if (size.GetWidth() == 16)
	{
		bitmap->SetSize(344, 240);
		size = wxSize(344, 240);
		_localBmp = wxBitmap(size.GetWidth(), size.GetHeight(), -1);
	}

	auto bmp = static_cast<wxImage*>(event.GetClientData());
	wxImage out;
	//ApplyRotation to image
	if (rotation == 90)
		out = bmp->Rotate90();
	else if (rotation == -90)
	{
		out = bmp->Rotate180();
		out = out.Rotate90();
	}
	else if (rotation == -180)
		out = bmp->Rotate180();
	else if (rotation == 180)
		out = bmp->Rotate180();
	else if (rotation == -270)
		out = bmp->Rotate90();
	else if (rotation == 270)
	{
		out = bmp->Rotate180();
		out = out.Rotate90();
	}
	else
		out = *bmp;

	float new_ratio = 1;
	int pictureWidth = out.GetWidth();
	int pictureHeight = out.GetHeight();
	int _width = size.GetWidth();
	int _height = size.GetHeight();
	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(_width) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(_height) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > _height)
	{
		new_ratio = static_cast<float>(_height) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > _width)
	{
		new_ratio = static_cast<float>(_width) / static_cast<float>(pictureWidth);
	}
	scale = out.Scale(pictureWidth * new_ratio, pictureHeight * new_ratio);


	int xPos = (size.GetWidth() - scale.GetWidth()) / 2;
	int yPos = (size.GetHeight() - scale.GetHeight()) / 2;


	wxMemoryDC bbBuffer;
	bbBuffer.SelectObject(_localBmp);
	bbBuffer.SetBackground(*wxBLACK_BRUSH);
	bbBuffer.Clear();
	bbBuffer.DrawBitmap(scale, xPos, yPos);
	bbBuffer.SelectObject(wxNullBitmap);


	

#ifdef __APPLE__
   // bitmap->SetPosition(xPos, yPos);
    bitmap->SetBitmap(_localBmp);
#else
	//wxPoint pt = bitmap->GetPosition();
	//wxClientDC dc(this);
	//dc.DrawBitmap(_localBmp, pt.x, pt.y);

	bitmap->SetBitmap(_localBmp);

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
	int id = event.GetInt();
	auto statusText = static_cast<wxString*>(event.GetClientData());
	if (statusText != nullptr)
	{
		switch (id)
		{
		case 1:
			labelTime->SetLabel(*statusText);
			break;
		case 2:
			labelFrame->SetLabel(*statusText);
			break;
		case 3:
			labelTimeMissing->SetLabel(*statusText);
			break;
		default:
			labelProgression->SetLabel(*statusText);
			break;
		}

		delete statusText;
	}
}

void CompressVideo::SetTextProgression(const wxString& texte, const int& type)
{
	//labelProgression->SetLabel(texte);
	auto event = new wxCommandEvent(wxEVENT_SETSTATUSTEXT);
	auto statusText = new wxString(texte);
	event->SetClientData(statusText);
	event->SetInt(type);
	wxQueueEvent(this, event);
}

void CompressVideo::SetPos(const int& max, const int& pos)
{
	if (ggProgress->GetRange() != max)
	{
		auto event = new wxCommandEvent(wxEVENT_SETRANGEPROGRESSBAR);
		event->SetInt(max);
		wxQueueEvent(this, event);
	}


	auto event = new wxCommandEvent(wxEVENT_SETVALUEPROGRESSBAR);
	event->SetInt(pos);
	wxQueueEvent(this, event);

	//ggProgress->SetRange(max);
	//ggProgress->SetValue(pos);
}


void CompressVideo::SetBitmap(cv::Mat& bmp)
{
	auto event = new wxCommandEvent(wxEVENT_UPDATEBITMAP);
	event->SetClientData(CLibPicture::ConvertRegardsBitmapToWXImagePt(bmp));
	wxQueueEvent(this, event);
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
