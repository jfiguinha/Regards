#include "ThumbnailEffect.h"
#include "InfosSeparationBarEffect.h"
#include <ThumbnailDataStorage.h>
#include <Histogramme.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <LibResource.h>
#include <FiltreEffet.h>
#include "wx/stdpaths.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
#include <libPicture.h>
#include <FilterData.h>
#include <LoadingResource.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#include "../include/picture_id.h"
#else
#include <config_id.h>
#include <picture_id.h>
#endif
#include <ImageLoadingFormat.h>
using namespace Regards::Window;
using namespace Regards::FiltreEffet;
using namespace Regards::Control;

wxDEFINE_EVENT(EVENT_ICONEUPDATE, wxCommandEvent);


class CThreadBitmapEffect
{
public:

	CThreadBitmapEffect()
	{
		thumbnailData = nullptr;
		_thread = nullptr;
		thumbnail = nullptr;
        picture = nullptr;
	};
	~CThreadBitmapEffect() 
	{
	};

	wxString filename;
    wxString filepath;
	int numIcone;
    CRegardsBitmap * picture;
	CThumbnailDataStorage * thumbnailData;
	thread * _thread;
	CThumbnailEffect * thumbnail;
};

CThumbnailEffect::CThumbnailEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnail(parent, id, statusBarInterface, themeThumbnail, testValidity)
{
	isAllProcess = true;
    processIdle = false;
	barseparationHeight = 40;
	config = nullptr;
	config = CParamInit::getInstance();
	colorEffect = CLibResource::LoadStringFromResource("LBLCOLOREFFECT",1); //"Color Effect";
	convolutionEffect = CLibResource::LoadStringFromResource("LBLCONVOLUTIONEFFECT",1); //"Convolution Effect";
	specialEffect = CLibResource::LoadStringFromResource("LBLSPECIALEFFECT",1); //"Special Effect";
	histogramEffect = CLibResource::LoadStringFromResource("LBLHISTOGRAMEFFECT",1); //"Special Effect";
	blackRoomEffect = CLibResource::LoadStringFromResource("LBLBLACKROOM",1);
	videoLabelEffect = CLibResource::LoadStringFromResource("LBLVIDEOEFFECT",1);
    hdrEffect = CLibResource::LoadStringFromResource("LBLHDREFFECT",1);
    rotateEffect = CLibResource::LoadStringFromResource("LBLROTATEEFFECT",1);

    //hdrEffect = "HDR Effect";
	//rotateEffect = "Rotate Effect";
	Connect(EVENT_ICONEUPDATE, wxCommandEventHandler(CThumbnailEffect::UpdateRenderIcone));
}

CThumbnailEffect::~CThumbnailEffect(void)
{
	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
		infosSeparationBar = nullptr;

	}
	listSeparator.clear();
}

wxString CThumbnailEffect::GetWaitingMessage()
{
	return "Window CThumbnailEffect waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " + to_string(nbProcess);
}

wxString CThumbnailEffect::GetFilename()
{
	return filename;
}

CIcone * CThumbnailEffect::FindElement(const int &xPos, const int &yPos)
{
	int x = xPos;
	int y = yPos;

    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			wxRect rc = icone->GetPos();
			if ((rc.x < x && x < (rc.width + rc.x)) && (rc.y < y && y < (rc.height + rc.y)))
			{
				return icone;
			}
		}
	}
	return nullptr;
}


CInfosSeparationBarEffect * CThumbnailEffect::CreateNewSeparatorBar(const wxString &libelle)
{
	CInfosSeparationBarEffect * infosSeparationBar = new CInfosSeparationBarEffect(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());
	listSeparator.push_back(infosSeparationBar);
	return infosSeparationBar;
}


float CThumbnailEffect::CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight)
{
	float newRatio = 1.0f;
	//int left = 0;
	//int top = 0;

	if (width > height)
		newRatio = (float)tailleBitmapWidth / (float)(width);
	else
		newRatio = (float)tailleBitmapHeight / (float)(height);

	if ((height * newRatio) > tailleBitmapHeight)
	{
		newRatio = (float)tailleBitmapHeight / (float)(height);
	}
	else
	{
		if ((width * newRatio) > tailleBitmapWidth)
		{
			newRatio = (float)tailleBitmapHeight / (float)(width);
		}
	}

	return newRatio;
}

