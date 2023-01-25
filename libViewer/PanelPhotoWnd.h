#pragma once
#include "ToolbarPhoto.h"
#include "TabWindow.h"
#include "CategoryFolderWindow.h"
using namespace std;
using namespace Regards::Window;
//using namespace Regards::Control;

class wxGenericDirCtrl;

namespace Regards::Viewer
{
	class CPanelPhotoWnd : public CTabWindow
	{
	public:
		CPanelPhotoWnd(wxWindow* parent, wxWindowID id);
		~CPanelPhotoWnd() override;

	protected:
		void SetFolder(wxCommandEvent& folderEvent);
		void OnSelChanged(wxCommandEvent& aEvent);
		void SaveParameter(wxCommandEvent& event);
		void UpdateCriteria(wxCommandEvent& event);
		void OnRefreshData(wxCommandEvent& event);

		void LoadInfo() override;


		void RemoveFolder(const wxString& folder);
		wxString AddFolder(const wxString& folder, const bool& showDialog = true);

		CCategoryFolderWindow* categoryFolderWnd;
		wxGenericDirCtrl* folderWnd;
		CToolbarPhoto* photoToolbar;
	};
}
