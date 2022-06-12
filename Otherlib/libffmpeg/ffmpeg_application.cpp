#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"


#include <CL/cl_d3d11.h>

extern "C" {
#include "ffmpeg.h"
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
#include <libavutil/hwcontext.h>
#include <libavutil/opt.h>
#include <libavutil/avassert.h>
#include <libavutil/imgutils.h>
#include <libavutil/hwcontext_d3d11va.h>
}

#include <opencv2/core/directx.hpp>
using namespace cv;



/*
 * Copyright (c) 2017 Jun Zhao
 * Copyright (c) 2017 Kaixuan Liu
 *
 * HW Acceleration API (video decoding) decode sample
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

 /**
  * @file
  * HW-Accelerated decoding example.
  *
  * @example hw_decode.c
  * This example shows how to do HW-accelerated decoding with output
  * frames from the HW video surfaces.
  */

#include <stdio.h>


  // FFmpeg context attached to OpenCL context
class OpenCL_FFMPEG_Context : public ocl::Context::UserContext {
public:
	OpenCL_FFMPEG_Context(AVBufferRef* ctx) {
		ctx_ = av_buffer_ref(ctx);
	}
	virtual ~OpenCL_FFMPEG_Context() {
		av_buffer_unref(&ctx_);
	}
	AVBufferRef* GetAVHWDevice() {
		return ctx_;
	}
private:
	AVBufferRef* ctx_;
};


