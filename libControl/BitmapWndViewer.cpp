#include "BitmapWndViewer.h"
#include <FiltreEffet.h>
#include <RgbEffectParameter.h>
#include <BrightAndContrastEffectParameter.h>
#include <CloudsEffectParameter.h>
#include <FreeRotateEffectParameter.h>
#include <MotionBlurEffectParameter.h>
#include <PhotoFiltreEffectParameter.h>
#include <PosterisationEffectParameter.h>
#include <RgbEffectParameter.h>
#include <SolarisationEffectParameter.h>
#include <LensFlareEffectParameter.h>
#include <SwirlEffectParameter.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <Crop.h>
#include <Selection.h>
#include <Histogramme.h>
#include "effect.h"
#include <BitmapInterface.h>
#include <LibResource.h>/#include "MySmtpListener.h"
#include <LoadingResource.h>
#include <libPicture.h>
#include "wx/stdpaths.h"
#include <chrono>
#include <thread>
#include <FileUtility.h>
#include <FilterData.h>

#ifdef __APPLE__
    #include <SaveFromCFunction.h>
    #include <SaveFileFormat.h>
    #include "SendEmail.h"
#elif defined(WIN32)
	#include <LocalMapi.h>
#else
    #include "email.h"
#endif
using namespace Regards::Control;

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



void CBitmapWndViewer::SendEmail()
{
#ifdef __APPLE__
    
    CSendEmail sendEmail;
    sendEmail.SendEmail("", "My Photo", bitmap->GetFilename());
    
#elif WIN32

	CMapi m_cMapi;
	vector<string> attachment;

	attachment.push_back(bitmap->GetFilename().ToStdString());
	if (attachment.size() > 0)
		m_cMapi.SendEmail("", attachment);

#else
    wxEmailMessage email("My Photo","This is my Photo \n","");
    email.AddFile(bitmap->GetFilename());
    
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxString documentPath = stdp.GetDocumentsDir();
    
    wxString emailFile = documentPath + "/temp.eml";
    
    wxFileOutputStream outputFile(emailFile);
    //filename.Assign(emailFile);
    email.Encode(outputFile);
	wxLaunchDefaultApplication(emailFile);
#endif
}


void CBitmapWndViewer::SetDiaporamaMode()
{
	isDiaporama = true;
}

void CBitmapWndViewer::SetNormalMode()
{
	isDiaporama = false;
}

void CBitmapWndViewer::PrintPicture()
{
	statusBarInterface->PrintPreview(this->bitmap);
}

void CBitmapWndViewer::LoadingResource()
{
    CLoadingResource loadingResource;
    arrowPrevious = loadingResource.LoadRegardsBmpResource("IDB_CIRCLE_PREVIOUS");
    arrowNext = loadingResource.LoadRegardsBmpResource("IDB_CIRCLE_NEXT");
    CRegardsBitmap * loadingStep = nullptr;
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP1");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP2");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP3");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP4");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP5");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP6");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP7");
    vecLoadingBitmap.push_back(loadingStep);
    loadingStep = loadingResource.LoadRegardsBmpResource("IDB_LOADING_STEP8");
    vecLoadingBitmap.push_back(loadingStep);
}


CBitmapWndViewer::CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, CWindowMain * main, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface)
	: CBitmapWnd(parent, id, slider, main, theme)
{
	
	this->statusBarInterface = statusBarInterface;
    LoadingResource();
	step = 0;
	fixArrow = true;
	//numEffect = 1;
	nextPicture = nullptr;
	startTransition = false;
	this->bitmapInterface = bitmapInterface;
	oldMouse.x = 0;
	oldMouse.y = 0;
	effectParameter = nullptr;
	m_cDessin = nullptr;
	transitionTimer = new wxTimer(this, TIMER_TRANSITION);
	Connect(TIMER_TRANSITION, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnTransition), nullptr, this);
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapWndViewer::onIdle));
}


void CBitmapWndViewer::OnTransition(wxTimerEvent& event)
{
	if (m_bTransition)
	{
		step += 10;
		if (step == 100)
		{
			step = 0;
			EndTransition();
			m_bTransition = false;
		}
		else
		{
			m_bTransition = true;
			transitionTimer->Start(50, true);
			Refresh();
		}
	}
}

void CBitmapWndViewer::onIdle(wxIdleEvent& evt)
{
	if (preview == IDM_CROP || preview == IDM_REDEYE || preview == IDM_FILTRELENSFLARE)
	{
		wxWindowDC windDC(this);
		AfterDrawBitmap(&windDC);
	}
}

