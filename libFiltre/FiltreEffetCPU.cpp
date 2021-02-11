#include <header.h>
#include "FiltreEffetCPU.h"
#include <opencv2/core/ocl.hpp>
#include <opencv2/core.hpp>
#include <opencv2/xphoto.hpp>
#include <opencv2/imgproc.hpp>
#include "InterpolationBicubic.h"
#include "InterpolationBilinear.h"
#include "Interpolation.h"
#include "InterpolationFilters.h"
#include "PerlinNoise.h"
#include "LensFlare.h"
#include "MotionBlur.h"
#include "Filtre.h"
#include "Rotation.h"
#include "Histogramme.h"
#include "RedEye.h"
#include "GaussianBlur.h"
#include "InterpolationFilters.h"
#include "Wave.h"
#include "clahe.h"
#include <FilterData.h>
#include <ImageLoadingFormat.h>
#include <OpenCVEffect.h>
#include <hqdn3d.h>
#include <RegardsBitmap.h>
//BM3D
#include "bm3dfilter.h"

using namespace Regards::OpenCV;

extern float value[256];
using namespace Regards::FiltreEffet;

CFiltreEffetCPU::CFiltreEffetCPU(const CRgbaquad &backColor, CImageLoadingFormat * bitmap)
	: IFiltreEffet(backColor)
{
	this->backColor = backColor;
	bitmapOut = nullptr;
	pBitmap = bitmap->GetRegardsBitmap();

}

int CFiltreEffetCPU::GetWidth()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapWidth();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapWidth();
	}
	return 0;
}

int CFiltreEffetCPU::GetHeight()
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			return bitmapOut->GetBitmapHeight();
	}
	else
	{
		if (pBitmap != nullptr)
			return pBitmap->GetBitmapHeight();
	}
	return 0;
}

CRegardsBitmap * CFiltreEffetCPU::GetPtBitmap()
{
	return pBitmap;
}

int CFiltreEffetCPU::OilPaintingEffect(const int &size, const int &dynRatio)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;
	cv::Mat dst;
	cv::Mat image(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
	cv::cvtColor(image, dst, cv::COLOR_BGRA2BGR);
	cv::xphoto::oilPainting(dst, image, size, dynRatio, cv::COLOR_BGR2Lab);
	cv::cvtColor(image, dst, cv::COLOR_BGR2BGRA);
	bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	return 0;
}

void CFiltreEffetCPU::SetBitmap(CImageLoadingFormat * bitmap)
{
	if (preview)
	{
		if (bitmapOut != nullptr)
			delete bitmapOut;
		bitmapOut = bitmap->GetRegardsBitmap();
	}
	else
	{
		if (pBitmap != nullptr)
			delete pBitmap;
		pBitmap = bitmap->GetRegardsBitmap();
	}

}

int CFiltreEffetCPU::RedEye()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRedEye redeye;
		return redeye.RemoveRedEye(bitmap);
	}
	return 0;
}

CRegardsFloatBitmap * CFiltreEffetCPU::GetFloatBitmap(const bool &source)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

    CImageLoadingFormat imageLoading(false);
    imageLoading.SetPicture(bitmap);
    return imageLoading.GetFloatBitmap(true);
}

int CFiltreEffetCPU::WaveFilter(int x, int y, short height, int scale, int radius)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CWaveFilter * waveFilter = new  CWaveFilter();
		waveFilter->ProcessEffect(bitmap, x, y, height, scale, radius);
		delete waveFilter;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::Bm3d(const int & fSigma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		int nbProcess = thread::hardware_concurrency();
        CBm3DFilter * bm3dFilter = new CBm3DFilter(bitmap, value[fSigma]);
		bm3dFilter->DetermineData(nbProcess - 1);
		bm3dFilter->ExecuteFilter();
		CRegardsBitmap * copyBitmap = bm3dFilter->GetResult();
		memcpy(bitmap->GetPtBitmap(), copyBitmap->GetPtBitmap(), copyBitmap->GetBitmapSize());
		delete bm3dFilter;
        return 0; 
	}
	return -1;
}

