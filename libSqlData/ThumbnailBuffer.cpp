#include "header.h"
#include "ThumbnailBuffer.h"
#include <libPicture.h>

std::map<wxString, CThumbnailBuffer::PictureStock> CThumbnailBuffer::listPicture;

wxImage CThumbnailBuffer::GetPicture(const wxString& filename)
{
	wxImage image;
	const clock_t begin_time = clock();
	float diff_min = 0;
	wxString keyToDelete = "";
	

	std::map<wxString, PictureStock>::iterator it = listPicture.find(filename);
	if (it == listPicture.end())
	{
		PictureStock pictureStock;
		image = Regards::Picture::CLibPicture::ReadThumbnail(filename);
		pictureStock.picture = image;
		pictureStock.date = begin_time;
		listPicture[filename] = pictureStock;
	}
	else
	{
		auto val = it->second;
		image = val.picture;
		val.date = begin_time;
	}


	if (CThumbnailBuffer::listPicture.size() > 100)
	{
		for (auto const& [key, val] : listPicture)
		{
			CThumbnailBuffer::PictureStock pictureStock = val;
			float diff = float(begin_time - pictureStock.date) / CLOCKS_PER_SEC;
			if (diff > diff_min)
			{
				diff_min = diff;
				keyToDelete = key;
			}
		}

		listPicture.erase(keyToDelete);
	}
	
	return image;
}