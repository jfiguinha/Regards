/*
*********************************************************************
 * File:	ximawmf.cpp
 * Purpose:	Windows Metafile Class Loader and Writer
 * Author:	Volker Horch - vhorch@gmx.de
 * created:	13-Jun-2002
 *
 * Note:	If the code below works, i wrote it.
 *			If it doesn't work, i don't know who wrote it.
*********************************************************************
 */

/*
*********************************************************************
	Note by Author:
*********************************************************************

	Metafile Formats:
	=================

	There are 2 kinds of Windows Metafiles:
	- Standard Windows Metafile
	- Placeable Windows Metafile

	A StandardWindows Metafile looks like:
	- Metafile Header (MEATAHEADER)
	- Metafile Records 

	A Placeable Metafile looks like:
	- Aldus Header (METAFILEHEADER)
	- Metafile Header (METAHEADER)
	- Metafile Records

	The "Metafile Header" and the "Metafile Records" are the same
	for both formats. However, the Standard Metafile does not contain any
	information about the original dimensions or x/y ratio of the Metafile.

	I decided, to allow only placeable Metafiles here. If you also want to
	enable Standard Metafiles, you will have to guess the dimensions of
	the image.

*********************************************************************
	Limitations:	see ximawmf.h
					you may configure some stuff there
*********************************************************************
*/

#include "ximawmf.h"

#if CXIMAGE_SUPPORT_WMF && CXIMAGE_SUPPORT_WINDOWS

