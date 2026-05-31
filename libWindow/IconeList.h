#pragma once
#include "Icone.h"
#include <tbb/concurrent_unordered_map.h>

namespace Regards::Window
{
	class CIconeList
	{
	public:
		CIconeList()
		{
		};
		virtual ~CIconeList();
		int GetNbElement();
		CIcone* GetElement(const int& numElement);
		int GetPhotoId(const int& numElement);
		void EraseThumbnailList();
        void EraseThumbnailListWithIcon();
		void AddElement(CIcone* icone);
		wxString GetFilename(const int& numElement);
		CIcone* FindElementByPosition(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent);
		CIcone* FindElementByPhotoId(const int& photoId);
		CIcone* FindElementByFilename(const wxString& filename);
        bool IfElementExistByFilename(const wxString& filename);
		CIcone* FindFaceElement(wxString filepath, int numFace, pItemCompFonctFace* _pf);
		CIcone* GetLastElement();
		void RemoveElement(int numElement);
        void SortById();
        void SortByFilename();
        
	private:
        
		IconeVector pIconeList;
		tbb::concurrent_unordered_map<int, CIcone*> pIconeByPhotoId;
        tbb::concurrent_unordered_map<wxString, CIcone*> pIconeByFilename;
	};
}
