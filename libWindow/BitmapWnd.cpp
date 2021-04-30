#include "header.h"
#include "BitmapWnd.h"
#include <RGBAQuad.h>
#include <FiltreEffet.h>
#include "ClosedHandCursor.h"
#include "ScrollbarWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <wx/dcbuffer.h>
#include <RegardsBitmap.h>
#include <FilterData.h>
#include <MetadataExiv2.h>
#include <window_id.h>
#include <config_id.h>
#include <SqlPhotos.h>
#include <OpenCLEffect.h>
#include <FileUtility.h>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#endif
#include <ImageLoadingFormat.h>
#include <RegardsFloatBitmap.h>
#include <OpenCLContext.h>
#include <OpenCLEngine.h>
#include "RenderBitmapOpenGL.h"
#include <utility.h>
using namespace Regards::Sqlite;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;
using namespace Regards::exiv2;
using namespace Regards::OpenCL;
const float CBitmapWnd::TabRatio[] = { 0.01f, 0.02f, 0.03f, 0.04f, 0.05f, 0.06f, 0.08f, 0.12f, 0.16f, 0.25f, 0.33f, 0.5f, 0.66f, 0.75f, 1.0f, 1.33f, 1.5f, 1.66f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 12.0f, 16.0f};
const long CBitmapWnd::Max = 26;

#define TIMER_RESIZE 1
#define TIMER_LOADING 4

#ifndef RENDEROPENGL
extern COpenCLEngine * openCLEngine;
extern COpenCLContext * openclContext;
#endif

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWnd::CBitmapWnd(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID idMain, const CThemeBitmapWindow & theme)
//	: CWindowMain(parent, id)
#ifdef RENDEROPENGL
: CWindowOpenGLMain("CBitmapWnd", parent, id)
#else
: CWindowMain("CBitmapWnd", parent, id)
#endif
{
#ifdef RENDEROPENGL
	pageCurlOpenGL = nullptr;
	glTexture = nullptr;
	openCLEngine = nullptr;
	renderOpenGL = nullptr;
#endif
	idWindowMain = idMain;
	//bitmap = nullptr;
	sliderInterface = nullptr;
	config = nullptr;
    
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
	showScroll = true;
	themeBitmap = theme;

	mouseUpdate = nullptr;
	effectParameter = nullptr;

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
	Connect(wxEVENT_UPDATEBITMAP, wxCommandEventHandler(CBitmapWnd::OnUpdateBitmap));
	Connect(wxEVENT_LEFTPOSITION, wxCommandEventHandler(CBitmapWnd::OnLeftPosition));
	Connect(wxEVENT_TOPPOSITION, wxCommandEventHandler(CBitmapWnd::OnTopPosition));
	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CBitmapWnd::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CBitmapWnd::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CBitmapWnd::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CBitmapWnd::OnMoveBottom));
	Connect(wxEVENT_SCROLLMOVE, wxCommandEventHandler(CBitmapWnd::OnScrollMove));
	bitmapwidth = 0;
	bitmapheight = 0;
	bitmapUpdate = false;
	source = nullptr;
	bitmapLoad = false;

	themeBitmap.colorBack = themeBitmap.colorScreen;
	//filterInterpolation = CUBICFILTER;
}

void CBitmapWnd::OnScrollMove(wxCommandEvent& event)
{
	isMoving = event.GetInt();
}

int CBitmapWnd::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	return supportOpenCL;
}


void CBitmapWnd::OnMoveLeft(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	this->Refresh();
}

void CBitmapWnd::OnMoveRight(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	this->Refresh();
}

void CBitmapWnd::OnMoveTop(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	this->Refresh();
}

void CBitmapWnd::OnMoveBottom(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	this->Refresh();
}

void CBitmapWnd::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
	this->Refresh();
}

void CBitmapWnd::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
	this->Refresh();
}

void CBitmapWnd::OnUpdateBitmap(wxCommandEvent& event)
{
	CImageLoadingFormat * picture = (CImageLoadingFormat *)event.GetClientData();
	if (picture != nullptr)
		UpdateBitmap(picture, false);
}


