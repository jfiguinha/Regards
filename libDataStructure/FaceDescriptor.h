#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class CFaceDescriptor
{
public:
	CFaceDescriptor()
	{
		descriptor = nullptr;
	};
	~CFaceDescriptor()
	{
		if(descriptor != nullptr)
			delete[] descriptor;
	};

	int numFace;
	char * descriptor;
	long size;
};