// Remove black dots (upto 4x4 in size) of noise from a pure black & white image.
// ie: The input image should be mostly white (255) and just contains some black (0) noise
// in addition to the black (0) edges.
void CFiltreEffetCPU::RemovePepperNoise(cv::Mat &mask)
{
	// For simplicity, ignore the top & bottom row border.
	for (int y = 2; y < mask.rows - 2; y++) {
		// Get access to each of the 5 rows near this pixel.
		uchar *pThis = mask.ptr(y);
		uchar *pUp1 = mask.ptr(y - 1);
		uchar *pUp2 = mask.ptr(y - 2);
		uchar *pDown1 = mask.ptr(y + 1);
		uchar *pDown2 = mask.ptr(y + 2);

		// For simplicity, ignore the left & right row border.
		pThis += 2;
		pUp1 += 2;
		pUp2 += 2;
		pDown1 += 2;
		pDown2 += 2;
		for (int x = 2; x < mask.cols - 2; x++) {
			uchar v = *pThis;   // Get the current pixel value (either 0 or 255).
			// If the current pixel is black, but all the pixels on the 2-pixel-radius-border are white
			// (ie: it is a small island of black pixels, surrounded by white), then delete that island.
			if (v == 0) {
				bool allAbove = *(pUp2 - 2) && *(pUp2 - 1) && *(pUp2) && *(pUp2 + 1) && *(pUp2 + 2);
				bool allLeft = *(pUp1 - 2) && *(pThis - 2) && *(pDown1 - 2);
				bool allBelow = *(pDown2 - 2) && *(pDown2 - 1) && *(pDown2) && *(pDown2 + 1) && *(pDown2 + 2);
				bool allRight = *(pUp1 + 2) && *(pThis + 2) && *(pDown1 + 2);
				bool surroundings = allAbove && allLeft && allBelow && allRight;
				if (surroundings == true) {
					// Fill the whole 5x5 block as white. Since we know the 5x5 borders
					// are already white, just need to fill the 3x3 inner region.
					*(pUp1 - 1) = 255;
					*(pUp1 + 0) = 255;
					*(pUp1 + 1) = 255;
					*(pThis - 1) = 255;
					*(pThis + 0) = 255;
					*(pThis + 1) = 255;
					*(pDown1 - 1) = 255;
					*(pDown1 + 0) = 255;
					*(pDown1 + 1) = 255;
				}
				// Since we just covered the whole 5x5 block with white, we know the next 2 pixels
				// won't be black, so skip the next 2 pixels on the right.
				pThis += 2;
				pUp1 += 2;
				pUp2 += 2;
				pDown1 += 2;
				pDown2 += 2;
			}
			// Move to the next pixel.
			pThis++;
			pUp1++;
			pUp2++;
			pDown1++;
			pDown2++;
		}
	}
}