////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
bool CxImageWMF::Decode(CxFile *hFile, int32_t nForceWidth, int32_t nForceHeight)
{
	if (hFile == NULL) return false;

	HENHMETAFILE	hMeta;
	HDC				hDC;
	int32_t				cx,cy;

	//save the current position of the file
	int32_t pos = hFile->Tell();

	// Read the Metafile and convert to an Enhanced Metafile
	METAFILEHEADER	mfh;
	hMeta = ConvertWmfFiletoEmf(hFile, &mfh);
	if (hMeta) {	// ok, it's a WMF

/////////////////////////////////////////////////////////////////////
//	We use the original WMF size information, because conversion to
//	EMF adjusts the Metafile to Full Screen or does not set rclBounds at all
//	ENHMETAHEADER	emh;
//	uint32_t			uRet;
//	uRet = GetEnhMetaFileHeader(hMeta,					// handle of enhanced metafile 
//								sizeof(ENHMETAHEADER),	// size of buffer, in bytes 
//								&emh); 					// address of buffer to receive data  
//	if (!uRet){
//		DeleteEnhMetaFile(hMeta);
//		return false;
//	}
//	// calculate size
//	cx = emh.rclBounds.right - emh.rclBounds.left;
//	cy = emh.rclBounds.bottom - emh.rclBounds.top;
/////////////////////////////////////////////////////////////////////

		// calculate size
		// scale the metafile (pixels/inch of metafile => pixels/inch of display)
		// mfh.inch already checked to be <> 0

		hDC = ::GetDC(0);
		int32_t cx1 = ::GetDeviceCaps(hDC, LOGPIXELSX);
		int32_t cy1 = ::GetDeviceCaps(hDC, LOGPIXELSY);
		::ReleaseDC(0, hDC);

		cx = (mfh.inch/2 + (mfh.bbox.right - mfh.bbox.left) * cx1) / mfh.inch;
		cy = (mfh.inch/2 + (mfh.bbox.bottom - mfh.bbox.top) * cy1) / mfh.inch;

	} else {		// maybe it's an EMF...

		hFile->Seek(pos,SEEK_SET);

		ENHMETAHEADER	emh;
		hMeta = ConvertEmfFiletoEmf(hFile, &emh);

		if (!hMeta){
			strcpy(info.szLastError,"corrupted WMF");
			return false; // definitively give up
		}

		// ok, it's an EMF; calculate canvas size
		cx = emh.rclBounds.right - emh.rclBounds.left;
		cy = emh.rclBounds.bottom - emh.rclBounds.top;

		// alternative methods, sometime not so reliable... [DP]
		//cx = emh.szlDevice.cx;
		//cy = emh.szlDevice.cy;
		//
		//hDC = ::GetDC(0);
		//float hscale = (float)GetDeviceCaps(hDC, HORZRES)/(100.0f * GetDeviceCaps(hDC, HORZSIZE));
		//float vscale  =  (float)GetDeviceCaps(hDC, VERTRES)/(100.0f * GetDeviceCaps(hDC, VERTSIZE));
		//::ReleaseDC(0, hDC);
		//cx = (int32_t)((emh.rclFrame.right - emh.rclFrame.left) * hscale);
		//cy = (int32_t)((emh.rclFrame.bottom - emh.rclFrame.top) * vscale);
	}

	if (info.nEscape == -1) {	// Check if cancelled
		head.biWidth = cx;
		head.biHeight= cy;
		info.dwType = CXIMAGE_FORMAT_WMF;
		DeleteEnhMetaFile(hMeta);
		strcpy(info.szLastError,"output dimensions returned");
		return true;
	}

	if (!cx || !cy)	{
		DeleteEnhMetaFile(hMeta);
		strcpy(info.szLastError,"empty WMF");
		return false;
	}

	if (nForceWidth) cx=nForceWidth;
	if (nForceHeight) cy=nForceHeight;
	ShrinkMetafile(cx, cy);		// !! Otherwise Bitmap may have bombastic size

	HDC hDC0 = ::GetDC(0);	// DC of screen
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC0, cx, cy);	// has # colors of display
	hDC = CreateCompatibleDC(hDC0);	// memory dc compatible with screen
	::ReleaseDC(0, hDC0);	// don't need anymore. get rid of it.

	if (hDC){
		if (hBitmap){
			RECT rc = {0,0,cx,cy};
			int32_t bpp = ::GetDeviceCaps(hDC, BITSPIXEL);

			HBITMAP hBitmapOld = (HBITMAP)SelectObject(hDC, hBitmap);

			// clear out the entire bitmap with windows background
			// because the MetaFile may not contain background information
			uint32_t	dwBack = XMF_COLOR_BACK;
#if XMF_SUPPORT_TRANSPARENCY
			if (bpp == 24) dwBack = XMF_COLOR_TRANSPARENT;
#endif
		    uint32_t OldColor = SetBkColor(hDC, dwBack);
		    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rc, NULL, 0, NULL);
			SetBkColor(hDC, OldColor);

			//retrieves optional palette entries from the specified enhanced metafile
			PLOGPALETTE plogPal;
			PBYTE pjTmp; 
			HPALETTE hPal; 
			int32_t iEntries = GetEnhMetaFilePaletteEntries(hMeta, 0, NULL);
			if (iEntries) { 
				if ((plogPal = (PLOGPALETTE)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, 
					sizeof(uint32_t) + sizeof(PALETTEENTRY)*iEntries )) == NULL) { 
					DeleteObject(hBitmap);
					DeleteDC(hDC);
					DeleteEnhMetaFile(hMeta);
					strcpy(info.szLastError,"Cancelled");
					return false;
				} 

				plogPal->palVersion = 0x300; 
				plogPal->palNumEntries = (uint16_t) iEntries; 
				pjTmp = (PBYTE) plogPal; 
				pjTmp += 4; 

				GetEnhMetaFilePaletteEntries(hMeta, iEntries, (PPALETTEENTRY)pjTmp); 
				hPal = CreatePalette(plogPal); 
				GlobalFree(plogPal); 

				SelectPalette(hDC, hPal, FALSE); 
				RealizePalette(hDC); 
			} 
			
			// Play the Metafile into Memory DC
			BOOL bRet = PlayEnhMetaFile(hDC,	// handle to a device context 
									hMeta,	// handle to an enhanced metafile  
									&rc); 	// pointer to bounding rectangle

			SelectObject(hDC, hBitmapOld);
			DeleteEnhMetaFile(hMeta);	// we are done with this one

			if (info.nEscape) {	// Check if cancelled
				DeleteObject(hBitmap);
				DeleteDC(hDC);
				strcpy(info.szLastError,"Cancelled");
				return false;
			}

			// the Bitmap now has the image.
			// Create our DIB and convert the DDB into DIB
			if (!Create(cx, cy, bpp, CXIMAGE_FORMAT_WMF)) {
				DeleteObject(hBitmap);
				DeleteDC(hDC);
				return false;
			}

#if XMF_SUPPORT_TRANSPARENCY
			if (bpp == 24) {
				RGBQUAD	rgbTrans = { XMF_RGBQUAD_TRANSPARENT };
				SetTransColor(rgbTrans);
			}
