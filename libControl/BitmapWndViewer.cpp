#include <header.h>
#include "BitmapWndViewer.h"
#include <ConvertUtility.h>
#include <OpenCLEffect.h>
#include <FiltreEffet.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <SavePicture.h>
#include <Selection.h>
#include <BitmapInterface.h>
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include <MetadataExiv2.h>
#include "wx/stdpaths.h"
#include <FilterData.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <RegardsBitmap.h>
#include <OpenCLContext.h>
#include <wx/mimetype.h>
#include <GLTexture.h>
#include <RenderBitmapOpenGL.h>
#include "PageCurlFilter.h"
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

class CMoveEffectTextureEffect : public CBitmapFusionFilter
{
public:
	CMoveEffectTextureEffect() {};
	~CMoveEffectTextureEffect() {};

	virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float & scale_factor, const bool & isNext, float& ratio)
	{
		int xtexture = 0;
		int pos = 0;

		wxRect local;
		float newRatio = bmpViewer->CalculPictureRatio(nextPicture->GetWidth(), nextPicture->GetHeight());
		local.width = nextPicture->GetWidth() * newRatio;
		local.height = nextPicture->GetHeight() * newRatio;
		local.x = (bmpViewer->GetWidth() - out.width) / 2;
		local.y = (bmpViewer->GetHeight() - out.height) / 2;


		if (local.width != out.width)
		{
			GenerateEffectTexture(nextPicture, bmpViewer);
		}

		if (isNext)
		{
			int posMin = (bmpViewer->GetWidth() - out.width) / 2;
			pos = (posMin * scale_factor) + bmpViewer->GetWidth() * ((float)(100 - etape) / 100.0f);
			if (renderOpenGL != nullptr)
				renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos, out.y * scale_factor, false);

		}
		else
		{
			//xtexture = (float)(out.width) * scale_factor;
			//pos = out.x - (bmpViewer->GetWidth() * ((float)(etape) / 100.0f));
			xtexture = (float)(out.width) * scale_factor;
			pos = (((out.x + xtexture) * scale_factor) * ((float)(etape) / 100.0f)) - xtexture;
			if (renderOpenGL != nullptr)
				renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos, out.y * scale_factor, false);

		}
	}

	virtual void RenderMoveTexture(int & x, int & y, GLTexture* glTexture, const int &etape, const bool &isNext)
	{
		if (etape > 0 && etape < 110)
		{
			if (isNext)
			{
				x = x - ((float)(x + glTexture->GetWidth()) * ((float)(etape) / 100.0f));
			}
			else
			{
				x = x + ((float)(x + glTexture->GetWidth()) * ((float)(etape) / 100.0f));
			}
		}
	}

	int GetTypeFilter()
	{
		return IDM_AFTEREFFECT_MOVE;
	}

};

class CNoneEffectTextureEffect : public CBitmapFusionFilter
{
public:
	CNoneEffectTextureEffect() {};
	~CNoneEffectTextureEffect() {};

	int GetTypeFilter()
	{
		return IDM_AFTEREFFECT_NONE;
	}

	virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
	{
		bmpViewer->StopTransitionEffect(bmpSecond);
	}

};

class CDiaporamaEffect : public CBitmapFusionFilter
{
public:
	CDiaporamaEffect() {};
	~CDiaporamaEffect() {};

	int GetTypeFilter()
	{
		return IDM_DIAPORAMA_TRANSITION;
	}

	virtual void SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
	{
		bmpViewer->StartTransitionEffect(bmpSecond, true);
	}

	virtual void AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio)
	{
		if (etape < 110)
		{
			ratio = ratio + 0.0005;
			bmpViewer->CalculCenterPositionPicture();
		}
	}
};

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
	return nullptr;
}


void CBitmapWndViewer::CalculCenterPositionPicture()
{
	CalculPositionPicture(centerX, centerY);
}

void CBitmapWndViewer::SendEmail()
{
    wxString subject = CLibResource::LoadStringFromResource(L"LBLEMAILSUBJECT", 1);;
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

void CBitmapWndViewer::SetListener(IMouseUpdate * mouseUpdate)
{
	this->mouseUpdate = mouseUpdate;
}

void CBitmapWndViewer::RemoveListener()
{
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
    
	this->Refresh();
}

CEffectParameter * CBitmapWndViewer::GetParameter()
{
	return effectParameter;
}

void CBitmapWndViewer::UpdateFiltre(CEffectParameter * effectParameter)
{
	this->effectParameter = effectParameter;
    updateFilter = true;
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


void CBitmapWndViewer::PrintPicture()
{
	CRegardsBitmap * bitmap = CBitmapWnd::GetBitmap(true);
	wxWindow * mainWindow = this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_PRINTPICTURE);
		evt.SetClientData(bitmap);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CBitmapWndViewer::LoadingResource()
{
	wxColor colorToReplace = wxColor(0, 0, 0);
	wxColor colorActifReplacement = wxColor(255, 255, 255);
	arrowPrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG" , 32 * scaleFactor, 32 * scaleFactor);
	arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	arrowNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", 32 * scaleFactor, 32 * scaleFactor);
	arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

}

CBitmapWndViewer::CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface)
	: CBitmapWnd(parent, id, slider, mainViewerId, theme)
{

	mouseUpdate = nullptr;
	etape = 0;
	preview = 0;
	m_bTransition = false;
	isDiaporama = false;
	isInUse = false;
	invertColor = false;
	transitionTimer = nullptr;
	selectEffectTimer = nullptr;
	clickTimer = nullptr;
    LoadingResource();
	etape = 0;
	fixArrow = true;
	nextPicture = nullptr;
	startTransition = false;
	this->bitmapInterface = bitmapInterface;
	oldMouse.x = 0;
	oldMouse.y = 0;
	m_cDessin = nullptr;
	afterEffect = nullptr;
	transitionTimer = new wxTimer(this, TIMER_TRANSITION);
	clickTimer = new wxTimer(this, TIMER_CLICK);
	Connect(TIMER_TRANSITION, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnTransition), nullptr, this);
	Connect(TIMER_CLICK, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnClick), nullptr, this);
}

