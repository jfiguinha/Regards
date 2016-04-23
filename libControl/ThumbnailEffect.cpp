#include "ThumbnailEffect.h"
#include "InfosSeparationBarEffect.h"
#include <ThumbnailDataStorage.h>
#include <Histogramme.h>
#include <FileUtility.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <libResource.h>
#include <FiltreEffet.h>
#include "effect.h"
#include "wx/stdpaths.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
#include <LoadingResource.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
using namespace Regards::Window;
using namespace Regards::FiltreEffet;
using namespace Regards::Control;

CThumbnailEffect::CThumbnailEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnail(parent, id, statusBarInterface, themeThumbnail)
{
	config = CParamInit::getInstance();
	colorEffect = CLibResource::LoadStringFromResource("LBLCOLOREFFECT",1); //"Color Effect";
	convolutionEffect = CLibResource::LoadStringFromResource("LBLCONVOLUTIONEFFECT",1); //"Convolution Effect";
	specialEffect = CLibResource::LoadStringFromResource("LBLSPECIALEFFECT",1); //"Special Effect";
	histogramEffect = CLibResource::LoadStringFromResource("LBLHISTOGRAMEFFECT",1); //"Special Effect";
	_filterSoften = CLibResource::LoadStringFromResource("LBLfilterSoften",1); //"Soften";
	_filterBlur = CLibResource::LoadStringFromResource("LBLfilterBlur",1); //"Blur";
	_filterSolarize = CLibResource::LoadStringFromResource("LBLfilterSolarize",1); //"Solarize";
	_filterGaussian = CLibResource::LoadStringFromResource("LBLfilterGaussian",1); //"Gaussian Blur";
	_filterMedian = CLibResource::LoadStringFromResource("LBLfilterMedian",1); //"Median";
	_filterMotion = CLibResource::LoadStringFromResource("LBLfilterMotion",1); //"Motion Blur";
	_filterRotate = CLibResource::LoadStringFromResource("LBLfilterRotate",1); //"Rotate Free";
	_filterLight = CLibResource::LoadStringFromResource("LBLfilterLight",1); //"LIGHT and CONTRAST";
	_filterPhoto = CLibResource::LoadStringFromResource("LBLfilterPhoto",1); //"PHOTO FILTRE";
	_filterPosterisation = CLibResource::LoadStringFromResource("LBLfilterPosterisation",1); //"POSTERISATION";
	_filterColor = CLibResource::LoadStringFromResource("LBLfilterColor",1); //"COLOR BALANCE";
	_filterSwirl = CLibResource::LoadStringFromResource("LBLfilterSwirl",1); //"SWIR";
	_filterClouds = CLibResource::LoadStringFromResource("LBLfilterClouds",1); //"CLOUDS";
	_filterErode = CLibResource::LoadStringFromResource("LBLfilterErode",1); //"ERODE";
	_filterDilate = CLibResource::LoadStringFromResource("LBLfilterDilate",1); //"DILATE";
	_filterSharpen = CLibResource::LoadStringFromResource("LBLfilterSharpen",1); //"SHARPEN";
	_filterSharpenStrong = CLibResource::LoadStringFromResource("LBLfilterSharpenStrong",1); //"SHARPEN STRONG";
	_filterNoise = CLibResource::LoadStringFromResource("LBLfilterNoise",1); //"NOISE";
	_filterMosaic = CLibResource::LoadStringFromResource("LBLfilterMosaic",1); //"MOSAIC";
	_filterEmboss = CLibResource::LoadStringFromResource("LBLfilterEmboss",1); //"EMBOSS";
	_filterGrey = CLibResource::LoadStringFromResource("LBLfilterGrey",1); //"GREY LEVE";
	_filterSepia = CLibResource::LoadStringFromResource("LBLfilterSepia",1); //"SEPIA";
	_filterBlack = CLibResource::LoadStringFromResource("LBLfilterBlack",1); //"BLACK AND WHITE";
	_filterEdge = CLibResource::LoadStringFromResource("LBLfilterEdge",1); //"EDGE";
	_filterNegatif = CLibResource::LoadStringFromResource("LBLfilterNegatif",1); //"NEGATIF";
	_filterLensFlare = CLibResource::LoadStringFromResource("LBLfilterLensFlare",1); //"LENS FLARE";
	_filterRedEye = CLibResource::LoadStringFromResource("LBLfilterRedEye",1); //"LENS FLARE";
    _filterCrop = CLibResource::LoadStringFromResource("LBLCROP",1);
	_filterHistogramNormalize = CLibResource::LoadStringFromResource("LBLHistogramNormalize",1); //"LENS FLARE";
	_filterHistogramEqualize = CLibResource::LoadStringFromResource("LBLHistogramEqualize",1); //"LENS FLARE";
	_filterHistogramLog = CLibResource::LoadStringFromResource("LBLHistogramLog",1); //"LENS FLARE";
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

wxString CThumbnailEffect::GetFilename()
{
	return filename;
}

CIcone * CThumbnailEffect::FindElement(const int &xPos, const int &yPos)
{
	int x = xPos;
	int y = yPos;

	for (CIcone * icone : pIconeList)
	{
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
	infosSeparationBar->SetWidth(width);
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

void CThumbnailEffect::SetFile(const wxString &filename, CRegardsBitmap * bitmap)
{


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

	CInfosSeparationBarEffect * infosSeparationColorEffect = CreateNewSeparatorBar(colorEffect);
	CInfosSeparationBarEffect * infosSeparationConvolutionEffect = CreateNewSeparatorBar(convolutionEffect);
	CInfosSeparationBarEffect * infosSeparationSpecialEffect = CreateNewSeparatorBar(specialEffect);
	CInfosSeparationBarEffect * infosSeparationHistogramEffect = CreateNewSeparatorBar(histogramEffect);

	numSelect = nullptr;
	numActif = nullptr;

	CRegardsBitmap * thumbnail = nullptr;
	//Load thumbnail
	
	if (config != nullptr)
	{
		float ratio = 0.0;
		int tailleAffichageBitmapWidth = this->themeThumbnail.themeIcone.GetWidth();
		int tailleAffichageBitmapHeight = this->themeThumbnail.themeIcone.GetHeight();
		GetBitmapDimension(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), tailleAffichageBitmapWidth, tailleAffichageBitmapHeight, ratio);
		thumbnail = new CRegardsBitmap(tailleAffichageBitmapWidth, tailleAffichageBitmapHeight);
		CFiltreEffet * filtreEffet = new CFiltreEffet(bitmap, CRgbaquad(0, 0, 0), config->GetEffectLibrary());
		filtreEffet->InterpolationBicubic(thumbnail);
		delete(filtreEffet);
	}

	int i = 0;
	CRgbaquad colorQuad = CRgbaquad(themeThumbnail.colorBack.Red(), themeThumbnail.colorBack.Green(), themeThumbnail.colorBack.Blue());

	for (int numEffect = 1; numEffect < 32; numEffect++)
	{
		int numElement = (int)pIconeList.size();
		CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
		thumbnailData->SetNumElement(i++);
		thumbnailData->SetNumPhotoId(numEffect);
		CFiltreEffet * filtre = nullptr;
		CRegardsBitmap * pBitmap = nullptr;

		switch (numEffect)
		{
		case IDM_REDEYE:
		{
			
			pBitmap = loadingResource.LoadRegardsBmpResource("IDB_REDEYE");
			thumbnailData= new CThumbnailDataStorage(_filterRedEye);
			infosSeparationSpecialEffect->AddPhotoToList(numElement);
			break;
		}
                
            case IDM_CROP:
            {
                
                CRegardsBitmap * pResBitmap = loadingResource.LoadRegardsBmpResource("IDB_CROP");
                pBitmap = new CRegardsBitmap(pResBitmap->GetBitmapWidth(), pResBitmap->GetBitmapHeight());
                pBitmap->SetBackgroundColor(CRgbaquad(255,255,255));
                pBitmap->InsertBitmap(pResBitmap, 0, 0);
                thumbnailData= new CThumbnailDataStorage(_filterCrop);
                infosSeparationSpecialEffect->AddPhotoToList(numElement);
                delete pResBitmap;
                break;
            }

		case IDM_HISTOGRAMNORMALIZE:
			pBitmap = new CRegardsBitmap();
			pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());
			filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
			filtre->HistogramNormalize();
			thumbnailData= new CThumbnailDataStorage(_filterHistogramNormalize);
			infosSeparationHistogramEffect->AddPhotoToList(numElement);
			break;

		case IDM_HISTOGRAMEQUALIZE:
			pBitmap = new CRegardsBitmap();
			pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());
			filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
			filtre->HistogramEqualize();
			thumbnailData= new CThumbnailDataStorage(_filterHistogramEqualize);
			infosSeparationHistogramEffect->AddPhotoToList(numElement);
			break;

		case IDM_HISTOGRAMLOG:
			pBitmap = new CRegardsBitmap();
			pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());
			filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
			filtre->HistogramLog();
			thumbnailData= new CThumbnailDataStorage(_filterHistogramLog);
			infosSeparationHistogramEffect->AddPhotoToList(numElement);
			break;

		default:
			pBitmap = new CRegardsBitmap();
			filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
			pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());;
			break;
		}

		if (filtre != nullptr)
		{
			switch (numEffect)
			{
			case IDM_FILTRE_SOFTEN:
				filtre->Soften();
				thumbnailData= new CThumbnailDataStorage(_filterSoften);
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_FLOU:
				filtre->Blur();
				thumbnailData= new CThumbnailDataStorage(_filterBlur);
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;

			case IDM_AJUSTEMENT_SOLARISATION:
				filtre->Solarize(50);
				thumbnailData= new CThumbnailDataStorage(_filterSolarize);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_FLOUGAUSSIEN:
				filtre->GaussianBlur();
				thumbnailData= new CThumbnailDataStorage(_filterGaussian);
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTREANTIBRUIT:
				filtre->Median();
				thumbnailData= new CThumbnailDataStorage(_filterMedian);
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_MOTIONBLUR:
				filtre->MotionBlur(20, 5, 40);
				thumbnailData= new CThumbnailDataStorage(_filterMotion);
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_ROTATE_FREE:
				filtre->RotateFree(15);
				thumbnailData= new CThumbnailDataStorage(_filterRotate);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_IMAGE_LIGHTCONTRAST:
				filtre->BrightnessAndContrast(20, 20);
				thumbnailData= new CThumbnailDataStorage(_filterLight);
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;

			case ID_AJUSTEMENT_PHOTOFILTRE:
				filtre->PhotoFiltre(CRgbaquad(255, 0, 0), 30);
				thumbnailData= new CThumbnailDataStorage(_filterPhoto);
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;

			case ID_AJUSTEMENT_POSTERISATION:
				filtre->Posterize(20, 20);
				thumbnailData= new CThumbnailDataStorage(_filterPosterisation);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_COLOR_BALANCE:
				thumbnailData= new CThumbnailDataStorage(_filterColor);
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_SWIRL:
				thumbnailData= new CThumbnailDataStorage(_filterSwirl);
				filtre->Swirl(20, 20);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_CLOUDS:
				thumbnailData= new CThumbnailDataStorage(_filterClouds);
				filtre->CloudsFilter(CRgbaquad(0, 0, 0), CRgbaquad(255, 255, 255), 3, 5, 8);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_ERODE:
				thumbnailData= new CThumbnailDataStorage(_filterErode);
				filtre->Erode();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_DILATE:
				thumbnailData= new CThumbnailDataStorage(_filterDilate);
				filtre->Dilate();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_SHARPEN:
				thumbnailData= new CThumbnailDataStorage(_filterSharpen);
				filtre->Sharpen();
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_SHARPENSTRONG:
				thumbnailData= new CThumbnailDataStorage(_filterSharpenStrong);
				filtre->SharpenStrong();
				infosSeparationConvolutionEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRENOISE:
				thumbnailData= new CThumbnailDataStorage(_filterNoise);
				filtre->Noise();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_MOSAIQUE:
				thumbnailData= new CThumbnailDataStorage(_filterMosaic);
				filtre->FiltreMosaic();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_EMBOSS:
				thumbnailData= new CThumbnailDataStorage(_filterEmboss);
				filtre->Emboss();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_GREY_LEVEL:
				thumbnailData= new CThumbnailDataStorage(_filterGrey);
				filtre->NiveauDeGris();
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;
			case IDM_IMAGE_SEPIA:
				thumbnailData= new CThumbnailDataStorage(_filterSepia);
				filtre->Sepia();
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;
			case IDM_BLACKANDWHITE:
				thumbnailData= new CThumbnailDataStorage(_filterBlack);
				filtre->NoirEtBlanc();
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRE_EDGE:
				thumbnailData= new CThumbnailDataStorage(_filterEdge);
				filtre->FiltreEdge();
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;
			case IDM_NEGATIF:
				thumbnailData= new CThumbnailDataStorage(_filterNegatif);
				filtre->Negatif();
				infosSeparationColorEffect->AddPhotoToList(numElement);
				break;
			case IDM_FILTRELENSFLARE:
				thumbnailData= new CThumbnailDataStorage(_filterLensFlare);
				filtre->LensFlare(20, 20, 20, 1, 20, 45, 20);
				infosSeparationSpecialEffect->AddPhotoToList(numElement);
				break;

			}
		}

		thumbnailData->SetNumPhotoId(numEffect);
		thumbnailData->SetBitmap(pBitmap);

		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pIconeList.push_back(pBitmapIcone);

		if (filtre != nullptr)
			delete(filtre);
	}



	SetNbFiles(i);

	this->Refresh();
}

void CThumbnailEffect::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
		nbElementByRow++;

	int nbElementEnY = (int)pIconeList.size() / nbElementByRow;
	if (nbElementEnY * nbElementByRow < pIconeList.size())
		nbElementEnY++;

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	//Calcul du width max
    for (int i = 0;i < listSeparator.size();i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
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
		infosSeparationBar->RenderIcone(deviceContext);

		y += barseparationHeight;

		for (int numElement : infosSeparationBar->listElement)
		{
			CIcone * pBitmapIcone = pIconeList.at(numElement);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			pBitmapIcone->SetSizeIcone(themeThumbnail.themeIcone.GetRealWidth(), themeThumbnail.themeIcone.GetRealHeight());


			//if visible
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();

			if ((right > 0 && left < width) && (top < height && bottom > 0))
			{
				RenderBitmap(deviceContext, pBitmapIcone);
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

		int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
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

