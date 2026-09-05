#include <header.h>
#include "ZoomEffectTexture.h"
#include <RGBAQuad.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <GLTexture.h>
#include "effect_id.h"

using namespace Regards::Filter;
using namespace Regards::OpenGL;

CZoomEffectTexture::CZoomEffectTexture()
{
	initTexture = true;
}

CZoomEffectTexture::~CZoomEffectTexture()
{}

void CZoomEffectTexture::RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert,
	const int& width, const int& height, const int& left, const int& top)
{
	// 1. Configuration des unités de multi-texturing
	glActiveTexture(GL_TEXTURE0);
	pictureFirst->Enable(); // Texture Source reliée à l'unité 0

	glActiveTexture(GL_TEXTURE1);
	pictureNext->Enable();  // Texture Cible (Next) reliée à l'unité 1

	// Le blending reste actif pour assurer la transparence lors du cross-fade interne du shader
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 2. Chargement du nouveau shader Zoom
	COpenGLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_ZOOM", GL_FRAGMENT_SHADER);
	if (shader != nullptr)
	{
		shader->EnableShader(renderOpenGL->GetProjectionMatrix());

		// Envoi des échantillonneurs et paramètres au Shader
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

	// 3. Dessin géométrique du Quad de rendu
	renderOpenGL->GetRenderOpengl()->RenderQuad(pictureFirst.get(), width, height, false, false, left, top, false);

	// 4. Reset et nettoyage
	if (shader != nullptr)
		shader->DisableShader();

	glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int CZoomEffectTexture::GetTypeFilter()
{
	// Remplacer par votre constante d'effet (ex: IDM_AFTEREFFECT_ZOOM)
	return IDM_AFTEREFFECT_ZOOM;
}

void CZoomEffectTexture::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	initTexture = true;
	bmpViewer->StartTransitionEffect(bmpSecond, false);
}

bool CZoomEffectTexture::RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
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

void CZoomEffectTexture::GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer)
{
	bool init = false;
	std::unique_ptr<CImageLoadingFormat> bitmapOut;
	std::unique_ptr<CImageLoadingFormat> bitmapFirst = std::make_unique<CImageLoadingFormat>();

	if (initTexture || (pictureFirst->GetWidth() != bmpViewer->GetWidth() && pictureFirst->GetHeight() != bmpViewer->GetHeight()))
	{
		init = true;
		initTexture = false;
	}

	if (init)
	{
		// Génération de la Texture Cible (Next Picture)
		CRgbaquad colorBack = bmpViewer->GetBackColor();
		auto mat = cv::Mat(bmpViewer->GetHeight(), bmpViewer->GetWidth(), CV_8UC4,
			cv::Scalar(colorBack.GetBlue(), colorBack.GetGreen(), colorBack.GetRed(), 255));
		bitmapFirst->SetPicture(mat);
		bitmapOut.reset(GenerateInterpolationBitmapTexture(nextPicture, bmpViewer));
		if (bitmapOut != nullptr)
		{
			bitmapFirst->InsertBitmap(bitmapOut.get(), out.x, out.y);
		}

		mat = bitmapFirst->GetMatrix().getMat();
		cv::flip(mat, mat, 0);
		Regards::Picture::CPictureArray pictureArray = Regards::Picture::CPictureArray(mat);
		pictureNext->SetData(pictureArray, nullptr);
	}

	if (init)
	{
		// Génération de la Texture Source
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

GLTexture* CZoomEffectTexture::GetTexture(const int& numTexture)
{
	if (numTexture == 0)
		return pictureFirst.get();

	return pictureNext.get();
}