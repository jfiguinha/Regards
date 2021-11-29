#include <header.h>
#include "BitmapWndViewer.h"
#include <ConvertUtility.h>
#include <FiltreEffet.h>
#include <RegardsConfigParam.h>
#include <SavePicture.h>
#include <Selection.h>
#include <BitmapInterface.h>
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include "wx/stdpaths.h"
#include <FilterData.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <RegardsBitmap.h>
#include <OpenCLContext.h>
#include <wx/mimetype.h>
#include <GLTexture.h>
#include <RenderBitmapOpenGL.h>
#include "PageCurlFilter.h"
#include "MoveEffectTexture.h"
#include "NoneEffectTextureEffect.h"
#include "DiaporamaEffect.h"
#include <wx/busyinfo.h>
#include <wx/activityindicator.h>
#ifdef __APPLE__
    #include <SaveFromCFunction.h>
    #include <SaveFileFormat.h>
    #include <SendEmail.h>
    #include <OpenCL/opencl.h>
#elif defined(WIN32)
#include <LocalMapi.h>
#else
    #include "email.h"
#endif
#include <config_id.h>
using namespace Regards::Filter;
using namespace Regards::Control;
using namespace Regards::OpenCL;
using namespace Regards::OpenGL;
using namespace Regards::Window;

enum
{
	TIMER_TRANSITION = 1,
	TIMER_SELECTEFFECT = 2,
	TIMER_CLICK = 3
};


#define PREVIEW_NONE 0
#define TIMER_TRANSITION_TIME 30
#define TIMER_TRANSITION 31
#define TIMER_CLICK 32
#define TIMER_CLICK_TIME 200

IAfterEffect* CBitmapWndViewer::AfterEffectPt(const int& numFilter)
{
	switch (numFilter)
	{
	case IDM_DIAPORAMA_TRANSITION:
		return new CDiaporamaEffect();

	case IDM_DIAPORAMA_MOVE:
	case IDM_AFTEREFFECT_MOVE:
		return new CMoveEffectTextureEffect();

	case IDM_AFTEREFFECT_PAGECURL:
		return new CPageCurlFilter();

	case IDM_DIAPORAMA_FUSION:
	case IDM_AFTEREFFECT_FUSION:
		return new CBitmapFusionFilter();

	default:
		return new CNoneEffectTextureEffect();
	}
}


void CBitmapWndViewer::CalculCenterPositionPicture()
{
	CalculPositionPicture(centerX, centerY);
}

void CBitmapWndViewer::SendEmail()
{
	wxString subject = CLibResource::LoadStringFromResource(L"LBLEMAILSUBJECT", 1);
	wxString body = CLibResource::LoadStringFromResource(L"lblemailbody", 1);

#ifdef __APPLE__
    
    CSendEmail sendEmail;
    sendEmail.SendEmail("", subject, CConvertUtility::ConvertToStdString(filename));
    
#elif defined(WIN32)

	CMapi m_cMapi;
	vector<string> attachment;


	attachment.push_back(CConvertUtility::ConvertToStdString(filename));
	if (attachment.size() > 0)
		m_cMapi.SendEmail("", attachment);

#else
    wxExecute("thunderbird -remote \"xfeDoCommand(composeMessage,subject='" + subject + "',body='" + body + "',attachment='" + filename + "')\"");    
#endif
}

void CBitmapWndViewer::SetListener(IMouseUpdate* mouseUpdate)
{
	this->mouseUpdate = mouseUpdate;
}

void CBitmapWndViewer::RemoveListener(const bool& applyCancel)
{
	if (mouseUpdate != nullptr && applyCancel)
	{
		mouseUpdate->CancelPreview(this);
		updateFilter = true;
		if(source != nullptr)
		{
			bitmapwidth = source->GetWidth();
			bitmapheight = source->GetHeight();
			ShrinkImage();
		}

	}
	
	mouseUpdate = nullptr;
	effectParameter = nullptr;

	if (openclContext != nullptr)
	{
		if (!openclContext->IsSharedContextCompatible())
		{
			if (renderOpenGL != nullptr)
			{
				delete renderOpenGL;
				renderOpenGL = nullptr;
			}
		}
	}

	loadBitmap = true;
	parentRender->Refresh();
}

CEffectParameter* CBitmapWndViewer::GetParameter()
{
	return effectParameter;
}

