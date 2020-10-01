#include <header.h>
//
//  FilterWindowParam.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "FilterWindowParam.h"
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <FiltreEffet.h>
#include <RegardsBitmap.h>
#include <PiccanteHDR.h>
#include <Draw.h>
#include <BitmapDisplay.h>
//#include <ParamInit.h>
//#include <RegardsConfigParam.h>
#include <RenderOpenGL.h>
bool CFilterWindowParam::supportOpenCL = false;

CFilterWindowParam::CFilterWindowParam()
{
	//CRegardsConfigParam* config = CParamInit::getInstance();
	//if (config != nullptr)
	//	supportOpenCL = config->GetIsOpenCLSupport();
}

void CFilterWindowParam::SetOpenCLCompatible(const bool & openCLCompatible)
{
    supportOpenCL = openCLCompatible;
}

CFilterWindowParam::~CFilterWindowParam()
{
    
}

CImageLoadingFormat * CFilterWindowParam::ApplyMouseMoveEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CDraw * dessing)
{
	return nullptr;
}

void CFilterWindowParam::Drawing(wxMemoryDC * dc, IBitmapDisplay * bitmapViewer, CDraw * m_cDessin)
{
	m_cDessin->Dessiner(dc, 0, 0, 1, wxColour(30, 30, 30), wxColour(30, 30, 30), wxColour(255, 255, 255), 2);
}

void CFilterWindowParam::DrawingToPicture(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin)
{
	if (m_cDessin != nullptr)
	{
		wxImage image = filtreEffet->GetwxImage();
		wxBitmap bitmap = wxBitmap(image);
		wxMemoryDC dc;
		dc.SelectObject(bitmap);
		wxRect rc(0, 0, image.GetWidth(), image.GetHeight());
		wxImage render = filtreEffet->GetwxImage();
		Drawing(&dc, bitmapViewer, m_cDessin);
		dc.SelectObject(wxNullBitmap);
		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		wxImage * local_image = new wxImage(bitmap.ConvertToImage());
		imageLoad->SetPicture(local_image);
		filtreEffet->SetBitmap(imageLoad);
	}
}

bool CFilterWindowParam::IsOpenGLCompatible()
{
	if (CFiltreData::IsOpenGLCompatible(GetTypeFilter()))
		return true;
	return false;
}

void CFilterWindowParam::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int & widthOutput, int & heightOutput)
{
	if (CFiltreData::IsOpenGLCompatible(GetTypeFilter()))
		return;

	if (CFiltreData::IsPiccanteCompatible(GetTypeFilter()))
	{
		filtreEffet->SetPreview(true);
		CRegardsFloatBitmap * test = filtreEffet->GetFloatBitmap(false);
		switch (GetTypeFilter())
		{
		case IDM_BEST_EXPOSURE:
			CPiccanteHDR::BestExposure(test);
			break;
		case IDM_FILTER_KUWAHARA:
			CPiccanteHDR::FilterKuwahara(test);
			break;
		case IDM_FILTER_BILATERAL2DS:
			CPiccanteHDR::FilterBilateral2DS(test);
			break;
		}

		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(test);
		filtreEffet->SetBitmap(imageLoad);
	}
	else
	{
		filtreEffet->SetPreview(true);
		if (CFiltreData::IsOpenCLCompatible(GetTypeFilter()) && supportOpenCL)
			filtreEffet->RenderEffect(GetTypeFilter(), effectParameter);
		else
		{
			CRegardsBitmap * bitmap = filtreEffet->GetBitmap(false);
			if (bitmap != nullptr)
			{
				CImageLoadingFormat image;
				image.SetPicture(bitmap);
				CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
				filtre->RenderEffect(GetTypeFilter(), effectParameter);
				CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
				imageLoad->SetPicture(filtre->GetBitmap(true));
				filtreEffet->SetBitmap(imageLoad);
				delete filtre;
			}
		}

		widthOutput = filtreEffet->GetWidth();
		heightOutput = filtreEffet->GetHeight();
	}
}

