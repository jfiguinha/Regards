#pragma once
#include <Thumbnail.h>
#include <InfosSeparationBar.h>
using namespace Regards::Window;

namespace Regards::Control
{
	class CInfosSeparationBarEffect;

	class CThumbnailEffect : public CThumbnail
	{
	public:
		CThumbnailEffect(wxWindow* parent, wxWindowID idCTreeWithScrollbarInterface,
		                 const CThemeThumbnail& themeThumbnail, const bool& testValidity);
		~CThumbnailEffect(void) override;

		void SetFile(const wxString& filename, CImageLoadingFormat* imageLoading);
		//void SetIconeSize(const int &width, const int &height);
		wxString GetFilename();
		wxString GetWaitingMessage() override;
		void UpdateScroll() override;
		static bool ItemCompFonct(int x, int y, CIcone* icone, CWindowMain* parent); /* Définit une fonction. */
	private:
		//virtual void SetTheme();
		void GetBitmapDimension(const int& width, const int& height, int& tailleAffichageBitmapWidth,
		                        int& tailleAffichageBitmapHeight, float& newRatio);
		float CalculRatio(const int& width, const int& height, const int& tailleBitmapWidth,
		                  const int& tailleBitmapHeight);
		CInfosSeparationBarEffect* CreateNewSeparatorBar(const wxString& libelle);
		//void SetActiveScroll(bool visible);
		void RenderIcone(wxDC* deviceContext) override;

		CIcone* FindElement(const int& xPos, const int& yPos) override;
		void ProcessIdle() override;
		static void LoadPicture(void* param);
		void UpdateRenderIcone(wxCommandEvent& event);
		bool isAllProcess;
		wxString filename;
		int barseparationHeight;
		InfosSeparationBarVector listSeparator;
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
		//const CThemeThumbnail& theme_thumbnail_;
		//const bool& test_validity_;
		//const wxWindowID id_;
	};
}
