// ReSharper disable All
#include "header.h"
#include "BitmapWndRender.h"
#include <RGBAQuad.h>
#include <FiltreEffet.h>
#include "ClosedHandCursor.h"
#include "ScrollbarWnd.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <wx/dcbuffer.h>
#include <chrono>
#include <MetadataExiv2.h>
#include <window_id.h>
#include <config_id.h>
#include <SqlPhotos.h>
#include <OpenCLEffect.h>
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#endif
#include <ImageLoadingFormat.h>
#include "RenderBitmapOpenGL.h"
#include <utility.h>
#include <WindowUtility.h>
#include <picture_utility.h>
using namespace Regards::Sqlite;
using namespace Regards::FiltreEffet;
using namespace Regards::Window;
using namespace Regards::exiv2;
using namespace Regards::OpenCL;
using namespace std::chrono;
extern bool processrecognitionison;


#define TIMER_RESIZE 1
#define TIMER_LOADING 4

extern float clamp(float val, float minval, float maxval);

std::vector<int> _value = {
	1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200,
	1600
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

CBitmapWndRender::CBitmapWndRender(CSliderInterface* slider, wxWindowID idMain, const CThemeBitmapWindow& theme)
{
	glTexture = nullptr;
	renderBitmapOpenGL = nullptr;
	idWindowMain = idMain;
	//bitmap = nullptr;
	sliderInterface = nullptr;
	config = nullptr;
	//copyBmpSrc = nullptr;
	filtreEffet = nullptr;
	flipVertical = 0;
	flipHorizontal = 0;
	angle = 0;
    start = std::chrono::system_clock::now();
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
	this->value = _value;
	bitmapwidth = 0;
	bitmapheight = 0;
	bitmapUpdate = false;
	source = nullptr;
	bitmapLoad = false;
	themeBitmap.colorBack = themeBitmap.colorScreen;
	parentRender = nullptr;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
CImageLoadingFormat* CBitmapWndRender::GetBitmap(const bool& source)
{
	//
	if (filtreEffet != nullptr && bitmapLoad && this->source != nullptr)
	{
		CImageLoadingFormat* bitmap = new CImageLoadingFormat();
		cv::Mat mat = this->source->GetOpenCVPicture();
		bitmap->SetPicture(mat);
		bitmap->SetOrientation(orientation);
		bitmap->SetFilename(this->filename);
		return bitmap;
	}
	return nullptr;
}

void CBitmapWndRender::SetOpenCLOpenGLInterop(const bool& openclOpenGLInterop)
{
	isOpenCLOpenGLInterop = openclOpenGLInterop;
}

void CBitmapWndRender::SetTabValue(const std::vector<int>& value)
{
	this->value = value;
}

CRgbaquad CBitmapWndRender::GetBackColor()
{
	return CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
};

void CBitmapWndRender::SetEndProgram(const bool& endProgram)
{
	this->endProgram = endProgram;
}

vector<int> CBitmapWndRender::GetListCommand()
{
	vector<int> list;
	list.push_back(wxEVENT_UPDATEBITMAP); // , wxCommandEventHandler(CBitmapWnd::OnUpdateBitmap));
	list.push_back(wxEVENT_LEFTPOSITION); //, wxCommandEventHandler(CBitmapWnd::OnLeftPosition));
	list.push_back(wxEVENT_TOPPOSITION); //, wxCommandEventHandler(CBitmapWnd::OnTopPosition));
	list.push_back(wxEVENT_MOVELEFT); //, wxCommandEventHandler(CBitmapWnd::OnMoveLeft));
	list.push_back(wxEVENT_MOVERIGHT); //, wxCommandEventHandler(CBitmapWnd::OnMoveRight));
	list.push_back(wxEVENT_MOVETOP); //, wxCommandEventHandler(CBitmapWnd::OnMoveTop));
	list.push_back(wxEVENT_MOVEBOTTOM); //, wxCommandEventHandler(CBitmapWnd::OnMoveBottom));
	list.push_back(wxEVENT_SCROLLMOVE); //, wxCommandEventHandler(CBitmapWnd::OnScrollMove));
	return list;
};

void CBitmapWndRender::OnCommand(wxCommandEvent& event)
{
	switch (event.GetEventType())
	{
	case wxEVENT_UPDATEBITMAP:
		OnUpdateBitmap(event);
		break;
	case wxEVENT_LEFTPOSITION:
		OnLeftPosition(event);
		needToRefresh = true;
		break;
	case wxEVENT_TOPPOSITION:
		OnTopPosition(event);
		needToRefresh = true;
		break;
	case wxEVENT_MOVELEFT:
		OnMoveLeft(event);
		needToRefresh = true;
		break;
	case wxEVENT_MOVERIGHT:
		OnMoveRight(event);
		needToRefresh = true;
		break;
	case wxEVENT_MOVETOP:
		OnMoveTop(event);
		needToRefresh = true;
		break;
	case wxEVENT_MOVEBOTTOM:
		OnMoveBottom(event);
		needToRefresh = true;
		break;
	case wxEVENT_SCROLLMOVE:
		OnScrollMove(event);
		break;
	}

	
}


void CBitmapWndRender::SetParent(wxWindow* parent)
{
	parentRender = parent;
}

int CBitmapWndRender::GetWidth()
{
	return parentRender->GetSize().GetWidth();
}

int CBitmapWndRender::GetHeight()
{
	return parentRender->GetSize().GetHeight();
}

void CBitmapWndRender::OnScrollMove(wxCommandEvent& event)
{
	isMoving = event.GetInt();
}

int CBitmapWndRender::IsSupportCuda()
{
	int supportCuda = 0;
	int useCuda = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
	{
		supportCuda = config->GetIsCudaSupport();
		useCuda = config->GetIsUseCuda();
	}
	return (supportCuda && useCuda) ? 1 : 0;
}

int CBitmapWndRender::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	if (!cv::ocl::haveOpenCL())
		supportOpenCL = 0;
	return supportOpenCL;
}


void CBitmapWndRender::OnMoveLeft(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
}

void CBitmapWndRender::OnMoveRight(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
}

void CBitmapWndRender::OnMoveTop(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
}

void CBitmapWndRender::OnMoveBottom(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
}

void CBitmapWndRender::OnLeftPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posLargeur = pos;
}

