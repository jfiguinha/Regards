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
		CThreadLoadInfos()
		{
			panelInfos = nullptr;
			infosFileWnd = nullptr;
			threadLoadInfos = nullptr;
		}

		CInfosFileWnd* panelInfos;
		CInfosFile* infosFileWnd;
		wxString filename;
		thread* threadLoadInfos;
	};

	class CInfosFileWnd : public CTreeWithScrollbar
	{
	public:
		CInfosFileWnd(wxWindow* parent, wxWindowID id, const CThemeScrollBar& themeScroll, const CThemeTree& theme);
		~CInfosFileWnd(void) override;
		void InfosUpdate(const wxString& filename);

	private:
		void UpdateTreeInfosEvent(wxCommandEvent& event);
		static void GenerateTreeInfos(CThreadLoadInfos* threadInfos);

		CInfosFile* infosFile;
		CInfosFile* oldInfosFileControl;
		wxString filename;
	};
}