CBitmapWndViewer::~CBitmapWndViewer()
{
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	delete(transitionTimer);
	delete(arrowPrevious);
	delete(arrowNext);
	delete(m_cDessin);
    
    for(CRegardsBitmap * bitmap : vecLoadingBitmap)
    {
        if(bitmap != nullptr)
            delete bitmap;
    }
}

void CBitmapWndViewer::AfterSetBitmap()
{
	preview = PREVIEW_NONE;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
}

void CBitmapWndViewer::SavePicture()
{
	CLibPicture libPicture;
	if (bitmap != nullptr)
	{
		wxString filename = bitmap->GetFilename();
		if (filename != "")
		{
			 wxString szFilter = "";
            
#ifdef __APPLE__
 
            
            SaveFileFormat saveFormat(nullptr);
            saveFormat.ShowModal();
            if(!saveFormat.IsOk())
                return;
            
            wxString extension;
            int iFormat = saveFormat.GetSaveFormat();
            switch(iFormat)
            {
                case 0:
                    iFormat = BMP;
                    extension = "bmp";
                    break;
                    
                case 1:
                    iFormat = TGA;
                    extension = "tga";
                    break;
                    
                case 2:
                    iFormat = PCX;
                    extension = "pcx";
                    break;
                    
                case 3:
                    iFormat = MNG;
                    extension = "mng";
                    break;
                    
                case 4:
                    iFormat = TIFF;
                    extension = "tif";
                    break;
                    
                case 5:
                    iFormat = PNG;
                    extension = "png";
                    break;
                    
                case 6:
                    iFormat = GIF;
                    extension = "gif";
                    break;
                    
                case 7:
                    iFormat = JPEG;
                    extension = "jpg";
                    break;
                    
                case 8:
                    iFormat = PNM;
                    extension = "pnm";
                    break;
                    
                case 9:
                    iFormat = JPC;
                    extension = "jpc";
                    break;
                    
                case 10:
                    iFormat = JPEG2000;
                    extension = "jp2";
                    break;
                    
                case 11:
                    iFormat = PPM;
                    extension = "ppm";
                    break;
                    
                case 12:
                    iFormat = XPM;
                    extension = "xpm";
                    break;
            }
           
            wxString file = saveFileProgrammaticVersion(extension.ToStdString());
            libPicture.SavePicture(file, bitmap);
            
#else
            szFilter = "Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files BPG(*.BPG)|*.bpg|Files MNG(*.MNG)|*.mng|Files PDF (*.PDF)|*.pdf|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files ICO (*.ICO)|*.ico|Files CUR (*.CUR)|*.cur|Files ANI (*.ANI)|*.ani";
            
            
            wxFileDialog saveFileDialog(this, filename, "", "",
                                        szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            
            if (saveFileDialog.ShowModal() == wxID_CANCEL)
                return;
            
            wxString file = saveFileDialog.GetPath();
            libPicture.SavePicture(file, bitmap);

#endif

            
            

		}
	}
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
	wxRect rc, rc2;
	int iLeft, iTop;
	int iTailleWidth, iTailleHeight;

	rc2.x = 0;
	rc2.y = 0;
	rc2.width = width;
	rc2.height = height;

	//GetClientRect(m_hWnd, &rc2);

	iTailleWidth = GetBitmapWidthWithRatio();
	iTailleHeight = GetBitmapHeightWithRatio();

	if (m_cDessin != nullptr)
		m_cDessin->SetRatio(ratio);

	DeterminePos(rc2, iTailleWidth, iTailleHeight, iLeft, iTop);

	rc.x = iLeft;
	rc.width = iTailleWidth;
	rc.y = iTop;
	rc.height = iTailleHeight;

	if (m_cDessin != nullptr)
		m_cDessin->SetMaxPosition(rc);
}

void CBitmapWndViewer::SetBitmapPreviewEffect(const int &effect)
{
	preview = effect;

	switch (preview)
	{
	case IDM_REDEYE:
	case IDM_CROP:
		{
			isInUse = true;
			delete(m_cDessin);
			m_cDessin = new CCrop();
			SetDessinRatio();
			wxSetCursor(wxCursor(wxCURSOR_CROSS));
			isInUse = false;
			//m_cDessin->InitPoint(0, 0, posLargeur, posHauteur, ratio);
		}
		break;
	case IDM_FILTRELENSFLARE:
	{
		isInUse = true;
		delete(m_cDessin);
		m_cDessin = new CSelection();
		SetDessinRatio();
		wxSetCursor(wxCursor(wxCURSOR_CROSS));
		isInUse = false;
		//m_cDessin->InitPoint(0, 0, posLargeur, posHauteur, ratio);
	}
	break;
	default:
		toolOption = MOVEPICTURE;
		break;
	}

	//this->Refresh();
}

void CBitmapWndViewer::Resize(const int &width, const int &height)
{
	switch (preview)
	{
	case IDM_FILTRELENSFLARE:
	case IDM_REDEYE:
	case IDM_CROP:
		if (!isInUse && m_cDessin != nullptr)
			SetDessinRatio();
		break;
	}
}

void CBitmapWndViewer::SetBitmapPreviewEffect(const int &effect, CEffectParameter * effectParameter)
{
	preview = effect;
	this->effectParameter = effectParameter;

	switch (effect)
	{
	case IDM_FILTRELENSFLARE:
	case IDM_REDEYE:
	case IDM_CROP:
		SetBitmapPreviewEffect(effect);
		break;
	}
	this->Refresh();
}

void CBitmapWndViewer::ApplyEffectWithParameter(const int &effect, CEffectParameter * effectParameter)
{
    
}

bool CBitmapWndViewer::SetBitmapEffect(const int &effect, CEffectParameter * effectParameter)
{
	CRgbaquad backColor = CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
	bool isOk = true;
	
	if (config != nullptr)
	{
		//int numLib = config->GetEffectLibrary();
		int numLib = config->GetEffectLibrary();
		CFiltreEffet * filtre = new CFiltreEffet(bitmap, backColor, numLib);
		
		if (filtre != nullptr)
		{
			switch (effect)
			{
                    
            case IDM_FILTRELENSFLARE:
				{
					if (effectParameter != nullptr)
					{
						wxPoint pt;
						m_cDessin->GetPoint(pt);
						CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
						filtre->LensFlare(pt.x, bitmap->GetBitmapHeight() - pt.y - 1, lensFlareParameter->size, 0, lensFlareParameter->brightness, lensFlareParameter->color, lensFlareParameter->colorIntensity);
					}
				}
				break;
            
            case IDM_CROP:
            {
                wxRect rcZoom;
                m_cDessin->GetPos(rcZoom);
                CRegardsBitmap * bitmapOut = new CRegardsBitmap();
                bitmap->VertFlipBuf();
                bitmapOut = bitmap->CropBitmap(rcZoom.x, rcZoom.y, rcZoom.width, rcZoom.height);
                bitmapOut->VertFlipBuf();
                SetBitmap(bitmapOut);
                break;
            }
            
            case IDM_REDEYE:
            {
                wxRect rc;
                wxRect rect;
                m_cDessin->GetPos(rc);
                bitmap->VertFlipBuf();
                filtre->RedEye(rc);
                bitmap->VertFlipBuf();
                break;
            }
            
            default:
                filtre->RenderEffect(effect, effectParameter);
                break;
            
            }
        }
        
        delete(filtre);
        
        SetBitmapPreviewEffect(MOVEPICTURE);
		
		if (isOk)
		{
			addToTexture = true;
			this->Refresh();
		}
	}
    
    
	return isOk;
}

void CBitmapWndViewer::StopTransition()
{
	m_bTransition = false;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

}

void CBitmapWndViewer::EndTransition()
{
	SetBitmap(nextPicture, false);
	startTransition = false;
	bitmapInterface->TransitionEnd();
}

//---------------------------------------------------------
//
//---------------------------------------------------------
void CBitmapWndViewer::SetTransitionBitmap(CRegardsBitmap * bmpSecond)
{
	if (step != 0)
	{
		StopTransition();
	}

	int numEffect = 0;
	
	if (isDiaporama)
		numEffect = config->GetDiaporamaTransitionEffect();
	else if (config != nullptr)
		numEffect = config->GetEffect();
	
	switch (numEffect)
	{
	case 0: //Pas d'effet
		{
			SetBitmap(bmpSecond, false);
			startTransition = false;
			bitmapInterface->TransitionEnd();
		}
		break;

	default:
		{


			if (bitmap != nullptr && !startTransition && bitmap->GetFilename() != bmpSecond->GetFilename())
			{
				startTransition = true;
				m_bTransition = true;
				nextPicture = bmpSecond;
				step = 0;
				transitionTimer->Start(50, true);
			}
			else
			{
				step = 0;
				m_bTransition = false;
				SetBitmap(bmpSecond, false);
				startTransition = false;
				//bitmapInterface->TransitionEnd();

			}
		}
		break;

	}


}

void CBitmapWndViewer::MouseRelease(const int &xPos, const int &yPos)
{
	if (toolOption == IDM_CROP || toolOption == IDM_FILTRELENSFLARE || toolOption == IDM_REDEYE)
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
	if (toolOption == IDM_CROP || toolOption == IDM_REDEYE || toolOption == IDM_FILTRELENSFLARE)
	{
		invertColor = true;
		mouseBlock = true;
		CaptureMouse();
		m_cDessin->InitPoint(xPos, yPos, posLargeur, posHauteur, ratio);
	}
	else if (fixArrow)
	{
		if (xPos < arrowPrevious->GetBitmapWidth())
		{
			bitmapInterface->ImagePrecedente();
		}
		else if (xPos >(width - arrowNext->GetBitmapWidth()))
		{
			bitmapInterface->ImageSuivante();
		}
	}
}

void CBitmapWndViewer::FixArrowNavigation(const bool &fix)
{
	fixArrow = fix;
}

void CBitmapWndViewer::AfterRenderBitmap(CRenderBitmapInterface * renderInterface)
{
	if (step == 10)
	{
		if (nextPicture != nullptr)
			renderInterface->AddTextureBitmap(nextPicture, "bitmapDestination");
	}

	int numEffect = 0;
	
	if (isDiaporama)
		numEffect = config->GetDiaporamaTransitionEffect();
	else if (config != nullptr)
		numEffect = config->GetEffect();

	switch (numEffect)
	{
	case 1:
		//TransitionEffect(renderInterface);
		if (nextPicture != nullptr)
		{
			if (!renderInterface->TestIsTextureExist("bitmapDestination"))
				renderInterface->AddTextureBitmap(nextPicture, "bitmapDestination");

			if (step == 90)
				renderInterface->Transition("bitmapDestination", 100);
			else if (step > 0)
				renderInterface->Transition("bitmapDestination", step);
		}
		break;

	default:
		break;

	}

	if (isDiaporama)
	{
		if (isDiaporama && numEffect == 0)
		{
			startTransition = false;
			bitmapInterface->TransitionEnd();
		}
	}
	else
	{

		//Insertion dans le HBITMAP
		if (fixArrow && (step == 0 || step == 90))
		{
			ShowArrowPrevious(renderInterface);
			ShowArrowNext(renderInterface);
		}
        
        if(showLoadingBitmap)
        {
            ShowLoadingBitmap(stepLoading);
        }
        
        if(CFiltreData::NeedPreview(preview))
        {
            switch (preview)
            {
                case IDM_ROTATE_FREE:
                {
                    CFreeRotateEffectParameter * freeRotate = (CFreeRotateEffectParameter *)effectParameter;
                    renderInterface->RotateFree(freeRotate->angle);
                }
                    break;
                    
                case IDM_FILTRE_MOTIONBLUR:
                {
                    CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
                    renderInterface->MotionBlur(motionBlurParameter->radius, motionBlurParameter->sigma, motionBlurParameter->angle);
                }
                    break;
                    
                case IDM_FILTRELENSFLARE:
                {
                    wxPoint pt;
                    m_cDessin->GetScreenPoint(pt);
                    CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
                    if (pt.x != 0 && pt.y != 0)
                        renderInterface->LensFlare(pt.x, pt.y, lensFlareParameter->size, 0, lensFlareParameter->brightness, lensFlareParameter->color, lensFlareParameter->colorIntensity);
                    
                }
                    break;
                    
                default:
                    renderInterface->RenderEffect(preview, effectParameter);
                    break;
                    
            }
        }
        
        
    }
    //preview = PREVIEW_NONE;
}

void CBitmapWndViewer::ShowLoadingBitmap(const int &stepLoading)
{
    CRegardsBitmap * bitmap = vecLoadingBitmap.at(0);
    if(bitmap != nullptr)
    {
        int xPos = (width - bitmap->GetBitmapWidth()) / 2;
        int yPos = (height - bitmap->GetBitmapHeight()) / 2;
        wxString loadingBitmapTextureName = "loadingbitmap" + to_string(stepLoading);
        if (!renderInterface->TestIsTextureExist(loadingBitmapTextureName))
            renderInterface->AddTextureBitmap(vecLoadingBitmap.at(stepLoading), loadingBitmapTextureName);
        
        CPhotoFiltreEffectParameter photoFiltreParameter;
        photoFiltreParameter.blue = 255;
        photoFiltreParameter.red = 255;
        photoFiltreParameter.green = 255;
        photoFiltreParameter.intensity = 50;
        renderInterface->RenderEffect(ID_AJUSTEMENT_PHOTOFILTRE, &photoFiltreParameter);
        renderInterface->AddAlphaBlendingBitmap(loadingBitmapTextureName, xPos, yPos);
    }
}

void CBitmapWndViewer::ShowArrowNext(CRenderBitmapInterface * renderInterface)
{
	int xPos = width - arrowNext->GetBitmapWidth();
	int yPos = (height - arrowNext->GetBitmapHeight()) / 2;

	if (!renderInterface->TestIsTextureExist("arrowNext"))
		renderInterface->AddTextureBitmap(arrowNext, "arrowNext");

	renderInterface->AddAlphaBlendingBitmap("arrowNext", xPos, yPos);
}

void CBitmapWndViewer::ShowArrowPrevious(CRenderBitmapInterface * renderInterface)
{
	int xPos = 0;
	int yPos = (height - arrowPrevious->GetBitmapHeight()) / 2;

	if (!renderInterface->TestIsTextureExist("arrowPrevious"))
		renderInterface->AddTextureBitmap(arrowPrevious, "arrowPrevious");

	renderInterface->AddAlphaBlendingBitmap("arrowPrevious", xPos, yPos);
}

bool CBitmapWndViewer::NeedAfterDrawBitmap()
{
    switch (preview)
    {
        case IDM_FILTRELENSFLARE:
        case IDM_REDEYE:
        case IDM_CROP:
        {
            return true;
        }
            break;
    }
    return false;
}

void CBitmapWndViewer::AfterDrawBitmap(wxDC * deviceContext)
{
	switch (preview)
	{
	case IDM_FILTRELENSFLARE:
		{
			if (!isInUse)
			{
				if (m_cDessin != nullptr)
					m_cDessin->Dessiner(deviceContext, posLargeur, posHauteur, ratio, wxColour(30, 30, 30), wxColour(30, 30, 30), wxColour(255, 255, 255), 2);
			}
		}
			break;
		case IDM_REDEYE:
		case IDM_CROP:
		{
			if (!isInUse)
			{
				if (m_cDessin != nullptr)
				{
                    m_cDessin->Dessiner(deviceContext, posLargeur, posHauteur, ratio, wxColour(0, 0, 0), wxColour(0, 0, 0), wxColour(0, 0, 0), 2);
				}
			}
		}
		break;
	}
}

void CBitmapWndViewer::KeyPress(const int &key)
{
	if (key == WXK_ESCAPE)
	{
		if (preview == IDM_CROP)
		{
			preview = PREVIEW_NONE;
			this->Refresh();
		}
	}
}

void CBitmapWndViewer::MouseMove(const int &xPos, const int &yPos)
{
	if (preview == IDM_CROP || preview == IDM_REDEYE)
	{
		::wxSetCursor(wxCursor(wxCURSOR_CROSS));
		if (!mouseBlock)
			m_cDessin->Selection(xPos, yPos, posLargeur, posHauteur, ratio);
		else
		{
			m_cDessin->MouseMove(xPos, yPos, posLargeur, posHauteur, ratio);
			this->Refresh();
		}

		
	}
	else if (preview == IDM_FILTRELENSFLARE)
	{
		if (mouseBlock)
		{
			m_cDessin->MouseMove(xPos, yPos, posLargeur, posHauteur, ratio);
			this->Refresh();
		}
	}
	else
	{
		bool isOnArrow = false;
		if (fixArrow && step == 0)
		{
			int yPosTop = (height - arrowNext->GetBitmapHeight()) / 2;
			int yPosBottom = (height - arrowNext->GetBitmapHeight()) / 2 + arrowNext->GetBitmapHeight();

			if (xPos < arrowPrevious->GetBitmapWidth() && (yPos > yPosTop && yPos < yPosBottom))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else if ((xPos >(width - arrowNext->GetBitmapWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
		}

	}


}
