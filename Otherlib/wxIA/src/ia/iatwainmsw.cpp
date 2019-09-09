/////////////////////////////////////////////////////////////////////////////
// Name:        iatwainmsw.cpp
// Purpose:     wxIATwainMSWProvider
// Author:      Derry Bryson, Technology Associates, Inc.
// Modified by: Michael Langfermann
// Created:     01/24/2003
// Copyright:   (c) 2003 Derry Bryson
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#include <header.h>

#ifdef WIN32

#ifdef __GNUG__
    #pragma implementation "iatwainmsw.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include <wx/log.h>

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#include <wx/msw/private.h>
#include <wx/image.h>
#include <wx/ia/ia.h>
#include <wx/ia/iatwainmsw.h>
#include <wx/ia/twainmsw.h>

IMPLEMENT_CLASS(wxIATwainMSWProvider, wxIAProvider)

wxIATwainMSWProvider::wxIATwainMSWProvider() :
    wxTwainMSWClient(0)
{
    m_monitor = NULL;
    SetTwain(new wxTwainMSW(this, wxTheApp->GetAppName(), wxEmptyString,
        wxTheApp->GetVendorName()));

    if (GetTwain())
        GetTwain()->Startup();
}

wxIATwainMSWProvider::~wxIATwainMSWProvider()
{
    if (GetTwain())
        GetTwain()->Shutdown();

    delete GetTwain();
}

bool wxIATwainMSWProvider::Ok()
{
    return GetTwain() && GetTwain()->Ok();
}

wxString wxIATwainMSWProvider::GetName()
{
    return _T("TWAIN");
}

int wxIATwainMSWProvider::GetSourceCount()
{
    wxCHECK_MSG(Ok(), FALSE, wxT("TWAIN not initialized"));
    return GetTwain()->GetSourceCount();
}

wxIASourceInfo wxIATwainMSWProvider::GetSourceInfo(int i)
{
    wxCHECK_MSG(Ok(), wxNullIASourceInfo, wxT("TWAIN not initialized"));

    const TW_IDENTITY *id = GetTwain()->GetSourceIdentity(i);
    return id ? wxIASourceInfo(wxTwainBase::GetStringValue(id->ProductName),
        wxTwainBase::GetStringValue(id->ProductFamily),
        wxTwainBase::GetStringValue(id->Manufacturer), wxIA_SOURCE_UNKNOWN)
        : wxNullIASourceInfo;
}

wxIAReturnCode wxIATwainMSWProvider::SelectSource(const wxString &name,
    wxIAUIMode uiMode, wxWindow *parent)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, wxT("TWAIN not initialized"));

    if (uiMode == wxIA_UIMODE_NORMAL)
    {
        if(GetTwain()->UserSelectSource(name))
            return wxIA_RC_SUCCESS;
        else if(GetTwain()->GetStatus().ConditionCode)
            return MapStatus(GetTwain()->GetStatus());
        else
            return wxIA_RC_CANCELLED;
    }
    if (GetTwain()->SelectSource(name))
        return wxIA_RC_SUCCESS;
    else
        return MapStatus(GetTwain()->GetStatus());
}

wxIAReturnCode wxIATwainMSWProvider::SelectDefaultSource()
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, wxT("TWAIN not initialized"));

    if (GetTwain()->SelectDefaultSource())
        return wxIA_RC_SUCCESS;
    else
        return MapStatus(GetTwain()->GetStatus());
}

bool wxIATwainMSWProvider::IsSourceSelected()
{
    wxCHECK_MSG(Ok(), FALSE, wxT("TWAIN not initialized"));

    return GetTwain()->IsSourceSelected();
}

wxIASourceInfo wxIATwainMSWProvider::GetSelSourceInfo()
{
    wxIASourceInfo sourceInfo;

    wxCHECK_MSG(Ok(), sourceInfo, wxT("TWAIN not initialized"));

    if (IsSourceSelected())
    {
        sourceInfo.SetName(GetTwain()->GetSourceProductName());
        sourceInfo.SetModel(GetTwain()->GetSourceProductFamily());
        sourceInfo.SetVendor(GetTwain()->GetSourceManufacturer());
        sourceInfo.SetType(wxIA_SOURCE_UNKNOWN);
    }

    return sourceInfo;
}

wxIAReturnCode wxIATwainMSWProvider::SetSelSourceConfig(const wxString &configString)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, wxT("TWAIN not initialized"));

    return wxIA_RC_NOTSUPPORTED;
}

wxString wxIATwainMSWProvider::GetSelSourceConfig()
{
    wxCHECK_MSG(Ok(), wxEmptyString, wxT("TWAIN not initialized"));

    return wxEmptyString;
}

wxIAReturnCode wxIATwainMSWProvider::AcquireImages(int numImages,
    wxIAUIMode uiMode, wxWindow *parent, wxIAMonitor *mon)
{
    wxCHECK_MSG(Ok(), wxIA_RC_NOTINITIALIZED, wxT("TWAIN not initialized"));

    m_monitor = mon;
    bool retVal = GetTwain()->AcquireImages(numImages, uiMode == wxIA_UIMODE_NORMAL);
    m_monitor = NULL;
    if (retVal)
        return wxIA_RC_SUCCESS;
    else if (GetTwain()->GetStatus().ConditionCode)
        return MapStatus(GetTwain()->GetStatus());
    else
        return wxIA_RC_CANCELLED;
}

