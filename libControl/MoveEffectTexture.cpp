#include <header.h>
#include "MoveEffectTexture.h"
#include <ImageLoadingFormat.h>
#include <RegardsBitmap.h>
#include "BitmapDisplay.h"
#include <FilterData.h>
using namespace Regards::Filter;

void CMoveEffectTextureEffect::AfterRender(CImageLoadingFormat* nextPicture, CRenderBitmapOpenGL* renderOpenGL, IBitmapDisplay* bmpViewer, const int& etape, const float& scale_factor, const bool& isNext, float& ratio)
{
	int xtexture = 0;
	int pos = 0;

	CRegardsBitmap* bitmapTemp = nextPicture->GetRegardsBitmap(true);
	int orientation = nextPicture->GetOrientation();
	bitmapTemp->RotateExif(orientation);
	//bitmapTemp->SetAlphaValue(0);

	float newRatio = bmpViewer->CalculPictureRatio(bitmapTemp->GetBitmapWidth(), bitmapTemp->GetBitmapHeight());
	int widthOutput = bitmapTemp->GetBitmapWidth() * newRatio;
	int heightOutput = bitmapTemp->GetBitmapHeight() * newRatio;

	delete bitmapTemp;


	if (pictureNext == nullptr || pictureNext->GetWidth() != widthOutput || pictureNext->GetHeight() != heightOutput)
	{
		GenerateEffectTexture(nextPicture, bmpViewer);
	}

	if (isNext)
	{
		int posMin = (bmpViewer->GetWidth() - out.width) / 2;
		pos = (posMin * scale_factor) + bmpViewer->GetWidth() * ((float)(100 - etape) / 100.0f);
		if (renderOpenGL != nullptr)
			renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos, out.y * scale_factor, false);

	}
	else
	{
		//xtexture = (float)(out.width) * scale_factor;
		//pos = out.x - (bmpViewer->GetWidth() * ((float)(etape) / 100.0f));
		xtexture = (float)(out.width) * scale_factor;
		pos = (((out.x + xtexture) * scale_factor) * ((float)(etape) / 100.0f)) - xtexture;
		if (renderOpenGL != nullptr)
			renderOpenGL->ShowSecondBitmap(GetTexture(0), out.width * scale_factor, out.height * scale_factor, pos, out.y * scale_factor, false);

	}
}

void CMoveEffectTextureEffect::RenderMoveTexture(int& x, int& y, GLTexture* glTexture, const int& etape, const bool& isNext)
{
	if (etape > 0 && etape < 110)
	{
		if (isNext)
		{
			x = x - ((float)(x + glTexture->GetWidth()) * ((float)(etape) / 100.0f));
		}
		else
		{
			x = x + ((float)(x + glTexture->GetWidth()) * ((float)(etape) / 100.0f));
		}
	}
}

int CMoveEffectTextureEffect::GetTypeFilter()
{
	return IDM_AFTEREFFECT_MOVE;
}