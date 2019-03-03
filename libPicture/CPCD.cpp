#include <header.h>
// CPCD.cpp: implementation of the CCPCD class.
//
//////////////////////////////////////////////////////////////////////

#include "CPCD.h"
#include "RegardsBitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
//#include <share.h>
#include <sys/stat.h>

//using namespace Regards::FiltreEffet;

#define HUFF1 0xc2000

#define RED_NUL     137
#define BLUE_NUL    156

#define LUN_MUL     360
#define RED_MUL     512
#define BLUE_MUL    512
#define GREEN1_MUL  (-RED_MUL/2)
#define GREEN2_MUL  (-BLUE_MUL/6)

#define RED_ADD     (-RED_NUL*RED_MUL)
#define BLUE_ADD    (-BLUE_NUL*BLUE_MUL)
#define GREEN1_ADD  (-RED_ADD/2)
#define GREEN2_ADD  (-BLUE_ADD/6)

#define PCD_TYPE_GRAY       1	/* gray           - 1 uint8_t/pixel */
#define PCD_TYPE_RGB        2	/* red,green,blue - 3 uint8_t/pixel */
#define PCD_TYPE_BGR        3	/* blue,green,red - 3 uint8_t/pixel */
#define PCD_TYPE_LUT_SHORT  4	/* lookup table   - 2 uint8_t/pixel */
#define PCD_TYPE_LUT_LONG   5	/* lookup table   - 4 uint8_t/pixel */

#define GET_RED   (LUT_range[RANGE + gray + \
			    LUT_red[red[x]]])
#define GET_GREEN (LUT_range[RANGE + gray + \
			    LUT_green1[red[x]] + \
			    LUT_green2[blue[x]]])
#define GET_BLUE  (LUT_range[RANGE + gray + \
			    LUT_blue[blue[x]]])

#define SETSHIFT       { shiftreg=(((stream[0]<<16) | (stream[1]<<8 ) | \
				    (stream[2])) >> (8-bit)) & 0xffff; }

#define LEFTSHIFT      { shiftreg=((shiftreg<<1) & 0xffff) | \
    ((stream[2]>>(7-bit++))&1); stream += bit>>3; bit &= 7; }

char pcd_rotor[] = {'-', '\\', '|', '/'};
int pcd_img_start[] = {0, 8192, 47104, 196608};
int pcd_def_width[] = {0, 192, 384, 768, 1536, 3072, 6144};
int pcd_def_height[] = {0, 128, 256, 512, 1024, 2048, 4096};
wchar_t  pcd_errmsg[512];

#define PCD_WIDTH(res,rot)   (rot&1?pcd_def_height[res]:pcd_def_width[res])
#define PCD_HEIGHT(res,rot)  (rot&1?pcd_def_width[res]:pcd_def_height[res])

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCPCD::CCPCD()
{
	LUT_flag = 0;
}

CCPCD::~CCPCD()
{

}

int CCPCD::pcd_read_htable(unsigned char *src, unsigned char **pseq, unsigned char **pbits)
{
    int             len, seq, seq2, bits, i, j;

    if (*pseq)
	free(*pseq);
    if (*pbits)
	free(*pbits);
    *pseq = (unsigned char *)malloc(0x10000 * sizeof(unsigned char));
    memset(*pseq, 0, 0x10000 * sizeof(char));
    *pbits = (unsigned char *)malloc(0x10000 * sizeof(unsigned char));
    memset(*pbits, 0, 0x10000 * sizeof(char));

    if (*pseq == nullptr || *pbits == nullptr)
	return -1;
    for (i = 1, len = *src; len >= 0; i += 4, len--) {
	seq = ((int) src[i + 1] << 8 | (int) src[i + 2]);
	bits = src[i] + 1;
	seq2 = seq + (0x10000 >> bits);
	for (j = seq; j < seq2; j++) {
	    if ((*pbits)[j]) 
		{
		//StringCchPrintf(pcd_errmsg,512, L"Invalid huffmann code table, seems the file is'nt a PhotoCD image");
		return -1;
	    }
	    (*pseq)[j] = src[i + 3];
	    (*pbits)[j] = bits;
	}
    }
    return i;
}