wxImage wxIATwainMSWProvider::GetImage()
{
    wxImage image;

    wxCHECK_MSG(Ok(), image, wxT("TWAIN not initialized"));

    if (m_bitmap.Ok())
    {
        image = m_bitmap.ConvertToImage();
        m_bitmap = wxNullBitmap;
    }
    return image;
}

wxBitmap wxIATwainMSWProvider::GetBitmap()
{
    wxBitmap bitmap;

    wxCHECK_MSG(Ok(), bitmap, wxT("TWAIN not initialized"));

    bitmap = m_bitmap;
    m_bitmap = wxNullBitmap;
    return bitmap;
}

HBITMAP wxIATwainMSWProvider::CreateDIB(HANDLE hDib)
{
    LPBITMAPINFOHEADER lpDib = NULL, lpBi = NULL;
    LPSTR lpBits = NULL;
    DWORD dwColorTableSize = 0;
    HBITMAP hBitMap = NULL;
    HDC hDC = NULL;
    HPALETTE hDibPal = NULL;

    if (hDib == NULL)
    {
        return(NULL);
    }

    if ((lpDib = (LPBITMAPINFOHEADER) GlobalLock(hDib))==NULL)
    {
        wxLogLastError("GlobalLock");
        return NULL;
    }

    lpBi = lpDib;

    // Calculate the color table size, then point past the BITMAPINFOHEADER
    // and color table, to the byte array of bitmap bits.
    dwColorTableSize = (DWORD)(DibNumColors (lpDib) * sizeof(RGBQUAD));
    lpBits = (LPSTR)lpDib + lpBi->biSize + dwColorTableSize;

    // Create a logical palette based on the colors in the DIB header,
    // and realize it
    hDC = GetDC(NULL);

    if ((hDibPal = CreateBIPalette (lpBi)) != NULL)
    {
        SelectPalette(hDC, hDibPal, FALSE);
        RealizePalette(hDC);
    }

    //  Microsoft confirmed a defect in CreateDIBitmap whereas the function
    //  failed with handles to 1Meg or larger 1 color bitmaps.  The workaround
    //  is to create a device dependent bitmap whenever a 1 color bitmap
    //  is detected.
    if (lpDib->biBitCount == 1)
    {
        // create a device dependent monochrome bitmap
        hBitMap = CreateBitmap((int)lpDib->biWidth,(int)lpDib->biHeight,1,1,lpBits);
        if (hBitMap)
        {
            SetDIBits(hDC,hBitMap,0,(UINT)lpDib->biHeight,lpBits,(LPBITMAPINFO)lpDib,DIB_RGB_COLORS);
        }
    }
    else
    {
        // Create a device-independent bitmap from the BMP
        hBitMap = CreateDIBitmap(hDC, lpDib, (LONG)CBM_INIT, (LPSTR)lpBits,
            (LPBITMAPINFO)lpDib, DIB_RGB_COLORS);
    }

    GlobalUnlock(hDib);

    ReleaseDC(NULL, hDC);
    DeleteObject(hDibPal);

    // Return handle to device-dependent bitmap
    return(hBitMap);
}

