#include "BitmapWndViewer.h"
#include <ConvertUtility.h>
#include <OpenCLEffect.h>
#include <FiltreEffet.h>
#include <RgbEffectParameter.h>
#include <BrightAndContrastEffectParameter.h>
#include <CloudsEffectParameter.h>
#include <FreeRotateEffectParameter.h>
#include <MotionBlurEffectParameter.h>
#include <PhotoFiltreEffectParameter.h>
#include <PosterisationEffectParameter.h>
#include <RgbEffectParameter.h>
#include <DecodeRawPicture.h>
#include <SolarisationEffectParameter.h>
#include <LensFlareEffectParameter.h>
#include <SwirlEffectParameter.h>
#include <WaveEffectParameter.h>
#include <GaussianBlurEffectParameter.h>
#include <DecodeRawParameter.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <Crop.h>
#include <SavePicture.h>
#include <Selection.h>
#include <Histogramme.h>
#include <BitmapInterface.h>
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include <MetadataExiv2.h>
//#include "MySmtpListener.h"
#include <LoadingResource.h>
#include <libPicture.h>
#include <InterpolationBicubic.h>
#include "wx/stdpaths.h"
#include <FileUtility.h>
#include <FilterData.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>
#include <RegardsFloatBitmap.h>
#include <PiccanteFilter.h>
#include <RegardsBitmap.h>
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
#include <utility.h>
#if defined(__WXMSW__)
	#include "../include/config_id.h"
#else
	#include <config_id.h>
#endif
using namespace Regards::Control;
using namespace Regards::OpenCL;
using namespace Regards::exiv2;
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
#ifdef __APPLE__
    
    CSendEmail sendEmail;
    sendEmail.SendEmail("", "My Photo", CConvertUtility::ConvertToStdString(filename));
    
#elif defined(WIN32)

	CMapi m_cMapi;
	vector<string> attachment;

    
	attachment.push_back(CConvertUtility::ConvertToStdString(filename));
	if (attachment.size() > 0)
		m_cMapi.SendEmail("", attachment);

#else
    wxEmailMessage email("My Photo","This is my Photo \n","");
    email.AddFile(filename);
    
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxString documentPath = stdp.GetDocumentsDir();
    
    wxString emailFile = documentPath + "/temp.eml";
    
    wxFileOutputStream outputFile(emailFile);
    //filename.Assign(emailFile);
    email.Encode(outputFile);
	wxLaunchDefaultApplication(emailFile);
#endif
}

CEffectParameter * CBitmapWndViewer::GetParameter()
{
	return nullptr;
}

void CBitmapWndViewer::UpdateFiltre(CEffectParameter * effectParameter)
{
    renderFilterUpdate = true;
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
}

void CBitmapWndViewer::PrintPicture()
{
	CRegardsBitmap * bitmap = GetBitmap(true);
	statusBarInterface->PrintPreview(bitmap);
	//delete bitmap;
}

void CBitmapWndViewer::LoadingResource()
{
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif    
	wxColor colorToReplace = wxColor(0, 0, 0);
	wxColor colorActifReplacement = wxColor(255, 255, 255);
	arrowPrevious = CreateFromSVG(CLibResource::GetVector("IDB_ARROWLPNG") , 32 * scale_factor, 32 * scale_factor);
	arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	arrowNext = CreateFromSVG(CLibResource::GetVector("IDB_ARROWRPNG"), 32 * scale_factor, 32 * scale_factor);
	arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

}


wxImage CBitmapWndViewer::CreateFromSVG(const wxString & vector, const int & buttonWidth, const int & buttonHeight)
{
	wxImage img;
	if (vector.size() > 0)
	{
		wxStringInputStream memBuffer(vector);
		wxSVGDocument svgDoc;
		svgDoc.Load(memBuffer);
		img = svgDoc.Render(buttonWidth, buttonHeight, NULL, true, true);
	}
	else
	{
		img.Create(buttonWidth, buttonHeight);
	}
	return img;
}


