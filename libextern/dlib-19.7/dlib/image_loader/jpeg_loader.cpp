// Copyright (C) 2010  Davis E. King (davis@dlib.net), Nils Labugt
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_JPEG_LOADER_CPp_
#define DLIB_JPEG_LOADER_CPp_

// only do anything with this file if DLIB_JPEG_SUPPORT is defined
#ifdef DLIB_JPEG_SUPPORT

#include "../array2d.h"
#include "../pixel.h"
#include "../dir_nav.h"
#include "jpeg_loader.h"
#include <stdio.h>
#ifdef DLIB_JPEG_STATIC
#   include "../external/libjpeg/jpeglib.h"
#else
#   include <jpeglib.h>
#endif
#include <sstream>
#include <setjmp.h>

namespace dlib
{

// ----------------------------------------------------------------------------------------

    jpeg_loader::
    jpeg_loader( const char* filename ) : height_( 0 ), width_( 0 ), output_components_(0)
    {
        read_image( filename );
    }

	jpeg_loader::jpeg_loader(const unsigned char * data, size_t len) : height_(0), width_(0), output_components_(0)
	{
		read_image(data, len);
	}

// ----------------------------------------------------------------------------------------

    jpeg_loader::
    jpeg_loader( const std::string& filename ) : height_( 0 ), width_( 0 ), output_components_(0)
    {
        read_image( filename.c_str() );
    }

// ----------------------------------------------------------------------------------------

    jpeg_loader::
    jpeg_loader( const dlib::file& f ) : height_( 0 ), width_( 0 ), output_components_(0)
    {
        read_image( f.full_name().c_str() );
    }

// ----------------------------------------------------------------------------------------

    bool jpeg_loader::is_gray() const
    {
        return (output_components_ == 1);
    }

// ----------------------------------------------------------------------------------------

    bool jpeg_loader::is_rgb() const
    {
        return (output_components_ == 3);
    }

// ----------------------------------------------------------------------------------------

    bool jpeg_loader::is_rgba() const
    {
        return (output_components_ == 4);
    }

// ----------------------------------------------------------------------------------------

    struct jpeg_loader_error_mgr 
    {
        jpeg_error_mgr pub;    /* "public" fields */
        jmp_buf setjmp_buffer;  /* for return to caller */
    };

    void jpeg_loader_error_exit (j_common_ptr cinfo)
    {
        /* cinfo->err really points to a jpeg_loader_error_mgr struct, so coerce pointer */
        jpeg_loader_error_mgr* myerr = (jpeg_loader_error_mgr*) cinfo->err;

        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }

// ----------------------------------------------------------------------------------------

    void jpeg_loader::read_image( const char* filename )
    {
        if ( filename == NULL )
        {
            throw image_load_error("jpeg_loader: invalid filename, it is NULL");
        }
        FILE *fp = fopen( filename, "rb" );
        if ( !fp )
        {
            throw image_load_error(std::string("jpeg_loader: unable to open file ") + filename);
        }

        jpeg_decompress_struct cinfo;
        jpeg_loader_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr.pub);

