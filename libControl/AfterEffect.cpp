#include "header.h"
#include "AfterEffect.h"


std::unique_ptr<GLTexture> IAfterEffect::pictureFirst = nullptr;
std::unique_ptr<GLTexture> IAfterEffect::pictureNext = nullptr;

IAfterEffect::IAfterEffect()
{
	if (pictureFirst == nullptr)
		pictureFirst = std::make_unique<GLTexture>();
	if (pictureNext == nullptr)
		pictureNext = std::make_unique<GLTexture>();
}