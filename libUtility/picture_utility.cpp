#include <header.h>
#include "picture_utility.h"
#include "ConvertUtility.h"
#include <wx/wfstream.h>
uint8_t* CPictureUtility::readfile(const wxString& fileName, size_t& _fileSize)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	uint8_t* _compressedImage = nullptr;
	if (wxFile::Exists(fileName))
	{
		wxFile file(fileName);
		_fileSize = file.Length();
		_compressedImage = new uint8_t[_fileSize];
		if (file.IsOpened())
			file.Read(_compressedImage, _fileSize);
		file.Close();
	}
	return _compressedImage;
}


void CPictureUtility::writefile(const wxString& fileName, uint8_t* data, const size_t & size)
{
	uint8_t* _compressedImage = nullptr;
	wxFile file;
	if(file.Create(fileName, true, wxFile::write))
		file.Write(data, size);
	file.Close();
}