int CCPCD::pcd_un_huff(struct PCD_IMAGE *img, unsigned char *start, int run)
{
    int    shiftreg, bit;
    unsigned char  *stream = start;
    int             y, type, shift;
    int             height, y1, y2;

    switch (run) {
    case 1:
	height = pcd_def_height[4];
	y1 = img->top >> (img->res - 4);
	y2 = (img->top + img->height) >> (img->res - 4);
	break;
    case 2:
	height = pcd_def_height[5];
	y1 = img->top >> (img->res - 5);
	y2 = (img->top + img->height) >> (img->res - 5);
	break;
    default:
	fprintf(stderr, "internal error: pcd_decode: run %i ???\n", run);
	exit(1);
    }

    for (y = 0; y < height;) {
	//ROTOR(y);
	for (;;) {
	    bit = 0;
	    stream = (unsigned char *)memchr(stream, 0xff, 10240);
	    if (stream[1] == 0xff)
		break;
	    stream++;
	}
	SETSHIFT;
	while (shiftreg != 0xfffe)
	    LEFTSHIFT;
	stream += 2;
	SETSHIFT;
	y = (shiftreg >> 1) & 0x1fff;
	type = (shiftreg >> 14);
	stream += 2;
	SETSHIFT;

	if (y > height) {
	    //StringCchPrintf(pcd_errmsg,512, L"Oops: invalid line nr (y=%i)\n", y);
	    return -1;
	}
	if (y < y1 || y >= y2)
	    continue;
	if (img->gray && type)
	    return 0;		/* cut color decoding */

	{
	     unsigned char *data;
	     int    x;
	    unsigned char  *seq;
	    unsigned char  *bits;
	    int             x1, x2;

	    switch (type) {
	    case 0:
		shift = 0;
		seq = img->seq1;
		bits = img->len1;
		data = img->luma[(y - y1) >> shift];
		break;
	    case 2:
		shift = 1;
		seq = img->seq2;
		bits = img->len2;
		data = img->blue[(y - y1) >> shift];
		break;
	    case 3:
		shift = 1;
		seq = img->seq3;
		bits = img->len3;
		data = img->red[(y - y1) >> shift];
		break;
	    default:
		//StringCchPrintf(pcd_errmsg,512, L"Oops: invalid line type (type=%i)\n", type);
		return -1;
	    }

	    if (run == 1) {
		x1 = img->left >> (img->res - 4 + shift);
		x2 = (img->width) >> (img->res - 4 + shift);
	    } else {
		x1 = img->left >> (img->res - 5 + shift);
		x2 = (img->width) >> (img->res - 5 + shift);
	    }
	    for (x = 0; x < x1; x++) {
		bit += bits[shiftreg];
		stream += bit >> 3, bit &= 7;
		SETSHIFT;
	    }
	    for (x = 0; x < x2; x++) {
		data[x] = LUT_range[RANGE + (int) data[x] +
				    (signed char) seq[shiftreg]];
		bit += bits[shiftreg];
		stream += bit >> 3, bit &= 7;
		SETSHIFT;
	    }
	}
    }
    return ((stream - start) + 0x6000 + 2047) & ~0x7ff;
}

int CCPCD::pcd_decode(struct PCD_IMAGE *img)
{
    int pos = HUFF1, rc;

    switch (img->res) 
	{
    case 1:
    case 2:
    case 3:
	/* nothing to do */
	break;
    case 4:
	pcd_inter_m2(img);

	if (!img->gray) {
	    pcd_inter_lines(img->blue, img->width >> 1, img->height >> 1);
	    pcd_inter_lines(img->red, img->width >> 1, img->height >> 1);
	}
	pcd_inter_lines(img->luma, img->width, img->height);
	if (-1 == (rc = pcd_read_htable(img->mmap + pos, &img->seq1, &img->len1)))
	    return -1;
	pos += rc;
	pos = (pos + 2047) & ~0x3ff;
	if (-1 == pcd_un_huff(img, img->mmap + pos, 1))
	    return (-1);
	//TELL('*');
	break;
    case 5:
	pcd_inter_m2(img);

	if (!img->gray) {
	    pcd_inter_lines(img->blue, img->width >> 2, img->height >> 2);
	    pcd_inter_pixels(img->blue, img->width >> 1, img->height >> 1);
	    pcd_inter_lines(img->blue, img->width >> 1, img->height >> 1);
	    pcd_inter_lines(img->red, img->width >> 2, img->height >> 2);
	    pcd_inter_pixels(img->red, img->width >> 1, img->height >> 1);
	    pcd_inter_lines(img->red, img->width >> 1, img->height >> 1);
	}
	pcd_inter_lines(img->luma, img->width >> 1, img->height >> 1);
	if (-1 == (rc = pcd_read_htable(img->mmap + pos, &img->seq1, &img->len1)))
	    return -1;
	pos += rc;
	pos = (pos + 2047) & ~0x3ff;
	if (-1 == (rc = pcd_un_huff(img, img->mmap + pos, 1)))
	    return (-1);
	pos += rc;
	//TELL('*');
	pcd_inter_pixels(img->luma, img->width, img->height);
	pcd_inter_lines(img->luma, img->width, img->height);
	if (-1 == (rc = pcd_read_htable(img->mmap + pos, &img->seq1, &img->len1)))
	    return -1;
	pos += rc;
	if (-1 == (rc = pcd_read_htable(img->mmap + pos, &img->seq2, &img->len2)))
	    return -1;
	pos += rc;
	if (-1 == (rc = pcd_read_htable(img->mmap + pos, &img->seq3, &img->len3)))
	    return -1;
	pos += rc;
	pos = (pos + 2047) & ~0x3ff;
	if (-1 == pcd_un_huff(img, img->mmap + pos, 2))
	    return -1;
	//TELL('*');
	break;
    default:
		return -1;
		break;
    }
    return 0;
}

