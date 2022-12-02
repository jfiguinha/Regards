#include "header.h"
#include "LoadingResource.h"

#include <LibResource.h>
#include <PictureData.h>
#include <ConvertUtility.h>

cv::Mat CLoadingResource::LoadRegardsBmpResource(const wxString& resourceName)
{
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
	return cv::imread(CConvertUtility::ConvertToStdString(resourcePath));
}

wxImage CLoadingResource::LoadImageResource(const wxString& resourceName)
{
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
	wxImage out;
	out.LoadFile(resourcePath, wxBITMAP_TYPE_PNG);
	return out;
}

wxImage CLoadingResource::ConvertTowxImageRGB(cv::Mat & mat)
{
	int width = mat.size().width;
	int height = mat.size().height;
	//int widthSrcSize = width * 4;
	unsigned char* data = mat.data;
	//int posData = 0;
	//int posDataOut = 0;
	//int posAlpha = 0;
	wxImage anImage(width, height, false);
	unsigned char* dataOut = anImage.GetData();
	int size = width * height;
	//bitmap->VertFlipBuf();

	if (data != nullptr)
	{
		tbb::parallel_for(0, size, 1, [=](int i)
			{
					int y = i / width;
					int x = i - (y * width);
					int positionInput = ((height - y - 1) * width + x) << 2;
					int position = i * 3;

					dataOut[position] = data[positionInput + 2];
					dataOut[position + 1] = data[positionInput + 1];
					dataOut[position + 2] = data[positionInput];
			});
	}

	return anImage;
}