CBitmapWndViewer::CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface, IStatusBarInterface * statusBarInterface)
	: CBitmapWnd(parent, id, slider, mainViewerId, theme)
{
	filtreraw = nullptr;
	cl_nextPicture = nullptr;
	pictureNext = nullptr;
	openclEffectVideo = nullptr;
	etape = 0;
	//Preview Parameter
	preview = 0;
	renderPreviewBitmap = nullptr;
	rawDecoder = nullptr;
	//Thread Parameter
	m_bTransition = false;
	isDiaporama = false;
	isInUse = false;
	invertColor = false;
	transitionTimer = nullptr;
	selectEffectTimer = nullptr;
	this->statusBarInterface = statusBarInterface;
    LoadingResource();
	etape = 0;
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

void CBitmapWndViewer::CreateContext()
{
	renderPreviewBitmap = new CRenderPreviewBitmap();
	openclEffectVideo = new COpenCLEffectVideo(this->openclContext);
}

void CBitmapWndViewer::OnTransition(wxTimerEvent& event)
{
	if (m_bTransition)
	{
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

void CBitmapWndViewer::onIdle(wxIdleEvent& evt)
{
	/*
	if (preview == IDM_CROP || preview == IDM_REDEYE || preview == IDM_FILTRELENSFLARE)
	{
		wxWindowDC windDC(this);
		AfterDrawBitmap(&windDC);
	}
	*/
}

CBitmapWndViewer::~CBitmapWndViewer()
{
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	delete(openclEffectVideo);
	delete(transitionTimer);
	delete(m_cDessin);
	delete(pictureNext);
	if(filtreraw != nullptr)
		delete filtreraw;
	//if(rawBitmap != nullptr)
	//	delete rawBitmap;

	if (renderPreviewBitmap != nullptr)
		delete renderPreviewBitmap;


	if(rawDecoder != nullptr)
	{
		delete rawDecoder;
	}
}

void CBitmapWndViewer::AfterSetBitmap()
{
    renderFilterUpdate = true;
    updateFilter = true;
	preview = PREVIEW_NONE;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();

	if(rawDecoder != nullptr)
	{
		delete rawDecoder;
		rawDecoder = nullptr;
	}

}

void CBitmapWndViewer::SavePicture()
{
	CRegardsBitmap * bitmap = GetBitmap(true);
	CSavePicture::SavePicture(this, bitmap, filename);
	if (bitmap != nullptr)
		delete bitmap;
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
	case IDM_WAVE_EFFECT:
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

	//this->FastRefresh(this);
}

void CBitmapWndViewer::Resize()
{
	switch (preview)
	{
	case IDM_WAVE_EFFECT:
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
	case IDM_WAVE_EFFECT:
	case IDM_FILTRELENSFLARE:
	case IDM_REDEYE:
	case IDM_CROP:
		SetBitmapPreviewEffect(effect);
		break;
	}
    RefreshWindow();
}

void CBitmapWndViewer::ApplyEffect(const int &effect)
{
	if(CFiltreData::NeedPreview(effect))
	{
		if(!CFiltreData::IsOpenCLPreviewCompatible(effect))
			this->isOpenGL = false;
	}
}

void CBitmapWndViewer::OnFiltreOk()
{
	this->isOpenGL = true;
    RefreshWindow();
}

void CBitmapWndViewer::OnFiltreCancel()
{
	this->isOpenGL = true;
    RefreshWindow();
}

bool CBitmapWndViewer::SetBitmapEffect(const int &effect, CEffectParameter * effectParameter)
{
	CRgbaquad backColor = CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
   
	//bool isOk = true;
	
	if (config != nullptr)
	{
		//int numLib = config->GetEffectLibrary();	
		if (filtreEffet != nullptr)
		{
			switch (effect)
			{
				case IDM_WAVE_EFFECT:
				{
					CRegardsBitmap * bitmap = GetBitmap(true);
					if (effectParameter != nullptr && bitmap != nullptr)
					{
                        CImageLoadingFormat image;
                        image.SetPicture(bitmap);                        
                        CFiltreEffet * filtre = new CFiltreEffet(backColor, this, nullptr, &image);
						wxPoint pt;
						m_cDessin->GetPoint(pt);
						CWaveEffectParameter * waveEffectParameter = (CWaveEffectParameter *)effectParameter;
						short height = waveEffectParameter->height;
						int radius = waveEffectParameter->radius;
						int scale = waveEffectParameter->scale;
						filtre->WaveFilter(pt.x, pt.y, height, radius, scale);

						CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
						imageLoad->SetPicture(filtre->GetBitmap(true));
						SetBitmap(imageLoad, true);
                        
                        delete filtre;
					}
				}
				break;
                    
				case IDM_FILTRELENSFLARE:
					{
						CRegardsBitmap * bitmap = GetBitmap(true);
						if (effectParameter != nullptr && bitmap != nullptr)
						{
                            CImageLoadingFormat image;
                            image.SetPicture(bitmap);                        
                            CFiltreEffet * filtre = new CFiltreEffet(backColor, this, nullptr, &image);
							
                            wxPoint pt;
							m_cDessin->GetPoint(pt);
							CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
							float puissance = (float)lensFlareParameter->size;
							float brightness = (float)lensFlareParameter->brightness ;
							float colorIntensity = (float)lensFlareParameter->colorIntensity;
							puissance = ((float)(bitmap->GetBitmapWidth() / 4) * ((float)puissance / 100.0f));
							filtre->LensFlare(pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, colorIntensity);

                            CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
                            imageLoad->SetPicture(filtre->GetBitmap(true));
                            SetBitmap(imageLoad, true);
                            
                            delete filtre;
						}
					}
					break;
            
				case IDM_CROP:
				{
					wxRect rcZoom;
					m_cDessin->GetPos(rcZoom);
					CRegardsBitmap * bitmap = GetBitmap(true);
					if(bitmap != nullptr)
					{
						CRegardsBitmap * bitmapOut = new CRegardsBitmap();
						bitmap->VertFlipBuf();
						bitmapOut = bitmap->CropBitmap(rcZoom.x, rcZoom.y, rcZoom.width, rcZoom.height);
						bitmapOut->SetOrientation(bitmap->GetOrientation());
						bitmapOut->VertFlipBuf();

						CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
						imageLoad->SetPicture(bitmapOut);
						SetBitmap(imageLoad);
						delete bitmap;
					}
					break;
				}
            
				case IDM_REDEYE:
				{
					CRegardsBitmap * bitmap = GetBitmap(true);
					if (bitmap != nullptr)
					{
                        CImageLoadingFormat image;
                        image.SetPicture(bitmap);                        
                        CFiltreEffet * filtre = new CFiltreEffet(backColor, this, nullptr, &image);
						wxRect rc;
						wxRect rect;
						m_cDessin->GetPos(rc);
						//bitmap->VertFlipBuf();
						filtre->RedEye(rc);
						//bitmap->VertFlipBuf();

                        CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
                        imageLoad->SetPicture(filtre->GetBitmap(true));
                        SetBitmap(imageLoad, true);
                        
                        delete filtre;
					}
					break;
				}

				case IDM_DECODE_RAW:
					{
						CImageLoadingFormat * bitmapLocal = rawDecoder->GetPicture();
						if(bitmapLocal != nullptr)
						{
							bitmapLocal->Flip();
							int iPos = filename.find(".");
							wxString out = filename.substr(0,iPos) + ".bmp";
							bitmapLocal->SetFilename(out);
							SetBitmap(bitmapLocal, true);
						}
					}
					break; 
            
				default:
					{
						if(CFiltreData::IsOpenCLCompatible(effect))
						{
							filtreEffet->SetPreview(false);
							filtreEffet->RenderEffect(effect, effectParameter);
							CRegardsBitmap * bitmap = GetBitmap(true);
							if (bitmap != nullptr)
							{
								CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
								imageLoad->SetPicture(bitmap);
								imageLoad->SetOrientation(orientation);
								SetBitmap(imageLoad);
							}
						}
						else if(CFiltreData::IsPiccanteCompatible(effect))
                        {
                            CRegardsFloatBitmap * test = filtreEffet->GetFloatBitmap(true);
                            printf("Infos Float Bitmap Width : %d Height : %d \n",test->GetWidth(), test->GetHeight());                            
                            
                            switch(effect)
                            {
                                case IDM_BEST_EXPOSURE:
                                    CPiccanteFilter::BestExposure(test);
                                    break;       
                                case IDM_FILTER_KUWAHARA:
                                    CPiccanteFilter::FilterKuwahara(test);
                                    break;
                                case IDM_FILTER_BILATERAL2DS: 
                                    CPiccanteFilter::FilterBilateral2DS(test);
                                    break;                                
                            }
                                    
                            CImageLoadingFormat imageLoadFormat(true);
                            imageLoadFormat.SetPicture(test);
							imageLoadFormat.SetOrientation(orientation);
                            filtreEffet->SetBitmap(&imageLoadFormat);                            
                        }
                        else
						{
							CRegardsBitmap * bitmap = GetBitmap(true);
							if (bitmap != nullptr)
							{
                                CImageLoadingFormat image;
                                image.SetPicture(bitmap);                        
                                CFiltreEffet * filtre = new CFiltreEffet(backColor, this, nullptr, &image);
								filtre->RenderEffect(effect, effectParameter);

                                CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
                                imageLoad->SetPicture(filtre->GetBitmap(true));
								imageLoad->SetOrientation(orientation);
                                SetBitmap(imageLoad, true);
                                
                                delete filtre;
							}
							
						}
					}
					break; 
			}
        }
              
        SetBitmapPreviewEffect(effect);
        RefreshWindow();
	}
      
	return 0;
}

void CBitmapWndViewer::StopTransition()
{
	m_bTransition = false;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
    
   // this_thread::sleep_for(chrono::milliseconds(50));
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
	
	//if (isDiaporama)
	//	numEffect = config->GetDiaporamaTransitionEffect();
    if (config != nullptr)
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

	default:
		{


			if (bitmapLoad && !startTransition && filename != bmpSecond->GetFilename())
			{
				startTransition = true;
				m_bTransition = true;
				nextPicture = bmpSecond;
				etape = 0;
				renderNext.Destroy();
				transitionTimer->Start(TIMER_TRANSITION_TIME, true);
			}
			else
			{
				etape = 0;
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
	if (toolOption == IDM_CROP || toolOption == IDM_WAVE_EFFECT || toolOption == IDM_FILTRELENSFLARE || toolOption == IDM_REDEYE)
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
     printf("xPos : %d width : %d arrow : %d",xPos,width,arrowNext.GetWidth());
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif 
	if (toolOption == IDM_CROP || toolOption == IDM_WAVE_EFFECT || toolOption == IDM_REDEYE || toolOption == IDM_FILTRELENSFLARE)
	{
		invertColor = true;
		mouseBlock = true;
		CaptureMouse();
		m_cDessin->InitPoint(xPos, yPos, posLargeur, posHauteur, ratio);
	}
	else if (fixArrow)
	{
       
		if (xPos < arrowPrevious.GetWidth() * scale_factor)
		{
			bitmapInterface->ImagePrecedente();
		}
		else if (xPos >(width - arrowNext.GetWidth() * scale_factor))
		{
            
			bitmapInterface->ImageSuivante();
		}
	}
}

void CBitmapWndViewer::FixArrowNavigation(const bool &fix)
{
	fixArrow = fix;
}

bool CBitmapWndViewer::NeedAfterRenderBitmap()
{
	int numEffect = 0;

	//if (isDiaporama)
	//	numEffect = config->GetDiaporamaTransitionEffect();
	if (config != nullptr)
		numEffect = config->GetEffect();

	if ((numEffect == 1 && (etape > 0 && etape < 100)) || CFiltreData::NeedPreview(preview))
		return true;
	return false;
}

void CBitmapWndViewer::ApplyPreviewEffect()
{
    filtreEffet->RenderEffectPreview(preview, effectParameter);
}

int CBitmapWndViewer::GetRawBitmapWidth()
{
	if(preview == IDM_DECODE_RAW)
		return rawWidth;
	return bitmapwidth;
}

int CBitmapWndViewer::GetRawBitmapHeight()
{
	if(preview == IDM_DECODE_RAW)
		return rawHeight;
	return bitmapheight; 
}
int CBitmapWndViewer::GetOrientation()
{
	if(preview == IDM_DECODE_RAW)
		return 4;
	return orientation; 
}

wxImage CBitmapWndViewer::RenderBitmap(wxDC * deviceContext)
{  
	wxImage render;
	if (!isDiaporama)
	{
		if(preview == IDM_DECODE_RAW)
		{
			CDecodeRawParameter * rawParameter = (CDecodeRawParameter *)effectParameter;
			if(rawParameter->update || rawDecoder == nullptr)
			{
				if(rawDecoder == nullptr)
				{
					rawDecoder = new CDecodeRawPicture(CConvertUtility::ConvertToStdString(filename));

				}
				
				if(rawDecoder->DecodePicture(rawParameter) == 0)
				{	

                    printf("RenderBitmap ratio %f\n",  ratio);	
                    
                    CImageLoadingFormat * picture = rawDecoder->GetPicture();
                    
					CRgbaquad color;

					if(filtreraw != nullptr)
                        delete filtreraw;
                    
                    filtreraw = new CFiltreEffet(color, this, openclContext, picture);
                    
					rawWidth = picture->GetWidth();
					rawHeight = picture->GetHeight();
                    
                    if (shrinkImage)
                    {
                        ratio = CalculRatio(GetBitmapWidth(), GetBitmapHeight());
                    }                
                    
                    printf("RenderBitmap ratio %f\n",  ratio);

				}

				rawParameter->update = false;

			}
			
			if (filtreraw != nullptr)
			{
                
				int widthOutput = int(GetBitmapWidthWithRatio());
				int heightOutput = int(GetBitmapHeightWithRatio());
                printf("widthOutput : %d and heightOutput %d ratio %f\n",  widthOutput, heightOutput, ratio);
				this->GenerateScreenBitmap(filtreraw, widthOutput, heightOutput);
				render = filtreraw->GetwxImage();
			}
		}
		else if (CFiltreData::NeedPreview(preview) && updateFilter)
		{
            updateFilter = false;
            int widthOutput = int(GetBitmapWidthWithRatio());
            int heightOutput = int(GetBitmapHeightWithRatio());
            
            if(CFiltreData::NeedOriginalPreview(preview))
            {
                if(renderFilterUpdate)
                {
                    CImageLoadingFormat newbitmap;
                    newbitmap.SetPicture(source->GetRegardsBitmap(true));
                    renderPreviewBitmap->SetNewBitmap(&newbitmap, this, nullptr);  
                }
            }
            else
            {

                this->GenerateScreenBitmap(filtreEffet, widthOutput, heightOutput);
                
                CImageLoadingFormat newbitmap;
                newbitmap.SetPicture(filtreEffet->GetBitmap(false));
                renderPreviewBitmap->SetNewBitmap(&newbitmap, this, nullptr);                
            }


			switch (preview)
			{
				case IDM_ROTATE_FREE:
				{
					CFreeRotateEffectParameter * freeRotate = (CFreeRotateEffectParameter *)effectParameter;
					renderPreviewBitmap->RotateFree(freeRotate->angle);
				}
				break;

				case IDM_FILTRE_MOTIONBLUR:
				{
					CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;
					renderPreviewBitmap->MotionBlur(motionBlurParameter->radius, motionBlurParameter->sigma, motionBlurParameter->angle);
				}
				break;

				case IDM_WAVE_EFFECT:
				{
					wxPoint pt;
					m_cDessin->GetScreenPoint(pt);
					if (pt.x == 0 && pt.y == 0)
					{
						pt.x = widthOutput / 4;
						pt.y = heightOutput / 4;
					}

					CWaveEffectParameter * waveEffectParameter = (CWaveEffectParameter *)effectParameter;
					if (pt.x != 0 && pt.y != 0)
					{
						int left = 0, top = 0;
						if (width > widthOutput)
							left = ((width - widthOutput) / 2);
						else
							left = 0;

						if (height > heightOutput)
							top = ((height - heightOutput) / 2);
						else
							top = 0;
						short height = waveEffectParameter->height;
						int radius = waveEffectParameter->radius;
						int scale = waveEffectParameter->scale;
						renderPreviewBitmap->WaveFilter(pt.x, pt.y, height, radius, scale, left, top);
					}
				}
					break;

				case IDM_FILTRELENSFLARE:
				{
					wxPoint pt;
					m_cDessin->GetScreenPoint(pt);
					if (pt.x == 0 && pt.y == 0)
					{
						pt.x = widthOutput / 4;
						pt.y = heightOutput / 4;
					}

					CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;
					if (pt.x != 0 && pt.y != 0)
					{
						int left = 0, top = 0;
						if (width > widthOutput)
							left = ((width - widthOutput) / 2);
						else
							left = 0;

						if (height > heightOutput)
							top = ((height - heightOutput) / 2);
						else
							top = 0;
						int puissance = (float)lensFlareParameter->size;
						int brightness = (float)lensFlareParameter->brightness;
						int radius = (float)lensFlareParameter->colorIntensity;
						renderPreviewBitmap->LensFlare(pt.x, pt.y, puissance, 0, brightness, lensFlareParameter->color, radius, left, top);
					}

				}
				break;

				default:
                    if(CFiltreData::NeedOriginalPreview(preview) && renderFilterUpdate)
					{
                        renderPreviewBitmap->RenderEffect(preview, effectParameter);
                        renderFilterUpdate = false;
                    }
					break;

			}
            
             if(CFiltreData::NeedOriginalPreview(preview))
             {
                 this->GenerateScreenBitmap(renderPreviewBitmap->GetFiltre(), widthOutput, heightOutput);
             }
            
			render = renderPreviewBitmap->GetRender();
		}
        else
        {
            render = renderPreviewBitmap->GetLastRender();
        }
	}

	return render;
}

void CBitmapWndViewer::AfterRender()
{
	if(bitmapLoad)
	{
		//Affichage de la transition
		int numEffect = 0;

		//if (isDiaporama)
		//	numEffect = config->GetDiaporamaTransitionEffect();
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


		switch (numEffect)
		{
		case 1:
			if(renderOpenGL != nullptr)
			{
				cl_int err;
                //int width = 0;
               // int height = 0;

				if(pictureNext == nullptr)
					pictureNext = new GLTexture();

				if (nextPicture != nullptr && etape == 0)
				{
                    if(openclContext->IsSharedContextCompatible()  && filtreEffet->GetLib() == LIBOPENCL)
                    {
                        if(cl_nextPicture != nullptr)
                        {
                            err = clReleaseMemObject(cl_nextPicture);
                            Error::CheckError(err);
                        }

                        CRegardsBitmap * bitmapTemp = nextPicture->GetRegardsBitmap();

                        //CMetadataExiv2 metadata(nextPicture->GetFilename());
                        int orientation = nextPicture->GetOrientation();
                        bitmapTemp->RotateExif(orientation);                        
                        pictureNext->Create(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight(), bitmapTemp->GetPtBitmap());
                        glBindTexture( GL_TEXTURE_2D, pictureNext->GetTextureID() );  
                        //cl_nextPicture = clCreateFromGLTexture2D(openclContext->GetContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, pictureNext->GetTextureID(), &err);
                        cl_nextPicture = clCreateFromGLTexture(openclContext->GetContext(), CL_MEM_READ_WRITE, GL_TEXTURE_2D, 0, pictureNext->GetTextureID(), &err);
                        delete bitmapTemp;
                    } 
                    else
                    {
                        //CRgbaquad color;
                        //COpenCLEffect openclEffect(color, openclContext);
                        CRegardsBitmap * bitmapTemp = nextPicture->GetRegardsBitmap();
                        int orientation = nextPicture->GetOrientation();
                        bitmapTemp->RotateExif(orientation);
                        bitmapTemp->SetAlphaValue(etape);
                        pictureNext->Create(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight(), bitmapTemp->GetPtBitmap());
                        glBindTexture( GL_TEXTURE_2D, pictureNext->GetTextureID() ); 
                        delete bitmapTemp;
                    }
				}
				else if(nextPicture != nullptr && etape > 0)
				{
                     if(openclContext->IsSharedContextCompatible()  && filtreEffet->GetLib() == LIBOPENCL)
                    {
                        cl_int err ;
                        err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
                        Error::CheckError(err);
                        openclEffectVideo->SetAlphaValue(cl_nextPicture, pictureNext->GetWidth(), pictureNext->GetHeight(), etape);
                        err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
                        Error::CheckError(err);
                        err = clFlush(openclContext->GetCommandQueue());
                        Error::CheckError(err);
                    }
				}

				if (nextPicture != nullptr && pictureNext != nullptr && etape > 0)
				{

                    if(openclContext->IsSharedContextCompatible()  && filtreEffet->GetLib() == LIBOPENCL)
                    {
                        try
                        {
                            cl_int err;
                            err = clEnqueueAcquireGLObjects(openclContext->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
                            Error::CheckError(err);
                            openclEffectVideo->GetRgbaBitmap(cl_nextPicture);
                            err = clEnqueueReleaseGLObjects(openclContext->GetCommandQueue(), 1, &cl_nextPicture, 0, 0, 0);
                            Error::CheckError(err);
                            err = clFlush(openclContext->GetCommandQueue());
                            Error::CheckError(err);
                        }
                        catch(...)
                        {

                        }
                    }


					float newRatio = CalculPictureRatio(pictureNext->GetWidth(), pictureNext->GetHeight());

					int widthOutput = pictureNext->GetWidth() * newRatio;
					int heightOutput = pictureNext->GetHeight() * newRatio;
					int left = (width - widthOutput) / 2;
					int top = (height - heightOutput) / 2;		

					renderOpenGL->ShowTransitionBitmap(pictureNext, widthOutput, heightOutput, left, top, etape);
				}
			}
			break;

		default:
			break;

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

void CBitmapWndViewer::AfterRenderBitmap(wxDC * deviceContext)
{
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
	case 1:
		//TransitionEffect(renderInterface);
		if (nextPicture != nullptr && etape > 0 && etape < 100)
		{
			float newRatio = CalculPictureRatio(nextPicture->GetWidth(), nextPicture->GetHeight());

			int widthOutput = nextPicture->GetWidth() * newRatio;
			int heightOutput = nextPicture->GetHeight() * newRatio;

			if (!(renderNext.GetWidth() == widthOutput && renderNext.GetHeight() == heightOutput))
			{
				CRgbaquad backColor = CRgbaquad(themeBitmap.colorBack.Red(), themeBitmap.colorBack.Green(), themeBitmap.colorBack.Blue());
				CFiltreEffet filtreEffet(backColor, this, openclContext, nextPicture);
				filtreEffet.Interpolation(widthOutput, heightOutput, 0, 0, 0, 0);
				renderNext = filtreEffet.GetwxImage();
				int size = width*height;
				unsigned char * alphaOut = new unsigned char[size];
				renderNext.SetAlpha(alphaOut);
				//delete bitmap;
			}

			int left = (width - renderNext.GetWidth()) / 2;
			int  top = (height - renderNext.GetHeight()) / 2;
			
			if (etape > 0)
			{
				//render = renderPreviewBitmap->Transition(nextPicture, width, height, step);
				unsigned char * alphaOut = renderNext.GetAlpha();
				int size = width*height;
				for (int i = 0; i < size; ++i)
					alphaOut[i] = (((float)etape / 100.0f) * 255.0f);
			}

			deviceContext->DrawBitmap(renderNext, left, top);
				
	
		}
		else
		{
			renderNext.Destroy();
		}
		break;

	default:
		break;

	}

	if (!isDiaporama)
	{
		//Insertion dans le HBITMAP
		if (fixArrow && (etape == 0 || etape == 90))
		{
			ShowArrowPrevious(deviceContext);
			ShowArrowNext(deviceContext);
		}
	}
}


void CBitmapWndViewer::ShowArrowNext(wxDC * deviceContext)
{
	int xPos = width - arrowNext.GetWidth();
	int yPos = (height - arrowNext.GetHeight()) / 2;
	deviceContext->DrawBitmap(arrowNext, xPos, yPos);
}

void CBitmapWndViewer::ShowArrowPrevious(wxDC * deviceContext)
{
	int xPos = 0;
	int yPos = (height - arrowPrevious.GetHeight()) / 2;
	deviceContext->DrawBitmap(arrowPrevious, xPos, yPos);
}

bool CBitmapWndViewer::NeedAfterDrawBitmap()
{
    switch (preview)
    {
		case IDM_WAVE_EFFECT:
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
		case IDM_WAVE_EFFECT:
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
            updateFilter = true;
            RefreshWindow();
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
            
			RefreshWindow();
		}
        updateFilter = true;
		
	}
	else if (preview == IDM_FILTRELENSFLARE || preview == IDM_WAVE_EFFECT)
	{
		if (mouseBlock)
		{
			m_cDessin->MouseMove(xPos, yPos, posLargeur, posHauteur, ratio);
            updateFilter = true;
			RefreshWindow();
		}
	}
	else
	{
		bool isOnArrow = false;
		if (fixArrow && etape == 0)
		{
			int yPosTop = (height - arrowNext.GetHeight()) / 2;
			int yPosBottom = (height - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

			if (xPos < arrowPrevious.GetWidth() && (yPos > yPosTop && yPos < yPosBottom))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
			else if ((xPos >(width - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
		}
        
	}


}
