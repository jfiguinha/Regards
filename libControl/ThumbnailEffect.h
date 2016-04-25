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

		};
	}
}

