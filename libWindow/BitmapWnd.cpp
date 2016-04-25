#include "BitmapWnd.h"
#include <IFiltreEffet.h>
#include <RGBAQuad.h>
#include <FiltreEffet.h>
#include "ClosedHandCursor.h"
#include "ScrollbarWnd.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include <LibResource.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <algorithm> 
#include <wx/dcbuffer.h>
#include <RegardsBitmap.h>
#include "WindowMain.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#include "../include/config_id.h"
#else
#include <window_id.h>
#include <config_id.h>
#endif

using namespace Regards::Window;
const float CBitmapWnd::TabRatio[] = { 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.12f, 0.16f, 0.25f, 0.33f, 0.5f, 0.66f, 0.75f, 1.0f, 1.33f, 1.5f, 1.66f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 12.0f, 16.0f};
const long CBitmapWnd::Max = 27;

#define TIMER_RESIZE 1
#define TIMER_LOADING 4


int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16 };


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd::CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface * slider, CWindowMain * windowMain, const CThemeBitmapWindow & theme)
#ifdef WIN32
: wxGLCanvas(parent, id)
#else
: wxGLCanvas(parent, id, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
#endif

//: CWindowOpenGLMain(parent, id)
{
	config = CParamInit::getInstance();
	sliderInterface = slider;
	zoom = false;
	mouseBlock = false;
	showScroll = false;
	shrinkImage = false;
	ratio = 1.0;
	posRatio = -1;
	isInUse = false;
	toolOption = 0;
	hCursorHand = CResourceCursor::GetClosedHand();
	postEffect = false;
	posHauteur = 0;
	posLargeur = 0;
	defaultPageSize = 50;
	defaultLineSize = 5;
	showScroll = true;
	fastRender = false;
	renderInterface = nullptr;
	themeBitmap = theme;
	//backColor = CRgbaquad(theme.colorBack.Red(), theme.colorBack.Green(), theme.colorBack.Blue());
	this->windowMain = windowMain;

    Connect(wxEVT_SIZE, wxSizeEventHandler(CBitmapWnd::OnSize));
    Connect(wxEVT_PAINT, wxPaintEventHandler(CBitmapWnd::OnPaint));
	Connect(wxEVT_MOTION, wxMouseEventHandler(CBitmapWnd::OnMouseMove));
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CBitmapWnd::OnLButtonDown));
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CBitmapWnd::OnRButtonDown));
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CBitmapWnd::OnLButtonUp));
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CBitmapWnd::OnLDoubleClick));
	Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CBitmapWnd::OnMouseWheel));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(CBitmapWnd::OnKeyDown));
	Connect(wxEVT_KEY_UP, wxKeyEventHandler(CBitmapWnd::OnKeyUp));
	Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseEventHandler(CBitmapWnd::OnMouseCaptureLost));
    
    loadingTimer = new wxTimer(this, TIMER_LOADING);
    Connect(TIMER_LOADING, wxEVT_TIMER, wxTimerEventHandler(CBitmapWnd::OnLoading), nullptr, this);
    
	themeBitmap.colorBack = themeBitmap.colorScreen;
	InitRenderInterface();
}

void CBitmapWnd::OnLoading(wxTimerEvent& event)
{
    stepLoading += 1;
    if (stepLoading == 8)
        stepLoading = 0;

    loadingTimer->Start(50, true);
    this->Refresh();
}


void CBitmapWnd::StopLoadingBitmap()
{
    if(useLoadingPicture)
    {
        showLoadingBitmap = false;
        if (loadingTimer->IsRunning())
            loadingTimer->Stop();
    }
}

void CBitmapWnd::StartLoadingBitmap()
{
    if(useLoadingPicture)
    {
        stepLoading = 0;
        showLoadingBitmap = true;
        loadingTimer->Start(50, true);
    }
}

void CBitmapWnd::SetFullscreen(const bool &fullscreen)
{
	if (fullscreen)
		themeBitmap.colorBack = themeBitmap.colorFullscreen;
	else
		themeBitmap.colorBack = themeBitmap.colorScreen;
}

void CBitmapWnd::InitRenderInterface()
{
	if (renderInterface != nullptr)
		delete renderInterface;

	renderInterface = nullptr;

	if (config != nullptr)
	{
		if (config->GetPreviewLibrary() == LIBOPENGL)
		{
			renderInterface = new CRenderBitmapInterfaceOpenGL();
			return;
		}
	}
	renderInterface = new CRenderBitmapInterfaceGDI();

    
}