void CFilterWindowParam::RotateExif(const int & orientation, CFiltreEffet * filtre)
{
	switch (orientation)
	{
	case 1:// top left side
		break;
	case 2:// top right side
		filtre->FlipHorizontal();
		break;
	case 3:// bottom right side
		filtre->FlipHorizontal();
		filtre->FlipVertical();
		break;
	case 4:// bottom left side
		filtre->FlipVertical();
		break;
	case 5://left side top
		filtre->Rotate90();
		filtre->FlipVertical();
		filtre->FlipHorizontal();
		break;
	case 6:// right side top
		filtre->Rotate90();
		filtre->FlipVertical();
		break;
	case 7:// right side bottom
		filtre->Rotate90();
		break;
	case 8:// left side bottom
		filtre->Rotate90();
		filtre->FlipHorizontal();
		break;
	}
}

CImageLoadingFormat * CFilterWindowParam::RenderEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, int numFiltre)
{
	CImageLoadingFormat * imageLoad = nullptr;

	CFiltreEffet * filtre = bitmapViewer->GetFiltreEffet();
	if (filtre != nullptr)
	{
		filtre->SetPreview(false);
		RotateExif(bitmapViewer->GetOrientation(), filtre);
	}

	if (CFiltreData::IsPiccanteCompatible(numFiltre))
	{
		CRegardsFloatBitmap * bitmap = bitmapViewer->GetFloatBitmap(true);
		switch (numFiltre)
		{
		case IDM_BEST_EXPOSURE:
			CPiccanteHDR::BestExposure(bitmap);
			break;
		case IDM_FILTER_KUWAHARA:
			CPiccanteHDR::FilterKuwahara(bitmap);
			break;
		case IDM_FILTER_BILATERAL2DS:
			CPiccanteHDR::FilterBilateral2DS(bitmap);
			break;
		}
		CImageLoadingFormat imageLoadFormat(true);
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(bitmap);
		imageLoad->SetOrientation(bitmapViewer->GetOrientation());
	}
	else if (CFiltreData::IsOpenCLCompatible(numFiltre) && supportOpenCL)
	{
		if (filtre != nullptr)
		{
			filtre->RenderEffect(numFiltre, effectParameter);
			imageLoad = new CImageLoadingFormat();
			CRegardsBitmap * bitmapOut = filtre->GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);
			imageLoad->SetOrientation(0);
		}
	}
	else
	{
		CRegardsBitmap * bitmap = bitmapViewer->GetBitmap(true);
		if (bitmap != nullptr)
		{
			CImageLoadingFormat image;
			bitmap->RotateExif(bitmapViewer->GetOrientation());
			image.SetPicture(bitmap);
			CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
			filtre->RenderEffect(numFiltre, effectParameter);
			imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(filtre->GetBitmap(true));
			imageLoad->SetOrientation(0);

			delete filtre;
		}
	}

	return imageLoad;
}

CImageLoadingFormat * CFilterWindowParam::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = CFilterWindowParam::RenderEffect(effectParameter, bitmapViewer, GetTypeFilter());
	return imageLoad;
}

void CFilterWindowParam::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
	GLSLShader * m_pShader = renderOpenGL->FindShader(L"IDR_GLSL_ALPHA_SHADER");
	if (m_pShader != nullptr)
	{
		m_pShader->EnableShader();
		if (!m_pShader->SetTexture("textureScreen", textureID))
		{
			printf("SetTexture textureScreen failed \n ");
		}
		if (!m_pShader->SetParam("intensity", 100))
		{
			printf("SetParam intensity failed \n ");
		}
	}
}

void CFilterWindowParam::DisableOpenGLShader()
{
	//glBindTexture(GL_TEXTURE_2D, 0);

	if (m_pShader != nullptr)
		m_pShader->DisableShader();
}