void CBitmapWndViewer::UpdateFiltre(CEffectParameter* effectParameter)
{
	this->effectParameter = effectParameter;
	updateFilter = true;
	if(mouseUpdate != nullptr)
	{
		if (mouseUpdate->IsSourcePreview())
		{
			needUpdate = true;
			copyBitmap = true;
		}
	}

	RefreshWindow();
}

void CBitmapWndViewer::SetDiaporamaMode()
{
	isDiaporama = true;
	// isDiaporama = false;
}

void CBitmapWndViewer::SetNormalMode()
{
	isDiaporama = false;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
	this->ShrinkImage();
}

void CBitmapWndViewer::BeforeInterpolationBitmap()
{
	if (preview > 1 && mouseUpdate != nullptr)
	{
		
		if (copyBmpSrc == nullptr || updateFilter)
		{
			wxBeginBusyCursor();
			mouseUpdate->ApplyPreviewEffectSource(effectParameter, this, filtreEffet, m_cDessin);
			if (copyBmpSrc != nullptr)
				delete copyBmpSrc;
			copyBmpSrc = filtreEffet->GetBitmap(true);
			updateFilter = false;
			bitmapwidth = copyBmpSrc->GetBitmapWidth();
			bitmapheight = copyBmpSrc->GetBitmapHeight();

			if (mouseUpdate->NeedToShrink())
				ShrinkImage();
			
			wxEndBusyCursor();
		}
		else if(mouseUpdate->IsSourcePreview())
		{
			CImageLoadingFormat image(false);
			image.SetPicture(copyBmpSrc);
			filtreEffet->SetBitmap(&image);
		}
		

	}

}

void CBitmapWndViewer::PrintPicture()
{
	CRegardsBitmap* bitmap = GetBitmap(true);
	wxWindow* mainWindow = wxWindow::FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PRINTPICTURE);
		evt.SetClientData(bitmap);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

CBitmapWndViewer::CBitmapWndViewer(CSliderInterface* slider, wxWindowID mainViewerId,
                                   const CThemeBitmapWindow& theme, CBitmapInterface* bitmapInterface)
	: CBitmapWndRender(slider, mainViewerId, theme)
{
	
	//TRACE();
#ifndef WIN32
	double scale_factor = parentRender->GetContentScaleFactor();
#else
	const double scale_factor = 1.0f;
#endif

	mouseUpdate = nullptr;
	etape = 0;
	preview = 0;
	m_bTransition = false;
	isDiaporama = false;
	isInUse = false;
	invertColor = false;
	transitionTimer = nullptr;
	clickTimer = nullptr;
	etape = 0;
	fixArrow = true;
	nextPicture = nullptr;
	startTransition = false;
	this->bitmapInterface = bitmapInterface;
	oldMouse.x = 0;
	oldMouse.y = 0;
	m_cDessin = nullptr;
	afterEffect = nullptr;



	arrowPrevious.x = 0;
	arrowPrevious.y = 0;
	arrowPrevious.width = 32 * scale_factor;
	arrowPrevious.height = 32 * scale_factor;

	arrowNext.x = 0;
	arrowNext.y = 0;
	arrowNext.width = 32 * scale_factor;
	arrowNext.height = 32 * scale_factor;
}

void CBitmapWndViewer::SetParent(wxWindow* parent)
{
	parentRender = parent;
	transitionTimer = new wxTimer(parentRender, TIMER_TRANSITION);
	clickTimer = new wxTimer(parentRender, TIMER_CLICK);
}

void CBitmapWndViewer::OnTimer(wxTimerEvent& event)
{
	switch (event.GetEventType())
	{
	case TIMER_TRANSITION:
		OnTransition(event);
		break;
	case TIMER_CLICK:
		OnClick(event);
		break;
	}
}

vector<int> CBitmapWndViewer::GetListTimer()
{
	vector<int> listTimer;
	listTimer.push_back(TIMER_TRANSITION);
	listTimer.push_back(TIMER_CLICK);
	return listTimer;
}
//parentRender->Connect(TIMER_TRANSITION, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnTransition), nullptr, parentRender);
//parentRender->Connect(TIMER_CLICK, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnClick), nullptr, parentRender);

void CBitmapWndViewer::OnClick(wxTimerEvent& event)
{
	event.GetId();
	if (typeClick == 0)
	{
		bitmapInterface->ImagePrecedente();
	}
	else
	{
		bitmapInterface->ImageSuivante();
	}
	clickTimer->Start(TIMER_CLICK_TIME, true);
}

