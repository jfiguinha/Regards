/*
* This file is part of FFmpeg.
*
* FFmpeg is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* FFmpeg is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with FFmpeg; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "header.h"
#include "ffmpeg_dxva2.h"
#ifdef WIN32
#include "VideoControlInterface.h"
#include <DShow.h>

CVideoControlInterface * videoctrl = nullptr;

extern "C"
{

#include "libavcodec/dxva2.h"

#include "libavutil/avassert.h"
#include "libavutil/buffer.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/pixfmt.h"
}

    /* define all the GUIDs used directly here,
    to avoid problems with inconsistent dxva2api.h versions in mingw-w64 and different MSVC version */
#include <initguid.h>
    DEFINE_GUID(IID_IDirectXVideoDecoderService, 0xfc51a551, 0xd5e7, 0x11d9, 0xaf, 0x55, 0x00, 0x05, 0x4e, 0x43, 0xff, 0x02);

    DEFINE_GUID(DXVA2_ModeMPEG2_VLD, 0xee27417f, 0x5e28, 0x4e65, 0xbe, 0xea, 0x1d, 0x26, 0xb5, 0x08, 0xad, 0xc9);
    DEFINE_GUID(DXVA2_ModeMPEG2and1_VLD, 0x86695f12, 0x340e, 0x4f04, 0x9f, 0xd3, 0x92, 0x53, 0xdd, 0x32, 0x74, 0x60);
    DEFINE_GUID(DXVA2_ModeH264_E, 0x1b81be68, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
    DEFINE_GUID(DXVA2_ModeH264_F, 0x1b81be69, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
    DEFINE_GUID(DXVADDI_Intel_ModeH264_E, 0x604F8E68, 0x4951, 0x4C54, 0x88, 0xFE, 0xAB, 0xD2, 0x5C, 0x15, 0xB3, 0xD6);
    DEFINE_GUID(DXVA2_ModeVC1_D, 0x1b81beA3, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
    DEFINE_GUID(DXVA2_ModeVC1_D2010, 0x1b81beA4, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
    DEFINE_GUID(DXVA2_ModeHEVC_VLD_Main, 0x5b11d51b, 0x2f4c, 0x4452, 0xbc, 0xc3, 0x09, 0xf2, 0xa1, 0x16, 0x0c, 0xc0);
    DEFINE_GUID(DXVA2_ModeVP9_VLD_Profile0, 0x463707f8, 0xa1d0, 0x4585, 0x87, 0x6d, 0x83, 0xaa, 0x6d, 0x60, 0xb8, 0x9e);
    DEFINE_GUID(DXVA2_NoEncrypt, 0x1b81beD0, 0xa0c7, 0x11d3, 0xb9, 0x84, 0x00, 0xc0, 0x4f, 0x2e, 0x73, 0xc5);
    DEFINE_GUID(GUID_NULL, 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

    typedef IDirect3D9* WINAPI pDirect3DCreate9(UINT);
    typedef HRESULT WINAPI pCreateDeviceManager9(UINT *, IDirect3DDeviceManager9 **);

    typedef struct dxva2_mode {
        const GUID     *guid;
        enum AVCodecID codec;
    } dxva2_mode;

    static const dxva2_mode dxva2_modes[] = {
        /* MPEG-2 */
        { &DXVA2_ModeMPEG2_VLD, AV_CODEC_ID_MPEG2VIDEO },
        { &DXVA2_ModeMPEG2and1_VLD, AV_CODEC_ID_MPEG2VIDEO },

        /* H.264 */
        { &DXVA2_ModeH264_F, AV_CODEC_ID_H264 },
        { &DXVA2_ModeH264_E, AV_CODEC_ID_H264 },
        /* Intel specific H.264 mode */
        { &DXVADDI_Intel_ModeH264_E, AV_CODEC_ID_H264 },

        /* VC-1 / WMV3 */
        { &DXVA2_ModeVC1_D2010, AV_CODEC_ID_VC1 },
        { &DXVA2_ModeVC1_D2010, AV_CODEC_ID_WMV3 },
        { &DXVA2_ModeVC1_D, AV_CODEC_ID_VC1 },
        { &DXVA2_ModeVC1_D, AV_CODEC_ID_WMV3 },

        /* HEVC/H.265 */
        { &DXVA2_ModeHEVC_VLD_Main, AV_CODEC_ID_HEVC },

        /* VP8/9 */
        { &DXVA2_ModeVP9_VLD_Profile0, AV_CODEC_ID_VP9 },

        { NULL, AV_CODEC_ID_NONE },
    };



    typedef struct DXVA2SurfaceWrapper {
        DXVA2Context         *ctx;
        LPDIRECT3DSURFACE9   surface;
        IDirectXVideoDecoder *decoder;
    } DXVA2SurfaceWrapper;

    static void dxva2_destroy_decoder(AVCodecContext *s)
    {
        InputStream  *ist = (InputStream *)s->opaque;
        DXVA2Context *ctx = (DXVA2Context *)ist->hwaccel_ctx;
        int i;

        if (ctx->surfaces) {
            for (i = 0; i < ctx->num_surfaces; i++) {
                if (ctx->surfaces[i])
                    IDirect3DSurface9_Release(ctx->surfaces[i]);
            }
        }
        av_freep(&ctx->surfaces);
        av_freep(&ctx->surface_infos);
        ctx->num_surfaces = 0;
        ctx->surface_age = 0;

		if(ctx->m_pSharedSurface)
			IDirect3DSurface9_Release(ctx->m_pSharedSurface);



        if (ctx->decoder) {
            //IDirectXVideoDecoder_Release(ctx->decoder);
            ctx->decoder->Release();
            ctx->decoder = NULL;
        }
    }

    void dxva2_uninit(AVCodecContext *s)
    {
        InputStream  *ist = (InputStream  *)s->opaque;
        DXVA2Context *ctx = (DXVA2Context *)ist->hwaccel_ctx;

        ist->hwaccel_uninit = NULL;
        ist->hwaccel_get_buffer = NULL;
        ist->hwaccel_retrieve_data = NULL;

        if (ctx->decoder)
            dxva2_destroy_decoder(s);

        if (ctx->decoder_service)
            ctx->decoder_service->Release();

		av_freep(&ctx->tmp_frame->data[0]);
        av_frame_free(&ctx->tmp_frame);

        av_freep(&ist->hwaccel_ctx);
        av_freep(&s->hwaccel_context);
    }

    static void dxva2_release_buffer(void *opaque, uint8_t *data)
    {
        DXVA2SurfaceWrapper *w = (DXVA2SurfaceWrapper *)opaque;
        DXVA2Context        *ctx = w->ctx;
        int i;

        for (i = 0; i < ctx->num_surfaces; i++) {
            if (ctx->surfaces[i] == w->surface) {
                ctx->surface_infos[i].used = 0;
                break;
            }
        }
        IDirect3DSurface9_Release(w->surface);
        //IDirectXVideoDecoder_Release(w->decoder);
        w->decoder->Release();
        av_free(w);
    }

    static int dxva2_get_buffer(AVCodecContext *s, AVFrame *frame, int flags)
    {
        InputStream  *ist = (InputStream  *)s->opaque;
        DXVA2Context *ctx = (DXVA2Context *)ist->hwaccel_ctx;
        int i, old_unused = -1;
        LPDIRECT3DSURFACE9 surface;
        DXVA2SurfaceWrapper *w = NULL;

        av_assert0(frame->format == AV_PIX_FMT_DXVA2_VLD);

        for (i = 0; i < ctx->num_surfaces; i++) {
            surface_info *info = &ctx->surface_infos[i];
            if (!info->used && (old_unused == -1 || info->age < ctx->surface_infos[old_unused].age))
                old_unused = i;
        }
        if (old_unused == -1) {
            av_log(NULL, AV_LOG_ERROR, "No free DXVA2 surface!\n");
            return AVERROR(ENOMEM);
        }
        i = old_unused;

        surface = ctx->surfaces[i];

        w = (DXVA2SurfaceWrapper *)av_mallocz(sizeof(*w));
        if (!w)
            return AVERROR(ENOMEM);

        frame->buf[0] = av_buffer_create((uint8_t*)surface, 0,
            dxva2_release_buffer, w,
            AV_BUFFER_FLAG_READONLY);
        if (!frame->buf[0]) {
            av_free(w);
            return AVERROR(ENOMEM);
        }

        w->ctx = ctx;
        w->surface = surface;
        IDirect3DSurface9_AddRef(w->surface);
        w->decoder = ctx->decoder;
        //IDirectXVideoDecoder_AddRef(w->decoder);
        w->decoder->AddRef();

        ctx->surface_infos[i].used = 1;
        ctx->surface_infos[i].age = ctx->surface_age++;

        frame->data[3] = (uint8_t *)surface;

        return 0;
    }


static inline void av_image_copy_plane(uint8_t       *dst, int dst_linesize,
                            const uint8_t *src, int src_linesize,
                            int bytewidth, int height)
   {
       if (!dst || !src)
           return;
       av_assert0(abs(src_linesize) >= bytewidth);
       av_assert0(abs(dst_linesize) >= bytewidth);

#pragma omp parallel for
	for (auto y = 0; y < height; y++)
	{
           memcpy(dst + (dst_linesize * y), src + (src_linesize * y), bytewidth);
          // dst += dst_linesize;
          // src += src_linesize;
       }
 }

    static int dxva2_retrieve_data(AVCodecContext *s, AVFrame *frame)
    {
        LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)frame->data[3];
        InputStream        *ist = (InputStream *)s->opaque;
        DXVA2Context       *ctx = (DXVA2Context *)ist->hwaccel_ctx;
        D3DSURFACE_DESC    surfaceDesc;
        D3DLOCKED_RECT     LockedRect;
        HRESULT            hr;
        int                ret;

        IDirect3DSurface9_GetDesc(surface, &surfaceDesc);

        ctx->tmp_frame->width = frame->width;
        ctx->tmp_frame->height = frame->height;
        ctx->tmp_frame->format = AV_PIX_FMT_NV12;

        ret = av_frame_get_buffer(ctx->tmp_frame, 32);
        if (ret < 0)
            return ret;

        hr = IDirect3DSurface9_LockRect(surface, &LockedRect, NULL, D3DLOCK_READONLY);
        if (FAILED(hr)) {
            av_log(NULL, AV_LOG_ERROR, "Unable to lock DXVA2 surface\n");
            return AVERROR_UNKNOWN;
        }

		
	
        av_image_copy_plane(ctx->tmp_frame->data[0], ctx->tmp_frame->linesize[0],
            (uint8_t*)LockedRect.pBits,
            LockedRect.Pitch, frame->width, frame->height);
		
        av_image_copy_plane(ctx->tmp_frame->data[1], ctx->tmp_frame->linesize[1],
            (uint8_t*)LockedRect.pBits + LockedRect.Pitch * surfaceDesc.Height,
            LockedRect.Pitch, frame->width, frame->height / 2);

        IDirect3DSurface9_UnlockRect(surface);

        ret = av_frame_copy_props(ctx->tmp_frame, frame);
        if (ret < 0)
            goto fail;

        av_frame_unref(frame);
        av_frame_move_ref(frame, ctx->tmp_frame);

        return 0;
    fail:
        av_frame_unref(ctx->tmp_frame);
        return ret;
    }

    static int dxva2_alloc(AVCodecContext *s, CVideoControlInterface * video, int width, int height)
    {
        InputStream *ist = (InputStream *)s->opaque;
        int loglevel = (ist->hwaccel_id == HWACCEL_AUTO) ? AV_LOG_VERBOSE : AV_LOG_ERROR;
        DXVA2Context *ctx;
        HRESULT hr;
		unsigned resetToken = 0;
        ctx = (DXVA2Context *)av_mallocz(sizeof(*ctx));
        if (!ctx)
            return AVERROR(ENOMEM);

        ist->hwaccel_ctx = ctx;
        ist->hwaccel_uninit = dxva2_uninit;
        ist->hwaccel_get_buffer = dxva2_get_buffer;
        ist->hwaccel_retrieve_data = dxva2_retrieve_data;

		video->InitVideoDevice(ist->hwaccel_device, ctx, width, height);

        ctx->tmp_frame = av_frame_alloc();
        if (!ctx->tmp_frame)
            goto fail;

        s->hwaccel_context = av_mallocz(sizeof(struct dxva_context));
        if (!s->hwaccel_context)
            goto fail;

        return 0;
    fail:
        dxva2_uninit(s);
        return AVERROR(EINVAL);
    }

    static int dxva2_get_decoder_configuration(AVCodecContext *s, const GUID *device_guid,
        const DXVA2_VideoDesc *desc,
        DXVA2_ConfigPictureDecode *config)
    {
        InputStream  *ist = (InputStream *)s->opaque;
        int loglevel = (ist->hwaccel_id == HWACCEL_AUTO) ? AV_LOG_VERBOSE : AV_LOG_ERROR;
        DXVA2Context *ctx = (DXVA2Context *)ist->hwaccel_ctx;
        unsigned cfg_count = 0, best_score = 0;
        DXVA2_ConfigPictureDecode *cfg_list = NULL;
        DXVA2_ConfigPictureDecode best_cfg = { { 0 } };
        HRESULT hr;
        int i;

        //hr = IDirectXVideoDecoderService_GetDecoderConfigurations(ctx->decoder_service, device_guid, desc, NULL, &cfg_count, &cfg_list);
        hr = ctx->decoder_service->GetDecoderConfigurations(*device_guid, desc, NULL, &cfg_count, &cfg_list);
        if (FAILED(hr)) {
            av_log(NULL, loglevel, "Unable to retrieve decoder configurations\n");
            return AVERROR(EINVAL);
        }

        for (i = 0; i < cfg_count; i++) {
            DXVA2_ConfigPictureDecode *cfg = &cfg_list[i];

            unsigned score;
            if (cfg->ConfigBitstreamRaw == 1)
                score = 1;
            else if (s->codec_id == AV_CODEC_ID_H264 && cfg->ConfigBitstreamRaw == 2)
                score = 2;
            else
                continue;
            if (IsEqualGUID(cfg->guidConfigBitstreamEncryption, DXVA2_NoEncrypt))
                score += 16;
            if (score > best_score) {
                best_score = score;
                best_cfg = *cfg;
            }
        }
        CoTaskMemFree(cfg_list);

        if (!best_score) {
            av_log(NULL, loglevel, "No valid decoder configuration available\n");
            return AVERROR(EINVAL);
        }

        *config = best_cfg;
        return 0;
    }

    static int dxva2_create_decoder(AVCodecContext *s)
    {
        InputStream  *ist = (InputStream *)s->opaque;
        int loglevel = (ist->hwaccel_id == HWACCEL_AUTO) ? AV_LOG_VERBOSE : AV_LOG_ERROR;
        DXVA2Context *ctx = (DXVA2Context *)ist->hwaccel_ctx;
        struct dxva_context *dxva_ctx = (dxva_context *)s->hwaccel_context;
        GUID *guid_list = NULL;
        unsigned guid_count = 0, i, j;
        GUID device_guid = GUID_NULL;
        D3DFORMAT target_format = D3DFMT_UNKNOWN;
        DXVA2_VideoDesc desc = { 0 };
        DXVA2_ConfigPictureDecode config;
        HRESULT hr;
        int surface_alignment;
        int ret;

        //hr = IDirectXVideoDecoderService_GetDecoderDeviceGuids(ctx->decoder_service, &guid_count, &guid_list);
        hr = ctx->decoder_service->GetDecoderDeviceGuids(&guid_count, &guid_list);
        if (FAILED(hr)) {
            av_log(NULL, loglevel, "Failed to retrieve decoder device GUIDs\n");
            goto fail;
        }

        for (i = 0; dxva2_modes[i].guid; i++) {
            D3DFORMAT *target_list = NULL;
            unsigned target_count = 0;
            const dxva2_mode *mode = &dxva2_modes[i];
            if (mode->codec != s->codec_id)
                continue;

            for (j = 0; j < guid_count; j++) {
                if (IsEqualGUID(*mode->guid, guid_list[j]))
                    break;
            }
            if (j == guid_count)
                continue;

            //hr = IDirectXVideoDecoderService_GetDecoderRenderTargets(ctx->decoder_service, mode->guid, &target_count, &target_list);
            hr = ctx->decoder_service->GetDecoderRenderTargets(*mode->guid, &target_count, &target_list);
            if (FAILED(hr)) {
                continue;
            }
            for (j = 0; j < target_count; j++) {
                const D3DFORMAT format = target_list[j];
                if (format == MKTAG('N', 'V', '1', '2')) {
                    target_format = format;
                    break;
                }
            }
            CoTaskMemFree(target_list);
            if (target_format) {
                device_guid = *mode->guid;
                break;
            }
        }
        CoTaskMemFree(guid_list);

        if (IsEqualGUID(device_guid, GUID_NULL)) {
            av_log(NULL, loglevel, "No decoder device for codec found\n");
            goto fail;
        }

        desc.SampleWidth = s->coded_width;
        desc.SampleHeight = s->coded_height;
        desc.Format = target_format;

        ret = dxva2_get_decoder_configuration(s, &device_guid, &desc, &config);
        if (ret < 0) {
            goto fail;
        }

        /* decoding MPEG-2 requires additional alignment on some Intel GPUs,
        but it causes issues for H.264 on certain AMD GPUs..... */
        if (s->codec_id == AV_CODEC_ID_MPEG2VIDEO)
            surface_alignment = 32;
        /* the HEVC DXVA2 spec asks for 128 pixel aligned surfaces to ensure
        all coding features have enough room to work with */
        else if (s->codec_id == AV_CODEC_ID_HEVC)
            surface_alignment = 128;
        else
            surface_alignment = 16;

        /* 4 base work surfaces */
        ctx->num_surfaces = 4;

        /* add surfaces based on number of possible refs */
        if (s->codec_id == AV_CODEC_ID_H264 || s->codec_id == AV_CODEC_ID_HEVC)
            ctx->num_surfaces += 16;
        else if (s->codec_id == AV_CODEC_ID_VP9)
            ctx->num_surfaces += 8;
        else
            ctx->num_surfaces += 2;

        /* add extra surfaces for frame threading */
        if (s->active_thread_type & FF_THREAD_FRAME)
            ctx->num_surfaces += s->thread_count;

        ctx->surfaces = (LPDIRECT3DSURFACE9 *)av_mallocz(ctx->num_surfaces * sizeof(*ctx->surfaces));
        ctx->surface_infos = (surface_info *)av_mallocz(ctx->num_surfaces * sizeof(*ctx->surface_infos));

        if (!ctx->surfaces || !ctx->surface_infos) {
            av_log(NULL, loglevel, "Unable to allocate surface arrays\n");
            goto fail;
        }

        /*hr = IDirectXVideoDecoderService_CreateSurface(ctx->decoder_service,
            FFALIGN(s->coded_width, surface_alignment),
            FFALIGN(s->coded_height, surface_alignment),
            ctx->num_surfaces - 1,
            target_format, D3DPOOL_DEFAULT, 0,
            DXVA2_VideoDecoderRenderTarget,
            ctx->surfaces, NULL);*/
        hr = ctx->decoder_service->CreateSurface(FFALIGN(s->coded_width, surface_alignment),
            FFALIGN(s->coded_height, surface_alignment),
            ctx->num_surfaces - 1,
            target_format, D3DPOOL_DEFAULT, 0,
            DXVA2_VideoDecoderRenderTarget,
            ctx->surfaces, NULL);
        if (FAILED(hr)) {
            av_log(NULL, loglevel, "Failed to create %d video surfaces\n", ctx->num_surfaces);
            goto fail;
        }

        /*hr = IDirectXVideoDecoderService_CreateVideoDecoder(ctx->decoder_service, &device_guid,
            &desc, &config, ctx->surfaces,
            ctx->num_surfaces, &ctx->decoder);*/
        hr = ctx->decoder_service->CreateVideoDecoder(device_guid,
            &desc, &config, ctx->surfaces,
            ctx->num_surfaces, &ctx->decoder);
        if (FAILED(hr)) {
            av_log(NULL, loglevel, "Failed to create DXVA2 video decoder\n");
            goto fail;
        }

        ctx->decoder_guid = device_guid;
        ctx->decoder_config = config;

        dxva_ctx->cfg = &ctx->decoder_config;
        dxva_ctx->decoder = ctx->decoder;
        dxva_ctx->surface = ctx->surfaces;
        dxva_ctx->surface_count = ctx->num_surfaces;

        if (IsEqualGUID(ctx->decoder_guid, DXVADDI_Intel_ModeH264_E))
            dxva_ctx->workaround |= FF_DXVA2_WORKAROUND_INTEL_CLEARVIDEO;

        return 0;
    fail:
        dxva2_destroy_decoder(s);
        return AVERROR(EINVAL);
    }

    int dxva2_init(AVCodecContext *s, CVideoControlInterface * video, int width, int height)
    {
        InputStream *ist = (InputStream *)s->opaque;
        int loglevel = (ist->hwaccel_id == HWACCEL_AUTO) ? AV_LOG_VERBOSE : AV_LOG_ERROR;
        DXVA2Context *ctx;
        int ret;

        if (!ist->hwaccel_ctx) {
            ret = dxva2_alloc(s, video, width, height);
            if (ret < 0)
                return ret;
        }
        ctx = (DXVA2Context *)ist->hwaccel_ctx;

        if (s->codec_id == AV_CODEC_ID_H264 &&
            (s->profile & ~FF_PROFILE_H264_CONSTRAINED) > FF_PROFILE_H264_HIGH) {
            av_log(NULL, loglevel, "Unsupported H.264 profile for DXVA2 HWAccel: %d\n", s->profile);
            return AVERROR(EINVAL);
        }

        if (ctx->decoder)
            dxva2_destroy_decoder(s);

        ret = dxva2_create_decoder(s);
        if (ret < 0) {
            av_log(NULL, loglevel, "Error creating the DXVA2 decoder\n");
            return ret;
        }

        return 0;
    }

#endif