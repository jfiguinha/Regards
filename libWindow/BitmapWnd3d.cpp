// ReSharper disable All
#include "header.h"
#include "BitmapWnd3d.h"
#include "BitmapWndRender.h"
#include <RenderOpenGL.h>
#include <OpenCLContext.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

extern COpenCLContext* openclContext2d;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd3D::CBitmapWnd3D(wxWindow* parent, wxWindowID id)
	: CWindowOpenGLMain("CBitmapWnd3D", parent, id)
{

	renderOpenGL = nullptr;
	Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapWnd3D::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CBitmapWnd3D::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapWnd3D::OnLButtonDown));
	Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CBitmapWnd3D::OnRButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CBitmapWnd3D::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CBitmapWnd3D::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CBitmapWnd3D::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CBitmapWnd3D::OnKeyDown));
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(CBitmapWnd3D::OnKeyUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CBitmapWnd3D::OnMouseCaptureLost));
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapWnd3D::OnIdle));

}

void CBitmapWnd3D::SetBitmapRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;

	this->bitmapWndRender->SetParent(this);

	vector<int> listTimer = bitmapWndRender->GetListTimer();
	for (int id : listTimer)
	{
		Connect(id, wxEVT_TIMER, wxTimerEventHandler(CBitmapWnd3D::OnTimer), nullptr, this);
	}

	vector<int> listCommand = bitmapWndRender->GetListCommand();
	for (int id : listCommand)
	{
		Connect(id, wxCommandEventHandler(CBitmapWnd3D::OnCommand));

	}
}

void CBitmapWnd3D::UpdateRenderInterface(IBitmapRenderInterface* bitmapWndRender)
{
	this->bitmapWndRender = bitmapWndRender;
}

bool CBitmapWnd3D::GetProcessEnd()
{
	return bitmapWndRender->GetProcessEnd();
}

void CBitmapWnd3D::OnIdle(wxIdleEvent& evt)
{
	bitmapWndRender->SetEndProgram(endProgram);
	bitmapWndRender->OnIdle(evt);
}

void CBitmapWnd3D::UpdateScreenRatio()
{
	bitmapWndRender->UpdateScreenRatio();
}

void CBitmapWnd3D::Resize()
{
	//updateFilter = true;
	bitmapWndRender->UpdateResized();
	this->Refresh();
}

IBitmapRenderInterface * CBitmapWnd3D::GetWndPt()
{
	return bitmapWndRender;
}

void CBitmapWnd3D::OnCommand(wxCommandEvent& event)
{
	bitmapWndRender->OnCommand(event);
}

void CBitmapWnd3D::OnTimer(wxTimerEvent& event)
{
	bitmapWndRender->OnTimer(event);
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWnd3D::~CBitmapWnd3D(void)
{
}

void CBitmapWnd3D::OnRButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnRButtonDown(event);//TRACE();
	this->SetFocus();

}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLButtonDown(wxMouseEvent& event)
{
	bitmapWndRender->OnLButtonDown(event);
	this->SetFocus();

}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLButtonUp(wxMouseEvent& event)
{
	//TRACE();
	if (event.LeftDClick())
		return;

	bitmapWndRender->OnLButtonUp(event);
}

void CBitmapWnd3D::OnKeyUp(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyUp(event);
}

void CBitmapWnd3D::OnKeyDown(wxKeyEvent& event)
{
	bitmapWndRender->OnKeyDown(event);
}

//-----------------------------------------------------------------
//Gestion du volant de souris
//-----------------------------------------------------------------
void CBitmapWnd3D::OnMouseWheel(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseWheel(event);
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CBitmapWnd3D::OnMouseMove(wxMouseEvent& event)
{
	bitmapWndRender->OnMouseMove(event);
	this->Refresh();
}


//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd3D::OnPaint(wxPaintEvent& event)
{
	if (GetWidth() == 0 || GetHeight() == 0)
		return;

	if (renderOpenGL == nullptr)
	{
		renderOpenGL = new CRenderOpenGL(this);
		renderOpenGL->Init(this);
	}

#ifdef __WXGTK__
    renderOpenGL->SetCurrent(*this);
	bitmapWndRender->OnPaint3D(this, renderOpenGL, openclContext2d);
#else
	if (openclContext == nullptr)
	{
		openclContext = Regards::OpenCL::COpenCLEngine::CreateInstance();
	}
    renderOpenGL->SetCurrent(*this);
	bitmapWndRender->OnPaint3D(this, renderOpenGL, openclContext);
#endif


}


//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLDoubleClick(wxMouseEvent& event)
{
	bitmapWndRender->OnLDoubleClick(event);
}