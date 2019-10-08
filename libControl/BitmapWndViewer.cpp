#include <header.h>
#include "BitmapWndViewer.h"
#include <ConvertUtility.h>
#include <OpenCLEffect.h>
#include <FiltreEffet.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
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
#include <Crop.h>
#include <wx/mimetype.h>
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
#include <config_id.h>

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
}

void CBitmapWndViewer::PrintPicture()
{
	CRegardsBitmap * bitmap = GetBitmap(true);
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
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif    
	wxColor colorToReplace = wxColor(0, 0, 0);
	wxColor colorActifReplacement = wxColor(255, 255, 255);
	arrowPrevious = CLibResource::CreatePictureFromSVG("IDB_ARROWLPNG" , 32 * scale_factor, 32 * scale_factor);
	arrowPrevious.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());
	arrowNext = CLibResource::CreatePictureFromSVG("IDB_ARROWRPNG", 32 * scale_factor, 32 * scale_factor);
	arrowNext.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
		colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

}

COpenCLContext * CBitmapWndViewer::GetOpenCLContext()
{
	return openclContext;
}

CBitmapWndViewer::CBitmapWndViewer(wxWindow* parent, wxWindowID id, CSliderInterface * slider, wxWindowID mainViewerId, const CThemeBitmapWindow & theme, CBitmapInterface * bitmapInterface)
	: CBitmapWnd(parent, id, slider, mainViewerId, theme)
{
	filtreraw = nullptr;
	cl_nextPicture = nullptr;
	pictureNext = nullptr;
	openclEffectVideo = nullptr;
	mouseUpdate = nullptr;
	etape = 0;
	//Preview Parameter
	preview = 0;
	//renderPreviewBitmap = nullptr;
	//rawDecoder = nullptr;
	//Thread Parameter
	m_bTransition = false;
	isDiaporama = false;
	isInUse = false;
	invertColor = false;
	transitionTimer = nullptr;
	selectEffectTimer = nullptr;
    LoadingResource();
	etape = 0;
	fixArrow = true;
	//numEffect = 1;
	nextPicture = nullptr;
	startTransition = false;
	this->bitmapInterface = bitmapInterface;
	oldMouse.x = 0;
	oldMouse.y = 0;
	//effectParameter = nullptr;
	m_cDessin = nullptr;
	transitionTimer = new wxTimer(this, TIMER_TRANSITION);
	Connect(TIMER_TRANSITION, wxEVT_TIMER, wxTimerEventHandler(CBitmapWndViewer::OnTransition), nullptr, this);
	Connect(wxEVT_IDLE, wxIdleEventHandler(CBitmapWndViewer::onIdle));

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
}

void CBitmapWndViewer::AfterSetBitmap()
{
    updateFilter = true;
	preview = PREVIEW_NONE;
	if (transitionTimer->IsRunning())
		transitionTimer->Stop();
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
			SetTool(preview);
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
		SetTool(preview);
	}
	break;
	default:
		toolOption = MOVEPICTURE;
		break;
	}

	//Refresh();
}

void CBitmapWndViewer::Resize()
{
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		if (!isInUse && m_cDessin != nullptr)
			SetDessinRatio();
	}
}

void CBitmapWndViewer::ApplyPreviewEffect(int & widthOutput, int & heightOutput)
{
	if(preview > 1 && mouseUpdate != nullptr)
		mouseUpdate->ApplyPreviewEffect(effectParameter, this, filtreEffet, m_cDessin, widthOutput, heightOutput);
}

void CBitmapWndViewer::OnFiltreOk()
{
    RefreshWindow();
}

void CBitmapWndViewer::OnFiltreCancel()
{
    RefreshWindow();
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
     printf("xPos : %d width : %d arrow : %d",xPos,width,arrowNext.GetWidth());
#ifdef __WXGTK__
    double scale_factor = GetContentScaleFactor();
#else
    double scale_factor = 1.0f;
#endif 
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		invertColor = true;
		mouseBlock = true;
		CaptureMouse();
		m_cDessin->InitPoint(xPos - xPosImage, yPos - yPosImage, 0, 0, 1);
	}
	else if (fixArrow)
	{

		int yPosTop = (height - arrowNext.GetHeight()) / 2;
		int yPosBottom = (height - arrowNext.GetHeight()) / 2 + arrowNext.GetHeight();

		if (xPos < arrowPrevious.GetWidth() && (yPos > yPosTop && yPos < yPosBottom))
		{
			bitmapInterface->ImagePrecedente();
		}
		else if ((xPos > (width - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
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

CFiltreEffet * CBitmapWndViewer::GetFiltreEffet()
{
	return filtreEffet;
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
	if(preview == IDM_DECODE_RAW)
		return 4;
	return orientation; 
}

CDraw * CBitmapWndViewer::GetDessinPt()
{
	return m_cDessin;
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
	if (CFiltreData::SupportMouseClick(toolOption))
	{
		CFiltreData::SetCursor(toolOption);
		if (mouseBlock)
		{
			m_cDessin->MouseMove(xPos - xPosImage, yPos - yPosImage, 0, 0, 1);
			updateFilter = true;
			this->Refresh();
		}
		else if(CFiltreData::SupportMouseSelection(toolOption))
		{
			m_cDessin->Selection(xPos - xPosImage, yPos - yPosImage, 0, 0, 1);
			updateFilter = true;
			this->Refresh();
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
			else if ((xPos > (width - arrowNext.GetWidth()) && (yPos > yPosTop && yPos < yPosBottom)))
			{
				isOnArrow = true;
				::wxSetCursor(wxCursor(wxCURSOR_HAND));
			}
		}
	}
}
