#include "FiltreEffect.h"
#include <effect.h>
#include <ConvertUtility.h>
#include <TreeDataEffect.h>
#include "CloudsEffectParameter.h"
#include "BrightAndContrastEffectParameter.h"
#include "FreeRotateEffectParameter.h"
#include "MotionBlurEffectParameter.h"
#include "SolarisationEffectParameter.h"
#include "PosterisationEffectParameter.h"
#include "PhotoFiltreEffectParameter.h"
#include "RgbEffectParameter.h"
#include "SwirlEffectParameter.h"
#include "LensFlareEffectParameter.h"
#include <BitmapWndViewer.h>
#include <libResource.h>
#include <RegardsBitmap.h>
#define TAILLEMAX 1024
using namespace Regards::Viewer;

CFiltreEffect::CFiltreEffect(CBitmapWndViewer * bitmapViewer, CThemeTree * theme, CTreeElementControlInterface * interfaceControl)
{
	this->bitmapViewer = bitmapViewer;
	themeTree = *theme;
	themeTree.themeTriangle.SetHeight(themeTree.GetRowHeight());
	themeTree.themeCheckbox.SetHeight(themeTree.GetRowHeight());
	themeTree.themeDelete.SetHeight(themeTree.GetRowHeight());
	themeTree.themeSlide.SetHeight(themeTree.GetRowHeight());
	themeTree.themeTexte.SetHeight(themeTree.GetRowHeight());
	eventControl = interfaceControl;
	rowWidth.push_back(0);
	rowWidth.push_back(0);
	libelleEffectRadius = CLibResource::LoadStringFromResource(L"LBLEFFECTRADIUS",1);
	libelleEffectSigma = CLibResource::LoadStringFromResource(L"LBLEFFECTSIGMA",1);
	libelleEffectAngle = CLibResource::LoadStringFromResource(L"LBLEFFECTANGLE",1);
	libelleCloudsFrequency = CLibResource::LoadStringFromResource(L"LBLCLOUDSFREQUENCY",1);
	libelleCloudsAmplitude = CLibResource::LoadStringFromResource(L"LBLCLOUDSAMPLITUDE",1);
	libelleCloudsColorFrontRed = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTRED",1);
	libelleCloudsColorFrontGreen = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTGREEN",1);
	libelleCloudsColorFrontBlue = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORFRONTBLUE",1);
	libelleCloudsColorBackRed = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKRED",1);
	libelleCloudsColorBackGreen = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKGREEN",1);
	libelleCloudsColorBackBlue = CLibResource::LoadStringFromResource(L"LBLCLOUDSCOLORBACKBLUE",1);
	libelleEffectColorRed = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORRED",1);
	libelleEffectColorGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORGREEN",1);
	libelleEffectColorBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTCOLORBLUE",1);
	libelleEffectLevel = CLibResource::LoadStringFromResource(L"LBLEFFECTLEVEL",1);
	libelleEffectGamma = CLibResource::LoadStringFromResource(L"LBLEFFECTGAMMA",1);
	libelleEffectIntensity = CLibResource::LoadStringFromResource(L"LBLEFFECTINTENSITY",1);
	libelleEffectContrast = CLibResource::LoadStringFromResource(L"LBLEFFECTCONTRAST",1);
	libelleEffectLightness = CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);
	libelleEffectThreshold = CLibResource::LoadStringFromResource(L"LBLEFFECTTHRESHOLD",1);
	libelleRotationAngle = CLibResource::LoadStringFromResource(L"LBLROTATIONANGLE",1);
	libelleColor = CLibResource::LoadStringFromResource(L"LBLCOLOREFFECT",1);
}

CFiltreEffect::~CFiltreEffect(void)
{
}

