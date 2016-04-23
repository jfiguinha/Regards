/*
 * BPG encoder
 *
 * Copyright (c) 2014 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "libbpg.h"

typedef struct {
    int w, h;
    BPGImageFormatEnum format;
    uint8_t has_alpha;
    uint8_t has_w_plane;
    uint8_t limited_range;
    uint8_t premultiplied_alpha;
    BPGColorSpaceEnum color_space;
    uint8_t bit_depth;
    uint8_t pixel_shift; /* (1 << pixel_shift) bytes per pixel */
    uint8_t *data[4];
    int linesize[4];
} Image;

typedef struct {
    int qp; /* quantizer 0-51 */
    int lossless; /* 0-1 lossless mode */
    int sei_decoded_picture_hash; /* 0=no hash, 1=MD5 hash */
    int compress_level; /* 1-9 */
    int verbose;
} HEVCEncodeParams;


typedef enum {
	HEVC_ENCODER_X265
} HEVCEncoderEnum;

static char *hevc_encoder_name[1] = {
	"x265"
};

int x265_encode_picture(uint8_t **pbuf, Image *img, 
                        const HEVCEncodeParams *params);
int jctvc_encode_picture(uint8_t **pbuf, Image *img, 
                         const HEVCEncodeParams *params);
void save_yuv(Image *img, const char *filename);


#define HEVC_ENCODER_COUNT 1
#define USE_X265 1
#define CONFIG_BPG_VERSION "0.9.4"

typedef uint16_t PIXEL;

void put_ue(uint8_t **pp, uint32_t v);

int clamp_pix(int a, int pixel_max);

int sub_mod_int(int a, int b, int m);

int add_mod_int(int a, int b, int m);

typedef struct {
	int c_shift;
	int c_rnd;
	int c_0_25, c_0_5, c_one;
	int rgb_to_ycc[3 * 3];
	int y_one;
	int y_offset;
	int bit_depth;
	int pixel_max;
	int c_center;
} ColorConvertState;

void convert_init(ColorConvertState *s, int in_bit_depth,
	int out_bit_depth, BPGColorSpaceEnum color_space,
	int limited_range);

/* 8 bit input */
void rgb24_to_ycc(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);


void rgb24_to_rgb(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);

void rgb24_to_ycgco(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);


/* Note: used for alpha/W so no limited range */
void gray8_to_gray(ColorConvertState *s,
	PIXEL *y_ptr, const uint8_t *src, int n, int incr);

void luma8_to_gray(ColorConvertState *s,
	PIXEL *y_ptr, const uint8_t *src, int n, int incr);


/* 16 bit input */

void rgb48_to_ycc(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);


void rgb48_to_ycgco(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);


/* Note: use for alpha/W so no limited range */
void gray16_to_gray(ColorConvertState *s,
	PIXEL *y_ptr, const uint16_t *src, int n, int incr);

void luma16_to_gray(ColorConvertState *s,
	PIXEL *y_ptr, const uint16_t *src, int n, int incr);

void rgb48_to_rgb(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src1, int n, int incr);

typedef void RGBConvertFunc(ColorConvertState *s,
	PIXEL *y_ptr, PIXEL *cb_ptr, PIXEL *cr_ptr,
	const void *src, int n, int incr);

static RGBConvertFunc *rgb_to_cs[2][BPG_CS_COUNT] = {
	{
		rgb24_to_ycc,
		rgb24_to_rgb,
		rgb24_to_ycgco,
		rgb24_to_ycc,
		rgb24_to_ycc,
	},
	{
		rgb48_to_ycc,
		rgb48_to_rgb,
		rgb48_to_ycgco,
		rgb48_to_ycc,
		rgb48_to_ycc,
	}
};

/* val = 1.0 - val */
void gray_one_minus(ColorConvertState *s, PIXEL *y_ptr, int n);

/* val = -val for chroma */
void gray_neg_c(ColorConvertState *s, PIXEL *y_ptr, int n);


/* decimation */

/* phase = 0 */
#define DP0TAPS2 7
#define DP0TAPS (2 * DP0TAPS + 1)
#define DP0C0 64
#define DP0C1 40
#define DP0C3 (-11)
#define DP0C5 4
#define DP0C7 (-1)

/* phase = 0.5 */
#define DP1TAPS2 5
#define DP1TAPS (2 * DP1TAPS2)
#define DP1C0 57
#define DP1C1 17
#define DP1C2 (-8)
#define DP1C3 (-4)
#define DP1C4 2

