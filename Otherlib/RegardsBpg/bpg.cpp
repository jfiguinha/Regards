// DllFormatBpg.cpp : définit les fonctions exportées pour l'application DLL.
//

#include "bpg.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C"
{
#include "bpgenc.h"
#include <png.h>
}
#define EXPORT __attribute__((visibility("default")))

EXPORT
CBpg::CBpg() {

}

EXPORT
CBpg* NewBpg(void) {
    return new CBpg;
}

EXPORT void DeleteBpg(CBpg * bpg) {
    delete bpg;
}

void CBpg::GetDimensionsBpg(const char * fileName, int & width, int & height)
{
	FILE *f;
	uint8_t *buf;
	BPGDecoderContext *img;
	BPGImageInfo img_info_s;
	int buf_len;
	f = fopen(fileName, "rb");
	if (!f)
	{
		width = 0;
		height = 0;
		return;
	}
	else
	{
		fseek(f, 0, SEEK_END);
		buf_len = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = (uint8_t *)malloc(buf_len);
		if (fread(buf, 1, buf_len, f) == buf_len)
		{
			fclose(f);
			img = bpg_decoder_open();
			if (bpg_decoder_decode(img, buf, buf_len) >= 0)
			{
				bpg_decoder_get_info(img, &img_info_s);
				width = img_info_s.width;
				height = img_info_s.height;
			}
			bpg_decoder_close(img);
		}

	}
}


#define IMAGE_HEADER_MAGIC 0x425047fb

#define DEFAULT_OUTFILENAME "out.bpg"
#define DEFAULT_QP 28
#define DEFAULT_BIT_DEPTH 8

#ifdef RExt__HIGH_BIT_DEPTH_SUPPORT
#define BIT_DEPTH_MAX 14
#else
#define BIT_DEPTH_MAX 12
#endif
#define DEFAULT_COMPRESS_LEVEL 8

