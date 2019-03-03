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
			CThumbnailEffect(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity);
			virtual ~CThumbnailEffect(void);

			void SetFile(const wxString &filename);
			void SetIconeSize(const int &width, const int &height);
			wxString GetFilename();
            wxString GetWaitingMessage();
            
		private:
			//virtual void SetTheme();
			void GetBitmapDimension(const int &width, const int &height, int &tailleAffichageBitmapWidth, int &tailleAffichageBitmapHeight, float &newRatio);
			float CalculRatio(const int &width, const int &height, const int &tailleBitmapWidth, const int &tailleBitmapHeight);
			CInfosSeparationBarEffect * CreateNewSeparatorBar(const wxString &libelle);
			void SetActiveScroll(bool visible);
			void RenderIcone(wxDC * deviceContext);
			void UpdateScroll();
			CIcone * FindElement(const int &xPos, const int &yPos);
			virtual void ProcessIdle();
			static void LoadPicture(void * param);
			void UpdateRenderIcone(wxCommandEvent& event);
			bool isAllProcess;
			wxString filename;
			int barseparationHeight;
			InfosSeparationBarVector listSeparator;
			CRegardsConfigParam * config;
			wxString colorEffect; //L"Color Effect";
			wxString convolutionEffect; //L"Convolution Effect";
			wxString specialEffect; //L"Special Effect";
			wxString histogramEffect; //L"Special Effect";
			wxString blackRoomEffect;
			wxString rotateEffect;
            wxString hdrEffect;
			wxString videoLabelEffect;

		};
	}
}