static ID3D11Texture2D* texture_D3D11;
extern "C"
{



	static bool use_opencl = true;
	static bool is_d3dva = true;
	static AVBufferRef* hw_device_ctx = NULL;
	static enum AVPixelFormat hw_pix_fmt;
	static FILE* output_file = NULL;



	static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
		const enum AVPixelFormat* pix_fmts)
	{
		const enum AVPixelFormat* p;

		for (p = pix_fmts; *p != -1; p++) {
			if (*p == hw_pix_fmt)
				return *p;
		}

		fprintf(stderr, "Failed to get HW surface format.\n");
		return AV_PIX_FMT_NONE;
	}

	static ID3D11Texture2D* hw_get_d3d11_texture(AVFrame* hw_frame, int* subresource) {
		ID3D11Texture2D* texture = NULL;
		if (AV_PIX_FMT_D3D11 == hw_frame->format) {
			texture = (ID3D11Texture2D*)hw_frame->data[0]; // As defined by AV_PIX_FMT_D3D11
			*subresource = (intptr_t)hw_frame->data[1]; // As defined by AV_PIX_FMT_D3D11
		}
		return texture;
	}


	// In D3D11 case we allocate additional texture as single texture (not texture array) because
	// OpenCL interop with D3D11 doesn't support/work with NV12 sub-texture of texture array.
	static ID3D11Texture2D* hw_get_d3d11_single_texture(AVFrame* hw_frame, AVD3D11VADeviceContext* d3d11_device_ctx, ID3D11Texture2D* texture) {
		AVHWFramesContext* frames_ctx = (AVHWFramesContext*)hw_frame->hw_frames_ctx->data;
		if (AV_HWDEVICE_TYPE_QSV == frames_ctx->device_ctx->type) {
			frames_ctx = (AVHWFramesContext*)frames_ctx->user_opaque; // we stored pointer to child context in 'user_opaque' field
		}
		if (!frames_ctx || AV_HWDEVICE_TYPE_D3D11VA != frames_ctx->device_ctx->type) {
			return NULL;
		}
		ID3D11Texture2D* singleTexture = (ID3D11Texture2D*)frames_ctx->user_opaque;
		if (!singleTexture && d3d11_device_ctx && texture) {
			D3D11_TEXTURE2D_DESC desc = {};
			texture->GetDesc(&desc);
			desc.ArraySize = 1;
			desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;
			if (SUCCEEDED(d3d11_device_ctx->device->CreateTexture2D(&desc, NULL, &singleTexture))) {
				frames_ctx->user_opaque = singleTexture;
			}
		}
		return singleTexture;
	}

	static
		AVD3D11VADeviceContext* hw_get_d3d11_device_ctx(AVHWDeviceContext* hw_device_ctx) {
		if (AV_HWDEVICE_TYPE_D3D11VA == hw_device_ctx->type) {
			return (AVD3D11VADeviceContext*)hw_device_ctx->hwctx;
		}
		return NULL;
	}

	static AVHWDeviceType hw_check_opencl_context(AVHWDeviceContext* ctx)
	{
		ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrentRef();
		if (!ctx || ocl_context.empty())
			return AV_HWDEVICE_TYPE_NONE;

		ID3D11Device* d3d11device_ocl = (ID3D11Device*)ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_D3D11_DEVICE_KHR);
		AVD3D11VADeviceContext* d3d11_device_ctx = hw_get_d3d11_device_ctx(ctx);
		if (d3d11_device_ctx && d3d11device_ocl && d3d11_device_ctx->device == d3d11device_ocl)
			return AV_HWDEVICE_TYPE_D3D11VA;

		return AV_HWDEVICE_TYPE_NONE;
	}


	static void hw_init_opencl(AVBufferRef* ctx) {
		if (!ctx)
			return;
		AVHWDeviceContext* hw_device_ctx = (AVHWDeviceContext*)ctx->data;
		if (!hw_device_ctx)
			return;

		AVD3D11VADeviceContext* d3d11_device_ctx = hw_get_d3d11_device_ctx(hw_device_ctx);
		if (d3d11_device_ctx) {
			directx::ocl::initializeContextFromD3D11Device(d3d11_device_ctx->device);
		}


		if (hw_check_opencl_context(hw_device_ctx) != AV_HWDEVICE_TYPE_NONE) {
			// Attach AVHWDeviceContext to OpenCL context
			ocl::Context& ocl_context = ocl::OpenCLExecutionContext::getCurrent().getContext();
			ocl_context.setUserContext(std::make_shared<OpenCL_FFMPEG_Context>(ctx));
		}
		
	}

	// GPU color conversion NV12->BGRA via OpenCL extensions
	static bool hw_copy_frame_to_umat(AVBufferRef* ctx, AVFrame* hw_frame, cv::OutputArray output) {
		CV_UNUSED(hw_frame);
		CV_UNUSED(output);
		if (!ctx)
			return false;

		try {
			// check that current OpenCL context initilized with binding to same VAAPI/D3D11 context
			AVHWDeviceContext* hw_device_ctx = (AVHWDeviceContext*)ctx->data;
			AVHWDeviceType child_type = hw_check_opencl_context(hw_device_ctx);
			if (child_type == AV_HWDEVICE_TYPE_NONE)
				return false;



			if (child_type == AV_HWDEVICE_TYPE_D3D11VA) {
				AVD3D11VADeviceContext* d3d11_device_ctx = hw_get_d3d11_device_ctx(hw_device_ctx);
				int subresource = 0;
				ID3D11Texture2D* texture = hw_get_d3d11_texture(hw_frame, &subresource);
				ID3D11Texture2D* singleTexture = hw_get_d3d11_single_texture(hw_frame, d3d11_device_ctx, texture);
				if (texture && singleTexture) {
					// Copy D3D11 sub-texture to D3D11 single texture
					d3d11_device_ctx->device_context->CopySubresourceRegion(singleTexture, 0, 0, 0, 0, texture, subresource, NULL);
					// Copy D3D11 single texture to cv::UMat
					directx::convertFromD3D11Texture2D(singleTexture, output);
					return true;
				}
			}

		}
		catch (...)
		{
			return false;
		}


		return false;
	}



	static int hw_decoder_init(AVCodecContext* ctx, const enum AVHWDeviceType type)
	{
		int err = 0;
		ocl::OpenCLExecutionContext ocl_context;// = ocl::OpenCLExecutionContext::getCurrentRef();

		if ((err = av_hwdevice_ctx_create(&hw_device_ctx, type,
			NULL, NULL, 0)) < 0) {
			fprintf(stderr, "Failed to create specified HW device.\n");
			return err;
		}
		ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);

		// if OpenCL context not created yet or property HW_ACCELERATION_USE_OPENCL set, create OpenCL context with binding to video acceleration context
		if (ocl::haveOpenCL()) {
			if (ocl_context.empty() || use_opencl) {
				try {
					hw_init_opencl(hw_device_ctx);
					ocl_context = ocl::OpenCLExecutionContext::getCurrentRef();
					if (!ocl_context.empty()) {
						//CV_LOG_INFO(NULL, "FFMPEG: Created OpenCL context with " << hw_child_name <<
					//		" video acceleration on OpenCL device: " << ocl_context.getDevice().name());
					}
				}
				catch (...) {
					//CV_LOG_INFO(NULL, "FFMPEG: Exception creating OpenCL context with " << hw_child_name << " video acceleration");
				}
			}
			else {
				//CV_LOG_INFO(NULL, "FFMPEG: Can't bind " << hw_child_name << " video acceleration context to already created OpenCL context");
			}
		}

		return err;
	}

	static int decode_write(AVCodecContext* avctx, AVPacket* packet, AVD3D11VADeviceContext* hw_d3d11_dev_ctx, bool & firstFrame)
	{
		AVFrame* frame = NULL, * sw_frame = NULL;
		AVFrame* tmp_frame = NULL;
		uint8_t* buffer = NULL;
		int size;
		int ret = 0;

		ret = avcodec_send_packet(avctx, packet);
		if (ret < 0) {
			fprintf(stderr, "Error during decoding\n");
			return ret;
		}

		while (1) {
			if (!(frame = av_frame_alloc()) || !(sw_frame = av_frame_alloc())) {
				fprintf(stderr, "Can not alloc frame\n");
				ret = AVERROR(ENOMEM);
				goto fail;
			}

			ret = avcodec_receive_frame(avctx, frame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				av_frame_free(&frame);
				av_frame_free(&sw_frame);
				return 0;
			}
			else if (ret < 0) {
				fprintf(stderr, "Error while decoding\n");
				goto fail;
			}

			/*
			if (firstFrame)
			{
				// create intermediate texture
				D3D11_TEXTURE2D_DESC pDes{};
				ZeroMemory(&pDes, sizeof(pDes));
				pDes.Width = frame->width;
				pDes.Height = frame->height;
				pDes.ArraySize = 1;
				pDes.MipLevels = 1;
				pDes.Format = DXGI_FORMAT_NV12;
				pDes.SampleDesc.Count = 1;
				pDes.SampleDesc.Quality = 0;
				pDes.Usage = D3D11_USAGE_DEFAULT;
				pDes.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
				pDes.CPUAccessFlags = 0;
				pDes.MiscFlags = 0;

				
				hw_d3d11_dev_ctx->device->CreateTexture2D(&pDes, NULL, &texture_D3D11);

				// connect opengl and directx textures
				//HANDLE hRegisterObject = wglDXRegisterObjectNV(
				//	device_handle, texture_D3D11, texture_opengl, GL_TEXTURE_2D, WGL_ACCESS_READ_WRITE_NV
				//);

				firstFrame = false;
			}
			*/

			if (frame->format == hw_pix_fmt)
			{
				bool error = true;

				if (is_d3dva)
				{
					cv::UMat output;
					error = hw_copy_frame_to_umat(hw_device_ctx, frame, output);
					/*
					cv::Mat output;
					int subresource = 0;
					ID3D11Texture2D* texture = hw_get_d3d11_texture(frame, &subresource);
					ID3D11Texture2D* singleTexture = hw_get_d3d11_single_texture(frame, hw_d3d11_dev_ctx, texture);
					if (texture && singleTexture) {
						// Copy D3D11 sub-texture to D3D11 single texture
						hw_d3d11_dev_ctx->device_context->CopySubresourceRegion(singleTexture, 0, 0, 0, 0, texture, subresource, NULL);
						// Copy D3D11 single texture to cv::UMat
						cv::directx::convertFromD3D11Texture2D(singleTexture, output);
						return true;
					}
					*/
				}
				if(error)
				{
					/* retrieve data from GPU to CPU */
					if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
						fprintf(stderr, "Error transferring the data to system memory\n");
						goto fail;
					}
					tmp_frame = sw_frame;
				}
			}
			else
				tmp_frame = frame;

			size = av_image_get_buffer_size((AVPixelFormat)tmp_frame->format, tmp_frame->width, tmp_frame->height, 1);
			buffer = (uint8_t *)av_malloc(size);
			if (!buffer) {
				fprintf(stderr, "Can not alloc buffer\n");
				ret = AVERROR(ENOMEM);
				goto fail;
			}
			ret = av_image_copy_to_buffer(buffer, size,
				(const uint8_t* const*)tmp_frame->data,
				(const int*)tmp_frame->linesize, (AVPixelFormat)tmp_frame->format,
				tmp_frame->width, tmp_frame->height, 1);
			if (ret < 0) {
				fprintf(stderr, "Can not copy image to buffer\n");
				goto fail;
			}

			if ((ret = fwrite(buffer, 1, size, output_file)) < 0) {
				fprintf(stderr, "Failed to dump raw data.\n");
				goto fail;
			}

		fail:
			av_frame_free(&frame);
			av_frame_free(&sw_frame);
			av_freep(&buffer);
			if (ret < 0)
				return ret;
		}
	}
}




