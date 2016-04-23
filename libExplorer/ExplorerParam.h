#pragma once
#include <ConfigParam.h>

using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Explorer
	{
		class CExplorerParam : public CConfigParam
		{
		public:
			CExplorerParam();
			~CExplorerParam();

			void SetCatalogCriteria(const wxString &state);
			wxString GetCatalogCriteria();

			void SetCatalogOpenTriangle(const wxString &state);
			wxString GetCatalogOpenTriangle();

			void SetPositionWindow(const wxRect &rc);
			void GetPositionWindow(wxRect &rc);

			void SetPositionFolderCriteria(const int &position);
			void SetPositionPreviewThumbnail(const int &position);
			void SetPositionPreviewInfos(const int &position);
			void SetPositionFolderCriteriaPreviewThumbnail(const int &position);

			void SetShowFolder(const int &show);
			void SetShowPreview(const int &show);
			void SetShowInfos(const int &show);

			int GetPositionFolderCriteria();
			int GetPositionPreviewThumbnail();
			int GetPositionPreviewInfos();
			int GetPositionFolderCriteriaPreviewThumbnail();

			int GetShowFolder();
			int GetShowPreview();
			int GetShowInfos();

			//Program Extern
			wxString GetViewerProgram();
			wxString GetShopProgram();

			void SetViewerProgram(const wxString &name);
			void SetShopProgram(const wxString &name);

			//Preview Parameter
			int GetEffect();
			void SetEffect(const int &numEffect);


		private:


			void LoadParameter();
			void SaveParameter();

			//Loading 
			void GetWindowParameter(xml_node<> * window_node);
			void GetWindowSplitterParameter(xml_node<> * window_node);
			wxRect GetWindowPositionParameter(xml_node<> * position_node);
			void GetPositionParameter(xml_node<> * position_node);

			void GetFolderCriteriaParameter(xml_node<> * position_node);
			void GetPreviewThumbnailParameter(xml_node<> * position_node);
			void GetPreviewInfosParameter(xml_node<> * position_node);
			void GetCategoryFolderImageViewParameter(xml_node<> * position_node);
			void GetProgramParameter(xml_node<> * position_node);
			void GetPreviewParameter(xml_node<> * position_node);
			void GetCriteriaParameter(xml_node<> * position_node);


			//Saving
			void SetWindowParameter(xml_node<>* sectionWindow);
			void SetWindowPositionParameter(xml_node<> * sectionWindowPosition, const wxRect &rc);
			void SetPositionParameter(xml_node<>* sectionPosition);
			void SetWindowSplitterParameter(xml_node<>* sectionPosition);
			void SetFolderCriteriaParameter(xml_node<>* sectionWindow);
			void SetPreviewThumbnailParameter(xml_node<>* sectionWindow);
			void SetPreviewInfosParameter(xml_node<>* sectionWindow);
			void SetCategoryFolderImageViewParameter(xml_node<>* sectionWindow);
			void SetProgramExtern(xml_node<>* sectionPosition);
			void SetPreviewParameter(xml_node<>* sectionWindow);
			void SetCriteriaParameter(xml_node<>* section);


			wxRect positionRegardsViewer;
			int showFolder;
			int positionFolderCriteria;

			int showPreview;
			int positionPreviewThumbnail;

			int showInfos;
			int positionPreviewInfos;

			int positionFolderCriteriaPreviewThumbnail;

			//Program Extern
			wxString viewerProgram;
			wxString shopProgram;
			wxString criteriaList;
			wxString criteriaTriangleList;

			int numEffect;
		};
	}
}
