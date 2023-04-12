#pragma once

class CImageLoadingFormat;
class wxPoppler;
class wxPdfDocument;

namespace Regards
{
	namespace Picture
	{
		class CRegardsPDF
		{
		public:
			CRegardsPDF(const wxString& filename);
			~CRegardsPDF();
			bool IsOk();
			wxImage GetPicture(const int& numPicture);
			static int SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap);
			static int GetNbFrame(const wxString& filename, bool& error);
			static int SavePictureOption(int& option, int& quality);
			static bool SaveToPDF(wxImage* poImage, const wxString& pdfFile, int option = -1, int quality = -1);
			static void SavePictureToPdf(const wxString& fileName, CImageLoadingFormat* bitmap, const int& option,
			                             const int& quality);
			static wxString ExtractPage(const wxString& filename, const vector<int>& listPage);
			static void AddPdfPage(wxPdfDocument* oPdfDocument, CImageLoadingFormat* imageFormat, int option,
			                       int quality, int numpage);
			static void AddPage(const wxString& fileToAdd, const wxString& filename, const vector<int>& listPage,
			                    int oldAnimationPosition);
			static void RemovePage(const wxString& filename, const vector<int>& listPage);

		private:
			static bool SaveToPDF(wxImage* poImage, const wxString& fileName, const wxString& pictureName, int option,
			                      int quality);
			static void SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap, const int& option,
				const int& quality);
			wxPoppler* poppler = nullptr;
			bool isValid = false;
		};
	}
}
