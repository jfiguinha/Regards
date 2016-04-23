#pragma once
#include <stdint.h>

class CPictureData
{
public:
	CPictureData(){};
	~CPictureData();
		
	uint8_t * data = nullptr;
	int	size;
	int width;
	int height;
};


