#pragma once

class CThumbnailMessage
{
public:
	int nbPhoto;
	size_t nbElement;
	int thumbnailPos;
	int typeMessage;
};

class CPictureInfosMessage
{
public:
	wxString filename;
	wxString infos;
};
