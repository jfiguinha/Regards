#pragma once

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