int CCPCD::pcd_inter_lines(unsigned char **data, int width, int height)
{
     unsigned char *src1, *src2, *dest;
     int    x;
    int             y;

    for (y = 0; y < height - 2; y += 2) 
	{
		src1 = data[y];
		dest = data[y + 1];
		src2 = data[y + 2];
			for (x = 0; x < width - 2; x += 2) 
			{
				dest[x] = (src1[x] + src2[x] + 1) >> 1;
				dest[x + 1] = (src1[x] + src2[x] + src1[x + 2] + src2[x + 2] + 2) >> 2;
			}
		dest[x] = dest[x + 1] = (src1[x] + src2[x] + 1) >> 1;
    }
    src1 = data[y];
    dest = data[y + 1];
    for (x = 0; x < width - 2; x += 2) 
	{
		dest[x] = src1[x];
		dest[x + 1] = (src1[x] + src1[x + 2] + 1) >> 1;
    }
    dest[x] = dest[x + 1] = src1[x];
    return 0;
}

int CCPCD::pcd_inter_pixels(unsigned char **data, int width, int height)
{
     unsigned char *src, *dest;
     int    x;
    int             y;

    for (y = height - 2; y >= 0; y -= 2) 
	{
		src = data[y >> 1];
		dest = data[y];
		dest[width - 2] = dest[width - 1] = src[(width >> 1) - 1];
		for (x = width - 4; x >= 0; x -= 2) 
		{
			dest[x] = src[x >> 1];
			dest[x + 1] = (src[x >> 1] + src[(x >> 1) + 1] + 1) >> 1;
		}
    }
    return 0;
}

int CCPCD::pcd_inter_m2(struct PCD_IMAGE *img)
{
     unsigned char *src, *dest;
     int    x;
    int             y;
    int             left = img->left >> (img->res - 3);
    int             top = img->top >> (img->res - 3);
    int             width = img->width >> (img->res - 3);
    int             height = img->height >> (img->res - 3);

    src = img->mmap + pcd_img_start[3] +
	(pcd_def_width[3] >> 1) * 3 * top;

    for (y = 0; y < height; y += 2) {
	/* luma */
	src += left;
	for (dest = img->luma[y << 1], x = 0; x < width - 1; x++) {
	    *(dest++) = src[x], *(dest++) = (src[x] + src[x + 1] + 1) >> 1;
	}
	*(dest++) = src[x], *(dest++) = src[x];
	src += pcd_def_width[3];
	for (dest = img->luma[(y << 1) + 2], x = 0; x < width - 1; x++) {
	    *(dest++) = src[x], *(dest++) = (src[x] + src[x + 1] + 1) >> 1;
	}
	*(dest++) = src[x], *(dest++) = src[x];
	src += pcd_def_width[3] - left;

	/* chroma */
	src += left >> 1;
	for (dest = img->blue[y], x = 0; x < (width >> 1) - 1; x++) {
	    *(dest++) = src[x], *(dest++) = (src[x] + src[x + 1] + 1) >> 1;
	}
	*(dest++) = src[x], *(dest++) = src[x];
	src += pcd_def_width[3] >> 1;
	for (dest = img->red[y], x = 0; x < (width >> 1) - 1; x++) {
	    *(dest++) = src[x], *(dest++) = (src[x] + src[x + 1] + 1) >> 1;
	}
	*(dest++) = src[x], *(dest++) = src[x];
	src += (pcd_def_width[3] - left) >> 1;
    }
    return 0;
}

