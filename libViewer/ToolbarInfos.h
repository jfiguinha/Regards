#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
#include <ConfigParam.h>
#include <string>
#include <vector>
using namespace std;
using namespace Regards::Window;


namespace Regards
{
	namespace Viewer
	{

		class CToolbarInfos : public CToolbarWindow
		{
		public:
			CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface);
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
#ifdef VIEWER
            void SetCriteriaPush();
#endif
            void SetInfosPush();
            void SetMapPush();
            void SetEffectPush();
            void SetHistoryPush();
            void SetEffectParameterPush();

		private:

			void OnSize(wxSizeEvent& event);
			void EventManager(const int &id);

			CToolbarInterface * toolbarInterface;
			bool isVideo = false;
			int width = 0;
			int height = 0;
			CToolbarTexte * infos = nullptr;
			CToolbarTexte * history = nullptr;
			CToolbarTexte * effect = nullptr;
			CToolbarTexte * map = nullptr;
			CToolbarTexte * effectParameter = nullptr;
#ifdef VIEWER
            CToolbarTexte * criteria = nullptr;
#endif
		};


	}
}