#pragma once
#include <Photos.h>

namespace Regards::Viewer
{
	class CImageList
	{
	public:
		CImageList()
		{
		};
		virtual ~CImageList();
		void SetImageList(const PhotosVector& pictures);
		int GetNbElement();
		CPhotos GetElement(const int& numElement, bool& isValid);
		wxString GetFilePath(const int& numElement, bool& isValid);
		int FindFileIndex(const wxString& filename);
		int FindFileIndex(const int& id);
		PhotosVector GetCopy();
		PhotosVector* GetPointer();
		void Lock();
		void Unlock();

	private:
		PhotosVector photolist;
		mutex muList;
	};
}