void CBitmapWnd::SetFullscreen(const bool &fullscreen)
{
    TRACE();
	if (fullscreen)
		themeBitmap.colorBack = themeBitmap.colorFullscreen;
	else
		themeBitmap.colorBack = themeBitmap.colorScreen;
}

COpenCLContext * CBitmapWnd::GetOpenCLContext()
{
	return openclContext;
}

CFiltreEffet * CBitmapWnd::GetFiltreEffet()
{
	return filtreEffet;
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

#ifdef RENDEROPENGL

	if (renderOpenGL)
		renderOpenGL->SetCurrent(*this);

	if(renderOpenGL != nullptr)
		delete renderOpenGL;

	if (openCLEngine != nullptr)
		delete openCLEngine;
	openCLEngine = nullptr;
#endif
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
	float screenWidth = float(GetWidth());
	float screenHeight = float(GetHeight());
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
	float screenWidth = float(GetWidth());
	float screenHeight = float(GetHeight());

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
		posLargeur = std::clamp(0, int(screenXPos - middleScreenWidth), posLargeurMax);
	}

	if (screenHeight > bitmapRatioHeight)
	{
		posHauteur = 0;
	}
	else
	{
		posHauteur = std::clamp(0, int(screenYPos - middleScreenHeight), posHauteurMax);
	}
}

int CBitmapWnd::GetHPos()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif    
    
	return posLargeur * scale_factor;
}

