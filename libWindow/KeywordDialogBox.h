//
//  CalendarDialogBox.hpp
//  Regards.libExplorer
//
//  Created by figuinha jacques on 28/09/2015.
//  Copyright © 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/checklst.h>

namespace Regards
{
    namespace Window
    {

        class CKeywordDialogBox : public wxDialog
        {
        protected:
            wxStaticLine* m_staticLine15;
            wxButton* m_buttonOK;
            wxButton* m_buttonCancel;
			wxCheckListBox  * m_listKeyword;
            
        public:
			CKeywordDialogBox(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Select Keyword"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,200), long style = wxDEFAULT_DIALOG_STYLE);
            virtual ~CKeywordDialogBox();
			bool IsOk();
            void OnOk(wxCommandEvent& event);
			void OnCancel(wxCommandEvent& event);
        };
    }
}
