#include "header.h"
#include "Icone.h"
#include <ThumbnailData.h>
#include <ParamInit.h>
//#include <InterpolationBicubicwxImage.h>
#include <LibResource.h>
#include <FileUtility.h>
#include "LoadingResource.h"
#include "WindowMain.h"
#include <config_id.h>
#include <wxSVG/SVGDocument.h>
#include <wx/sstream.h>
#ifdef WIN32
#include <FiltreEffetCPU.h>
#endif
#include <RegardsBitmap.h>
#include <RegardsConfigParam.h>

using namespace Regards::Window;

#define WM_PICTURELOAD 1

int CIcone::GetXPos()
{
	return x;
}
int CIcone::GetYPos()
{
	return y;
}

int CIcone::GetWidth()
{
	if (themeIcone.showOnlyThumbnail)
		width = themeIcone.GetWidth() - (themeIcone.GetMarge() << 1);
    else
        width =  themeIcone.GetWidth() - (themeIcone.GetMarge() << 1);

    return width;
}

int CIcone::GetHeight()
{
	if (themeIcone.showOnlyThumbnail)
		height = themeIcone.GetHeight() - (themeIcone.GetMarge() << 1);
    else
        height =  int(float(themeIcone.GetHeight()) * 0.75);
    
    return height;
}

CIcone& CIcone::operator=(const CIcone& other)
{
	// check for self-assignment
	if (&other == this)
		return *this;

	//---------------------------------------------------
	//Theme
	//---------------------------------------------------
	//other.themeIcone = this->themeIcone;

	//---------------------------------------------------
	//Variable
	//---------------------------------------------------
	//int interpolationMethod;
	deleteData = true;
	pThumbnailData = new CThumbnailData(other.pThumbnailData->GetFilename());
	*pThumbnailData = *other.pThumbnailData;
	themeIcone = other.themeIcone;
	bitmapCheckOn = other.bitmapCheckOn;
	bitmapCheckOff = other.bitmapCheckOff;
	useBackgroundColor = other.useBackgroundColor;
	photoVector = other.photoVector;
	checkOnVector = other.checkOnVector;
	checkOffVector = other.checkOffVector;
	backgroundColor = other.backgroundColor;
	
	tempImageVector = other.tempImageVector;
	pictureLoad = other.pictureLoad;
	showSelected = other.showSelected;
	isChecked = other.isChecked;
	isSelected = other.isSelected;
	photoDefault = other.photoDefault;
	posXThumbnail = other.posXThumbnail;
	posYThumbnail = other.posYThumbnail;
	numElement = other.numElement;
	x = other.x;
	y = other.y;

	thumbnailIconeCache = other.thumbnailIconeCache;
	filename = other.filename;
	state = other.state;
	numLib = other.numLib;
	width = other.width;
	height = other.height;
	showLoading = other.showLoading;
	pictureLoading = other.pictureLoading;
	transparent = other.transparent;
	eraseBitmap = other.eraseBitmap;
	bitmapLocal = other.bitmapLocal;
	return *this;
}

bool CIcone::operator == (const CIcone &n2)
{
	int left = x;
	int right = x + GetWidth();
	int top = y;
	int bottom = y + GetHeight();

	if ((left < n2.x && n2.x < right) && (top < n2.y && n2.y < bottom))
	{
		return true;
	}
	return false;
}

void CIcone::SetWindowPos(const int &x, const int &y)
{
    double scale_factor = 1.0f;
	this->x = x / scale_factor;
	this->y = y / scale_factor;
}

wxRect CIcone::GetPos()
{
    double scale_factor = 1.0f;
    wxRect rc;
	rc.x = x;
	rc.width = themeIcone.GetWidth() / scale_factor;
	rc.y = y;
	rc.height = themeIcone.GetHeight() / scale_factor;
	return rc;
}

