#pragma once


class AppContext
{
public:

	AppContext()
	{
		for (auto i = 0; i < 256; i++)
			value[i] = static_cast<float>(i);
		


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

	void LoadWxDefaultPictureThumbnail(const wxString& fileName)
	{
		defaultPictureThumbnailPicture = wxImage(fileName);
		defaultPictureMatThumbnailPicture = WxToCvMat(defaultPictureThumbnailPicture);
	}

	void LoadWxDefaultPictureThumbnailVideo(const wxString& fileName)
	{
		defaultPictureThumbnailVideo = wxImage(fileName);
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
		if(defaultPictureMatThumbnailPicture.empty())
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
		if(defaultPictureMatThumbnailVideo.empty())
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
	string buildOption = "";//"-cl-mad-enable -cl-unsafe-math-optimizations";
	cv::ocl::OpenCLExecutionContext clExecCtx;
	std::map<wxString, cv::ocl::Program> openclBinaryMapping;
	bool isGPsAvailable = false;
	double value[256];

private:

	cv::Mat defaultPictureMat;
	cv::Mat defaultPictureMatThumbnailPicture;
	cv::Mat defaultPictureMatThumbnailVideo;
	wxImage defaultPicture;
	wxImage defaultPictureThumbnailPicture;
	wxImage defaultPictureThumbnailVideo;

	

	

};