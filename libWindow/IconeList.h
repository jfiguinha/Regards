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
		CIcone* FindElement(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent);
		void SortById();
		CIcone* FindElementPhotoId(const int& photoId);
		CIcone* FindFaceElement(wxString filepath, int numFace, pItemCompFonctFace* _pf);
		bool FindElement(wxString filename);
		void SortByFilename();
		CIcone* GetLastElement();
		void RemoveElement(int numElement);
		CIcone * FindElementByFilename(wxString filename);
	private:
		IconeVector pIconeList;
		tbb::concurrent_unordered_map<wxString, CIcone*> pIconeByFilename;
		tbb::concurrent_unordered_map<int, CIcone*> pIconeByPhotoId;
		//mutex muList;
	};
}
