#include <header.h>
#include "picture_utility.h"
#include <ConvertUtility.h>
#include <wx/wfstream.h>
uint8_t* CPictureUtility::readfile(const wxString& fileName, size_t& _fileSize)
{
	_fileSize = 0;
	/*
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	uint8_t* _compressedImage = nullptr;
	FILE* file;
	if ((file = fopen(CConvertUtility::ConvertToUTF8(fileName), "rb")) == nullptr)
		cout << "File Failed To Load\n";
	else
	{
		long prev = ftell(file);
		fseek(file, 0L, SEEK_END);
		_fileSize = ftell(file);
		fseek(file, prev, SEEK_SET);

		//Creating a buffer and saving it back
		_compressedImage = new uint8_t[_fileSize];
		//cout << "fileSize" << fileSize;
		fread(_compressedImage, _fileSize, 1, file);
		fclose(file);
	}
	*/
	wxFileInputStream input(fileName);
	wxMemoryOutputStream memOut(NULL);
	input.Read(memOut);
	uint8_t* _compressedImage = nullptr;
	wxStreamBuffer* buffer = memOut.GetOutputStreamBuffer();
	if (buffer != nullptr)
	{
		_fileSize = buffer->GetBufferSize();
		_compressedImage = new uint8_t[_fileSize];
		memcpy(_compressedImage, (char*)buffer->GetBufferStart(), buffer->GetBufferSize());
	}
	return _compressedImage;
}


void CPictureUtility::writefile(const wxString& fileName, uint8_t* data, const size_t & size)
{
	//const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	FILE* file;
	if ((file = fopen(CConvertUtility::ConvertToUTF8(fileName), "wb")) == nullptr)
		cout << "File Failed To Load\n";
	else
	{
		fwrite(data, size, 1, file);
		fclose(file);
	}
}