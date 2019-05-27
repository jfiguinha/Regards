#pragma once
#include "ToolbarPhoto.h"
#include <ScrollbarWnd.h>
#include "TabWindow.h"
#include "ListFace.h"
#include <directoryctrl.h>
#include "CategoryFolderWindow.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{       
		class CPanelPhotoWnd : public CTabWindow
		{
		public:
			CPanelPhotoWnd(wxWindow* parent, wxWindowID id);
			~CPanelPhotoWnd();
			void RefreshData();
		protected:
        
			void SetFolder(wxCommandEvent& folderEvent);
			void OnSelChanged(wxCommandEvent& aEvent);
			void SaveParameter(wxCommandEvent& event);
			void UpdateCriteria(wxCommandEvent& event);

			void LoadInfo();
            
			CCategoryFolderWindow * categoryFolderWnd;
			wxGenericDirCtrl * folderWnd;
            CToolbarPhoto * photoToolbar;
		};

	}
}