int CBitmapWnd::GetVPos()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif    
    
	return posHauteur * scale_factor;
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

	UpdateScrollBar();
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


	UpdateScrollBar();

}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWnd::UpdateScrollBar()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		CControlSize * controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = int(GetBitmapWidthWithRatio());
		controlSize->controlHeight = int(GetBitmapHeightWithRatio());
		evt.SetClientData(controlSize);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	if (parent != nullptr)
	{
		wxSize * size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
	this->Refresh();
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
    
	UpdateScrollBar();

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

	UpdateScrollBar();
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

wxString CBitmapWnd::GetFilename()
{
	return filename;
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

void CBitmapWnd::UpdateBitmap(CImageLoadingFormat* bitmapIn, const bool& updateAll)
{
	TRACE();
	printf("CBitmapWnd::SetBitmap \n");
	//this->SetFocus();
	if (bitmapIn != nullptr)
	{
		if (bitmapIn->IsOk())
		{
			if (updateAll)
			{
				loadBitmap = true;
				bitmapLoad = true;
				bitmapUpdate = true;
				flipVertical = 0;
				flipHorizontal = 0;
				angle = 0;

				printf("CBitmapWnd::SetBitmap  muBitmap.lock()\n");
				muBitmap.lock();
				if (source != nullptr)
				{
					delete source;
					printf("CBitmapWnd::SetBitmap   delete source\n");
					source = nullptr;
				}

				source = bitmapIn;
				//source = nullptr;
				muBitmap.unlock();
				printf("CBitmapWnd::SetBitmap  muBitmap.unlock()\n");
				bitmapwidth = bitmapIn->GetWidth();
				bitmapheight = bitmapIn->GetHeight();
				orientation = bitmapIn->GetOrientation();
				ShrinkImage(false);
			}
			else
			{

				loadBitmap = true;
				bitmapLoad = true;
				filename = bitmapIn->GetFilename();
				bitmapUpdate = true;

				printf("CBitmapWnd::SetBitmap  muBitmap.lock()\n");
				muBitmap.lock();
				if (source != nullptr)
				{
					delete source;
					printf("CBitmapWnd::SetBitmap   delete source\n");
					source = nullptr;
				}

				source = bitmapIn;
				muBitmap.unlock();
			}
			RefreshWindow();

		}
	}
}

void CBitmapWnd::ReinitPicture()
{
	TRACE();
	printf("CBitmapWnd::SetBitmap \n");

	bitmapLoad = true;
	bitmapUpdate = true;
	flipVertical = 0;
	flipHorizontal = 0;
	angle = 0;

	printf("CBitmapWnd::SetBitmap  muBitmap.lock()\n");

	printf("CBitmapWnd::SetBitmap  muBitmap.unlock()\n");
	toolOption = MOVEPICTURE;
	ShrinkImage(false);
	AfterSetBitmap();
	RefreshWindow();

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
#if defined(WIN32) && defined(_DEBUG)
			OutputDebugString(L"CBitmapWnd::SetBitmap");
			OutputDebugString(L"\n");
			OutputDebugString(bitmapIn->GetFilename());
			OutputDebugString(L"\n");
#endif

			loadBitmap = true;
			bitmapLoad = true;
			filename = bitmapIn->GetFilename();
			bitmapUpdate = true;
			flipVertical = 0;
			flipHorizontal = 0;
			angle = 0;
            
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

			RemoveListener();

			this->Refresh();
			this->Update();

            //RefreshWindow();
          
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
	if(filtreEffet != nullptr && bitmapLoad && this->source != nullptr)
	{
		CRegardsBitmap * bitmap = this->source->GetRegardsBitmap(source);
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
	if(filtreEffet != nullptr && bitmapLoad && this->source != nullptr)
	{
		CRegardsFloatBitmap * bitmap = this->source->GetFloatBitmap(source);
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

void CBitmapWnd::SetOrientation(const int &exif)
{
	orientation = exif;
	ShrinkImage(true);
	//updateFilter = true;
	//RefreshWindow();
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
	CSqlPhotos sqlPhotos;
	int exif = sqlPhotos.GetPhotoExif(filename);
	if (exif != -1)
	{
		exif = GetExifOrientation(angle);
		sqlPhotos.UpdatePhotoExif(filename, exif);
		int numPhoto = sqlPhotos.GetPhotoId(filename);
		wxWindow* window = this->FindWindowById(idWindowMain);
		if (window != nullptr)
		{
			//wxString thumbnail = CFileUtility::GetThumbnailPath(to_string(numPhoto));

			wxString* _filename = new wxString(filename);
			wxCommandEvent evt(wxEVENT_UPDATETHUMBNAILEXIF);
			evt.SetInt(numPhoto);
			evt.SetExtraLong(90);
			evt.SetClientData(_filename);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	
    UpdateResized();
    RefreshWindow();
}

void CBitmapWnd::Rotate270()
{
    TRACE();
	angle += 270;
	angle = angle % 360;
    updateFilter = true;

	CSqlPhotos sqlPhotos;
	int exif = sqlPhotos.GetPhotoExif(filename);
	if (exif != -1)
	{
		exif = GetExifOrientation(angle);
		sqlPhotos.UpdatePhotoExif(filename, exif);
		int id = sqlPhotos.GetPhotoId(filename);
		wxWindow* window = this->FindWindowById(idWindowMain);
		if (window != nullptr)
		{
			wxString* _filename = new wxString(filename);
			wxCommandEvent evt(wxEVENT_UPDATETHUMBNAILEXIF);
			evt.SetInt(id);
			evt.SetExtraLong(270);
			evt.SetClientData(_filename);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}

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
	//wxSetCursor(wxCursor(wxCURSOR_ARROW));
	::wxSetCursor(*wxSTANDARD_CURSOR);

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
		wxCommandEvent evt(TOOLBAR_UPDATE_ID);
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

#ifndef WIN32
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

		default:
			::wxSetCursor(*wxSTANDARD_CURSOR);
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
	switch (event.GetKeyCode())
	{
	case WXK_CONTROL:
		controlKeyPush = false;
		break;
	}
}

void CBitmapWnd::MoveTop()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWnd::MoveLeft()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWnd::MoveBottom()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWnd::MoveRight()
{
	wxWindow * parent = this->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
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

	case WXK_CONTROL:
		controlKeyPush = true;
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
	int move = 0;
#ifdef __APPLE__
    
	if (event.m_wheelRotation == 1)
		move = 0;
	else if (event.m_wheelRotation == -1)
		move = 1;
    
#else
    
	if (event.m_wheelRotation == 120)
		move = 0;
	else
		move = 1;
    
#endif

	if (controlKeyPush)
		move += 10;

	switch (move)
	{
	case 0:
		this->MoveTop();
		break;
	case 1:
		this->MoveBottom();
		break;
	case 10:
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMIN);
            this->GetParent()->GetEventHandler()->AddPendingEvent(evt);
            this->ZoomOn();
        }
		break;
	case 11:
        {
            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMOUT);
            this->GetParent()->GetEventHandler()->AddPendingEvent(evt);    
            this->ZoomOut();
        }
		break;
	}
}

void CBitmapWnd::Resize()
{
	updateFilter = true;
	UpdateResized();
	this->Refresh();
}

int CBitmapWnd::UpdateResized()
{
	oldWidth = 0;
	oldHeight = 0;

    TRACE();
    if (bitmapLoad)
    {
	    if (shrinkImage)
        {
            ShrinkImage();
            //ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());
            //RefreshWindow();
        }
        else if (showScroll)
        {
            CalculPositionPicture(centerX, centerY);
            UpdateScrollBar();
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
#ifndef WIN32
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

					UpdateScrollBar();

#if defined(WIN32) && defined(_DEBUG)
					TCHAR message[200];
					wsprintf(message, L"posLargeur : %d et posHauteur : %d \n",posLargeur,posHauteur);
					OutputDebugString(message);
#endif

                    updateFilter = true;

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

		default:
		{
			::wxSetCursor(*wxSTANDARD_CURSOR);
			break;
		}

	}

	MouseMove(xPos* scale_factor, yPos* scale_factor);
    
    this->Refresh();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWnd::TestMaxX()
{
    TRACE();
	int xValue = int(GetBitmapWidthWithRatio()) - GetWidth();

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
	int yValue = int(GetBitmapHeightWithRatio()) - GetHeight();

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
		newRatio = (float)GetWidth() / (float)(pictureWidth);
	else
		newRatio = (float)GetHeight() / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > GetHeight())
	{
		newRatio = (float)GetHeight() / (float)(pictureHeight);
	}
	if ((pictureWidth * newRatio) > GetWidth())
	{
		newRatio = (float)GetWidth() / (float)(pictureWidth);
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
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 
   
	int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = int(GetBitmapHeightWithRatio()) * scale_factor;
	int xValue = 0;
	int yValue = 0;


	if (widthOutput > GetWidth()* scale_factor)
	{
		left = 0;
		xValue = posLargeur * scale_factor;
	}
	else
	{
		xValue = 0;
		left = (GetWidth()* scale_factor - widthOutput) / 2;
	}

	widthInterpolationSize = GetWidth()* scale_factor - (left * 2);


	if (heightOutput > GetHeight()* scale_factor)
	{
		top = 0;
		yValue = posHauteur* scale_factor;
	}
	else
	{
		yValue = 0;
		top = (GetHeight()* scale_factor - heightOutput) / 2;
	}

	heightInterpolationSize = GetHeight()* scale_factor - (top * 2);

	rc.x = max(xValue,0);
	if (invert)
    {
        int heightmax = heightOutput - (GetHeight() * scale_factor) - yValue;
        rc.y = max(heightmax, 0);
    }
	else
		rc.y = max(yValue,0);
	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CBitmapWnd::UpdateScreenRatio()
{
    TRACE();
    Resize();
}

void CBitmapWnd::GenerateExifPosition(int & localAngle, int & localflipHorizontal, int & localflipVertical)
{
    TRACE();
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
}

int CBitmapWnd::GetExifOrientation(const int& angle)
{
	if (angle > 44 && angle < 135) //Rotate 90
		return 8;
	else if (angle > 134 && angle < 225) //Rotate 180
		return 3;
	else if (angle > 224 && angle < 315)
		return 6;
	return 0;
}

void CBitmapWnd::GenerateScreenBitmap(CFiltreEffet * filtreEffet, int &widthOutput, int &heightOutput)
{
    TRACE();
    
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	int filterInterpolation = 0;



	if(regardsParam != nullptr)
		filterInterpolation = regardsParam->GetInterpolationType();
	//if(raw)
	//	GenerateExifRawPosition(localAngle, localflipHorizontal, localflipVertical);
	//else
	GenerateExifPosition(localAngle, localflipHorizontal, localflipVertical);

	if (GetWidth() * scale_factor >= widthOutput && GetHeight() * scale_factor >= heightOutput)
	{
		filtreEffet->Interpolation(widthOutput, heightOutput, filterInterpolation, localflipHorizontal, localflipVertical, localAngle);
	}
	else
	{
		int left = 0, top = 0;
		int tailleAffichageWidth = widthOutput;
		int tailleAffichageHeight = heightOutput;

		if (GetWidth() * scale_factor > tailleAffichageWidth)
			left = ((GetWidth() * scale_factor - tailleAffichageWidth) / 2);
		else
			left = 0;

		if (GetHeight() * scale_factor > tailleAffichageHeight)
			top = ((GetHeight() * scale_factor - tailleAffichageHeight) / 2);
		else
			top = 0;

		wxRect rc(0, 0, 0, 0);
		CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, true);
		filtreEffet->Interpolation(widthOutput, heightOutput, rc, filterInterpolation, localflipHorizontal, localflipVertical, localAngle);
	}


	if (regardsParam != nullptr)
	{
		if (regardsParam->GetAutoConstrast() && preview == 0)
		{
			filtreEffet->SetPreviewMode(true);
			filtreEffet->BrightnessAndContrastAuto(1);
		}
	}
}

int CBitmapWnd::GetWidth()
{

	return GetWindowWidth();
}

int CBitmapWnd::GetHeight()
{
	return GetWindowHeight();
}

#ifdef RENDEROPENGL

void CBitmapWnd::RenderToScreenWithOpenCLSupport()
{
	CRgbaquad color;

#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 

	int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = int(GetBitmapHeightWithRatio()) * scale_factor;  

	muBitmap.lock();

	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
		if (filtreEffet != nullptr)
			delete filtreEffet;
		filtreEffet = new CFiltreEffet(color, openclContext, source);
		loadBitmap = false;
	}

	//if (filtreEffet == nullptr)
	//	filtreEffet = new CFiltreEffet(color, openclContext, source);

	muBitmap.unlock();
    
     printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);

	//UpdateScrollBar();

	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
		if (widthOutput < 0 || heightOutput < 0)
			return;          

		GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

		ApplyPreviewEffect(widthOutput, heightOutput);
        
        printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);

        bool textureBinging = false;
        
		if (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL)
		{
			printf("CBitmapWnd IsSharedContextCompatible \n");

			try
			{
				glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, openclContext->GetContext());
			}
			catch (...)
			{
			}

			try
			{
				cl_int err;
				cl_mem cl_image = renderOpenGL->GetOpenCLTexturePt();
                if(cl_image != nullptr)
                {
                    err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
                    Error::CheckError(err);
                    filtreEffet->GetRgbaBitmap(cl_image);
                    err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_image, 0, 0, 0);
                    Error::CheckError(err);
                    err = clFlush(openclContext->GetCommandQueue());
                    Error::CheckError(err);
                    
                    textureBinging = true;
                }
			}
			catch (...)
			{

			}
		}
		
        if(!textureBinging)
		{
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

	renderOpenGL->CreateScreenRender(GetWidth()* scale_factor, GetHeight()* scale_factor, CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue()));

	RenderTexture(openclContext->IsSharedContextCompatible());	
}

void CBitmapWnd::RenderToScreenWithoutOpenCLSupport()
{
    printf("CBitmapWnd RenderToScreenWithoutOpenCLSupport \n");
    
	CRgbaquad color;

#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 

	int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = int(GetBitmapHeightWithRatio()) * scale_factor;

	
	if (loadBitmap)
	{
		if (filtreEffet != nullptr)
			delete filtreEffet;
		filtreEffet = new CFiltreEffet(color, nullptr, source);
		//if (filtreEffet == nullptr)
		//	filtreEffet = new CFiltreEffet(color, nullptr, source);
        
        if(IsOpenGLDecoding())
        {
            if (glTextureSrc != nullptr)
                delete glTextureSrc;

            CRegardsBitmap * bitmap = filtreEffet->GetPtBitmap();

            glTextureSrc = new GLTexture();
            glTextureSrc->Create(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());

        }
		loadBitmap = false;
	}
	

	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
        renderOpenGL->CreateScreenRender(GetWidth() * scale_factor, GetHeight() * scale_factor, CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue()));

		if(IsOpenGLDecoding())
        {
            printf("CBitmapWnd RenderToScreenWithoutOpenCLSupport openGLRenderBitmap \n");
            
            int left = 0, top = 0;
            int tailleAffichageWidth = widthOutput;
            int tailleAffichageHeight = heightOutput;
            int filterInterpolation = 0;
            if (GetWidth() * scale_factor > tailleAffichageWidth)
                left = ((GetWidth() * scale_factor - tailleAffichageWidth) / 2);
            else
                left = 0;

            if (GetHeight() * scale_factor > tailleAffichageHeight)
                top = ((GetHeight() * scale_factor - tailleAffichageHeight) / 2);
            else
                top = 0;

            wxRect rc(0, 0, 0, 0);
            CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, true);
            GLTexture * glTextureOutput = new GLTexture(widthOutput, heightOutput);

            CRegardsConfigParam * regardsParam = CParamInit::getInstance();
            if (regardsParam != nullptr)
                filterInterpolation = regardsParam->GetInterpolationType();

            renderOpenGL->RenderInterpolation(glTextureSrc, glTextureOutput, rc, flipHorizontal, flipVertical, angle, filterInterpolation);
            
            renderOpenGL->RenderToTexture();

            if (!ApplyPreviewEffect(widthOutput, heightOutput))
            {
                CRegardsBitmap* bitmap = nullptr;
                bitmap = filtreEffet->GetBitmap(false);
                glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, nullptr);
                if (glTexture != nullptr)
                    glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
                else
                    printf("CBitmapWnd GetDisplayTexture Error \n");
                delete bitmap;
            }  
            else
                glTexture = renderOpenGL->GetDisplayTexture();
        }
        else
        {
            printf("CBitmapWnd RenderToScreenWithoutOpenCLSupport without OpenGL \n");
            
            GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

            ApplyPreviewEffect(widthOutput, heightOutput);
            
            printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);

            CRegardsBitmap* bitmap = nullptr;
            bitmap = filtreEffet->GetBitmap(false);

            glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, nullptr);
            if (glTexture != nullptr)
                glTexture->SetData(bitmap->GetPtBitmap(), bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
            else
                printf("CBitmapWnd GetDisplayTexture Error \n");
            delete bitmap;
        }
	}
	RenderTexture(false);
}