HPALETTE wxIATwainMSWProvider::CreateBIPalette (LPBITMAPINFOHEADER lpbi)
{
#define PALVERSION 0x300
#define MAXPALETTE 256

    HGLOBAL hPal = NULL;
    LOGPALETTE *pPal = NULL;
    HPALETTE hPalette = NULL;
    WORD nNumColors = 0;
    BYTE Red = 0;
    BYTE Green = 0;
    BYTE Blue = 0;
    WORD i = 0;
    RGBQUAD FAR *pRgb = NULL;

    if (!lpbi)
    {
        return NULL;
    }

    if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
    {
        return NULL;
    }

    // Get a pointer to the color table and the number of colors in it
    pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
    nNumColors = DibNumColors(lpbi);

    if (nNumColors)
    {
        // Allocate for the logical palette structure
        //  SDH - 02/01/95 - For compatability with the Large model...
        //  pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors *
        //                  sizeof(PALETTEENTRY));
        hPal = GlobalAlloc(GPTR, sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        pPal = (LOGPALETTE*)(GlobalLock (hPal));
        if (!pPal)
        {
            return NULL;
        }

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        // Fill in the palette entries from the DIB color table and
        // create a logical color palette.
        for (i = 0; i < nNumColors; i++)
        {
            pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
            pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
            pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;
        }

        hPalette = CreatePalette(pPal);

        GlobalUnlock(hPal);
        GlobalFree(hPal);
    }
    else if (lpbi->biBitCount == 24)
    {
        // A 24 bitcount DIB has no color table entries so, set
        // the number of to the maximum value (256).
        nNumColors = MAXPALETTE;

        hPal = GlobalAlloc(GPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
        pPal = (LOGPALETTE*)GlobalLock (hPal);

        if (!pPal)
        {
            return NULL;
        }

        pPal->palNumEntries = nNumColors;
        pPal->palVersion    = PALVERSION;

        Red = Green = Blue = 0;

        // Generate 256 (= 8*8*4) RGB combinations to fill
        // the palette entries.
        for (i = 0; i < pPal->palNumEntries; i++)
        {
            pPal->palPalEntry[i].peRed   = Red;
            pPal->palPalEntry[i].peGreen = Green;
            pPal->palPalEntry[i].peBlue  = Blue;
            pPal->palPalEntry[i].peFlags = (BYTE)0;

            if (!(Red += 32) && !(Green += 32))
            {
                Blue += 64;
            }
        }

        hPalette = CreatePalette(pPal);

        GlobalUnlock(hPal);
        GlobalFree(hPal);
    }
    return hPalette;
}

int wxIATwainMSWProvider::DibNumColors (void *pv)
{
    int Bits = 0;
    LPBITMAPINFOHEADER lpbi = NULL;
    LPBITMAPCOREHEADER lpbc = NULL;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

    //    With the BITMAPINFO format headers, the size of the palette
    //    is in biClrUsed, whereas in the BITMAPCORE - style headers, it
    //    is dependent on the bits per pixel ( = 2 raised to the power of
    //    bits/pixel).
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
    {
        if (lpbi->biClrUsed != 0)
        {
            return (WORD)lpbi->biClrUsed;
        }

        Bits = lpbi->biBitCount;
    }
    else
    {
        Bits = lpbc->bcBitCount;
    }

    switch (Bits)
    {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            // A 24 bitcount DIB has no color table
            return 0;
    }
}

bool wxIATwainMSWProvider::HandleImage(TW_IMAGEINFO& info)
{
    wxLogDebug("width = %d, height = %d, depth = %d", info.ImageWidth, info.ImageLength, info.BitsPerPixel);
#if 0
    m_bitmap = wxBitmap(info.ImageWidth, info.ImageLength);
    if(m_bitmap.GetHBITMAP())
        ::DeleteObject((HBITMAP)m_bitmap.GetHBITMAP());
    m_bitmap.SetHBITMAP((WXHANDLE)CreateDIB((HBITMAP)GetTwain()->GetHBitmap()));
#else
    wxLogDebug("info.BitsPerPixel = %d", info.BitsPerPixel);
    m_bitmap = wxBitmap(info.ImageWidth, info.ImageLength, info.BitsPerPixel > 1 ? 24 : 1);
    wxMemoryDC memdc;
    memdc.SelectObject(m_bitmap);
    LPBITMAPINFOHEADER dib = (LPBITMAPINFOHEADER)::GlobalLock((HBITMAP)GetTwain()->GetHBitmap());
    LPSTR dibits = (LPSTR)dib + dib->biSize + DibNumColors(dib) * sizeof(RGBQUAD);
    ::StretchDIBits((HDC)memdc.GetHDC(), 0, 0, info.ImageWidth, info.ImageLength,
        0, 0, info.ImageWidth, info.ImageLength, dibits,
        (LPBITMAPINFO)dib, DIB_RGB_COLORS, SRCCOPY);
    memdc.SelectObject(wxNullBitmap);
    ::GlobalUnlock((HBITMAP)GetTwain()->GetHBitmap());
#endif

    if (m_evtHandler)
    {
        wxIAEvent event(wxEVT_IA_GETIMAGE, this);
        m_evtHandler->ProcessEvent(event);
        return !event.ShouldAbort();
    }
    return TRUE;
}

bool wxIATwainMSWProvider::UpdateStatus(const wxString &text, size_t quantum,
    size_t span)
{
    if (m_monitor)
        return m_monitor->Update(text, quantum, span);
    if (m_evtHandler)
    {
        wxIAEvent event(wxEVT_IA_UPDATE, this, text, quantum, span);
        m_evtHandler->ProcessEvent(event);
        return !event.ShouldAbort();
    }

    return TRUE;
}

wxIAReturnCode wxIATwainMSWProvider::MapStatus(TW_STATUS status)
{
    switch(status.ConditionCode)
    {
        case TWCC_SUCCESS :
            return wxIA_RC_SUCCESS;

        case TWCC_LOWMEMORY :
            return wxIA_RC_NOMEM;

        case TWCC_NODS :
            return wxIA_RC_NOSOURCE;

        case TWCC_BADVALUE :
            return wxIA_RC_INVAL;

        case TWCC_CAPUNSUPPORTED :
        case TWCC_BADCAP :
        case TWCC_CAPBADOPERATION :
            return wxIA_RC_NOTSUPPORTED;

        case TWCC_DENIED :
            return wxIA_RC_ACCESSDENIED;

        case TWCC_PAPERJAM :
        case TWCC_PAPERDOUBLEFEED :
            return wxIA_RC_JAMMED;

        case TWCC_CHECKDEVICEONLINE :
            return wxIA_RC_NOTREADY;

        default:
        case TWCC_BUMMER :
            return wxIA_RC_UNKNOWNERROR;
    }
}

#endif