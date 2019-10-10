#include <header.h>
#include "DecodeRaw.h"
#include "DecodeRawParameter.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <FilterData.h>
#include <BitmapWndViewer.h>
#include <Metadata.h>
#include <ConvertUtility.h>
#include <FiltreEffet.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Viewer;

CDecodeRaw::CDecodeRaw()
{
    //libelleEffectContrast = CLibResource::LoadStringFromResource(L"LBLEFFECTCONTRAST",1);
    libellebright =  CLibResource::LoadStringFromResource(L"LBLEFFECTBRIGHTNESS",1);//"Effect.Brightness";
    libellehighlight = CLibResource::LoadStringFromResource(L"LBLEFFECTHIGHLIGHT",1);//"Effect.Highlight";
	libellethreshold = CLibResource::LoadStringFromResource(L"LBLEFFECTTHRESHOLD",1);//"Effect.Treshold";
	libelleuse_auto_wb = CLibResource::LoadStringFromResource(L"LBLEFFECTAUTOWB",1);//"Effect.AutoWB";
	libelleuse_camera_wb= CLibResource::LoadStringFromResource(L"LBLEFFECTCAMERAWB",1);//"Effect.CameraWB";
	libelleuse_halfsize= CLibResource::LoadStringFromResource(L"LBLEFFECTHALFSIZE",1);//"Effect.Half Size";
	libelleuse_cameramatrix= CLibResource::LoadStringFromResource(L"LBLEFFECTUSECAMERAMATRIX",1);//"Effect.Use Camera Matrix";
	libelleuse_aberRedEnable = CLibResource::LoadStringFromResource(L"LBLEFFECTENABLEREDMULTIPLIER",1);//"Effect.Aber.Enable Red Multiplier";
	libelleuse_aberRed = CLibResource::LoadStringFromResource(L"LBLEFFECTREDMULTIPLIER",1);//"Effect.Aber.Red Multiplier";
	libelleuse_aberGreenEnable= CLibResource::LoadStringFromResource(L"LBLEFFECTENABLEGREENMULTIPLIER",1);//"Effect.Aber.Enable Green Multiplier";
	libelleuse_aberGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTGREENMULTIPLIER",1);//"Effect.Aber.Green Multiplier";
	libelleuse_UserMultiRed = CLibResource::LoadStringFromResource(L"LBLEFFECTUSERMULTIPLIERRED",1);//"Effect.User Multiplier.Red";
	libelleuse_UserMultiGreen = CLibResource::LoadStringFromResource(L"LBLEFFECTUSERMULTIPLIERGREEN",1);//"Effect.User Multiplier.Green";
	libelleuse_UserMultiBlue = CLibResource::LoadStringFromResource(L"LBLEFFECTUSERMULTIPLIERBLUE",1);//"Effect.User Multiplier.Blue";
	libelleuse_UserMultiOther = CLibResource::LoadStringFromResource(L"LBLUserMultiOther",1);
	libelleuse_UserFlip = CLibResource::LoadStringFromResource(L"LBLUserFlip",1);
	libelleuse_UserInterpolation = CLibResource::LoadStringFromResource(L"LBLUserInterpolation",1);
	libelleuse_UserBlack =CLibResource::LoadStringFromResource(L"LBLUserBlack",1);
	libelleuse_UserBlackChannelRed = CLibResource::LoadStringFromResource(L"LBLUserBlackChannelRed",1);
	libelleuse_UserBlackChannelGreen= CLibResource::LoadStringFromResource(L"LBLUserBlackChannelGreen",1);
	libelleuse_UserBlackChannelBlue = CLibResource::LoadStringFromResource(L"LBLUserBlackChannelBlue",1);
	libelleuse_UserBlackChannelOther= CLibResource::LoadStringFromResource(L"LBLUserBlackChannelOther",1);
	libelleuse_UserSaturation = CLibResource::LoadStringFromResource(L"LBLUserSaturation",1);//"Effect.Saturation";
	libelleuse_med_passes = CLibResource::LoadStringFromResource(L"LBLmed_passes",1);//"Effect.Median Filter Passes";
	libelleuse_noautobright = CLibResource::LoadStringFromResource(L"LBLnoautobright",1);//"Effect.No auto bright";
	libelleauto_bright_thr = CLibResource::LoadStringFromResource(L"LBLauto_bright_thr",1);//"Effect.Auto brighness increase";
	libelleadjust_maximum_thr= CLibResource::LoadStringFromResource(L"LBLadjust_maximum_thr",1);//"Effect.Correcting channel overflow";
	libelleuse_fuji_rotate= CLibResource::LoadStringFromResource(L"LBLfuji_rotate",1);//"Effect.Use Fuji Rotate";
	libellegreen_matching= CLibResource::LoadStringFromResource(L"LBLgreen_matching",1);//"Effect.Green channels disbalance";
	libelledcb_iterations= CLibResource::LoadStringFromResource(L"LBLdcb_iterations",1);//"Effect.Number of DCB correction passes";
	libelledcb_enhance_fl= CLibResource::LoadStringFromResource(L"LBLdcb_enhance_fl",1);//"Effect.DCB interpolation with enhance interpolated colors";
	libellefbdd_noiserd= CLibResource::LoadStringFromResource(L"LBLfbdd_noiserd",1);//"Effect.Controls FBDD noise reduction";
	libelleeeci_refine= CLibResource::LoadStringFromResource(L"LBLeeci_refine",1);//"Effect.Use EECI refine for VCD interpolation";
	libellees_med_passes= CLibResource::LoadStringFromResource(L"LBLes_med_passes",1);//"Effect.Number of edge-sensitive median filter passes after VCD+AHD demosaic";
	libelleca_correc= CLibResource::LoadStringFromResource(L"LBLca_correc",1);//"Effect.Chromatic aberrations suppression";
	libellecared= CLibResource::LoadStringFromResource(L"LBLcared",1);//"Effect.Chromatic.Red";
	libellecablue= CLibResource::LoadStringFromResource(L"LBLcablue",1);//"Effect.Chromatic.Blue";
	libellecfaline= CLibResource::LoadStringFromResource(L"LBLcfaline",1);//"Effect.Use Line noise reduction";
	libellelinenoise= CLibResource::LoadStringFromResource(L"LBLlinenoise",1);//"Effect.Line noise reduction";
	libellecfa_clean= CLibResource::LoadStringFromResource(L"LBLcfa_clean",1); //"Effect.Use Reduce impulse noise and Gaussian high frequency"; 
	libellelclean= CLibResource::LoadStringFromResource(L"LBLlclean",1);//"Effect.Reduce impulse noise.Luminance";
	libellecclean= CLibResource::LoadStringFromResource(L"LBLcclean",1);//"Effect.Reduce impulse noise.Color";
	libellecfa_green= CLibResource::LoadStringFromResource(L"LBLcfa_green",1); //"Effect.Use Reduces maze artifacts Green"; 
	libellegreen_thresh= CLibResource::LoadStringFromResource(L"LBLgreen_thresh",1);//"Effect.Reduces maze artifacts Green";
	libelleexp_correc= CLibResource::LoadStringFromResource(L"LBLexp_correc",1); //"Effect.Use Exposure correction before demosaic"; 
	libelleexp_shift = CLibResource::LoadStringFromResource(L"LBLexp_shift",1);//"Effect.Exposure correction.Shift in linear scale";
	libelleexp_preser= CLibResource::LoadStringFromResource(L"LBLexp_preser",1);//"Effect.Exposure correction.Preserve highlights";
	libellewf_debanding= CLibResource::LoadStringFromResource(L"LBLwf_debanding",1);//"Effect.Use banding suppression";
	libellewf_deband_tresholdRed= CLibResource::LoadStringFromResource(L"LBLwf_deband_tresholdRed",1);//"Effect.Banding suppression.Red";
	libellewf_deband_tresholdGreen= CLibResource::LoadStringFromResource(L"LBLwf_deband_tresholdGreen",1);//"Effect.Banding suppression.Green";
	libellewf_deband_tresholdBlue= CLibResource::LoadStringFromResource(L"LBLwf_deband_tresholdBlue",1);//"Effect.Banding suppression.Blue";
	libellewf_deband_tresholdOther= CLibResource::LoadStringFromResource(L"LBLwf_deband_tresholdOther",1);//"Effect.Banding suppression.Other";
	firstUpdate = true;
	rawDecoder = nullptr;
}

