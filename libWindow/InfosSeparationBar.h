#pragma once
#include "Icone.h"
#include <algorithm>
#include <ThumbnailData.h>
#include <Photos.h>
using namespace std;
class CRegardsBitmap;
class CThumbnailData;


namespace Regards
{
	namespace Window
	{
		class CInfosSeparationBar
		{
		public:
			CInfosSeparationBar(const CThemeInfosSeparationBar &theme);
			virtual ~CInfosSeparationBar(void);

			inline bool operator== (const CInfosSeparationBar &n1);

            const wxString & GetTitle()
            {
                return title;
            }
			void Clear();
			void SetTitle(const wxString &title);
			void SetWindowPos(const int &x, const int &y);
			void SetWidth(const int &width);
			virtual void OnClick(const int &x, const int &y);
			int GetXPos();
			int GetYPos();
			const int & GetWidth();
			const int & GetHeight();
			wxRect GetPos();

			virtual void RenderIcone(wxDC * dc);
			
			vector<int> listElement;

		protected:

			void RenderTitle(wxDC * dc);

			int x;
			int y;
			int width;

			CThemeInfosSeparationBar theme;
			
			wxString title;

			
		};
		typedef std::vector<CInfosSeparationBar *> InfosSeparationBarVector;

	}
}
