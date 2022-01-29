// ReSharper disable All
#include "header.h"
#include "BitmapWnd2d.h"
#include "BitmapWndRender.h"
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;
extern COpenCLContext* openclContext;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd2D::CBitmapWnd2D(wxWindow* parent, wxWindowID id)
	: CWindowMain("CBitmapWnd2D", parent, id)
{


	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapWnd2D::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CBitmapWnd2D::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapWnd2D::OnLButtonDown));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CBitmapWnd2D::OnRButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CBitmapWnd2D::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CBitmapWnd2D::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CBitmapWnd2D::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CBitmapWnd2D::OnKeyDown));
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(CBitmapWnd2D::OnKeyUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CBitmapWnd2D::OnMouseCaptureLost));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapWnd2D::OnIdle));

}

void CBitmapWnd2D::SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;

	this->bitmapWndRender->SetParent(this);

	vector<int> listTimer = bitmapWndRender->GetListTimer();
	for (int id : listTimer)
	{
		Connect(id, wxEVT_TIMER, wxTimerEventHandler(CBitmapWnd2D::OnTimer), nullptr, this);
	}

	vector<int> listCommand = bitmapWndRender->GetListCommand();
	for (int id : listCommand)
	{
		Connect(id, wxCommandEventHandler(CBitmapWnd2D::OnCommand));

	}
}

void CBitmapWnd2D::UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;
}

bool CBitmapWnd2D::GetProcessEnd()
{
	return bitmapWndRender->GetProcessEnd();
}

void CBitmapWnd2D::OnIdle(wxIdleEvent& evt)
{
	bitmapWndRender->SetEndProgram(endProgram);
	bitmapWndRender->OnIdle(evt);
}

void CBitmapWnd2D::UpdateScreenRatio()
{
	bitmapWndRender->UpdateScreenRatio();
}

void CBitmapWnd2D::Resize()
{
	//updateFilter = true;
	bitmapWndRender->UpdateResized();
	//this->Refresh();
}

IBitmapRenderInterface * CBitmapWnd2D::GetWndPt()
{
	return bitmapWndRender;
}

void CBitmapWnd2D::OnCommand(wxCommandEvent& event)
{
	bitmapWndRender->OnCommand(event);
}

void CBitmapWnd2D::OnTimer(wxTimerEvent& event)
{
	bitmapWndRender->OnTimer(event);
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWnd2D::~CBitmapWnd2D(void)
{
}

void CBitmapWnd2D::OnRButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnRButtonDown(event);//TRACE();
	this->SetFocus();

}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWnd2D::OnLButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnLButtonDown(event);
	this->SetFocus();

}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd2D::OnLButtonUp(wxMouseEvent& event)
{
	//TRACE();
	if (event.LeftDClick())
		return;

	bitmapWndRender->OnLButtonUp(event);
}

void CBitmapWnd2D::OnKeyUp(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyUp(event);
}

void CBitmapWnd2D::OnKeyDown(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyDown(event);
}

//-----------------------------------------------------------------
//Gestion du volant de souris
//-----------------------------------------------------------------
void CBitmapWnd2D::OnMouseWheel(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseWheel(event);
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CBitmapWnd2D::OnMouseMove(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseMove(event);
	//this->Refresh();
}


//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd2D::OnPaint(wxPaintEvent& event)
{
	if (GetWidth() == 0 || GetHeight() == 0)
		return;

	if (openclContext == nullptr)
	{
		openclContext = Regards::OpenCL::COpenCLEngine::CreateInstance();
	}

	bitmapWndRender->OnPaint2D(this);
	

}


//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd2D::OnLDoubleClick(wxMouseEvent& event)
{
	bitmapWndRender->OnLDoubleClick(event);
}
