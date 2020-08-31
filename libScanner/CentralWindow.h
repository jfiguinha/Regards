#pragma once
#include <Splitter.h>
#include <PanelWithClickToolbar.h>
#include "ScannerFrame.h"
using namespace std;
using namespace Regards::Window;
class CImageLoadingFormat;
class wxPdfDocument;


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

			wxString LoadFile();
			wxString LoadFile(const wxString &filename);
			CImageLoadingFormat * GetImage();
			wxString GetFilename();
			void OnOpen(const int &type);
			void UpdateScreenRatio();
			static void ProcessAddFile(const wxString &fileToAdd, const wxString &filename, const vector<int> & listPage, int oldAnimationPosition);

		private:
			void AddPdfPage(wxPdfDocument & oPdfDocument, CImageLoadingFormat * imageFormat, int option, int quality, int numpage);
			void OnSave(wxCommandEvent& event);
			void OnAddPage(wxCommandEvent& event);
			void OnDeletePage(wxCommandEvent& event);
			void OnOpenFile(wxCommandEvent& event);
			void OnPrint(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnExtractPage(wxCommandEvent& event);
            void OnScan(wxCommandEvent& event);
			wxString ProcessExtractFile(const vector<int> & listPage);
			void ProcessFile(const vector<int> & listPage);
			wxString ProcessLoadFiles(wxArrayString & listFile);
			int LoadPictureFile(wxArrayString & listFile, wxString filenameOutput);

			void RedrawBarPos();
			void OnSize(wxSizeEvent& event);
			CToolbarPDF * toolbarPDF;
			CViewerPDF * previewWindow;
			int width;
			int height;
			wxString filename;
			CScannerFrame * frame;
		};
	}
}