int CCPCD::pcd_get_image_line_0(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale)
{
    unsigned char red[3072];
    unsigned char blue[3072];
    int             uint8_ts, maxx;

    switch (type) {
    case PCD_TYPE_GRAY:
	uint8_ts = 1;
	break;
    case PCD_TYPE_RGB:
	uint8_ts = 3;
	break;
    case PCD_TYPE_BGR:
	uint8_ts = 3;
	break;
    case PCD_TYPE_LUT_SHORT:
	uint8_ts = 2;
	break;
    case PCD_TYPE_LUT_LONG:
	uint8_ts = 4;
	break;
    default:
	fprintf(stderr, "Oops: invalid type (%i) for output format\n", type);
	exit(1);
    }

    if (img->rot & 2) {
	y = (img->height >> scale) - y - 1;
	dest += ((img->width >> scale) - 1) * uint8_ts;
	uint8_ts = -uint8_ts;
    }
    if (type != PCD_TYPE_GRAY && !scale) {
	 int    x;
	 unsigned char *src1, *src2;

	maxx = (img->width >> 1) - 1;
	if (y & 1) {
	    src1 = img->blue[y >> 1];
	    src2 = img->blue[(y + 1 == img->height ? y : y + 1) >> 1];
	    for (x = 0; x < maxx; x++) {
		blue[x * 2] = (src1[x] + src2[x] + 1) >> 1;
		blue[x * 2 + 1] = (src1[x] + src1[x + 1] + src2[x] + src2[x + 1] + 2) >> 2;
	    }
	    blue[x * 2 + 1] = blue[x * 2] = (src1[x] + src2[x] + 1) >> 1;

	    src1 = img->red[y >> 1];
	    src2 = img->red[(y + 1 == img->height ? y : y + 1) >> 1];
	    for (x = 0; x < maxx; x++) {
		red[x * 2] = (src1[x] + src2[x] + 1) >> 1;
		red[x * 2 + 1] = (src1[x] + src1[x + 1] + src2[x] + src2[x + 1] + 2) >> 2;
	    }
	    red[x * 2 + 1] = red[x * 2] = (src1[x] + src2[x] + 1) >> 1;
	} else {
	    src1 = img->blue[y >> 1];
	    for (x = 0; x < maxx; x++) {
		blue[x * 2] = src1[x];
		blue[x * 2 + 1] = (src1[x] + src1[x + 1] + 1) >> 1;
	    }
	    blue[x * 2 + 1] = blue[x * 2] = src1[x];

	    src1 = img->red[y >> 1];
	    for (x = 0; x < maxx; x++) {
		red[x * 2] = src1[x];
		red[x * 2 + 1] = (src1[x] + src1[x + 1] + 1) >> 1;
	    }
	    red[x * 2 + 1] = red[x * 2] = src1[x];
	}
    }
    if (type != PCD_TYPE_GRAY && scale) {
	memcpy(blue, img->blue[y], img->width >> 1);
	memcpy(red, img->red[y], img->width >> 1);
    }
    maxx = img->width >> scale;
    switch (type) {
    case PCD_TYPE_GRAY:
	{
	     int    x;
	     unsigned char *luma = img->luma[y << scale];

	    for (x = 0; x < maxx; x++, dest += uint8_ts)
		*dest = LUT_gray_char[luma[x << scale]];
	}
	break;
    case PCD_TYPE_RGB:
	{
	     int    x, gray;
	     unsigned char *luma = img->luma[y << scale];

	    for (x = 0; x < maxx; x++, dest += uint8_ts) {
		gray = LUT_gray_int[luma[x << scale]];
		dest[0] = GET_RED;
		dest[1] = GET_GREEN;
		dest[2] = GET_BLUE;
	    }
	}
	break;
    case PCD_TYPE_BGR:
	{
	     int    x, gray;
	     unsigned char *luma = img->luma[y << scale];

	    for (x = 0; x < maxx; x++, dest += uint8_ts) {
		gray = LUT_gray_int[luma[x << scale]];
		dest[0] = GET_BLUE;
		dest[1] = GET_GREEN;
		dest[2] = GET_RED;
	    }
	}
	break;
    case PCD_TYPE_LUT_SHORT:
	{
	     int    x, gray;
	     unsigned char *luma = img->luma[y << scale];
	    unsigned long  *lr = img->lut_red;
	    unsigned long  *lg = img->lut_green;
	    unsigned long  *lb = img->lut_blue;

	    for (x = 0; x < maxx; x++, dest += uint8_ts) {
		gray = LUT_gray_int[luma[x << scale]];
		*((unsigned long *) dest) =
		    lr[GET_RED] | lg[GET_GREEN] | lb[GET_BLUE];
	    }
	}
	break;
    case PCD_TYPE_LUT_LONG:
	{
	     int    x, gray;
	     unsigned char *luma = img->luma[y << scale];
	    unsigned long  *lr = img->lut_red;
	    unsigned long  *lg = img->lut_green;
	    unsigned long  *lb = img->lut_blue;

	    for (x = 0; x < maxx; x++, dest += uint8_ts) {
		gray = LUT_gray_int[luma[x << scale]];
		*((unsigned long *) dest) =
		    lr[GET_RED] | lg[GET_GREEN] | lb[GET_BLUE];
	    }
	}
	break;
    default:
		return -1;
    }
    return 0;
}


