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

#pragma once
#ifdef __WXSCANSANE__

class wxScanSane;
#include "wx/dynarray.h"
#include "wx/splitter.h"
#include "wxImagePanel.h"
#include "Scanner.h"

typedef struct
{
    SANE_Bool sane_bool;
    SANE_Int sane_int;
    SANE_Fixed sane_fixed;
    SANE_String sane_string;
    SANE_Status sane_status;
} SaneOptionValue;

WX_DEFINE_ARRAY(const SANE_Option_Descriptor*, SaneDescriptorArray);

class CChoiceValue
{
public:
    int id;
    wxString title;
};

class wxScanSaneAcquireDialog : public wxDialog
{
public:
    wxScanSaneAcquireDialog(wxWindow *parent, wxWindowID id,
        const wxString &title, wxScanSane *sane,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxSize(500, 400),
        const long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    ~wxScanSaneAcquireDialog();
    wxImage GetImage();

protected:
    //
    //  Event handlers
    //
    void OnOk(wxCommandEvent& event);
    void OnScan(wxCommandEvent& event);
    void OnPreview(wxCommandEvent& event);
    void OnUpdateUI(wxUpdateUIEvent& event);
    void OnChoiceSelected(wxCommandEvent& event);
    //
    //  Misc helper methods
    //
    void GetOptionDescriptors();
    wxPanel *MakeSettingsPanel(wxWindow *parent);
    wxPanel *MakePreviewPanel(wxWindow *parent);
    wxString GetUnitString(SANE_Unit unit);
    void GetOptionValues();
    void SetOptionValues();

    wxImage scanImage;
    wxImagePanel * panelPreview;
    wxScanSane *m_sane;
    SaneDescriptorArray m_descriptors;
    wxVector<SaneOptionValue> m_optionValues;
    wxWindow *m_optionControls;
    wxVector<CChoiceValue> listchoiceValue;
    int dpi = 75;
    Scanner::ScanMode color = Scanner::ScanMode::GRAY;

private:
};

#endif