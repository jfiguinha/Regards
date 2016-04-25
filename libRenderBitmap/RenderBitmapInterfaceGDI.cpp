// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderBitmapInterfaceGDI.h"
#include <FiltreEffet.h>
#include <LoadingResource.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
using namespace Regards::Render;


CRenderBitmapInterfaceGDI::CRenderBitmapInterfaceGDI()
{
}

CRenderBitmapInterfaceGDI::~CRenderBitmapInterfaceGDI()
{
	for (CBitmapManager * texture : listTexture)
	{
		delete(texture);
	}
	listTexture.clear();
    
    if(renderBitmap != nullptr)
    {
        delete renderBitmap;
        renderBitmap = nullptr;
    }
}

void CRenderBitmapInterfaceGDI::CreateScreenRender(wxDC * deviceContext, const CRgbaquad &color)
{
	colorBackground = color;
}

int CRenderBitmapInterfaceGDI::AddTextureBitmap(CRegardsBitmap * bitmap, const wxString &entry)
{
	bool find = false;
	for (CBitmapManager * texture : listTexture)
	{
		if (texture->filename == entry)
		{
			texture->texture = bitmap;
			find = true;
			break;
		}
	}

	if (!find)
	{
		CBitmapManager * texture = new CBitmapManager();
		texture->filename = entry;
		texture->texture = bitmap;
		listTexture.push_back(texture);
	}


	return 0;
}

int CRenderBitmapInterfaceGDI::GetTextureID(const wxString &textureName)
{
	for (CBitmapManager * texture : listTexture)
	{
		if (texture->filename == textureName)
		{
			return 1;
		}
	}
	return 0;
}

float CRenderBitmapInterfaceGDI::CalculPictureRatio(const int &pictureWidth, const int &pictureHeight)
{
	if (pictureWidth == 0 && pictureHeight == 0)
		return 1.0f;

	float newRatio = 1;

	//int tailleAffichageWidth = 0, tailleAffichageHeight = 0;

	if (pictureWidth > pictureHeight)
		newRatio = (float)width / (float)(pictureWidth);
	else
		newRatio = (float)height / (float)(pictureHeight);

	if ((pictureHeight * newRatio) > height)
	{
		newRatio = (float)height / (float)(pictureHeight);
	}
	else
	{
		if ((pictureWidth * newRatio) > width)
		{
			newRatio = (float)width / (float)(pictureWidth);
		}
	}

	return newRatio;
}

CRegardsBitmap * CRenderBitmapInterfaceGDI::ScaleToScreen(CRegardsBitmap * in)
{
	CRegardsBitmap * out = nullptr;
	float ratioSecond = CalculPictureRatio(in->GetBitmapWidth(), in->GetBitmapHeight());
	int outWidth = in->GetBitmapWidth() * ratioSecond;
	int outHeight = in->GetBitmapHeight() * ratioSecond;

	
	if (config != nullptr)
	{
		CFiltreEffet * filtre = new CFiltreEffet(in, colorBackground, config->GetEffectLibrary());
		delete(bitmapScale);
		bitmapScale = new CRegardsBitmap(outWidth, outHeight);
		filtre->InterpolationBicubic(bitmapScale);

		bitmapZone.x = (width - outWidth) / 2;
		bitmapZone.y = (height - outHeight) / 2;

		out = new CRegardsBitmap(width, height);
		out->SetBackgroundColor(colorBackground);
		out->InsertBitmapWithoutAlpha(bitmapScale, bitmapZone.x, bitmapZone.y);

		delete(filtre);
	}

	return out;
}

CRegardsBitmap * CRenderBitmapInterfaceGDI::FindBitmap(const wxString &name)
{
	//bool find = false;
	for (CBitmapManager * texture : listTexture)
	{
		if (texture->filename == name)
		{
			return texture->texture;
		}
	}

	return nullptr;
}

void CRenderBitmapInterfaceGDI::CreateScreenTexture(const int &width, const int &height)
{
    if(this->width == width && this->height == height)
    {
        renderBitmap->SetBackgroundColor(colorBackground);;
        return;
    }
    
    
    if(renderBitmap != nullptr)
    {
        delete renderBitmap;
        renderBitmap = nullptr;
    }
	renderBitmap = new CRegardsBitmap(width, height);
	renderBitmap->SetBackgroundColor(colorBackground);
	this->width = width;
	this->height = height;
}

bool CRenderBitmapInterfaceGDI::TestIsTextureExist(const wxString &entry)
{
	bool find = false;
	for (CBitmapManager * texture : listTexture)
	{
		if (texture->filename == entry)
		{
			find = true;
			break;
		}
	}
	return find;
}