void CIcone::SetSizeIcone(const int &width, const int &height)
{
	if (width != themeIcone.GetRealWidth() || height != themeIcone.GetRealHeight())
	{
		themeIcone.SetWidth(width);
		themeIcone.SetHeight(height);
		DestroyCache();
	}
}

void CIcone::SetTheme(CThemeIcone theme)
{
    int width = themeIcone.GetRealWidth();
    int height = themeIcone.GetRealHeight();
    
    themeIcone = theme;
    
    if (width != themeIcone.GetRealWidth() || height != themeIcone.GetRealHeight())
    {
        DestroyCache();
    }
    
    //SetSizeIcone(width, height);
    
}

void CIcone::DestroyCache()
{
	eraseBitmap = true;
}

void CIcone::SetSelected(const bool &value)
{
    DestroyCache();
	isSelected = value;
	if (!value)
		state = INACTIFICONE;
	else
		state = SELECTEDICONE;
}

bool CIcone::GetSelected()
{
	return isSelected;
}


wxImage CIcone::LoadImageResource(const wxString & resourceName)
{
	CRegardsBitmap * data = CLoadingResource::LoadRegardsBmpResource(resourceName);
	wxImage bitmap = CLoadingResource::ConvertTowxImageRGB(data);
	delete data;
	return bitmap;
}
CIcone::CIcone()
{
	deleteData = true;
	pThumbnailData = nullptr;
	showSelected = false;
	isChecked = false;
	isSelected = false;
	photoDefault = true;
	x = 0;
	y = 0;
	config = nullptr;
	width = 0;
	height = 0;
	showLoading = false;
	pictureLoad = false;
	posXThumbnail = 0;
	posYThumbnail = 0;
	state = INACTIFICONE;
	isChecked = false;
	numLib = LIBCPU;

	config = CParamInit::getInstance();
	if (config != nullptr)
		numLib = config->GetEffectLibrary();

	photoVector = CLibResource::GetVector(L"IDB_PHOTOTEMP");
    checkOnVector = CLibResource::GetVector(L"IDB_CHECKBOX_ON");
    checkOffVector = CLibResource::GetVector(L"IDB_CHECKBOX_OFF");
	useBackgroundColor = false;
	thumbnailIconeCache = config->GetThumbnailIconeCache();
}

void CIcone::SetBackgroundColor(const wxColour & backgroundColor)
{
	this->backgroundColor = backgroundColor;
	useBackgroundColor = true;
	state = USEBACKGROUNDCOLOR;
}

void CIcone::StartLoadingPicture()
{
    showLoading = true;
}

void CIcone::StopLoadingPicture()
{
    showLoading = false;
    //DestroyCache();
}

void CIcone::DeleteCache()
{
    DestroyCache();
}

void CIcone::SetPictureLoading(const wxImage &imageLoading)
{
    pictureLoading = imageLoading;
}

wxImage CIcone::CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vector)
{
    wxImage img;
    if(vector.size() > 0)
    {
        wxStringInputStream memBuffer(vector);
        wxSVGDocument svgDoc;
        svgDoc.Load(memBuffer);
        img = svgDoc.Render(buttonWidth,buttonHeight,NULL,true,true);
        
    }
    else
    {
        img.Create(buttonWidth, buttonHeight);
    }
    return img;
}

wxImage CIcone::GenerateVideoIcone()
{
	
	wxImage image = LoadImageResource(L"IDB_CADRE_VIDEO");
	return image.ResampleBicubic(themeIcone.GetWidth(), image.GetHeight());
}

bool CIcone::OnClick(const int &x, const int &y, const int &posLargeur, const int &posHauteur)
{
    DestroyCache();
	wxRect checkPos;
	int xPos = (x + posLargeur) - this->x;
	int yPos = (y + posHauteur) - this->y;
	checkPos.x = themeIcone.GetMarge();
	checkPos.width = checkPos.x + themeIcone.GetCheckboxWidth();
	checkPos.y = themeIcone.GetHeight() - themeIcone.GetCheckboxHeight();
	checkPos.height = checkPos.y + themeIcone.GetCheckboxHeight();
	if ((checkPos.x < xPos && xPos < checkPos.width) && (checkPos.y < yPos && yPos < checkPos.height))
	{
        
		isChecked = !isChecked;
		SetChecked(isChecked);
        return true;
	}
    return false;
}