#endif
		    // We're finally ready to get the DIB. Call the driver and let
		    // it party on our bitmap. It will fill in the color table,
		    // and bitmap bits of our global memory block.
			bRet = GetDIBits(hDC, hBitmap, 0,
			        (uint32_t)cy, GetBits(), (LPBITMAPINFO)pDib, DIB_RGB_COLORS);

			DeleteObject(hBitmap);
			DeleteDC(hDC);

			return (bRet!=0);
		} else {
			DeleteDC(hDC);
		}
	} else {
		if (hBitmap) DeleteObject(hBitmap);
	}

	DeleteEnhMetaFile(hMeta);

	return false;
}

/**********************************************************************
 Function:	CheckMetafileHeader
 Purpose:	Check if the Metafileheader of a file is valid
**********************************************************************/
BOOL CxImageWMF::CheckMetafileHeader(METAFILEHEADER *metafileheader)
{
	uint16_t	*pw;
	uint16_t	cs;
	int32_t		i;

	// check magic #
	if (metafileheader->key != 0x9ac6cdd7L)	return false;

	// test checksum of header
	pw = (uint16_t *)metafileheader;
	cs = *pw;
	pw++;
	for (i = 0; i < 9; i++)	{
		cs ^= *pw;
		pw++;
	}

	if (cs != metafileheader->checksum)	return false;

	// check resolution
	if ((metafileheader->inch <= 0) || (metafileheader->inch > 2540)) return false;

	return true;
}

