///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatacore.h
// Purpose:     Definition of font data for the Adobe core fonts
// Author:      Ulrich Telle
// Created:     2009-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatacore.h Definition of font data for the Adobe core fonts

#ifndef _PDF_FONT_DATA_CORE_H_
#define _PDF_FONT_DATA_CORE_H_

// wxWidgets headers
#include <wx/strconv.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

// Forward declarations
class WXDLLIMPEXP_FWD_PDFDOC wxPdfFontDescription;

class wxPdfKernPairDesc;

/// Class representing the Adobe core fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataCore : public wxPdfFontData
{
public :
	/// Default constructor
	wxPdfFontDataCore();

	/// Constructor
	/**
	* \param family the family name of the font
	* \param name the font name
	* \param alias the alias name of the font or an empty string
	* \param cwArray an array with the character widths
	* \param kpArray an array with kerning pairs
	* \param desc a font description
	*/
	wxPdfFontDataCore(const wxString& family, const wxString& name, const wxString& alias,
	                  short* cwArray, const wxPdfKernPairDesc* kpArray,
	                  const wxPdfFontDescription& desc);

	/// Default destructor
	~wxPdfFontDataCore() override;

	/// Get the character width array as string
	/**
	* \param subset flag whether subsetting is enabled
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the string representation of the character widths
	*/
	wxString GetWidthsAsString(bool subset = false, wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                           wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

	/// Get the width of a string
	/**
	* \param s the string for which the width should be calculated
	* \param encoding the character to glyph mapping
	* \param withKerning flag indicating whether kerning should be taken into account
	* \return the width of the string
	*/
	double GetStringWidth(const wxString& s, const wxPdfEncoding* encoding = nullptr,
	                      bool withKerning = false) const override;

	/// Check whether the font oan show all characters of a given string
	/**
	* \param s the string to be checked
	* \param encoding the character to glyph mapping
	* \return TRUE if the font can show all characters of the string, FALSE otherwise
	*/
	bool CanShow(const wxString& s, const wxPdfEncoding* encoding = nullptr) const override;

	/// Convert character codes to glyph numbers
	/**
	* \param s the string to be converted
	* \param encoding the character to glyph mapping
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the converted string
	*/
	wxString ConvertCID2GID(const wxString& s, const wxPdfEncoding* encoding = nullptr,
	                        wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                        wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

#if wxUSE_UNICODE
	/// Get the associated encoding converter
	/**
	* \return the encoding converter associated with this font
	*/
	wxMBConv* GetEncodingConv() const override;
#endif

protected:

private:
};

#endif
