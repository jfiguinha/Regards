#pragma once
#include "InfosFile.h"
#include <TreeWithScrollbar.h>

using namespace Regards::Control;
using namespace std;
class CTreeElementTriangle;
class CTreeElementTexte;

namespace Regards::Control
{
	class CInfosFileWnd;

	class CThreadLoadInfos
	{
	public:
		CInfosFileWnd* panelInfos = nullptr;
		std::unique_ptr<CInfosFile> infosFileWnd;
		wxString filename;
		std::thread threadLoadInfos;
	};

	class CInfosFileWnd : public CTreeWithScrollbar
	{
	public:
		CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll, const CThemeTree& theme);
		~CInfosFileWnd(void) = default;
		void InfosUpdate(const wxString& filename);

	private:
		void UpdateTreeInfosEvent(wxCommandEvent& event);
		static void GenerateTreeInfos(CThreadLoadInfos* threadInfos);

		std::unique_ptr<CInfosFile> infosFile;
		wxString filename;
	};
}
