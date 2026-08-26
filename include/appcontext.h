#pragma once

// Simple logging helpers to unify output (uses std::cout / std::cerr as requested)
static void LogInfo(const wxString& msg)
{
	std::cout << msg.utf8_string() << std::endl;
}

static void LogError(const wxString& msg)
{
	std::cerr << msg.utf8_string() << std::endl;
}

class AppContext
{
public:

	AppContext()
	{
		for (auto i = 0; i < 256; i++)
			value[i] = static_cast<float>(i);


		//Loading animation 

	}


	int GetExifFromAngleAndFlip(const int& angle, const int& flipH, const int& flipV)
	{
		if (angle == 0 && flipH == 0 && flipV == 0)
			return 0;
		if (angle == 0 && flipH == 1 && flipV == 0)
			return 1;
		if (angle == 0 && flipH == 0 && flipV == 1)
			return 2;
		if (angle == 0 && flipH == 1 && flipV == 1)
			return 3;

		if (angle == 90 && flipH == 0 && flipV == 0)
			return 4;
		if (angle == 90 && flipH == 1 && flipV == 0)
			return 5;
		if (angle == 90 && flipH == 0 && flipV == 1)
			return 6;
		if (angle == 90 && flipH == 1 && flipV == 1)
			return 7;

		if (angle == 180 && flipH == 0 && flipV == 0)
			return 8;
		if (angle == 180 && flipH == 1 && flipV == 0)
			return 9;
		if (angle == 180 && flipH == 0 && flipV == 1)
			return 10;
		if (angle == 180 && flipH == 1 && flipV == 1)
			return 11;

		if (angle == 270 && flipH == 0 && flipV == 0)
			return 12;
		if (angle == 270 && flipH == 1 && flipV == 0)
			return 13;
		if (angle == 270 && flipH == 0 && flipV == 1)
			return 14;
		if (angle == 270 && flipH == 1 && flipV == 1)
			return 15;


		return 0;
	}

	void GetAngleAndFlip(const int64_t& exif, int& angle, int& flipH, int& flipV)
	{
		switch (exif)
		{
		case 0:
			angle = 0;
			flipH = 0;
			flipV = 0;
			break;
		case 1:
			angle = 0;
			flipH = 1;
			flipV = 0;
			break;
		case 2:
			angle = 0;
			flipH = 0;
			flipV = 1;
			break;
		case 3:
			angle = 0;
			flipH = 1;
			flipV = 1;
			break;
		case 4:
			angle = 90;
			flipH = 0;
			flipV = 0;
			break;
		case 5:
			angle = 90;
			flipH = 1;
			flipV = 0;
			break;
		case 6:
			angle = 90;
			flipH = 0;
			flipV = 1;
			break;
		case 7:
			angle = 90;
			flipH = 1;
			flipV = 1;
			break;
		case 8:
			angle = 180;
			flipH = 0;
			flipV = 0;
			break;
		case 9:
			angle = 180;
			flipH = 1;
			flipV = 0;
			break;
		case 10:
			angle = 180;
			flipH = 0;
			flipV = 1;
			break;
		case 11:
			angle = 180;
			flipH = 1;
			flipV = 1;
			break;
		case 12:
			angle = 270;
			flipH = 0;
			flipV = 0;
			break;
		case 13:
			angle = 270;
			flipH = 1;
			flipV = 0;
			break;
		case 14:
			angle = 270;
			flipH = 0;
			flipV = 1;
			break;
		case 15:
			angle = 270;
			flipH = 1;
			flipV = 1;
			break;
		}
	}


	cv::Mat WxToCvMat(const wxImage& wx)
	{
		cv::Mat im2(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC3, wx.GetData());
		if (wx.HasAlpha())
		{
			std::vector<cv::Mat> ch;
			cv::split(im2, ch);
			cv::Mat alpha(cv::Size(wx.GetWidth(), wx.GetHeight()), CV_8UC1, wx.GetAlpha());
			ch.push_back(alpha);
			cv::merge(ch, im2);
			cvtColor(im2, im2, cv::COLOR_RGBA2BGRA);
		}
		else
		{
			cvtColor(im2, im2, cv::COLOR_RGB2BGRA);
		}
		return im2;
	}

	void LoadWxDefaultPicture(const wxString& fileName)
	{
		defaultPicture = wxImage(fileName);
		defaultPictureMat = WxToCvMat(defaultPicture);
	}

	void SetWxDefaultPictureThumbnail(wxImage picture)
	{
		defaultPictureThumbnailPicture = picture;
		defaultPictureMatThumbnailPicture = WxToCvMat(defaultPictureThumbnailPicture);
	}

	void SetWxDefaultPictureThumbnailVideo(wxImage picture)
	{
		defaultPictureThumbnailVideo = picture;
		defaultPictureMatThumbnailVideo = WxToCvMat(defaultPictureThumbnailVideo);
	}

	wxImage GetWxDefaultPicture()
	{
		if (defaultPicture.IsOk())
			return defaultPicture;
		else
		{
			return wxImage();
		}
	}

	cv::Mat GetDefaultPicture()
	{
		if (defaultPictureMat.empty())
			defaultPictureMat = WxToCvMat(defaultPicture);

		if (defaultPictureMat.empty())
		{
			return cv::Mat();
		}
		return defaultPictureMat;
	}


	wxImage GetWxDefaultPictureThumbnail()
	{
		if (defaultPictureThumbnailPicture.IsOk())
			return defaultPictureThumbnailPicture;
		else
		{
			return wxImage();
		}
	}

	cv::Mat GetDefaultPictureThumbnail()
	{
		if (defaultPictureMatThumbnailPicture.empty())
			defaultPictureMatThumbnailPicture = WxToCvMat(defaultPictureThumbnailPicture);

		if (defaultPictureMatThumbnailPicture.empty())
		{
			return cv::Mat();
		}
		return defaultPictureMatThumbnailPicture;
	}

	wxImage GetWxDefaultVideoThumbnail()
	{
		if (defaultPictureThumbnailVideo.IsOk())
			return defaultPictureThumbnailVideo;
		else
		{
			return wxImage();
		}
	}

	cv::Mat GetDefaultVideoThumbnail()
	{
		if (defaultPictureMatThumbnailVideo.empty())
			defaultPictureMatThumbnailVideo = WxToCvMat(defaultPictureThumbnailVideo);

		if (defaultPictureMatThumbnailVideo.empty())
		{
			return cv::Mat();
		}
		return defaultPictureMatThumbnailVideo;
	}

	float clamp(float val, float minval, float maxval)
	{
		return std::max(minval, std::min(val, maxval));
	}

	int openclOpenGLInterop = 0;
	string platformName = "";
	bool isOpenCLInitialized = false;
	bool firstElementToShow = true;
	int numElementToLoad = 5;
	string buildOption = "-cl-mad-enable -cl-unsafe-math-optimizations";

	
	bool isGPsAvailable = false;
	double value[256];
	wxString special_key = "map=6";
private:

	cv::Mat defaultPictureMat;
	cv::Mat defaultPictureMatThumbnailPicture;
	cv::Mat defaultPictureMatThumbnailVideo;
	wxImage defaultPicture;
	wxImage defaultPictureThumbnailPicture;
	wxImage defaultPictureThumbnailVideo;





};