void CBitmapWnd::RenderTexture(const bool &invertPos)
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 

	if (glTexture != nullptr)
	{
		int x = (GetWidth() * scale_factor - glTexture->GetWidth()) / 2;
		int y = (GetHeight() * scale_factor - glTexture->GetHeight()) / 2;

		renderOpenGL->RenderToScreen(mouseUpdate, effectParameter, x, y, invertPos);

		xPosImage = x;
		yPosImage = y;
	}

}

int CBitmapWnd::IsOpenGLDecoding()
{
  	int supportOpenGL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenGL = config->GetVideoLibrary();

	return supportOpenGL;  
}

//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd::OnPaint(wxPaintEvent& event)
{
	TRACE();
#if defined(WIN32) && defined(_DEBUG)
	OutputDebugString(L"CBitmapWnd::OnPaint");
	OutputDebugString(L"\n");
#endif

	//bool isPageCurl = false;

	wxPaintDC(this);

	if (GetWidth() == 0 || GetHeight() == 0)
		return;



#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"OnPaint\n");
#endif

	if (renderOpenGL == nullptr)
	{
		renderOpenGL = new CRenderBitmapOpenGL(this);

		//Now we have a context, retrieve pointers to OGL functions
		renderOpenGL->Init(this);

#ifndef WIN32
		double scale_factor = GetContentScaleFactor();
#else
		double scale_factor = 1.0f;
#endif 

		renderOpenGL->LoadingResource(scale_factor);
	}

	if (IsSupportOpenCL())
	{
		if (openCLEngine == nullptr)
		{
			openCLEngine = new COpenCLEngine(true);
			if (openCLEngine != nullptr)
				openclContext = openCLEngine->GetInstance();
		}

	}
	else
		openclContext = nullptr;

	if (renderOpenGL != nullptr)
	{
		renderOpenGL->SetCurrent(*this);

		if (!IsSupportOpenCL())
        {
            printf("CBitmapWnd OnPaint RenderToScreenWithoutOpenCLSupport\n");
            RenderToScreenWithoutOpenCLSupport();
        }
		else
        {
            printf("CBitmapWnd OnPaint RenderToScreenWithOpenCLSupport \n");
            RenderToScreenWithOpenCLSupport();
        }
			

		AfterRender();
	}

    this->SwapBuffers();

	printf("CBitmapWnd End OnPaint \n");

#if defined(WIN32) && defined(_DEBUG)
	DWORD LasttickCount = GetTickCount();				// Get The Tick Count
	DWORD Result = LasttickCount - tickCount;

	wchar_t Temp[10];
	swprintf_s(Temp, L"%d", Result);
	OutputDebugString(L"Render Time : ");
	OutputDebugString(Temp);
	OutputDebugString(L"\n");
#endif

#ifndef __APPLE__
	
    if(openclContext != nullptr)
    {
       if (!openclContext->IsSharedContextCompatible())
       {
            DeleteTexture();
           
            if (renderOpenGL != nullptr)
            {
                delete renderOpenGL;
                renderOpenGL = nullptr;
            }  
       }   
    }
	
#endif

	oldWidth = GetWidth();
	oldHeight = GetHeight();
}

