#pragma once
#include "Icone.h"

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
		CIcone* FindElement(wxString filename, pItemStringCompFonct * _pf);
		bool FindElement(wxString filename);
		void SortByFilename();
	private:
		IconeVector pIconeList;
		//mutex muList;
	};
}
