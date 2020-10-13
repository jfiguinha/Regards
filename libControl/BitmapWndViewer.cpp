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

using namespace Regards::Control;
using namespace Regards::OpenCL;
using namespace Regards::OpenGL;
using namespace Regards::Window;
enum
{
	TIMER_TRANSITION = 1,
	TIMER_SELECTEFFECT = 2
};

#define TIMER_TRANSITION 1

#define PREVIEW_NONE 0
#define PREVIEW_PHOTOFITLRE 1
#define PREVIEW_RGB 2
#define PREVIEW_ROTATEFREE 3
#define PREVIEW_SWIRL 4
#define PREVIEW_LIGHTANDCONTRAST 5
#define PREVIEW_POSTERIZE 6
#define PREVIEW_SOLARIZE 7
#define PREVIEW_CLOUDS 8
#define TIMER_TRANSITION_TIME 30


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
/*
    wxEmailMessage email("My Photo","This is my Photo \n","");
    email.AddFile(filename);
    
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxString documentPath = stdp.GetDocumentsDir();
    
    wxString emailFile = documentPath + "/temp.eml";
    
    wxFileOutputStream outputFile(emailFile);
    //filename.Assign(emailFile);
    email.Encode(outputFile);
	//wxLaunchDefaultApplication(emailFile);
    wxMimeTypesManager manager;
    wxFileType *filetype=manager.GetFileTypeFromExtension("eml");
    wxString command=filetype->GetOpenCommand(emailFile);
     * */
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
	//statusBarInterface->PrintPreview(bitmap);
	//delete bitmap;
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
	//filtreraw = nullptr;
#ifdef RENDEROPENGL
	pictureNext = new GLTexture();
	pictureFirst = new GLTexture();
#endif
	mouseUpdate = nullptr;
	etape = 0;
	preview = 0;
	m_bTransition = false;
	isDiaporama = false;
	isInUse = false;
	invertColor = false;
	transitionTimer = nullptr;
	selectEffectTimer = nullptr;
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
	Connect(TIMER_TRANSITION, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnTransition), nullptr, this);
}

