#pragma once

#if JPEG_LIB_VERSION < 80

#define JPEG_MEMSRC(A,B,C) jpeg_mem_src_int(A,B,C)
/* Read JPEG image from a memory segment */

static void init_source (j_decompress_ptr cinfo) {}
static wxjpeg_boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;
  return !!src->bytes_in_buffer;
}
static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

  if (num_bytes > (int)src->bytes_in_buffer)
    throw("JPEG Decoder - read out of buffer");
  if (num_bytes > 0) {
    src->next_input_byte += (size_t) num_bytes;
    src->bytes_in_buffer -= (size_t) num_bytes;
  }
}
static void term_source (j_decompress_ptr cinfo) {}
static void jpeg_mem_src (j_decompress_ptr cinfo, unsigned char* buffer, long nbytes)
{
  struct jpeg_source_mgr* src;

  if (cinfo->src == NULL) {   /* first time for this JPEG object? */
    cinfo->src = (struct jpeg_source_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
      sizeof(struct jpeg_source_mgr));
  }

  src = (struct jpeg_source_mgr*) cinfo->src;
  src->init_source = init_source;
  src->fill_input_buffer = fill_input_buffer;
  src->skip_input_data = skip_input_data;
  src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->term_source = term_source;
  src->bytes_in_buffer = nbytes;
  src->next_input_byte = (JOCTET*)buffer;
}
#else
#define JPEG_MEMSRC(A,B,C) jpeg_mem_src(A,B,C)
#endif