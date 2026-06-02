#pragma once

class CRegardsFloatBitmap;

class CJxl
{
public:
	CJxl()
	{
	};

	virtual ~CJxl()
	{
	};
	static void GetDimensions(const wxString& path, int& width, int& height);
	static cv::Mat GetPicture(const wxString& path);
	static void GetMetadata(const wxString& filename, uint8_t*& data, unsigned int& size);
	static void WriteFile(cv::Mat& matFloat, const wxString& path);

private:
	static bool DecodeJpegXlOneShot(const uint8_t* jxl, size_t size,
	                                cv::Mat& matFloat, size_t& xsize,
	                                size_t& ysize, std::vector<uint8_t>* icc_profile);
	static void* DecodeJpegDim(FILE* file);
	static bool DecodeJpegXlExif(const uint8_t* jxl, size_t size, std::vector<uint8_t>* exif);
	static bool EncodeJxlOneshot(cv::Mat& matFloat, std::vector<uint8_t>* compressed);
};
