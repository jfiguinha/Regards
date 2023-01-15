#pragma once
#include <WindowMain.h>
#include "ValidationToolbar.h"
#include "ThumbnailFileSelection.h"
#include <ScrollbarWnd.h>
using namespace std;
using namespace Regards::Window;
class CImageLoadingFormat;


namespace Regards::Scanner
{
	class CThumbnailSelection : public CWindowMain
	{
	public:
		CThumbnailSelection(wxWindow* parent, wxWindowID id, wxString filename);
		~CThumbnailSelection() override;
		vector<int> GetSelectItem();
		void UpdateScreenRatio() override;

	private:
		void RedrawBarPos();
		void OnSize(wxSizeEvent& event) override;

		CScrollbarWnd* thumbscrollbar;
		CThumbnailFileSelection* thumbnailFileSelection;
		CValidationToolbar* validationToolbar;
		int width;
		int height;
	};
}
