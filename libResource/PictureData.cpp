#include "header.h"
#include "PictureData.h"

CPictureData::~CPictureData()
{
	if (data != nullptr)
		delete[] data;
};


		


