#include <header.h>
#include "ToolbarKeyword.h"
#include <LibResource.h>
#include "ListFace.h"
#include <window_id.h>
#include <SqlCriteria.h>
#include <KeywordDialogBox.h>
using namespace Regards::Viewer;
using namespace Regards::Window;
using namespace Regards::Sqlite;
#define WM_ADDKEYWORD 1023
#define WM_REMOVEKEYWORD 1024


CToolbarKeyword::CToolbarKeyword(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical)
	: CToolbarWindow(parent, id, theme, vertical)
{
	themeToolbar = theme;
    wxString addkeyword = CLibResource::LoadStringFromResource(L"LBLADDKEYWORD",1);//L"History";
    wxString removekeyword = CLibResource::LoadStringFromResource(L"LBLREMOVEKEYWORD",1);

	CToolbarButton * add = new CToolbarButton(themeToolbar.button);
	add->SetButtonResourceId(L"IDB_PLUS");
	add->SetCommandId(WM_ADDKEYWORD);
	add->SetLibelleTooltip(addkeyword);
	navElement.push_back(add);

	CToolbarButton * moins = new CToolbarButton(themeToolbar.button);
	moins->SetButtonResourceId(L"IDB_MINUS");
	moins->SetCommandId(WM_REMOVEKEYWORD);
    moins->SetLibelleTooltip(removekeyword);
	navElement.push_back(moins);


}

CToolbarKeyword::~CToolbarKeyword()
{
}


void CToolbarKeyword::EventManager(const int &id)
{
	switch (id)
	{
		case WM_ADDKEYWORD:
		{
			wxTextEntryDialog dlg(this, wxT("New Keyword : \n"),
				wxT("Create a new keyword"), 
				"", wxOK | wxCANCEL);

			if (dlg.ShowModal() == wxID_OK)
			{
				bool isNew = false;
				// We can be certain that this string contains letters only.
				wxString value = dlg.GetValue();
				CSqlCriteria sqlCriteria;
				int id = sqlCriteria.GetOrInsertCriteriaId(1, 7, value, isNew);

				if (isNew)
				{
					wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
					wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
					wxQueueEvent(mainWnd, eventChange);
					
					wxWindow * keyword = this->FindWindowById(KEYWORDCRITERIAWINDOWID);
					wxCommandEvent * eventRefresh = new wxCommandEvent(wxEVENT_REFRESHDATA);
					wxQueueEvent(keyword, eventRefresh);
				}
			}
		}
		break;

		case WM_REMOVEKEYWORD:
		{
			CKeywordDialogBox keywordDialog(this);
			keywordDialog.ShowModal();
			if (keywordDialog.IsOk())
			{
				wxWindow * mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
				wxCommandEvent * eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
				wxQueueEvent(mainWnd, eventChange);

				wxWindow * keyword = this->FindWindowById(KEYWORDCRITERIAWINDOWID);
				wxCommandEvent * eventRefresh = new wxCommandEvent(wxEVENT_REFRESHDATA);
				wxQueueEvent(keyword, eventRefresh);
			}
		}
		break;
	}
}