bool CBitmapWnd::IsGpGpuCompatible()
{
	return directComputeCompatible;
}

int CBitmapWnd::GetWidth()
{
	return GetSize().x;
}
int CBitmapWnd::GetHeight()
{
    return GetSize().y;
}

float CBitmapWnd::GetRatio()
{
	return ratio;
}

int CBitmapWnd::GetPosRatio()
{
	return posRatio;
}

void CBitmapWnd::SetRatioPos(const int &pos)
{
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio = pos;

	if (posRatio > Max)
	{
		posRatio = Max;
	}
	ratio = TabRatio[posRatio];

	CalculPositionPicture(centerX, centerY);

	this->Refresh();
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWnd::~CBitmapWnd(void)
{
}

void CBitmapWnd::SetKey(const int &iKey)
{
	this->iKey = iKey;
	switch(toolOption)
	{
		case MOVEPICTURE:
		{
			if(iKey == WXK_CONTROL)
				wxSetCursor(hCursorZoomIn);
			else
				wxSetCursor(hCursorZoomOut);
			break;
		}
	}
}

int CBitmapWnd::GetKey()
{
	return iKey;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CBitmapWnd::SetTool(const int &tool)
{
	toolOption = tool;

	switch(tool)
	{
		case ZOOMPICTURE:
			{
				if (iKey == WXK_CONTROL)
					wxSetCursor(hCursorZoomIn);
				else
					wxSetCursor(hCursorZoomOut);
			
			}
			break;

		default:
			{
				SetToolOption(tool, iKey);
				break;
			}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CBitmapWnd::GetTool()
{
	return toolOption;
}


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void CBitmapWnd::SetZoom(bool active)
{
	zoom = active;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CBitmapWnd::GetZoom()
{
	return zoom;
}

void CBitmapWnd::CalculCenterPicture()
{
	float bitmapRatioWidth = GetBitmapWidthWithRatio();
	float bitmapRatioHeight = GetBitmapHeightWithRatio();
	float screenWidth = float(width);
	float screenHeight = float(height);
	float middleScreenWidth = screenWidth / 2.0f;
	float middleScreenHeight = screenHeight / 2.0f;

	if (screenWidth > bitmapRatioWidth)
	{
		centerX = (bitmapRatioWidth / 2.0f) / ratio;
	}
	else
	{
		centerX = (posLargeur + middleScreenWidth) / ratio;
	}

	if (screenHeight > bitmapRatioHeight)
	{
		centerY = (bitmapRatioHeight / 2.0f) / ratio;
	}
	else
	{
		centerY = (posHauteur + middleScreenHeight) / ratio;
	}
}

void CBitmapWnd::CalculPositionPicture(const float &x, const float &y)
{
	float bitmapRatioWidth = GetBitmapWidthWithRatio();
	float bitmapRatioHeight = GetBitmapHeightWithRatio();
	float screenWidth = float(width);
	float screenHeight = float(height);

	float middleScreenWidth = screenWidth / 2.0f;
	float middleScreenHeight = screenHeight / 2.0f;

	float screenXPos = x * ratio;
	float screenYPos = y * ratio;

	int posLargeurMax = bitmapRatioWidth - screenWidth;
	int posHauteurMax = bitmapRatioHeight - screenHeight;

	if (screenWidth > bitmapRatioWidth)
	{
		posLargeur = 0;
	}
	else
	{
		posLargeur = min(max(0, int(screenXPos - middleScreenWidth)), posLargeurMax);
	}

	if (screenHeight > bitmapRatioHeight)
	{
		posHauteur = 0;
	}
	else
	{
		posHauteur = min(max(0, int(screenYPos - middleScreenHeight)), posHauteurMax);
	}
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::ZoomOn()
{
	CalculCenterPicture();

	if(posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio++;
	if(posRatio > Max)
	{
		posRatio = Max;
	}
	ratio = TabRatio[posRatio];

	CalculPositionPicture(centerX, centerY);

	bool update;
	UpdateScrollBar(update);

	if (update)
	{
		this->Refresh();
	}
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::SetZoomPosition(const int &position)
{
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio = position;
	if (posRatio > Max)
	{
		posRatio = Max;
	}
	ratio = TabRatio[posRatio];

	CalculPositionPicture(centerX, centerY);

	bool update;
	UpdateScrollBar(update);

	if (update)
	{
		this->Refresh();
	}
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::SetPosition(const int &left, const int &top)
{

}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::UpdateScrollBar(bool &update)
{
	update = true;
	if (showScroll)
	{
		scrollbar->SetControlSize(int(GetBitmapWidthWithRatio()), int(GetBitmapHeightWithRatio()));
		scrollbar->SetPosition(posLargeur, posHauteur);
	}
}


//-----------------------------------------------------------------
//Zoom -
//-----------------------------------------------------------------
void CBitmapWnd::ZoomOut()
{
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio--;
	if (posRatio < 0)
		posRatio = 0;

	ratio = TabRatio[posRatio];

	CalculPositionPicture(centerX, centerY);

	bool update;
	UpdateScrollBar(update);

	if (update)
	{
		this->Refresh();
	}
}

//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWnd::ShrinkImage(const bool &redraw)
{
	shrinkImage = true;
	ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	//Calcul position largeur et hauteur initial
	posLargeur = 0;
	posHauteur = 0;
	centerX = float(GetBitmapWidth()) / 2.0f;
	centerY = float(GetBitmapHeight()) / 2.0f;

	bool update;
	UpdateScrollBar(update);

	if (update)
		this->Refresh();
}


//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWnd::SetShrinkImage(bool active)
{
	if (shrinkImage != active)
	{
		shrinkImage = active;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CBitmapWnd::GetShrinkImage()
{
	return shrinkImage;
}

//-----------------------------------------------------------------
//Affichage de la barre de scrolling
//-----------------------------------------------------------------
void CBitmapWnd::SetShowScroll(bool visible)
{
	showScroll = visible;
}

//-----------------------------------------------------------------
//Définition de la méthode d'interpolation
//-----------------------------------------------------------------
bool CBitmapWnd::GetShowScroll()
{
	return showScroll;
}

//-----------------------------------------------------------------
//Définition de la méthode d'interpolation
//-----------------------------------------------------------------
void CBitmapWnd::SetInterpolation(int interpolation)
{
	interpolation = interpolation;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
int CBitmapWnd::GetInterpolation()
{
	return interpolation;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWnd::GetBitmapWidthWithRatio()
{
	return float(GetBitmapWidth()) * ratio;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWnd::GetBitmapHeightWithRatio()
{
	return  float(GetBitmapHeight()) * ratio;
}


void CBitmapWnd::SetBitmapParameter(const bool &externBitmap, const bool &addToTexture)
{
	this->externBitmap = externBitmap;
	this->addToTexture = addToTexture;
}


//-----------------------------------------------------------------
//Affectation de l'image
//-----------------------------------------------------------------
void CBitmapWnd::SetBitmap(CRegardsBitmap * bitmap, const bool &copy)
{
	this->addToTexture = true;
	toolOption = MOVEPICTURE;
	if (this->bitmap != nullptr)
	{
		delete(this->bitmap);
	}

	if (copy)
	{
		if (this->bitmap == nullptr)
			this->bitmap = new CRegardsBitmap();

		this->bitmap->SetBitmap(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		this->bitmap->SetFilename(bitmap->GetFilename());

	}
	else
		this->bitmap = bitmap;

	ShrinkImage(false);
	AfterSetBitmap();
}

void CBitmapWnd::CalculScreenPosFromReal(const int &xReal, const int &yReal, int &xScreen, int &yScreen)
{
	xScreen = int(float(xReal) * ratio);
	yScreen = int(float(yReal) * ratio);
}

void CBitmapWnd::CalculRealPosFromScreen(const int &xScreen, const int &yScreen, int &xReal, int &yReal)
{
	xReal = int(float(xScreen) / ratio);
	yReal = int(float(yScreen) / ratio);
}


//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
CRegardsBitmap * CBitmapWnd::GetBitmap()
{
	return bitmap;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::GetInfosBitmap(wxString &filename, int &widthPicture, int &heightPicture)
{
	widthPicture = bitmap->GetBitmapWidth();
	heightPicture = bitmap->GetBitmapHeight();
	filename = this->filename;
}

int CBitmapWnd::GetBitmapWidth()
{
	if (bitmap != nullptr)
		return bitmap->GetBitmapWidth();

	return 0;
}

int CBitmapWnd::GetBitmapHeight()
{
	if (bitmap != nullptr)
		return bitmap->GetBitmapHeight();

	return 0;
}

void CBitmapWnd::FlipVertical()
{
	if (bitmap != nullptr && config != nullptr)
	{
		CRgbaquad color; //theme.colorBack
		CFiltreEffet * filtre = new CFiltreEffet(bitmap, color, config->GetEffectLibrary());
		filtre->FlipVertical();
		delete filtre;
	}
	addToTexture = true;
	this->Refresh();
}

void CBitmapWnd::Rotate90()
{
	if (bitmap != nullptr && config != nullptr)
	{
		CRgbaquad color; //theme.colorBack
		CFiltreEffet * filtre = new CFiltreEffet(bitmap, color, config->GetEffectLibrary());
		filtre->Rotate90();
		delete filtre;
	}
	addToTexture = true;
    UpdateResized();
	this->Refresh();
}

void CBitmapWnd::Rotate270()
{
	if (bitmap != nullptr)
	{
		
		if (config != nullptr)
		{
			CRgbaquad color; //theme.colorBack
			CFiltreEffet * filtre = new CFiltreEffet(bitmap, color, config->GetEffectLibrary());
			filtre->Rotate270();
			delete filtre;
		}
	}
	addToTexture = true;
    UpdateResized();
	this->Refresh();
}

void CBitmapWnd::FlipHorizontal()
{
	if (bitmap != nullptr)
	{
		
		if (config != nullptr)
		{
			CRgbaquad color; //theme.colorBack
			CFiltreEffet * filtre = new CFiltreEffet(bitmap, color, config->GetEffectLibrary());
			filtre->FlipHorizontal();
			delete filtre;
		}
	}
	addToTexture = true;
	this->Refresh();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::MouseClick(const int &xPos, const int &yPos)
{
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	mouseBlock = true;
	CaptureMouse();
	SetFocus();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::MouseRelease(const int &xPos, const int &yPos)
{
	mouseBlock = false;
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
    if(HasCapture())
        ReleaseMouse();
}

void CBitmapWnd::OnRButtonDown(wxMouseEvent& event)
{
	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TOOLBAR_UPDATE_ID);
		this->windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWnd::OnLButtonDown(wxMouseEvent& event)
{
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();

	switch(toolOption)
	{
		case MOVEPICTURE:
			{
				mouseBlock = true;
				mouseScrollX = xPos;
				mouseScrollY = yPos;
				CaptureMouse();
				wxSetCursor(hCursorHand);
			}
			break;

		case ZOOMPICTURE:
			{
				wxPoint pt;
				pt.y = xPos;
				pt.x = yPos;
				//int iXImage = int(float(pt.x + posLargeur) / ratio);
				//int iYImage = int(float(pt.y + posHauteur) / ratio);
			}
			break;
	}

	MouseClick(xPos, yPos);
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::OnLButtonUp(wxMouseEvent& event)
{
    
    if(event.LeftDClick())
        return;
    
	int xPos = event.GetX();
	int yPos = event.GetY();

	switch(toolOption)
	{
		case MOVEPICTURE:
			{
				mouseBlock = false;
				mouseScrollX = xPos;
				mouseScrollY = yPos;
                if(HasCapture())
                    ReleaseMouse();
			}
			break;
	}

	MouseRelease(xPos, yPos);
}

void CBitmapWnd::OnKeyUp(wxKeyEvent& event)
{
	SetKey(0);
}

void CBitmapWnd::OnKeyDown(wxKeyEvent& event)
{
	SetKey(event.GetKeyCode());

	switch (event.GetKeyCode())
	{
	case WXK_UP:
		this->MoveTop();
		break;
	case WXK_LEFT:
		this->MoveLeft();
		break;
	case WXK_DOWN:
		this->MoveBottom();
		break;
	case WXK_RIGHT:
		this->MoveRight();
		break;

	case WXK_NUMPAD_ADD:
	case WXK_ADD:
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMIN);
            this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
            this->ZoomOn();
        }
		break;

	case WXK_NUMPAD_SUBTRACT:
	case WXK_SUBTRACT:
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMOUT);
            this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
            this->ZoomOut();
        }
		break;

	default:
		KeyPress(event.GetKeyCode());
		break;
	}


}

//-----------------------------------------------------------------
//Gestion du volant de souris
//-----------------------------------------------------------------
void CBitmapWnd::OnMouseWheel(wxMouseEvent& event)
{
#ifdef __APPLE__
    
    if (event.m_wheelRotation == 1)
        this->MoveTop();
    else if (event.m_wheelRotation == -1)
        this->MoveBottom();
    
#else
    
	if (event.m_wheelRotation == 120)
		this->MoveTop();
	else
		this->MoveBottom();
    
#endif
}

//------------------------------------------------------------------------------------
//Agrandissement de la fenête
//------------------------------------------------------------------------------------
void CBitmapWnd::OnSize(wxSizeEvent& event)
{
    int _width = event.GetSize().GetX();
    int _height = event.GetSize().GetY();

	if (_width == 20 && _height == 20)
	{
		//not corrected size
	}
	else
	{
		width = _width;
		height = _height;
        UpdateResized();
        Resize(width, height);
	}
}

int CBitmapWnd::UpdateResized()
{
    if (bitmap != nullptr)
    {
        if (shrinkImage)
        {
            ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());
            this->Refresh();
        }
        else if (showScroll)
        {
            CalculPositionPicture(centerX, centerY);
            bool update = false;
            UpdateScrollBar(update);
            if (update)
                this->Refresh();
        }
    }
    
    //Toolbar posRatio
    sliderInterface->SetTrackBarPosition(posRatio);
    return 0;
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CBitmapWnd::OnMouseMove(wxMouseEvent& event)
{
	int xPos = event.GetX();
	int yPos = event.GetY();

	switch(toolOption)
	{
		case MOVEPICTURE:
			{
                ::wxSetCursor(hCursorHand);

				if(mouseBlock && !shrinkImage)
				{
					int diffX = mouseScrollX - xPos;
					int diffY = mouseScrollY - yPos;
					posLargeur += diffX;
					posHauteur += diffY;

					mouseScrollX = xPos;
					mouseScrollY = yPos;

					TestMaxX();
					TestMaxY();

					bool update;
					UpdateScrollBar(update);

#ifdef WIN32
					TCHAR message[200];
					wsprintf(message, L"posLargeur : %d et posHauteur : %d \n",posLargeur,posHauteur);
					OutputDebugString(message);
#endif

					if (update)
						this->Refresh();
				}
			}
			break;

		case ZOOMPICTURE:
			{
				if (iKey == WXK_CONTROL)
					wxSetCursor(hCursorZoomIn);
				else
					wxSetCursor(hCursorZoomOut);
				break;
			}
	}

	MouseMove(xPos, yPos);
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWnd::TestMaxX()
{
	int xValue = int(GetBitmapWidthWithRatio()) - width;

	if(posLargeur >= xValue)
		posLargeur = xValue;

	if(posLargeur < 0)
		posLargeur = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWnd::TestMaxY()
{
	int yValue = int(GetBitmapHeightWithRatio()) - height;

	if(posHauteur >= yValue)
		posHauteur = yValue;

	if(posHauteur < 0)
		posHauteur = 0;
}


float CBitmapWnd::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)width / (float)(pictureWidth);
	else
		newRatio = (float)height / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > height)
	{
		newRatio = (float)height / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > width)
		{
			newRatio = (float)width / (float)(pictureWidth);
		}
	}

	return newRatio;
}
//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CBitmapWnd::CalculRatio(const int &pictureWidth, const int &pictureHeight)
{
	float newRatio = CalculPictureRatio(pictureWidth, pictureHeight);

	//Détermination du ration par rapport au tableau
	
	if(newRatio != 0.0)
	{
		for(int i = 0;i < Max;i++)
		{
			if(newRatio < TabRatio[i])
			{
				ratio = TabRatio[i];
				posRatio = i;
				break;
			}
		}
	}

	return newRatio;
}

void CBitmapWnd::GetPositionPicture(int &left, int &top)
{
	int tailleAffichageWidth = int(GetBitmapWidthWithRatio());
	int tailleAffichageHeight = int(GetBitmapHeightWithRatio());

	if(width > tailleAffichageWidth)
		left = ((width - tailleAffichageWidth) / 2);
	else
	{
		left = 0;
	}

	if(height > tailleAffichageHeight)
	{
		top = ((height - tailleAffichageHeight) / 2);
	}
	else
	{
		top = 0;
	}
}

void CBitmapWnd::TestMaxPosition()
{
	TestMaxX();
	TestMaxY();
}

CBitmapWindowContext& CBitmapWnd::GetContext(wxGLCanvas *canvas)
{
    CBitmapWindowContext *glContext;
    
    if ( !m_glContext )
    {
        // Create the OpenGL context for the first mono window which needs it:
        // subsequently created windows will all share the same context.
        m_glContext = new CBitmapWindowContext(canvas);
    }
    glContext = m_glContext;
    
    glContext->SetCurrent(*canvas);
    
    return *glContext;
}

void CBitmapWnd::CalculRectPictureInterpolation(wxRect &rc, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top)
{
	int widthOutput = int(GetBitmapWidthWithRatio());
	int heightOutput = int(GetBitmapHeightWithRatio());
	int xValue = 0;
	int yValue = 0;


	if (widthOutput > width)
	{
		left = 0;
		xValue = posLargeur;
	}
	else
	{
		xValue = 0;
		left = (width - widthOutput) / 2;
	}

	widthInterpolationSize = width - (left * 2);


	if (heightOutput > height)
	{
		top = 0;
		yValue = posHauteur;
	}
	else
	{
		yValue = 0;
		top = (height - heightOutput) / 2;
	}

	heightInterpolationSize = height - (top * 2);

	rc.x = xValue;
	rc.y = (heightOutput - height) - yValue;
	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CBitmapWnd::Update()
{ 
	bool update;
	UpdateScrollBar(update);

	if (update)
		this->Refresh();
}

void CBitmapWnd::RenderBitmap(CRenderBitmapInterface * renderInterface)
{
	if (bitmap != nullptr && width > 0 && height > 0)
	{
		int left, top;
		left = top = 0;
		GetPositionPicture(left, top);

		int widthOutput = int(GetBitmapWidthWithRatio());
		int heightOutput = int(GetBitmapHeightWithRatio());

		if (width >= widthOutput && height >= heightOutput)
		{
			renderInterface->AddBitmapToScale(L"BitmapSource", widthOutput, heightOutput, left, top);
		}
		else
		{
			wxRect rc = { 0, 0, 0, 0 };
			CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top);
			renderInterface->AddBitmapToScalePosition(L"BitmapSource", widthOutput, heightOutput, rc, left, top);
		}
	}
	
}

bool CBitmapWnd::NeedAfterDrawBitmap()
{
	return false;
}


void CBitmapWnd::DrawBitmap(wxDC * dc)
{
	CRgbaquad backColor = CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
    
    //CRgbaquad backColor = CRgbaquad(255,0,0);
	if (renderInterface->GetType() == RENDEROPENGL)
	{
		CBitmapWindowContext&  context = GetContext(this);
		//CRenderBitmapInterfaceOpenGL* openGLRender = (CRenderBitmapInterfaceOpenGL *)renderInterface;
		renderInterface->CreateScreenTexture(width, height);
		renderInterface->CreateScreenRender(dc, backColor);
        if (bitmap != nullptr)
        {
            //Test si la texture existe
            int textureId = renderInterface->GetTextureID(L"BitmapSource");
            if (addToTexture || textureId == 0)
            {
                renderInterface->AddTextureBitmap(bitmap, L"BitmapSource");
            }
        }
        
        BeforeDrawBitmap(renderInterface);
        RenderBitmap(renderInterface);
        AfterRenderBitmap(renderInterface);
		 if(NeedAfterDrawBitmap())
        {
			 wxBufferedPaintDC dcLocal(this); 
			 renderInterface->RenderToScreen(&dcLocal);
			 AfterDrawBitmap(&dcLocal);
		 }
		else
		{
			SwapBuffers();
		}

	}
	else
	{
		renderInterface->CreateScreenRender(dc, backColor);
		renderInterface->CreateScreenTexture(width, height);
        if (bitmap != nullptr)
        {
            //Test si la texture existe
            int textureId = renderInterface->GetTextureID(L"BitmapSource");
            if (addToTexture || textureId == 0)
            {
                renderInterface->AddTextureBitmap(bitmap, L"BitmapSource");
            }
        }
        
        BeforeDrawBitmap(renderInterface);
        RenderBitmap(renderInterface);
        AfterRenderBitmap(renderInterface);
        renderInterface->RenderToScreen(dc);
        AfterDrawBitmap(dc);
	}
		
	addToTexture = false;
}

void CBitmapWnd::UpdateScreenRatio()
{
    Update();
}

//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd::OnPaint(wxPaintEvent& event)
{
    bool update = false;
    UpdateScrollBar(update);
    
	if (renderInterface->GetType() == RENDEROPENGL)	
		DrawBitmap(nullptr);
	else
	{
		wxBufferedPaintDC dc(this);  
		if (bitmap != nullptr && width > 0 && height > 0)
			DrawBitmap(&dc);
		else
		{
			wxRect rc = { 0, 0, width, height };
			CWindowMain::FillRect(&dc, rc, themeBitmap.colorBack);
		}
	}
}

//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd::OnLDoubleClick(wxMouseEvent& event)
{
#ifdef __APPLE__
    OnRButtonDown(event);
#else
    ShrinkImage(true);
#endif
}