void CBitmapWndViewer::OnTransition(wxTimerEvent& event)
{
	int numEffect = 0;
	if (isDiaporama)
		numEffect = config->GetDiaporamaTransitionEffect();
	else if (config != nullptr)
		numEffect = config->GetEffect();

	if (m_bTransition)
	{
		if(isDiaporama)
			etape += 1;
		else
			etape += 10;
		if (etape == 110)
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

	//delete(openclEffectVideo);
	delete(transitionTimer);
	delete(m_cDessin);
#ifdef RENDEROPENGL
	delete(pictureNext);
	delete(pictureFirst);
#endif
	if (afterEffect != nullptr)
		delete afterEffect;
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
	CSavePicture::SavePicture(this, imageLoading, filename);
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
		toolOption = MOVEPICTURE;
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

void CBitmapWndViewer::ApplyPreviewEffect(int & widthOutput, int & heightOutput)
{
	if(preview > 1 && mouseUpdate != nullptr)
		mouseUpdate->ApplyPreviewEffect(effectParameter, this, filtreEffet, m_cDessin, widthOutput, heightOutput);
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
	//SetBitmap(nextPicture, false);
	startTransition = false;
	bitmapInterface->TransitionEnd();
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
		numEffect = config->GetDiaporamaTransitionEffect();
    else if (config != nullptr)
		numEffect = config->GetEffect();

	if(!bitmapLoad)
		numEffect = 0;
	
	switch (numEffect)
	{
	case 0: //Pas d'effet
		{
			SetBitmap(bmpSecond, false);
			startTransition = false;
			bitmapInterface->TransitionEnd();
		}
		break;

	case 2:
		{
			SetBitmap(bmpSecond, false);
			startTransition = true;
			m_bTransition = true;
			etape = 0;
			transitionTimer->Start(TIMER_TRANSITION_TIME, true);
		}
	break;

	case 4:
	{
		initTexture = true;
		startTransition = true;
		m_bTransition = true;
		nextPicture = bmpSecond;
		etape = 0;

		if (afterEffect != nullptr)
			delete afterEffect;

		afterEffect = CFiltreData::AfterEffectPt(IDM_AFTEREFFECT_START + numEffect);

		transitionTimer->Start(TIMER_TRANSITION_TIME, true);
		break;
	}

	default:
		{
			if (afterEffect != nullptr)
				delete afterEffect;

			afterEffect = CFiltreData::AfterEffectPt(IDM_AFTEREFFECT_START + numEffect);

			if (bitmapLoad && !startTransition && filename != bmpSecond->GetFilename())
			{
				startTransition = true;
				m_bTransition = true;
				nextPicture = bmpSecond;
				etape = 0;

#ifdef RENDEROPENGL

				if (renderOpenGL != nullptr && afterEffect != nullptr)
				{

					if (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL)
						afterEffect->GenerateTexture(pictureNext, nextPicture);
				}

#endif

				//renderNext.Destroy();
				transitionTimer->Start(TIMER_TRANSITION_TIME, true);
			}
			else
			{
				etape = 0;
				m_bTransition = false;
				SetBitmap(bmpSecond, false);
				startTransition = false;
			}
		}
		break;

	}


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
}

void CBitmapWndViewer::MouseClick(const int &xPos, const int &yPos)
{
#ifndef WIN32
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif
    
     printf("xPos : %d width : %d arrow : %d",xPos, GetWidth(),arrowNext.GetWidth());

	if (CFiltreData::SupportMouseClick(toolOption))
	{
		int hpos = CBitmapWnd::GetHPos();
		int vpos = CBitmapWnd::GetVPos();

		invertColor = true;
		mouseBlock = true;
		CaptureMouse();
		m_cDessin->InitPoint(xPos - xPosImage, yPos - yPosImage, hpos, vpos, CBitmapWnd::GetRatio());
	}
	else if (fixArrow)
	{

		int yPosTop = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2;
		int yPosBottom = (GetHeight() * scale_factor - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

		if (xPos < arrowPrevious.GetWidth() && (yPos > yPosTop && yPos < yPosBottom))
		{
			bitmapInterface->ImagePrecedente();
		}
		else if ((xPos > (GetWidth() * scale_factor - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
		{
			bitmapInterface->ImageSuivante();
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

int CBitmapWndViewer::IsSupportOpenCL()
{
	int supportOpenCL = 0;
	CRegardsConfigParam* config = CParamInit::getInstance();
	if (config != nullptr)
		supportOpenCL = config->GetIsOpenCLSupport();

	return supportOpenCL;
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

		switch (numEffect)
		{
			case 2:
			{
				if (etape < 100)
				{
					ratio = ratio + 0.0005;
					CalculPositionPicture(centerX, centerY);
				}
				break;
			}

			case 4:
				break;


			case 3:
			{
				if (etape > 0 && etape < 110)
				{
					wxRect out;
					//Génération de la texture
					if (renderOpenGL != nullptr && nextPicture != nullptr)
					{

						if (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL)
						{
							afterEffect->GenerateBitmapOpenCLEffect(pictureNext, nextPicture, this, out);
						}
						else
						{
							CRegardsBitmap * bitmapOut = afterEffect->GenerateBitmapEffect(nextPicture, this, out);
							if (bitmapOut != nullptr)
							{
								pictureNext->Create(bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight(), bitmapOut->GetPtBitmap());
								delete bitmapOut;
							}
						}

						int xtexture = (float)(this->GetWidth() - (out.x * scale_factor));
						int pos = (out.x * scale_factor) + xtexture * ((float)(100 - etape) / 100.0f);
						if (renderOpenGL != nullptr)
							renderOpenGL->ShowSecondBitmap(pictureNext, out.width  * scale_factor, out.height * scale_factor, pos, out.y * scale_factor);
					}
				}
				break;
			}

			default:
			{
				if (numEffect != 0 && etape < 110 && nextPicture != nullptr && afterEffect != nullptr)
				{
					wxRect out;

					if (openclContext->IsSharedContextCompatible() && filtreEffet->GetLib() == LIBOPENCL)
					{
						afterEffect->GenerateBitmapOpenCLEffect(pictureNext, nextPicture, this, out);
					}
					else
					{
						CRegardsBitmap * bitmapOut = afterEffect->GenerateBitmapEffect(nextPicture, this, out);
						if (bitmapOut != nullptr)
						{
							pictureNext->Create(bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight(), bitmapOut->GetPtBitmap());
							delete bitmapOut;
						}
					}

					if (renderOpenGL != nullptr)
						renderOpenGL->ShowSecondBitmapWithAlpha(pictureNext, etape, out.width  * scale_factor, out.height * scale_factor, out.x * scale_factor, out.y * scale_factor);
				}
				break;
			}

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


		switch (numEffect)
		{
			case 3:
				if(etape < 100)
					x = x - ((float)(x + glTexture->GetWidth()) * ((float)(etape) / 100.0f));
				break;
		}

		if (numEffect == 4 && (etape > 0 && etape < 110))
		{
			wxRect out;
			{
				bool init = false;
				if (bitmapNext == nullptr)
				{
					bitmapNext = new CRegardsBitmap(GetWidth(), GetHeight());
					init = true;
				}
				else if (initTexture || (bitmapNext->GetBitmapWidth() != GetWidth() && bitmapNext->GetBitmapHeight() != GetHeight()))
				{
					delete bitmapNext;
					bitmapNext = new CRegardsBitmap(GetWidth(), GetHeight());
					init = true;
				}

				if (init)
				{
					bitmapNext->SetBackgroundColor(CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue(), 255));
					CRegardsBitmap * bitmapOut = afterEffect->GenerateBitmapEffect(nextPicture, this, out);
					if (bitmapOut != nullptr)
						bitmapNext->InsertBitmap(bitmapOut, out.x, out.y);
					delete bitmapOut;
					pictureNext->Create(bitmapNext->GetBitmapWidth(), bitmapNext->GetBitmapHeight(), bitmapNext->GetPtBitmap());
				}
			}

			//Show First Bitmap
			{
				bool init = false;
				if (bitmapFirst == nullptr)
				{
					bitmapFirst = new CRegardsBitmap(GetWidth(), GetHeight());
					init = true;
				}
				else if (initTexture || (bitmapFirst->GetBitmapWidth() != GetWidth() && bitmapFirst->GetBitmapHeight() != GetHeight()))
				{
					delete bitmapFirst;
					bitmapFirst = new CRegardsBitmap(GetWidth(), GetHeight());
					init = true;
				}

				if (init)
				{
					bitmapFirst->SetBackgroundColor(CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue(), 255));
					CRegardsBitmap * bitmapOut = afterEffect->GenerateBitmapEffect(source, this, out);
					if (bitmapOut != nullptr)
						bitmapFirst->InsertBitmap(bitmapOut, out.x, out.y);
					delete bitmapOut;
					pictureFirst->Create(bitmapFirst->GetBitmapWidth(), bitmapFirst->GetBitmapHeight(), bitmapFirst->GetPtBitmap());
				}
			}
			//renderOpenGL->ShowSecondBitmap(pictureNext,  GetWidth(), GetHeight(), 0, 0);
			renderOpenGL->RenderWithPageCurl(pictureFirst, pictureNext, etape, false, GetWidth(), GetHeight(), 0, 0);
			initTexture = false;
		}
		else
			renderOpenGL->RenderToScreen(mouseUpdate, effectParameter, x, y, invertPos);


		

		xPosImage = x;
		yPosImage = y;
	}
}

void CBitmapWndViewer::DeleteTexture()
{
    if(pictureNext != nullptr)
        pictureNext->Delete();
	if (pictureFirst != nullptr)
		pictureFirst->Delete();
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

		CFiltreData::SetCursor(toolOption);
		if (mouseBlock)
		{
			m_cDessin->MouseMove(xPos - xPosImage, yPos - yPosImage, hpos, vpos, CBitmapWnd::GetRatio());
			updateFilter = true;
			this->Refresh();
		}
		else if(CFiltreData::SupportMouseSelection(toolOption))
		{
			m_cDessin->Selection(xPos - xPosImage, yPos - yPosImage, hpos, vpos, CBitmapWnd::GetRatio());
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
		}
	}
}