void CThumbnailEffect::GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio)
{
	newRatio = CalculRatio(width, height, tailleAffichageBitmapWidth, tailleAffichageBitmapHeight);

	tailleAffichageBitmapWidth = int(float(width) * newRatio);
	tailleAffichageBitmapHeight = int(float(height) * newRatio);
}

void CThumbnailEffect::SetFile(const wxString &filename)
{
	processIdle = false;

	CLoadingResource loadingResource;
	this->filename = filename;
	CRgbaquad backcolor = CRgbaquad(themeThumbnail.colorBack.Red(), themeThumbnail.colorBack.Green(), themeThumbnail.colorBack.Blue());
	InitScrollingPos();
	EraseThumbnailList();
	CreateOrLoadStorageFile();

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
		infosSeparationBar = nullptr;
	}
	listSeparator.clear();

	isAllProcess = false;
	numSelect = nullptr;
	numActif = nullptr;

	CLibPicture picture;
	int format = picture.TestImageFormat(filename);
	if(format == RAWFILE)
	{
		CInfosSeparationBarEffect * blackRoom = CreateNewSeparatorBar(blackRoomEffect);
		int numElement = (int)iconeList->GetNbElement();
		CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
		thumbnailData->SetNumElement(0);
		thumbnailData->SetNumPhotoId(IDM_DECODE_RAW);
		CRegardsBitmap * pBitmap = loadingResource.LoadRegardsBmpResource("IDB_BLACKROOM");
		thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(IDM_DECODE_RAW));
		blackRoom->AddPhotoToList(numElement);

		thumbnailData->SetNumPhotoId(IDM_DECODE_RAW);

		CImageLoadingFormat image;
		image.SetPicture(pBitmap, true);
		thumbnailData->SetBitmap(&image);

		CIcone * pBitmapIcone = new CIcone(nullptr);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);

		SetNbFiles(1);
        
        isAllProcess = true;
	}
	else if(picture.TestIsVideo(filename))
	{
		CInfosSeparationBarEffect * videoEffect = CreateNewSeparatorBar(videoLabelEffect);
		int numElement = (int)iconeList->GetNbElement();
		CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
		thumbnailData->SetNumElement(0);
		thumbnailData->SetNumPhotoId(IDM_DECODE_RAW);
		CRegardsBitmap * pBitmap = loadingResource.LoadRegardsBmpResource("IDB_BLACKROOM");
		thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(IDM_FILTRE_VIDEO));
		videoEffect->AddPhotoToList(numElement);

		thumbnailData->SetNumPhotoId(IDM_FILTRE_VIDEO);

		CImageLoadingFormat image;
		image.SetPicture(pBitmap, true);
		thumbnailData->SetBitmap(&image);

		CIcone * pBitmapIcone = new CIcone(nullptr);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);

		SetNbFiles(1);

        isAllProcess = true;
	}
	else
	{

		CInfosSeparationBarEffect * infosSeparationColorEffect = CreateNewSeparatorBar(colorEffect);
		CInfosSeparationBarEffect * infosSeparationConvolutionEffect = CreateNewSeparatorBar(convolutionEffect);
		CInfosSeparationBarEffect * infosSeparationSpecialEffect = CreateNewSeparatorBar(specialEffect);
		CInfosSeparationBarEffect * infosSeparationHistogramEffect = CreateNewSeparatorBar(histogramEffect);
		CInfosSeparationBarEffect * infosSeparationRotateEffect = CreateNewSeparatorBar(rotateEffect);
        CInfosSeparationBarEffect * infosSeparationHDREffect = CreateNewSeparatorBar(hdrEffect);

		int i = 0;
		CRgbaquad colorQuad = CRgbaquad(themeThumbnail.colorBack.Red(), themeThumbnail.colorBack.Green(), themeThumbnail.colorBack.Blue());

		for (int numEffect = FILTER_START; numEffect < FILTER_END; numEffect++)
		{
			int numElement = (int)iconeList->GetNbElement();
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
			thumbnailData->SetNumElement(i++);
			thumbnailData->SetNumPhotoId(numEffect);
			switch (numEffect)
			{
                case IDM_FILTER_BM3D:
                {
					CRegardsBitmap * pBitmap = loadingResource.LoadRegardsBmpResource("IDB_FILTRE_BM3D");
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));
					infosSeparationConvolutionEffect->AddPhotoToList(numElement);
                    CImageLoadingFormat image;
                    image.SetPicture(pBitmap, true);
                    thumbnailData->SetBitmap(&image); 
                    break;
                }
                
				case IDM_REDEYE:
				{
					CRegardsBitmap * pBitmap = loadingResource.LoadRegardsBmpResource("IDB_REDEYE");
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));
					infosSeparationSpecialEffect->AddPhotoToList(numElement);
                    CImageLoadingFormat image;
                    image.SetPicture(pBitmap, true);
                    thumbnailData->SetBitmap(&image);                 
					break;
				}
                
				case IDM_CROP:
				{
                
					CRegardsBitmap * pResBitmap = loadingResource.LoadRegardsBmpResource("IDB_CROP");
					CRegardsBitmap * pBitmap = new CRegardsBitmap(pResBitmap->GetBitmapWidth(), pResBitmap->GetBitmapHeight());
					pBitmap->SetBackgroundColor(CRgbaquad(255,255,255));
					pBitmap->InsertBitmap(pResBitmap, 0, 0);
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));
					infosSeparationSpecialEffect->AddPhotoToList(numElement);
					delete pResBitmap;
                    
                    CImageLoadingFormat image;
                    image.SetPicture(pBitmap, true);
                    thumbnailData->SetBitmap(&image);                
					break;
				}
                
				case IDM_WAVE_EFFECT:
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));
					infosSeparationSpecialEffect->AddPhotoToList(numElement);
					break;

				case IDM_FILTRELENSFLARE:
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));
					infosSeparationSpecialEffect->AddPhotoToList(numElement);
					break;
                
				default:
					thumbnailData->SetFilename(CFiltreData::GetFilterLabel(numEffect));               
					int typeEffect = CFiltreData::GetTypeEffect(numEffect);
                
                
					switch (typeEffect)
					{
						case SPECIAL_EFFECT:
							infosSeparationSpecialEffect->AddPhotoToList(numElement);
							break;
                        
						case COLOR_EFFECT:
							infosSeparationColorEffect->AddPhotoToList(numElement);
							break;
                        
						case CONVOLUTION_EFFECT:
							infosSeparationConvolutionEffect->AddPhotoToList(numElement);
							break;
                        
						case HISTOGRAM_EFFECT:
							infosSeparationHistogramEffect->AddPhotoToList(numElement);
							break;

						case ROTATE_EFFECT:
							infosSeparationRotateEffect->AddPhotoToList(numElement);
							break;
                            
                        case HDR_EFFECT:
                            infosSeparationHDREffect->AddPhotoToList(numElement);
                            break;
					}
					break;
			}
			thumbnailData->SetNumPhotoId(numEffect);
			CIcone * pBitmapIcone = new CIcone(nullptr);
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			iconeList->AddElement(pBitmapIcone);
		}

		SetNbFiles(i);
	}

	threadDataProcess = true;
    processIdle = true;
    bufferUpdate = true;
	this->FastRefresh(this);
}

