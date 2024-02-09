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
		std::shared_ptr<CIcone> GetElement(const int& numElement);
		int GetPhotoId(const int& numElement);
		void EraseThumbnailList();
		void AddElement(std::shared_ptr<CIcone> icone);
		wxString GetFilename(const int& numElement);
		std::shared_ptr<CIcone> FindElement(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent);
		void SortById();
		std::shared_ptr<CIcone> FindElementPhotoId(const int& photoId);
		std::shared_ptr<CIcone> FindElement(wxString filename, pItemStringCompFonct * _pf);
		void SortByFilename();
	private:
		IconeVector pIconeList;
		//mutex muList;
	};
}
