// ReSharper disable All
#include "header.h"
/////////////////////////////////////////////////////////////////////////////
// Name:        md5.cpp
// Purpose:     MD5 file checksum
// Author:      Francesco Montorsi
// Created:     2005/07/13
// RCS-ID:      $Id: md5.cpp,v 1.5 2005/10/20 16:06:01 frm Exp $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence + RDS Data Security license
/////////////////////////////////////////////////////////////////////////////

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include "wxmd5.h"

#ifndef WIN32

// C headers for MD5
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MD5_HASHBYTES 16


void MD5Init(struct MD5_CTX *context);

void MD5Update(struct MD5_CTX *context, md5byte const *buf, unsigned len);

void MD5Final(unsigned char digest[16], struct MD5_CTX *context);

void MD5Transform(UWORD32 buf[4], UWORD32 const in[16]);

static void
byteSwap(UWORD32 *buf, unsigned words)
{
    const uint32_t byteOrderTest = 0x1;
    if (((char *)&byteOrderTest)[0] == 0) {
        md5byte *p = (md5byte *)buf;
        
        do {
            *buf++ = (UWORD32)((unsigned)p[3] << 8 | p[2]) << 16 |
            ((unsigned)p[1] << 8 | p[0]);
            p += 4;
        } while (--words);
    }
}

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void
MD5Init(struct MD5_CTX *ctx)
{
    ctx->buf[0] = 0x67452301;
    ctx->buf[1] = 0xefcdab89;
    ctx->buf[2] = 0x98badcfe;
    ctx->buf[3] = 0x10325476;
    
    ctx->bytes[0] = 0;
    ctx->bytes[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void
MD5Update(struct MD5_CTX *ctx, md5byte const *buf, unsigned len)
{
    UWORD32 t;
    
    /* Update byte count */
    
    t = ctx->bytes[0];
    if ((ctx->bytes[0] = t + len) < t)
        ctx->bytes[1]++;	/* Carry from low to high */
    
    t = 64 - (t & 0x3f);	/* Space available in ctx->in (at least 1) */
    if (t > len) {
        memcpy((md5byte *)ctx->in + 64 - t, buf, len);
        return;
    }
    /* First chunk is an odd size */
    memcpy((md5byte *)ctx->in + 64 - t, buf, t);
    byteSwap(ctx->in, 16);
    MD5Transform(ctx->buf, ctx->in);
    buf += t;
    len -= t;
    
    /* Process data in 64-byte chunks */
    while (len >= 64) {
        memcpy(ctx->in, buf, 64);
        byteSwap(ctx->in, 16);
        MD5Transform(ctx->buf, ctx->in);
        buf += 64;
        len -= 64;
    }
    
    /* Handle any remaining bytes of data. */
    memcpy(ctx->in, buf, len);
}


/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f,w,x,y,z,in,s) \
(w += f(x,y,z) + in, w = (w<<s | w>>(32-s)) + x)

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void
MD5Transform(UWORD32 buf[4], UWORD32 const in[16])
{
    register UWORD32 a, b, c, d;
    
    a = buf[0];
    b = buf[1];
    c = buf[2];
    d = buf[3];
    
    MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);
    
    MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);
    
    MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);
    
    MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);
    
    buf[0] += a;
    buf[1] += b;
    buf[2] += c;
    buf[3] += d;
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void
MD5Final(md5byte digest[16], struct MD5_CTX *ctx)
{
    int count = ctx->bytes[0] & 0x3f;	/* Number of bytes in ctx->in */
    md5byte *p = (md5byte *)ctx->in + count;
    
    /* Set the first char of padding to 0x80.  There is always room. */
    *p++ = 0x80;
    
    /* Bytes of padding needed to make 56 bytes (-8..55) */
    count = 56 - 1 - count;
    
    if (count < 0) {	/* Padding forces an extra block */
        memset(p, 0, count + 8);
        byteSwap(ctx->in, 16);
        MD5Transform(ctx->buf, ctx->in);
        p = (md5byte *)ctx->in;
        count = 56;
    }
    memset(p, 0, count);
    byteSwap(ctx->in, 14);
    
    /* Append length in bits and transform */
    ctx->in[14] = ctx->bytes[0] << 3;
    ctx->in[15] = ctx->bytes[1] << 3 | ctx->bytes[0] >> 29;
    MD5Transform(ctx->buf, ctx->in);
    
    byteSwap(ctx->buf, 4);
    memcpy(digest, ctx->buf, 16);
    memset(ctx, 0, sizeof(ctx));	/* In case it's sensitive */
}

