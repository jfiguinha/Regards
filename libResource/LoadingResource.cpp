#include "header.h"
#include "LoadingResource.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
#include <PictureData.h>
#include <ConvertUtility.h>

CRegardsBitmap* CLoadingResource::LoadRegardsBmpResource(const wxString& resourceName)
{
	auto bitmap = new CRegardsBitmap();
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
	cv::Mat data = cv::imread(CConvertUtility::ConvertToUTF8(resourcePath));
	bitmap->SetMatrix(data);
	return bitmap;
}

wxImage CLoadingResource::LoadImageResource(const wxString& resourceName)
{
	wxString resourcePath = CLibResource::LoadBitmapFromResource(resourceName);
	wxImage out;
	out.LoadFile(resourcePath, wxBITMAP_TYPE_PNG);
	return out;
}

wxImage CLoadingResource::ConvertTowxImageRGB(CRegardsBitmap* bitmap)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	//int widthSrcSize = width * 4;
	unsigned char* data = bitmap->GetPtBitmap();
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
