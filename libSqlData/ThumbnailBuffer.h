#pragma once

class CThumbnailBuffer
{

public:

	static wxImage GetPicture(const wxString& filename);
	static void RemovePicture(const wxString& filename);

private:

	static std::map<wxString, wxImage> listPicture;
	static std::vector<wxString> listFile;
	static std::mutex muPictureBuffer;
	static std::mutex muListFile;
};