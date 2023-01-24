#include "header.h"
#include "ThumbnailBuffer.h"
#include <libPicture.h>

std::map<wxString, wxImage> CThumbnailBuffer::listPicture;
std::vector<wxString> CThumbnailBuffer::listFile;
std::mutex CThumbnailBuffer::muPictureBuffer;
std::mutex CThumbnailBuffer::muListFile;

wxImage CThumbnailBuffer::GetPicture(const wxString& filename)
{
	wxImage image;
	float diff_min = 0;
	wxString keyToDelete = "";

	muPictureBuffer.lock();

	std::map<wxString, wxImage>::iterator it = listPicture.find(filename);

	if (it == listPicture.end())
	{
		image = listPicture[filename] = Regards::Picture::CLibPicture::ReadThumbnail(filename);

		muListFile.lock();
		listFile.push_back(filename);
		muListFile.unlock();
	}
	else
	{

		image = it->second;
		muListFile.lock();
		std::vector<wxString>::iterator it = std::find(listFile.begin(), listFile.end(), filename);
		listFile.erase(it);
		listFile.push_back(filename);
		muListFile.unlock();
	}
	muPictureBuffer.unlock();

	if (listFile.size() > 100)
	{
		muListFile.lock();
		wxString firstFile = listFile[0];
		listFile.erase(listFile.begin());
		muListFile.unlock();

		muPictureBuffer.lock();
		std::map<wxString, wxImage>::iterator it = listPicture.find(firstFile);
		listPicture.erase(it);
		muPictureBuffer.unlock();
	}
	
	return image;
}