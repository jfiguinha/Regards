#include "header.h"
#include "AfterEffect.h"


GLTexture* IAfterEffect::pictureFirst = nullptr;
GLTexture* IAfterEffect::pictureNext = nullptr;

IAfterEffect::IAfterEffect()
{
	if (pictureFirst == nullptr)
		pictureFirst = new GLTexture();
	if (pictureNext == nullptr)
		pictureNext = new GLTexture();
}