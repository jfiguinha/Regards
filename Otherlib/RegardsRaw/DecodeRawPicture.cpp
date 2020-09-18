#include "DecodeRawPicture.h"
#include "libraw/libraw.h"
#include <DecodeRawParameter.h>
#include <header.h>
#include <ImageLoadingFormat.h>
#define SWAP(a,b) { a ^= b; a ^= (b ^= a); }
#include <ximage.h>

CDecodeRawPicture::CDecodeRawPicture(const string & fileName)
{
	rawProcessor = new LibRaw();
	result = 0;
	result = rawProcessor->open_file(fileName.c_str());
	if(result == LIBRAW_SUCCESS)
	{
		// step two: positioning libraw_internal_data.unpacker_data.data_offset
		result = rawProcessor->unpack();
	}
}


CDecodeRawPicture::~CDecodeRawPicture()
{
	if(rawProcessor != nullptr)
	{
		rawProcessor->recycle();
		delete rawProcessor;
	}
}


CImageLoadingFormat * CDecodeRawPicture::DecodePicture(CDecodeRawParameter * decodeRawParameter)
{


	rawProcessor->imgdata.params.bright = decodeRawParameter->bright;
	rawProcessor->imgdata.params.highlight = decodeRawParameter->highlight;
	rawProcessor->imgdata.params.threshold = decodeRawParameter->threshold;
	rawProcessor->imgdata.params.use_auto_wb = decodeRawParameter->use_auto_wb;
	rawProcessor->imgdata.params.use_camera_wb = decodeRawParameter->use_camera_wb;
	rawProcessor->imgdata.params.half_size  = decodeRawParameter->half_size;
	rawProcessor->imgdata.params.use_camera_matrix= decodeRawParameter->use_camera_matrix;
	if(decodeRawParameter->aberRedEnable)
		rawProcessor->imgdata.params.aber[0] = decodeRawParameter->aberRed;
	if(decodeRawParameter->aberGreenEnable)
		rawProcessor->imgdata.params.aber[2] = decodeRawParameter->aberGreen;

	rawProcessor->imgdata.params.user_mul[0] = decodeRawParameter->multiRed;
	rawProcessor->imgdata.params.user_mul[1] = decodeRawParameter->multiGreen;
	rawProcessor->imgdata.params.user_mul[2] = decodeRawParameter->multiBlue;
	rawProcessor->imgdata.params.user_mul[3] = decodeRawParameter->multiOther;

	rawProcessor->imgdata.params.user_flip = decodeRawParameter->flip;
	rawProcessor->imgdata.params.user_qual = decodeRawParameter->interpolation;
	
	rawProcessor->imgdata.params.user_black =decodeRawParameter->black;
	rawProcessor->imgdata.params.user_cblack[0] = decodeRawParameter->blackchannelRed;
	rawProcessor->imgdata.params.user_cblack[1] = decodeRawParameter->blackchannelGreen;
	rawProcessor->imgdata.params.user_cblack[2] = decodeRawParameter->blackchannelBlue;
	rawProcessor->imgdata.params.user_cblack[3] = decodeRawParameter->blackchannelOther;
	rawProcessor->imgdata.params.user_sat =decodeRawParameter->saturation;
	rawProcessor->imgdata.params.med_passes = decodeRawParameter->medPasses;
	rawProcessor->imgdata.params.no_auto_bright = decodeRawParameter->noautobright;
	rawProcessor->imgdata.params.auto_bright_thr = decodeRawParameter->autobright;
	rawProcessor->imgdata.params.adjust_maximum_thr = decodeRawParameter->adjust_maximum_thr;
	rawProcessor->imgdata.params.use_fuji_rotate = decodeRawParameter->use_fuji_rotate;
	rawProcessor->imgdata.params.green_matching = decodeRawParameter->green_matching;
	rawProcessor->imgdata.params.dcb_iterations = decodeRawParameter->dcb_iterations;
	rawProcessor->imgdata.params.dcb_enhance_fl = decodeRawParameter->dcb_enhance_fl;
	rawProcessor->imgdata.params.fbdd_noiserd = decodeRawParameter->fbdd_noiserd;
	/*
	rawProcessor->imgdata.params.eeci_refine = decodeRawParameter->eeci_refine;

	rawProcessor->imgdata.params.es_med_passes = decodeRawParameter->es_med_passes;
	rawProcessor->imgdata.params.ca_correc = decodeRawParameter->ca_correc;
	rawProcessor->imgdata.params.cared = decodeRawParameter->cared;
	rawProcessor->imgdata.params.cablue = decodeRawParameter->cablue;
	rawProcessor->imgdata.params.cfaline = decodeRawParameter->cfaline; 
	rawProcessor->imgdata.params.linenoise = decodeRawParameter->linenoise;
	rawProcessor->imgdata.params.cfa_clean = decodeRawParameter->cfa_clean; 
	rawProcessor->imgdata.params.lclean = decodeRawParameter->lclean;
	rawProcessor->imgdata.params.cclean = decodeRawParameter->cclean;
	rawProcessor->imgdata.params.cfa_green = decodeRawParameter->cfa_green; 
	rawProcessor->imgdata.params.green_thresh = decodeRawParameter->green_thresh;
	*/
	rawProcessor->imgdata.params.exp_correc = decodeRawParameter->exp_correc; 
	rawProcessor->imgdata.params.exp_shift = decodeRawParameter->exp_shift;
	rawProcessor->imgdata.params.exp_preser = decodeRawParameter->exp_preser;
	/*
	rawProcessor->imgdata.params.wf_debanding = decodeRawParameter->wf_debanding; 
	rawProcessor->imgdata.params.wf_deband_treshold[0] = decodeRawParameter->wf_deband_tresholdRed;
	rawProcessor->imgdata.params.wf_deband_treshold[1]  = decodeRawParameter->wf_deband_tresholdGreen;
	rawProcessor->imgdata.params.wf_deband_treshold[2]  = decodeRawParameter->wf_deband_tresholdBlue;
	rawProcessor->imgdata.params.wf_deband_treshold[3]  = decodeRawParameter->wf_deband_tresholdOther;
	*/
	rawProcessor->imgdata.params.use_rawspeed = 0;

	try
	{
		result = rawProcessor->dcraw_process();
	}
	catch(...)
	{

	}

	int width = 0;
	int height = 0;


	CImageLoadingFormat * imageLoadingFormat = nullptr;
	CxImage * image = new CxImage();
	if(result == 0)
	{
		imageLoadingFormat = new CImageLoadingFormat();
		int raw_color, raw_bitsize;
		rawProcessor->get_mem_image_format(&width, &height, &raw_color, &raw_bitsize);
		image->Create(width,height,raw_bitsize*raw_color);

		int iTaille = raw_color * (raw_bitsize/8);
		int stride = ((iTaille * width + iTaille) & ~iTaille);
		//rawProcessor->copy_mem_image(image->GetBits(), width * raw_color * (raw_bitsize/8), 1);
		rawProcessor->copy_mem_image(image->GetBits(), stride, 1);
		//image->Flip();
		imageLoadingFormat->SetPicture(image);
	}

	return imageLoadingFormat;
}