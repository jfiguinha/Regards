#pragma once
#include "ThumbnailVerticalSeparator.h"
using namespace Regards::Window;

namespace Regards::Control
{
	class CInfosSeparationBarEffect;

	class CThumbnailEffect : public CThumbnailVerticalSeparator
	{
	public:
		CThumbnailEffect(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                 const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailEffect(void) override;

		void SetFile(const wxString& filename, CImageLoadingFormat* imageLoading);
		//void SetIconeSize(const int &width, const int &height);
		wxString GetFilename();

		void UpdateScroll() override;
	private:
		//virtual void SetTheme();
		void GetBitmapDimension(const int& width, const int& height, int& tailleAffichageBitmapWidth,
		                        int& tailleAffichageBitmapHeight, float& newRatio);
		float CalculRatio(const int& width, const int& height, const int& tailleBitmapWidth,
		                  const int& tailleBitmapHeight);
		CInfosSeparationBarEffect* CreateNewSeparatorBar(const wxString& libelle);
		static bool ItemCompFonct(int x, int y, CIcone* icone, CWindowMain* parent);
		CIcone* FindElement(const int& xPos, const int& yPos) override;
		void ProcessIdle() override;
		static void LoadPicture(void* param);
		void UpdateRenderIcone(wxCommandEvent& event);
		bool isAllProcess;
		wxString filename;
		int barseparationHeight;

		CRegardsConfigParam* config;
		wxString colorEffect; //L"Color Effect";
		wxString convolutionEffect; //L"Convolution Effect";
		wxString specialEffect; //L"Special Effect";
		wxString histogramEffect; //L"Special Effect";
		wxString blackRoomEffect;
		wxString rotateEffect;
		wxString hdrEffect;
		wxString videoLabelEffect;
		CImageLoadingFormat* imageLoading;



		int nbProcess = 0;

	};
}