int TestHWDecode(std::vector<std::string> & arrayOfStrings)
{
	AVFormatContext* input_ctx = NULL;
	int video_stream, ret;
	AVStream* video = NULL;
	AVCodecContext* decoder_ctx = NULL;
	AVCodec* decoder = NULL;
	AVPacket* packet = NULL;
	enum AVHWDeviceType type;

	int i;
	bool firstFrame = true;

	if (arrayOfStrings.size() < 4) {
		fprintf(stderr, "Usage: %s <device type> <input file> <output file>\n", arrayOfStrings[0].c_str());
		return -1;
	}

	type = av_hwdevice_find_type_by_name(arrayOfStrings[1].c_str());
	if (type == AV_HWDEVICE_TYPE_NONE) {
		fprintf(stderr, "Device type %s is not supported.\n", arrayOfStrings[1].c_str());
		fprintf(stderr, "Available device types:");
		while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
			fprintf(stderr, " %s", av_hwdevice_get_type_name(type));
		fprintf(stderr, "\n");
		return -1;
	}

	packet = av_packet_alloc();
	if (!packet) {
		fprintf(stderr, "Failed to allocate AVPacket\n");
		return -1;
	}

	/* open the input file */
	if (avformat_open_input(&input_ctx, arrayOfStrings[2].c_str(), NULL, NULL) != 0) {
		fprintf(stderr, "Cannot open input file '%s'\n", arrayOfStrings[2].c_str());
		return -1;
	}

	if (avformat_find_stream_info(input_ctx, NULL) < 0) {
		fprintf(stderr, "Cannot find input stream information.\n");
		return -1;
	}

	/* find the video stream information */
	ret = av_find_best_stream(input_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
	if (ret < 0) {
		fprintf(stderr, "Cannot find a video stream in the input file\n");
		return -1;
	}
	video_stream = ret;

	for (i = 0;; i++) {
		const AVCodecHWConfig* config = avcodec_get_hw_config(decoder, i);
		if (!config) {
			fprintf(stderr, "Decoder %s does not support device type %s.\n",
				decoder->name, av_hwdevice_get_type_name(type));
			return -1;
		}
		if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
			config->device_type == type) {
			hw_pix_fmt = config->pix_fmt;
			break;
		}
	}

	if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
		return AVERROR(ENOMEM);

	video = input_ctx->streams[video_stream];
	if (avcodec_parameters_to_context(decoder_ctx, video->codecpar) < 0)
		return -1;

	decoder_ctx->get_format = get_hw_format;

	if (hw_decoder_init(decoder_ctx, type) < 0)
		return -1;

	if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0) {
		fprintf(stderr, "Failed to open codec for stream #%u\n", video_stream);
		return -1;
	}

	/* open the file to dump raw data */
	output_file = fopen(arrayOfStrings[3].c_str(), "w+b");

	// setup WGL_NV_DX_interop extension
	AVBufferRef* hw_device_ctx_buffer = decoder_ctx->hw_device_ctx;
	AVHWDeviceContext* hw_d3d11_device_ctx = (AVHWDeviceContext*)hw_device_ctx_buffer->data;
	AVD3D11VADeviceContext* hw_d3d11_dev_ctx = (AVD3D11VADeviceContext*)hw_d3d11_device_ctx->hwctx;
	//HANDLE device_handle = wglDXOpenDeviceNV(hw_d3d11_dev_ctx->device);


	/* actual decoding and dump the raw data */
	while (ret >= 0) {
		if ((ret = av_read_frame(input_ctx, packet)) < 0)
			break;

		if (video_stream == packet->stream_index)
			ret = decode_write(decoder_ctx, packet, hw_d3d11_dev_ctx, firstFrame);

		av_packet_unref(packet);
	}

	/* flush the decoder */
	ret = decode_write(decoder_ctx, NULL, hw_d3d11_dev_ctx, firstFrame);

	if (output_file)
		fclose(output_file);
	av_packet_free(&packet);
	avcodec_free_context(&decoder_ctx);
	avformat_close_input(&input_ctx);
	av_buffer_unref(&hw_device_ctx);

	return 0;
}