int CFiltreEffetCPU::CartoonifyImage(const bool & sketchMode, const bool & alienMode, const bool & evilMode)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	cv::Mat dst;
	cv::Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());

	// Convert from BGR color to Grayscale
	cv::Mat srcGray;
	cv::cvtColor(src, srcGray, cv::COLOR_BGRA2GRAY);

	// Remove the pixel noise with a good Median filter, before we start detecting edges.
	medianBlur(srcGray, srcGray, 7);

	cv::Size size = src.size();
	cv::Mat mask = cv::Mat(size, CV_8U);
	cv::Mat edges = cv::Mat(size, CV_8U);
	if (!evilMode) {
		// Generate a nice edge mask, similar to a pencil line drawing.
		Laplacian(srcGray, edges, CV_8U, 5);
		threshold(edges, mask, 80, 255, cv::THRESH_BINARY_INV);
		// Mobile cameras usually have lots of noise, so remove small
		// dots of black noise from the black & white edge mask.
		RemovePepperNoise(mask);
	}
	else {
		// Evil mode, making everything look like a scary bad guy.
		// (Where "srcGray" is the original grayscale image plus a medianBlur of size 7x7).
		cv::Mat edges2;
		Scharr(srcGray, edges, CV_8U, 1, 0);
		Scharr(srcGray, edges2, CV_8U, 1, 0, -1);
		edges += edges2;
		threshold(edges, mask, 12, 255, cv::THRESH_BINARY_INV);
		medianBlur(mask, mask, 3);
	}
	//imshow("edges", edges);
	//imshow("mask", mask);

	// For sketch mode, we just need the mask!
	if (sketchMode) {
		// The output image has 3 channels, not a single channel.
		cvtColor(mask, dst, cv::COLOR_GRAY2BGRA);
		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		return -1;
	}

	// Do the bilateral filtering at a shrunken scale, since it
	// runs so slowly but doesn't need full resolution for a good effect.
	cv::Size smallSize;
	smallSize.width = size.width / 2;
	smallSize.height = size.height / 2;
	cv::Mat smallImg = cv::Mat(smallSize, CV_8UC3);
	resize(src, smallImg, smallSize, 0, 0, cv::INTER_LINEAR);

	// Perform many iterations of weak bilateral filtering, to enhance the edges
	// while blurring the flat regions, like a cartoon.
	cv::Mat tmp = cv::Mat(smallSize, CV_8UC3);
	int repetitions = 7;        // Repetitions for strong cartoon effect.
	for (int i = 0; i < repetitions; i++) {
		int size = 9;           // Filter size. Has a large effect on speed.
		double sigmaColor = 9;  // Filter color strength.
		double sigmaSpace = 7;  // Positional strength. Effects speed.
		bilateralFilter(smallImg, tmp, size, sigmaColor, sigmaSpace);
		bilateralFilter(tmp, smallImg, size, sigmaColor, sigmaSpace);
	}

	if (alienMode) {
		// Apply an "alien" filter, when given a shrunken image and the full-res edge mask.
		// Detects the color of the pixels in the middle of the image, then changes the color of that region to green.
		ChangeFacialSkinColor(smallImg, edges);
	}

	// Go back to the original scale.
	resize(smallImg, src, size, 0, 0, cv::INTER_LINEAR);

	// Use the blurry cartoon image, except for the strong edges that we will leave black.
	src.copyTo(dst, mask);
	bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
}

