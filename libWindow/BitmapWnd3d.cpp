// ReSharper disable All
#include "header.h"
#include "BitmapWnd3d.h"
#include "BitmapWndRender.h"
#include <RenderOpenGL.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>


#include <opencv2/core/opengl.hpp>
#include <utility.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#elif defined(__WXGTK__)
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#endif
#endif


extern cv::ocl::OpenCLExecutionContext clExecCtx;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd3D::CBitmapWnd3D(wxWindow* parent, wxWindowID id)
	: CWindowOpenGLMain("CBitmapWnd3D", parent, id)
{
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
	Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(CBitmapWnd3D::OnMouseLeave));
}

void CBitmapWnd3D::OnMouseLeave(wxMouseEvent& event)
{
	if (HasCapture())
		ReleaseMouse();
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

IBitmapRenderInterface* CBitmapWnd3D::GetWndPt()
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
	bitmapWndRender->OnRButtonDown(event); //
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
	//
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
	//this->Refresh();
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


	/* clock_t clock(void) returns the number of clock ticks
   elapsed since the program was launched.To get the number
   of seconds used by the CPU, you will need to divide by
   CLOCKS_PER_SEC.where CLOCKS_PER_SEC is 1000000 on typical
   32 bit system.  */
   

	//clock_t start, end;

	// Recording the starting clock tick.
	//start = clock();

	if (!clExecCtx.empty())
		clExecCtx.bind();
	renderOpenGL->SetCurrent(*this);

    /*
	// Recording the end clock tick.
	end = clock();

	// Calculating total time taken by the program.
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	cout << "Time taken by program is : " << fixed
		<< time_taken << setprecision(5);
	cout << " sec " << endl;
    */

	bitmapWndRender->OnPaint3D(this, renderOpenGL);
}


//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd3D::OnLDoubleClick(wxMouseEvent& event)
{
	bitmapWndRender->OnLDoubleClick(event);
}
