#include "LoadingResource.h"
#include <RegardsBitmap.h>
#include <LibResource.h>
wxBitmap CLoadingResource::LoadBmpResource(const wxString & resourceName, const bool &loadAlpha)
{
	CRegardsBitmap * data = LoadRegardsBmpResource(resourceName);
	wxBitmap bitmap = wxBitmap(ConvertTowxImage(data, loadAlpha));
	delete data;
	return bitmap;
}

wxImage CLoadingResource::LoadImageResource(const wxString & resourceName, const bool &loadAlpha)
{
	CRegardsBitmap * data = LoadRegardsBmpResource(resourceName);
	wxImage bitmap = ConvertTowxImage(data, loadAlpha);
	delete data;
	return bitmap;
}

CRegardsBitmap * CLoadingResource::LoadRegardsBmpResource(const wxString & resourceName)
{
	CLibResource libResource;
	CPictureData * data = libResource.LoadBitmapFromResource(resourceName);
	CRegardsBitmap * bitmap = new CRegardsBitmap();
	bitmap->SetBitmap(data->data, data->width, data->height);
	delete data;
	return bitmap;
}


wxImage CLoadingResource::ConvertTowxImage(CRegardsBitmap * bitmap, const bool &loadAlpha, const bool &invertY)
{
    if(bitmap == nullptr)
        return wxImage(0,0);
    
    int width = bitmap->GetBitmapWidth();
    int height = bitmap->GetBitmapHeight();
    int widthSrcSize = width * 4;
    unsigned char * data = bitmap->GetPtBitmap();
    int posData = 0;
    int posDataOut = 0;
    int posAlpha = 0;
	 wxImage anImage = wxImage(width,height);
	 if(anImage.IsOk())
	 {
		 	 if(loadAlpha)
		 anImage.InitAlpha();
    
		if (data != nullptr)
		{

			for (int y = 0; y < height; y++)
			{
				if(invertY)
					posData = ((height - y) * widthSrcSize) - widthSrcSize;
				else
					posData = y * widthSrcSize;

				for (int x = 0; x < width; x++)
				{
					 anImage.SetRGB(x, y, data[posData + 2], data[posData + 1], data[posData]);
					// if (loadAlpha)
						//anImage.SetAlpha(x, y, data[posData + 3]);

					posData += 4;

				}
			}
		}
	 }
	 else
	 {
		 exit(1);
	 }
    
    return anImage;
}


wxImage CLoadingResource::ConvertTowxImageRGB(CRegardsBitmap * bitmap, const bool &loadAlpha)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	//int widthSrcSize = width * 4;
	unsigned char * data = bitmap->GetPtBitmap();
	//int posData = 0;
	//int posDataOut = 0;
	//int posAlpha = 0;
	wxImage anImage(width, height, false);
	unsigned char * dataOut = anImage.GetData();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (int i = 0; i < width*height; ++i)
		{
			dataOut[i * 3] = data[i << 2]; // R
			dataOut[i * 3 + 1] = data[(i << 2) + 1]; // G
			dataOut[i * 3 + 2] = data[(i << 2) + 2]; // B
		}

		/*
#pragma omp parallel for
		for (int y = 0; y < height; y++)
		{
			posData = (y * widthSrcSize) - widthSrcSize;
			posDataOut = y * (width * 3);
			posAlpha = y * width;
			for (int x = 0; x < width; x++)
			{
				memcpy(dataOut + posDataOut, data + posData, 3 * sizeof(uint8_t));
				posData += 4;
				posDataOut += 3;
			}
		}*/
	}

	return anImage;
}