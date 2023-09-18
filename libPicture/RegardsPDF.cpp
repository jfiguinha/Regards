#include "header.h"
#include "RegardsPDF.h"
#include "FileUtility.h"
#include "ImageLoadingFormat.h"
#include <wx/wxpoppler.h>
#include <CompressionOption.h>
#include <TiffOption.h>
#include <wx/pdfdocument.h>
#include <PDFOption.h>
#include <qpdf/QPDF.hh>
#include <LibResource.h>
#include <qpdf/QPDFWriter.hh>
#include <ConvertUtility.h>
#include <wx/busyinfo.h>
using namespace Regards::Picture;

CRegardsPDF::CRegardsPDF(const wxString& filename)
{
	poppler = new wxPoppler();
	isValid = poppler->Open(filename);
	if (isValid)
		isValid = poppler->SetDpi(300);
}

CRegardsPDF::~CRegardsPDF()
{
	delete poppler;
}


bool CRegardsPDF::IsOk()
{
	return isValid;
}

wxImage CRegardsPDF::GetPicture(const int& numPicture)
{
	wxImage image;
	isValid = poppler->SelectPage(numPicture);

	if (isValid)
		isValid = poppler->RenderPage();

	if (isValid)
		image = poppler->GetImage();

	return image;
}

int CRegardsPDF::GetNbFrame(const wxString& filename, bool& error)
{
	wxPoppler poppler;
	int m_ani_images = 0;
	bool isValid = poppler.Open(filename);
	if (isValid)
	{
		m_ani_images = poppler.GetPageCount();
		error = false;
	}
	else
		error = true;
	return m_ani_images;
}

void CRegardsPDF::SavePictureToPdf(const wxString& fileName, CImageLoadingFormat* bitmap, const int& option,
                                   const int& quality)
{
	wxString fileToAdd = "";
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();

#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif


	if (option == 0)
	{
		wxLogNull logNo;
#ifdef WIN32
		file = tempFolder + "\\temporary.jpg";
#else
		file = tempFolder + "/temporary.jpg";
#endif


		if (wxFileExists(file))
			wxRemoveFile(file);

		wxImage image = bitmap->GetwxImage();
		image.SetOption("wxIMAGE_OPTION_QUALITY", quality);
		image.SaveFile(file, wxBITMAP_TYPE_JPEG);
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\temporary.tiff";
#else
		file = tempFolder + "/temporary.tiff";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

		wxImage image = bitmap->GetwxImage();
		image.SetOption("wxIMAGE_OPTION_TIFF_COMPRESSION", 5);
		image.SaveFile(file, wxBITMAP_TYPE_TIFF);
	}

	wxImage image;
	image.LoadFile(file);
	//int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
	//int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);
	//	double dblDpiBy72 = (double)m_nDpi / 72.0;

	image.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);
	//image.SetOption(wxIMAGE_OPTION_RESOLUTIONX, 200);// bitmap->GetResolution());
	//image.SetOption(wxIMAGE_OPTION_RESOLUTIONY, 200);// bitmap->GetResolution());
	//image.SetOption(wxIMAGE_OPTION_RESOLUTION, 200);//bitmap->GetResolution());

	SaveToPDF(&image, fileName, file, option, quality);
	wxRemoveFile(file);
}



void CRegardsPDF::SavePicture(const wxString& file, CImageLoadingFormat* bitmap, const int& option,
	const int& quality)
{

	if (option == 0)
	{
		wxLogNull logNo;

		if (wxFileExists(file))
			wxRemoveFile(file);

		wxImage image = bitmap->GetwxImage();
		image.SetOption("wxIMAGE_OPTION_QUALITY", quality);
		image.SaveFile(file, wxBITMAP_TYPE_JPEG);
	}
	else
	{
		if (wxFileExists(file))
			wxRemoveFile(file);

		wxImage image = bitmap->GetwxImage();
		image.SetOption("wxIMAGE_OPTION_TIFF_COMPRESSION", 5);
		image.SaveFile(file, wxBITMAP_TYPE_TIFF);
	}

}