void CIcone::RenderPictureBitmap(wxDC * memDC, const wxImage & bitmapScale, const int &type)
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = themeIcone.GetWidth();
	rc.height = themeIcone.GetHeight();

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked)
			memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
		else
			memDC->GradientFillLinear(rc, themeIcone.colorBack, themeIcone.colorBack);
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
        if (isChecked)
            memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
        else
            memDC->GradientFillLinear(rc, themeIcone.colorTop, themeIcone.colorBottom);
		break;
	default:
		if(useBackgroundColor)
			CWindowMain::FillRect(memDC, rc, backgroundColor);
		else
			CWindowMain::FillRect(memDC, rc, themeIcone.colorBack);
		break;
	}


	if (pThumbnailData != nullptr)
	{
		CalculPosition(bitmapScale);
		if(bitmapScale.IsOk())
			memDC->DrawBitmap(bitmapScale, posXThumbnail, themeIcone.GetMarge());

		if (!themeIcone.showOnlyThumbnail)
		{
			wxString libelle = "";

			if (pThumbnailData->GetTypeElement() == TYPEPHOTO)
				libelle = CFileUtility::GetFileName(pThumbnailData->GetFilename().c_str());
			else if (pThumbnailData->GetTypeElement() == TYPEMULTIPAGE)
				libelle = pThumbnailData->GetLibelle();
			else
				libelle = pThumbnailData->GetFilename();

			if (libelle != "")
			{

                CThemeFont themeFont = themeIcone.font;
				wxSize sizeTexte;
				do
				{
					sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeFont);
					if (sizeTexte.x > (themeIcone.GetWidth() - (themeIcone.GetMarge() * 2)))
						themeFont.SetFontSize(themeFont.GetFontRealSize() - 1);
				} while (sizeTexte.x > (themeIcone.GetWidth() - (themeIcone.GetMarge() * 2)));

				int xPos = (themeIcone.GetWidth() - sizeTexte.x) / 2;
				//int yPos = bitmapScale.GetHeight() + themeIcone.GetMarge() + ((themeIcone.GetHeight() - (bitmapScale.GetHeight() + themeIcone.GetMarge())) - sizeTexte.y) / 2;
                int yPos = rc.height - (themeIcone.GetMarge() * 2 + sizeTexte.y);
				CWindowMain::DrawTexte(memDC, libelle, xPos, yPos, themeFont);
			}
		}

		if (showSelected)
		{
   
                if(!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != themeIcone.GetCheckboxHeight() || bitmapCheckOn.GetWidth() != themeIcone.GetCheckboxWidth()))
                {
                    bitmapCheckOn = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_ON", themeIcone.GetCheckboxWidth(), themeIcone.GetCheckboxHeight());
                }
                
                if(!bitmapCheckOff.IsOk() || (bitmapCheckOff.GetHeight() != themeIcone.GetCheckboxHeight() || bitmapCheckOff.GetWidth() != themeIcone.GetCheckboxWidth()))
                {
                    bitmapCheckOff = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_OFF", themeIcone.GetCheckboxWidth(), themeIcone.GetCheckboxHeight());
                    
                }

            switch (type)
            {
                case INACTIFICONE:
                    if (isChecked && bitmapCheckOn.IsOk())
                        memDC->DrawBitmap(bitmapCheckOn.ConvertToDisabled(), themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
                    else if(bitmapCheckOff.IsOk())
                        memDC->DrawBitmap(bitmapCheckOff.ConvertToDisabled(), themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());
                    break;
                default:
                    if (isChecked && bitmapCheckOn.IsOk())
                        memDC->DrawBitmap(bitmapCheckOn, themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
                    else if(bitmapCheckOff.IsOk())
                        memDC->DrawBitmap(bitmapCheckOff, themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());
                    break;
            }
        

		}
	}
    
    if(showLoading && pictureLoading.IsOk())
    {
        if(!transparent.IsOk() || (transparent.GetWidth() != bitmapScale.GetWidth() || transparent.GetHeight() != bitmapScale.GetHeight()))
        {
            transparent = wxImage(bitmapScale.GetWidth(), bitmapScale.GetHeight());
            transparent.InitAlpha();
            for(int y = 0;y < bitmapScale.GetHeight();y++)
            {
                for(int x = 0;x < bitmapScale.GetWidth(); x++)
                {
                    transparent.SetRGB(x,y,255,255,255);
                    transparent.SetAlpha(x,y,128);
                }
            }
        }
           
		if(transparent.IsOk())
			memDC->DrawBitmap(transparent, posXThumbnail, themeIcone.GetMarge());
        
        

        //wxImage picture = pictureLoading.Scale(bitmapScale.GetWidth() / 2, bitmapScale.GetHeight() /2);

        int xLocal = posXThumbnail + (bitmapScale.GetWidth() - pictureLoading.GetWidth()) / 2;
        int yLocal = themeIcone.GetMarge() + (bitmapScale.GetHeight() - pictureLoading.GetHeight()) / 2;
        //if(pictureLoading.HasMask())
            
		if(pictureLoading.IsOk())
			memDC->DrawBitmap(pictureLoading.ConvertToGreyscale(), xLocal, yLocal, true);
    }
}