void CThumbnailEffect::LoadPicture(void * param)
{
	CThreadBitmapEffect * threadLoadingBitmap = (CThreadBitmapEffect *)param;
	if (threadLoadingBitmap == nullptr)
		return;
    
    CImageLoadingFormat thumbnail; 
    CSqlThumbnail sqlThumbnail;  
    CRgbaquad colorQuad = CRgbaquad(threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Red(), threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Green(), threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Blue());
	CRegardsBitmap * bitmap = sqlThumbnail.GetPictureThumbnail(threadLoadingBitmap->filepath);
    /*
    if(bitmap != nullptr)
    {      
        float ratio = 0.0;
        CImageLoadingFormat image; 
        image.SetPicture(bitmap);
        CFiltreEffet * filtre = new CFiltreEffet(colorQuad, nullptr, &image);        
        int tailleAffichageBitmapWidth = threadLoadingBitmap->thumbnail->themeThumbnail.themeIcone.GetWidth();
        int tailleAffichageBitmapHeight = threadLoadingBitmap->thumbnail->themeThumbnail.themeIcone.GetHeight();
        threadLoadingBitmap->thumbnail->GetBitmapDimension(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), tailleAffichageBitmapWidth, tailleAffichageBitmapHeight, ratio);
        filtre->Interpolation(tailleAffichageBitmapWidth, tailleAffichageBitmapHeight, 0,0,0,0);  
        thumbnail.SetPicture(filtre->GetBitmap(false));  
        delete filtre;    
    }
    */
    
	if(bitmap != nullptr)
    {
        thumbnail.SetPicture(bitmap);  
        CRgbaquad colorQuad = CRgbaquad(threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Red(), threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Green(), threadLoadingBitmap->thumbnail->themeThumbnail.colorBack.Blue());
        CFiltreEffet * filtre = new CFiltreEffet(colorQuad, nullptr, &thumbnail);

        switch (threadLoadingBitmap->thumbnailData->GetNumPhotoId())
        {
            case IDM_WAVE_EFFECT:
                filtre->WaveFilter(20,20, bitmap->GetBitmapHeight() / 2,2,20);
                break;

            case IDM_FILTRELENSFLARE:
                filtre->LensFlare(20, 20, 20, 1, 20, 45, 20);
                break;

            default:
                {
                    
                    CEffectParameter * effect = CFiltreData::GetDefaultEffectParameter(threadLoadingBitmap->thumbnailData->GetNumPhotoId());
                    filtre->RenderEffect(threadLoadingBitmap->thumbnailData->GetNumPhotoId(), effect);
                    if(effect != nullptr)
                        delete effect;
                }
                break;
        }
        
        threadLoadingBitmap->picture = filtre->GetBitmap(true);
        delete filtre;      
    }   
	

    
	wxCommandEvent * event = new wxCommandEvent(EVENT_ICONEUPDATE);
	event->SetClientData(threadLoadingBitmap);
	wxQueueEvent(threadLoadingBitmap->thumbnail, event);

}