int CBpg::WritePictureBpg(const char * fileName, const uint8_t * data, int & width, int & height, int lossless_mode, int compress_level)
{
	Image *img, *img_alpha;
	HEVCEncodeParams p_s, *p = &p_s;
	uint8_t *out_buf, *alpha_buf, *extension_buf;
	int out_buf_len, alpha_buf_len, verbose;
	uint8_t *hevc_buf;
	int hevc_buf_len;
	int qp, c, option_index, sei_decoded_picture_hash, extension_buf_len;
	int keep_metadata, cb_size, alpha_qp;
	int bit_depth, i, limited_range, premultiplied_alpha;
	int c_h_phase;
	BPGImageFormatEnum format;
	BPGColorSpaceEnum color_space;
	BPGMetaData *md;
	HEVCEncoderEnum encoder_type;
	qp = DEFAULT_QP;
	alpha_qp = -1;
	sei_decoded_picture_hash = 0;
	format = BPG_FORMAT_420;
	c_h_phase = 1;
	color_space = BPG_CS_YCbCr;
	keep_metadata = 0;
	verbose = 0;
	compress_level = DEFAULT_COMPRESS_LEVEL;
	bit_depth = DEFAULT_BIT_DEPTH;
	lossless_mode = 0;
	encoder_type = HEVC_ENCODER_X265;
	limited_range = 0;
	premultiplied_alpha = 0;

	img = read_regards(&md, data, color_space, bit_depth, limited_range, premultiplied_alpha, width, height);

	if (!img) {
		return ERROR_CREATING_IMAGE;
	}

	if (!keep_metadata && md) {
		bpg_md_free(md);
		md = NULL;
	}

	img->has_alpha = 0;

	/* extract the alpha plane */
	if (img->has_alpha) {
		int c_idx;

		img_alpha = (Image *)malloc(sizeof(Image));
		memset(img_alpha, 0, sizeof(*img_alpha));
		if (img->format == BPG_FORMAT_GRAY)
			c_idx = 1;
		else
			c_idx = 3;

		img_alpha->w = img->w;
		img_alpha->h = img->h;
		img_alpha->format = BPG_FORMAT_GRAY;
		img_alpha->has_alpha = 0;
		img_alpha->color_space = BPG_CS_YCbCr;
		img_alpha->bit_depth = bit_depth;
		img_alpha->pixel_shift = img->pixel_shift;
		img_alpha->data[0] = img->data[c_idx];
		img_alpha->linesize[0] = img->linesize[c_idx];

		img->data[c_idx] = NULL;
		img->has_alpha = 0;
	}
	else {
		img_alpha = NULL;
	}

	if (img->format == BPG_FORMAT_444) {
		if (format == BPG_FORMAT_420) {
			if (image_ycc444_to_ycc420(img, c_h_phase) != 0)
				goto error_convert;
		}
		else if (format == BPG_FORMAT_422) {
			if (image_ycc444_to_ycc422(img, c_h_phase) != 0)  {
			error_convert:
				//fprintf(stderr, "Cannot convert image\n");
				//exit(1);
				return ERROR_CONVERT_IMAGE;
			}
		}
	}

	cb_size = 8; /* XXX: should make it configurable. We assume the
				 HEVC encoder uses the same value */
	width = img->w;
	height = img->h;
	image_pad(img, cb_size);
	if (img_alpha)
		image_pad(img_alpha, cb_size);

	/* convert to the allocated pixel width to 8 bit if needed by the
	HEVC encoder */
	if (img->bit_depth == 8) {
		image_convert16to8(img);
		if (img_alpha)
			image_convert16to8(img_alpha);
	}

	memset(p, 0, sizeof(*p));
	p->qp = qp;
	p->lossless = lossless_mode;
	p->sei_decoded_picture_hash = sei_decoded_picture_hash;
	p->compress_level = compress_level;
	p->verbose = verbose;
	out_buf_len = hevc_encode_picture2(&out_buf, img, p, encoder_type);
	if (out_buf_len < 0) {
		//fprintf(stderr, "Error while encoding picture\n");
		//exit(1);
		return ERROR_ENCODING_PICTURE;
	}

	alpha_buf = NULL;
	alpha_buf_len = 0;
	if (img_alpha) {
		memset(p, 0, sizeof(*p));
		if (alpha_qp < 0)
			p->qp = qp;
		else
			p->qp = alpha_qp;
		p->lossless = lossless_mode;
		p->sei_decoded_picture_hash = sei_decoded_picture_hash;
		p->compress_level = compress_level;
		p->verbose = verbose;

		alpha_buf_len = hevc_encode_picture2(&alpha_buf, img_alpha, p, encoder_type);
		if (alpha_buf_len < 0) {
			//fprintf(stderr, "Error while encoding picture (alpha plane)\n");
			return ERROR_ENCODING_ALPHA_PANE;
		}
	}

	hevc_buf = NULL;
	hevc_buf_len = build_modified_hevc(&hevc_buf, out_buf, out_buf_len,
		alpha_buf, alpha_buf_len);
	if (hevc_buf_len < 0) {
		//fprintf(stderr, "Error while creating HEVC data\n");
		//exit(1);
		return ERROR_HEVC_DATA;
	}
	free(out_buf);
	free(alpha_buf);


	//-----------------------------------------------------------------------
	//Write Extension
	//-----------------------------------------------------------------------
	/* prepare the extension data */
	extension_buf = NULL;
	extension_buf_len = 0;
	if (md) {
		BPGMetaData *md1;
		int max_len;
		uint8_t *q;

		max_len = 0;
		for (md1 = md; md1 != NULL; md1 = md1->next) {
			max_len += md1->buf_len + 5 * 2;
		}
		extension_buf = (uint8_t *)malloc(max_len);
		q = extension_buf;
		for (md1 = md; md1 != NULL; md1 = md1->next) {
			put_ue(&q, md1->tag);
			put_ue(&q, md1->buf_len);
			memcpy(q, md1->buf, md1->buf_len);
			q += md1->buf_len;
		}
		extension_buf_len = q - extension_buf;

		bpg_md_free(md);
	}


	FILE * f = fopen(fileName, "wb");
	if (!f) {
		return -1;
	}

	{
		uint8_t img_header[128], *q;
		int v, has_alpha, has_extension, alpha2_flag, alpha1_flag, format;

		has_alpha = (img_alpha != NULL);
		has_extension = (extension_buf_len > 0);


		if (has_alpha) {
			if (img->has_w_plane) {
				alpha1_flag = 0;
				alpha2_flag = 1;
			}
			else {
				alpha1_flag = 1;
				alpha2_flag = img->premultiplied_alpha;
			}
		}
		else {
			alpha1_flag = 0;
			alpha2_flag = 0;
		}

		q = img_header;
		*q++ = (IMAGE_HEADER_MAGIC >> 24) & 0xff;
		*q++ = (IMAGE_HEADER_MAGIC >> 16) & 0xff;
		*q++ = (IMAGE_HEADER_MAGIC >> 8) & 0xff;
		*q++ = (IMAGE_HEADER_MAGIC >> 0) & 0xff;

		if (c_h_phase == 0 && img->format == BPG_FORMAT_420)
			format = BPG_FORMAT_420_VIDEO;
		else if (c_h_phase == 0 && img->format == BPG_FORMAT_422)
			format = BPG_FORMAT_422_VIDEO;
		else
			format = img->format;
		v = (format << 5) | (alpha1_flag << 4) | (img->bit_depth - 8);
		*q++ = v;
		v = (img->color_space << 4) | (has_extension << 3) |
			(alpha2_flag << 2) | (img->limited_range << 1);
		*q++ = v;
		put_ue(&q, width);
		put_ue(&q, height);

		put_ue(&q, 0); /* zero length means up to the end of the file */
		if (has_extension) {
			put_ue(&q, extension_buf_len); /* extension data length */
		}

		fwrite(img_header, 1, q - img_header, f);

		if (has_extension) {
			if (fwrite(extension_buf, 1, extension_buf_len, f) != extension_buf_len) {
				//fprintf(stderr, "Error while writing extension data\n");
				return ERROR_WRITING_EXTENSION_DATA;
			}
			free(extension_buf);
		}

		if (fwrite(hevc_buf, 1, hevc_buf_len, f) != hevc_buf_len) {
			//fprintf(stderr, "Error while writing HEVC image planes\n");
			//exit(1);
			return ERROR_WRITING_HEVC_PLANES;
		}
		free(hevc_buf);
	}

	fclose(f);

	image_free(img);
	if (img_alpha)
		image_free(img_alpha);

	return 0;
}

