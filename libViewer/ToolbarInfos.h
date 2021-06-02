#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
#include <ConfigParam.h>
using namespace Regards::Window;


namespace Regards
{
	namespace Viewer
	{

		class CToolbarInfos : public CToolbarWindow
		{
		public:
			CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical);
			~CToolbarInfos();
			void SetVideoToolbar();
			void SetPictureToolbar();
            void SetPictureThumbnailToolbar();

			void SetInfosActif();
			void SetMapActif();
			void SetMapInactif();
			void SetEffectParameterInactif();
			void SetEffectParameterActif(const wxString &libelle);

			void SetEffectActif();
			void SetEffectInactif();
            void SetCriteriaPush();

			void SetHistogramPush();
            void SetInfosPush();
            void SetMapPush();
            void SetEffectPush();
            void SetHistoryPush();
            void SetEffectParameterPush();
			void SetAudioVideoPush();
			void SetVideoEffectPush();
		private:

			virtual void Resize();
			void EventManager(const int &id);

			CToolbarInterface * toolbarInterface;
			bool isVideo;
			CToolbarTexte * infos;
			CToolbarTexte * history;
			CToolbarTexte * effect;
			CToolbarTexte * audiovideo;
			CToolbarTexte * videoeffect;
			CToolbarTexte * map;
			CToolbarTexte * effectParameter;
            CToolbarTexte * criteria;
			CToolbarTexte* histogramParameter;

		};


	}
}