#else

void CBitmapWnd::RenderToScreenWithOpenCLSupport(wxDC * dc)
{
	CRgbaquad color;

#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 

	if (width == 0 || height == 0)
		return;

	int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = int(GetBitmapHeightWithRatio())* scale_factor;


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

	wxImage picture;

	if (bitmapLoad && width > 0 && height > 0)
	{
		int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
		int heightOutput = int(GetBitmapHeightWithRatio()) * scale_factor;

		if (widthOutput < 0 || heightOutput < 0)
			return;

		GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

		ApplyPreviewEffect(widthOutput, heightOutput);

		picture = filtreEffet->GetwxImage();

	}

	if (picture.IsOk())
	{
		int x = (width - picture.GetWidth()) / 2;
		int y = (height - picture.GetHeight()) / 2;
		dc->DrawBitmap(picture, x, y);
		xPosImage = x;
		yPosImage = y;
	}

}

void CBitmapWnd::RenderToScreenWithoutOpenCLSupport(wxDC * dc)
{
	CRgbaquad color;

#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 

	if (width == 0 || height == 0)
		return;

	int widthOutput = int(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = int(GetBitmapHeightWithRatio())* scale_factor;

	wxImage picture;

	if (source != nullptr && bitmapLoad)
	{
		//CRegardsBitmap* bitmapSpecial = nullptr;

		if (filtreEffet != nullptr)
			delete filtreEffet;

		filtreEffet = new CFiltreEffet(color, nullptr, source);

		GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

		ApplyPreviewEffect(widthOutput, heightOutput);

		picture = filtreEffet->GetwxImage();
	}

	if (picture.IsOk())
	{
		int x = (width - picture.GetWidth()) / 2;
		int y = (height - picture.GetHeight()) / 2;
		dc->DrawBitmap(picture, x, y);
		xPosImage = x;
		yPosImage = y;
	}
}

//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWnd::OnPaint(wxPaintEvent& event)
{
	TRACE();
	wxPaintDC dc(this);
	wxBitmap localmemBitmap(width,height);
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = width;
	rc.height = height;
	wxMemoryDC memDC(localmemBitmap);


#if defined(WIN32) && defined(_DEBUG)
	DWORD tickCount = GetTickCount();
	OutputDebugString(L"OnPaint\n");
#endif

	printf("CBitmapWnd OnPaint \n");

	FillRect(&memDC, rc, themeBitmap.colorBack);

	if (!IsSupportOpenCL())
		RenderToScreenWithoutOpenCLSupport(&memDC);
	else
		RenderToScreenWithOpenCLSupport(&memDC);

	AfterRender(&memDC);

	memDC.SelectObject(wxNullBitmap);

	dc.DrawBitmap(localmemBitmap, 0, 0);

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

#endif

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
