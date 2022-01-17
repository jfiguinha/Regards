#include <header.h>
#include "regards_webp.h"
#include "RegardsBitmap.h"
#include <webp/decode.h>
#include <webp/encode.h>
#include "picture_utility.h"
#include <webp/demux.h>

class CRegardsWebpImpl
{
public:


    static int IsWebP(const WebPData* const webp_data);

};


CRegardsBitmap* CRegardsWebp::GetPicture(const wxString& filename)
{
	CRegardsBitmap * picture = nullptr;
	size_t data_size;
	uint8_t* _compressedImage = CPictureUtility::readfile(filename, data_size);
	if (_compressedImage != nullptr && data_size > 0)
	{
		picture = new CRegardsBitmap();
		int width = 0, height = 0;
		uint8_t* data = WebPDecodeBGRA(_compressedImage, data_size, &width, &height);
        cv::Mat bitmapMatrix = cv::Mat(height, width, CV_8UC4, data);
        picture->SetMatrix(bitmapMatrix);
		delete[] _compressedImage;
	}
	return picture;
}

vector<CRegardsBitmap*> CRegardsWebp::GetAllPicture(const wxString& filename, int& delay)
{
    vector<CRegardsBitmap*> pictureList;
    int prev_frame_timestamp = 0;
    WebPAnimDecoder* dec;
    WebPAnimInfo anim_info;
    WebPData webp_data;
    WebPDataInit(&webp_data);
    int nbFrame = 0;

    webp_data.bytes = CPictureUtility::readfile(filename, webp_data.size);

    if (CRegardsWebpImpl::IsWebP(&webp_data)) {

        dec = WebPAnimDecoderNew(&webp_data, NULL);
        if (dec == NULL) {
            goto End;
        }

        if (!WebPAnimDecoderGetInfo(dec, &anim_info)) {
            fprintf(stderr, "Error getting global info about the animation\n");
            goto End;
        }

        nbFrame = anim_info.frame_count;

        // Decode frames.
        while (WebPAnimDecoderHasMoreFrames(dec)) {
            CRegardsBitmap* curr_bitmap = new CRegardsBitmap();
            uint8_t* frame_rgba;
            int timestamp;

            if (!WebPAnimDecoderGetNext(dec, &frame_rgba, &timestamp)) {
                goto End;
            }
  
            delay = timestamp - prev_frame_timestamp;

            cv::Mat bitmapMatrix = cv::Mat(anim_info.canvas_height, anim_info.canvas_width, CV_8UC4, frame_rgba);
            curr_bitmap->SetMatrix(bitmapMatrix, true);
           // curr_bitmap->SetBitmap(frame_rgba, anim_info.canvas_width, anim_info.canvas_height, true, true);
            curr_bitmap->ConvertToBgr();
            curr_bitmap->SetFilename(filename);
            prev_frame_timestamp = timestamp;

            pictureList.push_back(curr_bitmap);
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
	
    if (CRegardsWebpImpl::IsWebP(&webp_data)) {

        dec = WebPAnimDecoderNew(&webp_data, NULL);
        if (dec == NULL) {
            goto End;
        }

        if (!WebPAnimDecoderGetInfo(dec, &anim_info)) {
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
int CRegardsWebpImpl::IsWebP(const WebPData* const webp_data) {
    return (WebPGetInfo(webp_data->bytes, webp_data->size, NULL, NULL) != 0);
}


void CRegardsWebp::SavePicture(const wxString& fileName, CRegardsBitmap* regards, const int& _option)
{
	uint8_t* output = nullptr;

	size_t size = WebPEncodeBGRA(regards->GetPtBitmap(),
		regards->GetBitmapWidth(), regards->GetBitmapHeight(), regards->GetWidthSize(),
		_option, &output);
	CPictureUtility::writefile(fileName, output, size);

	free(output);
}

void CRegardsWebp::GetPictureDimension(const wxString & filename, int& width, int& height)
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