int CRegardsPDF::SavePictureOption(int& option, int& quality)
{
	int returnValue = 0;
	PDFOption pdfOption(nullptr);
	pdfOption.ShowModal();
	if (pdfOption.IsOk())
	{
		option = pdfOption.CompressionOption();
		switch (option)
		{
		case 0:
			{
				CompressionOption jpegOption(nullptr);
				jpegOption.ShowModal();
				if (jpegOption.IsOk())
				{
					quality = jpegOption.CompressionLevel();
				}
				break;
			}
		case 1:
			{
				TiffOption tiffOption(nullptr);
				tiffOption.ShowModal();
				if (tiffOption.IsOk())
					quality = tiffOption.CompressionOption();
			}
			break;
		default: ;
		}
		returnValue = 1;
	}
	return returnValue;
}

bool CRegardsPDF::SaveToPDF(wxImage* poImage, const wxString& pdfFile, int option, int quality)
{
	wxLogNull logNo;
	int _option = option;
	int _quality = quality;

	wxString fileToSave;
	if (option == 0)
		fileToSave = CFileUtility::GetTempFile("scanner.jpg");
	else
		fileToSave = CFileUtility::GetTempFile("scanner.tif");

	if (wxFileExists(fileToSave))
		wxRemoveFile(fileToSave);

	if (option == 0)
		poImage->SaveFile(fileToSave, wxBITMAP_TYPE_JPEG);
	else
		poImage->SaveFile(fileToSave, wxBITMAP_TYPE_TIFF);

	SaveToPDF(poImage, pdfFile, fileToSave, option, _quality);

	return true;
}


bool CRegardsPDF::SaveToPDF(wxImage* poImage, const wxString& fileName, const wxString& pictureName, int option,
                            int quality)
{
	if (poImage->HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT))
	{
		//int nResolutionUnit= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONUNIT );
		int nResolution = 0;

		// Get image resolution-
		if (poImage->HasOption(wxIMAGE_OPTION_RESOLUTION))
		{
			nResolution = poImage->GetOptionInt(wxIMAGE_OPTION_RESOLUTION);
		}
		else if (poImage->HasOption(wxIMAGE_OPTION_RESOLUTIONX) && (poImage->HasOption(wxIMAGE_OPTION_RESOLUTIONY)))
		{
			int nResolutionX = poImage->GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
			int nResolutionY = poImage->GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);

			if (nResolutionX == nResolutionY)
			{
				nResolution = nResolutionX;
			}
		}
		if (nResolution)
		{
			/*
			// Save image in a temporary file.
			wxString strTempFileName= wxFileName::CreateTempFileName( wxEmptyString );
			poImage->SaveFile(strTempFileName, wxBITMAP_TYPE_TIFF);

			// Create a PDF document, add a page, and put the image on it.
			wxPdfDocument oPdfDocument;
			oPdfDocument.AddPage( ( poImage->GetHeight() > poImage->GetWidth() ) ? wxPORTRAIT : wxLANDSCAPE );
			oPdfDocument.SetImageScale( (double)nResolution * ( nResolutionUnit == wxIMAGE_RESOLUTION_CM ? 2.54 : 1.0 ) / 72.0 );
			oPdfDocument.Image( strTempFileName, 0 , 0, 0, 0, wxT( "image/tiff" ) );
			// Remove temporary file.
			::wxRemoveFile( strTempFileName );
			oPdfDocument.Close();
			oPdfDocument.SaveAsFile(fileName);
			*/

			// Create a PDF document, add a page, and put the image on it.
			wxPdfDocument oPdfDocument;
			//int tpl = oPdfDocument.BeginTemplate(0, 0, poImage->GetWidth(), poImage->GetHeight());
			wxPrintOrientation orientation = (poImage->GetHeight() > poImage->GetWidth()) ? wxPORTRAIT : wxLANDSCAPE;
			oPdfDocument.AddPage(orientation);

			//float nResolutionUnit = wxIMAGE_RESOLUTION_CM ? 2.54 : 1.0;
			//float imageScale = (double)nResolution * (float)nResolutionUnit / (float)72.0;
			//double pictureWidth = oPdfDocument.GetPageWidth() * nResolutionUnit;
			//double pictureHeight = oPdfDocument.GetPageHeight() * nResolutionUnit;

			if (option == 0)
				oPdfDocument.Image(pictureName, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(),
				                   wxT("image/jpeg"));
			else
				oPdfDocument.Image(pictureName, 0, 0, oPdfDocument.GetPageWidth(), oPdfDocument.GetPageHeight(),
				                   wxT("image/tiff"));

			oPdfDocument.Close();
			oPdfDocument.SaveAsFile(fileName);

			return true;
		}
	}
	// File saving not possible or not implemented.
	return false;
}