int CCPCD::pcd_get_image(struct PCD_IMAGE *img, unsigned char *dest, int type, int scale)
{
    int             y, maxx, maxy, uint8_ts;

    if (img->res == 0) 
	{
		return -1;
    }
    switch (type) 
	{
		case PCD_TYPE_GRAY:
			uint8_ts = 1;
			break;
		case PCD_TYPE_RGB:
			uint8_ts = 3;
			break;
		case PCD_TYPE_BGR:
			uint8_ts = 3;
			break;
		case PCD_TYPE_LUT_SHORT:
			uint8_ts = 2;
			break;
		case PCD_TYPE_LUT_LONG:
			uint8_ts = 4;
			break;
		default:
			return -1;
    }
    maxx = (img->rot & 1 ? img->height : img->width) >> scale;
    maxy = (img->rot & 1 ? img->width : img->height) >> scale;

    for (y = 0; y < maxy; y++, dest += uint8_ts * maxx) 
	{
		pcd_get_image_line(img, y, dest, type, scale);
    }

    return 0;
}

int CCPCD::pcd_get_image_line_90(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale)
{
    unsigned char red[3072];
    unsigned char blue[3072];
    unsigned char **luma = img->luma;
    int             uint8_ts, maxx, y1, y2;

    switch (type) 
	{
		case PCD_TYPE_GRAY:
			uint8_ts = 1;
			break;
		case PCD_TYPE_RGB:
			uint8_ts = 3;
			break;
		case PCD_TYPE_BGR:
			uint8_ts = 3;
			break;
		case PCD_TYPE_LUT_SHORT:
			uint8_ts = 2;
			break;
		case PCD_TYPE_LUT_LONG:
			uint8_ts = 4;
			break;
		default:
			return -1;
	}

    if (!(img->rot & 2)) 
	{
		y = (img->width >> scale) - y - 1;
    } 
	else 
	{
		dest += ((img->height >> scale) - 1) * uint8_ts;
		uint8_ts = -uint8_ts;
    }

    if (type != PCD_TYPE_GRAY && !scale) 
	{
	 int    x;
	 unsigned char **src;

	y1 = y >> 1;
	y2 = (y + 1 == img->width ? y : y + 1) >> 1;
	maxx = (img->height >> 1) - 1;

	if (y & 1) 
	{
	    src = img->blue;
	    for (x = 0; x < maxx; x++) 
		{
			blue[x * 2] = (src[x][y1] + src[x][y2] + 1) >> 1;
			blue[x * 2 + 1] = (src[x][y1] + src[x][y2] +
					   src[x + 1][y1] + src[x + 1][y2] + 2) >> 2;
	    }
	    blue[x * 2 + 1] = blue[x * 2] = (src[x][y1] + src[x][y2] + 1) >> 1;

	    src = img->red;
	    for (x = 0; x < maxx; x++) 
		{
			red[x * 2] = (src[x][y1] + src[x][y2] + 1) >> 1;
			red[x * 2 + 1] = (src[x][y1] + src[x][y2] +
					   src[x + 1][y1] + src[x + 1][y2] + 2) >> 2;
	    }
	    red[x * 2 + 1] = red[x * 2] = (src[x][y1] + src[x][y2] + 1) >> 1;
	} 
	else 
	{
	    src = img->blue;
	    for (x = 0; x < maxx; x++) {
		blue[x * 2] = src[x][y1];
		blue[x * 2 + 1] = (src[x][y1] + src[x + 1][y1] + 1) >> 1;
	    }
	    blue[x * 2 + 1] = blue[x * 2] = src[x][y1];

	    src = img->red;
	    for (x = 0; x < maxx; x++) {
		red[x * 2] = src[x][y1];
		red[x * 2 + 1] = (src[x][y1] + src[x + 1][y1] + 1) >> 1;
	    }
	    red[x * 2 + 1] = red[x * 2] = src[x][y1];
	}
    }
    if (type != PCD_TYPE_GRAY && scale) 
	{
	 int    x;
	 unsigned char **src;

	maxx = (img->height >> 1);

	src = img->blue;
	for (x = 0; x < maxx; x++)
	    blue[x] = src[x][y];

	src = img->red;
	for (x = 0; x < maxx; x++)
	    red[x] = src[x][y];
    }
    maxx = (img->height >> scale);
    switch (type) 
	{
		case PCD_TYPE_GRAY:
			{
				 int    x;

				for (x = 0; x < maxx; x++, dest += uint8_ts)
				*dest = LUT_gray_char[luma[x << scale][y << scale]];
			}
			break;
		case PCD_TYPE_RGB:
			{
				 int    x, gray;

				for (x = 0; x < maxx; x++, dest += uint8_ts) {
				gray = LUT_gray_int[luma[x << scale][y << scale]];
				dest[0] = GET_RED;
				dest[1] = GET_GREEN;
				dest[2] = GET_BLUE;
				}
			}
			break;
		case PCD_TYPE_BGR:
			{
				 int    x, gray;

				for (x = 0; x < maxx; x++, dest += uint8_ts) {
				gray = LUT_gray_int[luma[x << scale][y << scale]];
				dest[0] = GET_BLUE;
				dest[1] = GET_GREEN;
				dest[2] = GET_RED;
				}
			}
			break;
		case PCD_TYPE_LUT_SHORT:
			{
				 int    x, gray;
				unsigned long  *lr = img->lut_red;
				unsigned long  *lg = img->lut_green;
				unsigned long  *lb = img->lut_blue;

				for (x = 0; x < maxx; x++, dest += uint8_ts) {
				gray = LUT_gray_int[luma[x << scale][y << scale]];
				*((unsigned long *) dest) =
					lr[GET_RED] | lg[GET_GREEN] | lb[GET_BLUE];
				}
			}
			break;
		case PCD_TYPE_LUT_LONG:
			{
				 int    x, gray;
				unsigned long  *lr = img->lut_red;
				unsigned long  *lg = img->lut_green;
				unsigned long  *lb = img->lut_blue;

				for (x = 0; x < maxx; x++, dest += uint8_ts) {
				gray = LUT_gray_int[luma[x << scale][y << scale]];
				*((unsigned long *) dest) =
					lr[GET_RED] | lg[GET_GREEN] | lb[GET_BLUE];
				}
			}
			break;
		default:
			return -1;
    }
    return 0;
}