void CBitmapWndViewer::OnTransition(wxTimerEvent& event)
{
	if (m_bTransition)
	{
		//if(isDiaporama)
		//	etape += 1;
		//else
		etape += 10;


		if (etape > 100)
		{
			etape = 0;
			EndTransition();
			m_bTransition = false;
		}
		else
		{
			m_bTransition = true;
			transitionTimer->Start(TIMER_TRANSITION_TIME, true);
			parentRender->Refresh();
		}
	}
}

CBitmapWndViewer::~CBitmapWndViewer()
{
	
	
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	if (clickTimer->IsRunning())
		clickTimer->Stop();

	if (afterEffect != nullptr)
	{
		afterEffect->DeleteTexture();
		delete(afterEffect);
		afterEffect = nullptr;
	}

	//delete(openclEffectVideo);
	delete(transitionTimer);
	delete(clickTimer);
	delete(m_cDessin);
}

void CBitmapWndViewer::AfterSetBitmap()
{
	updateFilter = true;
	preview = PREVIEW_NONE;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
}

void CBitmapWndViewer::ExportPicture()
{
	CSavePicture::ExportPicture(parentRender, filename);
}

void CBitmapWndViewer::SavePicture()
{
	CRegardsBitmap* bitmap = GetBitmap(true);
	auto imageLoading = new CImageLoadingFormat();
	imageLoading->SetPicture(bitmap);
	CSavePicture::SavePicture(nullptr, imageLoading, filename);
	if (imageLoading != nullptr)
		delete imageLoading;
}

void CBitmapWndViewer::DeterminePos(wxRect& rc, const int& nTailleAffichageWidth, const int& nTailleAffichageHeight,
                                    int& left, int& top)
{
	if (rc.width > nTailleAffichageWidth)
		left = ((rc.width - nTailleAffichageWidth) / 2);
	else
		left = 0;

	if (rc.height > nTailleAffichageHeight)
		top = ((rc.height - nTailleAffichageHeight) / 2);
	else
		top = 0;
}

void CBitmapWndViewer::SetDessinRatio()
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif

	wxRect rc, rc2;
	int iLeft, iTop;
	int iTailleWidth, iTailleHeight;

	rc2.x = 0;
	rc2.y = 0;
	rc2.width = GetWidth() * scale_factor;
	rc2.height = GetHeight() * scale_factor;

	//GetClientRect(m_hWnd, &rc2);

	iTailleWidth = GetBitmapWidthWithRatio() * scale_factor;
	iTailleHeight = GetBitmapHeightWithRatio() * scale_factor;

	if (m_cDessin != nullptr)
		m_cDessin->SetRatio(ratio);

	DeterminePos(rc2, iTailleWidth, iTailleHeight, iLeft, iTop);

	//rc.x = iLeft;
	rc.x = 0;
	rc.width = iTailleWidth;
	//rc.y = iTop;
	rc.y = 0;
	rc.height = iTailleHeight;

	if (m_cDessin != nullptr)
		m_cDessin->SetMaxPosition(rc);
}

void CBitmapWndViewer::SetBitmapPreviewEffect(const int& effect)
{
	preview = effect;
	isInUse = true;
	if (m_cDessin != nullptr)
		delete(m_cDessin);

	m_cDessin = CFiltreData::GetDrawingPt(effect);
	if (m_cDessin != nullptr)
	{
		SetDessinRatio();
		wxSetCursor(wxCursor(wxCURSOR_CROSS));
		isInUse = false;
		SetTool(preview);
	}
	else
	{
		wxSetCursor(*wxSTANDARD_CURSOR);
		toolOption = MOVEPICTURE;
	}
}

void CBitmapWndViewer::Resize()
{
	//updateFilter = true;
	UpdateResized();

	if (CFiltreData::SupportMouseClick(toolOption))
	{
		if (!isInUse && m_cDessin != nullptr)
			SetDessinRatio();
	}

	//this->Refresh();
}

bool CBitmapWndViewer::ApplyPreviewEffect(int& widthOutput, int& heightOutput)
{
	if (preview > 1 && mouseUpdate != nullptr)
	{
		if (IsOpenGLDecoding())
		{
			if (!mouseUpdate->IsOpenGLCompatible())
				GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);
		}

		mouseUpdate->ApplyPreviewEffect(effectParameter, this, filtreEffet, m_cDessin, widthOutput, heightOutput);

		if (mouseUpdate->NeedToUpdateSource())
			updateFilter = true;

		if (IsOpenGLDecoding())
			if (!mouseUpdate->IsOpenGLCompatible())
				return false;
	}
	return true;
}

