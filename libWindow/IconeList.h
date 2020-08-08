#include "Icone.h"

namespace Regards
{
	namespace Window
	{
	    class CIconeList
        {
        public:
            CIconeList(){};
            virtual ~CIconeList();
            int GetNbElement();
            CIcone * GetElement(const int &numElement);
            void EraseThumbnailList();
            void DestroyCacheThumbnailList();
            void AddElement(CIcone * icone);
			//void Lock();
			//void Unlock();
			CIcone * FindElement(const int &xPos, const int &yPos, pItemCompFonct * _pf, CWindowMain * parent);
        private:
            IconeVector pIconeList;
            //mutex muList;
        };
    }
}