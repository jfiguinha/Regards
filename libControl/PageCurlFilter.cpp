#include <header.h>
//
//  BrightAndContrastFilter.cpp
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#include "PageCurlFilter.h"
#include <RGBAQuad.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <GLTexture.h>

#include "effect_id.h"
using namespace Regards::Filter;
using namespace Regards::OpenGL;

CPageCurlFilter::CPageCurlFilter()
{
	initTexture = true;

}

CPageCurlFilter::~CPageCurlFilter()
{

}

void CPageCurlFilter::RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert,
	const int& width, const int& height, const int& left, const int& top)
{
	// 1. Configuration des unités de texture (Multi-texturing Core Profile)
	glActiveTexture(GL_TEXTURE0);
	pictureFirst->Enable(); // Lié à l'unité 0

	glActiveTexture(GL_TEXTURE1);
	pictureNext->Enable();  // Lié à l'unité 1

	// Activation du blending pour gérer les ombres transparentes de la page cornée
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 2. Récupération et configuration du shader
	COpenGLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_PAGECURL", GL_FRAGMENT_SHADER);
	if (shader != nullptr)
	{
		// Envoi de la matrice de projection via votre accesseur
		shader->EnableShader(renderOpenGL->GetProjectionMatrix());

		// Liaison explicite des échantillonneurs aux unités correspondantes (0 et 1)
		if (!shader->m_pShader->SetTexture("sourceTex", pictureFirst->GetTextureID(), 0))
		{
			printf("SetTexture sourceTex failed \n");
		}
		if (!shader->m_pShader->SetTexture("targetTex", pictureNext->GetTextureID(), 1))
		{
			printf("SetTexture targetTex failed \n");
		}
		if (!shader->m_pShader->SetParam("time", time))
		{
			printf("SetParam time failed \n");
		}
		if (!shader->m_pShader->SetParam("invertTex", invert))
		{
			printf("SetParam invertTex failed \n");
		}
	}

	// 3. CORRECTION : Utilisation du pipeline moderne RenderQuad au lieu de ShowSecondBitmap
	// Signature à 8 arguments issue de votre classe : 
	// RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH, const bool& flipV, int left, int top, bool inverted)
	renderOpenGL->GetRenderOpengl()->RenderQuad(pictureFirst.get(), width, height, false, false, left, top, false);

	// 4. Nettoyage des états OpenGL
	if (shader != nullptr)
		shader->DisableShader();

	glDisable(GL_BLEND);

	// Désactivation propre des unités de texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int CPageCurlFilter::GetTypeFilter()
{
	return IDM_AFTEREFFECT_PAGECURL;
}


void CPageCurlFilter::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	initTexture = true;
	bmpViewer->StartTransitionEffect(bmpSecond, false);
}

bool CPageCurlFilter::RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
                                    IBitmapDisplay* bmpViewer, CRenderBitmapOpenGL* renderOpenGL,
                                    const float& scale_factor, const int& etape)
{
	if (etape > 0 && etape < 110)
	{
		GenerateTexture(nextPicture, source, bmpViewer);
		int widthOutput = bmpViewer->GetWidth() * scale_factor;
		int heightOutput = bmpViewer->GetHeight() * scale_factor;
		RenderTexture(renderOpenGL, etape, false, widthOutput, heightOutput, 0, 0);

		return true;
	}
	return false;
}

void CPageCurlFilter::GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
                                      IBitmapDisplay* bmpViewer)
{
	bool init = false;
	std::unique_ptr<CImageLoadingFormat> bitmapOut;
	std::unique_ptr<CImageLoadingFormat> bitmapFirst = std::make_unique<CImageLoadingFormat>();
	if (initTexture || (pictureFirst->GetWidth() != bmpViewer->GetWidth() && pictureFirst->GetHeight() != bmpViewer->GetHeight()))
	{
		init = true;
		initTexture = false;
	}

	{
		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			auto mat = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4,
			                   cv::Scalar(colorBack.GetBlue(), colorBack.GetGreen(), colorBack.GetRed(), 255));
			bitmapFirst->SetPicture(mat);
			bitmapOut.reset(GenerateInterpolationBitmapTexture(nextPicture, bmpViewer));
			if (bitmapOut != nullptr)
			{
				bitmapFirst->InsertBitmap(bitmapOut.get(), out.x, out.y);
				//bitmapNext.Flip();
			}

			mat = bitmapFirst->GetMatrix().getMat();
			cv::flip(mat, mat, 0);
            Regards::Picture::CPictureArray pictureArray = Regards::Picture::CPictureArray(mat); 
			pictureNext->SetData(pictureArray, nullptr);
		}
	}
	{

		if (init)
		{
			CRgbaquad colorBack = bmpViewer->GetBackColor();
			auto mat = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4,
			                   cv::Scalar(colorBack.GetBlue(), colorBack.GetGreen(), colorBack.GetRed(), 255));
			bitmapFirst->SetPicture(mat);
			bitmapOut.reset(GenerateInterpolationBitmapTexture(source, bmpViewer));
			if (bitmapOut != nullptr)
			{
				bitmapFirst->InsertBitmap(bitmapOut.get(), out.x, out.y);
				bitmapFirst->Flip();
			}

			pictureFirst->SetData(bitmapFirst->GetMatrix(), nullptr);
		}
	}

	
}


GLTexture* CPageCurlFilter::GetTexture(const int& numTexture)
{
	if (numTexture == 0)
		return pictureFirst.get();

	return pictureNext.get();
}
