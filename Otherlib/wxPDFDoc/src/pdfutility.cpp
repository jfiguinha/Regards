///////////////////////////////////////////////////////////////////////////////
// Name:        pdfutility.cpp
// Purpose:
// Author:      Ulrich Telle
// Created:     2009-05-20
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfutility.cpp Implementation of wxPdfUtility methods

// For compilers that support precompilation, includes <wx.h>.
#include <header.h>

// includes
#include "wx/intl.h"

#include "wx/pdfutility.h"

bool wxPdfUtility::ms_seeded = false;
int  wxPdfUtility::ms_s1     = 0;
int  wxPdfUtility::ms_s2     = 0;

#define MODMULT(a, b, c, m, s) q = s / a; s = b * (s - a * q) - c * q; if (s < 0) s += m

wxString
wxPdfUtility::GetUniqueId(const wxString& prefix)
{
  wxString uid = (prefix.Length() <= 114) ? prefix : prefix.Left(114);

  wxDateTime ts;
  ts.SetToCurrent();

  int q;
  int z;
  if (!ms_seeded)
  {
    ms_seeded = true;
    ms_s1 = ts.GetSecond() ^ (~ts.GetMillisecond());
    if (ms_s1 == 0) ms_s1 = 1;
    ms_s2 = wxGetProcessId();
  }
  MODMULT(53668, 40014, 12211, 2147483563L, ms_s1);
  MODMULT(52774, 40692,  3791, 2147483399L, ms_s2);

  z = ms_s1 - ms_s2;
  if (z < 1)
  {
    z += 2147483562;
  }

  uid += wxString::Format(wxS("%08x%05x"), ts.GetSecond(), ts.GetMillisecond());
  uid += Double2String(z * 4.656613e-9,8);

  return uid;
}

wxString
wxPdfUtility::Double2String(double value, int precision)
{
  wxString number;
#if wxCHECK_VERSION(2,9,1)
  number = wxString::FromCDouble(value, precision);
#else
  wxString format;
  if (precision == -1)
  {
    format = wxS("%g");
  }
  else // Use fixed precision.
  {
    format.Printf(wxS("%%.%df"), precision);
  }
  number = wxString::Format(format, value);
#if wxUSE_INTL
  wxString dpSeparator = wxLocale::GetInfo(wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER);
#else
  wxString dpSeparator(wxS(","));
#endif
  number.Replace(dpSeparator, wxS("."));
#endif
  return number;
}

double
wxPdfUtility::String2Double(const wxString& str)
{
  wxString value = str.Strip(wxString::both);
  double result = 0;
  double sign = 1;
  int scale = 0;
  int exponent = 0;
  int expsign = 1;
  int j = 0;
  int jMax = (int) value.Length();
  if (jMax > 0)
  {
    if (value[j] == wxS('+'))
    {
      j++;
    }
    else if (value[j] == wxS('-'))
    {
      sign = -1;
      j++;
    }
    while (j < jMax && wxIsdigit(value[j]))
    {
      result = result*10 + (value[j] - wxS('0'));
      j++;
    }
    if (j < jMax && value[j] == wxS('.'))
    {
      j++;
      while (j < jMax && wxIsdigit(value[j]))
      {
        result = result*10 + (value[j] - wxS('0'));
        scale++;
        j++;
      }
    }
    if (j < jMax && (value[j] == wxS('E') || value[j] == wxS('e')))
    {
      j++;
      if (value[j] == wxS('+'))
      {
        j++;
      }
      else if (value[j] == wxS('-'))
      {
        expsign = -1;
        j++;
      }
      while (j < jMax && wxIsdigit(value[j]))
      {
        exponent = exponent*10 + (value[j] - wxS('0'));
        j++;
      }
      exponent *= expsign;
    }
    result = sign * result * pow(10.0, exponent-scale);
  }
  return result;
}

wxString
wxPdfUtility::Convert2Roman(int value)
{
  wxString result = wxEmptyString;

  if (value > 0 && value < 4000)
  {
    static wxString romans = wxS("MDCLXVI");
    int pos = 6;  // Point to LAST character in 'romans'
    int currentDigit;

    while (value > 0)
    {
      currentDigit = value % 10;
      if (currentDigit == 4 || currentDigit == 9)
      {
        result.Prepend(romans.Mid(pos  - currentDigit / 4, 1));
        result.Prepend(romans.Mid(pos, 1));
      }
      else
      {
        int x = currentDigit % 5;
        while (x-- > 0)
        {
          result.Prepend(romans.Mid(pos, 1));
        }
        if (currentDigit >= 5)
        {
          result.Prepend(romans.Mid(pos - 1, 1));
        }
      }
      value /= 10;
      pos -= 2;
    }
  }
  else
  {
    result = wxS("???");
  }
  return result;
}

double
wxPdfUtility::ForceRange(double value, double minValue, double maxValue)
{
  if (value < minValue)
  {
    value = minValue;
  }
  else if (value > maxValue)
  {
    value = maxValue;
  }
  return value;
}

wxString
wxPdfUtility::RGB2String(const wxColour& colour)
{
  double r = colour.Red();
  double g = colour.Green();
  double b = colour.Blue();
  wxString rgb = Double2String(r/255.,3) + wxS(" ") +
                 Double2String(g/255.,3) + wxS(" ") +
                 Double2String(b/255.,3);
  return rgb;
}
