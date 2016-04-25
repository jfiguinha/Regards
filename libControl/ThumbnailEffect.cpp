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
#include <FilterData.h>
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
                thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(numEffect));
                infosSeparationSpecialEffect->AddPhotoToList(numElement);
                break;
            }
                
            case IDM_CROP:
            {
                
                CRegardsBitmap * pResBitmap = loadingResource.LoadRegardsBmpResource("IDB_CROP");
                pBitmap = new CRegardsBitmap(pResBitmap->GetBitmapWidth(), pResBitmap->GetBitmapHeight());
                pBitmap->SetBackgroundColor(CRgbaquad(255,255,255));
                pBitmap->InsertBitmap(pResBitmap, 0, 0);
                thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(numEffect));
                infosSeparationSpecialEffect->AddPhotoToList(numElement);
                delete pResBitmap;
                break;
            }
                
            case IDM_FILTRELENSFLARE:
                pBitmap = new CRegardsBitmap();
                pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());
                filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
                thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(numEffect));
                filtre->LensFlare(20, 20, 20, 1, 20, 45, 20);
                infosSeparationSpecialEffect->AddPhotoToList(numElement);
                break;
                
            default:
                pBitmap = new CRegardsBitmap();
                filtre = new CFiltreEffet(pBitmap, colorQuad, LIBCPU);
                pBitmap->SetBitmap(thumbnail->GetPtBitmap(), thumbnail->GetBitmapWidth(), thumbnail->GetBitmapHeight());
                CEffectParameter * effect = CFiltreData::GetDefaultEffectParameter(numEffect);
                filtre->RenderEffect(numEffect, effect);
                thumbnailData= new CThumbnailDataStorage(CFiltreData::GetFilterLabel(numEffect));
                if(effect != nullptr)
                    delete effect;
                
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
                }
                break;
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