void CBitmapWndViewer::OnClick(wxTimerEvent& event)
{
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
			Refresh();
		}
	}
}

CBitmapWndViewer::~CBitmapWndViewer()
{
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	if (clickTimer->IsRunning())
		clickTimer->Stop();

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
	CSavePicture::ExportPicture(this, filename);
}

void CBitmapWndViewer::SavePicture()
{
	CRegardsBitmap * bitmap = GetBitmap(true);
	CImageLoadingFormat * imageLoading = new CImageLoadingFormat();
	imageLoading->SetPicture(bitmap);
	CSavePicture::SavePicture(nullptr, imageLoading, filename);
	if (imageLoading != nullptr)
		delete imageLoading;
}

void CBitmapWndViewer::DeterminePos(wxRect &rc, const int &nTailleAffichageWidth, const int &nTailleAffichageHeight, int &left, int &top)
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

void CBitmapWndViewer::SetBitmapPreviewEffect(const int &effect)
{
	preview = effect;
	isInUse = true;
	if(m_cDessin != nullptr)
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
		::wxSetCursor(*wxSTANDARD_CURSOR);
		toolOption = MOVEPICTURE;
	}
}

void CBitmapWndViewer::Resize()
{
	updateFilter = true;
	UpdateResized();

	if (CFiltreData::SupportMouseClick(toolOption))
	{
		if (!isInUse && m_cDessin != nullptr)
			SetDessinRatio();
	}

	//this->Refresh();
}

bool CBitmapWndViewer::ApplyPreviewEffect(int & widthOutput, int & heightOutput)
{
	if (preview > 1 && mouseUpdate != nullptr)
	{
        if(IsOpenGLDecoding())
        {
            if (!mouseUpdate->IsOpenGLCompatible())
                GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);
        }
        
		mouseUpdate->ApplyPreviewEffect(effectParameter, this, filtreEffet, m_cDessin, widthOutput, heightOutput);

        if(IsOpenGLDecoding())
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
	if(m_cDessin != nullptr)
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
    if(isDiaporama)
    {
		SetBitmap(nextPicture, false);
        nextPicture	= nullptr;
    }

	if (afterEffect != nullptr)
	{
		delete(afterEffect);
		afterEffect = nullptr;
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
void CBitmapWndViewer::SetTransitionBitmap(CImageLoadingFormat * bmpSecond)
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

	if(!bitmapLoad)
		numEffect = 0;

	if (afterEffect != nullptr)
	{
		delete(afterEffect);
		afterEffect = nullptr;
	}

	afterEffect = AfterEffectPt(numEffect);
	if (afterEffect != nullptr)
		afterEffect->SetTransitionBitmap(true, this, bmpSecond);
}

void CBitmapWndViewer::StartTransitionEffect(CImageLoadingFormat* bmpSecond, const bool &setPicture)
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

void CBitmapWndViewer::MouseRelease(const int &xPos, const int &yPos)
{
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		mouseBlock = false;
		mouseScrollX = xPos;
		mouseScrollY = yPos;
        if(HasCapture())
            ReleaseMouse();
	}

	if (clickTimer->IsRunning())
		clickTimer->Stop();
}

void CBitmapWndViewer::MouseClick(const int &xPos, const int &yPos)
{
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		int hpos = CBitmapWnd::GetHPos();
		int vpos = CBitmapWnd::GetVPos();
        
        int x = (xPos - xPosImage);
        int y = (yPos - yPosImage);
        
                
        printf("MouseClick xPos : %d xPosImage : %d \n",xPos, xPosImage);
        printf("MouseClick yPos : %d yPosImage : %d \n",yPos, yPosImage);
        
        printf("MouseClick x : %d \n",x);
        printf("MouseClick y : %d \n",y);

		invertColor = true;
		mouseBlock = true;
		CaptureMouse();
		m_cDessin->InitPoint(x, y, hpos, vpos, CBitmapWnd::GetRatio());
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

void CBitmapWndViewer::FixArrowNavigation(const bool &fix)
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


CDraw * CBitmapWndViewer::GetDessinPt()
{
	return m_cDessin;
}

void CBitmapWndViewer::SetNextPictureMove(const bool& value)
{
	isNext = value;
}

#ifdef RENDEROPENGL
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

		if (isDiaporama)
		{
			if (isDiaporama && numEffect == 0)
			{
				startTransition = false;
				bitmapInterface->TransitionEnd();
			}
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
			/*
#ifndef WIN32
			double scale_factor = GetContentScaleFactor();
#else
			double scale_factor = 1.0f;
#endif 

			if(renderOpenGL != nullptr)
				renderOpenGL->ReloadResource(scale_factor);
			*/
			renderOpenGL->ShowArrowPrevious();
			renderOpenGL->ShowArrowNext();
		}
	}
}

