///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatatype1.h
// Purpose:     Definition of font data for Type1 fonts
// Author:      Ulrich Telle
// Created:     2009-05-15
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatatype1.h Definition of font data for Type1 fonts

#ifndef _PDF_FONT_DATA_TYPE1_H_
#define _PDF_FONT_DATA_TYPE1_H_

// wxWidgets headers
#include <wx/strconv.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/xml/xml.h>

// wxPdfDocument headers
#include "wx/pdfdocdef.h"
#include "wx/pdfarraytypes.h"
#include "wx/pdffontdata.h"

WX_DECLARE_STRING_HASH_MAP_WITH_DECL(wxUint16, wxPdfFontType1GlyphWidthMap, class WXDLLIMPEXP_PDFDOC);

/// Class representing Type 1 fonts. (For internal use only)
class WXDLLIMPEXP_PDFDOC wxPdfFontDataType1 : public wxPdfFontData
{
public:
	///< Default constructor
	wxPdfFontDataType1(wxMemoryInputStream* pfbStream = nullptr);

	/// Default destructor
	~wxPdfFontDataType1() override;

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
	wxString ConvertCID2GID(const wxString& s, const wxPdfEncoding* encoding,
	                        wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                        wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

	/// Load the font metrics XML file
	/**
	* \param root the root node of the XML font metric file
	* \return TRUE if the metric file could be processed successfully, FALSE otherwise
	*/
	bool LoadFontMetrics(wxXmlNode* root) override;

	/// Initialize font data
	/**
	* \return TRUE if the font data has been initialized successfully, FALSE otherwise
	*/
	bool Initialize() override;

	/// Get the character width array as string
	/**
	* \param subset flag whether subsetting is enabled
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the string representation of the character widths
	*/
	wxString GetWidthsAsString(bool subset = false, wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                           wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const override;

#if wxUSE_UNICODE
	/// Get the character width array as string
	/**
	* \param glyphNames the list of glyph names available in the font
	* \param subset flag whether subsetting is enabled
	* \param usedGlyphs the list of used glyphs
	* \param subsetGlyphs the mapping of glyphs to subset glyphs
	* \return the string representation of the character widths
	*/
	virtual wxString GetWidthsAsString(const wxArrayString& glyphNames, bool subset = false,
	                                   wxPdfSortedArrayInt* usedGlyphs = nullptr,
	                                   wxPdfChar2GlyphMap* subsetGlyphs = nullptr) const;

	/// Get a list of glyph names available in the font
	/**
	* \param[out] glyphNames the list of glyph names
	* \return TRUE if the glyph names are available, FALSE otherwise
	*/
	bool GetGlyphNames(wxArrayString& glyphNames) const override;
#endif

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

#if wxUSE_UNICODE
	/// Set the encoding type
	/**
	* \param encodingType the encoding type
	*/
	void SetEncodingType(const wxString& encodingType) { m_encodingType = encodingType; }

	/// Get the encoding type
	/**
	* \return the encoding type
	*/
	wxString GetEncodingType() const { return m_encodingType; }

	/// Set encoding map
	/**
	* \param encodingMap the encoding map
	*/
	void SetEncodingMap(const wxArrayString& encodingMap) { m_encodingMap = encodingMap; }

	/// Get encoding map
	/**
	* \return the encoding map
	*/
	wxArrayString GetEncodingMap() const { return m_encodingMap; }

	/// Set glyph width map
	/**
	* \param glyphWidthMap the map of glyph widths
	*/
	void SetType1GlyphWidthMap(wxPdfFontType1GlyphWidthMap* glyphWidthMap) { m_glyphWidthMap = glyphWidthMap; }

	/// Get the associated encoding converter
	/**
	* \return the encoding converter associated with this font
	*/
	wxMBConv* GetEncodingConv() const override { return m_conv; }

	/// Create the associated default encoding converter
	void CreateDefaultEncodingConv() override;
#endif

protected:
	/// Compress the font data
	bool CompressFontData(wxOutputStream* fontData, wxInputStream* pfbFile);

#if wxUSE_UNICODE
	wxString m_encodingType; ///< encoding type
	wxArrayString m_encodingMap; ///< encoding map
	wxArrayString m_glyphNames; ///< list of glyph names
	wxPdfFontType1GlyphWidthMap* m_glyphWidthMap; ///< mapping of glyph widths
#endif

	wxMemoryInputStream* m_pfbStream; ///< Font stream in PFB format
	wxMBConv* m_conv; ///< Associated encoding converter
};

#endif
