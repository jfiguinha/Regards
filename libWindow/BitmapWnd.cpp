#include "header.h"
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
#include <wx/dcbuffer.h>
#include <RegardsBitmap.h>
#include <FilterData.h>
#include <MetadataExiv2.h>
#include <window_id.h>
#include <config_id.h>
#include <OpenCLEffect.h>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#endif
#include <utility.h>
#include <ImageLoadingFormat.h>
#include <RegardsFloatBitmap.h>

using namespace Regards::FiltreEffet;
using namespace Regards::Window;
using namespace Regards::exiv2;
const float CBitmapWnd::TabRatio[] = { 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.12f, 0.16f, 0.25f, 0.33f, 0.5f, 0.66f, 0.75f, 1.0f, 1.33f, 1.5f, 1.66f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 12.0f, 16.0f};
const long CBitmapWnd::Max = 27;

#define TIMER_RESIZE 1
#define TIMER_LOADING 4


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd::CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID idMain, const CThemeBitmapWindow & theme)
//	: CWindowMain(parent, id)
: CWindowOpenGLMain("CBitmapWnd", parent, id)
{
	isOpenGL = true;
	glTexture = nullptr;
	renderOpenGL = new CRenderBitmapInterfaceOpenGL(this);
#ifdef WIN32
    renderOpenGL->Init(this);
#endif 
	idWindowMain = idMain;
	//bitmap = nullptr;
	sliderInterface = nullptr;
	config = nullptr;

    openCLEngine = nullptr;
	filtreEffet = nullptr;
	flipVertical = 0;
	flipHorizontal = 0;
	angle = 0;

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
	themeBitmap = theme;

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

	bitmapwidth = 0;
	bitmapheight = 0;
	bitmapUpdate = false;
	source = nullptr;
	bitmapLoad = false;

	themeBitmap.colorBack = themeBitmap.colorScreen;
	filterInterpolation = CUBICFILTER;
}

void CBitmapWnd::SetFilterInterpolation(const int &filter)
{
    TRACE();
	filterInterpolation = filter;
    RefreshWindow();
}


void CBitmapWnd::SetFullscreen(const bool &fullscreen)
{
    TRACE();
	if (fullscreen)
		themeBitmap.colorBack = themeBitmap.colorFullscreen;
	else
		themeBitmap.colorBack = themeBitmap.colorScreen;
}

int CBitmapWnd::GetWidth()
{
    TRACE();
	return GetSize().x;
}
int CBitmapWnd::GetHeight()
{
    TRACE();
    return GetSize().y;
}

float CBitmapWnd::GetRatio()
{
    TRACE();
	return ratio;
}

int CBitmapWnd::GetPosRatio()
{
    TRACE();
	return posRatio;
}