CDecodeRaw::~CDecodeRaw()
{
	if (rawDecoder != nullptr)
		delete rawDecoder;
}

int CDecodeRaw::GetTypeFilter()
{
    return IDM_DECODE_RAW;
}

void CDecodeRaw::AddMetadataElement(vector<CMetadata> & element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

void CDecodeRaw::Filter(CEffectParameter * effectParameter, CRegardsBitmap * source, IFiltreEffectInterface * filtreInterface)
{
    CDecodeRawParameter * decodeParameter = (CDecodeRawParameter *)effectParameter;

	if (source != nullptr)
		orientation = source->GetOrientation();

	if (rawDecoder != nullptr)
		delete rawDecoder;

	rawDecoder = new CDecodeRawPicture(CConvertUtility::ConvertToStdString(source->GetFilename()));
    
    vector<int> elementColor;
    for (auto i = 0; i < 10; i++)
        elementColor.push_back(i);

    vector<float> elementBright;
    for (float i = 1.0; i < 10.0; i+=0.1)
        elementBright.push_back(i);

    vector<float> elementchromatic;
    for (float i = 0.1; i < 4.1; i+=0.1)
        elementchromatic.push_back(i);

	vector<int> elementValue;
    for (auto i = -100; i < 101; i++)
        elementValue.push_back(i);

	vector<int> elementValuePos;
    for (auto i = 0; i < 101; i++)
        elementValuePos.push_back(i);

	vector<int> elementValueNeg;
    for (auto i = -100; i < 1; i++)
        elementValueNeg.push_back(i);

	vector<float> elementAdjustMax;
    for (float i = 0.01; i < 1.0; i+=0.01)
        elementAdjustMax.push_back(i);

	vector<float> elementlinenoise;
    for (float i = 0.000; i < 0.021; i+=0.001)
        elementlinenoise.push_back(i);

	vector<float> elementcclean;
    for (float i = 0.005; i < 0.055; i+=0.005)
        elementcclean.push_back(i);

	vector<float> elementgreentresh;
    for (float i = 0.01; i < 0.1; i+=0.01)
        elementgreentresh.push_back(i);

	vector<float> elementexp_shift;
    for (float i = 0.25; i < 8.0; i+=0.25)
        elementexp_shift.push_back(i);
	
	vector<float> elementexp_preser;
    for (float i = 0.1; i < 1.0; i+=0.1)
        elementexp_preser.push_back(i);

	vector<CMetadata> elementNoiseReduction;
	AddMetadataElement(elementNoiseReduction,"do not use",0);
	AddMetadataElement(elementNoiseReduction,"light FBDD reduction",1);
	AddMetadataElement(elementNoiseReduction,"full FBDD reduction",2);

	vector<CMetadata> elementFlip;
	AddMetadataElement(elementFlip,"none",0);
	AddMetadataElement(elementFlip,"Flip Horizontal",1);
	AddMetadataElement(elementFlip,"Flip Vertical",2);
	AddMetadataElement(elementFlip,"180",3);
	AddMetadataElement(elementFlip,"270",4);
	AddMetadataElement(elementFlip,"90 CCW",5);
	AddMetadataElement(elementFlip,"90 CW",6);


	vector<CMetadata> elementInterpolation;
	AddMetadataElement(elementInterpolation,"linear interpolation",0);
	AddMetadataElement(elementInterpolation,"VNG interpolation",1);
	AddMetadataElement(elementInterpolation,"PPG interpolation",2);
	AddMetadataElement(elementInterpolation,"AHD interpolation",3);
	AddMetadataElement(elementInterpolation,"DCB interpolation",4);
	AddMetadataElement(elementInterpolation,"Modified AHD interpolation by Paul Lee",5);
	AddMetadataElement(elementInterpolation,"AFD interpolation (5-pass)",6);
	AddMetadataElement(elementInterpolation,"VCD interpolation",7);
	AddMetadataElement(elementInterpolation,"Mixed VCD/Modified AHD interpolation",8);
	AddMetadataElement(elementInterpolation,"LMMSE interpolation",9);
	AddMetadataElement(elementInterpolation,"AMaZE intepolation",10);
    
    filtreInterface->AddTreeInfos(libellebright, new CTreeElementValueFloat(decodeParameter->bright,1), &elementBright,4);
    filtreInterface->AddTreeInfos(libellehighlight, new CTreeElementValueInt(decodeParameter->highlight), &elementColor);
	filtreInterface->AddTreeInfos(libellethreshold, new CTreeElementValueInt(decodeParameter->threshold), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_auto_wb, new CTreeElementValueInt(decodeParameter->use_auto_wb), &decodeParameter->use_auto_wb, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_camera_wb, new CTreeElementValueInt(decodeParameter->use_camera_wb), &decodeParameter->use_camera_wb, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_halfsize, new CTreeElementValueInt(decodeParameter->half_size), &decodeParameter->half_size, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_cameramatrix, new CTreeElementValueInt(decodeParameter->use_camera_matrix), &decodeParameter->use_camera_matrix, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_aberRedEnable, new CTreeElementValueInt(decodeParameter->aberRedEnable), &decodeParameter->aberRedEnable, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_aberRed, new CTreeElementValueInt(decodeParameter->aberRed), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_aberGreenEnable, new CTreeElementValueInt(decodeParameter->aberGreenEnable), &decodeParameter->aberGreenEnable, 2, 2);
	filtreInterface->AddTreeInfos(libelleuse_aberGreen, new CTreeElementValueInt(decodeParameter->aberGreen), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserMultiRed, new CTreeElementValueInt(decodeParameter->multiRed), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserMultiGreen, new CTreeElementValueInt(decodeParameter->multiGreen), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserMultiBlue, new CTreeElementValueInt(decodeParameter->multiBlue), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserMultiOther, new CTreeElementValueInt(decodeParameter->multiOther), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserFlip, new CTreeElementValueInt(decodeParameter->flip), &elementFlip, 3, 3);
	filtreInterface->AddTreeInfos(libelleuse_UserInterpolation, new CTreeElementValueInt(decodeParameter->interpolation), &elementInterpolation, 3, 3);
	filtreInterface->AddTreeInfos(libelleuse_UserBlack, new CTreeElementValueInt(decodeParameter->black), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserBlackChannelRed, new CTreeElementValueInt(decodeParameter->blackchannelRed), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserBlackChannelGreen, new CTreeElementValueInt(decodeParameter->blackchannelGreen), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserBlackChannelBlue, new CTreeElementValueInt(decodeParameter->blackchannelBlue), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserBlackChannelOther, new CTreeElementValueInt(decodeParameter->blackchannelOther), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_UserSaturation, new CTreeElementValueInt(decodeParameter->saturation), &elementValueNeg);
	filtreInterface->AddTreeInfos(libelleuse_med_passes, new CTreeElementValueInt(decodeParameter->medPasses), &elementValue);
	filtreInterface->AddTreeInfos(libelleuse_noautobright, new CTreeElementValueInt(decodeParameter->noautobright), &decodeParameter->noautobright, 2, 2);
	filtreInterface->AddTreeInfos(libelleauto_bright_thr, new CTreeElementValueInt(decodeParameter->autobright), &elementValue);
	filtreInterface->AddTreeInfos(libelleadjust_maximum_thr, new CTreeElementValueFloat(decodeParameter->adjust_maximum_thr,2), &elementAdjustMax,4);
	filtreInterface->AddTreeInfos(libelleuse_fuji_rotate, new CTreeElementValueInt(decodeParameter->use_fuji_rotate), &decodeParameter->use_fuji_rotate,2,2);
	filtreInterface->AddTreeInfos(libellegreen_matching, new CTreeElementValueInt(decodeParameter->green_matching), &decodeParameter->green_matching,2,2);
	filtreInterface->AddTreeInfos(libelledcb_iterations, new CTreeElementValueInt(decodeParameter->dcb_iterations), &elementValue);
	filtreInterface->AddTreeInfos(libelledcb_enhance_fl, new CTreeElementValueInt(decodeParameter->dcb_enhance_fl), &elementValue);
	filtreInterface->AddTreeInfos(libellefbdd_noiserd, new CTreeElementValueInt(decodeParameter->fbdd_noiserd), &elementNoiseReduction, 3, 3);
	filtreInterface->AddTreeInfos(libelleeeci_refine, new CTreeElementValueInt(decodeParameter->eeci_refine), &elementValue);
	filtreInterface->AddTreeInfos(libellees_med_passes, new CTreeElementValueInt(decodeParameter->medPasses), &elementValue);
	filtreInterface->AddTreeInfos(libelleca_correc, new CTreeElementValueInt(decodeParameter->ca_correc), &decodeParameter->ca_correc,2,2);
	filtreInterface->AddTreeInfos(libellecared, new CTreeElementValueFloat(decodeParameter->cared,1), &elementchromatic,4);
	filtreInterface->AddTreeInfos(libellecablue, new CTreeElementValueFloat(decodeParameter->cablue,1), &elementchromatic,4);
	filtreInterface->AddTreeInfos(libellecfaline, new CTreeElementValueInt(decodeParameter->cfaline), &decodeParameter->cfaline,2,2);
	filtreInterface->AddTreeInfos(libellelinenoise, new CTreeElementValueFloat(decodeParameter->linenoise,3), &elementlinenoise, 4);
	filtreInterface->AddTreeInfos(libellecfa_clean, new CTreeElementValueInt(decodeParameter->cfa_clean), &decodeParameter->cfa_clean,2,2);
	filtreInterface->AddTreeInfos(libellelclean, new CTreeElementValueFloat(decodeParameter->lclean,3), &elementcclean,4);
	filtreInterface->AddTreeInfos(libellecclean, new CTreeElementValueFloat(decodeParameter->cclean,3), &elementcclean,4);
	filtreInterface->AddTreeInfos(libellecfa_green, new CTreeElementValueInt(decodeParameter->cfa_green), &decodeParameter->cfa_green,2,2);
	filtreInterface->AddTreeInfos(libellegreen_thresh, new CTreeElementValueFloat(decodeParameter->green_thresh,2), &elementgreentresh,4);
	filtreInterface->AddTreeInfos(libelleexp_correc, new CTreeElementValueInt(decodeParameter->exp_correc), &decodeParameter->exp_correc,2,2);
	filtreInterface->AddTreeInfos(libelleexp_shift, new CTreeElementValueFloat(decodeParameter->exp_shift,2), &elementexp_shift,4);
	filtreInterface->AddTreeInfos(libelleexp_preser, new CTreeElementValueFloat(decodeParameter->exp_preser,1), &elementexp_preser,4);
	filtreInterface->AddTreeInfos(libellewf_debanding, new CTreeElementValueInt(decodeParameter->wf_debanding), &decodeParameter->wf_debanding,2,2);
	filtreInterface->AddTreeInfos(libellewf_deband_tresholdRed, new CTreeElementValueInt(decodeParameter->wf_deband_tresholdRed), &elementValue);
	filtreInterface->AddTreeInfos(libellewf_deband_tresholdGreen, new CTreeElementValueInt(decodeParameter->wf_deband_tresholdGreen), &elementValue);
	filtreInterface->AddTreeInfos(libellewf_deband_tresholdBlue, new CTreeElementValueInt(decodeParameter->wf_deband_tresholdBlue), &elementValue);
	filtreInterface->AddTreeInfos(libellewf_deband_tresholdOther, new CTreeElementValueInt(decodeParameter->wf_deband_tresholdOther), &elementValue);
}

void CDecodeRaw::FilterChangeParam(CEffectParameter * effectParameter,  CTreeElementValue * valueData, const wxString &key)
{
    CDecodeRawParameter * decodeParameter = (CDecodeRawParameter *)effectParameter;
    
    //Video Parameter
    if (key == libellebright)
    {
		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();
        decodeParameter->bright = value;
    }
    else if (key == libellehighlight)
    {
		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->highlight = value;
    }
    else if (key == libellethreshold)
    {
		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->threshold = value / 10.0;
    }
    else if (key == libelleuse_auto_wb)
    {
		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->use_auto_wb = value;
    }
    else if (key == libelleuse_camera_wb)
    {		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->use_camera_wb = value;
    }
    else if (key == libelleuse_halfsize)
    {		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->half_size = value;
    }
    else if (key == libelleuse_cameramatrix)
    {		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
        decodeParameter->use_camera_matrix = value;
    }
	else if (key == libelleuse_aberRedEnable)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->aberRedEnable = value;
	}
	else if (key == libelleuse_aberRed)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->aberRed = value;
	}
	else if (key == libelleuse_aberGreenEnable)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->aberGreenEnable = value;
	}
	else if (key == libelleuse_aberGreen)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->aberGreen = value;
	}
	else if (key == libelleuse_UserMultiRed)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->multiRed = value;
	}
	else if (key == libelleuse_UserMultiGreen)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->multiGreen = value;
	}
	else if (key == libelleuse_UserMultiBlue)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->multiBlue = value;
	}
	else if (key == libelleuse_UserMultiOther)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->multiOther = value;
	}
	else if (key == libelleuse_UserFlip)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->flip = value;
	}
	else if (key == libelleuse_UserInterpolation)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->interpolation = value;
	}
	else if (key == libelleuse_UserBlack)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->black = value;
	}
	else if (key == libelleuse_UserBlackChannelRed)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->blackchannelRed = value;
	}
	else if (key == libelleuse_UserBlackChannelGreen)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->blackchannelGreen = value;
	}
	else if (key == libelleuse_UserBlackChannelBlue)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->blackchannelBlue = value;
	}
	else if (key == libelleuse_UserBlackChannelOther)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->blackchannelOther = value;
	}
	else if (key == libelleuse_UserSaturation)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->saturation = value;
	}
	else if (key == libelleuse_med_passes)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->medPasses = value;
	}
	else if (key == libelleuse_noautobright)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();
		decodeParameter->noautobright = value;
	}
	else if (key == libelleauto_bright_thr)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->autobright= value;}
	else if (key == libelleadjust_maximum_thr)
	{		CTreeElementValueFloat * valueFloat = (CTreeElementValueFloat *)valueData;
		float value = valueFloat->GetValue();decodeParameter->adjust_maximum_thr= value;}
	else if (key == libelleuse_fuji_rotate)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->use_fuji_rotate= value;}
	else if (key == libellegreen_matching)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->green_matching= value;}
	else if (key == libelledcb_iterations)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->dcb_iterations= value;}
	else if (key == libelledcb_enhance_fl)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->dcb_enhance_fl= value;}
	else if (key == libellefbdd_noiserd)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->fbdd_noiserd= value;}
	else if (key == libelleeeci_refine)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->eeci_refine= value;}
	else if (key == libellees_med_passes)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->medPasses= value;}
	else if (key == libelleca_correc)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->ca_correc= value;}
	else if (key == libellecared)
	{		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();decodeParameter->cared= value;}
	else if (key == libellecablue)
	{		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();decodeParameter->cablue= value;}
	else if (key == libellecfaline)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->cfaline= value;}
	else if (key == libellelinenoise)
	{		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();decodeParameter->linenoise= value;}
	else if (key == libellecfa_clean)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->cfa_clean= value;}
	else if (key == libellelclean)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->lclean= value;}
	else if (key == libellecclean)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->cclean= value;}
	else if (key == libellecfa_green)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->cfa_green= value;}
	else if (key == libellegreen_thresh)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->green_thresh= value;}
	else if (key == libelleexp_correc)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->exp_correc= value;}
	else if (key == libelleexp_shift)
	{		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();decodeParameter->exp_shift= value;}
	else if (key == libelleexp_preser)
	{		CTreeElementValueFloat * valueInt = (CTreeElementValueFloat *)valueData;
		float value = valueInt->GetValue();decodeParameter->exp_preser= value;}
	else if (key == libellewf_debanding)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->wf_debanding= value;}
	else if (key == libellewf_deband_tresholdRed)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->wf_deband_tresholdRed= value;}
	else if (key == libellewf_deband_tresholdGreen)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->wf_deband_tresholdGreen= value;}
	else if (key == libellewf_deband_tresholdBlue)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->wf_deband_tresholdBlue= value;}
	else if (key == libellewf_deband_tresholdOther)
	{		CTreeElementValueInt * valueInt = (CTreeElementValueInt *)valueData;
		int value = valueInt->GetValue();decodeParameter->wf_deband_tresholdOther= value;}
	decodeParameter->update = true;
}