char * MD5End(MD5_CTX *ctx, char *buf)
{
	int i;
	unsigned char digest[MD5_HASHBYTES];
	char hex[] = "0123456789abcdef";

	if (!buf)
	{
		buf = (char *)malloc(33);
	}

	if (!buf)
		return 0;

	MD5Final(digest, ctx);
	for (i = 0; i<MD5_HASHBYTES; i++) {
		buf[i + i] = hex[digest[i] >> 4];
		buf[i + i + 1] = hex[digest[i] & 0x0f];
	}
	buf[i + i] = '\0';
	return buf;
}


// --------------------------
// wxMD5 - static functions
// --------------------------

wxString wxMD5::GetMD5(const wxString &string)
{
	int lenght = (int)string.Len();
	//wxStringBuffer buf(string, lenght);

	MD5_CTX ctx;
	char tmp[40];		// MD5 are fixed sized to 32 chars
	
	MD5Init(&ctx);
	MD5Update(&ctx, (const unsigned char*)string.GetData(), lenght*sizeof(wxChar));
	MD5End(&ctx, tmp);

	return wxString(tmp, wxConvUTF8);
}

wxString wxMD5::GetFileMD5(wxInputStream &stream)
{
    unsigned char buffer[102400];
	char tmp[40];		// MD5 are fixed sized to 32 chars
    MD5_CTX ctx;

	if (!stream.IsOk())
		return wxEmptyString;

   	MD5Init(&ctx);

    do
    {
		if (stream.Read(buffer, sizeof(buffer)).LastRead() <= 0)
			return wxEmptyString;

    	MD5Update(&ctx, buffer, (int)stream.LastRead());
    } while (!stream.Eof());

	MD5End(&ctx, tmp);
    return wxString(tmp, wxConvUTF8);
}

wxString wxMD5::GetFileMD5(const wxString &filename)
{ 
	wxFileInputStream stream(filename); 
	return GetFileMD5(stream); 
}

#else

#include <stdio.h>
#include <wincrypt.h>
#include <Windows.h>

constexpr auto BUFSIZE = 1024;
constexpr auto MD5LEN = 16;

wxString wxMD5::GetFileMD5(const wxString& filename)
{
	BOOL b_result;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cb_hash;
	CHAR rgbDigits[] = "0123456789abcdef";
	//LPCWSTR filename = L"filename.txt";
	// Logic to check usage goes here.

	// ReSharper disable once CppInconsistentNaming
	const HANDLE hFile = CreateFile(filename.c_str(),
	                                GENERIC_READ,
	                                FILE_SHARE_READ,
	                                nullptr,
	                                OPEN_EXISTING,
	                                FILE_FLAG_SEQUENTIAL_SCAN,
	                                nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		GetLastError();
		//printf("Error opening file %s\nError: %d\n", filename,
		//	dwStatus);
		return "";
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
	                         nullptr,
	                         nullptr,
	                         PROV_RSA_FULL,
	                         CRYPT_VERIFYCONTEXT))
	{
		GetLastError();
		//printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		return "";
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		GetLastError();
		//printf("CryptAcquireContext failed: %d\n", dwStatus);
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	while ((b_result = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, nullptr)))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			GetLastError();
			//printf("CryptHashData failed: %d\n", dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return "";
		}
	}

	if (!b_result)
	{
		GetLastError();
		//printf("ReadFile failed: %d\n", dwStatus);
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return "";
	}
	wxString output = "";
	cb_hash = MD5LEN;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cb_hash, 0))
	{
		//printf("MD5 hash of file %s is: ", filename);
		for (DWORD i = 0; i < cb_hash; i++)
		{
			char message[10];
			sprintf(message, "%c%c", rgbDigits[rgbHash[i] >> 4],
			        rgbDigits[rgbHash[i] & 0xf]);
			output.append(message);
		}
	}
	else
	{
		GetLastError();
		//printf("CryptGetHashParam failed: %d\n", dwStatus);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);

	return output;
}
#endif
