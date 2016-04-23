#pragma once
#include <Thumbnail.h>
#include <ParamInit.h>
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CInfosSeparationBarEffect;

		class CThumbnailEffect : public CThumbnail
		{
		public:
			CThumbnailEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail);
			virtual ~CThumbnailEffect(void);

			void SetFile(const wxString &filename, CRegardsBitmap * bitmap);
			void SetIconeSize(const int &width, const int &height);
			wxString GetFilename();

		private:
			//virtual void SetTheme();
			void GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio);
			float CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight);
			CInfosSeparationBarEffect * CreateNewSeparatorBar(const wxString &libelle);
			void SetActiveScroll(bool visible);
			void RenderIcone(wxDC * deviceContext);
			void UpdateScroll();
			CIcone * FindElement(const int &xPos, const int &yPos);


			wxString filename;
			int barseparationHeight = 40;
			InfosSeparationBarVector listSeparator;
			CRegardsConfigParam * config = nullptr;
			wxString colorEffect; //L"Color Effect";
			wxString convolutionEffect; //L"Convolution Effect";
			wxString specialEffect; //L"Special Effect";
			wxString histogramEffect; //L"Special Effect";
			wxString _filterSoften; //L"Soften";
			wxString _filterBlur; //L"Blur";
			wxString _filterSolarize; //L"Solarize";
			wxString _filterGaussian; //L"Gaussian Blur";
			wxString _filterMedian; //L"Median";
			wxString _filterMotion; //L"Motion Blur";
			wxString _filterRotate; //L"Rotate Free";
			wxString _filterLight; //L"LIGHT and CONTRAST";
			wxString _filterPhoto; //L"PHOTO FILTRE";
			wxString _filterPosterisation; //L"POSTERISATION";
			wxString _filterColor; //L"COLOR BALANCE";
			wxString _filterSwirl; //L"SWIRL";
			wxString _filterClouds; //L"CLOUDS";
			wxString _filterErode; //L"ERODE";
			wxString _filterDilate; //L"DILATE";
			wxString _filterSharpen; //L"SHARPEN";
			wxString _filterSharpenStrong; //L"SHARPEN STRONG";
			wxString _filterNoise; //L"NOISE";
			wxString _filterMosaic; //L"MOSAIC";
			wxString _filterEmboss; //L"EMBOSS";
			wxString _filterGrey; //L"GREY LEVEL";
			wxString _filterSepia; //L"SEPIA";
			wxString _filterBlack; //L"BLACK AND WHITE";
			wxString _filterEdge; //L"EDGE";
			wxString _filterNegatif; //L"NEGATIF";
			wxString _filterLensFlare; //L"LENS FLARE";
			wxString _filterRedEye; //L"LENS FLARE";
            wxString _filterCrop;
			wxString _filterHistogramNormalize; //L"LENS FLARE";
			wxString _filterHistogramEqualize; //L"LENS FLARE";
			wxString _filterHistogramLog; //L"LENS FLARE";
		};
	}
}

