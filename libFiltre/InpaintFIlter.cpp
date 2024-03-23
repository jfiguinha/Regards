#include <header.h>
#include "InpaintFilter.h"
#include "EffectParameter.h"
#include <LibResource.h>
#include <FilterData.h>
#include <FiltreEffet.h>
#include <Draw.h>
#include <ImageLoadingFormat.h>
#include <BitmapDisplay.h>
#include <Crop.h>
#include "FiltreEffetCPU.h"
#include <effect_id.h>
#include "InpaintFilterParam.h"
#include <Metadata.h>
using namespace Regards::Filter;
using namespace cv;

CInpaintFilter::CInpaintFilter()
{
	libelleAlgorithm = "Effect.Algorithm";
}

CInpaintFilter::~CInpaintFilter()
{
}

void CInpaintFilter::AddMetadataElement(vector<CMetadata>& element, wxString value, int key)
{
	CMetadata linear;
	linear.value = value;
	linear.depth = key;
	element.push_back(linear);
}

CEffectParameter* CInpaintFilter::GetEffectPointer()
{
	return new CInpaintFilterParameter();
}



void CInpaintFilter::Filter(CEffectParameter* effectParameter, cv::Mat& source, const wxString& filename,
	IFiltreEffectInterface* filtreInterface)
{
	this->source = source;
	this->filename = filename;
	CInpaintFilterParameter* inpaintParam = (CInpaintFilterParameter*)effectParameter;

	vector<CMetadata> formatPicture;
	AddMetadataElement(formatPicture, "SHIFTMAP", 0);
	AddMetadataElement(formatPicture, "FSR FAST", 1);
	AddMetadataElement(formatPicture, "FSR BEST", 2);

	filtreInterface->AddTreeInfos(libelleAlgorithm, new CTreeElementValueInt(inpaintParam->algo),
		&formatPicture, 3, 3);
}

void CInpaintFilter::FilterChangeParam(CEffectParameter* effectParameter, CTreeElementValue* valueData,
	const wxString& key)
{
	auto videoEffectParameter = static_cast<CInpaintFilterParameter*>(effectParameter);

	float value = 0.0;
	switch (valueData->GetType())
	{
	case TYPE_ELEMENT_INT:
	{
		auto intValue = static_cast<CTreeElementValueInt*>(valueData);
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_FLOAT:
	{
		auto intValue = static_cast<CTreeElementValueFloat*>(valueData);
		value = intValue->GetValue();
	}
	break;
	case TYPE_ELEMENT_BOOL:
	{
		auto intValue = static_cast<CTreeElementValueBool*>(valueData);
		value = intValue->GetValue();
	}
	break;
	default:;
	}

	//Video Parameter

	if (key == libelleAlgorithm)
	{
		videoEffectParameter->algo = value;
	}
}



int CInpaintFilter::GetTypeFilter()
{
	return IDM_INPAINT;
}

int CInpaintFilter::GetNameFilter()
{
	return IDM_INPAINT;
}


wxString CInpaintFilter::GetFilterLabel()
{
	return CLibResource::LoadStringFromResource("LBLINPAINT", 1);
}

CImageLoadingFormat* CInpaintFilter::ApplyEffect(CEffectParameter* effectParameter, IBitmapDisplay* bitmapViewer)
{
	cv::Mat out;
	CImageLoadingFormat* imageLoad = nullptr;
	wxRect rcZoom;
	auto videoEffectParameter = static_cast<CInpaintFilterParameter*>(effectParameter);
	bitmapViewer->GetDessinPt()->GetPos(rcZoom);
	if (!source.empty())
	{
		CImageLoadingFormat image;
		image.SetPicture(source);
		image.RotateExif(orientation);

		CRgbaquad rgbaQuad;
		CFiltreEffetCPU filtreCPU(rgbaQuad, &image);

		try
		{
			cv::Rect rect;
			rect.x = rcZoom.x;
			rect.y = rcZoom.y;
			rect.width = rcZoom.width;
			rect.height = rcZoom.height;

			out = source(rect);
           
            cv::Mat mask = GenerateMaskFromZone(rect, source);
           // cv::imwrite("d:\\mask.png", mask);

			//cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR);
            
			filtreCPU.Inpaint(mask, videoEffectParameter->algo);
            
			imageLoad = new CImageLoadingFormat();
			cv::Mat bitmapOut = filtreCPU.GetBitmap(true);
			imageLoad->SetPicture(bitmapOut);

            
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}


	}

	return imageLoad;
}

 cv::Mat CInpaintFilter::GenerateMaskFromZone(const cv::Rect & rect, const cv::Mat & src)
 {
    vector<vector<cv::Point>> contours;
    vector<cv::Point> pts;
    pts.push_back(cv::Point(rect.x,rect.y));
    pts.push_back(cv::Point(rect.x + rect.width,rect.y));
    pts.push_back(cv::Point(rect.x + rect.width,rect.y + rect.height));
    pts.push_back(cv::Point(rect.x,rect.y + rect.height));
    contours.push_back(pts);
    
    cv::Mat mask(src.size(),CV_8UC1);
    mask = 0;

	//Rectangle
	{
		cv::Mat out = src(rect);
		cvtColor(out, out, cv::COLOR_BGR2GRAY);
		out = 255;
		out.copyTo(mask(rect));
	}

	/*
	{
		cv::Mat out = src(rect);
		cvtColor(out, out, cv::COLOR_BGR2GRAY);
		cv::threshold(out, out, 127, 255, 0);
		
		int thresh = 100;
		Mat canny_output;
		Canny(out, canny_output, thresh, thresh * 2);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
		Scalar color = Scalar(255, 255, 255);
		fillPoly(drawing, contours, color);

		for (size_t i = 0; i < contours.size(); i++)
		{
			Scalar color = Scalar(255, 255, 255);
			drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
		}
		drawing.copyTo(mask(rect));
		out.copyTo(mask(rect));
		cvtColor(mask, mask, cv::COLOR_BGR2GRAY);
	}
	*/

	/*
	{
		cv::Mat out = src(rect);
		cvtColor(out, out, cv::COLOR_BGR2GRAY);
		//cv::threshold(out, out, 127, 255, 0);
		// Threshold.
 // Set values equal to or above 220 to 0.
 // Set values below 220 to 255.
		Mat im_th;
		threshold(out, im_th, 127, 255, THRESH_BINARY_INV);

		// Floodfill from point (0, 0)
		Mat im_floodfill = im_th.clone();
		floodFill(im_floodfill, cv::Point(0, 0), Scalar(255));

		// Invert floodfilled image
		Mat im_floodfill_inv;
		bitwise_not(im_floodfill, im_floodfill_inv);

		cvtColor(im_floodfill_inv, im_floodfill_inv, COLOR_GRAY2BGR);
		im_floodfill_inv.copyTo(mask(rect));
	}
	*/

	
    //cv::drawContours(mask,contours,0,cv::Scalar(255),-1);
	cv::bitwise_not(mask, mask);


	//imwrite("d:\\mask.png" , mask);
    return mask;
 }