void CBitmapWnd::SetRatioPos(const int &pos)
{
    TRACE();
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
    RefreshWindow();
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWnd::~CBitmapWnd(void)
{
    TRACE();
	if(filtreEffet != nullptr)
		delete filtreEffet;
	filtreEffet = nullptr;

	if (openCLEngine != nullptr)
		delete openCLEngine;
	openCLEngine = nullptr;

	if(renderOpenGL != nullptr)
		delete renderOpenGL;

}

void CBitmapWnd::SetKey(const int &iKey)
{
    TRACE();
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
    TRACE();
	return iKey;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CBitmapWnd::SetTool(const int &tool)
{
    TRACE();
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
    TRACE();
	return toolOption;
}


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void CBitmapWnd::SetZoom(bool active)
{
    TRACE();
	zoom = active;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CBitmapWnd::GetZoom()
{
    TRACE();
	return zoom;
}

void CBitmapWnd::CalculCenterPicture()
{
    TRACE();
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
    TRACE();
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
    TRACE();
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

    updateFilter = true;

	bool update;
	UpdateScrollBar(update);

	if (update)
	{
        RefreshWindow();
	}
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::SetZoomPosition(const int &position)
{
    TRACE();
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
        RefreshWindow();
	}
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::SetPosition(const int &left, const int &top)
{
    TRACE();
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::UpdateScrollBar(bool &update)
{
    TRACE();
	update = true;
	if (showScroll)
	{
        
		scrollbar->SetControlSize(int(GetBitmapWidthWithRatio()), int(GetBitmapHeightWithRatio()), true);
		scrollbar->SetPosition(posLargeur, posHauteur);
		posLargeur = scrollbar->GetPosLargeur();
		posHauteur = scrollbar->GetPosHauteur();
	}
}


//-----------------------------------------------------------------
//Zoom -
//-----------------------------------------------------------------
void CBitmapWnd::ZoomOut()
{
    TRACE();
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio--;
	if (posRatio < 0)
		posRatio = 0;

	ratio = TabRatio[posRatio];

	CalculPositionPicture(centerX, centerY);

    updateFilter = true;
    
	bool update;
	UpdateScrollBar(update);

	if (update)
	{
        RefreshWindow();
	}
}

//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWnd::ShrinkImage(const bool &redraw)
{
    TRACE();
	shrinkImage = true;
	ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	//Calcul position largeur et hauteur initial
	posLargeur = 0;
	posHauteur = 0;
	centerX = float(GetBitmapWidth()) / 2.0f;
	centerY = float(GetBitmapHeight()) / 2.0f;

    updateFilter = true;

	bool update;
	UpdateScrollBar(update);

	if (update && redraw)
    {
        RefreshWindow();
    }
}


//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWnd::SetShrinkImage(bool active)
{
    TRACE();
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
    TRACE();
	return shrinkImage;
}

//-----------------------------------------------------------------
//Affichage de la barre de scrolling
//-----------------------------------------------------------------
void CBitmapWnd::SetShowScroll(bool visible)
{
    TRACE();
	showScroll = visible;
}

//-----------------------------------------------------------------
//Définition de la méthode d'interpolation
//-----------------------------------------------------------------
bool CBitmapWnd::GetShowScroll()
{
    TRACE();
	return showScroll;
}

//-----------------------------------------------------------------
//Définition de la méthode d'interpolation
//-----------------------------------------------------------------
void CBitmapWnd::SetInterpolation(int interpolation)
{
    TRACE();
	this->interpolation = interpolation;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
int CBitmapWnd::GetInterpolation()
{
    TRACE();
	return interpolation;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWnd::GetBitmapWidthWithRatio()
{
    TRACE();
	return float(GetBitmapWidth()) * ratio;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWnd::GetBitmapHeightWithRatio()
{
    TRACE();
	return  float(GetBitmapHeight()) * ratio;
}


void CBitmapWnd::SetBitmapParameter(const bool &externBitmap, const bool &addToTexture)
{
    TRACE();
	this->externBitmap = externBitmap;
	//if (addToTexture)
	//	render.Destroy();
	//this->addToTexture = addToTexture;
}

void CBitmapWnd::SetIsBitmapThumbnail(const bool &isThumbnail)
{
    TRACE();
    this->isThumbnail = isThumbnail;
}

//-----------------------------------------------------------------
//Affectation de l'image
//-----------------------------------------------------------------
void CBitmapWnd::SetBitmap(CImageLoadingFormat * bitmapIn, const bool &copy)
{
    TRACE();
    printf("CBitmapWnd::SetBitmap \n");
    //this->SetFocus();
	if(bitmapIn != nullptr)
	{
		if(bitmapIn->IsOk())
		{
			bitmapLoad = true;
			filename = bitmapIn->GetFilename();
			bitmapUpdate = true;
			flipVertical = 0;
			flipHorizontal = 0;
			angle = 0;
			isOpenGL = true;
            
            printf("CBitmapWnd::SetBitmap  muBitmap.lock()\n");
            muBitmap.lock();
            if(source != nullptr)
            {
                delete source;
                printf("CBitmapWnd::SetBitmap   delete source\n");
                source = nullptr;
            }            

			source = bitmapIn;
            //source = nullptr;
            muBitmap.unlock();
            printf("CBitmapWnd::SetBitmap  muBitmap.unlock()\n");
			toolOption = MOVEPICTURE;
			bitmapwidth = bitmapIn->GetWidth();
			bitmapheight = bitmapIn->GetHeight();
			orientation = bitmapIn->GetOrientation();
			ShrinkImage(false);
			AfterSetBitmap();
            RefreshWindow();
          
		}
	}
}

void CBitmapWnd::CalculScreenPosFromReal(const int &xReal, const int &yReal, int &xScreen, int &yScreen)
{
    TRACE();
	xScreen = int(float(xReal) * ratio);
	yScreen = int(float(yReal) * ratio);
}

void CBitmapWnd::CalculRealPosFromScreen(const int &xScreen, const int &yScreen, int &xReal, int &yReal)
{
    TRACE();
	xReal = int(float(xScreen) / ratio);
	yReal = int(float(yScreen) / ratio);
}


//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
CRegardsBitmap * CBitmapWnd::GetBitmap(const bool &source)
{
    TRACE();
	if(filtreEffet != nullptr && bitmapLoad)
	{
		CRegardsBitmap * bitmap =  filtreEffet->GetBitmap(source);
		if (bitmap != nullptr)
		{
			bitmap->SetOrientation(orientation);
			bitmap->SetFilename(this->filename);
		}
		return bitmap;
	}
	return nullptr;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
CRegardsFloatBitmap * CBitmapWnd::GetFloatBitmap(const bool &source)
{
    TRACE();
	if(filtreEffet != nullptr && bitmapLoad)
	{
		CRegardsFloatBitmap * bitmap =  filtreEffet->GetFloatBitmap(source);
		return bitmap;
	}
	return nullptr;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::GetInfosBitmap(wxString &filename, int &widthPicture, int &heightPicture)
{
    TRACE();
	widthPicture =bitmapwidth;
	heightPicture = bitmapheight;
	filename = this->filename;
}

int CBitmapWnd::GetOrientation()
{
    TRACE();
	return orientation;
}

int CBitmapWnd::GetRawBitmapWidth()
{
    TRACE();
	return bitmapwidth;
}

int CBitmapWnd::GetRawBitmapHeight()
{
    TRACE();
	return bitmapheight; 
}

int CBitmapWnd::GetBitmapWidth()
{
    TRACE();
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	GenerateExifPosition(localAngle, localflipHorizontal, localflipVertical);

	if (bitmapLoad)
	{
		if(localAngle == 90 || localAngle == 270)
			return GetRawBitmapHeight(); 
		else
			return GetRawBitmapWidth();
	}
	return 0;
}

int CBitmapWnd::GetBitmapHeight()
{
    TRACE();
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	GenerateExifPosition(localAngle, localflipHorizontal, localflipVertical);

	if (bitmapLoad)
	{
		if(localAngle == 90 || localAngle == 270)
			return GetRawBitmapWidth();
		else
			return GetRawBitmapHeight(); 
			
	}

	return 0;
}

void CBitmapWnd::FlipVertical()
{
    TRACE();
	if(flipVertical)
		flipVertical = 0;
	else
		flipVertical = 1;
    updateFilter = true;
    RefreshWindow();
}

void CBitmapWnd::Rotate90()
{
    TRACE();
	angle += 90;
	angle = angle % 360;
    updateFilter = true;
    UpdateResized();
    RefreshWindow();
}

void CBitmapWnd::Rotate270()
{
    TRACE();
	angle += 270;
	angle = angle % 360;
    updateFilter = true;
    UpdateResized();
    RefreshWindow();
}

void CBitmapWnd::FlipHorizontal()
{
    TRACE();
	if(flipHorizontal)
		flipHorizontal = 0;
	else
		flipHorizontal = 1;
    updateFilter = true;
    RefreshWindow();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::MouseClick(const int &xPos, const int &yPos)
{
    TRACE();
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
    TRACE();
	mouseBlock = false;
	wxSetCursor(wxCursor(wxCURSOR_ARROW));
    if(HasCapture())
        ReleaseMouse();
}

void CBitmapWnd::OnRButtonDown(wxMouseEvent& event)
{
    TRACE();
    this->SetFocus();
	wxWindow * window = this->FindWindowById(idWindowMain);
	if (window != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TOOLBAR_UPDATE_ID);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWnd::OnLButtonDown(wxMouseEvent& event)
{
    TRACE();
	this->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
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

	MouseClick(xPos * scale_factor, yPos * scale_factor);
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWnd::OnLButtonUp(wxMouseEvent& event)
{
    TRACE();
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
    TRACE();
	SetKey(0);
    
}

void CBitmapWnd::OnKeyDown(wxKeyEvent& event)
{
    TRACE();
	SetKey(event.GetKeyCode());

	switch (event.GetKeyCode())
	{
	case WXK_UP:
        updateFilter = true;
		this->MoveTop();
		break;
	case WXK_LEFT:
        updateFilter = true;
		this->MoveLeft();
		break;
	case WXK_DOWN:
        updateFilter = true;
		this->MoveBottom();
		break;
	case WXK_RIGHT:
        updateFilter = true;
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
    TRACE();
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
    TRACE();
    updateContext = true;
    int _width =  event.GetSize().GetX();
    int _height =  event.GetSize().GetY();

	if (_width == 20 && _height == 20)
	{
		//not corrected size
	}
	else
	{
#ifdef __WXGTK__        
        double scale_factor = GetContentScaleFactor();
#else
        double scale_factor = 1.0f;
#endif
		width = _width * scale_factor;
		height = _height * scale_factor;        
		//width = _width;
		//height = _height;
        updateFilter = true;
        UpdateResized();
        Resize();
	}
}

int CBitmapWnd::UpdateResized()
{
    TRACE();
    if (bitmapLoad)
    {
	    if (shrinkImage)
        {
            ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());
            RefreshWindow();
        }
        else if (showScroll)
        {
            CalculPositionPicture(centerX, centerY);
            bool update = false;
            UpdateScrollBar(update);
            if (update)
            {
                RefreshWindow();         
            }

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
    TRACE();
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
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

#if defined(WIN32) && defined(_DEBUG)
					TCHAR message[200];
					wsprintf(message, L"posLargeur : %d et posHauteur : %d \n",posLargeur,posHauteur);
					OutputDebugString(message);
#endif

                    updateFilter = true;
                    
					if (update)
                    {
                        RefreshWindow();
                    }
                    //Refresh();
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

	MouseMove(xPos * scale_factor, yPos * scale_factor);
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWnd::TestMaxX()
{
    TRACE();
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
    TRACE();
	int yValue = int(GetBitmapHeightWithRatio()) - height;

	if(posHauteur >= yValue)
		posHauteur = yValue;

	if(posHauteur < 0)
		posHauteur = 0;
}


float CBitmapWnd::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
    TRACE();
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
    TRACE();
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


void CBitmapWnd::TestMaxPosition()
{
    TRACE();
	TestMaxX();
	TestMaxY();
}


void CBitmapWnd::CalculRectPictureInterpolation(wxRect &rc, int &widthInterpolationSize, int &heightInterpolationSize, int &left, int &top, const bool &invert)
{
    TRACE();
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

	rc.x = max(xValue,0);
	if (invert)
		rc.y = max((heightOutput - height) - yValue, 0);
	else
		rc.y = max(yValue,0);
	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CBitmapWnd::Update()
{ 
    TRACE();
	bool update;
	UpdateScrollBar(update);

	if (update)
    {
        RefreshWindow();
    }
}

bool CBitmapWnd::NeedAfterDrawBitmap()
{
    TRACE();
	return false;
}

void CBitmapWnd::UpdateScreenRatio()
{
    TRACE();
    Update();
}

void CBitmapWnd::GenerateExifPosition(int & localAngle, int & localflipHorizontal, int & localflipVertical)
{
    TRACE();
	/*
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	*/
    //bool ret = true;
    switch (GetOrientation())
    {
        case 1:// top left side
            break;
        case 2:// top right side
            localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            break;
        case 3:// bottom right side
            localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            localflipVertical = (localflipVertical == 1) ? 0:1;
            break;
        case 4:// bottom left side
            localflipVertical = (localflipVertical == 1) ? 0:1;
            break;
        case 5://left side top
            localAngle += 90;
            localflipVertical = (localflipVertical == 1) ? 0:1;
            //localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            break;
        case 6:// right side top
            localAngle += 90;
			localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            localflipVertical = (localflipVertical == 1) ? 0:1;
            break;
        case 7:// right side bottom
            localAngle += 90;
			localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            break;
        case 8:// left side bottom
            localAngle += 90;
            //localflipHorizontal = (localflipHorizontal == 1) ? 0:1;
            break;
    }

	localAngle = localAngle % 360;
	//localAngle = 360 - localAngle;
}



void CBitmapWnd::GenerateScreenBitmap(CFiltreEffet * filtreEffet, int &widthOutput, int &heightOutput)
{
    TRACE();
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	//if(raw)
	//	GenerateExifRawPosition(localAngle, localflipHorizontal, localflipVertical);
	//else
	GenerateExifPosition(localAngle, localflipHorizontal, localflipVertical);

	if (width >= widthOutput && height >= heightOutput)
	{
		filtreEffet->Interpolation(widthOutput, heightOutput, filterInterpolation, localflipHorizontal, localflipVertical, localAngle);
	}
	else
	{
		int left = 0, top = 0;
		int tailleAffichageWidth = widthOutput;
		int tailleAffichageHeight = heightOutput;

		if (width > tailleAffichageWidth)
			left = ((width - tailleAffichageWidth) / 2);
		else
			left = 0;

		if (height > tailleAffichageHeight)
			top = ((height - tailleAffichageHeight) / 2);
		else
			top = 0;

		wxRect rc(0, 0, 0, 0);
		CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, true);
		filtreEffet->Interpolation(widthOutput, heightOutput, rc, filterInterpolation, localflipHorizontal, localflipVertical, localAngle);
	}
}

//#define __OPENGLONLY__

//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd::OnPaint(wxPaintEvent& event)
{
    TRACE();
    CRgbaquad color;

        
    #if defined(WIN32) && defined(_DEBUG)
        DWORD tickCount = GetTickCount();
        OutputDebugString(L"OnPaint\n");
    #endif

    printf("CBitmapWnd OnPaint \n");

    #ifndef WIN32
        if(!renderOpenGL->IsInit())
        {
            renderOpenGL->Init(this);
            
        }
    #endif

    #ifdef __WXGTK__
        double scale_factor = GetContentScaleFactor();
    #else
        double scale_factor = 1.0f;
    #endif 

        if (width == 0 || height == 0)
            return;
            
        int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
        int heightOutput = int(GetBitmapHeightWithRatio())* scale_factor;

        if (widthOutput <0 || heightOutput < 0)
            return;
            
    #ifdef WIN32
        renderOpenGL->SetCurrent(*this);
    #else
        if(updateContext)
        {
            renderOpenGL->SetCurrent(*this);
            updateContext = false;
        }
    #endif
               
		int supportOpenCL = 0;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
			supportOpenCL = config->GetIsOpenCLSupport();

		if (!supportOpenCL)
		{

			if (source != nullptr)
			{
				if (filtreEffet != nullptr)
					delete filtreEffet;

				filtreEffet = new CFiltreEffet(color, nullptr, source);

				renderOpenGL->LoadingResource(scale_factor);


				if (bitmapLoad && width > 0 && height > 0)
				{
					GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);
				}

				if (NeedAfterRenderBitmap())
					ApplyPreviewEffect();

				CRegardsBitmap* bitmap = filtreEffet->GetBitmap(false);

				glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, nullptr);
				if (glTexture != nullptr)
					glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
				else
				{
					//Error
					printf("CBitmapWnd GetDisplayTexture Error \n");
				}
				delete bitmap;

				renderOpenGL->CreateScreenRender(width, height, CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue()));
			}

			if (glTexture != nullptr)
			{
				int x = (width - glTexture->GetWidth()) / 2;
				int y = (height - glTexture->GetHeight()) / 2;
				renderOpenGL->RenderToScreen(x, y, false);
				AfterRender();

			}

		}
		else
		{
			if (openCLEngine == nullptr)
			{
				openCLEngine = new COpenCLEngine();
				if (openCLEngine != nullptr)
					openclContext = openCLEngine->GetInstance();

				CreateContext();

				renderOpenGL->LoadingResource(scale_factor);
			}

			muBitmap.lock();

			if (openCLEngine != nullptr && source != nullptr)
			{
				if (filtreEffet != nullptr)
					delete filtreEffet;

				filtreEffet = new CFiltreEffet(color, openclContext, source);
			}


			muBitmap.unlock();

			bool update = false;
			UpdateScrollBar(update);

			if (isOpenGL)
			{
				if (bitmapLoad && width > 0 && height > 0)
				{
					int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
					int heightOutput = int(GetBitmapHeightWithRatio()) * scale_factor;

					if (widthOutput < 0 || heightOutput < 0)
						return;

					GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

					if (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL)
					{
						printf("CBitmapWnd IsSharedContextCompatible \n");

						try
						{
							glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, openclContext->GetContext());
						}
						catch (...)
						{
							return;
						}
						if (NeedAfterRenderBitmap())
							ApplyPreviewEffect();

						try
						{
							cl_int err;
							cl_mem cl_image = renderOpenGL->GetOpenCLTexturePt();
							err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
							Error::CheckError(err);
							filtreEffet->GetRgbaBitmap(cl_image);
							err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
							Error::CheckError(err);
							err = clFlush(openclContext->GetCommandQueue());
							Error::CheckError(err);
						}
						catch (...)
						{

						}
					}
					else //not shared compatibility
					{
						printf("CBitmapWnd Is Not SharedContextCompatible \n");

						if (NeedAfterRenderBitmap())
							ApplyPreviewEffect();


						CRegardsBitmap* bitmap = filtreEffet->GetBitmap(false);
						if (!filtreEffet->OpenCLHasEnoughMemory() && openclContext != nullptr)
							bitmap->VertFlipBuf();

						glTexture = renderOpenGL->GetDisplayTexture();

						if (glTexture != nullptr)
							glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
						else
							printf("CBitmapWnd GetDisplayTexture Error \n");
						delete bitmap;

					}

				}

				renderOpenGL->CreateScreenRender(width, height, CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue()));

				if (glTexture != nullptr)
				{
					int x = (width - glTexture->GetWidth()) / 2;
					int y = (height - glTexture->GetHeight()) / 2;
					if (openclContext->IsSharedContextCompatible())
						renderOpenGL->RenderToScreen(x, y, true);
					else
						renderOpenGL->RenderToScreen(x, y, false);
				}
				AfterRender();

			}
			else if (bitmapLoad)
			{

				//CFiltreEffet _filtreEffet(color, openclContext);
				int widthOutput = int(GetBitmapWidthWithRatio());
				int heightOutput = int(GetBitmapHeightWithRatio());

				wxBitmap test_bitmap(width, height);
				wxMemoryDC dc;
				dc.SelectObject(test_bitmap);
				wxRect rc(0, 0, width, height);
				CWindowMain::FillRect(&dc, rc, themeBitmap.colorBack);


				if (bitmapLoad && width > 0 && height > 0)
				{
					wxImage render;

					int left = 0, top = 0;


					if (NeedAfterRenderBitmap())
					{
						render = RenderBitmap(&dc);
					}
					else
					{
						GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);
						render = filtreEffet->GetwxImage();
					}

					if (render.IsOk())
					{
						left = (width - render.GetWidth()) / 2;
						top = (height - render.GetHeight()) / 2;
						dc.DrawBitmap(render, left, top);
						AfterRenderBitmap(&dc);
						AfterDrawBitmap(&dc);
					}
				}

				//Import render bitmap into texture and show it

				dc.SelectObject(wxNullBitmap);
				CImageLoadingFormat imageLoadingFormat(false);
				wxImage renderImage = test_bitmap.ConvertToImage();
				imageLoadingFormat.SetPicture(&renderImage);
				CFiltreEffet _filtreEffet(color, openclContext, &imageLoadingFormat);

				if (openclContext->IsSharedContextCompatible() && _filtreEffet.GetLib() == LIBOPENCL)
				{
					printf("CBitmapWnd Is SharedContextCompatible \n");
					glTexture = renderOpenGL->GetDisplayTexture(width, height, openclContext->GetContext());

					try
					{
						cl_int err;
						cl_mem cl_image = renderOpenGL->GetOpenCLTexturePt();
						err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
						_filtreEffet.GetRgbaBitmap(cl_image);
						err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
						Error::CheckError(err);
						err = clFlush(openclContext->GetCommandQueue());
						Error::CheckError(err);
					}
					catch (...)
					{

					}
				}
				else
				{
					printf("CBitmapWnd Is Not SharedContextCompatible \n");

					CRegardsBitmap* bitmap = imageLoadingFormat.GetRegardsBitmap(true);

					if (!filtreEffet->OpenCLHasEnoughMemory() && openclContext != nullptr)
						bitmap->VertFlipBuf();

					glTexture = renderOpenGL->GetDisplayTexture();
					if (glTexture != nullptr)
						glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
					else
					{
						//Error
						printf("CBitmapWnd GetDisplayTexture Error \n");
					}
					delete bitmap;
				}
				renderOpenGL->CreateScreenRender(width, height, CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue()));

				if (glTexture != nullptr)
				{
					if (openclContext->IsSharedContextCompatible() && _filtreEffet.GetLib() == LIBOPENCL)
						renderOpenGL->RenderToScreen(0, 0, true);
					else
						renderOpenGL->RenderToScreen(0, 0, false);
				}
			}
		}

	this->SwapBuffers();

    printf("CBitmapWnd End OnPaint \n");
	//scrollbar->SetPosition(posLargeur, posHauteur);

#if defined(WIN32) && defined(_DEBUG)
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif

   
}

void CBitmapWnd::RefreshWindow()
{
	Refresh();
}

//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWnd::OnLDoubleClick(wxMouseEvent& event)
{
    TRACE();
#ifdef __APPLE__
    OnRButtonDown(event);
#else
    ShrinkImage(true);
#endif
}