int CCPCD::pcd_get_image_line(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale)
{
    if (img->res == 0) 
	{
		return -1;
    }
    if (img->rot & 1)
		return pcd_get_image_line_90(img, y, dest, type, scale);
    else
		return pcd_get_image_line_0(img, y, dest, type, scale);
}

void CCPCD::pcd_get_LUT_init()
{
    int    i;

    /* only once needed */
    if (LUT_flag)
		return;

    LUT_flag = 1;

    /* init Lookup tables */
    for (i = 0; i < 256; i++) {
	LUT_gray_int[i] = i * LUN_MUL >> 8;
	LUT_red[i] = (RED_ADD + i * RED_MUL) >> 8;
	LUT_blue[i] = (BLUE_ADD + i * BLUE_MUL) >> 8;
	LUT_green1[i] = (GREEN1_ADD + i * GREEN1_MUL) >> 8;
	LUT_green2[i] = (GREEN2_ADD + i * GREEN2_MUL) >> 8;
	LUT_gray_char[i] = LUT_gray_int[i] > 255 ? 255 : LUT_gray_int[i];

	LUT_15_red[i] = (i & 0xf8) << 7;	/* bits -rrrrr-- -------- */
	LUT_15_green[i] = (i & 0xf8) << 2;	/* bits------gg ggg----- */
	LUT_15_blue[i] = (i & 0xf8) >> 3;	/* bits-------- ---bbbbb */

	LUT_16_red[i] = (i & 0xf8) << 8;	/* bits rrrrr--- -------- */
	LUT_16_green[i] = (i & 0xfc) << 3;	/* bits -----ggg ggg----- */
	LUT_16_blue[i] = (i & 0xf8) >> 3;	/* bits -------- ---bbbbb */

	LUT_24_red[i] = i << 16;	/* uint8_t -r-- */
	LUT_24_green[i] = i << 8;	/* uint8_t --g- */
	LUT_24_blue[i] = i;	/* uint8_t ---b */
    }
    for (i = 0; i < RANGE; i++)
	LUT_range[i] = 0;
    for (; i < RANGE + 256; i++)
	LUT_range[i] = i - RANGE;
    for (; i < 2 * RANGE + 256; i++)
	LUT_range[i] = 255;
}

void ReadFile(char *name)
{
	FILE *file;
	char *buffer;
	unsigned long fileLen;

	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
		return;
	}
	
	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return;
	}

	//Read file contents into buffer
	fread(buffer, fileLen, 1, file);
	fclose(file);

	//Do what ever with buffer

	free(buffer);
}

int CCPCD::pcd_open (struct PCD_IMAGE *img,const string & filename)
{
  FILE * fd = nullptr;

  pcd_get_LUT_init();
  memset(img, 0, sizeof(struct PCD_IMAGE));
  

  fd = fopen(filename.c_str(), "r");
  if (fd == nullptr)
    {
      return -1;
    }
   
  fseek(fd, 0, SEEK_END);
  img->size = ftell(fd);
  //fclose(fd);
  //fd = fopen(filename.c_str(), "r");
  //fseek(fd, 0, SEEK_SET);
  rewind (fd);

  img->mmap = (unsigned char *)malloc(img->size);

  if (img->mmap == nullptr) 
    {
      pcd_close(img);
      return -1;
    }

   int result =fread(img->mmap, 1, img->size, fd);
  //fread(fd,img->mmap,img->size);
	
  //int result =  fread(img->mmap, 1, img->size, fd);
   /*
  if(result != img->size)
  {
	  fclose(fd);
      pcd_close(img);
      return -1;
  }*/
  
  fclose(fd);

  if (0 == strncmp("PCD_OPA", (const char *)img->mmap, 7)) 
    {
      /* this is the thumbnails file */
      img->thumbnails = (int) img->mmap[10] << 8 | (int) img->mmap[11];
    } 
  else 
    {
      if (img->size < 786432)
	{
	  pcd_close(img);
	  return -1;
	}
    }
  return img->thumbnails;
}