void CBitmapWndViewer::OnFiltreOk()
{
	RemoveListener();
}

void CBitmapWndViewer::OnFiltreCancel()
{
	RemoveListener();
}

wxPoint CBitmapWndViewer::GetMousePosition()
{
	wxPoint pt;
	if (m_cDessin != nullptr)
		m_cDessin->GetPoint(pt);
	return pt;
}

void CBitmapWndViewer::StopTransition()
{
	m_bTransition = false;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	startTransition = false;
	m_bTransition = false;
	etape = 0;
}

void CBitmapWndViewer::EndTransition()
{
	if (isDiaporama)
	{
		SetBitmap(nextPicture, false);
		nextPicture = nullptr;
	}

	startTransition = false;
	bitmapInterface->TransitionEnd();
}

bool CBitmapWndViewer::IsOpenCLCompatible()
{
	if (openclContext == nullptr)
		return false;

	if (filtreEffet == nullptr)
		return false;

	return (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL);
}

//---------------------------------------------------------
//
//---------------------------------------------------------
void CBitmapWndViewer::SetTransitionBitmap(CImageLoadingFormat* bmpSecond)
{
	if (etape != 0)
	{
		StopTransition();
	}

	int numEffect = 0;

	if (isDiaporama)
	{
		nextPicture = nullptr;
		numEffect = config->GetDiaporamaTransitionEffect();
	}
	else if (config != nullptr)
		numEffect = config->GetEffect();

	if (!bitmapLoad)
		numEffect = 0;

	if (oldTransNumEffect != numEffect)
	{
		if (afterEffect != nullptr)
		{
			delete(afterEffect);
			afterEffect = nullptr;
		}
		afterEffect = AfterEffectPt(numEffect);

		oldTransNumEffect = numEffect;
	}

	if (afterEffect != nullptr)
		afterEffect->SetTransitionBitmap(true, this, bmpSecond);
}

void CBitmapWndViewer::StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool& setPicture)
{
	if (setPicture)
	{
		nextPicture = nullptr;
		SetBitmap(bmpSecond, false);
	}
	else
		nextPicture = bmpSecond;
	startTransition = true;
	m_bTransition = true;
	etape = 0;
	transitionTimer->Start(TIMER_TRANSITION_TIME, true);
}

void CBitmapWndViewer::StopTransitionEffect(CImageLoadingFormat* bmpSecond)
{
	etape = 0;
	m_bTransition = false;
	SetBitmap(bmpSecond, false);
	startTransition = false;
	nextPicture = nullptr;
	EndTransition();
}

void CBitmapWndViewer::MouseRelease(const int& xPos, const int& yPos)
{
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		mouseBlock = false;
		mouseScrollX = xPos;
		mouseScrollY = yPos;
		if (parentRender->HasCapture())
			parentRender->ReleaseMouse();
	}

	if (clickTimer->IsRunning())
		clickTimer->Stop();
}