//-----------------------------------------------------------------------------
//Sauvegarde
//-----------------------------------------------------------------------------
int CRegardsPDF::SavePicture(const wxString& fileName, CImageLoadingFormat* bitmap)
{
	const wxString wxfileName = fileName;

	int option = 0;
	int quality = 0;

	if (SavePictureOption(option, quality) == 1)
	{
		SavePictureToPdf(wxfileName, bitmap, option, quality);
	}

	return 0;
}


void CRegardsPDF::AddPdfPage(wxPdfDocument* oPdfDocument, CImageLoadingFormat* imageFormat, int option, int quality,
                             int numpage)
{
	wxString file;
	wxString documentPath = CFileUtility::GetDocumentFolderPath();

#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif


	//Save
	if (option == 0)
	{
#ifdef WIN32
		file = tempFolder + "\\temporary" + to_string(numpage) + ".jpg";
#else
		file = tempFolder + "/temporary" + to_string(numpage) + ".jpg";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

		 SavePicture(file, imageFormat, option, quality);
		//SavePicture(file, imageFormat);
	}
	else
	{
#ifdef WIN32
		file = tempFolder + "\\temporary" + to_string(numpage) + ".tiff";
#else
		file = tempFolder + "/temporary" + to_string(numpage) + ".tiff";
#endif

		if (wxFileExists(file))
			wxRemoveFile(file);

		SavePicture(file, imageFormat, option, quality);
		//SavePicture(file, imageFormat);
	}

	wxImage image;
	image.LoadFile(file);
	//int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
	//int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);
	//	double dblDpiBy72 = (double)m_nDpi / 72.0;

	image.SetOption(wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES);


	if (image.HasOption(wxIMAGE_OPTION_RESOLUTIONUNIT))
	{
		//int nResolutionUnit = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONUNIT);
		int nResolution = 0;

		// Get image resolution-
		if (image.HasOption(wxIMAGE_OPTION_RESOLUTION))
		{
			nResolution = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTION);
		}
		else if (image.HasOption(wxIMAGE_OPTION_RESOLUTIONX) && (image.HasOption(wxIMAGE_OPTION_RESOLUTIONY)))
		{
			int nResolutionX = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
			int nResolutionY = image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);

			if (nResolutionX == nResolutionY)
			{
				nResolution = nResolutionX;
			}
		}
		if (nResolution)
		{
			//int tpl = oPdfDocument.BeginTemplate(0, 0, image.GetWidth(), image.GetHeight());
			wxPrintOrientation orientation = (image.GetHeight() > image.GetWidth()) ? wxPORTRAIT : wxLANDSCAPE;
			oPdfDocument->AddPage(orientation);

			if (option == 0)
				oPdfDocument->Image(file, 0, 0, oPdfDocument->GetPageWidth(), oPdfDocument->GetPageHeight(),
				                    wxT("image/jpeg"));
			else
				oPdfDocument->Image(file, 0, 0, oPdfDocument->GetPageWidth(), oPdfDocument->GetPageHeight(),
				                    wxT("image/tiff"));
		}
	}
}

wxString CRegardsPDF::ExtractPage(const wxString& filename, const vector<int>& listPage)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";

#else
	wxString tempFolder = documentPath + "/temp";
#endif

#ifdef WIN32
	file = tempFolder + "\\extract.pdf";
#else
	file = tempFolder + "/extract.pdf";
#endif

	if (wxFileExists(file))
		wxRemoveFile(file);


	QPDF inpdf;
	inpdf.processFile(CConvertUtility::ConvertToStdString(filename).c_str());
	const std::vector<QPDFObjectHandle>& pages = inpdf.getAllPages();
	//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
	int pageno = 0;

	std::string outfile = CConvertUtility::ConvertToStdString(file);
	QPDF outpdf;
	outpdf.emptyPDF();


	for (auto iter = pages.begin();
	     iter != pages.end(); ++iter)
	{
		bool find = false;
		for (int i : listPage)
		{
			if (i == pageno)
			{
				find = true;
				break;
			}
		}

		if (find)
		{
			QPDFObjectHandle page = *iter;
			outpdf.addPage(page, false);
		}

		pageno++;
	}

	QPDFWriter outpdfw(outpdf, outfile.c_str());
	outpdfw.write();

	return file;
}


