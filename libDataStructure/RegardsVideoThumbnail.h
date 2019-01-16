#pragma once

class CRegardsBitmap;

class CRegardsVideoThumbnail
{
public:

	CRegardsVideoThumbnail(){};
	~CRegardsVideoThumbnail(){};

	CRegardsBitmap * image = nullptr;
	int rotation = 0;
	int percent = 0;
	LONGLONG timePosition = 0;
};