CImageLoadingFormat * CDecodeRaw::ApplyEffect(CEffectParameter * effectParameter, CBitmapWndViewer * bitmapViewer)
{
	if (effectParameter == nullptr || bitmapViewer == nullptr || rawDecoder == nullptr)
		return nullptr;

	CDecodeRawParameter * rawParameter = (CDecodeRawParameter *)effectParameter;

	return rawDecoder->DecodePicture(rawParameter);
}

void CDecodeRaw::ApplyPreviewEffect(CEffectParameter * effectParameter, Regards::Control::CBitmapWndViewer * bitmapViewer, CFiltreEffet * filtreEffet, CDraw * m_cDessin, int & widthOutput, int & heightOutput)
{
	if (effectParameter == nullptr || bitmapViewer == nullptr || rawDecoder == nullptr)
		return;

	wxString filename = bitmapViewer->GetFilename();

	CDecodeRawParameter * rawParameter = (CDecodeRawParameter *)effectParameter;

	if (rawParameter->update)
	{
		CImageLoadingFormat * imageLoad = rawDecoder->DecodePicture(rawParameter);
		if (imageLoad != nullptr)
		{
			imageLoad->SetOrientation(4);
			if (firstUpdate)
			{
				bitmapViewer->UpdateBitmap(imageLoad, true);
				firstUpdate = false;
			}
			else
			{
				bitmapViewer->UpdateBitmap(imageLoad);
			}
			//int orientation = bitmapViewer->GetOrientation();
			//imageLoad->ApplyExifOrientation(orientation);
			//imageLoad->SetOrientation(4);
			//wxCommandEvent eventChange(wxEVENT_UPDATEBITMAP);
			//eventChange.SetClientData(imageLoad);
			//bitmapViewer->GetEventHandler()->AddPendingEvent(eventChange);
		}
		rawParameter->update = false;
	}

}