void CRegardsPDF::AddPage(const wxString& fileToAdd, const wxString& filename, const vector<int>& listPage,
                          int oldAnimationPosition)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();

#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif

#ifdef WIN32
		file = tempFolder + "\\add.pdf";
#else
		file = tempFolder + "/add.pdf";
#endif

	if (wxFileExists(file))
		wxRemoveFile(file);

	if (file != "")
	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
		wxBusyInfo wait(libelle);
		std::vector<QPDFObjectHandle> oldpages;
		int oldpageno_len = 0;
		int i = 0;
		bool fileIn = false;
		if (filename != "" && wxFileExists(filename))
			fileIn = true;

		QPDF oldpdf;
		if (fileIn)
			oldpdf.processFile(CConvertUtility::ConvertToUTF8(filename));

		QPDF inpdf;
		inpdf.processFile(CConvertUtility::ConvertToUTF8(fileToAdd));


		std::string outfile = CConvertUtility::ConvertToStdString(file);
		QPDF outpdf;
		outpdf.emptyPDF();


		if (fileIn)
		{
			oldpages = oldpdf.getAllPages();
			oldpageno_len = oldpages.size();

			for (; i <= oldAnimationPosition; i++)
			{
				auto object = oldpages.at(i);
				QPDFObjectHandle& page(object);
				outpdf.addPage(page, false);
			}
		}


		const std::vector<QPDFObjectHandle>& pages = inpdf.getAllPages();
		//int pageno_len = pages.size();
		int pageno = 0;


		for (auto newiter = pages.begin(); newiter != pages.end(); ++newiter)
		{
			bool find = false;
			for (int i1 : listPage)
			{
				if (i1 == pageno)
				{
					find = true;
					break;
				}
			}

			if (find)
			{
				QPDFObjectHandle page = *newiter;
				outpdf.addPage(page, false);
			}

			pageno++;
		}

		if (fileIn)
		{
			//for (int i = oldAnimationPosition; i < oldpageno_len; i++)
			for (; i < oldpageno_len; i++)
			{
				auto page = oldpages.at(i);
				outpdf.addPage(page, false);
			}
		}

		QPDFWriter outpdfw(outpdf, outfile.c_str());
		outpdfw.setCompressStreams(true);
		outpdfw.setRecompressFlate(true);
		outpdfw.write();
	}
#ifndef DEMO
	if (wxFileExists(filename))
	{
		bool isRemove = wxRemoveFile(filename);
		if (!isRemove)
		{
			printf("not remove");
		}
	}

	wxCopyFile(file, filename);
#endif
}


void CRegardsPDF::RemovePage(const wxString& filename, const vector<int>& listPage)
{
	wxString file = "";
	wxString documentPath = CFileUtility::GetDocumentFolderPath();
#ifdef WIN32
	wxString tempFolder = documentPath + "\\temp";
#else
	wxString tempFolder = documentPath + "/temp";
#endif

#ifdef WIN32
	file = tempFolder + "\\delete.pdf";
#else
	file = tempFolder + "/delete.pdf";
#endif

	if (wxFileExists(file))
		wxRemoveFile(file);


	{
		wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
		wxBusyInfo wait(libelle);
		QPDF inpdf;
		inpdf.processFile(CConvertUtility::ConvertToUTF8(filename));
		const std::vector<QPDFObjectHandle>& pages = inpdf.getAllPages();
		//int pageno_len = QIntC::to_int(QUtil::uint_to_string(pages.size()).length());
		int pageno = 0;

		std::string outfile = CConvertUtility::ConvertToStdString(file);
		QPDF outpdf;
		outpdf.emptyPDF();


		for (auto iter = pages.begin(); iter != pages.end(); ++iter)
		{
			bool find = false;
			for (int i : listPage)
			{
				if (i == pageno)
				{
					find = true;
					break;
				}
			}

			if (!find)
			{
				QPDFObjectHandle page = *iter;
				outpdf.addPage(page, false);
			}

			pageno++;
		}

		QPDFWriter outpdfw(outpdf, CConvertUtility::ConvertToUTF8(outfile));
		outpdfw.write();
	}


#ifndef DEMO
	if (wxFileExists(filename))
		wxRemoveFile(filename);

	wxCopyFile(file, filename);
#endif
}