int CCPCD::pcd_get_rot (struct PCD_IMAGE *img, int nr)
{
  if (img->thumbnails) 
    {
      return img->mmap[12 + nr] & 3;
    } 
  else 
    {
      return img->mmap[0x48] & 3;
    }
	return 0;
}

int CCPCD::pcd_get_maxres (struct PCD_IMAGE *img)
{
  if (img->thumbnails) 
    {
      return 1;
    } 
  else 
    {
      if (img->size == 786432)
	return 3;
      else
	return 5;
    }
}

int CCPCD::pcd_select (struct PCD_IMAGE *img, int res, int nr, int gray, int verbose, int rot, int *left, int *top, int *width, int *height)
{
  int             y;
  unsigned char  *ptr;

  /* free old stuff */
  pcd_free(img);

  /* sanity checks... */
  if (0 == img->thumbnails) 
  {
    if (res < 1 || res > 5) 
	{
	  return -1;
	}
    if (img->size == 786432 && res > 3) 
    {
		return -1;
    }
  } 
  else 
  {
    if (nr < 0 || nr >= img->thumbnails) 
	{
	  return -1;
	}
  }

  /* width/height == 0: fill in default image size */
  if (*left == 0 && *width == 0)
    *width = PCD_WIDTH(res, rot);
  if (*top == 0 && *height == 0)
    *height = PCD_HEIGHT(res, rot);

  if (5 == res)
    *left &= ~7, *top &= ~7, *width &= ~7, *height &= ~7;
  else if (4 == res)
    *left &= ~3, *top &= ~3, *width &= ~3, *height &= ~3;
  else
    *left &= ~1, *top &= ~1, *width &= ~1, *height &= ~1;
  if (*left < 0 || *top < 0 || *width < 1 || *height < 1 ||
      *left + *width > PCD_WIDTH(res, rot) ||
      *top + *height > PCD_HEIGHT(res, rot)) 
    {
      return -1;
    }
  /* recalc coordinates (rotation) */
  switch (rot) 
    {
    case 0:			/* none */
      img->left = *left;
      img->top = *top;
      img->width = *width;
      img->height = *height;
      break;
    case 1:			/* 90° ccw */
      img->left = PCD_HEIGHT(res, rot) - *top - *height;
      img->top = *left;
      img->width = *height;
      img->height = *width;
      break;
    case 2:			/* 180° */
      img->left = PCD_WIDTH(res, rot) - *left - *width;
      img->top = PCD_HEIGHT(res, rot) - *top - *height;
      img->width = *width;
      img->height = *height;
      break;
    case 3:			/* 90° cw */
      img->left = *top;
      img->top = PCD_WIDTH(res, rot) - *left - *width;
      img->width = *height;
      img->height = *width;
      break;
    default:
      return -1;
    }
  /* prepeare */
  img->res = res;
  img->nr = nr;
  img->gray = gray;
  img->verbose = verbose;
  img->rot = rot;
  img->luma = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
  img->red = (unsigned char **)malloc(img->height * sizeof(unsigned char *) >> 1);
  img->blue = (unsigned char **)malloc(img->height * sizeof(unsigned char *) >> 1);

  if (img->luma == nullptr ||
      img->red == nullptr ||
      img->blue == nullptr) 
    {
      pcd_free(img);
      return -1;
    }
  if (res <= 3) 
    {
      /* just fill in pointers */
      if (img->thumbnails) 
	{
	  ptr = img->mmap + 10240 + 36864 * nr +
	    (pcd_def_width[res] >> 1) * 3 * img->top;
	} 
      else 
	{
	  ptr = img->mmap + pcd_img_start[res] +
	    (pcd_def_width[res] >> 1) * 3 * img->top;
	}
      for (y = 0; y < img->height; 
	   y += 2, ptr += (pcd_def_width[res] >> 1) * 6) 
	{
	  img->luma[y] = ptr + img->left;
	  img->luma[y + 1] = ptr + img->left + (pcd_def_width[res] >> 1) * 2;
	  img->blue[y >> 1] = ptr + (img->left >> 1) + 
	    (pcd_def_width[res] >> 1) * 4;
	  img->red[y >> 1] = ptr + (img->left >> 1) + 
	    (pcd_def_width[res] >> 1) * 5;
	}
    } 
  else 
    {
      /* high res, have to malloc memory */
      img->data = (unsigned char *)malloc(img->width * img->height * 3 / 2);
      if (img->data == nullptr) 
	{
	  pcd_free(img);
	  return -1;
	}
      ptr = img->data;
      for (y = 0; y < img->height; y++, ptr += img->width)
	img->luma[y] = ptr;
      for (y = 0; y < img->height >> 1; y++, ptr += img->width >> 1)
	img->blue[y] = ptr;
      for (y = 0; y < img->height >> 1; y++, ptr += img->width >> 1)
	img->red[y] = ptr;
    }

	return 0;
}

