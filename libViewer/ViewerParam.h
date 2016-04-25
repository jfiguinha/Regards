#pragma once
#include <ConfigParam.h>

using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CViewerParam : public CConfigParam
		{
		public:
			CViewerParam();
			~CViewerParam();

			void SetOptionDiaporama(const bool &fullscreen, const  int &effect, const  int &delai, const  bool &enAvant);
			void SetPositionWindow(const wxRect &rc);
			void SetPositionSplitter(const int &position);
			void SetShowThumbnail(const bool &show);
			void SetShowVideoThumbnail(const bool &show);
			void SetShowInfos(const bool &infos);
			void SetShowFilter(const bool &infos);

			void GetOptionDiaporama(bool &fullscreen, int &effect, int &delai, bool &enAvant);
			void GetPositionWindow(wxRect &rc);
			void GetPositionSplitter(int &position);
			void GetShowThumbnail(bool &show);
			void GetShowVideoThumbnail(bool &show);
			void GetShowInfos(bool &infos);
			void GetShowFilter(bool &infos);

			bool GetFullscreenDiaporamaOption();
			bool GetEnAvantDiaporamaOption();
			int GetDelaiDiaporamaOption();

			wxString GetLastFolder();
			void SetLastFolder(const wxString &folder);
            
            wxString GetLastSqlRequest();
            void SetLastSqlRequest(const wxString &sqlRequest);

			void SetCatalogCriteria(const wxString &state);
			wxString GetCatalogCriteria();

			void SetCatalogOpenTriangle(const wxString &state);
			wxString GetCatalogOpenTriangle();

			int GetPositionCriteriaPreview();
			void SetPositionCriteriaPreview(const int &pos);

		private:

			void LoadParameter();
			void SaveParameter();

			//Loading 
			void GetDiaporamaParameter(xml_node<> * diaporama_node);
			void GetWindowParameter(xml_node<> * window_node);
			wxRect GetWindowPositionParameter(xml_node<> * position_node);
			void GetPositionParameter(xml_node<> * position_node);
			void GetCriteriaParameter(xml_node<> * position_node);

			//Saving
			void SetDiaporamaParameter(xml_node<>* sectionDiaporama);
			void SetWindowParameter(xml_node<>* sectionWindow);
			void SetWindowPositionParameter(xml_node<> * sectionWindowPosition, const wxRect &rc);
			void SetPositionParameter(xml_node<>* sectionPosition);
			void SetCriteriaParameter(xml_node<>* section);


			wxString folder;
			wxRect positionRegardsViewer;
			bool showThumbnail;
			bool showVideoThumbnail;
			int position;
			bool showInfos;
			bool fullscreen;
			int delai;
			int numEffect;
			bool enAvant;
			wxString criteriaList;
			wxString criteriaTriangleList;
			int positionCriteriaPreview;
			bool showFilter;
            wxString sqlRequest = "";
		};
	}
}
