// CPCD.h: interface for the CCPCD class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <stdint.h>
#include <string>
using namespace std;

class CRegardsBitmap;

#define RANGE       320

struct PCD_IMAGE {
	int             size;
	unsigned char  *mmap;
	int             thumbnails;	/* # of thumbnails, 0 for normal image */

	int             res, nr, gray, verbose;
	int             left, top, width, height, rot;
	unsigned char **luma;
	unsigned char **red;
	unsigned char **blue;
	unsigned char  *data;

	unsigned long  *lut_red;
	unsigned long  *lut_green;
	unsigned long  *lut_blue;

	unsigned char  *seq1;	/* huffman tables */
	unsigned char  *len1;
	unsigned char  *seq2;
	unsigned char  *len2;
	unsigned char  *seq3;
	unsigned char  *len3;
};

class CCPCD  
{
public:
	void GetJDimensions(const string & fileName,int & width,int & height);



	CCPCD();
	virtual ~CCPCD();

	CRegardsBitmap * readPCD(const string & szFileName);



private:

	int pcd_open (struct PCD_IMAGE *img, const string & filename);
	int pcd_get_rot (struct PCD_IMAGE *img, int nr);
	int pcd_get_maxres (struct PCD_IMAGE *img);
	int pcd_select (struct PCD_IMAGE *img, int res, int nr, int gray, int verbose, int rot, int *left, int *top, int *width, int *height);
	int pcd_free(struct PCD_IMAGE *img);
	int pcd_close(struct PCD_IMAGE *img);
	void pcd_set_lookup(struct PCD_IMAGE *img, unsigned long *red, unsigned long *green, unsigned long *blue);
	int pcd_get_image_line_0(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale);
	int pcd_get_image_line_90(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale);
	int pcd_get_image_line(struct PCD_IMAGE *img, int y, unsigned char *dest, int type, int scale);
	int pcd_get_image(struct PCD_IMAGE *img, unsigned char *dest, int type, int scale);
	int pcd_inter_m2(struct PCD_IMAGE *img);
	int pcd_inter_pixels(unsigned char **data, int width, int height);
	int pcd_inter_lines(unsigned char **data, int width, int height);
	int pcd_read_htable(unsigned char *src, unsigned char **pseq, unsigned char **pbits);
	int pcd_un_huff(struct PCD_IMAGE *img, unsigned char *start, int run);
	int pcd_decode(struct PCD_IMAGE *img);
	
	void rgb_to_bgrx(const int & w, const int & h, const uint8_t * diskmem, uint8_t * buff);

	void pcd_get_LUT_init();

	unsigned int LUT_flag;
	unsigned char LUT_gray_char[256];
	unsigned int LUT_gray_int[256];
	unsigned int LUT_red[256];
	unsigned int LUT_blue[256];
	unsigned int LUT_green1[256];
	unsigned int LUT_green2[256];
	unsigned int    LUT_range[256 + 2 * RANGE];

	unsigned long   LUT_15_red[256];
	unsigned long   LUT_15_green[256];
	unsigned long   LUT_15_blue[256];

	unsigned long   LUT_16_red[256];
	unsigned long   LUT_16_green[256];
	unsigned long   LUT_16_blue[256];

	unsigned long   LUT_24_red[256];
	unsigned long   LUT_24_green[256];
	unsigned long   LUT_24_blue[256];



	//HANDLE fileh;

};