void CIcone::RenderVideoBitmap(wxDC * memDC, const wxImage &  bitmapScale, const int &type)
{
	wxImage bitmapImageActif;
	wxImage bitmapImageCadreVideo = GenerateVideoIcone();
	wxImage bitmapImageCadreActif = bitmapImageCadreVideo.ConvertToDisabled();
	//int xBitmapPos = x + (themeIcone.GetWidth() - GetWidth()) / 2;
	//int bottomPos = 0;

	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = themeIcone.GetWidth();
	rc.height = themeIcone.GetHeight();

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked)
			memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
		else
			CWindowMain::FillRect(memDC, rc, themeIcone.colorBack);	
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
		memDC->GradientFillLinear(rc, themeIcone.colorTop, themeIcone.colorBottom);
		break;
	}

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked && bitmapImageCadreActif.IsOk())
			memDC->DrawBitmap(bitmapImageCadreActif, 0, 0);
		else if(bitmapImageCadreVideo.IsOk())
			memDC->DrawBitmap(bitmapImageCadreVideo, 0, 0);
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
		if(bitmapImageCadreActif.IsOk())
			memDC->DrawBitmap(bitmapImageCadreActif, 0, 0);
		break;
	}
	
	//Size Thumbnail Max
	int heightThumbnailMax = (themeIcone.GetHeight() - 40) - (bitmapImageCadreVideo.GetHeight() * 2);
	if (bitmapScale.GetHeight() > heightThumbnailMax)
		bitmapImageActif = bitmapScale.ResampleBicubic(bitmapScale.GetWidth(), heightThumbnailMax);
	else
		bitmapImageActif = bitmapScale;

	if (pThumbnailData != nullptr )
	{
		int yPos = 0;
		CalculPosition(bitmapScale);

		if (!themeIcone.showOnlyThumbnail)
		{
			wxString libelle = pThumbnailData->GetTimePositionLibelle();
			//wxSize sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeIcone.font);
			yPos = bitmapImageCadreVideo.GetHeight();
		}
		else
		{
			yPos = bitmapImageCadreVideo.GetHeight() + ((themeIcone.GetHeight() - (bitmapImageCadreVideo.GetHeight() * 2)) - bitmapImageActif.GetHeight()) / 2;
		}

		switch (type)
		{
		case INACTIFICONE:
			if (isChecked)
			{
				if(bitmapImageActif.IsOk())
					memDC->DrawBitmap(bitmapImageActif, posXThumbnail, yPos);

				if(bitmapImageCadreActif.IsOk())
					memDC->DrawBitmap(bitmapImageCadreActif, 0, themeIcone.GetHeight() - bitmapImageCadreVideo.GetHeight());
			}
			else
			{
				memDC->DrawBitmap(bitmapImageActif.ConvertToGreyscale(), posXThumbnail, yPos);
				memDC->DrawBitmap(bitmapImageCadreVideo, 0, themeIcone.GetHeight() - bitmapImageCadreVideo.GetHeight());
			}
			break;
		case SELECTEDICONE:
		case ACTIFICONE:
			memDC->DrawBitmap(bitmapImageActif, posXThumbnail, yPos);
			memDC->DrawBitmap(bitmapImageCadreActif, 0, themeIcone.GetHeight() - bitmapImageCadreVideo.GetHeight());
			break;
		}
		
		if (!themeIcone.showOnlyThumbnail)
		{
			wxString libelle = L"";

			libelle = pThumbnailData->GetTimePositionLibelle();

			if (libelle != L"")
			{
				CThemeFont themeFont = themeIcone.font;
				wxSize sizeTexte;
				do
				{
					sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeFont);
					if (sizeTexte.x > themeIcone.GetWidth())
						themeFont.SetFontSize(themeFont.GetFontSize() - 1);
				} while (sizeTexte.x > themeIcone.GetWidth());

				int xPos = (themeIcone.GetWidth() - sizeTexte.x) / 2;
				int y = (themeIcone.GetHeight() - (bitmapImageCadreVideo.GetHeight() * 2 + bitmapImageActif.GetHeight() + sizeTexte.y)) / 2;
				CWindowMain::DrawTexte(memDC, libelle, xPos, yPos + y + bitmapImageActif.GetHeight(), themeFont);
			}
		}

		if (showSelected)
		{
			if (isChecked)
				memDC->DrawBitmap(bitmapCheckOn, themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
			else
				memDC->DrawBitmap(bitmapCheckOff, themeIcone.GetMarge(), themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());

		}
	}

}