void CThumbnailEffect::ProcessIdle()
{
    TRACE();
    printf("CThumbnailEffect::ProcessIdle() \n");
	//int nbProcesseur = thread::hardware_concurrency();
	int nbProcesseur = 1;
	CRegardsConfigParam * config = CParamInit::getInstance();
	if (config != nullptr)
		nbProcesseur = config->GetThumbnailProcess();

	if (isAllProcess)
    {
        processIdle = false;
		return;
    }       

	while(nbProcess < nbProcesseur)
	{
		
		for (auto i = 0;i <  iconeList->GetNbElement();i++)
		{
            CIcone * icone = iconeList->GetElement(i);
			iconeList->Lock();
			if (icone != nullptr)
			{
				CThumbnailData * pThumbnailData = icone->GetData();
                if(pThumbnailData != nullptr)
                {
                    bool isLoad = pThumbnailData->IsLoad();
                    bool isProcess = pThumbnailData->IsProcess();
                    if (!isLoad && !isProcess)
                    {
                        CThreadBitmapEffect * pLoadBitmap = new CThreadBitmapEffect();
                        pLoadBitmap->thumbnail = this;
                        pLoadBitmap->thumbnailData = (CThumbnailDataStorage *)pThumbnailData;
                        pLoadBitmap->filepath = filename;
                        pLoadBitmap->filename = pThumbnailData->GetFilename();
                        pLoadBitmap->numIcone = i;
                        pLoadBitmap->_thread = new thread(LoadPicture, pLoadBitmap);
                        nbProcess++;
                        pThumbnailData->SetIsProcess(true);
                    }
                }
			}
			iconeList->Unlock();
		}
		
	}
	//Test si tout a été fait
	isAllProcess = true;
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		iconeList->Lock();
		if (icone != nullptr)
		{
			bool isLoad = false;
			bool isProcess = false;
			CThumbnailData * pThumbnailData = icone->GetData();
            if(pThumbnailData != nullptr)
            {
                isLoad = pThumbnailData->IsLoad();
                isProcess = pThumbnailData->IsProcess();                
            }

            if (!isLoad && !isProcess)
			{
				isAllProcess = false;
			}
		}
		iconeList->Unlock();
		if (!isAllProcess)
			break;
	}

}

