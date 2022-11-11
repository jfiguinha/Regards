#pragma once

class CPictureUtility
{
public:
	static uint8_t* readfile(const wxString& fileName, size_t& _fileSize);
	static void writefile(const wxString& fileName, uint8_t* data, const size_t& size);
	static wxString GetPhotoCancel();
};