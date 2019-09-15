///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontvolt.h
// Purpose:     Definition of VOLT font data
// Author:      Ulrich Telle
// Created:     2010-09-21
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontvolt.h Definition of VOLT font data

#ifndef _PDF_FONT_VOLT_H_
#define _PDF_FONT_VOLT_H_

// wxWidgets headers
#include <wx/dynarray.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

/// Class representing the Adobe core fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfVolt
{
public :
  /// Default constructor
  wxPdfVolt();

  /// Default destructor
  virtual ~wxPdfVolt();

  /// Load VOLT data
  void LoadVoltData(wxXmlNode* volt);

  /// Process VOLT rules
  wxString ProcessRules(const wxString& text);

protected:

private:
  wxArrayPtrVoid m_rules; ///< Array of VOLT rules
};

#endif