void CBpg::Test(const char * fileName, int * width, int * height)
{
    *width = 0;
    *height = 0;
}



int CBpg::GetPictureBpg(const char * fileName, int &size, int & width, int & height, uint8_t * & outputBuffer, const int &outputSize)
{
	FILE *f;
	uint8_t *buf;
	BPGDecoderContext *img;
	BPGImageInfo img_info_s;
	int buf_len;
	f = fopen(fileName, "rb");
	if (!f)
	{
		size = 0;
		width = 0;
		height = 0;
		return NULL;
	}
	else
	{
		fseek(f, 0, SEEK_END);
		buf_len = ftell(f);
		fseek(f, 0, SEEK_SET);

		buf = (uint8_t *)malloc(buf_len);
		if (fread(buf, 1, buf_len, f) == buf_len)
		{
			fclose(f);
			img = bpg_decoder_open();

			if (bpg_decoder_decode(img, buf, buf_len) >= 0)
			{
				bpg_decoder_get_info(img, &img_info_s);
	
				width = img_info_s.width;
				height = img_info_s.height;
				size = (width * height) * 4;
				int linesize = width * 4;
				if (outputBuffer == nullptr)
				{
                    free(buf);
                    bpg_decoder_close(img);
					return 0;
				}

				if (outputSize != size)
                {
                    free(buf);
                    bpg_decoder_close(img);
                    return -1;
                }
					
			
                
				uint16_t * rgb_line = new uint16_t[linesize];
				bpg_decoder_start(img, BPG_OUTPUT_FORMAT_RGBA32);
				int i = 0;
				for (int y = 0; y < height; y++)
				{
					i = (height - (y + 1)) * width * 4;
					bpg_decoder_get_line(img, rgb_line);
					for (int x = 0; x < linesize; x += 4, i += 4)
					{
						outputBuffer[i] = rgb_line[x + 2];
						outputBuffer[i + 1] = rgb_line[x + 1];
						outputBuffer[i + 2] = rgb_line[x];
						outputBuffer[i + 3] = rgb_line[x + 3];
					}
				}
                 
                /*
                int i = 0;
                uint8_t * rgb_line = new uint8_t[3 * width];
                bpg_decoder_start(img, BPG_OUTPUT_FORMAT_RGB24);
                for (int y = 0; y < height; y++) {
                    bpg_decoder_get_line(img, rgb_line);
                    for (int x = 0; x < linesize; x += 3, i += 4)
                    {
                        outputBuffer[i] = rgb_line[x];
                        outputBuffer[i + 1] = rgb_line[x + 1];
                        outputBuffer[i + 2] = rgb_line[x + 2];
                        outputBuffer[i + 3] = 0;
                    }
                }
                */
				delete[] rgb_line;
				free(buf);
			}
			bpg_decoder_close(img);
		}

	}
	return 0;
}




