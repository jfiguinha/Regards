#include "PictureData.h"

CPictureData::~CPictureData()
{
	if (data != nullptr)
		delete[] data;
};
		


