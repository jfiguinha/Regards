#pragma once
#include "Icone.h"

namespace Regards
{
	namespace Window
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
			void AddElement(CIcone* icone);
			wxString GetFilename(const int& numElement);
			CIcone* FindElement(const int& xPos, const int& yPos, pItemCompFonct* _pf, CWindowMain* parent);
		private:
			IconeVector pIconeList;
			//mutex muList;
		};
	}
}