void CFiltreEffect::Init(CEffectParameter * effectParameter, CRegardsBitmap * source, const int &filtre)
{
	this->filtre = filtre;
	this->effectParameter = effectParameter;
	this->source = source;

	switch (filtre)
	{
	case IDM_FILTRE_MOTIONBLUR:
		MotionBlur();
		break;

	case IDM_FILTRE_CLOUDS:
		Clouds();
		break;

	case IDM_FILTRE_SWIRL:
		Swirl();
		break;

	case IDM_COLOR_BALANCE:
		RGBFiltre();
		break;

	case ID_AJUSTEMENT_POSTERISATION:
		Posterisation();
		break;

	case ID_AJUSTEMENT_PHOTOFILTRE:
		PhotoFiltre();
		break;

	case IDM_IMAGE_LIGHTCONTRAST:
		BrightAndContrast();
		break;

	case IDM_AJUSTEMENT_SOLARISATION:
		Solarisation();
		break;

	case IDM_FILTRELENSFLARE:
		LensFlare();
		break;

	case IDM_ROTATE_FREE:
		RotateFree();
		break;

	case IDM_CROP:
		Crop();
		break;
	}

	CreateElement();
}

void CFiltreEffect::LensFlare()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;

	vector<int> elementIntensity;
	for (int i = 0; i < 101; i++)
		elementIntensity.push_back(i);

	vector<int> elementColor;
	for (int i = 0; i < 361; i++)
		elementColor.push_back(i);

	vector<int> elementBitmapHeight;
	for (int i = 0; i < 101; i++)
		elementBitmapHeight.push_back(i);

	AddTreeInfos(libelleEffectLightness, lensFlareParameter->brightness, elementIntensity, index++, top, child);
	AddTreeInfos(libelleEffectIntensity, lensFlareParameter->colorIntensity, elementIntensity, index++, top, child);
	AddTreeInfos(libelleEffectRadius, lensFlareParameter->size, elementBitmapHeight, index++, top, child);
	AddTreeInfos(libelleColor, lensFlareParameter->color, elementColor, index++, top, child);
}

void CFiltreEffect::LensFlareChangeParam(const int &valueData, const wxString &key)
{
	CLensFlareEffectParameter * lensFlareParameter = (CLensFlareEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectLightness)
	{
		lensFlareParameter->brightness = value;
	}
	else if (key == libelleEffectIntensity)
	{
		lensFlareParameter->colorIntensity = value;
	}
	else if (key == libelleEffectRadius)
	{
		lensFlareParameter->size = value;
	}
	else if (key == libelleColor)
	{
		lensFlareParameter->color = value;
	}
}

void CFiltreEffect::MotionBlur()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;

	vector<int> elementAngle;
	for (int i = 0; i < 361; i++)
		elementAngle.push_back(i);


	vector<int> elementSample;
	for (int i = 0; i < 11; i++)
		elementSample.push_back(i);

	vector<int> velocity;
	for (int i = 0; i < 101; i++)
		velocity.push_back(i);

	AddTreeInfos(libelleEffectRadius, motionBlurParameter->radius, elementSample, index++, top, child);
	AddTreeInfos(libelleEffectSigma, motionBlurParameter->sigma, velocity, index++, top, child);
	AddTreeInfos(libelleEffectAngle, motionBlurParameter->angle, elementAngle, index++, top, child);
}

void CFiltreEffect::MotionBlurChangeParam(const int &valueData, const wxString &key)
{

	CMotionBlurEffectParameter * motionBlurParameter = (CMotionBlurEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectRadius)
	{
		motionBlurParameter->radius = value;
	}
	else if (key == libelleEffectSigma)
	{
		motionBlurParameter->sigma = value;
	}
	else if (key == libelleEffectAngle)
	{
		motionBlurParameter->angle = value;
	}
}