static void png_write_data (png_structp png_ptr, png_bytep data,
                            png_size_t length)
{
    std::vector<uint8_t> *p;
    int ret;
    
    p = (std::vector<uint8_t> *)png_get_io_ptr(png_ptr);
    if(p != nullptr)
        p->insert(p->end(), data, data + length);
    else
        png_error(png_ptr, "PNG Write Error");
    /*
    ret = fwrite(data, 1, length, f);
    if (ret != length)
        png_error(png_ptr, "PNG Write Error");*/
}

static void png_save(BPGDecoderContext *img, int bit_depth, std::vector<uint8_t> *out)
{
    BPGImageInfo img_info_s, *img_info = &img_info_s;
    //std::vector<uint8_t> * f;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep row_pointer;
    int y, color_type, bpp;
    BPGDecoderOutputFormat out_fmt;

    
    if (bit_depth != 8 && bit_depth != 16) {
        fprintf(stderr, "Only bit_depth = 8 or 16 are supported for PNG output\n");
        exit(1);
    }
    
    bpg_decoder_get_info(img, img_info);
    
    /*
    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "%s: I/O error\n", filename);
        exit(1);
    }*/
    
    png_ptr = png_create_write_struct_2(PNG_LIBPNG_VER_STRING,
                                        NULL,
                                        NULL,  /* error */
                                        NULL, /* warning */
                                        NULL,
                                        NULL,
                                        NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_set_write_fn(png_ptr, (png_voidp)out, &png_write_data, NULL);
    
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        fprintf(stderr, "PNG write error\n");
        exit(1);
    }
    
    if (img_info->has_alpha)
        color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else
        color_type = PNG_COLOR_TYPE_RGB;
    
    png_set_IHDR(png_ptr, info_ptr, img_info->width, img_info->height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    png_write_info(png_ptr, info_ptr);
    
#if __BYTE_ORDER__ != __ORDER_BIG_ENDIAN__
    if (bit_depth == 16) {
        png_set_swap(png_ptr);
    }
#endif
    
    if (bit_depth == 16) {
        if (img_info->has_alpha)
            out_fmt = BPG_OUTPUT_FORMAT_RGBA64;
        else
            out_fmt = BPG_OUTPUT_FORMAT_RGB48;
    } else {
        if (img_info->has_alpha)
            out_fmt = BPG_OUTPUT_FORMAT_RGBA32;
        else
            out_fmt = BPG_OUTPUT_FORMAT_RGB24;
    }
    
    bpg_decoder_start(img, out_fmt);
    
    bpp = (3 + img_info->has_alpha) * (bit_depth / 8);
    row_pointer = (png_bytep)png_malloc(png_ptr, img_info->width * bpp);
    for (y = 0; y < img_info->height; y++) {
        bpg_decoder_get_line(img, row_pointer);
        png_write_row(png_ptr, row_pointer);
    }
    png_free(png_ptr, row_pointer);
    
    png_write_end(png_ptr, NULL);
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    //fclose(f);
    
    
    //return nullptr;
}

std::vector<uint8_t> CBpg::GetPngPicture(const char * fileName, int &size, int & width, int & height)
{
    FILE *f;
    BPGDecoderContext *img;
    uint8_t *buf;
    int buf_len, bit_depth, c, show_info;
    const char *p;
    std::vector<uint8_t> vectorBuffer;
    
    bit_depth = 8;
    show_info = 0;
    
    f = fopen(fileName, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", fileName);
        exit(1);
    }
    
    fseek(f, 0, SEEK_END);
    buf_len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    buf = (uint8_t *)malloc(buf_len);
    if (fread(buf, 1, buf_len, f) != buf_len) {
        fprintf(stderr, "Error while reading file\n");
        exit(1);
    }
    
    fclose(f);
    
    img = bpg_decoder_open();
    
    if (bpg_decoder_decode(img, buf, buf_len) < 0) {
        fprintf(stderr, "Could not decode image\n");
        exit(1);
    }
    free(buf);
    
    png_save(img, bit_depth, &vectorBuffer);
    
    bpg_decoder_close(img);
    return vectorBuffer;
}
