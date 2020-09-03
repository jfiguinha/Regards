#pragma once
#include "ConfigParam.h"

using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CMainParam : public CConfigParam
		{
		public:
			CMainParam();
			virtual ~CMainParam();

			void SetOptionDiaporama(const bool &fullscreen, const  int &effect, const  int &delai, const  bool &enAvant);
			void SetPositionWindow(const wxRect &rc);
			void SetPositionSplitter(const int &position);
			void SetShowThumbnail(const bool &show);
			void SetShowVideoThumbnail(const bool &show);
			void SetShowInfos(const bool &infos);
			void SetShowFilter(const bool &infos);
			void SetPertinenceValue(const int &value);
			void SetViewerMode(const int &mode);

			void SetPositionLeftPanel(const wxRect &rc);
			void SetPositionRightPanel(const wxRect &rc);

			wxRect GetPositionLeftPanel();
			wxRect GetPositionRightPanel();

			int GetPertinenceValue();
			int GetViewerMode();
			void GetOptionDiaporama(bool &fullscreen, int &effect, int &delai, bool &enAvant);
			void GetPositionWindow(wxRect &rc);
			void GetPositionSplitter(int &position);
			void GetShowThumbnail(bool &show);
			void GetShowVideoThumbnail(bool &show);
			void GetShowInfos(bool &infos);
			void GetShowFilter(bool &infos);
			void GetCheckIsUpdate(bool &infos);
			int GetDefaultPositionPreviewFace();

			bool GetFullscreenDiaporamaOption();
			bool GetEnAvantDiaporamaOption();
			int GetDelaiDiaporamaOption();

			wxString GetLastFolder();
			void SetLastFolder(const wxString &folder);

			void SetShowFolder(const bool &infos);
			void GetShowFolder(bool &infos);

			void SetShowFace(const bool &infos);
			void GetShowFace(bool &infos);
            
            wxString GetLastSqlRequest();
            void SetLastSqlRequest(const wxString &sqlRequest);

			void SetCatalogCriteria(const wxString &state);
			wxString GetCatalogCriteria();

			void SetCatalogOpenTriangle(const wxString &state);
			wxString GetCatalogOpenTriangle();

			int GetPositionCriteriaPreview();
			void SetPositionCriteriaPreview(const int &pos);

			bool IsThumbnailBottom();
			void SetThumbnailBottom(const bool &isBottom);

			void SetPositionPreviewThumbnail(const int &pos);
			int GetPositionPreviewThumbnail();

			void SetPositionCriteriaFolder(const int &pos);
			int GetPositionCriteriaFolder();	
			void SetCheckIsUpdate(const bool &infos);

			bool GetCheckThumbnailValidity();

			int GetPositionPreviewFace();
			void SetPositionPreviewFace(const int &pos);

		private:

			void LoadParameter();
			void SaveParameter();

			//Loading 
			void GetDiaporamaParameter(xml_node<> * diaporama_node);
			void GetWindowParameter(xml_node<> * window_node);
			wxRect GetWindowPositionParameter(xml_node<> * position_node);
			void GetPositionParameter(xml_node<> * position_node, wxRect & position);
			void GetCriteriaParameter(xml_node<> * position_node);

			//Saving
			void SetDiaporamaParameter(xml_node<>* sectionDiaporama);
			void SetWindowParameter(xml_node<>* sectionWindow);
			void SetWindowPositionParameter(xml_node<> * sectionWindowPosition, const wxRect &rc);
			void SetPositionParameter(xml_node<>* sectionPosition, const wxRect &pos, const wxString &nodeName);
			void SetCriteriaParameter(xml_node<>* section);

			int pertinence;
			wxString folder;
			wxRect positionRegardsViewer;
			bool showThumbnail;
			bool showVideoThumbnail;
			int position;
			bool showInfos;
			bool showFace;
			bool fullscreen;
			int delai;
			int numEffect;
			bool enAvant;
			wxString criteriaList;
			wxString criteriaTriangleList;
			int positionCriteriaPreview;
			bool showFilter;
            wxString sqlRequest;
			bool showFolder;
			bool isThumbnailBottom;
			int positionPreviewThumbnail;
			int positionFolderCriteria;
			int positionPreviewFace;
			bool check;
			int windowMode;
			wxRect leftPanelPos;
			wxRect rightPanelPos;
		};
	}
}
