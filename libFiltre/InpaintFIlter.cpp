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
#include <effect_id.h>
using namespace Regards::Filter;

CInpaintFilter::CInpaintFilter()
{
}

CInpaintFilter::~CInpaintFilter()
{
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
    CFiltreEffet* filter = bitmapViewer->GetFiltreEffet();
	bitmapViewer->GetDessinPt()->GetPos(rcZoom);
	if (!source.empty())
	{
		imageLoad = new CImageLoadingFormat();
		imageLoad->SetPicture(source);
		//imageLoad->Flip();
		imageLoad->RotateExif(orientation);

		try
		{
			cv::Mat matrix = imageLoad->GetOpenCVPicture();
			cv::Rect rect;
			rect.x = rcZoom.x;
			rect.y = rcZoom.y;
			rect.width = rcZoom.width;
			rect.height = rcZoom.height;
            
            cv::Mat mask = GenerateMaskFromZone(rect, matrix);
            cv::imwrite("mask.png", mask);
            
            out = filter->Inpaint(mask);
            
			//out = matrix(rect);
            
            //Generate Mask
            
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
			std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
		}


		imageLoad->SetPicture(out);
		//imageLoad->Flip();
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
    cv::drawContours(mask,contours,0,cv::Scalar(255),-1);

    return mask;
 }
