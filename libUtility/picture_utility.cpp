#include <header.h>
#include "picture_utility.h"
#include "ConvertUtility.h"
#include <wx/wfstream.h>
#include "FileUtility.h"
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

void CPictureUtility::VerifRectSize(cv::Rect& rect, cv::Mat& matSrc)
{
	rect.x = max(rect.x, 0);
	rect.y = max(rect.y, 0);

	bool is_inside = (rect & cv::Rect(0, 0, matSrc.cols, matSrc.rows)) == rect;
	if (!is_inside)
	{
		if ((rect.width + rect.x) > matSrc.cols)
		{
			rect.width = matSrc.cols - rect.x - 1;
		}

		if ((rect.height + rect.y) > matSrc.rows)
		{
			rect.height = matSrc.rows - rect.y - 1;
		}
	}
}


wxString CPictureUtility::GetPhotoCancel()
{
#ifdef WIN32
	return CFileUtility::GetResourcesFolderPath() + "\\photo_cancel.png";
#else
	return CFileUtility::GetResourcesFolderPath() + "/photo_cancel.png";
#endif
}