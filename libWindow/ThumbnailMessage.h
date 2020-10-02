#pragma once

class CThumbnailMessage
{
public:
	int nbPhoto;
	int nbElement;
	int thumbnailPos;
	int typeMessage;
};

class CPictureInfosMessage
{
public:
	wxString filename;
	wxString infos;
};