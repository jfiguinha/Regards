#include <header.h>
#include "regards_webp.h"
#include <webp/decode.h>
#include <webp/encode.h>
#include "picture_utility.h"
#include <webp/demux.h>

class CRegardsWebpImpl
{
public:
	static int IsWebP(const WebPData* webp_data);
};


cv::Mat CRegardsWebp::GetPicture(const wxString& filename)
{
	cv::Mat mat;
	size_t data_size;
	uint8_t* _compressedImage = CPictureUtility::readfile(filename, data_size);
	if (_compressedImage != nullptr && data_size > 0)
	{
		int width = 0, height = 0;
		uint8_t* data = WebPDecodeBGRA(_compressedImage, data_size, &width, &height);
		mat = cv::Mat(height, width, CV_8UC4, data);
		delete[] _compressedImage;
	}
	return mat;
}

void CRegardsWebp::ApplyTransform(cv::Mat& image)
{
	if (image.channels() == 3)
		cvtColor(image, image, cv::COLOR_BGR2BGRA);
	else if (image.channels() == 1)
		cvtColor(image, image, cv::COLOR_GRAY2BGRA);

	cvtColor(image, image, cv::COLOR_RGBA2BGRA);

	//cv::flip(image, image, 0);
}

vector<cv::Mat> CRegardsWebp::GetAllPicture(const wxString& filename, int& delay)
{
	vector<cv::Mat> pictureList;
	int prev_frame_timestamp = 0;
	WebPAnimDecoder* dec;
	WebPAnimInfo anim_info;
	WebPData webp_data;
	WebPDataInit(&webp_data);
	int nbFrame = 0;

	webp_data.bytes = CPictureUtility::readfile(filename, webp_data.size);

	if (CRegardsWebpImpl::IsWebP(&webp_data))
	{
		dec = WebPAnimDecoderNew(&webp_data, nullptr);
		if (dec == nullptr)
		{
			goto End;
		}

		if (!WebPAnimDecoderGetInfo(dec, &anim_info))
		{
			fprintf(stderr, "Error getting global info about the animation\n");
			goto End;
		}

		nbFrame = anim_info.frame_count;

		// Decode frames.
		while (WebPAnimDecoderHasMoreFrames(dec))
		{
			uint8_t* frame_rgba;
			int timestamp;

			if (!WebPAnimDecoderGetNext(dec, &frame_rgba, &timestamp))
			{
				goto End;
			}

			delay = timestamp - prev_frame_timestamp;

			auto bitmapMatrix = cv::Mat(anim_info.canvas_height, anim_info.canvas_width, CV_8UC4, frame_rgba);
			ApplyTransform(bitmapMatrix);
			prev_frame_timestamp = timestamp;
			pictureList.push_back(bitmapMatrix);
		}
	}

End:
	WebPAnimDecoderDelete(dec);

	return pictureList;
}

int CRegardsWebp::GetNbFrame(const wxString& filename)
{
	WebPAnimDecoder* dec;
	WebPAnimInfo anim_info;
	WebPData webp_data;
	WebPDataInit(&webp_data);
	int nbFrame = 0;

	webp_data.bytes = CPictureUtility::readfile(filename, webp_data.size);

	if (CRegardsWebpImpl::IsWebP(&webp_data))
	{
		dec = WebPAnimDecoderNew(&webp_data, nullptr);
		if (dec == nullptr)
		{
			goto End;
		}

		if (!WebPAnimDecoderGetInfo(dec, &anim_info))
		{
			fprintf(stderr, "Error getting global info about the animation\n");
			goto End;
		}

		nbFrame = anim_info.frame_count;
	}

End:
	WebPAnimDecoderDelete(dec);
	return nbFrame;
}

// Returns true if this is a valid WebP bitstream.
int CRegardsWebpImpl::IsWebP(const WebPData* const webp_data)
{
	return (WebPGetInfo(webp_data->bytes, webp_data->size, nullptr, nullptr) != 0);
}


void CRegardsWebp::SavePicture(const wxString& fileName, cv::Mat& source, const int& _option)
{
	uint8_t* output = nullptr;

	size_t size = WebPEncodeBGRA(source.data,
	                             source.size().width, source.size().height, source.size().width * 4,
	                             _option, &output);
	CPictureUtility::writefile(fileName, output, size);

	free(output);
}

void CRegardsWebp::GetPictureDimension(const wxString& filename, int& width, int& height)
{
	size_t data_size;
	uint8_t* data = CPictureUtility::readfile(filename, data_size);
	if (data != nullptr && data_size > 0)
	{
		//int result = 0;
		WebPGetInfo(data, data_size, &width, &height);
		delete[] data;
	}
}