int CCPCD::pcd_free(struct PCD_IMAGE *img)
{
	img->res = 0;
	if (img->data)
		free(img->data);
	if (img->luma)
		free(img->luma);
	if (img->red)
		free(img->red);
	if (img->blue)
		free(img->blue);
	if (img->seq1)
		free(img->seq1);
	if (img->len1)
		free(img->len1);
	if (img->seq2)
		free(img->seq2);
	if (img->len2)
		free(img->len2);
	if (img->seq3)
		free(img->seq3);
	if (img->len3)
		free(img->len3);
	img->data = nullptr;
	img->luma = img->red = img->blue = nullptr;
	img->seq1 = img->seq2 = img->seq3 = nullptr;
	img->len1 = img->len2 = img->len3 = nullptr;
	return 0;
}

int CCPCD::pcd_close(struct PCD_IMAGE *img)
{
  pcd_free(img);
  free(img->mmap);
  //munmap(img->mmap, img->size);
  memset(img, 0, sizeof(struct PCD_IMAGE));

  return 0;
}

void CCPCD::pcd_set_lookup(struct PCD_IMAGE *img, unsigned long *red, unsigned long *green, unsigned long *blue)
{
    img->lut_red = red;
    img->lut_green = green;
    img->lut_blue = blue;
}

/* in and out must be 16-uint8_t aligned */
void CCPCD::rgb_to_bgrx(const int & w, const int & h, const uint8_t * diskmem, uint8_t * buff)
{
	int i, j;
	int picptr = 0, srcptr = 0;

	for (j = 0; j<h; j++) {
		for (i = 0; i<w; i++, picptr += 4, srcptr += 3) {
			memcpy(buff + picptr, diskmem + srcptr, 3 * sizeof(uint8_t));
		}
	}
}

/* in and out must be 16-uint8_t aligned */
void CCPCD::rgb_to_rgbx(const int & w, const int & h, const uint8_t * diskmem, uint8_t * buff)
{
	int i, j;
	int picptr = 0, srcptr = 0;

	for (j = 0; j<h; j++) {
		for (i = 0; i<w; i++, picptr += 4, srcptr += 3) 
		{
			buff[picptr] = diskmem[srcptr+2];
			buff[picptr+1] = diskmem[srcptr+1];
			buff[picptr+2] = diskmem[srcptr];
			buff[picptr+3] = 0;
			//memcpy(buff + picptr, diskmem + srcptr, 3 * sizeof(uint8_t));
		}
	}
}

CRegardsBitmap * CCPCD::readPCD(const string & szFileName, const bool &isThumbnail)
{
	CRegardsBitmap *  bitmap = nullptr;
	int left,top,w,h;
	left = top = w = h = 0;
	PCD_IMAGE m_pcdImage;
	memset(&m_pcdImage,0,sizeof(PCD_IMAGE));
	pcd_open(&m_pcdImage, szFileName);
	if (pcd_select(&m_pcdImage, 4, 0, false, false, pcd_get_rot(&m_pcdImage, 0), &left, &top, &w, &h) == 0)
	{
		if(pcd_decode(&m_pcdImage) == 0)
		{
			//int i = 0;
			uint8_t * m_buf = new uint8_t[w * h * 3];
			pcd_get_image(&m_pcdImage, m_buf, PCD_TYPE_RGB, 0);
			bitmap = new CRegardsBitmap(w,h);
			if(isThumbnail)
				rgb_to_bgrx(w,h, m_buf, bitmap->GetPtBitmap());
			else
			{
				rgb_to_rgbx(w,h, m_buf, bitmap->GetPtBitmap());
				bitmap->VertFlipBuf();
			}
			//m_image.BGRFromCRgbaquad(m_buf, w, h);
			//m_image.VertFlipBuf(m_buf, w * 3, h);

		}
	}
	pcd_close(&m_pcdImage);
	pcd_free(&m_pcdImage);

	return bitmap;
}

void CCPCD::GetJDimensions(const string &  fileName, int & width, int & height)
{
	int left,top,w,h;
	left = top = w = h = 0;
	PCD_IMAGE m_pcdImage;
	memset(&m_pcdImage,0,sizeof(PCD_IMAGE));
	pcd_open(&m_pcdImage,fileName);
	if (pcd_select(&m_pcdImage, 4, 0, false, false, pcd_get_rot(&m_pcdImage, 0), &left, &top, &w, &h) == 0)
	{
		width = w;
		height = h;
	}
	pcd_close(&m_pcdImage);
	pcd_free(&m_pcdImage);
}
