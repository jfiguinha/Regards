// InsertResource.cpp : définit le point d'entrée pour l'application console.
//

#include"stdafx.h"
#include"SqlInit.h"
#include <SqlResource.h>
using namespace Regards::Sqlite;


wxString to_string(const float & value)
{
	wxString chaine;
	chaine << value;
	return chaine;
}


float value[256];

int _tmain(int argc, _TCHAR* argv[])
{
	CSqlInit::InitializeSQLServerDatabase();
	CSqlResource sqlResource;
	//sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"H:\\developpement\\Regards\\resource\\res\\opencl\\noise.cl");
	sqlResource.InsertVector(L"IDB_PHOTOINDEX", L"H:\\developpement\\Regards\\resource\\res\\vector\\index.svg");
	/*
	sqlResource.InsertLibelle(L"LBLeffectDenoisingSigmaP",L"Effect.Denoising.Sigma P",1);
	sqlResource.InsertLibelle(L"LBLeffectDenoisingFSize",L"Effect.Denoising.FSize",1);
	sqlResource.InsertLibelle(L"LBLeffectDenoisingSigmaX",L"Effect.Denoising.Sigma X",1);
	sqlResource.InsertLibelle(L"LBLeffectSharpenLevel",L"Effect.Sharpen.Level",1);
	sqlResource.InsertLibelle(L"LBLtoneConstrastLevel",L"Tone.Constrast.Level",1);
	sqlResource.InsertLibelle(L"LBLtoneBrightnessLevel",L"Tone.Brightness.Level",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostRed",L"Color Boost.Red",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostGreen",L"Color Boost.Green",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostBlue",L"Color Boost.Blue",1);
	sqlResource.InsertLibelle(L"LBLenableEffect",L"Effect.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableBlurEffect",L"Effect.Denoising.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableSharpenEffect",L"Effect.Sharpen.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableColorEffect",L"Color Boost.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableBandCEffect",L"Tone.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableGrayScale",L"Effect.Gray Scale",1);
	sqlResource.InsertLibelle(L"LBLshowFPS",L"Effect.Show FPS",1);
	*/
	//sqlResource.InsertLibelle(L"REGARDSVERSION", L"2.0.0.6", 1);
	/*
	sqlResource.InsertLibelle(L"LBLUserMultiOther",L"Effect.User Multiplier.Other",1);
	sqlResource.InsertLibelle(L"LBLUserFlip",L"Effect.User_flip",1);
	sqlResource.InsertLibelle(L"LBLUserInterpolation",L"Effect.Interpolation Quality",1);
	sqlResource.InsertLibelle(L"LBLUserBlack",L"Effect.Black Level",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelRed",L"Effect.Black.Channel Red",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelGreen",L"Effect.Black.Channel Green",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelBlue",L"Effect.Black.Channel Blue",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelOther",L"Effect.Black.Channel Other",1);
	sqlResource.InsertLibelle(L"LBLUserSaturation",L"Effect.Saturation",1);
	sqlResource.InsertLibelle(L"LBLmed_passes",L"Effect.Median Filter Passes",1);
	sqlResource.InsertLibelle(L"LBLnoautobright",L"Effect.No auto bright",1);
	sqlResource.InsertLibelle(L"LBLauto_bright_thr",L"Effect.Auto brighness increase",1);
	sqlResource.InsertLibelle(L"LBLadjust_maximum_thr",L"Effect.Correcting channel overflow",1);
	sqlResource.InsertLibelle(L"LBLfuji_rotate",L"Effect.Use Fuji Rotate",1);
	sqlResource.InsertLibelle(L"LBLgreen_matching",L"Effect.Green channels disbalance",1);
	sqlResource.InsertLibelle(L"LBLdcb_iterations",L"Effect.Number of DCB correction passes",1);
	sqlResource.InsertLibelle(L"LBLdcb_enhance_fl",L"Effect.DCB interpolation with enhance interpolated colors",1);
	sqlResource.InsertLibelle(L"LBLfbdd_noiserd",L"Effect.Controls FBDD noise reduction",1);
	sqlResource.InsertLibelle(L"LBLeeci_refine",L"Effect.Use EECI refine for VCD interpolation",1);
	sqlResource.InsertLibelle(L"LBLes_med_passes",L"Effect.Number of edge-sensitive median filter passes after VCD+AHD demosaic",1);
	sqlResource.InsertLibelle(L"LBLca_correc",L"Effect.Chromatic aberrations suppression",1);
	sqlResource.InsertLibelle(L"LBLcared",L"Effect.Chromatic.Red",1);
	sqlResource.InsertLibelle(L"LBLcablue",L"Effect.Chromatic.Blue",1);
	sqlResource.InsertLibelle(L"LBLcfaline",L"Effect.Use Line noise reduction",1);
	sqlResource.InsertLibelle(L"LBLlinenoise",L"Effect.Line noise reduction",1);
	sqlResource.InsertLibelle(L"LBLcfa_clean",L"Effect.Use Reduce impulse noise and Gaussian high frequency",1); 
	sqlResource.InsertLibelle(L"LBLlclean",L"Effect.Reduce impulse noise.Luminance",1);
	sqlResource.InsertLibelle(L"LBLcclean",L"Effect.Reduce impulse noise.Color",1);
	sqlResource.InsertLibelle(L"LBLcfa_green",L"Effect.Use Reduces maze artifacts Green",1); 
	sqlResource.InsertLibelle(L"LBLgreen_thresh",L"Effect.Reduces maze artifacts Green",1);
	sqlResource.InsertLibelle(L"LBLexp_correc",L"Effect.Use Exposure correction before demosaic",1); 
	sqlResource.InsertLibelle(L"LBLexp_shift",L"Effect.Exposure correction.Shift in linear scale",1);
	sqlResource.InsertLibelle(L"LBLexp_preser",L"Effect.Exposure correction.Preserve highlights",1);
	sqlResource.InsertLibelle(L"LBLwf_debanding",L"Effect.Use banding suppression",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdRed",L"Effect.Banding suppression.Red",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdGreen",L"Effect.Banding suppression.Green",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdBlue",L"Effect.Banding suppression.Blue",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdOther",L"Effect.Banding suppression.Other",1);
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGYUV420",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegyuv420.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGDXVA2YUV420",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegdxva2.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGDXVA2",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegdxva2yuv420.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	/*
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	sqlResource.InsertText(L"IDR_BILATERALFILTER_RAW",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\bilateralfilter.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\interpolation.cl");
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	/*
		sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\sharpenMasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION_RGB", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation_rgb.cl");
	*/

	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	/*
	sqlResource.InsertText(L"IDR_BILATERALFILTER_RAW",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\bilateralfilter.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\swirl.cl");
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\interpolation.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	//sqlResource.InsertBitmap(L"IDB_BLACKROOM", L"PNG", L"d:\\resource\\res\\bitmap\\1475171356_raw.png");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	//sqlResource.InsertText(L"IDR_GLSL_ALPHA_SHADER",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\opengl\\alpha_shader.cg");
	//sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color_conversion.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	//sqlResource.InsertLibelle(L"LBLTEST", L"Test.Test", 1);

	CSqlInit::KillSqlEngine();
	return 0;

	sqlResource.InsertVector(L"IDB_CALENDAR", L"d:\\resource\\res\\vector\\calendar.svg");
	sqlResource.InsertVector(L"IDB_MAPLOCATION", L"d:\\resource\\res\\vector\\map_location.svg");
	sqlResource.InsertVector(L"IDB_PHOTOTEMP", L"d:\\resource\\res\\vector\\photo_temp.svg");
	sqlResource.InsertVector(L"IDB_LINK_ASSOC", L"d:\\resource\\res\\vector\\link.svg");
	sqlResource.InsertVector(L"IDB_ARROWRPNG", L"d:\\resource\\res\\vector\\next.svg");
	sqlResource.InsertVector(L"IDB_ARROWLPNG", L"d:\\resource\\res\\vector\\back.svg");
	sqlResource.InsertVector(L"IDB_ARROWTRACKRPNG", L"d:\\resource\\res\\vector\\trackend.svg");
	sqlResource.InsertVector(L"IDB_ARROWTRACKLPNG", L"d:\\resource\\res\\vector\\trackback.svg");
	sqlResource.InsertVector(L"IDB_BUTTON_CLICK", L"d:\\resource\\res\\vector\\butonclick.svg");
	sqlResource.InsertVector(L"IDB_BUTTON_CLICK_VERTICAL", L"d:\\resource\\res\\vector\\butonclick_vertical.svg");
	//sqlResource.InsertVector(L"IDB_DONATE", L"d:\\resource\\res\\vector\\donate.svg");
	sqlResource.InsertVector(L"IDB_FLIPHORZ", L"d:\\resource\\res\\vector\\flip_horizontal.svg");
	sqlResource.InsertVector(L"IDB_FLIPVERT", L"d:\\resource\\res\\vector\\flip_vertical.svg");
	sqlResource.InsertVector(L"IDB_PICTUREDETAILS", L"d:\\resource\\res\\vector\\pictures_details.svg");
	//sqlResource.InsertVector(L"IDB_EYEIRIS", L"d:\\resource\\res\\vector\\eye.svg");
	sqlResource.InsertVector(L"IDB_BOULESLIDER", L"d:\\resource\\res\\vector\\bouleSlider.svg");
	//sqlResource.InsertVector(L"IDB_CADRE_VIDEO", L"d:\\resource\\res\\vector\\cadre_video.svg");
	sqlResource.InsertVector(L"IDB_VOLUME_UP_VIDEO", L"d:\\resource\\res\\vector\\volumeup.svg");
	sqlResource.InsertVector(L"IDB_VOLUME_DOWN_VIDEO", L"d:\\resource\\res\\vector\\volumedown.svg");
	sqlResource.InsertVector(L"IDB_PHOTO", L"d:\\resource\\res\\vector\\photo.svg");
	//sqlResource.InsertVector(L"IDB_LINK_ASSOC", L"d:\\resource\\res\\vector\\link_assoc.svg");
	//sqlResource.InsertVector(L"IDB_REDEYE", L"d:\\resource\\res\\vector\\redeye.svg");


	sqlResource.InsertVector(L"IDB_OK", L"d:\\resource\\res\\vector\\ok.svg");
	sqlResource.InsertVector(L"IDB_CANCEL", L"d:\\resource\\res\\vector\\cancel.svg");
	sqlResource.InsertVector(L"IDB_DELETE", L"d:\\resource\\res\\vector\\delete.svg");
	sqlResource.InsertVector(L"IDB_CROP", L"d:\\resource\\res\\vector\\crop.svg");
	
	sqlResource.InsertVector(L"IDB_EMAIL", L"d:\\resource\\res\\vector\\email.svg");
	sqlResource.InsertVector(L"IDB_EXIT", L"d:\\resource\\res\\vector\\exit.svg");

	sqlResource.InsertVector(L"IDB_FOLDER", L"d:\\resource\\res\\vector\\folder.svg");
	sqlResource.InsertVector(L"IDB_FOLDER_REFRESH", L"d:\\resource\\res\\vector\\folder_refresh.svg");
	sqlResource.InsertVector(L"IDB_INFOS", L"d:\\resource\\res\\vector\\infos.svg");
	sqlResource.InsertVector(L"IDB_MULTIPLESELECT", L"d:\\resource\\res\\vector\\copy.svg");
	sqlResource.InsertVector(L"IDB_NEWPNG", L"d:\\resource\\res\\vector\\new.svg");
	sqlResource.InsertVector(L"IDB_PHOTOVIEWERPNG", L"d:\\resource\\res\\vector\\viewer.svg");
	
	sqlResource.InsertVector(L"IDB_PICTURETOOLBAR", L"d:\\resource\\res\\vector\\picture_toolbar.svg");
	sqlResource.InsertVector(L"IDB_PICTURE", L"d:\\resource\\res\\vector\\pictures.svg");
	sqlResource.InsertVector(L"IDB_PRINTERPNG", L"d:\\resource\\res\\vector\\Printer_Button.svg");
	sqlResource.InsertVector(L"IDB_ROTATION270", L"d:\\resource\\res\\vector\\rotation270.svg");
	sqlResource.InsertVector(L"IDB_ROTATION90", L"d:\\resource\\res\\vector\\rotation90.svg");
	sqlResource.InsertVector(L"IDB_SAVE", L"d:\\resource\\res\\vector\\save.svg");
	sqlResource.InsertVector(L"IDB_SHRINK", L"d:\\resource\\res\\vector\\shrink.svg");

	//sqlResource.InsertVector(L"IDB_CIRCLE_NEXT", L"d:\\resource\\res\\vector\\circle_next_arrow.svg");
	//sqlResource.InsertVector(L"IDB_CIRCLE_PREVIOUS", L"d:\\resource\\res\\vector\\circle_previous_arrow.svg");
	sqlResource.InsertVector(L"IDB_CHECKBOX_ON", L"d:\\resource\\res\\vector\\checkbox_on.svg");
	sqlResource.InsertVector(L"IDB_CHECKBOX_OFF", L"d:\\resource\\res\\vector\\checkbox_off.svg");


	sqlResource.InsertVector(L"IDB_PLAY", L"d:\\resource\\res\\vector\\play.svg");
	sqlResource.InsertVector(L"IDB_STOP", L"d:\\resource\\res\\vector\\stop.svg");
	sqlResource.InsertVector(L"IDB_PAUSE", L"d:\\resource\\res\\vector\\pause.svg");
	sqlResource.InsertVector(L"IDB_THUMBNAILPNG", L"d:\\resource\\res\\vector\\thumbnail.svg");
	sqlResource.InsertVector(L"IDB_ZOOMMOINS", L"d:\\resource\\res\\vector\\zoommoins.svg");
	sqlResource.InsertVector(L"IDB_ZOOMPLUS", L"d:\\resource\\res\\vector\\zoomplus.svg");

	
	sqlResource.InsertVector(L"IDB_PLAY_VIDEO", L"d:\\resource\\res\\vector\\play_video.svg");
	sqlResource.InsertVector(L"IDB_PAUSE_VIDEO", L"d:\\resource\\res\\vector\\pause_video.svg");

	sqlResource.InsertVector(L"IDB_VOLUME_SPEAKER_VIDEO", L"d:\\resource\\res\\vector\\speaker.svg");
	sqlResource.InsertVector(L"IDB_SCREENPNG", L"d:\\resource\\res\\vector\\monitor.svg");

	sqlResource.InsertVector(L"IDB_PLUS", L"d:\\resource\\res\\vector\\plus.svg");
	sqlResource.InsertVector(L"IDB_MINUS", L"d:\\resource\\res\\vector\\minus.svg");

	/*
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP1", L"PNG", L"d:\\resource\\res\\bitmap\\loading1.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP2", L"PNG", L"d:\\resource\\res\\bitmap\\loading2.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP3", L"PNG", L"d:\\resource\\res\\bitmap\\loading3.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP4", L"PNG", L"d:\\resource\\res\\bitmap\\loading4.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP5", L"PNG", L"d:\\resource\\res\\bitmap\\loading5.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP6", L"PNG", L"d:\\resource\\res\\bitmap\\loading6.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP7", L"PNG", L"d:\\resource\\res\\bitmap\\loading7.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP8", L"PNG", L"d:\\resource\\res\\bitmap\\loading8.png");

	sqlResource.InsertBitmap(L"IDB_ARROWRPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowright.png");
	sqlResource.InsertBitmap(L"IDB_ARROWLPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowleft.png");
	sqlResource.InsertBitmap(L"IDB_ARROWTRACKRPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowtrackright.png");
	sqlResource.InsertBitmap(L"IDB_ARROWTRACKLPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowtrackleft.png");
	sqlResource.InsertBitmap(L"IDB_BUTTON_CLICK", L"PNG", L"d:\\resource\\res\\bitmap\\butonclick.png");
	sqlResource.InsertBitmap(L"IDB_BUTTON_CLICK_VERTICAL", L"PNG", L"d:\\resource\\res\\bitmap\\butonclick_vertical.png");
	sqlResource.InsertBitmap(L"IDB_OK", L"PNG", L"d:\\resource\\res\\bitmap\\ok.png");
	sqlResource.InsertBitmap(L"IDB_CANCEL", L"PNG", L"d:\\resource\\res\\bitmap\\cancel.png");
	sqlResource.InsertBitmap(L"IDB_DELETE", L"PNG", L"d:\\resource\\res\\bitmap\\delete.png");
	sqlResource.InsertBitmap(L"IDB_CROP", L"PNG", L"d:\\resource\\res\\bitmap\\crop.png");
	sqlResource.InsertBitmap(L"IDB_DONATE", L"PNG", L"d:\\resource\\res\\bitmap\\donate.png");
	sqlResource.InsertBitmap(L"IDB_EMAIL", L"PNG", L"d:\\resource\\res\\bitmap\\email.png");
	sqlResource.InsertBitmap(L"IDB_EXIT", L"PNG", L"d:\\resource\\res\\bitmap\\exit.png");
	sqlResource.InsertBitmap(L"IDB_FLIPHORZ", L"PNG", L"d:\\resource\\res\\bitmap\\flip_horizontal.png");
	sqlResource.InsertBitmap(L"IDB_FLIPVERT", L"PNG", L"d:\\resource\\res\\bitmap\\flip_vertical.png");
	sqlResource.InsertBitmap(L"IDB_FOLDER", L"PNG", L"d:\\resource\\res\\bitmap\\folder.png");
	sqlResource.InsertBitmap(L"IDB_FOLDER_REFRESH", L"PNG", L"d:\\resource\\res\\bitmap\\folder_refresh.png");
	sqlResource.InsertBitmap(L"IDB_INFOS", L"PNG", L"d:\\resource\\res\\bitmap\\infos.png");
	sqlResource.InsertBitmap(L"IDB_MULTIPLESELECT", L"PNG", L"d:\\resource\\res\\bitmap\\copy.png");
	sqlResource.InsertBitmap(L"IDB_NEWPNG", L"PNG", L"d:\\resource\\res\\bitmap\\new.png");
	sqlResource.InsertBitmap(L"IDB_PHOTOVIEWERPNG", L"PNG", L"d:\\resource\\res\\bitmap\\viewer.png");
	sqlResource.InsertBitmap(L"IDB_PICTUREDETAILS", L"PNG", L"d:\\resource\\res\\bitmap\\pictures_details.png");
	sqlResource.InsertBitmap(L"IDB_PICTURETOOLBAR", L"PNG", L"d:\\resource\\res\\bitmap\\picture_toolbar.png");
	sqlResource.InsertBitmap(L"IDB_PICTURE", L"PNG", L"d:\\resource\\res\\bitmap\\pictures.png");
	sqlResource.InsertBitmap(L"IDB_PRINTERPNG", L"PNG", L"d:\\resource\\res\\bitmap\\Printer_Button.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION270", L"PNG", L"d:\\resource\\res\\bitmap\\rotation270.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION90", L"PNG", L"d:\\resource\\res\\bitmap\\rotation90.png");
	sqlResource.InsertBitmap(L"IDB_SAVE", L"PNG", L"d:\\resource\\res\\bitmap\\save.png");
	sqlResource.InsertBitmap(L"IDB_SHRINK", L"PNG", L"d:\\resource\\res\\bitmap\\shrink.png");

	sqlResource.InsertBitmap(L"IDB_CIRCLE_NEXT", L"PNG", L"d:\\resource\\res\\bitmap\\circle_next_arrow.png");
	sqlResource.InsertBitmap(L"IDB_CIRCLE_PREVIOUS", L"PNG", L"d:\\resource\\res\\bitmap\\circle_previous_arrow.png");
	sqlResource.InsertBitmap(L"IDB_CHECKBOX_ON", L"PNG", L"d:\\resource\\res\\bitmap\\checkbox_on.png");
	sqlResource.InsertBitmap(L"IDB_CHECKBOX_OFF", L"PNG", L"d:\\resource\\res\\bitmap\\checkbox_off.png");
	sqlResource.InsertBitmap(L"IDB_EYEIRIS", L"PNG", L"d:\\resource\\res\\bitmap\\eye.png");
	sqlResource.InsertBitmap(L"IDB_BOULESLIDER", L"PNG", L"d:\\resource\\res\\bitmap\\bouleSlider.png");

	sqlResource.InsertBitmap(L"IDB_PLAY", L"PNG", L"d:\\resource\\res\\bitmap\\play.png");
	sqlResource.InsertBitmap(L"IDB_STOP", L"PNG", L"d:\\resource\\res\\bitmap\\stop.png");
	sqlResource.InsertBitmap(L"IDB_PAUSE", L"PNG", L"d:\\resource\\res\\bitmap\\pause.png");
	sqlResource.InsertBitmap(L"IDB_THUMBNAILPNG", L"PNG", L"d:\\resource\\res\\bitmap\\thumbnail.png");
	sqlResource.InsertBitmap(L"IDB_ZOOMMOINS", L"PNG", L"d:\\resource\\res\\bitmap\\zoommoins.png");
	sqlResource.InsertBitmap(L"IDB_ZOOMPLUS", L"PNG", L"d:\\resource\\res\\bitmap\\zoomplus.png");

	sqlResource.InsertBitmap(L"IDB_CADRE_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\cadre_video.png");
	sqlResource.InsertBitmap(L"IDB_PLAY_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\play_video.png");
	sqlResource.InsertBitmap(L"IDB_PAUSE_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\pause_video.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_UP_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\volumeup.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_DOWN_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\volumedown.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_SPEAKER_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\speaker.png");
	sqlResource.InsertBitmap(L"IDB_SCREENPNG", L"PNG", L"d:\\resource\\res\\bitmap\\monitor.png");
	sqlResource.InsertBitmap(L"IDB_LINK_ASSOC", L"PNG", L"d:\\resource\\res\\bitmap\\link_assoc.png");
	sqlResource.InsertBitmap(L"IDB_REDEYE", L"PNG", L"d:\\resource\\res\\bitmap\\redeye.png");
	sqlResource.InsertBitmap(L"IDB_PLUS", L"PNG", L"d:\\resource\\res\\bitmap\\plus.png");
	sqlResource.InsertBitmap(L"IDB_MINUS", L"PNG", L"d:\\resource\\res\\bitmap\\minus.png");
	sqlResource.InsertBitmap(L"IDB_PHOTO", L"PNG", L"d:\\resource\\res\\bitmap\\photo.png");

	//CSqlResource sqlResource;
	//wstring libelle = sqlResource.GetText(1);
	/*
	sqlResource.InsertBitmap(L"IDB_PHOTO", L"PNG", L"d:\\resource\\res\\bitmap\\photo.png");
	sqlResource.InsertBitmap(L"IDB_REPORTVIEWPNG",L"PNG",L"d:\\resource\\res\\bitmap\\details.png");
	sqlResource.InsertBitmap(L"IDB_LARGEICONPNG",L"PNG",L"d:\\resource\\res\\bitmap\\large.png");
	sqlResource.InsertBitmap(L"IDB_LISTVIEWBUTTON",L"PNG",L"d:\\resource\\res\\bitmap\\list.png");
	sqlResource.InsertBitmap(L"IDB_SMALLICONPNG",L"PNG",L"d:\\resource\\res\\bitmap\\small.png");
	sqlResource.InsertBitmap(L"IDB_PHOTOEDITORPNG",L"PNG",L"d:\\resource\\res\\bitmap\\editor.png");
	sqlResource.InsertBitmap(L"IDB_FOLDERUPPNG" ,L"PNG",L"d:\\resource\\res\\bitmap\\folderup.png");
	sqlResource.InsertBitmap(L"IDB_GLASSPNG",L"PNG",L"d:\\resource\\res\\bitmap\\1398712562_search_32.png");
	sqlResource.InsertBitmap(L"IDB_EDITORPNG" ,L"PNG",L"d:\\resource\\res\\bitmap\\eidtor 2.png");
	sqlResource.InsertBitmap(L"IDB_CATALOG",L"PNG",L"d:\\resource\\res\\bitmap\\catalog.png");
	sqlResource.InsertBitmap(L"IDB_PREVIEW",L"PNG",L"d:\\resource\\res\\bitmap\\preview.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION90",L"PNG",L"d:\\resource\\res\\bitmap\\rotation90.png");
	sqlResource.InsertBitmap(L"IDB_ZOOM_SELECTION",L"PNG",L"d:\\resource\\res\\bitmap\\zoom_selection.png");
	sqlResource.InsertBitmap(L"IDB_SELECTION" ,L"PNG",L"d:\\resource\\res\\bitmap\\selection.png");
	sqlResource.InsertBitmap(L"IDB_MOVE_PICTURE",L"PNG",L"d:\\resource\\res\\bitmap\\move_picture.png");
	sqlResource.InsertBitmap(L"IDB_PEN" ,L"PNG",L"d:\\resource\\res\\bitmap\\pen.png");
	sqlResource.InsertBitmap(L"IDB_SYSTEM_FOLDER" ,L"PNG",L"d:\\resource\\res\\bitmap\\system_folder.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_UP" ,L"PNG",L"d:\\resource\\res\\bitmap\\volumeup.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_DOWN" ,L"PNG",L"d:\\resource\\res\\bitmap\\volumedown.png");
	sqlResource.InsertBitmap(L"IDB_EFFECT" ,L"PNG",L"d:\\resource\\res\\bitmap\\effect.png");
	sqlResource.InsertBitmap(L"IDB_TOOLS",L"PNG",L"d:\\resource\\res\\bitmap\\tools.png");
	sqlResource.InsertBitmap(L"IDB_COPY",L"PNG",L"d:\\resource\\res\\bitmap\\copy.png");
	sqlResource.InsertBitmap(L"IDB_TITLEREGARDS",L"PNG",L"d:\\resource\\res\\bitmap\\title.png");*/
	//return 0;
	

	sqlResource.InsertText(L"IDR_GLSL_SHADER_BELL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_Bell.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BILINEAR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiLinear.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BSPLINE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BSpline.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_CATMULL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_CatMull.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\matrix_convolution.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MEDIAN_SHADER",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\median_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_TRIANGLE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiTriangle.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NIVEAUDEGRIS",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\niveaudegris.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\color_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_EDGE_DETECTION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\edge_detection.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SEPIA",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\sepia_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOIRETBLANC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\noiretblanc_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NEGATIF",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\negative_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_DILATE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\dilate_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ERODE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\erode_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\mosaic_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPV",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\flipvert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPH",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\fliphorz_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT90",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rotation90_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT270",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rotation270_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTFREE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\freerotation_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INTERPOLATION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_interpolation.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBICYUV",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_YUV.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_VIDEO_EFFECT",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_video_effect.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_GAUSSIAN_BLUR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\gaussian_blur.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SHARPEN_MASKING",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\sharpen_masking.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RENDER_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_BitmapRender.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADD_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmap_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScale_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE_POSITION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScalePosition_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDALPHABLENDING_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddAlphaBlendingBitmapShader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_TRANSITION	",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScale_shaderTransition.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INIT",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\init_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SOLARIZE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\solarize.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RGB",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rgb.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BRIGHTANDCONTRAST",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\brightandcontrast.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_CLOUDS",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\clouds.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SWIRL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\swirl.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PHOTOFILTRE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\photofiltre.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_POSTERIZE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\posterize.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOTIONBLUR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\motionBlur.cg");
		//Version 4
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_v4.cg");
	sqlResource.InsertText(L"IDR_GLSL_VERTEXSHADER_COORDONATE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\vertex_shader_coordonate.cg");
	sqlResource.InsertText(L"IDR_GLSL_COMPUTE_TEST",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\compute_test.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPV_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\flipvert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPH_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\fliphorz_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT90_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation90_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT270_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation270_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NIVEAUDEGRIS_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\niveaudegris.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOIRETBLANC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\noiretblanc_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\matrix_convolution.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION5X5_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\matrix_convolution5x5.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_COPY_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\copy_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MEDIAN_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\median_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_INTERPOLATION_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\pixel_shader_BiCubic.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_INTERPOLATION_ZONE_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\pixel_shader_BiCubic_zone.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SEPIA_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\sepia.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NEGATIF_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\negatif.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RGB_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rgb.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_LIGHTANDCONTRAST_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\lightnessandcontrast.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PHOTOFILTRE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\photo_filtre.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOSAIC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\mosaic_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ERODE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\erode_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_DILATE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\dilate_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOISE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\noise_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTATEFREE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation_free_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INSERT_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\insert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOTIONBLUR_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\motionblur.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PERLINNOISE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\perlinnoise_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SWIRL_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\swirl_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTATEFREEPREVIEW_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation_free_preview_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_POSTERIZE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\posterize_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NV12TORGB32_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\nv12torgb32.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SHARPENMASKING_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\sharpenMasking.cg");

	/////////////////////////////////////////////////////////////////////////////
	//
	// OPENCL
	//
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\sharpenMasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION_RGB", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation_rgb.cl");
	//Email
	sqlResource.InsertText(L"EMAIL", L"EMAIL_EML", L"d:\\resource\\email.eml");

	/////////////////////////////////////////////////////////////////////////////
	//
	// String Table
	//
	sqlResource.InsertLibelle(L"REGARDSVERSION", L"2.0.0.6", 1);
	sqlResource.InsertLibelle(L"ADRESSEWEBVERSION",L"/jacques.figuinha/version.txt", 1);
	sqlResource.InsertLibelle(L"SITEWEB", L"http://perso.numericable.fr/jacques.figuinha/regards/", 1);
	sqlResource.InsertLibelle(L"SERVEURWEB", L"perso.numericable.fr", 1);
	sqlResource.InsertLibelle(L"DONATE", L"http://perso.numericable.fr/jacques.figuinha/regards/donate.htm", 1);
	sqlResource.InsertLibelle(L"LBLINFOS", L"Infos", 1);
	sqlResource.InsertLibelle(L"LBLHISTORY", L"History", 1);
	sqlResource.InsertLibelle(L"LBLMAPS", L"Maps", 1);
	sqlResource.InsertLibelle(L"LBLEFFECT", L"Effect", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTPARAMETER",L"Effect Parameter", 1);
	sqlResource.InsertLibelle(L"LBLSAVE", L"Save", 1);
	sqlResource.InsertLibelle(L"LBLROTATE90", L"Rotate 90", 1);
	sqlResource.InsertLibelle(L"LBLCOLOREFFECT", L"Color Effect", 1);
	sqlResource.InsertLibelle(L"LBLEMAIL", L"Email", 1);
	sqlResource.InsertLibelle(L"LBLROTATE270", L"Rotate 270", 1);
	sqlResource.InsertLibelle(L"LBLCONVOLUTIONEFFECT",L"Convolution Effect", 1);
	sqlResource.InsertLibelle(L"LBLPRINT", L"Print", 1);
	sqlResource.InsertLibelle(L"LBLFLIPV", L"Flip vertical", 1);
	sqlResource.InsertLibelle(L"LBLSPECIALEFFECT",L"Special Effect", 1);
	sqlResource.InsertLibelle(L"LBLCROP", L"Crop", 1);
	sqlResource.InsertLibelle(L"LBLDOWNLOADNEWVERSION",L"Do you want to download new version ?", 1);
	sqlResource.InsertLibelle(L"LBLSHRINK", L"Shrink Picture", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTRADIUS", L"Effect.Radius", 1);
	sqlResource.InsertLibelle(L"LBLZOOMON", L"Zoom On", 1);
	sqlResource.InsertLibelle(L"LBLZOOMOFF", L"Zoom Off", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTSIGMA", L"Effect.Sigma", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTANGLE", L"Effect.Angle", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSFREQUENCY", L"Clouds.Frequency", 1);
	sqlResource.InsertLibelle(L"LBLfilterSoften", L"Soften", 1);
	sqlResource.InsertLibelle(L"LBLfilterBlur", L"Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterSolarize", L"Solarize", 1);
	sqlResource.InsertLibelle(L"LBLfilterGaussian", L"Gaussian Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterMedian", L"Median", 1);
	sqlResource.InsertLibelle(L"LBLfilterMotion", L"Motion Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterRotate", L"Rotate Free", 1);
	sqlResource.InsertLibelle(L"LBLfilterLight", L"Brigthness and Contrast", 1);
	sqlResource.InsertLibelle(L"LBLfilterPhoto", L"Photo Filtre", 1);
	sqlResource.InsertLibelle(L"LBLfilterPosterisation", L"Posterisation", 1);
	sqlResource.InsertLibelle(L"LBLfilterColor", L"Color Balance", 1);
	sqlResource.InsertLibelle(L"LBLfilterSwirl", L"Swirl", 1);
	sqlResource.InsertLibelle(L"LBLfilterClouds", L"Clouds", 1);
	sqlResource.InsertLibelle(L"LBLfilterErode", L"Erode", 1);
	sqlResource.InsertLibelle(L"LBLfilterDilate", L"Dilate", 1);
	sqlResource.InsertLibelle(L"LBLfilterSharpen", L"Sharpen", 1);
	sqlResource.InsertLibelle(L"LBLfilterSharpenStrong",L"Sharpen Strong", 1);
	sqlResource.InsertLibelle(L"LBLfilterNoise", L"Noise", 1);
	sqlResource.InsertLibelle(L"LBLfilterMosaic", L"Mosaic", 1);
	sqlResource.InsertLibelle(L"LBLfilterEmboss", L"Emboss", 1);
	sqlResource.InsertLibelle(L"LBLfilterGrey", L"Grey Level", 1);
	sqlResource.InsertLibelle(L"LBLfilterSepia", L"Sepia", 1);
	sqlResource.InsertLibelle(L"LBLfilterBlack", L"Black and White", 1);
	sqlResource.InsertLibelle(L"LBLfilterEdge", L"Edge", 1);
	sqlResource.InsertLibelle(L"LBLfilterNegatif", L"Negatif", 1);
	sqlResource.InsertLibelle(L"LBLfilterLensFlare", L"Lens Flare", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSAMPLITUDE", L"Clouds.Amplitude", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTRED", L"Clouds.Color Front.Red", 1);
	sqlResource.InsertLibelle(L"LBLFLIPH", L"Flip horizontal", 1);
	sqlResource.InsertLibelle(L"LBLOPENFOLDER", L"Open Folder", 1);
	sqlResource.InsertLibelle(L"LBLASSOCIATE", L"Associate", 1);
	sqlResource.InsertLibelle(L"LBLDONATE", L"Donate", 1);
	sqlResource.InsertLibelle(L"LBLQUIT", L"Quit", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTGREEN", L"Clouds.Color Front.Green", 1);
	sqlResource.InsertLibelle(L"LBLOPEN", L"Open", 1);
	sqlResource.InsertLibelle(L"LBLFULLSCREEN", L"Fullscreen", 1);
	sqlResource.InsertLibelle(L"LBLSETTINGS", L"Settings", 1);
	sqlResource.InsertLibelle(L"LBLINFORMATIONS", L"Informations", 1);
	sqlResource.InsertLibelle(L"LBLPREVIEW", L"Preview", 1);
	sqlResource.InsertLibelle(L"LBLVIDEOTHUMBNAIL", L"Video Thumbnail", 1);
	sqlResource.InsertLibelle(L"LBLTHUMBNAIL", L"Thumbnail", 1);
	sqlResource.InsertLibelle(L"LBLCRITERIA", L"Criteria", 1);
	sqlResource.InsertLibelle(L"LBLSELECTFOLDER", L"Select Folder", 1);
	sqlResource.InsertLibelle(L"LBLLISTPICTURE", L"List Picture", 1);
	sqlResource.InsertLibelle(L"LBLPREVIEWPICTURE", L"Preview Picture", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTBLUE", L"Clouds.Color Front.Blue", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKRED", L"Clouds.Color Back.Red", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKGREEN", L"Clouds.Color Back.Green", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKBLUE", L"Clouds.Color Back.Blue", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORRED", L"Effect.Color.Red", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORGREEN", L"Effect.Color.Green", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORBLUE", L"Effect.Color.Blue", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTLEVEL", L"Effect.Level", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTGAMMA", L"Effect.Gamma", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTINTENSITY", L"Effect.Intensity", 1);
	sqlResource.InsertLibelle(L"LBLSELECT", L"Select", 1);
	sqlResource.InsertLibelle(L"LBLDELETE", L"Delete files", 1);
	sqlResource.InsertLibelle(L"LBLCOPY", L"Copy files", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTMEDIANLEVEL", L"Effect.Median.Level", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCONTRAST", L"Effect.Constrast", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTBRIGHTNESS", L"Effect.Brightness", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTTHRESHOLD", L"Effect.Threshold", 1);
	sqlResource.InsertLibelle(L"LBLROTATIONANGLE", L"Rotation.Angle", 1);
	sqlResource.InsertLibelle(L"LBLOK", L"Ok", 1);
	sqlResource.InsertLibelle(L"LBLCANCEL", L"Cancel", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTSHARPENLEVEL", L"Effect.Sharpen.Level", 1);
	sqlResource.InsertLibelle(L"LBLFIRST", L"First", 1);
	sqlResource.InsertLibelle(L"LBLPREVIOUS", L"Previous", 1);
	sqlResource.InsertLibelle(L"LBLPLAY", L"Play", 1);
	sqlResource.InsertLibelle(L"LBLSTOP", L"Stop", 1);
	sqlResource.InsertLibelle(L"LBLNEXT", L"Next", 1);
	sqlResource.InsertLibelle(L"LBLEND", L"End", 1);
	sqlResource.InsertLibelle(L"LBLREFRESH", L"Refresh Folder", 1);
	sqlResource.InsertLibelle(L"LBLPHOTOVIEWER", L"Photo Viewer", 1);
	sqlResource.InsertLibelle(L"LBLLAST", L"Last", 1);
	sqlResource.InsertLibelle(L"LBLNOTGEO", L"Not Geolocalized", 1);
	sqlResource.InsertLibelle(L"LBLALLPHOTO", L"All photos", 1);
	sqlResource.InsertLibelle(L"LBLMONTHNAME", L"January,February,March,Avril,May,June,July,August,September,October,November,December", 1);
	sqlResource.InsertLibelle(L"LBLDAYNAME", L"Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday", 1);
	sqlResource.InsertLibelle(L"LBLTONECONTRASTLEVEL", L"Tone.Constrast.Level", 1);
	sqlResource.InsertLibelle(L"LBLTONEEXPOSURELEVEL", L"Tone.Exposure.Level", 1);
	sqlResource.InsertLibelle(L"LBLTONEBRIGHTNESSLEVEL", L"Tone.Brightness.Level", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTRED", L"Color Boost.Red", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTGREEN", L"Color Boost.Green", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTBLUE", L"Color Boost.Blue", 1);
	sqlResource.InsertLibelle(L"LBLPICTUREDETAILS", L"Picture Toolbar", 1);
	sqlResource.InsertLibelle(L"LBLFOLDER", L"Show Folder", 1);
	sqlResource.InsertLibelle(L"LBLfilterRedEye", L"Red Eye", 1);
	sqlResource.InsertLibelle(L"LBLHISTOGRAMEFFECT", L"Histogram Effect", 1);
	sqlResource.InsertLibelle(L"LBLHistogramNormalize", L"Histogram Normalize", 1);
	sqlResource.InsertLibelle(L"LBLHistogramEqualize", L"Histogram Equalize", 1);
	sqlResource.InsertLibelle(L"LBLHistogramLog", L"Histogram Log", 1);
	sqlResource.InsertLibelle(L"LBLDeleteText", L"Do you want to delete this files ?", 1);
	sqlResource.InsertLibelle(L"LBLDeleteCaption", L"Confirmation", 1);
	sqlResource.InsertLibelle(L"LBLDeleteMessage", L"Delete Processing", 1);
	sqlResource.InsertLibelle(L"LBLDeleteFinalMessage", L"Delete Files is finished.", 1);
	sqlResource.InsertLibelle(L"LBLOpenCLDevice", L"No OpenCL device compatible !", 1);
	sqlResource.InsertLibelle(L"LBLCopyText", L"Select Save Folder", 1);
	sqlResource.InsertLibelle(L"LBLCopyMessage", L"Copy Processing", 1);
	sqlResource.InsertLibelle(L"LBLCopyFinalMessage", L"Copy Files is finished.", 1);
	sqlResource.InsertLibelle(L"LBLClose", L"Close", 1);
	sqlResource.InsertLibelle(L"LBLGPSADRESS", L"http://www.geoplugin.net", 1);
	CSqlInit::KillSqlEngine();
	return 0;
}