/*
std::string convertSecondsToHHMMSS(int value)
{
	std::string result;
	// compute h, m, s
	std::string h = std::to_string(value / 3600);
	std::string m = std::to_string((value % 3600) / 60);
	std::string s = std::to_string(value % 60);
	// add leading zero if needed
	std::string hh = std::string(2 - h.length(), '0') + h;
	std::string mm = std::string(2 - m.length(), '0') + m;
	std::string ss = std::string(2 - s.length(), '0') + s;
	// return mm:ss if hh is 00
	if (hh.compare("00") != 0)
	{
		result = hh + ':' + mm + ":" + ss;
	}
	else
	{
		result = mm + ":" + ss;
	}
	return result;
}
*/
void CFFmpegApp::ExitFunction(int x)
{
	throw x;
}

int CFFmpegApp::ProgressBarFunction(int x, void* progressWnd)
{
	if (progressWnd != nullptr)
	{
		auto dialog = static_cast<wxProgressDialog*>(progressWnd);
		if (false == dialog->Update(x, "Video Progress ..."))
			return 1;
	}

	return 0;
}

void CFFmpegApp::ExecuteFFmpeg()
{
	arrayOfCstrings = new char*[arrayOfStrings.size()];

	for (int i = 0; i < arrayOfStrings.size(); ++i)
		arrayOfCstrings[i] = (char*)arrayOfStrings[i].c_str();


	wxProgressDialog* dialog = nullptr;

	if (showProgressWindow)
	{
		dialog = new wxProgressDialog("FFmpeg Process", "Checking...", 100, nullptr,
			wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	}

	try
	{
		void (*foo)(int);
		foo = &ExitFunction;

		int (*progress)(int, void*);
		progress = &ProgressBarFunction;

		ret = ExecuteFFMpegProgram(arrayOfStrings.size(), arrayOfCstrings, foo, progress, dialog);
	}
	catch (int e)
	{
		ret = e;
	}
	catch (...)
	{
	}

	if (dialog != nullptr)
		delete dialog;

	Cleanup(ret);
}

int CFFmpegApp::TestFFmpeg(const wxString& commandline)
{
	//ffmpeg -hwaccel_output_format cuda -i F:\music_video\test.mp4 -frames:v 1 -q:v 2 test.png
	/*
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-hwaccel_output_format");
	arrayOfStrings.push_back("cuda");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back("F:\\music_video\\test.mp4");
	arrayOfStrings.push_back("-frames:v");
	arrayOfStrings.push_back("1");
	arrayOfStrings.push_back("-q:v");
	arrayOfStrings.push_back("2");
	arrayOfStrings.push_back("d:\\test.png");
	*/
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("d3d11va");
	arrayOfStrings.push_back("d:\\video\\20200509_132206.mp4");
	arrayOfStrings.push_back("F:\\music_video\\20200509_132206.yuv");
	TestHWDecode(arrayOfStrings);

	return 0;
}

void CFFmpegApp::Cleanup(int ret)
{
	ffmpeg_cleanup(ret);
	delete[] arrayOfCstrings;
}


int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
                          const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-stream_loop");
	arrayOfStrings.push_back("-1");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back(extension.ToStdString());
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                 const wxString& timeVideo, const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("0:v:0");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("1:a:0");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back(timeVideoIn.ToStdString());
	arrayOfStrings.push_back("-to");
	arrayOfStrings.push_back(timeVideoOut.ToStdString());
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-an");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn, const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn.ToStdString());
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut.ToStdString());
	}
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}



int CFFmpegApp::ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile.ToStdString());
	arrayOfStrings.push_back("-c");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();

	return 0;
}

/*
int CFFmpegApp::ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile, const int& rotate)
{
	//ffmpeg -i 20200509_132328.mp4 -map_metadata 0 -metadata:s:v rotate=-90 -codec copy output.mp4
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-map_metadata");
	arrayOfStrings.push_back("0");
	arrayOfStrings.push_back("-metadata:s:v");
	arrayOfStrings.push_back("rotate=" + std::to_string(rotate));
	arrayOfStrings.push_back("-codec");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();
	return 0;
}
*/