void CBitmapWndViewer::MouseClick(const int& xPos, const int& yPos)
{
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		int hpos = GetHPos();
		int vpos = GetVPos();

		int x = (xPos - xPosImage);
		int y = (yPos - yPosImage);


		printf("MouseClick xPos : %d xPosImage : %d \n", xPos, xPosImage);
		printf("MouseClick yPos : %d yPosImage : %d \n", yPos, yPosImage);

		printf("MouseClick x : %d \n", x);
		printf("MouseClick y : %d \n", y);

		invertColor = true;
		mouseBlock = true;
		parentRender->CaptureMouse();
		m_cDessin->InitPoint(x, y, hpos, vpos, GetRatio());
	}
	else if (fixArrow)
	{
		int yPosTop = (GetHeight() - arrowNext.GetHeight()) / 2;
		int yPosBottom = (GetHeight() - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

		if (xPos < arrowPrevious.GetWidth() && (yPos > yPosTop && yPos < yPosBottom))
		{
			typeClick = 0;
			bitmapInterface->ImagePrecedente();
			clickTimer->Start(TIMER_CLICK_TIME, true);
		}
		else if ((xPos > (GetWidth() - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
		{
			typeClick = 1;
			bitmapInterface->ImageSuivante();
			clickTimer->Start(TIMER_CLICK_TIME, true);
		}
	}
}


CRgbaquad CBitmapWndViewer::GetBackColor()
{
	return CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
}

void CBitmapWndViewer::FixArrowNavigation(const bool& fix)
{
	fixArrow = fix;
}


int CBitmapWndViewer::GetRawBitmapWidth()
{
	return bitmapwidth;
}

int CBitmapWndViewer::GetRawBitmapHeight()
{
	return bitmapheight;
}

int CBitmapWndViewer::GetOrientation()
{
	//if(preview == IDM_DECODE_RAW)
	//	return 4;
	return orientation;
}


CDraw* CBitmapWndViewer::GetDessinPt()
{
	return m_cDessin;
}

void CBitmapWndViewer::SetNextPictureMove(const bool& value)
{
	isNext = value;
}


void CBitmapWndViewer::AfterRender()
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif


	if (bitmapLoad)
	{
		//Affichage de la transition
		int numEffect = 0;

		if (isDiaporama)
			numEffect = config->GetDiaporamaTransitionEffect();
		else if (config != nullptr)
			numEffect = config->GetEffect();

		if (isDiaporama && numEffect == 0)
		{
			startTransition = false;
				//bitmapInterface->TransitionEnd();
		}

		if (numEffect != 0 && (etape > 0 && etape < 101) && afterEffect != nullptr)
		{
			afterEffect->AfterRender(nextPicture, renderOpenGL, this, etape, scale_factor, isNext, ratio);
		}
	}

	if (!isDiaporama)
	{
		//Insertion dans le HBITMAP
		if (fixArrow && (etape == 0 || etape == 100))
		{
			renderOpenGL->ShowArrowPrevious();
			renderOpenGL->ShowArrowNext();
		}
	}
}

void CBitmapWndViewer::RenderTexture(const bool& invertPos)
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

		//Affichage de la transition
		int num_effect;

		if (isDiaporama)
			num_effect = config->GetDiaporamaTransitionEffect();
		else if (config != nullptr)
			num_effect = config->GetEffect();

		if (afterEffect != nullptr)
			afterEffect->RenderMoveTexture(x, y, glTexture, etape, isNext);

		bool isValid = false;
		if (afterEffect != nullptr)
			isValid = afterEffect->RenderTexture(nextPicture, source, this, renderOpenGL, scale_factor, etape);

		if (!isValid)
			renderOpenGL->RenderToScreen(mouseUpdate, effectParameter, x, y, invertPos);

		xPosImage = x;
		yPosImage = y;
	}
}

void CBitmapWndViewer::DeleteTexture()
{
	if (afterEffect != nullptr)
		afterEffect->DeleteTexture();

	if (renderOpenGL != nullptr)
		renderOpenGL->DeleteTexture();
}

void CBitmapWndViewer::KeyPress(const int& key)
{
	if (key == WXK_ESCAPE)
	{
		if (CFiltreData::SupportMouseSelection(toolOption))
		{
			preview = PREVIEW_NONE;
			updateFilter = true;
			RefreshWindow();
		}
	}
}

void CBitmapWndViewer::MouseMove(const int& xPos, const int& yPos)
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif


	if (CFiltreData::SupportMouseClick(toolOption))
	{
		
		int hpos = GetHPos();
		int vpos = GetVPos();

		int x = (xPos - xPosImage);
		int y = (yPos - yPosImage);

		CFiltreData::SetCursor(toolOption);
		if (mouseBlock)
		{
			m_cDessin->MouseMove(x, y, hpos, vpos, GetRatio());
			updateFilter = true;
			needUpdate = true;
			parentRender->Refresh();
		}
		else if (CFiltreData::SupportMouseSelection(toolOption))
		{
			m_cDessin->Selection(x, y, hpos, vpos, GetRatio());
			//updateFilter = true;
			parentRender->Refresh();
		}
	}
	else
	{
		if (fixArrow && etape == 0)
		{
			const int yPosTop = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2;
			const int yPosBottom = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

			if (xPos < arrowPrevious.GetWidth() * scale_factor && (yPos > yPosTop && yPos < yPosBottom))
			{
				wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else if ((xPos > (GetWidth() * scale_factor - arrowNext.GetWidth()) && (yPos > yPosTop && yPos <
				yPosBottom)))
			{
					wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else
			{
				wxSetCursor(*wxSTANDARD_CURSOR);
			}
		}
	}
}
