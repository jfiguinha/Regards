#include <header.h>
#include "MoveEffectTexture.h"
#include <ImageLoadingFormat.h>
#include <picture_utility.h>
#include "BitmapDisplay.h"
#include "effect_id.h"
using namespace Regards::Filter;

void CMoveEffectTextureEffect::AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL,
                                           IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor,
                                           const bool& isNext, float& ratio)
{
	int pos;

	cv::Mat bitmapTemp = nextPicture->GetOpenCVPicture();
	int orientation = nextPicture->GetOrientation();
	CPictureUtility::RotateExif(bitmapTemp, orientation);

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp.size().width, bitmapTemp.size().height);
	int widthOutput = bitmapTemp.size().width * newRatio;
	int heightOutput = bitmapTemp.size().height * newRatio;



	if (pictureNext == nullptr || pictureNext->GetWidth() != widthOutput || pictureNext->GetHeight() != heightOutput)
	{
		GenerateEffectTexture(nextPicture, bmpViewer);
	}

	if (isNext)
	{
		int posMin = (bmpViewer->GetWidth() - out.width) / 2;
		pos = (posMin * scale_factor) + bmpViewer->GetWidth() * (static_cast<float>(100 - etape) / 100.0f);
		if (renderOpenGL != nullptr)
			renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos,
			                               out.y * scale_factor, false);
	}
	else
	{
		int xtexture = static_cast<float>(out.width) * scale_factor;
		pos = (((out.x + xtexture) * scale_factor) * (static_cast<float>(etape) / 100.0f)) - xtexture;
		if (renderOpenGL != nullptr)
			renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos,
			                               out.y * scale_factor, false);
	}
}

void CMoveEffectTextureEffect::RenderMoveTexture(int& x, int& y, GLTexture* glTexture, const int& etape,
                                                 const bool& isNext)
{
	if (etape > 0 && etape < 110)
	{
		if (isNext)
		{
			x = x - (static_cast<float>(x + glTexture->GetWidth()) * (static_cast<float>(etape) / 100.0f));
		}
		else
		{
			x = x + (static_cast<float>(x + glTexture->GetWidth()) * (static_cast<float>(etape) / 100.0f));
		}
	}
}

int CMoveEffectTextureEffect::GetTypeFilter()
{
	return IDM_AFTEREFFECT_MOVE;
}