void CBitmapWndViewer::RenderTexture(const bool &invertPos)
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
		int numEffect = 0;

		if (isDiaporama)
			numEffect = config->GetDiaporamaTransitionEffect();
		else if (config != nullptr)
			numEffect = config->GetEffect();

		if (afterEffect != nullptr)
			afterEffect->RenderMoveTexture(x, y, glTexture,  etape,  isNext);

		bool isValid = false;
		if(afterEffect != nullptr)
			isValid = afterEffect->RenderTexture(nextPicture, source, this, renderOpenGL, scale_factor, etape);

		if(!isValid)
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

#else

void CBitmapWndViewer::LoadingResource(const double & scale_factor)
{
	wxColor colorToReplace = wxColor(0, 0, 0);
	wxColor colorActifReplacement = wxColor(255, 255, 255);
	arrowPrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG", 32 * scale_factor, 32 * scale_factor);
	arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	arrowNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", 32 * scale_factor, 32 * scale_factor);
	arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
}

void CBitmapWndViewer::ShowArrowNext(wxDC * memDC)
{
	int left = width - arrowNext.GetWidth();
	int top = (height - arrowNext.GetHeight()) / 2;
	memDC->DrawBitmap(arrowNext, left, top);

}

void CBitmapWndViewer::ShowArrowPrevious(wxDC * memDC)
{

	int left = 0;
	int top = (height - arrowPrevious.GetHeight()) / 2;
	memDC->DrawBitmap(arrowPrevious, left, top);

}

void CBitmapWndViewer::AfterRender(wxDC * dc)
{


	if(!arrowPrevious.IsOk())
		LoadingResource(scale_factor);

	if(bitmapLoad)
	{
		//Affichage de la transition
		int numEffect = 0;

		if (isDiaporama)
			numEffect = config->GetDiaporamaTransitionEffect();
		if (config != nullptr)
			numEffect = config->GetEffect();

		if (isDiaporama)
		{
			if (isDiaporama && numEffect == 0)
			{
				startTransition = false;
				bitmapInterface->TransitionEnd();
			}
		}

		if (numEffect != 0)
		{
			wxRect out;
			CRegardsBitmap * bitmapOut = afterEffect->GenerateBitmapEffect(nextPicture, etape, this, out);
			if (bitmapOut != nullptr)
			{
				CImageLoadingFormat image;
				image.SetPicture(bitmapOut);
				wxImage * imageR = image.GetwxImage();
				dc->DrawBitmap(*imageR, out.x, out.y, true);
				delete imageR;

			}
		}
	}

	if (!isDiaporama)
	{
		//Insertion dans le HBITMAP
		if (fixArrow && (etape == 0 || etape == 100))
		{
			ShowArrowPrevious(dc);
			ShowArrowNext(dc);
		}

	}
}
#endif
void CBitmapWndViewer::KeyPress(const int &key)
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

void CBitmapWndViewer::MouseMove(const int &xPos, const int &yPos)
{
#ifndef WIN32
	double scale_factor = GetContentScaleFactor();
#else
	double scale_factor = 1.0f;
#endif 


    
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		int hpos = CBitmapWnd::GetHPos();
		int vpos = CBitmapWnd::GetVPos();
        
        int x = (xPos - xPosImage);
        int y = (yPos - yPosImage);
        
    printf("MouseMove xPos : %d \n",x);
    printf("MouseMove yPos : %d \n",y);

		CFiltreData::SetCursor(toolOption);
		if (mouseBlock)
		{
			m_cDessin->MouseMove(x, y, hpos, vpos, CBitmapWnd::GetRatio());
			updateFilter = true;
			this->Refresh();
		}
		else if(CFiltreData::SupportMouseSelection(toolOption))
		{
			m_cDessin->Selection(x, y, hpos, vpos, CBitmapWnd::GetRatio());
			updateFilter = true;
			this->Refresh();
		}
	}
	else
	{
		bool isOnArrow = false;
		if (fixArrow && etape == 0)
		{
			int yPosTop = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2;
			int yPosBottom = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

			if (xPos < arrowPrevious.GetWidth() * scale_factor && (yPos > yPosTop && yPos < yPosBottom))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else if ((xPos > (GetWidth() * scale_factor - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else
			{
				wxSetCursor(*wxSTANDARD_CURSOR);
			}
		}
	}
}