// Apply an "alien" filter, when given a shrunken BGR image and the full-res edge mask.
// Detects the color of the pixels in the middle of the image, then changes the color of that region to green.
void CFiltreEffetCPU::ChangeFacialSkinColor(cv::Mat smallImgBGR, cv::Mat bigEdges)
{
	// Convert to Y'CrCb color-space, since it is better for skin detection and color adjustment.
	cv::Mat yuv = cv::Mat(smallImgBGR.size(), CV_8UC3);
	cvtColor(smallImgBGR, yuv, cv::COLOR_BGR2YCrCb);

	// The floodFill mask has to be 2 pixels wider and 2 pixels taller than the small image.
	// The edge mask is the full src image size, so we will shrink it to the small size,
	// storing into the floodFill mask data.
	int sw = smallImgBGR.cols;
	int sh = smallImgBGR.rows;
	cv::Mat maskPlusBorder = cv::Mat::zeros(sh + 2, sw + 2, CV_8U);
	cv::Mat mask = maskPlusBorder(cv::Rect(1, 1, sw, sh));  // mask is a ROI in maskPlusBorder.
	resize(bigEdges, mask, smallImgBGR.size());

	// Make the mask values just 0 or 255, to remove weak edges.
	threshold(mask, mask, 80, 255, cv::THRESH_BINARY);
	// Connect the edges together, if there was a pixel gap between them.
	dilate(mask, mask, cv::Mat());
	erode(mask, mask, cv::Mat());
	//imshow("constraints for floodFill", mask);

	// YCrCb Skin detector and color changer using multiple flood fills into a mask.
	// Apply flood fill on many points around the face, to cover different shades & colors of the face.
	// Note that these values are dependent on the face outline, drawn in drawFaceStickFigure().
	int const NUM_SKIN_POINTS = 6;
	cv::Point skinPts[NUM_SKIN_POINTS];
	skinPts[0] = cv::Point(sw / 2, sh / 2 - sh / 6);
	skinPts[1] = cv::Point(sw / 2 - sw / 11, sh / 2 - sh / 6);
	skinPts[2] = cv::Point(sw / 2 + sw / 11, sh / 2 - sh / 6);
	skinPts[3] = cv::Point(sw / 2, sh / 2 + sh / 16);
	skinPts[4] = cv::Point(sw / 2 - sw / 9, sh / 2 + sh / 16);
	skinPts[5] = cv::Point(sw / 2 + sw / 9, sh / 2 + sh / 16);
	// Skin might be fairly dark, or slightly less colorful.
	// Skin might be very bright, or slightly more colorful but not much more blue.
	const int LOWER_Y = 60;
	const int UPPER_Y = 80;
	const int LOWER_Cr = 25;
	const int UPPER_Cr = 15;
	const int LOWER_Cb = 20;
	const int UPPER_Cb = 15;
	cv::Scalar lowerDiff = cv::Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
	cv::Scalar upperDiff = cv::Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);
	// Instead of drawing into the "yuv" image, just draw 1's into the "maskPlusBorder" image, so we can apply it later.
	// The "maskPlusBorder" is initialized with the edges, because floodFill() will not go across non-zero mask pixels.
	cv::Mat edgeMask = mask.clone();    // Keep an duplicate copy of the edge mask.
	for (int i = 0; i < NUM_SKIN_POINTS; i++) {
		// Use the floodFill() mode that stores to an external mask, instead of the input image.
		const int flags = 4 | cv::FLOODFILL_FIXED_RANGE | cv::FLOODFILL_MASK_ONLY;
		floodFill(yuv, maskPlusBorder, skinPts[i], cv::Scalar(), NULL, lowerDiff, upperDiff, flags);
	}

	// After the flood fill, "mask" contains both edges and skin pixels, whereas
	// "edgeMask" just contains edges. So to get just the skin pixels, we can remove the edges from it.
	mask -= edgeMask;
	// "mask" now just contains 1's in the skin pixels and 0's for non-skin pixels.

	// Change the color of the skin pixels in the given BGR image.
	int Red = 0;
	int Green = 70;
	int Blue = 0;
	add(smallImgBGR, cv::Scalar(Blue, Green, Red), smallImgBGR, mask);
}


int CFiltreEffetCPU::ClaheFilter(int nBins, float clipLevel, int windowSize)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		/*
		CClahe * clahe = new Regards::FiltreEffet::CClahe();
		CRegardsBitmap * out = clahe->ClaheFilter(bitmap, nBins, clipLevel, windowSize);
		bitmap->SetBitmap(out->GetPtBitmap(), out->GetBitmapWidth(), out->GetBitmapHeight());
		delete clahe;
		delete out;
		return 0;
		*/

		cv::Mat dst;
		cv::Mat src(bitmap->GetBitmapHeight(), bitmap->GetBitmapWidth(), CV_8UC4, bitmap->GetPtBitmap());
		if (src.channels() >= 3) {
			// READ RGB color image and convert it to Lab
			
			cv::Mat channel;
			cv::cvtColor(src, dst, cv::COLOR_BGRA2BGR);
			cv::cvtColor(dst, dst, cv::COLOR_BGR2Lab);

			// Extract the L channel
			cv::extractChannel(dst, channel, 0);

			// apply the CLAHE algorithm to the L channel
			cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
			clahe->setClipLimit(4);
			clahe->apply(channel, channel);

			// Merge the the color planes back into an Lab image
			cv::insertChannel(channel, dst, 0);

			// convert back to RGB
			cv::cvtColor(dst, dst, cv::COLOR_Lab2BGR);
			cv::cvtColor(dst, dst, cv::COLOR_BGR2BGRA);
			// Temporary Mat not reused, so release from memory.
			channel.release();
		}

		bitmap->SetBitmap(dst.data, bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
	}
	return 0;
}