void CIcone::RenderBitmap(wxDC * memdc, const wxImage & bitmapScale, const int &type)
{
	if (pThumbnailData->GetTypeElement() == TYPEVIDEO)
		return RenderVideoBitmap(memdc, bitmapScale, type);

	return RenderPictureBitmap(memdc, bitmapScale, type);
}

CIcone::~CIcone(void)
{
	
	if (deleteData)
	{
		if (pThumbnailData != nullptr)
			delete pThumbnailData;
		pThumbnailData = nullptr;
	}
	
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void CIcone::SetData(CThumbnailData * thumbnailData, const bool &deleteData)
{
	pThumbnailData = thumbnailData;
	this->deleteData = deleteData;
}


int CIcone::GetBitmapWidth()
{
	if (themeIcone.showOnlyThumbnail)
		return themeIcone.GetWidth() - (themeIcone.GetMarge() << 1);

	return themeIcone.GetWidth() - (themeIcone.GetMarge() << 1);

}

int CIcone::GetBitmapHeight()
{
	if (themeIcone.showOnlyThumbnail)
		return themeIcone.GetHeight() - (themeIcone.GetMarge() << 1);

	return int(float(themeIcone.GetHeight()) * 0.75);
}

CThumbnailData * CIcone::GetData()
{
	return pThumbnailData;
}

CThumbnailData * CIcone::GetCopyData()
{
	CThumbnailData * data = new CThumbnailData(pThumbnailData->GetFilename());
	*data = *pThumbnailData;
	return data;
}


bool CIcone::DataNeedToBeDelete()
{
	return deleteData;
}

void CIcone::CalculPosition(const wxImage & render)
{
	int localx = (themeIcone.GetWidth() - GetWidth()) / 2;
	int localy = themeIcone.GetMarge();
	//float newRatio = 1.0f;
	int left = 0;
	int top = 0;

	if (GetWidth() > render.GetWidth())
		left = ((GetWidth() - render.GetWidth()) / 2);
	else
	{
		left = 0;
	}

	if (GetHeight() > render.GetHeight())
	{
		top = ((GetHeight() - render.GetHeight()) / 2);
	}
	else
	{
		top = 0;
	}

	posXThumbnail = localx + left;
	posYThumbnail = localy + top;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int CIcone::RenderIcone(wxDC * dc, const int &posLargeur, const int &posHauteur, const bool & flipHorizontal, const bool & flipVertical)
{
	int returnValue = 0;
	wxBitmap localmemBitmap(themeIcone.GetWidth(), themeIcone.GetHeight());  
    
    if (pThumbnailData != nullptr)
    {
        if(pThumbnailData->GetNbFrame() > 1 && (state == ACTIFICONE || state == SELECTEDICONE))
        {
            int numFrame = pThumbnailData->GetNumFrame();
            pThumbnailData->SetNumFrame(numFrame + 1);
            eraseBitmap = true;
        }
    }
    
    
	if (eraseBitmap)
	{
		wxMemoryDC memDC(localmemBitmap);
		wxImage image;
		wxImage scale;
		photoDefault = true;

		int tailleAffichageBitmapWidth = 0;
		int tailleAffichageBitmapHeight = 0;
		float ratio = 0.0;

		if (pThumbnailData != nullptr)
		{
			if (photoDefault)
			{
				image = pThumbnailData->GetwxImage();
				if (!image.IsOk())
				{
					photoDefault = false;
					wxColor colorToReplace = wxColor(0, 0, 0);
					wxColor colorActifReplacement = wxColor(255, 255, 255);
					image = CreateFromSVG(themeIcone.GetWidth(), themeIcone.GetHeight(), photoVector);
					image.Replace(colorToReplace.Red(), colorToReplace.Green(), colorToReplace.Blue(),
						colorActifReplacement.Red(), colorActifReplacement.Green(), colorActifReplacement.Blue());

					returnValue = 1;
				}
				if (flipHorizontal)
					image = image.Mirror();
				if (flipVertical)
					image = image.Mirror(0);
			}
		}
		if (image.IsOk())
		{
			GetBitmapDimension(image.GetWidth(), image.GetHeight(), tailleAffichageBitmapWidth, tailleAffichageBitmapHeight, ratio);
			if (config->GetThumbnailQuality() == 0)
				scale = image.Scale(tailleAffichageBitmapWidth, tailleAffichageBitmapHeight);
			else
				scale = image.ResampleBicubic(tailleAffichageBitmapWidth, tailleAffichageBitmapHeight);

		}

		RenderBitmap(&memDC, scale, state);

		memDC.SelectObject(wxNullBitmap);

		scale.Destroy();
		image.Destroy();

		bitmapLocal = localmemBitmap;

		eraseBitmap = false;
	}

    dc->DrawBitmap(bitmapLocal, x + posLargeur, y + posHauteur);
	return returnValue;
}

void CIcone::SetActive(const bool &value)
{
    DestroyCache();
	if (state != SELECTEDICONE)
	{
		if (value)
			state = ACTIFICONE;
		else
			state = INACTIFICONE;
	}
}



bool CIcone::IsChecked()
{
	return isChecked;
}

void CIcone::SetChecked(const bool &value)
{
    DestroyCache();
	isChecked = value;
}

float CIcone::CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight)
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

void CIcone::GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio)
{

	newRatio = CalculRatio(width, height, GetWidth(), GetHeight());

	tailleAffichageBitmapWidth = int(float(width) * newRatio);
	tailleAffichageBitmapHeight = int(float(height) * newRatio);
}