void CBitmapWndRender::OnTopPosition(wxCommandEvent& event)
{
	int pos = event.GetInt();
	posHauteur = pos;
}

void CBitmapWndRender::OnUpdateBitmap(wxCommandEvent& event)
{
	auto picture = static_cast<CImageLoadingFormat*>(event.GetClientData());
	if (picture != nullptr)
		UpdateBitmap(picture, false);
}


void CBitmapWndRender::SetFullscreen(const bool& fullscreen)
{
	//
	if (fullscreen)
		themeBitmap.colorBack = themeBitmap.colorFullscreen;
	else
		themeBitmap.colorBack = themeBitmap.colorScreen;
}

CFiltreEffet* CBitmapWndRender::GetFiltreEffet()
{
	return filtreEffet;
}


float CBitmapWndRender::GetRatio()
{
	//
	return ratio;
}

int CBitmapWndRender::GetPosRatio()
{
	//
	return posRatio;
}

void CBitmapWndRender::SetRatioPos(const int& pos)
{
	//
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio = pos;

	if (posRatio >= value.size())
	{
		posRatio = value.size() - 1;
	}
	ratio = ((float)value[posRatio] / 100.0f);

	CalculPositionPicture(centerX, centerY);
	RefreshWindow();
}

//-----------------------------------------------------------------
//Destructeur
//-----------------------------------------------------------------
CBitmapWndRender::~CBitmapWndRender(void)
{
	//
	if (filtreEffet != nullptr)
		delete filtreEffet;
	filtreEffet = nullptr;

	if (renderBitmapOpenGL != nullptr)
		delete renderBitmapOpenGL;

	if (source != nullptr)
		delete source;
}

void CBitmapWndRender::SetKey(const int& iKey)
{
	//
	this->iKey = iKey;
	switch (toolOption)
	{
	case MOVEPICTURE:
		{
			if (iKey == WXK_CONTROL)
				wxSetCursor(hCursorZoomIn);
			else
				wxSetCursor(hCursorZoomOut);
			break;
		}
	}
}