int CFiltreEffetCPU::BilateralFilter(int fSize,  float sigmaX, float sigmaP)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CBilateral * bilateral = new Regards::FiltreEffet::CBilateral(fSize, sigmaX, sigmaP);
		bilateral->SetParameter(bitmap, backColor);
		bilateral->Compute();
		delete bilateral;
		return 0;
	}
	return -1;
}

int CFiltreEffetCPU::NlmeansFilter(int fsize, int bsize, float sigma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNlmeans * nlmeans = new Regards::FiltreEffet::CNlmeans(fsize, bsize, sigma);
		nlmeans->SetParameter(bitmap, backColor);
		nlmeans->Compute();
		delete nlmeans;
		return 0;
	}
	return -1;
}


int CFiltreEffetCPU::SharpenMasking(const float &sharpness)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSharpenMasking * sharpenMasking = new Regards::FiltreEffet::CSharpenMasking(sharpness);
		sharpenMasking->SetParameter(bitmap, backColor);
		sharpenMasking->Compute();
		delete sharpenMasking;
		return 0;
	}
	return -1;
}

CFiltreEffetCPU::~CFiltreEffetCPU()
{
	if(bitmapOut != nullptr)
		delete bitmapOut;
	bitmapOut = nullptr;
    
    if(pBitmap != nullptr)
        delete pBitmap;
    pBitmap = nullptr;

	if (hq3d != nullptr)
		delete hq3d;
}

wxImage CFiltreEffetCPU::GetwxImage()
{
	int width = 0;
	int height = 0;
	unsigned char * data = nullptr;
	if(bitmapOut != nullptr)
	{
		width = bitmapOut->GetBitmapWidth();
		height = bitmapOut->GetBitmapHeight();
		data = bitmapOut->GetPtBitmap();
	}
	else
	{
		width = pBitmap->GetBitmapWidth();
		height = pBitmap->GetBitmapHeight();
		data = pBitmap->GetPtBitmap();
	}
	wxImage anImage(width, height, false);
	unsigned char * dataOut = anImage.GetData();
	int size = width*height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			int y = i / width;
			int x = i - (y * width);
			int calcul = (height - y - 1) * width + x;
			dataOut[i * 3] = data[(calcul << 2) + 2]; // R
			dataOut[i * 3 + 1] = data[(calcul << 2) + 1]; // G
			dataOut[i * 3 + 2] = data[(calcul << 2)]; // B
		}

	}

	return anImage;
}

wxImage CFiltreEffetCPU::GetwxImage(CRegardsBitmap * bitmap)
{
	int width = bitmap->GetBitmapWidth();
	int height = bitmap->GetBitmapHeight();
	unsigned char * data = bitmap->GetPtBitmap();
	wxImage anImage(width, height, false);
	unsigned char * dataOut = anImage.GetData();
	int size = width*height;
	//pBitmap->VertFlipBuf();

	if (data != nullptr)
	{
#pragma omp parallel for
		for (int i = 0; i < size; ++i)
		{
			int y = i / width;
			int x = i - (y * width);
			int pos = i * 3;
			int calcul = ((height - y - 1) * width + x) << 2;
			//int calcul = y * width + x;
			dataOut[pos] = data[calcul + 2]; // R
			dataOut[pos + 1] = data[calcul + 1]; // G
			dataOut[pos + 2] = data[calcul]; // B
		}

	}

	return anImage;
}


