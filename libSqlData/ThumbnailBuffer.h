#pragma once

class CThumbnailBuffer
{

public:

	static wxImage GetPicture(const wxString& filename);

private:
	struct PictureStock
	{
		wxImage picture;
		clock_t  date;
	};
	static std::map<wxString, PictureStock> listPicture;
};