void CRenderBitmapInterfaceGDI::AddAlphaBlendingBitmap(const wxString &textureName, const int &left, const int &top)
{
	CRegardsBitmap * bitmap = FindBitmap(textureName);
	CRegardsBitmap * outputCrop = renderBitmap->CropBitmap(left, top, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	if (outputCrop != nullptr)
	{
		outputCrop->InsertBitmap(bitmap, 0, 0);
		renderBitmap->InsertBitmapWithoutAlpha(outputCrop, left, top);
		delete(outputCrop);
	}
}

void CRenderBitmapInterfaceGDI::AddBitmap(const wxString &textureName, const int &left, const int &top)
{
	CRegardsBitmap * bitmap = FindBitmap(textureName);
	if (bitmap != nullptr)
		renderBitmap->InsertBitmapWithoutAlpha(bitmap, left, top);
}

void CRenderBitmapInterfaceGDI::AddBitmapToScale(const wxString &textureName, const int &widthScale, const int &heightScale, const int &left, const int &top)
{
	CRegardsBitmap * bitmap = FindBitmap(textureName);
	
	if (config != nullptr)
	{
		delete(bitmapScale);
		bitmapScale = new CRegardsBitmap(widthScale, heightScale);
		CFiltreEffet filtreEffet(bitmap, colorBackground, config->GetEffectLibrary());
		filtreEffet.InterpolationBicubic(bitmapScale);
		renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, left, top);

		bitmapZone.x = left;
		bitmapZone.y = top;
	}
}

void CRenderBitmapInterfaceGDI::AddBitmapToScalePosition(const wxString &textureName, const int &widthScale, const int &heightScale, const wxRect &rc, const int &left, const int &top)
{
	CRegardsBitmap * bitmap = FindBitmap(textureName);
	
	if (config != nullptr)
	{
		delete(bitmapScale);
		bitmapScale = new CRegardsBitmap(widthScale, heightScale);
		CFiltreEffet filtreEffet(bitmap, colorBackground, config->GetEffectLibrary());

		wxRect rect;
		rect.height = rc.height;
		rect.width = rc.width;
		rect.x = rc.x;
		rect.y = rc.y;
		filtreEffet.InterpolationBicubic(bitmapScale, rect);
		renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, left, top);

		bitmapZone.x = left;
		bitmapZone.y = top;
	}
}

void CRenderBitmapInterfaceGDI::RenderToScreen(wxDC * deviceContext)
{
	CLoadingResource loadingResource;
	deviceContext->DrawBitmap(loadingResource.ConvertTowxImage(renderBitmap, false), 0, 0);
	//deviceContext->StretchDIBits(0, 0, renderBitmap);
}

//void CRenderBitmapInterfaceGDI::Transition(CRegardsBitmap * bitmap, const int &step)
void CRenderBitmapInterfaceGDI::Transition(const wxString &textureName, const int &step)
{
	CRegardsBitmap * bitmap = FindBitmap(textureName);
	
	if (config != nullptr && bitmap != nullptr)
	{
		CRegardsBitmap * nextPictureScale = ScaleToScreen(bitmap);
		CFiltreEffet * filtre = new CFiltreEffet(renderBitmap, colorBackground, config->GetEffectLibrary());
		filtre->Fusion(nextPictureScale, step / 100.0f);
		delete(nextPictureScale);
		delete(filtre);
	}
}

void CRenderBitmapInterfaceGDI::RenderEffect(const int &numEffect, CEffectParameter * effectParameter)
{
    if (config != nullptr)
    {
        CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
        filtre->RenderEffect(numEffect, effectParameter);
        delete filtre;
        
        renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, bitmapZone.x, bitmapZone.y);
    }
}


void CRenderBitmapInterfaceGDI::RotateFree(const int &angle)
{
	
	if (config != nullptr)
	{
		CFiltreEffet * filtre = new CFiltreEffet(bitmapScale, colorBackground, config->GetEffectLibrary());
		filtre->RotateFree(angle);
		delete filtre;

		renderBitmap->SetBackgroundColor(this->colorBackground);

		bitmapZone.x = (renderBitmap->GetBitmapWidth() - bitmapScale->GetBitmapWidth()) / 2;
		bitmapZone.y = (renderBitmap->GetBitmapHeight() - bitmapScale->GetBitmapHeight()) / 2;

		renderBitmap->InsertBitmapWithoutAlpha(bitmapScale, bitmapZone.x, bitmapZone.y);
	}
}