void CFiltreEffect::Clouds()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;

	vector<int> elementFreq;
	for (int i = 0; i < 101; i++)
		elementFreq.push_back(i);

	vector<int> elementColor;
	for (int i = 0; i < 256; i++)
		elementColor.push_back(i);

	AddTreeInfos(libelleCloudsFrequency, cloudsEffectParameter->frequence * 100, elementFreq, index++, top, child);
	AddTreeInfos(libelleCloudsAmplitude, cloudsEffectParameter->amplitude * 100, elementFreq, index++, top, child);
	AddTreeInfos(libelleCloudsColorFrontRed, cloudsEffectParameter->colorFront.GetRed(), elementColor, index++, top, child);
	AddTreeInfos(libelleCloudsColorFrontGreen, cloudsEffectParameter->colorFront.GetGreen(), elementColor, index++, top, child);
	AddTreeInfos(libelleCloudsColorFrontBlue, cloudsEffectParameter->colorFront.GetBlue(), elementColor, index++, top, child);
	AddTreeInfos(libelleCloudsColorBackRed, cloudsEffectParameter->colorBack.GetRed(), elementColor, index++, top, child);
	AddTreeInfos(libelleCloudsColorBackGreen, cloudsEffectParameter->colorBack.GetGreen(), elementColor, index++, top, child);
	AddTreeInfos(libelleCloudsColorBackBlue, cloudsEffectParameter->colorBack.GetBlue(), elementColor, index++, top, child);
}

void CFiltreEffect::CloudsChangeParam(const int &valueData, const wxString &key)
{
	CCloudsEffectParameter * cloudsEffectParameter = (CCloudsEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleCloudsFrequency)
	{
		cloudsEffectParameter->frequence = value / 100.0;
	}
	if (key == libelleCloudsAmplitude)
	{
		cloudsEffectParameter->amplitude = value / 100.0;
	}
	if (key == L"Clouds.Octaves")
	{
		cloudsEffectParameter->octave = value;
	}
	if (key == libelleCloudsColorFrontRed)
	{
		cloudsEffectParameter->colorFront = CRgbaquad(value, cloudsEffectParameter->colorFront.GetGreen(), cloudsEffectParameter->colorFront.GetBlue());
	}
	if (key == libelleCloudsColorFrontGreen)
	{
		cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), value, cloudsEffectParameter->colorFront.GetBlue());
	}
	if (key == libelleCloudsColorFrontBlue)
	{
		cloudsEffectParameter->colorFront = CRgbaquad(cloudsEffectParameter->colorFront.GetRed(), cloudsEffectParameter->colorFront.GetGreen(), value);
	}
	if (key == libelleCloudsColorBackRed)
	{
		cloudsEffectParameter->colorBack = CRgbaquad(value, cloudsEffectParameter->colorBack.GetGreen(), cloudsEffectParameter->colorBack.GetBlue());
	}
	if (key == libelleCloudsColorBackGreen)
	{
		cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), value, cloudsEffectParameter->colorBack.GetBlue());
	}
	if (key == libelleCloudsColorBackBlue)
	{
		cloudsEffectParameter->colorBack = CRgbaquad(cloudsEffectParameter->colorBack.GetRed(), cloudsEffectParameter->colorBack.GetGreen(), value);
	}
}

void CFiltreEffect::Swirl()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;

	swirlEffectParameter->bitmapWidth = source->GetBitmapWidth();
	swirlEffectParameter->bitmapHeight = source->GetBitmapHeight();

	vector<int> elementAngle;
	for (int i = 0; i < 360; i++)
		elementAngle.push_back(i);

	vector<int> elementRadius;
	for (int i = 0; i < swirlEffectParameter->bitmapWidth; i++)
		elementRadius.push_back(i);

	AddTreeInfos(libelleEffectRadius, swirlEffectParameter->radius, elementRadius, index++, top, child);
	AddTreeInfos(libelleEffectAngle, swirlEffectParameter->angle, elementAngle, index++, top, child);
}

void CFiltreEffect::SwirlChangeParam(const int &valueData, const wxString &key)
{
	CSwirlEffectParameter * swirlEffectParameter = (CSwirlEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectRadius)
	{
		swirlEffectParameter->radius = value;
	}
	if (key == libelleEffectAngle)
	{
		swirlEffectParameter->angle = value;
	}
}

