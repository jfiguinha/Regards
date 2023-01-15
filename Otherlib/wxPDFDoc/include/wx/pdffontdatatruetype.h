///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatatruetype.h
// Purpose:    Definition of font data for TrueType fonts
// Author:      Ulrich Telle
// Created:     2009-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatatruetype.h Definition of font data for TrueType fonts

#ifndef _PDF_FONT_DATA_TRUETYPE_H_
#define _PDF_FONT_DATA_TRUETYPE_H_

// wxWidgets headers
#include <wx/strconv.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

class WXDLLIMPEXP_FWD_PDFDOC wxPdfVolt;

/// Class representing TrueType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataTrueType : public wxPdfFontData
{
public:
	/// Default constructor
	wxPdfFontDataTrueType();

	/// Default destructor
	~wxPdfFontDataTrueType() override;

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

	/// Get the character width array as string
	/**
	* \param subset flag whether subsetting is enabled
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the string representation of the character widths
	*/
	wxString GetWidthsAsString(bool subset = false, wxPdfSortedArrayInt* usedGlyphs = nullptr,
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
	size_t WriteFontData(wxOutputStream* fontData, wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                     wxPdfChar2GlyphMap* subsetGlyphs = nullptr) override;

#if wxUSE_UNICODE
	/// Get the associated encoding converter
	/**
	* \return the encoding converter associated with this font
	*/
	wxMBConv* GetEncodingConv() const override { return m_conv; }

	/// Create the associated default encoding converter
	void CreateDefaultEncodingConv() override;
#endif

protected:
	wxMBConv* m_conv; ///< Assocated encoding converter
};

#if wxUSE_UNICODE

/// Class representing Unicode TrueType fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataTrueTypeUnicode : public wxPdfFontData
{
public:
	///< Default constructor
	wxPdfFontDataTrueTypeUnicode();

	/// Default destructor
	~wxPdfFontDataTrueTypeUnicode() override;

	/// Initialize font data
	/**
	* \return TRUE if the font data has been initialized successfully, FALSE otherwise
	*/
	bool Initialize() override;

	/// Check whether VOLT data are available
	/**
	* \return TRUE if the font data contain VOLT data, FALSE otherwise
	*/
	bool HasVoltData() const override { return m_volt != nullptr; }

	/// Apply VOLT data
	/**
	* \param s text string for which VOLT data should be applied
	* \return text string modified according to the VOLT data
	*/
	wxString ApplyVoltData(const wxString& s) const override;

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
	size_t WriteFontData(wxOutputStream* fontData, wxPdfSortedArrayInt* usedGlyphs = nullptr,
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

	/// Set glyph widths array
	/**
	* \param glyphWidths array with glyph widths
	*/
	void SetGlyphWidths(const wxPdfArrayUint16& glyphWidths) override;

protected:
	wxPdfArrayUint16* m_gw; ///< Array of glyph widths
	wxMBConv* m_conv; ///< Associated encoding converter
	wxPdfVolt* m_volt; ///< VOLT data
};

#endif // wxUSE_UNICODE

#endif
