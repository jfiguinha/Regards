///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontdatacore.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2008-08-07
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontdatacore.cpp Implementation of PDF core fonts

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>

// includes

#include "wx/pdfcorefontdata.h"
#include "wx/pdffontdatacore.h"
#include "wx/pdffontdescription.h"
#include "wx/pdfencoding.h"

wxPdfFontDataCore::wxPdfFontDataCore(const wxString& family, const wxString& alias, const wxString& name,
                                     short* cwArray, const wxPdfKernPairDesc* kpArray,
                                     const wxPdfFontDescription& desc)
  : wxPdfFontData()
{
  m_type   = wxS("core");
  m_family = family;
  m_alias  = alias;
  m_name   = name;
  m_fullNames.Add(name);
  m_desc  = desc;
  m_style = FindStyleFromName(name);

  if (cwArray != NULL)
  {
    m_cw = new wxPdfGlyphWidthMap();
    int j;
    for (j = 0; j <256; j++)
    {
      (*m_cw)[j] = cwArray[j];
    }
  }

  if (kpArray != NULL)
  {
    m_kp = new wxPdfKernPairMap();
    wxPdfKernWidthMap* kwMap = NULL;
    wxPdfKernWidthMap::iterator kw;
    wxUint32 u1, u2;
    wxUint32 u1prev = 0;
    size_t k = 0;
    while ((u1 = kpArray[k].unicode1) != 0 && (u2 = kpArray[k].unicode2) != 0)
    {
      if (u1 != u1prev)
      {
        u1prev = u1;
        wxPdfKernPairMap::iterator kp = (*m_kp).find(u1);
        if (kp == (*m_kp).end())
        {
          kwMap = new wxPdfKernWidthMap();
          (*m_kp)[u1] = kwMap;
        }
        else
        {
          kwMap = kp->second;
        }
      }
      (*kwMap)[u2] = kpArray[k].kerning;
      ++k;
    }
  }

  m_initialized = true;
}

wxPdfFontDataCore::~wxPdfFontDataCore()
{
}

wxString
wxPdfFontDataCore::GetWidthsAsString(bool subset, wxPdfSortedArrayInt* usedGlyphs, wxPdfChar2GlyphMap* subsetGlyphs) const
{
  wxUnusedVar(subset);
  wxUnusedVar(usedGlyphs);
  wxUnusedVar(subsetGlyphs);
  wxString s = wxString(wxS("["));
  int i;
  for (i = 32; i <= 255; i++)
  {
    s += wxString::Format(wxS("%u "), (*m_cw)[i]);
  }
  s += wxString(wxS("]"));
  return s;
}

#if wxUSE_UNICODE
wxMBConv*
wxPdfFontDataCore::GetEncodingConv() const
{
  wxMBConv* conv = &wxConvISO8859_1;
  return conv;
}
#endif

double
wxPdfFontDataCore::GetStringWidth(const wxString& s, const wxPdfEncoding* encoding, bool withKerning) const
{
  wxUnusedVar(encoding);
  double w = 0;
  // Get width of a string in the current font
  wxString t = ConvertCID2GID(s);

  wxString::const_iterator ch;
  for (ch = t.begin(); ch != t.end(); ++ch)
  {
    w += (*m_cw)[*ch];
  }
  if (withKerning)
  {
    int kerningWidth = GetKerningWidth(t);
    if (kerningWidth != 0)
    {
      w += (double) kerningWidth;
    }
  }
  return w / 1000;
}

bool
wxPdfFontDataCore::CanShow(const wxString& s, const wxPdfEncoding* encoding) const
{
  bool canShow = true;
  const wxPdfChar2GlyphMap* usedMap = NULL;
  if (encoding != NULL)
  {
    usedMap = encoding->GetEncodingMap();
  }
  if (usedMap == NULL)
  {
    usedMap = m_encoding->GetEncodingMap();
  }
  if (usedMap != NULL)
  {
    wxPdfChar2GlyphMap::const_iterator charIter;
    wxString::const_iterator ch;
    for (ch = s.begin(); canShow && ch != s.end(); ++ch)
    {
      canShow = (usedMap->find(*ch) != usedMap->end());
    }
  }
  return canShow;
}

wxString
wxPdfFontDataCore::ConvertCID2GID(const wxString& s,
                                  const wxPdfEncoding* encoding,
                                  wxPdfSortedArrayInt* usedGlyphs,
                                  wxPdfChar2GlyphMap* subsetGlyphs) const
{
  // No conversion from cid to gid
  wxUnusedVar(usedGlyphs);
  wxUnusedVar(subsetGlyphs);
#if wxUSE_UNICODE
  const wxPdfChar2GlyphMap* convMap = FindEncodingMap(encoding);
  wxString t;
  if (convMap != NULL)
  {
    wxPdfChar2GlyphMap::const_iterator charIter;
    wxString::const_iterator ch;
    for (ch = s.begin(); ch != s.end(); ++ch)
    {
      charIter = (*convMap).find(*ch);
      if (charIter != (*convMap).end())
      {
#if wxCHECK_VERSION(2,9,0)
        t.Append(wxUniChar(charIter->second));
#else
        t.Append(wxChar(charIter->second));
#endif
      }
      else
      {
        t += wxS("?");
      }
    }
  }
  else
  {
    t = s;
  }
  return t;
#else
  // Return unchanged string in ANSI build
  wxUnusedVar(encoding);
  return s;
#endif
}
