#include <header.h>
#include "WaveTextureFilter.h"
#include <RGBAQuad.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <GLTexture.h>
#include "effect_id.h"

using namespace Regards::Filter;
using namespace Regards::OpenGL;

CWaveTextureFiltre::CWaveTextureFiltre()
{
	initTexture = true;
}

CWaveTextureFiltre::~CWaveTextureFiltre()
{}

void CWaveTextureFiltre::RenderTexture(CRenderBitmapOpenGL* renderOpenGL, const float& time, const float& invert,
	const int& width, const int& height, const int& left, const int& top)
{
	glActiveTexture(GL_TEXTURE0);
	pictureFirst->Enable();

	glActiveTexture(GL_TEXTURE1);
	pictureNext->Enable();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Liaison avec le nouveau Shader de Vagues
	COpenGLShader* shader = renderOpenGL->FindShader(L"IDR_GLSL_WAVE", GL_FRAGMENT_SHADER);
	if (shader != nullptr)
	{
		shader->EnableShader(renderOpenGL->GetProjectionMatrix());

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

	renderOpenGL->GetRenderOpengl()->RenderQuad(pictureFirst.get(), width, height, false, false, left, top, false);

	if (shader != nullptr)
		shader->DisableShader();

	glDisable(GL_BLEND);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int CWaveTextureFiltre::GetTypeFilter()
{
	// À remplacer par votre ID d'effet personnalisé (ex: IDM_AFTEREFFECT_WAVE) si nécessaire
	return IDM_AFTEREFFECT_WAVE;
}

void CWaveTextureFiltre::SetTransitionBitmap(const bool& start, IBitmapDisplay* bmpViewer, CImageLoadingFormat* bmpSecond)
{
	initTexture = true;
	bmpViewer->StartTransitionEffect(bmpSecond, false);
}

bool CWaveTextureFiltre::RenderTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source,
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

void CWaveTextureFiltre::GenerateTexture(CImageLoadingFormat* nextPicture, CImageLoadingFormat* source, IBitmapDisplay* bmpViewer)
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

GLTexture* CWaveTextureFiltre::GetTexture(const int& numTexture)
{
	if (numTexture == 0)
		return pictureFirst.get();

	return pictureNext.get();
}