        jerr.pub.error_exit = jpeg_loader_error_exit;

        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) 
        {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            fclose(fp);
            throw image_load_error(std::string("jpeg_loader: error while reading ") + filename);
        }


        jpeg_create_decompress(&cinfo);

        jpeg_stdio_src(&cinfo, fp);

        jpeg_read_header(&cinfo, TRUE);

        jpeg_start_decompress(&cinfo);

        height_ = cinfo.output_height;
        width_ = cinfo.output_width;
        output_components_ = cinfo.output_components;

        if (output_components_ != 1 && 
            output_components_ != 3 &&
            output_components_ != 4)
        {
            fclose( fp );
            jpeg_destroy_decompress(&cinfo);
            std::ostringstream sout;
            sout << "jpeg_loader: Unsupported number of colors (" << output_components_ << ") in file " << filename;
            throw image_load_error(sout.str());
        }

        std::vector<unsigned char*> rows;
        rows.resize(height_);

        // size the image buffer
        data.resize(height_*width_*output_components_);

        // setup pointers to each row
        for (unsigned long i = 0; i < rows.size(); ++i)
            rows[i] = &data[i*width_*output_components_];

        // read the data into the buffer
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, &rows[cinfo.output_scanline], 100);
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        fclose( fp );
    }

	// ----------------------------------------------------------------------------------------
	// This is based on the FILE source manager in libjpeg
	// Read From Buffer
	// ----------------------------------------------------------------------------------------
	const static JOCTET EOI_BUFFER[1] = { JPEG_EOI };
	struct my_source_mgr {
		struct jpeg_source_mgr pub;
		const JOCTET *data;
		size_t       len;
	};

	static void my_init_source(j_decompress_ptr cinfo) {}

	static boolean my_fill_input_buffer(j_decompress_ptr cinfo) {
		my_source_mgr* src = (my_source_mgr*)cinfo->src;
		// No more data.  Probably an incomplete image;  just output EOI.
		src->pub.next_input_byte = EOI_BUFFER;
		src->pub.bytes_in_buffer = 1;
		return TRUE;
	}
	static void my_skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
		my_source_mgr* src = (my_source_mgr*)cinfo->src;
		if (src->pub.bytes_in_buffer < num_bytes) {
			// Skipping over all of remaining data;  output EOI.
			src->pub.next_input_byte = EOI_BUFFER;
			src->pub.bytes_in_buffer = 1;
		}
		else {
			// Skipping over only some of the remaining data.
			src->pub.next_input_byte += num_bytes;
			src->pub.bytes_in_buffer -= num_bytes;
		}
	}
	static void my_term_source(j_decompress_ptr cinfo) {}

	static void my_set_source_mgr(j_decompress_ptr cinfo, const unsigned char * data, size_t len) {
		my_source_mgr* src;
		if (cinfo->src == 0) { // if this is first time;  allocate memory
			cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small)
				((j_common_ptr)cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
		}
		src = (my_source_mgr*)cinfo->src;
		src->pub.init_source = my_init_source;
		src->pub.fill_input_buffer = my_fill_input_buffer;
		src->pub.skip_input_data = my_skip_input_data;
		src->pub.resync_to_restart = jpeg_resync_to_restart; // default
		src->pub.term_source = my_term_source;
		// fill the buffers
		src->data = (const JOCTET *)data;
		src->len = len;
		src->pub.bytes_in_buffer = len;
		src->pub.next_input_byte = src->data;
	}

	void jpeg_loader::read_image(const unsigned char * buffer, size_t len) {
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;

		// Setup decompression structure
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		my_set_source_mgr(&cinfo, buffer, len);

		// read info from header.
		int r = jpeg_read_header(&cinfo, TRUE);
		jpeg_start_decompress(&cinfo);

		// ..<snip>..
		// .. decompress the JPEG here as usual
		// ..<snip>..
		height_ = cinfo.output_height;
		width_ = cinfo.output_width;
		output_components_ = cinfo.output_components;

		if (output_components_ != 1 &&
			output_components_ != 3 &&
			output_components_ != 4)
		{
			jpeg_destroy_decompress(&cinfo);
			std::ostringstream sout;
			sout << "jpeg_loader: Unsupported number of colors (" << output_components_ << ") in buffer ";
			throw image_load_error(sout.str());
		}

		std::vector<unsigned char*> rows;
		rows.resize(height_);

		// size the image buffer
		data.resize(height_*width_*output_components_);

		// setup pointers to each row
		for (unsigned long i = 0; i < rows.size(); ++i)
			rows[i] = &data[i*width_*output_components_];

		// read the data into the buffer
		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, &rows[cinfo.output_scanline], 100);
		}

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);


		jpeg_destroy_decompress(&cinfo);
	}
}

#endif // DLIB_JPEG_SUPPORT

#endif // DLIB_JPEG_LOADER_CPp_


