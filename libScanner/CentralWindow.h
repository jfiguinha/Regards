#pragma once
#include <Splitter.h>
#include <PanelWithClickToolbar.h>
#include "ScannerFrame.h"
using namespace std;
using namespace Regards::Window;
class CImageLoadingFormat;


namespace Regards
{
	namespace Scanner
	{
		class CToolbarPDF;
		class CBitmapViewerInfosWnd;

		class CCentralWindow : public CWindowMain
		{
		public:
			CCentralWindow(wxWindow* parent, wxWindowID id, CScannerFrame * frame);
			~CCentralWindow();

			void LoadFile();
			void LoadFile(const wxString &filename);
			CImageLoadingFormat * GetImage();
			wxString GetFilename();
			void OnOpen();

		private:

			void OnSave(wxCommandEvent& event);
			void OnAddPage(wxCommandEvent& event);
			void OnDeletePage(wxCommandEvent& event);
			void OnOpenFile(wxCommandEvent& event);
			void OnScan(wxCommandEvent& event);
			void OnPrint(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnExtractPage(wxCommandEvent& event);

			wxString ProcessExtractFile(const vector<int> & listPage);
			void ProcessFile(const vector<int> & listPage);
			void ProcessAddFile(const wxString &fileToAdd, const vector<int> & listPage, int oldAnimationPosition);

			void RedrawBarPos();
			void OnSize(wxSizeEvent& event);
			CToolbarPDF * toolbarPDF;
			CBitmapViewerInfosWnd * previewWindow;
			int width;
			int height;
			wxString filename;
			CScannerFrame * frame;
		};
	}
}