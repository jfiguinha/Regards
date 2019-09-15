///////////////////////////////////////////////////////////////////////////////
// Name:        pdffontvolt.cpp
// Purpose:     Implementation of VOLT data handling
// Author:      Ulrich Telle
// Created:     2010-06-23
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdffontvolt.cpp Implementation of the VOLT data handling

// For compilers that support precompilation, includes <wx/wx.h>.
#include <header.h>

#include <wx/regex.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "wx/pdffontvolt.h"

class wxPdfVoltRule
{
public :
  /// Default constructor
  wxPdfVoltRule()
    : m_repeat(false), m_match(wxS("")), m_replace(wxS(""))
  {
  }

  wxPdfVoltRule(bool repeat, const wxString& match, const wxString& replace)
    : m_repeat(repeat), m_match(match), m_replace(replace)
  {
    m_re.Compile(m_match);
  }

  ~wxPdfVoltRule()
  {
  }

public:
  bool     m_repeat;
  wxString m_match;
  wxString m_replace;
  wxRegEx  m_re;
};

wxPdfVolt::wxPdfVolt()
{
}

wxPdfVolt::~wxPdfVolt()
{
  size_t n = m_rules.GetCount();
  size_t j;
  for (j = 0; j < n; ++j)
  {
    delete (wxPdfVoltRule*) m_rules.Item(j);
  }
}

void
wxPdfVolt::LoadVoltData(wxXmlNode* volt)
{
  wxString repeat, match, replace;
  bool doRepeat;
  wxXmlNode* child = volt->GetChildren();
  while (child)
  {
    if (child->GetName() == wxS("ruleset"))
    {
      wxXmlNode* rule = child->GetChildren();
      while (rule)
      {
        if (rule->GetName() == wxS("rule"))
        {
#if wxCHECK_VERSION(2,9,0)
          repeat  = rule->GetAttribute(wxS("repeat"), wxS("false"));
          match   = rule->GetAttribute(wxS("match"), wxS(""));
          replace = rule->GetAttribute(wxS("replace"), wxS(""));
#else
          repeat  = rule->GetPropVal(wxS("repeat"), wxS("false"));
          match   = rule->GetPropVal(wxS("match"), wxS(""));
          replace = rule->GetPropVal(wxS("replace"), wxS(""));
#endif
          doRepeat = repeat.IsSameAs(wxS("true"));
          wxPdfVoltRule* voltRule = new wxPdfVoltRule(doRepeat, match, replace);
          m_rules.Add(voltRule);
        }
        rule = rule->GetNext();
      }
    }
    child = child->GetNext();
  }
}

wxString
wxPdfVolt::ProcessRules(const wxString& text)
{
#if 0
  wxFFileOutputStream dbgOut( wxS("d:/temp/pdfdoc-indic-dbg.txt"), wxS("a"));
  wxTextOutputStream dbgIndic( dbgOut );

  wxString str;
#endif
  wxString processText = text;
  size_t n = m_rules.GetCount();
  size_t j;
  for (j = 0; j < n; ++j)
  {
    wxPdfVoltRule* rule = (wxPdfVoltRule*) m_rules.Item(j);
    int matchCount;
    do
    {
      matchCount = rule->m_re.Replace(&processText, rule->m_replace);
#if 0
      str = wxEmptyString;
      wxString::const_iterator ch;
      for (ch = processText.begin(); ch != processText.end(); ++ch)
      {
        str += wxString::Format(wxS(" %04x"), *ch);
      }
      str += wxS("\n");
      dbgIndic.WriteString(str);
#endif
    }
    while (rule->m_repeat && matchCount > 0);
  }
#if 0
  dbgOut.Close();
#endif
  return processText;
}
