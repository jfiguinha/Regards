///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdataopentype.h
// Purpose:    Definition of font data for OpenType fonts
// Author:      Ulrich Telle
// Created:     2009-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdataopentype.h Definition of font data for OpenType fonts

#ifndef _PDF_FONT_DATA_OPENTYPE_H_
#define _PDF_FONT_DATA_OPENTYPE_H_

// OpenType fonts are only available in Unicode build
#if wxUSE_UNICODE

// wxWidgets headers
#include <wx/strconv.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

/// Class representing Unicode OpenType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataOpenTypeUnicode : public wxPdfFontData
{
public:
	///< Default constructor
	wxPdfFontDataOpenTypeUnicode();

	/// Default destructor
	~wxPdfFontDataOpenTypeUnicode() override;

	/// Initialize font data
	/**
	* \return TRUE if the font data are initialized, FALSE otherwise
	*/
	bool Initialize() override;

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

	/// Convert glyph number to string
	/**
	* \param glyph the glyph to be converted
	* \param encoding the character to glyph mapping
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the converted string
	*/
	wxString ConvertGlyph(wxUint32 glyph, const wxPdfEncoding* encoding = nullptr,
	                      wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                      wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

	/// Get the character width array as string
	/**
	* \param subset flag whether subsetting is enabled
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the string representation of the character widths
	*/
	wxString GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                           wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

	/// Load the font metrics XML file
	/**
	* \param root the root node of the XML font metric file
	* \return TRUE if the metric file could be processed successfully, FALSE otherwise
	*/
	bool LoadFontMetrics(wxXmlNode* root) override;

	/// Write font data
	/**
	* \param fontData the output stream
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the size of the written font data
	*/
	size_t WriteFontData(wxOutputStream* fontData,
	                     wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                     wxPdfChar2GlyphMap* subsetGlyphs = nullptr) override;

	/// Write character/glyph to unicode mapping
	/**
	* \param mapData the output stream
	* \param encoding the character to glyph mapping
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the size of the written data
	*/
	size_t WriteUnicodeMap(wxOutputStream* mapData,
	                       const wxPdfEncoding* encoding = nullptr,
	                       wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                       wxPdfChar2GlyphMap* subsetGlyphs = nullptr) override;

	/// Get the associated encoding converter
	/**
	* \return the encoding converter associated with this font
	*/
	wxMBConv* GetEncodingConv() const override { return m_conv; }

	/// Create the associated default encoding converter
	void CreateDefaultEncodingConv() override;

	/// Set the offset of the CFF section
	/**
	* \param cffOffset the offset of the CFF section
	*/
	void SetCffOffset(size_t cffOffset) { m_cffOffset = cffOffset; }

	/// Set the lenght of the CFF section
	/**
	* \param cffLength the offset of the CFF section
	*/
	void SetCffLength(size_t cffLength) { m_cffLength = cffLength; }

	/// Set glyph widths array
	/**
	* \param glyphWidths array with glyph widths
	*/
	void SetGlyphWidths(const wxPdfArrayUint16& glyphWidths) override;

protected:
	wxPdfArrayUint16* m_gw; ///< Array of glyph widths
	wxMBConv* m_conv; ///< Associated encoding converter
};

#endif // wxUSE_UNICODE

#endif