void CFiltreEffect::RGBFiltre()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = -255; i < 256; i++)
		elementColor.push_back(i);

	AddTreeInfos(libelleEffectColorRed, rgbEffectParameter->red + 255, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectColorGreen, rgbEffectParameter->green + 255, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectColorBlue, rgbEffectParameter->blue + 255, elementColor, index++, top, child);
}

void CFiltreEffect::RGBFiltreChangeParam(const int &valueData, const wxString &key)
{
	CRgbEffectParameter * rgbEffectParameter = (CRgbEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectColorRed)
	{
		rgbEffectParameter->red = value;
	}
	else if (key == libelleEffectColorGreen)
	{
		rgbEffectParameter->green = value;
	}
	else if (key == libelleEffectColorBlue)
	{
		rgbEffectParameter->blue = value;
	}
}

void CFiltreEffect::Posterisation()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = 0; i < 256; i++)
		elementColor.push_back(i);

	vector<int> elementGamma;
	for (int i = 0; i < 11; i++)
		elementGamma.push_back(i);

	AddTreeInfos(libelleEffectLevel, (int)posterisationEffectParameter->level, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectGamma, (int)posterisationEffectParameter->gamma * 10, elementGamma, index++, top, child);
}

void CFiltreEffect::PosterisationChangeParam(const int &valueData, const wxString &key)
{
	CPosterisationEffectParameter * posterisationEffectParameter = (CPosterisationEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectLevel)
	{
		posterisationEffectParameter->level = value;
	}
	if (key == libelleEffectGamma)
	{
		posterisationEffectParameter->gamma = value / 10.0;
	}
}

void CFiltreEffect::Crop()
{
	//int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;
}

void CFiltreEffect::PhotoFiltre()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = 0; i < 256; i++)
		elementColor.push_back(i);

	vector<int> intensity;
	for (int i = 0; i < 101; i++)
		intensity.push_back(i);

	AddTreeInfos(libelleEffectIntensity, photoEffectParameter->intensity, intensity, index++, top, child);
	AddTreeInfos(libelleEffectColorRed, photoEffectParameter->red, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectColorGreen, photoEffectParameter->green, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectColorBlue, photoEffectParameter->blue, elementColor, index++, top, child);
}

void CFiltreEffect::PhotoFiltreChangeParam(const int &valueData, const wxString &key)
{
	CPhotoFiltreEffectParameter * photoEffectParameter = (CPhotoFiltreEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectColorRed)
	{
		photoEffectParameter->red = value;
	}
	else if (key == libelleEffectColorGreen)
	{
		photoEffectParameter->green = value;
	}
	else if (key == libelleEffectColorBlue)
	{
		photoEffectParameter->blue = value;
	}
	else if (key == libelleEffectIntensity)
	{
		photoEffectParameter->intensity = value;
	}

}

void CFiltreEffect::BrightAndContrast()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = -100; i < 101; i++)
		elementColor.push_back(i);

	AddTreeInfos(libelleEffectContrast, (int)BrightAndContrastEffectParameter->contrast + 100, elementColor, index++, top, child);
	AddTreeInfos(libelleEffectLightness, (int)BrightAndContrastEffectParameter->brightness + 100, elementColor, index++, top, child);
}

void CFiltreEffect::BrightAndContrastChangeParam(const int &valueData, const wxString &key)
{
	CBrightAndContrastEffectParameter * BrightAndContrastEffectParameter = (CBrightAndContrastEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectContrast)
	{
		BrightAndContrastEffectParameter->contrast = value;
	}
	if (key == libelleEffectLightness)
	{
		BrightAndContrastEffectParameter->brightness = value;
	}
}

void CFiltreEffect::Solarisation()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = 0; i < 256; i++)
		elementColor.push_back(i);

	AddTreeInfos(libelleEffectThreshold, (int)solarisationEffectParameter->threshold, elementColor, index++, top, child);
}