/**********************************************************************
 Function:	ConvertWmfFiletoEmf
 Purpose:	Converts a Windows Metafile into an Enhanced Metafile
**********************************************************************/
HENHMETAFILE CxImageWMF::ConvertWmfFiletoEmf(CxFile *fp, METAFILEHEADER *metafileheader)
{
	HENHMETAFILE	hMeta;
	uint32_t	lenFile;
	uint32_t	len;
	uint8_t			*p;
	METAHEADER		mfHeader;
	uint32_t			seekpos;

	hMeta = 0;

	// get length of the file
	lenFile = fp->Size();

	// a placeable metafile starts with a METAFILEHEADER
	// read it and check metafileheader
	len = fp->Read(metafileheader, 1, sizeof(METAFILEHEADER));
	if (len < sizeof(METAFILEHEADER)) return (hMeta);

	if (CheckMetafileHeader(metafileheader)) {
		// This is a placeable metafile 
		// Convert the placeable format into something that can
		// be used with GDI metafile functions 
		seekpos = sizeof(METAFILEHEADER);
	} else {
		// Not a placeable wmf. A windows metafile?
		// at least not scaleable.
		// we could try to convert, but would loose ratio. don't allow this
		return (hMeta);

		//metafileheader->bbox.right = ?;
		//metafileheader->bbox.left = ?;
		//metafileheader->bbox.bottom = ?;
		//metafileheader->bbox.top = ?;
		//metafileheader->inch = ?;
		//
		//seekpos = 0;
		// fp->Seek(0, SEEK_SET);	// rewind
	}

	// At this point we have a metaheader regardless of whether
	// the metafile was a windows metafile or a placeable metafile
	// so check to see if it is valid. There is really no good
	// way to do this so just make sure that the mtType is either
	// 1 or 2 (memory or disk file) 
	// in addition we compare the length of the METAHEADER against
	// the length of the file. if filelength < len => no Metafile

	len = fp->Read(&mfHeader, 1, sizeof(METAHEADER));
	if (len < sizeof(METAHEADER)) return (hMeta);

	if ((mfHeader.mtType != 1) && (mfHeader.mtType != 2)) return (hMeta);

	// Length in Bytes from METAHEADER
	len = mfHeader.mtSize * 2;
	if (len > lenFile) return (hMeta);

	// Allocate memory for the metafile bits 
	p = (uint8_t *)malloc(len);
	if (!p)	return (hMeta);

	// seek back to METAHEADER and read all the stuff at once
	fp->Seek(seekpos, SEEK_SET);
	lenFile = fp->Read(p, 1, len);
	if (lenFile != len)	{
		free(p);
		return (hMeta);
	}

	// the following (commented code)  works, but adjusts rclBound of the
	// Enhanced Metafile to full screen.
	// the METAFILEHEADER from above is needed to scale the image

//	hMeta = SetWinMetaFileBits(len, p, NULL, NULL);

	// scale the metafile (pixels/inch of metafile => pixels/inch of display)

	METAFILEPICT	mfp;
	int32_t cx1, cy1;
	HDC hDC;

	hDC = ::GetDC(0);
	cx1 = ::GetDeviceCaps(hDC, LOGPIXELSX);
	cy1 = ::GetDeviceCaps(hDC, LOGPIXELSY);

	memset(&mfp, 0, sizeof(mfp));

	mfp.mm = MM_ANISOTROPIC;
	mfp.xExt = 10000; //(metafileheader->bbox.right - metafileheader->bbox.left) * cx1 / metafileheader->inch;
	mfp.yExt = 10000; //(metafileheader->bbox.bottom - metafileheader->bbox.top) * cy1 / metafileheader->inch;
	mfp.hMF = 0;

	// in MM_ANISOTROPIC mode xExt and yExt are in MM_HIENGLISH
	// MM_HIENGLISH means: Each logical unit is converted to 0.001 inch
	//mfp.xExt *= 1000;
	//mfp.yExt *= 1000;
	// ????
	//int32_t k = 332800 / ::GetSystemMetrics(SM_CXSCREEN);
	//mfp.xExt *= k;	mfp.yExt *= k;

	// fix for Win9x
	while ((mfp.xExt < 6554) && (mfp.yExt < 6554))
	{
		mfp.xExt *= 10;
		mfp.yExt *= 10;
	}

	hMeta = SetWinMetaFileBits(len, p, hDC, &mfp);

	if (!hMeta){ //try 2nd conversion using a different mapping
		mfp.mm = MM_TEXT;
		hMeta = SetWinMetaFileBits(len, p, hDC, &mfp);
	}

	::ReleaseDC(0, hDC);

	// Free Memory
	free(p);

	return (hMeta);
}
/////////////////////////////////////////////////////////////////////
HENHMETAFILE CxImageWMF::ConvertEmfFiletoEmf(CxFile *pFile, ENHMETAHEADER *pemfh)
{
	HENHMETAFILE	hMeta;
	int32_t iLen = pFile->Size();

	// Check the header first: <km>
	int32_t pos = pFile->Tell();
	int32_t iLenRead = pFile->Read(pemfh, 1, sizeof(ENHMETAHEADER));
	if (iLenRead < sizeof(ENHMETAHEADER))         return NULL;
	if (pemfh->iType != EMR_HEADER)               return NULL;
	if (pemfh->dSignature != ENHMETA_SIGNATURE)   return NULL;
	//if (pemfh->nBytes != (uint32_t)iLen)             return NULL;
	pFile->Seek(pos,SEEK_SET);

	uint8_t* pBuff = (uint8_t *)malloc(iLen);
	if (!pBuff)	return (FALSE);

	// Read the Enhanced Metafile
	iLenRead = pFile->Read(pBuff, 1, iLen);
	if (iLenRead != iLen) {
		free(pBuff);
		return NULL;
	}

	// Make it a Memory Metafile
	hMeta = SetEnhMetaFileBits(iLen, pBuff);

	free(pBuff);	// finished with this one

	if (!hMeta)	return NULL;	// oops.

	// Get the Enhanced Metafile Header
	uint32_t uRet = GetEnhMetaFileHeader(hMeta,				// handle of enhanced metafile 
								sizeof(ENHMETAHEADER),	// size of buffer, in bytes 
								pemfh); 				// address of buffer to receive data  
  
	if (!uRet) {
		DeleteEnhMetaFile(hMeta);
		return NULL;
	}

	return (hMeta);
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_ENCODE
/////////////////////////////////////////////////////////////////////
bool CxImageWMF::Encode(CxFile * hFile)
{
	if (hFile == NULL) return false;
	strcpy(info.szLastError, "Save WMF not supported");
	return false;
}
#endif	// CXIMAGE_SUPPORT_ENCODE
/////////////////////////////////////////////////////////////////////

/**********************************************************************
Function:	ShrinkMetafile
Purpose:	Shrink the size of a metafile to be not larger than
			the definition
**********************************************************************/
void CxImageWMF::ShrinkMetafile(int32_t &cx, int32_t &cy)
{
	int32_t	xScreen = XMF_MAXSIZE_CX;
	int32_t	yScreen = XMF_MAXSIZE_CY;

	if (cx > xScreen){
		cy = cy * xScreen / cx;
		cx = xScreen;
	}

	if (cy > yScreen){
		cx = cx * yScreen / cy;
		cy = yScreen;
	}
}

#endif	// CIMAGE_SUPPORT_WMF