int CBitmapWndRender::GetKey()
{
	//
	return iKey;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CBitmapWndRender::SetTool(const int& tool)
{
	//
	toolOption = tool;

	switch (tool)
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
int CBitmapWndRender::GetTool()
{
	//
	return toolOption;
}


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void CBitmapWndRender::SetZoom(bool active)
{
	//
	zoom = active;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CBitmapWndRender::GetZoom()
{
	//
	return zoom;
}

void CBitmapWndRender::CalculCenterPicture()
{
	//
	float bitmapRatioWidth = GetBitmapWidthWithRatio();
	float bitmapRatioHeight = GetBitmapHeightWithRatio();
	float screenWidth = static_cast<float>(GetWidth());
	float screenHeight = static_cast<float>(GetHeight());
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

void CBitmapWndRender::CalculPositionPicture(const float& x, const float& y)
{
	//
	float bitmapRatioWidth = GetBitmapWidthWithRatio();
	float bitmapRatioHeight = GetBitmapHeightWithRatio();
	float screenWidth = static_cast<float>(GetWidth());
	float screenHeight = static_cast<float>(GetHeight());

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
		posLargeur = clamp(0, static_cast<int>(screenXPos - middleScreenWidth), posLargeurMax);
	}

	if (screenHeight > bitmapRatioHeight)
	{
		posHauteur = 0;
	}
	else
	{
		posHauteur = clamp(0, static_cast<int>(screenYPos - middleScreenHeight), posHauteurMax);
	}
}

int CBitmapWndRender::GetHPos()
{
	return posLargeur * scale_factor;
}

int CBitmapWndRender::GetVPos()
{
	return posHauteur * scale_factor;
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWndRender::ZoomOn()
{
	//
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio++;
	if (posRatio >= value.size())
	{
		posRatio = value.size() - 1;
	}
	ratio = ((float)value[posRatio] / 100.0f);

	CalculPositionPicture(centerX, centerY);

	//updateFilter = true;

	UpdateScrollBar();

	//needToRefresh = true;
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWndRender::SetZoomPosition(const int& position)
{
	//
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio = position;
	if (posRatio >= value.size())
	{
		posRatio = value.size() - 1;
	}
	ratio = ((float)value[posRatio] / 100.0f);

	CalculPositionPicture(centerX, centerY);

	UpdateScrollBar();

	needToRefresh = true;
}

//-----------------------------------------------------------------
//Zoom +
//-----------------------------------------------------------------
void CBitmapWndRender::UpdateScrollBar()
{
	if (parentRender != nullptr)
	{
		auto controlSize = new CControlSize();
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		controlSize->controlWidth = static_cast<int>(GetBitmapWidthWithRatio());
		controlSize->controlHeight = static_cast<int>(GetBitmapHeightWithRatio());
		evt.SetClientData(controlSize);
		parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}

	if (parentRender != nullptr)
	{
		auto size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}
}


//-----------------------------------------------------------------
//Zoom -
//-----------------------------------------------------------------
void CBitmapWndRender::ZoomOut()
{
	//
	CalculCenterPicture();

	if (posRatio == -1)
		ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	shrinkImage = false;

	posRatio--;
	if (posRatio < 0)
		posRatio = 0;

	ratio = ((float)value[posRatio] / 100.0f);

	CalculPositionPicture(centerX, centerY);

	//updateFilter = true;

	UpdateScrollBar();

	//needToRefresh = true;
}

//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWndRender::ShrinkImage(const bool& redraw)
{
	//
	shrinkImage = true;
	ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());

	//Calcul position largeur et hauteur initial
	posLargeur = 0;
	posHauteur = 0;
	centerX = static_cast<float>(GetBitmapWidth()) / 2.0f;
	centerY = static_cast<float>(GetBitmapHeight()) / 2.0f;

	//updateFilter = true;

	UpdateScrollBar();

	sliderInterface->SetTrackBarPosition(posRatio);

	needToRefresh = true;
}


//-----------------------------------------------------------------
//Mise à l'échelle
//-----------------------------------------------------------------
void CBitmapWndRender::SetShrinkImage(bool active)
{
	//
	if (shrinkImage != active)
	{
		shrinkImage = active;
	}

	needToRefresh = true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CBitmapWndRender::GetShrinkImage()
{
	//
	return shrinkImage;
}

//-----------------------------------------------------------------
//Affichage de la barre de scrolling
//-----------------------------------------------------------------
void CBitmapWndRender::SetShowScroll(bool visible)
{
	//
	showScroll = visible;
}

//-----------------------------------------------------------------
//Définition de la méthode d'interpolation
//-----------------------------------------------------------------
bool CBitmapWndRender::GetShowScroll()
{
	//
	return showScroll;
}

wxString CBitmapWndRender::GetFilename()
{
	return filename;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWndRender::GetBitmapWidthWithRatio()
{
	//
	return static_cast<float>(GetBitmapWidth()) * ratio;
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
float CBitmapWndRender::GetBitmapHeightWithRatio()
{
	//
	return static_cast<float>(GetBitmapHeight()) * ratio;
}


void CBitmapWndRender::SetBitmapParameter(const bool& externBitmap, const bool& addToTexture)
{
	//
	this->externBitmap = externBitmap;
	//if (addToTexture)
	//	render.Destroy();
	//this->addToTexture = addToTexture;
}

void CBitmapWndRender::SetIsBitmapThumbnail(const bool& isThumbnail)
{
	//
	this->isThumbnail = isThumbnail;
}

void CBitmapWndRender::UpdateBitmap(CImageLoadingFormat* bitmapIn, const bool& updateAll)
{
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
				muBitmap.lock();
				if (source != nullptr)
				{
					delete source;
					source = nullptr;
				}

				source = bitmapIn;
				muBitmap.unlock();
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
				bitmapwidth = bitmapIn->GetWidth();
				bitmapheight = bitmapIn->GetHeight();
				bitmapUpdate = true;
				
				muBitmap.lock();
				if (source != nullptr)
				{
					delete source;
					source = nullptr;
				}

				source = bitmapIn;
				muBitmap.unlock();

				if (shrinkImage)
					ShrinkImage(false);
			}
			//RefreshWindow();
			updateFilter = true;
			parentRender->Refresh();
		}
	}
}

void CBitmapWndRender::ReinitPicture()
{
	//
	//printf("CBitmapWndRender::SetBitmap \n");

	bitmapLoad = true;
	bitmapUpdate = true;
	flipVertical = 0;
	flipHorizontal = 0;
	angle = 0;

	//printf("CBitmapWndRender::SetBitmap  muBitmap.lock()\n");

	//printf("CBitmapWndRender::SetBitmap  muBitmap.unlock()\n");
	toolOption = MOVEPICTURE;
	ShrinkImage(false);
	AfterSetBitmap();
	RefreshWindow();
}

//-----------------------------------------------------------------
//Affectation de l'image
//-----------------------------------------------------------------
void CBitmapWndRender::SetBitmap(CImageLoadingFormat* bitmapIn, const bool& copy)
{
	//
	//printf("CBitmapWndRender::SetBitmap \n");
	//if (copyBmpSrc != nullptr)
	//	delete copyBmpSrc;
	//copyBmpSrc = nullptr;
	//this->SetFocus();
	if (bitmapIn != nullptr)
	{
		if (bitmapIn->IsOk())
		{
#if defined(WIN32) && defined(_DEBUG)
			OutputDebugString(L"CBitmapWndRender::SetBitmap");
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


			//printf("CBitmapWndRender::SetBitmap  muBitmap.lock()\n");
			muBitmap.lock();
			if (source != nullptr)
			{
				delete source;
				//printf("CBitmapWndRender::SetBitmap   delete source\n");
				source = nullptr;
			}

			source = bitmapIn;
			//source = nullptr;
			muBitmap.unlock();
			//printf("CBitmapWndRender::SetBitmap  muBitmap.unlock()\n");
			toolOption = MOVEPICTURE;
			bitmapwidth = bitmapIn->GetWidth();
			bitmapheight = bitmapIn->GetHeight();
			orientation = bitmapIn->GetOrientation();


			ShrinkImage(false);
			AfterSetBitmap();

			RemoveListener(false);

			//needToRefresh = true;
			parentRender->Refresh();

			//RefreshWindow();
		}
	}
}

void CBitmapWndRender::CalculScreenPosFromReal(const int& xReal, const int& yReal, int& xScreen, int& yScreen)
{
	//
	xScreen = static_cast<int>(static_cast<float>(xReal) * (ratio));
	yScreen = static_cast<int>(static_cast<float>(yReal) * (ratio));
}

void CBitmapWndRender::CalculRealPosFromScreen(const int& xScreen, const int& yScreen, int& xReal, int& yReal)
{
	//
	xReal = static_cast<int>(static_cast<float>(xScreen) / (ratio));
	yReal = static_cast<int>(static_cast<float>(yScreen) / (ratio));
}


//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWndRender::GetInfosBitmap(wxString& filename, int& widthPicture, int& heightPicture)
{
	//
	widthPicture = bitmapwidth;
	heightPicture = bitmapheight;
	filename = this->filename;
}

int CBitmapWndRender::GetOrientation()
{
	//
	return orientation;
}

void CBitmapWndRender::SetOrientation(const int& exif)
{
	orientation = exif;
	ShrinkImage(true);
	//updateFilter = true;
	//RefreshWindow();
}

int CBitmapWndRender::GetRawBitmapWidth()
{
	//
	return bitmapwidth;
}

int CBitmapWndRender::GetRawBitmapHeight()
{
	//
	return bitmapheight;
}

int CBitmapWndRender::GetBitmapWidth()
{
	if (bitmapLoad)
	{
		if (angle == 90 || angle == 270)
			return GetRawBitmapHeight();
		return GetRawBitmapWidth();
	}
	return 0;
}

int CBitmapWndRender::GetBitmapHeight()
{
	if (bitmapLoad)
	{
		if (angle == 90 || angle == 270)
			return GetRawBitmapWidth();
		return GetRawBitmapHeight();
	}

	return 0;
}

bool CBitmapWndRender::UpdateExifInfos()
{
	CSqlPhotos sqlPhotos;
	int exifCode = sqlPhotos.GetExifFromAngleAndFlip(angle, flipHorizontal, flipVertical);
	sqlPhotos.UpdatePhotoExif(filename, exifCode);
	return true;
}

void CBitmapWndRender::FlipVertical()
{
	//
	if (flipVertical)
		flipVertical = 0;
	else
		flipVertical = 1;


	UpdateExifInfos();

	//updateFilter = true;
	RefreshWindow();
}

void CBitmapWndRender::Rotate90()
{
	//
	angle += 90;
	angle = angle % 360;

	UpdateExifInfos();
	UpdateResized();
	RefreshWindow();
}

int CBitmapWndRender::GetAngleFromExif()
{
	//
	switch (GetOrientation())
	{
	case 1: // top left side
	case 2: // top right side
		return 0;
	case 3: // bottom right side
	case 4: // bottom left side
		return 180;
	case 5: //left side top
	case 6: // right side top
		return 90;
	case 7: // right side bottom
	case 8: // left side bottom
		return 270;
	}
	return 0;
}

void CBitmapWndRender::Rotate270()
{
	//

	angle += 270;
	angle = angle % 360;
	//updateFilter = true;
	UpdateExifInfos();

	UpdateResized();
	RefreshWindow();
}

void CBitmapWndRender::FlipHorizontal()
{
	//
	if (flipHorizontal)
		flipHorizontal = 0;
	else
		flipHorizontal = 1;
	//updateFilter = true;

	UpdateExifInfos();

	RefreshWindow();
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWndRender::MouseClick(const int& xPos, const int& yPos)
{
	/*
	mouseScrollX = xPos;
	mouseScrollY = yPos;
	mouseBlock = true;
	parentRender->CaptureMouse();
	parentRender->SetFocus();
	*/
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWndRender::MouseRelease(const int& xPos, const int& yPos)
{
	//
	mouseBlock = false;
	//wxSetCursor(wxCursor(wxCURSOR_ARROW));
	wxSetCursor(*wxSTANDARD_CURSOR);

	if (parentRender->HasCapture())
		parentRender->ReleaseMouse();
}

void CBitmapWndRender::OnRButtonDown(wxMouseEvent& event)
{
	//
	parentRender->SetFocus();
	wxWindow* window = wxWindow::FindWindowById(idWindowMain);
	if (window != nullptr)
	{
		wxCommandEvent evt(TOOLBAR_UPDATE_ID);
		window->GetEventHandler()->AddPendingEvent(evt);
	}
}

//-----------------------------------------------------------------
//Gestion du click de souris
//-----------------------------------------------------------------
void CBitmapWndRender::OnLButtonDown(wxMouseEvent& event)
{
	parentRender->SetFocus();
	int xPos = event.GetX();
	int yPos = event.GetY();

	switch (toolOption)
	{
	case MOVEPICTURE:
		{
			mouseBlock = true;
			mouseScrollX = xPos;
			mouseScrollY = yPos;
			parentRender->CaptureMouse();
			wxSetCursor(hCursorHand);
		}
		break;

	default:
		wxSetCursor(*wxSTANDARD_CURSOR);
		break;
	}

	MouseClick(xPos, yPos);
}

//-----------------------------------------------------------------
//Obtention des dimensions du bitmap
//-----------------------------------------------------------------
void CBitmapWndRender::OnLButtonUp(wxMouseEvent& event)
{
	//
	if (event.LeftDClick())
		return;

	const int xPos = event.GetX();
	const int yPos = event.GetY();

	switch (toolOption)
	{
	case MOVEPICTURE:
		{
			mouseBlock = false;
			mouseScrollX = xPos;
			mouseScrollY = yPos;
			if (parentRender->HasCapture())
				parentRender->ReleaseMouse();
		}
		break;
	default: ;
	}

	MouseRelease(xPos, yPos);
}

void CBitmapWndRender::OnKeyUp(wxKeyEvent& event)
{
	//
	SetKey(0);
	switch (event.GetKeyCode())
	{
	case WXK_CONTROL:
		controlKeyPush = false;
		break;
	}
}

void CBitmapWndRender::MoveTop()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWndRender::MoveLeft()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWndRender::MoveBottom()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWndRender::MoveRight()
{
	wxWindow* parent = parentRender->GetParent();

	if (parent != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CBitmapWndRender::OnKeyDown(wxKeyEvent& event)
{
	//
	SetKey(event.GetKeyCode());

	switch (event.GetKeyCode())
	{
	case WXK_UP:
		//updateFilter = true;
		this->MoveTop();
		break;
	case WXK_LEFT:
		//updateFilter = true;
		this->MoveLeft();
		break;
	case WXK_DOWN:
		//updateFilter = true;
		this->MoveBottom();
		break;
	case WXK_RIGHT:
		//updateFilter = true;
		this->MoveRight();
		break;

	case WXK_CONTROL:
		controlKeyPush = true;
		break;

	case WXK_NUMPAD_ADD:
	case WXK_ADD:
		{
			const wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMIN);
			parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			this->ZoomOn();
		}
		break;

	case WXK_NUMPAD_SUBTRACT:
	case WXK_SUBTRACT:
		{
			const wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_BITMAPZOOMOUT);
			parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
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
void CBitmapWndRender::OnMouseWheel(wxMouseEvent& event)
{
	
	int move = 0;
	if (event.GetWheelRotation() > 0)
		move = 0;
	else
		move = 1;

	if (controlKeyPush)
		move += 10;
        
    if(shrinkImage)
    {
        move += 100;
    }
    
    auto stop = std::chrono::system_clock::now(); 

    printf("CBitmapWndRender::OnMouseWheel : %d \n", move);
    std::chrono::duration<float> t_diff = stop - start;
    std::chrono::milliseconds t_diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_diff);
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
			wxCommandEvent evt(wxEVT_BITMAPZOOMIN);
			parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			this->ZoomOn();
		}
		break;
	case 11:
		{
			wxCommandEvent evt(wxEVT_BITMAPZOOMOUT);
			parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
			this->ZoomOut();
		}
		break;
	case 100:
		{
            if(t_diff_ms.count() < 100)
                break;
            
            printf("void CBitmapWndRender::OnMouseWheel(wxMouseEvent& event) wxEVENT_PICTURENEXT \n");
            wxWindow * central = parentRender->GetParent()->FindWindowById(CENTRALVIEWERWINDOWID);
            if(central != nullptr)
            {
                wxCommandEvent evt(wxEVENT_PICTURENEXT);
                parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
            }
            
            start = std::chrono::system_clock::now();
		}
		break;
	case 101:
		{
            if(t_diff_ms.count() < 100)
                break;
                
            printf("void CBitmapWndRender::OnMouseWheel(wxMouseEvent& event) wxEVENT_PICTUREPREVIOUS \n");
            wxWindow * central = parentRender->GetParent()->FindWindowById(CENTRALVIEWERWINDOWID);
            if(central != nullptr)
            {
                wxCommandEvent evt(wxEVENT_PICTUREPREVIOUS);
                parentRender->GetParent()->GetEventHandler()->AddPendingEvent(evt);
            }
            
            start = std::chrono::system_clock::now();
		}
		break;
	default: ;
	}

    
}

void CBitmapWndRender::Resize()
{
	//updateFilter = true;
	UpdateResized();
	needToRefresh = true;
}

int CBitmapWndRender::UpdateResized()
{
	//
	if (bitmapLoad)
	{
		if (shrinkImage)
		{
			ShrinkImage();
		}
		else if (showScroll)
		{
			CalculPositionPicture(centerX, centerY);
			UpdateScrollBar();
		}
	}

	//Toolbar posRatio
	sliderInterface->SetTrackBarPosition(posRatio);
	SetDessinRatio();
	return 0;
}

//------------------------------------------------------------------------------------
//Déplacement de la souris
//------------------------------------------------------------------------------------
void CBitmapWndRender::OnMouseMove(wxMouseEvent& event)
{
	const int xPos = event.GetX();
	const int yPos = event.GetY();

	switch (toolOption)
	{
	case MOVEPICTURE:
		{
			wxSetCursor(hCursorHand);

			if (mouseBlock && !shrinkImage)
			{
				const int diffX = mouseScrollX - xPos;
				const int diffY = mouseScrollY - yPos;
				posLargeur += diffX;
				posHauteur += diffY;

				mouseScrollX = xPos;
				mouseScrollY = yPos;

				TestMaxX();
				TestMaxY();

				UpdateScrollBar();

#if defined(WIN32) && defined(_DEBUG)
			TCHAR message[200];
			wsprintf(message, L"posLargeur : %d et posHauteur : %d \n", posLargeur, posHauteur);
			OutputDebugString(message);
#endif

				if (mouseUpdate != nullptr)
				{
					if (mouseUpdate->IsSourcePreview())
					{
						updateFilter = true;
					}
				}

				needToRefresh = true;
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
			MouseMove(xPos * scale_factor, yPos * scale_factor); 
			break;
		}
	}

	//MouseMove(xPos * scale_factor, yPos * scale_factor);

	//needToRefresh = true;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWndRender::TestMaxX()
{
	//

	if (int xValue = static_cast<int>(GetBitmapWidthWithRatio()) - GetWidth(); posLargeur >= xValue)
		posLargeur = xValue;

	if (posLargeur < 0)
		posLargeur = 0;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CBitmapWndRender::TestMaxY()
{
	//

	if (int yValue = static_cast<int>(GetBitmapHeightWithRatio()) - GetHeight(); posHauteur >= yValue)
		posHauteur = yValue;

	if (posHauteur < 0)
		posHauteur = 0;
}


float CBitmapWndRender::CalculPictureRatio(const int& pictureWidth, const int& pictureHeight)
{
	//
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float new_ratio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		new_ratio = static_cast<float>(GetWidth()) / static_cast<float>(pictureWidth);
	else
		new_ratio = static_cast<float>(GetHeight()) / static_cast<float>(pictureHeight);

	if ((pictureHeight * new_ratio) > GetHeight())
	{
		new_ratio = static_cast<float>(GetHeight()) / static_cast<float>(pictureHeight);
	}
	if ((pictureWidth * new_ratio) > GetWidth())
	{
		new_ratio = static_cast<float>(GetWidth()) / static_cast<float>(pictureWidth);
	}

	return new_ratio;
}

//-----------------------------------------------------------------
//Calcul du ratio pour l'image plein écran
//-----------------------------------------------------------------
float CBitmapWndRender::CalculRatio(const int& pictureWidth, const int& pictureHeight)
{
	//
	float newRatio = CalculPictureRatio(pictureWidth, pictureHeight);

	//Détermination du ration par rapport au tableau

	if (newRatio != 0.0)
	{
		for (int i = 0; i < value.size(); i++)
		{
			if (newRatio < ((float)value[i] / 100.0f))
			{
				ratio = ((float)value[i] / 100.0f);
				posRatio = i;
				break;
			}
		}
	}

	return newRatio;
}


void CBitmapWndRender::TestMaxPosition()
{
	//
	TestMaxX();
	TestMaxY();
}


void CBitmapWndRender::CalculRectPictureInterpolation(wxRect& rc, int& widthInterpolationSize,
                                                      int& heightInterpolationSize,
                                                      int& left, int& top, const bool& invert)
{
	int widthOutput = static_cast<int>(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = static_cast<int>(GetBitmapHeightWithRatio()) * scale_factor;
	int xValue = 0;
	int yValue = 0;


	if (widthOutput > GetWidth() * scale_factor)
	{
		left = 0;
		xValue = posLargeur * scale_factor;
	}
	else
	{
		xValue = 0;
		left = (GetWidth() * scale_factor - widthOutput) / 2;
	}

	widthInterpolationSize = GetWidth() * scale_factor - (left * 2);


	if (heightOutput > GetHeight() * scale_factor)
	{
		top = 0;
		yValue = posHauteur * scale_factor;
	}
	else
	{
		yValue = 0;
		top = (GetHeight() * scale_factor - heightOutput) / 2;
	}

	heightInterpolationSize = GetHeight() * scale_factor - (top * 2);

	rc.x = max(xValue, 0);
	if (invert)
	{
		int heightmax = heightOutput - (GetHeight() * scale_factor) - yValue;
		rc.y = max(heightmax, 0);
	}
	else
		rc.y = max(yValue, 0);
	rc.width = widthOutput;
	rc.height = heightOutput;
}

void CBitmapWndRender::UpdateScreenRatio()
{
	//
	Resize();
}

void CBitmapWndRender::GenerateScreenBitmap(CFiltreEffet* filtreEffet, int& widthOutput, int& heightOutput)
{
	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	int localAngle = angle;
	int localflipHorizontal = flipHorizontal;
	int localflipVertical = flipVertical;
	int filterInterpolation = 0;


	if (regardsParam != nullptr)
		filterInterpolation = regardsParam->GetInterpolationType();

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
	CalculRectPictureInterpolation(rc, widthOutput, heightOutput, left, top, false);
	filtreEffet->Interpolation(widthOutput, heightOutput, rc, filterInterpolation, localflipHorizontal,
	                           localflipVertical, localAngle, value[posRatio]);

	if (regardsParam != nullptr)
	{
		if (regardsParam->GetAutoConstrast() && preview == 0)
		{
			filtreEffet->SetPreviewMode(true);
			filtreEffet->BrightnessAndContrastAuto(1);
		}
	}
}

void CBitmapWndRender::SetPreview(const int& value)
{
	preview = value;
}

void CBitmapWndRender::RenderToScreenWithCudaSupport()
{
	CRgbaquad color;

	int widthOutput = static_cast<int>(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = static_cast<int>(GetBitmapHeightWithRatio()) * scale_factor;


	bool invert = true;
	muBitmap.lock();
	bool bitmapIsLoad = false;

	if (loadBitmap)
	{
		if (filtreEffet == nullptr)
			filtreEffet = new CFiltreEffet(color, false, true, source);
		else
		{
			filtreEffet->SetBitmap(source);
		}

		loadBitmap = false;
		bitmapIsLoad = true;
	}
	muBitmap.unlock();


	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
		if (widthOutput < 0 || heightOutput < 0)
			return;

		if (updateFilter || mouseUpdate != nullptr)
		{
			if (!bitmapIsLoad)
				filtreEffet->SetBitmap(source);
			BeforeInterpolationBitmap();
			updateFilter = true;
		}


		//printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);
		if (updateFilter || widthOutputOld != widthOutput || heightOutputOld != heightOutput)
		{
			GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

			ApplyPreviewEffect(widthOutput, heightOutput);

			glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, isOpenCLOpenGLInterop);

			cv::cuda::GpuMat data = filtreEffet->GetGpuMat();
			glTexture->SetData(data);

		}

		updateFilter = false;

		widthOutputOld = widthOutput;
		heightOutputOld = heightOutput;

		renderOpenGL->CreateScreenRender(GetWidth() * scale_factor, GetHeight() * scale_factor,
			CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(),
				themeBitmap.colorBack.Blue()));

		RenderTexture(invert);
	}

}
void CBitmapWndRender::RenderToScreenWithOpenCLSupport()
{
	//printf("void CBitmapWndRender::RenderToScreenWithOpenCLSupport() \n");

	CRgbaquad color;

	int widthOutput = static_cast<int>(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = static_cast<int>(GetBitmapHeightWithRatio()) * scale_factor;


	bool invert = true;
	muBitmap.lock();
	bool bitmapIsLoad = false;

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	//bool useCuda = regardsParam->GetIsUseCuda();
	bool useOpenCL = regardsParam->GetIsOpenCLSupport();

	if (loadBitmap)
	{
		if (filtreEffet == nullptr)
			filtreEffet = new CFiltreEffet(color, useOpenCL, false, source);
		else
		{
			filtreEffet->SetBitmap(source);
		}

		loadBitmap = false;
		bitmapIsLoad = true;
	}
	muBitmap.unlock();

	//printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);

	//UpdateScrollBar();

	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
		if (widthOutput < 0 || heightOutput < 0)
			return;
		/*
		if (updateFilter)
		{
			if (!bitmapIsLoad)
				filtreEffet->SetBitmap(source);
			BeforeInterpolationBitmap();

			updateFilter = true;
			
		}
		*/
		if (updateFilter || mouseUpdate != nullptr)
		{
			if (!bitmapIsLoad)
				filtreEffet->SetBitmap(source);
			BeforeInterpolationBitmap();
			updateFilter = true;
		}


		//printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);
		if (updateFilter || widthOutputOld != widthOutput || heightOutputOld != heightOutput)
		{
			GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

			ApplyPreviewEffect(widthOutput, heightOutput);

			glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, isOpenCLOpenGLInterop);

			cv::UMat data = filtreEffet->GetUMat();
			if (!glTexture->SetData(data))
				isOpenCLOpenGLInterop = false;

			/*
			if (isOpenCLOpenGLInterop)
			{
				cv::UMat data = filtreEffet->GetUMat();
				if (!glTexture->SetData(data))
					isOpenCLOpenGLInterop = false;
			}
			else
			{
				cv::Mat data = filtreEffet->GetMat();
				glTexture->SetData(data);
			}
			*/

		}



		updateFilter = false;

		widthOutputOld = widthOutput;
		heightOutputOld = heightOutput;

		renderOpenGL->CreateScreenRender(GetWidth() * scale_factor, GetHeight() * scale_factor,
			CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(),
				themeBitmap.colorBack.Blue()));

		RenderTexture(invert);
	}


}



void CBitmapWndRender::RenderToScreenWithoutOpenCLSupport()
{
	//printf("CBitmapWndRender RenderToScreenWithoutOpenCLSupport \n");

	CRgbaquad color;

	int widthOutput = static_cast<int>(GetBitmapWidthWithRatio()) * scale_factor;
	int heightOutput = static_cast<int>(GetBitmapHeightWithRatio()) * scale_factor;

	if (loadBitmap)
	{
		if (filtreEffet == nullptr)
			filtreEffet = new CFiltreEffet(color, false, false, source);
		else
			filtreEffet->SetBitmap(source);
	}


	if (bitmapLoad && GetWidth() > 0 && GetHeight() > 0)
	{
		if (widthOutput < 0 || heightOutput < 0)
			return;


		if (updateFilter || mouseUpdate != nullptr)
		{
			BeforeInterpolationBitmap();
			updateFilter = true;
		}

		if (updateFilter || widthOutputOld != widthOutput || heightOutputOld != heightOutput)
		{
			GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

			ApplyPreviewEffect(widthOutput, heightOutput);

			//printf("widthOutput : %d heightOutput %d \n", widthOutput, heightOutput);

			cv::Mat bitmap = filtreEffet->GetMat();

			if (bitmap.size().width != widthOutput || bitmap.size().height != heightOutput)
			{
				float percent = CPictureUtility::CalculPictureRatio(bitmap.size().width, bitmap.size().height, widthOutput,
					heightOutput);
				cv::resize(bitmap, bitmap, cv::Size(bitmap.size().width * percent, bitmap.size().height * percent));
			}


			glTexture = renderOpenGL->GetDisplayTexture(widthOutput, heightOutput, isOpenCLOpenGLInterop);
			if (glTexture != nullptr)
				glTexture->SetData(bitmap);
		}


		renderOpenGL->CreateScreenRender(GetWidth() * scale_factor, GetHeight() * scale_factor,
			CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(),
				themeBitmap.colorBack.Blue()));

		RenderTexture(true);
	}

	updateFilter = false;

}

void CBitmapWndRender::OnIdle(wxIdleEvent& evt)
{
	if (needToRefresh)
		parentRender->Refresh();
	needToRefresh = false;
}

void CBitmapWndRender::RenderTexture(const bool& invertPos)
{
	if (glTexture != nullptr)
	{
		int x = (GetWidth() * scale_factor - glTexture->GetWidth()) / 2;
		int y = (GetHeight() * scale_factor - glTexture->GetHeight()) / 2;

		renderOpenGL->RenderToScreen(mouseUpdate, effectParameter, x, y, invertPos);

		xPosImage = x;
		yPosImage = y;
	}
}

void CBitmapWndRender::ReloadResource()
{
	//reloadResource = true;
}

void CBitmapWndRender::SetOpenGLOutput(const bool& value)
{
	isOpenGLShow = value;
}


void CBitmapWndRender::OnPaint2D(wxWindow* gdi)
{
	wxBufferedPaintDC dc(gdi);

	if (source != nullptr)
	{
		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		bool useCuda = regardsParam->GetIsUseCuda();
		bool useOpenCL = regardsParam->GetIsOpenCLSupport();
		CRgbaquad color;

		int widthOutput = static_cast<int>(GetBitmapWidthWithRatio());
		int heightOutput = static_cast<int>(GetBitmapHeightWithRatio());

		if (filtreEffet == nullptr)
		{
			filtreEffet = new CFiltreEffet(color, useOpenCL, useCuda, source);
		}
		else
			filtreEffet->SetBitmap(source);

		if (updateFilter)
		{
			BeforeInterpolationBitmap();
			updateFilter = false;
		}

		GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);

		int screenWidth = GetWidth() * scale_factor;
		int left = 0, top = 0;
		int tailleAffichageWidth = widthOutput;
		int tailleAffichageHeight = heightOutput;
		int filterInterpolation = 0;
		if (screenWidth > tailleAffichageWidth)
			left = ((screenWidth - tailleAffichageWidth) / 2);
		else
			left = 0;

		if (GetHeight() * scale_factor > tailleAffichageHeight)
			top = ((GetHeight() * scale_factor - tailleAffichageHeight) / 2);
		else
			top = 0;

		wxImage image = filtreEffet->GetwxImage();
		CWindowUtility winUtility;
		winUtility.FillRect(&dc, gdi->GetRect(), themeBitmap.colorBack);
		dc.DrawBitmap(image.Mirror(), left, top);
		AfterRender();
	}
	else
	{
		CWindowUtility winUtility;
		winUtility.FillRect(&dc, gdi->GetRect(), themeBitmap.colorBack);
	}
}

//-----------------------------------------------------------------
//Dessin de l'image
//-----------------------------------------------------------------
void CBitmapWndRender::OnPaint3D(wxGLCanvas* canvas, CRenderOpenGL* renderOpenGL)
{
#ifndef WIN32
	scale_factor = parentRender->GetContentScaleFactor();
#endif

	isOpenGLShow = true;

	if (renderBitmapOpenGL == nullptr)
	{
		this->renderOpenGL = renderOpenGL;
		renderBitmapOpenGL = new CRenderBitmapOpenGL(renderOpenGL);
		renderBitmapOpenGL->LoadingResource(scale_factor, themeBitmap.colorArrow);
		if (filtreEffet != nullptr)
			delete filtreEffet;
		filtreEffet = nullptr;
	}



	if (renderBitmapOpenGL != nullptr)
	{


		if (!IsSupportOpenCL() && !IsSupportCuda())
		{
			//printf("CBitmapWndRender OnPaint RenderToScreenWithoutOpenCLSupport\n");
			RenderToScreenWithoutOpenCLSupport();
		}
		else
		{
			//printf("CBitmapWndRender OnPaint RenderToScreenWithOpenCLSupport \n");
#ifndef __APPLE__

			if (IsSupportCuda())
			{
				RenderToScreenWithCudaSupport();
			}
			else
			{
#endif
				RenderToScreenWithOpenCLSupport();

#ifndef __APPLE__
			}
#endif
		}


		AfterRender();
	}

	canvas->SwapBuffers();
}

void CBitmapWndRender::RefreshWindow()
{
	needToRefresh = true;
}

//-----------------------------------------------------------------
//Gestion du double clique
//-----------------------------------------------------------------
void CBitmapWndRender::OnLDoubleClick(wxMouseEvent& event)
{
	//
#ifdef __APPLE__
	OnRButtonDown(event);
#else
	ShrinkImage(true);
#endif
}