void CFiltreEffetCPU::Interpolation(const int &widthOut, const int &heightOut, const int &method, int flipH, int flipV, int angle)
{
	if(bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	ApplyInterpolationFilters(pBitmap, bitmapOut, flipH, flipV, angle, method);
}

void CFiltreEffetCPU::Interpolation(const int &widthOut, const int &heightOut, const wxRect &rc, const int &method, int flipH, int flipV, int angle)
{
	if(bitmapOut != nullptr)
		delete bitmapOut;

	bitmapOut = new CRegardsBitmap(widthOut, heightOut);
	//CInterpolationBicubic interpolation;
	ApplyInterpolationFilters(pBitmap, bitmapOut, rc, flipH, flipV, angle, method);
}

int CFiltreEffetCPU::HistogramLog()
{
	return CHistogramme::HistogramLog(pBitmap);
}

int CFiltreEffetCPU::HistogramNormalize()
{
	return CHistogramme::HistogramNormalize(pBitmap);
}

int CFiltreEffetCPU::HistogramEqualize()
{
	return CHistogramme::HistogramEqualize(pBitmap);
}

int CFiltreEffetCPU::HQDn3D(const double & LumSpac, const double & ChromSpac, const double & LumTmp, const double & ChromTmp)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;


	if (bitmap != nullptr)
	{
		if (hq3d == nullptr)
		{
			hq3d = new Chqdn3d(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), LumSpac, LumTmp);
		}
		else if (oldLevelDenoise != LumSpac || bitmap->GetBitmapWidth() != oldwidthDenoise || bitmap->GetBitmapHeight() != oldheightDenoise)
		{
			delete hq3d;
			hq3d = new Chqdn3d(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), LumSpac, LumTmp);
		}

		hq3d->ApplyDenoise3D(bitmap);
	}
		
	return 0;
}

int CFiltreEffetCPU::LensFlare(const int &iPosX, const int &iPosY, const int &iPuissance, const int &iType, const int &iIntensity, const int &iColor, const int &iColorIntensity)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CLensFlare * filtre = new CLensFlare();
		filtre->LensFlare(bitmap, iPosX, iPosY, iPuissance, iType, iIntensity, iColor, iColorIntensity);
		delete filtre;

	}
	return 0;
}

//---------------------------------------------------------------------
//Effet CRgbaquad 
//---------------------------------------------------------------------
int CFiltreEffetCPU::RGBFilter(const int &red, const int &green, const int &blue)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRgbFiltre * filtre = new CRgbFiltre(red, green, blue);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Solarize(const long &threshold)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSolarize * filtre = new CSolarize((int)threshold);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}

int CFiltreEffetCPU::Posterize(const float &level, const float &gamma)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPosterize * filtre = new CPosterize(level, gamma);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;

	}
	return 0;
}
//---------------------------------------------------------------------
//Effet Clouds 
//---------------------------------------------------------------------
int CFiltreEffetCPU::CloudsFilter(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave, const int &intensity)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPerlinNoise * m_perlinNoise = new Regards::FiltreEffet::CPerlinNoise();
		CRegardsBitmap localBitmap(250,250);
		CRegardsBitmap * _local = new CRegardsBitmap(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight());
		m_perlinNoise->Clouds(&localBitmap, color1, color2, amplitude / 100.0f, frequence / 100.0f, octave);
		delete m_perlinNoise;
		CInterpolationBicubic interpolation;
		interpolation.Execute(&localBitmap, _local);
		Fusion(_local, intensity / 100.0f);
		delete _local;
	}
	return 0;
}

