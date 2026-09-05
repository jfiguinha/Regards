#include "header.h"
#include "AfterEffect.h"

IAfterEffect::IAfterEffect()
{
	if (pictureFirst == nullptr)
		pictureFirst = std::make_unique<GLTexture>();
	if (pictureNext == nullptr)
		pictureNext = std::make_unique<GLTexture>();
}