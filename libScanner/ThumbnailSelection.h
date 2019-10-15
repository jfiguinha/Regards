#pragma once
#include <WindowMain.h>
#include "ValidationToolbar.h"
#include "ThumbnailFileSelection.h"
using namespace std;
using namespace Regards::Window;
class CImageLoadingFormat;


namespace Regards
{
	namespace Scanner
	{

		class CThumbnailSelection : public CWindowMain
		{
		public:
			CThumbnailSelection(wxWindow* parent, wxWindowID id, wxString filename);
			~CThumbnailSelection();
			vector<int> GetSelectItem();

		private:

			void RedrawBarPos();
			void OnSize(wxSizeEvent& event);

			CScrollbarWnd * thumbscrollbar;
			CThumbnailFileSelection * thumbnailFileSelection;
			CValidationToolbar * validationToolbar;
			int width;
			int height;
		};
	}
}