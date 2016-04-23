#pragma once
#include <PaneInterface.h>
#include <ScrollbarWnd.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include <TreeWindow.h>
#include "CategoryWnd.h"
#include "ExplorerParam.h"
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CCategoryFolderWindow : public CWindowMain, public CPaneInterface
		{
		public:
			CCategoryFolderWindow(wxWindow* parent, wxWindowID id);
			~CCategoryFolderWindow();
			void UpdateCriteria();
            void UpdateScreenRatio();
			void UpdatePhotoList();
            void RefreshCriteriaSearch(wxCommandEvent& event);
            
		private:

			void Resize();
			void Init();

			CPane * paneCatalog = nullptr;
			CScrollbarWnd * catalogWndScroll = nullptr;
			CTreeWindow * treeWindow = nullptr;
			CCategoryWnd * catalogWndOld = nullptr;
			CExplorerParam * explorerconfig = nullptr;
			int oldPos = 0;
		};
	}
}