void CFiltreEffect::SolarisationChangeParam(const int &valueData, const wxString &key)
{
	CSolarisationEffectParameter * solarisationEffectParameter = (CSolarisationEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleEffectThreshold)
	{
		solarisationEffectParameter->threshold = value;
	}
}

void CFiltreEffect::UpdateScreenRatio()
{
    UpdateElement();
}

void CFiltreEffect::RotateFree()
{
	int index = 0;
	tree<CTreeData *>::iterator top;
	tree<CTreeData *>::iterator child;

	top = tr.begin();
	child = top;

	CFreeRotateEffectParameter * freeRotateEffectParameter = (CFreeRotateEffectParameter *)effectParameter;

	vector<int> elementColor;
	for (int i = 0; i < 361; i++)
		elementColor.push_back(i);

	AddTreeInfos(libelleRotationAngle, freeRotateEffectParameter->angle, elementColor, index++, top, child);
}

void CFiltreEffect::RotateFreeChangeParam(const int &valueData, const wxString &key)
{
	CFreeRotateEffectParameter * freeRotateEffectParameter = (CFreeRotateEffectParameter *)effectParameter;

	float value = valueData;
	//Video Parameter
	if (key == libelleRotationAngle)
	{
		freeRotateEffectParameter->angle = value;
	}
}

void CFiltreEffect::SlidePosChange(CTreeElement * treeElement, const int &position, const int &valueData, const wxString &key)
{
	switch (filtre)
	{
	case IDM_FILTRE_MOTIONBLUR:
		MotionBlurChangeParam(valueData, key);
		break;

	case IDM_FILTRE_CLOUDS:
		CloudsChangeParam(valueData, key);
		break;

	case IDM_FILTRE_SWIRL:
		SwirlChangeParam(valueData, key);
		break;

	case IDM_COLOR_BALANCE:
		RGBFiltreChangeParam(valueData, key);
		break;

	case ID_AJUSTEMENT_POSTERISATION:
		PosterisationChangeParam(valueData, key);
		break;

	case ID_AJUSTEMENT_PHOTOFILTRE:
		PhotoFiltreChangeParam(valueData, key);
		break;

	case IDM_IMAGE_LIGHTCONTRAST:
		BrightAndContrastChangeParam(valueData, key);
		break;

	case IDM_AJUSTEMENT_SOLARISATION:
		SolarisationChangeParam(valueData, key);
		break;

	case IDM_ROTATE_FREE:
		RotateFreeChangeParam(valueData, key);
		break;

	case IDM_FILTRELENSFLARE:
		LensFlareChangeParam(valueData, key);
		break;
	}

	if (bitmapViewer != nullptr)
		bitmapViewer->Refresh();

	eventControl->UpdateElement(treeElement);
}


void CFiltreEffect::AddTreeInfos(const wxString &exifKey, const int &position,const vector<int> & value, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child)
{
	wchar_t seps[] = L".";
	int item = 0;
	wchar_t informations[TAILLEMAX];
	wchar_t *next_token1 = nullptr;

	wcscpy(informations, exifKey.c_str());

	// Establish string and get the first token:
#ifdef WIN32
	wchar_t * token = wcstok(informations, seps); // C4996
#else
	wchar_t * token = wcstok(informations, seps, &next_token1); // C4996
#endif
	// Note: strtok is deprecated; consider using strtok_s instead
	while (token != nullptr)
	{
		CTreeDataEffect * treeData = new CTreeDataEffect();
		treeData->SetKey(token);
#ifdef WIN32
		token = wcstok(nullptr, seps); // C4996
#else
		token = wcstok(nullptr, seps, &next_token1); // C4996
#endif
		if (token != nullptr)
		{
			treeData->SetIsParent(true);

			if (index > 0)
			{
				if (item == 0)
				{
					tree<CTreeData *>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey());
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
				}
				else
				{
					tree<CTreeData *>::iterator it;
					//Recherche de la clé
					it = FindKey(treeData->GetKey(), child);
					if (it != nullptr)
					{
						child = it;
						item++;
						delete(treeData);
						continue;
					}
				}

			}

			if (item > 0)
			{
				child = tr.append_child(child, treeData);
			}
			else
			{
				child = tr.insert(top, treeData);
			}
		}
		else
		{
			treeData->SetIsParent(false);
			treeData->SetPosition(position);
			treeData->SetValue(value);
			treeData->SetExifKey(exifKey);
			tr.append_child(child, treeData);
		}
		item++;
	}
}

