/////////////////////////////////////////////////////////////////////////////
// Name:        iasanedlg.h
// Purpose:     wxIASane dialog
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by:
// Created:     01/29/2003
// RCS-ID:      $Id: iasanedlg.h,v 1.1.1.1 2003/04/01 20:32:33 dbryson Exp $
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IASANEDLG_H_
#define _WX_IASANEDLG_H_

#ifdef __GNUG__
#pragma interface "iasanedlg.h"
#endif

#include "wx/dynarray.h"
#include "wx/splitter.h"

typedef struct
{
    SANE_Bool sane_bool;
    SANE_Int sane_int;
    SANE_Fixed sane_fixed;
    SANE_String sane_string;
    SANE_Status sane_status;
} SaneOptionValue;

WX_DEFINE_ARRAY(const SANE_Option_Descriptor*, SaneDescriptorArray);

class wxIASaneAcquireDialog : public wxDialog
{
public:
    wxIASaneAcquireDialog(wxWindow *parent, wxWindowID id,
        const wxString &title, wxSane *sane,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxSize(500, 400),
        const long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~wxIASaneAcquireDialog();

protected:
    //
    //  Event handlers
    //
    void OnOk(wxCommandEvent& event);
    void OnUpdateUI(wxUpdateUIEvent& event);

    //
    //  Misc helper methods
    //
    void GetOptionDescriptors();
    wxPanel *MakeSettingsPanel(wxWindow *parent);
    wxPanel *MakePreviewPanel(wxWindow *parent);
    wxString GetUnitString(SANE_Unit unit);
    void GetOptionValues();
    void SetOptionValues();

    wxSane *m_sane;
    SaneDescriptorArray m_descriptors;
    wxVector<SaneOptionValue> m_optionValues;
    wxWindow *m_optionControls;

private:
};

#endif // _WX_IASANEDLG_H_