//---------------------------------------------------------------------
//Effet Swirl 
//---------------------------------------------------------------------
int CFiltreEffetCPU::Swirl(const float &radius, const float &angle)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSwirl * filtre = new CSwirl(angle, radius);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//---------------------------------------------------------------------
//Lumière et Contraste 
//---------------------------------------------------------------------
int CFiltreEffetCPU::BrightnessAndContrast(const double &brightness, const double &contrast)
{
	double offset;

	if (contrast == 0)
		Contrast(1, 128);
	else if (contrast > 0)
	{
		offset = contrast;
		offset /= 100;
		offset = 1 + offset;
		Contrast(offset, 128);
	}
	else if (contrast < 0)
	{
		offset = -contrast;
		offset /= 100;
		offset = 1 - offset;
		//lValue = 128 + lContrast;
		Contrast(offset, 128);
	}

	Lightness(brightness);

	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NiveauDeGris()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CGrayScale * filtre = new CGrayScale();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
int CFiltreEffetCPU::NoirEtBlanc()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CBlackAndWhite * filtre = new CBlackAndWhite();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sepia()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CSepiaFiltre * filtre = new CSepiaFiltre();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Soften()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { 1, 1, 1, 1, 8, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 16, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Blur(const int &radius)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		int sizeKernel = radius * 3;
		short * kernel = new short[sizeKernel];
		for(int i = 0;i < sizeKernel;i++)
			kernel[i] = 1;
		//{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, radius, sizeKernel, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::GaussianBlur(const int &radius, const int &boxSize)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CGaussianBlur gaussianBlur;
		gaussianBlur.GaussianBlur(bitmap, radius);
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Emboss()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 127);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::SharpenStrong()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Sharpen()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 16, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 8, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Erode()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CErode * filtre = new CErode();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Median()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMedian * filtre = new CMedian();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Noise()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNoise * filtre = new CNoise();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Dilate()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CDilate * filtre = new CDilate();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Negatif()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CNegatif * filtre = new CNegatif();
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Contrast(const double &contrast,const uint8_t & offset)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CContrast * filtre = new CContrast(contrast, offset);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

int CFiltreEffetCPU::Lightness(const double &factor)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CLightness * filtre = new CLightness(factor);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreEdge()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		short kernel[] = { -1, -1, -1, -1, 8, -1, -1, -1, -1 };
		CMatrixConvolution * filtre = new CMatrixConvolution(kernel, 3, 1, 0);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FiltreMosaic()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMosaic * filtre = new CMosaic(5);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}


	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipVertical()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->VertFlipBuf();
	}
	return 0;
}
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::FlipHorizontal()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->HorzFlipBuf();
	}

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::MotionBlur(const double &radius, const double &sigma, const double &angle)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CMotionBlur * filtre = new CMotionBlur();
		filtre->MotionBlur(bitmap, radius, sigma, angle);
		delete filtre;

	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::RotateFree(const double &angle, const int &widthOut, const int &heightOut)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, angle, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::PhotoFiltre(const CRgbaquad &clValue, const int &intensity)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CPhotoFiltre * filtre = new CPhotoFiltre(clValue, intensity);
		filtre->SetParameter(bitmap, backColor);
		filtre->Compute();
		delete filtre;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate90()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, 270.0f, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Rotate270()
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		CRotation * m_cEffet = new CRotation();
		m_cEffet->Rotate(bitmap, 90.0f, backColor);
		delete m_cEffet;
	}
	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Resize(const int &imageWidth, const int &imageHeight, const int &interpolation)
{
	CRegardsBitmap * scaleBitmap = new CRegardsBitmap(imageWidth, imageHeight);

	CInterpolationBicubic imageScale;
	imageScale.Execute(pBitmap, scaleBitmap);
	pBitmap->SetBitmap(scaleBitmap->GetPtBitmap(), imageWidth, imageHeight);

	delete scaleBitmap;

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
int CFiltreEffetCPU::Fusion(CRegardsBitmap * bitmapSecond, const float &pourcentage)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	if (bitmap != nullptr)
	{
		bitmap->FusionBitmap(bitmapSecond, pourcentage);
	}
	return 0;
}

void CFiltreEffetCPU::GetBitmap(CRegardsBitmap * & bitmap, const bool &source)
{
	if (bitmap != nullptr)
	{
		if (bitmapOut != nullptr && !source)
		{
			bitmap->SetFilename(bitmapOut->GetFilename());
			bitmap->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
		}
		else
		{
			bitmap->SetFilename(pBitmap->GetFilename());
			bitmap->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
		}
	}

}
#define SCALEBITS            8
#define ONE_HALF             (1 << (SCALEBITS - 1))
#define FIX(x)               ((int) ((x) * (1L<<SCALEBITS) + 0.5))