void CFiltreEffect::CreateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	CPositionElement * posElement = nullptr;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeDataEffect * data = (CTreeDataEffect *)*it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);

			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue().size() > 0)
			{
				xPos = themeTree.GetMargeX();
				treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetPosition(), data->GetValue(), data->GetExifKey());
				treeElementSlide->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);

				widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			if (isVisible)
			{
				yPos += themeTree.GetRowHeight();

				nbRow++;
				if (rowWidth[0] < widthElementColumn1)
					rowWidth[0] = widthElementColumn1;

				if (rowWidth[1] < widthElementColumn2)
					rowWidth[1] = widthElementColumn2;
			}
		}
		else
		{
			int xPos = widthPosition *profondeur;
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);


			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;

	}
}


void CFiltreEffect::UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent)
{
	CPositionElement * posElement = nullptr;
	tree<CTreeData *>::sibling_iterator it = tr.begin(parent);
	//tree<CTreeData *>::iterator itend = tr.end(parent);
	bool isVisible = true;
	//int i = 

	for (int i = 0; i < parent.number_of_children(); i++)
	{
		int profondeur = tr.depth(it);
		CTreeDataEffect * data = (CTreeDataEffect *)*it;

		if (data->GetValue().size() > 0 || it.number_of_children() == 0)
		{
			int xPos = widthPosition * profondeur;
			int widthElementColumn1 = 0;
			int widthElementColumn2 = 0;
			CTreeElementSlide * treeElementSlide = nullptr;
			CTreeElementTexte * treeElementTexte = nullptr;
			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}
			widthElementColumn1 = xPos + posElement->GetWidth() + themeTree.GetMargeX();

			if (data->GetValue().size() > 0)
			{
				xPos = themeTree.GetMargeX();
				posElement = GetElement(data, ELEMENT_SLIDE);
				if (posElement != nullptr)
				{
					CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)posElement->GetTreeElement();
					treeElementSlide->SetVisible(isVisible);
					posElement->SetX(xPos);
					posElement->SetY(yPos);
				}
				else
				{
					treeElementSlide = CreateSlideElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetPosition(), data->GetValue(), data->GetExifKey());
					treeElementSlide->SetVisible(isVisible);
					posElement = CreatePositionElement(xPos, yPos, nbRow, 1, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_SLIDE, treeElementSlide, data);
				}

				widthElementColumn2 = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			}

			if (isVisible)
			{
				yPos += themeTree.GetRowHeight();

				nbRow++;
				if (rowWidth[0] < widthElementColumn1)
					rowWidth[0] = widthElementColumn1;

				if (rowWidth[1] < widthElementColumn2)
					rowWidth[1] = widthElementColumn2;
			}
		}
		else
		{
			int xPos = widthPosition *profondeur;
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;

			posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);

			}

			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}

			widthElement = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();

			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		it++;

	}
}