#define DTAPS_MAX 7

/* chroma aligned with luma samples */
void decimate2p0_simple(PIXEL *dst, PIXEL *src, int n, int bit_depth);

/* same with more precision and no saturation */
void decimate2p0_simple16(int16_t *dst, PIXEL *src, int n, int bit_depth);

/* chroma half way between luma samples */
void decimate2p1_simple(PIXEL *dst, PIXEL *src, int n, int bit_depth);

/* same with more precision and no saturation */
void decimate2p1_simple16(int16_t *dst, PIXEL *src, int n, int bit_depth);

void decimate2_h(PIXEL *dst, PIXEL *src, int n, int bit_depth, int phase);


/* src1 is a temporary buffer of length n + 2 * DTAPS */
void decimate2_h16(int16_t *dst, PIXEL *src, int n, PIXEL *src1,
	int bit_depth, int phase);

void decimate2_v(PIXEL *dst, int16_t **src, int pos, int n,
	int bit_depth);


/* Note: we do the horizontal decimation first to use less CPU cache */
void decimate2_hv(uint8_t *dst, int dst_linesize,
	uint8_t *src, int src_linesize,
	int w, int h, int bit_depth, int h_phase);


void get_plane_res(Image *img, int *pw, int *ph, int i);


#define W_PAD 16

Image *image_alloc(int w, int h, BPGImageFormatEnum format, int has_alpha,
	BPGColorSpaceEnum color_space, int bit_depth);

void image_free(Image *img);

int image_ycc444_to_ycc422(Image *img, int h_phase);

int image_ycc444_to_ycc420(Image *img, int h_phase);

/* duplicate right and bottom samples so that the image has a width
and height multiple of cb_size (power of two) */
void image_pad(Image *img, int cb_size);

/* convert the 16 bit components to 8 bits */
void image_convert16to8(Image *img);

typedef struct BPGMetaData {
	uint32_t tag;
	uint8_t *buf;
	int buf_len;
	struct BPGMetaData *next;
} BPGMetaData;

BPGMetaData *bpg_md_alloc(uint32_t tag);

void bpg_md_free(BPGMetaData *md);

Image *read_regards(BPGMetaData **pmd,
	const unsigned char * data, BPGColorSpaceEnum color_space, int out_bit_depth,
	int limited_range, int premultiplied_alpha, int width, int height);

void save_yuv(Image *img, const char *filename);

/* return the position of the end of the NAL or -1 if error */
int find_nal_end(const uint8_t *buf, int buf_len);

/* return the position of the end of the NAL or -1 if error */
int extract_nal(uint8_t **pnal_buf, int *pnal_len,
	const uint8_t *buf, int buf_len);


/* big endian variable length 7 bit encoding */
void put_ue(uint8_t **pp, uint32_t v);

typedef struct {
	const uint8_t *buf;
	int idx;
	int buf_len;
} GetBitState;

void init_get_bits(GetBitState *s, const uint8_t *buf, int buf_len);

//void skip_bits(GetBitState *s, int n);

/* 1 <= n <= 25. return '0' bits if past the end of the buffer. */
//uint32_t get_bits(GetBitState *s, int n);

/* 1 <= n <= 32 */
//uint32_t get_bits_long(GetBitState *s, int n);

/* at most 32 bits are supported */
uint32_t get_ue_golomb(GetBitState *s);

typedef struct {
	uint8_t *buf;
	int idx;
} PutBitState;

void init_put_bits(PutBitState *s, uint8_t *buf);

void put_bit(PutBitState *s, int bit);

void put_bits(PutBitState *s, int n, uint32_t v);

void put_ue_golomb(PutBitState *s, uint32_t v);

typedef struct {
	uint8_t *buf;
	int size;
	int len;
} DynBuf;

void dyn_buf_init(DynBuf *s);

int dyn_buf_resize(DynBuf *s, int size);

/* suppress the VPS NAL and keep only the useful part of the SPS
header. The decoder can rebuild a valid HEVC stream if needed. */
int build_modified_sps(uint8_t **pout_buf, int *pout_buf_len,
	const uint8_t *buf, int buf_len);

int build_modified_hevc(uint8_t **pout_buf,
	const uint8_t *cbuf, int cbuf_len,
	const uint8_t *abuf, int abuf_len);

int hevc_encode_picture2(uint8_t **pbuf, Image *img,
	HEVCEncodeParams *params,
	HEVCEncoderEnum encoder_type);

#ifdef __cplusplus
}
#endif

