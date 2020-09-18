#include <header.h>
#include "raw.h"
#include "RegardsRaw.h"
#include <ximage.h>
#include <ImageLoadingFormat.h>
#ifdef TURBOJPEG
#include <turbojpeg.h>
#endif
#include <ConvertUtility.h>
#include <RegardsBitmap.h>

CImageLoadingFormat * CRaw::GetThumbnail(const wxString & fileName, const bool &thumbnail)
{
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
    CImageLoadingFormat * picture = nullptr;
    int type = 0;
    CxMemFile * memFile = CRegardsRaw::GetThumbnail(CConvertUtility::ConvertToStdString(fileName), type);
	if (memFile != nullptr)
	{
		if (type == JPEGOUTPUT)
		{
			picture = new CImageLoadingFormat();
#ifdef TURBOJPEG

			long unsigned int _jpegSize; //!< _jpegSize from above
			//unsigned char* _compressedImage; //!< _compressedImage from above
			CRegardsBitmap * image;
			int jpegSubsamp, width, height;
			uint8_t * _compressedImage = memFile->GetBuffer(false);
			_jpegSize = memFile->Size();

			tjhandle _jpegDecompressor = tjInitDecompress();

			tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);

			image = new CRegardsBitmap(width, height);

			//if (thumbnail)
			//	tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, image->GetPtBitmap(), width, 0, height, TJPF_RGBX, TJFLAG_FASTDCT);
			//else
				tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, image->GetPtBitmap(), width, 0, height, TJPF_BGRX, TJFLAG_FASTDCT | TJFLAG_BOTTOMUP);

			tjDestroy(_jpegDecompressor);

			picture->SetPicture(image);
			picture->SetFilename(fileName);
#else
			CxImage * image = new CxImage(memFile, CxImage::GetTypeIdFromName("jpg"));
			picture->SetPicture(image);
			picture->SetFilename(fileName);
#endif
		}
		else
		{
			picture = new CImageLoadingFormat();
			CxImage * image = new CxImage(memFile, CxImage::GetTypeIdFromName("ppm"));
			picture->SetPicture(image);
			picture->SetFilename(fileName);
		}

		delete memFile;
	}
	else
	{
		picture = new CImageLoadingFormat();
		CxImage * image = CRegardsRaw::GetPicture(CConvertUtility::ConvertToStdString(fileName));
		picture->SetPicture(image);
		picture->SetFilename(fileName);
		if (thumbnail)
		{
			picture->ConvertToBGR(true);
		}
	}

        
    return picture;
}


void CRaw::GetDimensions(const wxString & fileName, int & width, int & height)
{
    //const char * fichier = CConvertUtility::ConvertFromwxString(fileName);
	CRegardsRaw::GetDimensions(CConvertUtility::ConvertToStdString(fileName), width, height);
}

