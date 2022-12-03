#pragma once
class CRegardsBitmap;

class CLoadingResource
{
public:
	//static cv::Mat LoadRegardsBmpResource(const wxString& resourceName);
	static wxImage LoadImageResource(const wxString& resourceName);
	//static wxImage ConvertTowxImageRGB(cv::Mat & bitmap);
};