void CFiltreEffetCPU::GetYUV420P(uint8_t * & lum, uint8_t * & cb, uint8_t * & cr, const int &widthOut, const int &heightOut)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	/*
	int height_middle = widthOut / 2;
	int width_middle = heightOut / 2;
	uint8_t * src = bitmap->GetPtBitmap();

	for (int y = 0; y < height_middle; y++)
	{
		for (int x = 0; x < width_middle; x++)
		{
			int r1 = 0;
			int g1 = 0;
			int b1 = 0;
			int cr_position = x + y * width_middle;
			int lum_position = (x * 2) + (y * 2) * widthOut;
			int position = (x * 2) * 4 + (y * 2) * widthOut * 4;
			for (int i = 0; i < 2; i++)
			{
				int r = src[position + 2];
				int g = src[position + 1];
				int b = src[position + 0];
				r1 += r;
				g1 += g;
				b1 += b;
				lum[lum_position] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
				r = src[position + 6];
				g = src[position + 5];
				b = src[position + 4];
				r1 += r;
				g1 += g;
				b1 += b;
				lum[lum_position + 1] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

				position += widthOut * 4;
				lum_position += widthOut;
			}

			cb[cr_position] = (((-FIX(0.16874) * r1 - FIX(0.33126) * g1 + FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cr[cr_position] = (((FIX(0.50000) * r1 - FIX(0.41869) * g1 - FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
		}
	}
	*/

	int wrap, wrap3, x, y;
	int r, g, b, r1, g1, b1;
	uint8_t *p;
	wrap = bitmap->GetBitmapWidth();
	wrap3 = bitmap->GetBitmapWidth() * 4;
	p = bitmap->GetPtBitmap();
	for (y = 0; y < bitmap->GetBitmapHeight(); y += 2)
	{
		for (x = 0; x < bitmap->GetBitmapWidth(); x += 2)
		{
			r = p[2];
			g = p[1];
			b = p[0];
			r1 = r;
			g1 = g;
			b1 = b;
			lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			r = p[6];
			g = p[5];
			b = p[4];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			p += wrap3;
			lum += wrap;
			r = p[2];
			g = p[1];
			b = p[0];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[0] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
			r = p[6];
			g = p[5];
			b = p[4];
			r1 += r;
			g1 += g;
			b1 += b;
			lum[1] = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;

			cb[0] = (((-FIX(0.16874) * r1 - FIX(0.33126) * g1 +
				FIX(0.50000) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cr[0] = (((FIX(0.50000) * r1 - FIX(0.41869) * g1 -
				FIX(0.08131) * b1 + 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128);
			cb++;
			cr++;
			p += -wrap3 + 2 * 4;
			lum += -wrap + 2;
		}
		p += wrap3;
		lum += wrap;
	}
}

CRegardsBitmap * CFiltreEffetCPU::GetBitmap(const bool &source)
{
	CRegardsBitmap * copy = new CRegardsBitmap();
	if (bitmapOut != nullptr && !source)
	{
		copy->SetFilename(bitmapOut->GetFilename());
		copy->SetBitmap(bitmapOut->GetPtBitmap(), bitmapOut->GetBitmapWidth(), bitmapOut->GetBitmapHeight());
	}
	else
	{
		copy->SetFilename(pBitmap->GetFilename());
		copy->SetBitmap(pBitmap->GetPtBitmap(), pBitmap->GetBitmapWidth(), pBitmap->GetBitmapHeight());
	}
	return copy;
}

int CFiltreEffetCPU::BrightnessAndContrastAuto(float clipHistPercent)
{
	CRegardsBitmap* bitmap = nullptr;
	if (preview)
		bitmap = bitmapOut;
	else
		bitmap = pBitmap;

	COpenCVEffect::BrightnessAndContrastAuto(bitmap, clipHistPercent);

	return 0;
}