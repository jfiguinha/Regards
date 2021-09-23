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
#include <Draw.h>
#include <BitmapDisplay.h>
#include <RenderOpenGL.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

bool CFilterWindowParam::supportOpenCL = false;

CFilterWindowParam::CFilterWindowParam(): source(nullptr), m_pShader(nullptr)
{
	supportOpenCL = COpenCLEngine::SupportOpenCL();
}

CFilterWindowParam::~CFilterWindowParam()
{
    
}

void CFilterWindowParam::InitFilterOpenCLCompatible()
{
	supportOpenCL = COpenCLEngine::SupportOpenCL();
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
		
		wxImage render = filtreEffet->GetwxImage();
		Drawing(&dc, bitmapViewer, m_cDessin);
		dc.SelectObject(wxNullBitmap);
		CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
		wxImage * local_image = new wxImage(bitmap.ConvertToImage());
		imageLoad->SetPicture(local_image);
		filtreEffet->SetBitmap(imageLoad);
	}
}

bool CFilterWindowParam::SupportMouseClick()
{
	return false;
}

void CFilterWindowParam::SetCursor()
{
	::wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

bool CFilterWindowParam::SupportMouseSelection()
{
	return false;
}

bool CFilterWindowParam::IsOpenCLCompatible()
{
	return true;
}

bool CFilterWindowParam::IsOpenGLCompatible()
{
	//if (CFiltreData::IsOpenGLCompatible(GetTypeFilter()))
	//	return true;
	return false;
}

void CFilterWindowParam::ApplyPreviewEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * dessing, int& widthOutput, int& heightOutput)
{
	if (CFiltreData::IsOpenGLCompatible(GetNameFilter()))
		return;

	//filtreEffet->SetPreview(true);
	if (CFiltreData::IsOpenCLCompatible(GetNameFilter()) && supportOpenCL)
		filtreEffet->RenderEffect(GetNameFilter(), effectParameter);
	else
	{
		CRegardsBitmap * bitmap = filtreEffet->GetBitmap(false);
		if (bitmap != nullptr)
		{
			CImageLoadingFormat image;
			image.SetPicture(bitmap);
			CFiltreEffet * filtre = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
			filtre->RenderEffect(GetNameFilter(), effectParameter);
			CImageLoadingFormat * imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(filtre->GetBitmap(true));
			filtreEffet->SetBitmap(imageLoad);
			delete filtre;
		}
	}


}

void CFilterWindowParam::ApplyPreviewEffectSource(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer, CFiltreEffet* filtreEffet, CDraw* m_cDessin)
{

}

void CFilterWindowParam::ApplyExifToPoint(wxPoint& pt, int numExif, const int &width, const int &height)
{
	int y = width - pt.y;
	int x = pt.x;

	int width90 = height;
	int height90 = width;
	
	switch (numExif)
	{
	case 1:// top left side
		break;
	case 2:// top right side
		pt.x = width - pt.x;
		//filtre->FlipHorizontal();
		break;
	case 3:// bottom right side
		pt.x = width - pt.x;
		pt.y = height - pt.y;
		//filtre->FlipHorizontal();
		//filtre->FlipVertical();
		break;
	case 4:// bottom left side
		pt.y = height - pt.y;
		//filtre->FlipVertical();
		break;
	case 5://left side top
		//filtre->Rotate90();
		pt.y = x;
		pt.x = y;
		//pt.y = height - pt.y;
		pt.x = width - pt.x;
		//filtre->FlipVertical();
		break;
	case 6:// right side top
		//filtre->Rotate90();
		pt.y = x;
		pt.x = y;
		pt.x = width - pt.x;
		pt.y = height - pt.y;
		//filtre->FlipVertical();
		//filtre->FlipHorizontal();
		break;
	case 7:// right side bottom
		//filtre->Rotate90();
		pt.y = x;
		pt.x = y;
		//pt.x = width - pt.x;
		pt.y = height - pt.y;
		//filtre->FlipHorizontal();
		break;
	case 8:// left side bottom
		//filtre->Rotate90();
		pt.y = x;
		pt.x = y;
		break;
	default:;
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
		break;
	case 6:// right side top
		filtre->Rotate90();
		filtre->FlipVertical();
        filtre->FlipHorizontal();
		break;
	case 7:// right side bottom
		filtre->Rotate90();
        filtre->FlipHorizontal();
		break;
	case 8:// left side bottom
		filtre->Rotate90();
		
		break;
	default: ;
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

	if (CFiltreData::IsOpenCLCompatible(numFiltre) && supportOpenCL)
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
			CFiltreEffet * filtre_effet = new CFiltreEffet(bitmapViewer->GetBackColor(), nullptr, &image);
			filtre_effet->RenderEffect(numFiltre, effectParameter);
			imageLoad = new CImageLoadingFormat();
			imageLoad->SetPicture(filtre_effet->GetBitmap(true));
			imageLoad->SetOrientation(0);

			delete filtre_effet;
		}
	}

	return imageLoad;
}

CImageLoadingFormat * CFilterWindowParam::ApplyEffect(CEffectParameter * effectParameter, IBitmapDisplay * bitmapViewer)
{
	CImageLoadingFormat * imageLoad = CFilterWindowParam::RenderEffect(effectParameter, bitmapViewer, GetNameFilter());
	return imageLoad;
}

void CFilterWindowParam::ApplyOpenGLShader(CRenderOpenGL * renderOpenGL, CEffectParameter * effectParameter, const int &textureID)
{
    printf("GLSLShader IDR_GLSL_ALPHA_SHADER \n " );
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