void CThumbnailEffect::UpdateRenderIcone(wxCommandEvent& event)
{
	nbProcess--;

	CThreadBitmapEffect * threadLoadingBitmap = (CThreadBitmapEffect *)event.GetClientData();
	if (threadLoadingBitmap == nullptr)
	{
        bufferUpdate = true;
		this->FastRefresh(this);
		return;
	}


	if (threadDataProcess != false && threadLoadingBitmap != nullptr)
	{
		if (filename == threadLoadingBitmap->filepath)
		{

			if (threadLoadingBitmap->picture != nullptr)
			{
				CThumbnailData * pThumbnailData = nullptr;
				CIcone * icone = nullptr;
				if (threadLoadingBitmap->numIcone >= iconeList->GetNbElement())
					return;

				icone = iconeList->GetElement(threadLoadingBitmap->numIcone);
                if(icone != nullptr)
                {
                    bool needToRefresh = false;
                    pThumbnailData = icone->GetData();
                    if (pThumbnailData->GetFilename() != threadLoadingBitmap->filename)
                    {
                        icone = FindIcone(threadLoadingBitmap->filename);
                        pThumbnailData = nullptr;
                    }
                    else if (icone != nullptr && pThumbnailData != nullptr)
                    {
                        pThumbnailData->SetIsProcess(false);

                        CImageLoadingFormat image;
                        image.SetPicture(threadLoadingBitmap->picture, true);
                        pThumbnailData->SetBitmap(&image);
                        pThumbnailData->SetIsLoading(false);
                        icone->DestroyCache();
                        if (!render)
                        {
                            bufferUpdate = true;
                            needToRefresh = true;
                        }
                            
                    }

                    if(needToRefresh)
                         this->FastRefresh(this);
                }
			}
		}
	}

	if (threadLoadingBitmap->_thread != nullptr)
	{
		if (threadLoadingBitmap->_thread->joinable())
			threadLoadingBitmap->_thread->join();

		delete threadLoadingBitmap->_thread;

		threadLoadingBitmap->_thread = nullptr;
	}


	if (threadLoadingBitmap != nullptr)
	{
		delete threadLoadingBitmap;
		threadLoadingBitmap = nullptr;
	}
}

void CThumbnailEffect::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth() + positionXOld))
		nbElementByRow++;

	int nbElementEnY = (int)iconeList->GetNbElement() / nbElementByRow;
	if (nbElementEnY * nbElementByRow < iconeList->GetNbElement())
		nbElementEnY++;

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	//Calcul du width max
    for (int i = 0;i < listSeparator.size();i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth() + positionXOld))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}
	
	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		int nbElement = (int)infosSeparationBar->listElement.size();
		int nbElementEnY = nbElement / nbElementByRow;
		if (nbElementEnY * nbElementByRow < nbElement)
			nbElementEnY++;

		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);
		infosSeparationBar->Render(deviceContext,0,0);

		y += barseparationHeight;

		for (int numElement : infosSeparationBar->listElement)
		{
			CIcone * pBitmapIcone = iconeList->GetElement(numElement);
            if(pBitmapIcone != nullptr)
            {
                pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
                pBitmapIcone->SetWindowPos(x, y);
                pBitmapIcone->SetSizeIcone(themeThumbnail.themeIcone.GetRealWidth(), themeThumbnail.themeIcone.GetRealHeight());


                //if visible
                int left = x;
                int right = x + themeThumbnail.themeIcone.GetWidth();
                int top = y;
                int bottom = y + themeThumbnail.themeIcone.GetHeight();

                if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
                {
                    RenderBitmap(deviceContext, pBitmapIcone,0,0);
                }

                x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
                if (nbElementX == nbElementByRow)
                {
                    nbElementX = 0;
                    x = -posLargeur;
                    nbElementY++;
                    y += themeThumbnail.themeIcone.GetHeight();
                }
            }

		}

		if (nbElementX != 0)
		{
			nbElementX = 0;
			x = -posLargeur;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}

	}
	
}


void CThumbnailEffect::UpdateScroll()
{
	//bool update = false;
	thumbnailSizeX = 0;
	thumbnailSizeY = 0;

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		int nbElement = (int)infosSeparationBar->listElement.size();

		int nbElementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth() + positionXOld))
			nbElementByRow++;

		int nbElementEnY = (int)infosSeparationBar->listElement.size() / nbElementByRow;
		if (nbElementEnY * nbElementByRow < infosSeparationBar->listElement.size())
			nbElementEnY++;

		if (nbElement < nbElementByRow)
			nbElementByRow = nbElement;

		int sizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
		if (sizeX > thumbnailSizeX)
			thumbnailSizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY += nbElementEnY * themeThumbnail.themeIcone.GetHeight() + infosSeparationBar->GetHeight();
	}

	//int nbElement = pIconeList.size();

	scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
	scrollbar->SetPosition(posLargeur, posHauteur);
}