void CFiltreEffect::UpdateElement()
{
	for (CPositionElement * value : vectorPosElement)
	{
		if (value != nullptr)
		{
			CTreeElement * treeElement = value->GetTreeElement();
			if (treeElement != nullptr)
				treeElement->SetVisible(false);
		}
	}

	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	bool isVisible = true;

	while (it != itend) {
		CTreeData * data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = GetElement(data, ELEMENT_TRIANGLE);
			if (posElement != nullptr)
			{
				treeElementTriangle = (CTreeElementTriangle *)posElement->GetTreeElement();
				treeElementTriangle->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
				treeElementTriangle->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			}
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();

			posElement = GetElement(data, ELEMENT_TEXTE);
			if (posElement != nullptr)
			{
				CTreeElementTexte * treeElementTexte = (CTreeElementTexte *)posElement->GetTreeElement();
				treeElementTexte->SetVisible(isVisible);
				posElement->SetX(xPos);
				posElement->SetY(yPos);
			}
			else
			{
				treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
				treeElementTexte->SetVisible(isVisible);
				posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);
			}

			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			bool isOpen = treeElementTriangle->GetOpen();
			if (isOpen)
				UpdateChildTree(it);
		}
		it++;
	}
}


void CFiltreEffect::CreateElement()
{
	tree<CTreeData *>::sibling_iterator it = tr.begin();
	tree<CTreeData *>::iterator itend = tr.end();
	yPos = 0;
	nbRow = 0;
	bool isVisible = true;

	while (it != itend) {
		CTreeData * data = *it;
		int profondeur = tr.depth(it);
		if (profondeur == 0)
		{
			int xPos = themeTree.GetMargeX();
			int widthElement = 0;
			CTreeElementTexte * treeElementTexte = nullptr;
			CTreeElementTriangle * treeElementTriangle = nullptr;
			CPositionElement * posElement = nullptr;
			treeElementTriangle = CreateTriangleElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), true);
			treeElementTriangle->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TRIANGLE, treeElementTriangle, data);
			widthPosition = xPos + posElement->GetWidth() + themeTree.GetMargeX();
			xPos += posElement->GetWidth() + themeTree.GetMargeX();
			
			treeElementTexte = CreateTexteElement(themeTree.GetRowWidth(), themeTree.GetRowHeight(), data->GetKey());
			treeElementTexte->SetVisible(isVisible);
			posElement = CreatePositionElement(xPos, yPos, nbRow, 0, themeTree.GetRowWidth(), themeTree.GetRowHeight(), ELEMENT_TEXTE, treeElementTexte, data, false);


			widthElement += xPos + posElement->GetWidth() + themeTree.GetMargeX();
			yPos += themeTree.GetRowHeight();
			nbRow++;
			if (rowWidth[0] < widthElement)
				rowWidth[0] = widthElement;

			CreateChildTree(it);
		}
		it++;
	}
}

void CFiltreEffect::UnclickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->UnclickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());
	}
}

void CFiltreEffect::ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		//treeElementSlide->SetElementPos((x - xPos) - posLargeur, y - posHauteur);
		treeElementSlide->ClickElement(window, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());

	}
	else if(element->GetType() == ELEMENT_TRIANGLE)
	{
		CTreeElementTriangle * treeElementTriangle = (CTreeElementTriangle *)treeElement;
		treeElementTriangle->ClickElement(window, (x + posLargeur) - element->GetX(), (y + posHauteur) - element->GetY());
		UpdateElement();
		eventControl->Update();
	}

}

void CFiltreEffect::MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur)
{
	CTreeElement * treeElement = element->GetTreeElement();
	if (element->GetType() == ELEMENT_SLIDE)
	{
		CTreeElementSlide * treeElementSlide = (CTreeElementSlide *)treeElement;

		int xPos = 0;
		if (treeElementSlide->GetRow() > 0)
			xPos = GetWidthRow(treeElementSlide->GetRow() - 1);

		treeElementSlide->MouseOver(deviceContext, (x + posLargeur) - (element->GetX() + xPos), (y + posHauteur) - element->GetY());
		if (treeElementSlide->MouseBlock())
			treeElementSlide->DrawElement(deviceContext, (element->GetX() + xPos) - posLargeur, element->GetY() - posHauteur);
	}
}