#include <header.h>
#include "SavePicture.h"
#include <libPicture.h>
#ifdef __APPLE__
#include <SaveFileFormat.h>
#include <SaveFromCFunction.h>
#include <Picture_id.h>
#include <ConvertUtility.h>
#endif
#include <wx/filename.h>
#include "SelectPage.h"
#include <ImageLoadingFormat.h>

CSavePicture::CSavePicture()
{
}


CSavePicture::~CSavePicture()
{
}

wxString CSavePicture::SelectExternalFormat(wxWindow * window, const wxString &filename)
{
	wxString file = "";
	if (filename != "")
	{
		wxString szFilter = "";


#ifdef __APPLE__


		SaveFileFormat saveFormat(nullptr);
		saveFormat.ShowModal();
		if (!saveFormat.IsOk())
			return "";

		wxString extension;
		int iFormat = saveFormat.GetSaveFormat();
		switch (iFormat)
		{
		case 0:
			iFormat = BMP;
			extension = "bmp";
			break;

		case 1:
			iFormat = TGA;
			extension = "tga";
			break;

		case 2:
			iFormat = PCX;
			extension = "pcx";
			break;

		case 3:
			iFormat = MNG;
			extension = "mng";
			break;

		case 4:
			iFormat = TIFF;
			extension = "tif";
			break;

		case 5:
			iFormat = PNG;
			extension = "png";
			break;

		case 6:
			iFormat = GIF;
			extension = "gif";
			break;

		case 7:
			iFormat = JPEG;
			extension = "jpg";
			break;

		case 8:
			iFormat = PNM;
			extension = "pnm";
			break;

		case 9:
			iFormat = JPC;
			extension = "jpc";
			break;

		case 10:
			iFormat = JPEG2000;
			extension = "jp2";
			break;

		case 11:
			iFormat = PPM;
			extension = "ppm";
			break;

		case 12:
			iFormat = XPM;
			extension = "xpm";
			break;

		case 13:
			iFormat = WEBP;
			extension = "webp";
			break;

		case 14:
			iFormat = PDF;
			extension = "pdf";
			break;

		case 15:
			iFormat = JXR;
			extension = "jxr";
			break;

		case 16:
			iFormat = IFF;
			extension = "iff";
			break;

		case 17:
			iFormat = EXR;
			extension = "exr";
			break;

		case 18:
			iFormat = BPG;
			extension = "bpg";
			break;

		case 19:
			iFormat = PFM;
			extension = "pfm";
			break;
		}

		file = saveFileProgrammaticVersion(CConvertUtility::ConvertToStdString(extension));


#else

		std::vector<wxString> v = { ".pdf",".pnm",".bmp",".bpg",".pcx",".jpg",".tif",".gif",".png",".tga",".jp2",".jpc",".ppm",".mng",".webp",".iff",".xpm",".jxr",".exr",".j2k",".pfm" };
		szFilter = "Files PDF(*.PDF) | *.pdf|Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files BPG(*.BPG)|*.bpg|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files MNG(*.MNG)|*.mng|Files WEBP (*.WEBP)|*.webp|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files JXR (*.JXR)|*.jxr|Files EXR (*.EXR)|*.exr|Files J2K (*.J2K)|*.j2k|Files PFM (*.PFM)|*.pfm";


		wxFileDialog saveFileDialog(nullptr, filename, "", "",
			szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return "";

		int filterIndex = saveFileDialog.GetFilterIndex();
		file = saveFileDialog.GetPath();
		wxString ext = v.at(filterIndex);

		wxFileName fileName(file);
		wxString extension = ".";
		extension.append(fileName.GetExt());
		wxString selectedExtension = v[filterIndex];
		if (extension != selectedExtension)
			file.append(selectedExtension);

#endif
	}
	return file;

}


vector<int> CSavePicture::SelectPage(wxWindow * window, const wxString &filename)
{
	vector<int> listPage;
	if (filename != "")
	{
		CSelectFileDlg selectFile(window, -1, filename, _("Select Page To Extract"));
		if (selectFile.ShowModal() == wxID_OK)
		{
			listPage = selectFile.GetSelectItem();
		}
	}
	return listPage;
}


void CSavePicture::ExportPicture(wxWindow * window, const wxString &filename)
{
	bool multipage = false;
	CLibPicture libPicture;
	wxString file = "";
	if (filename != "")
	{
		file = SelectExternalFormat(window, filename);
	}

	//Select page if multipage
	if (libPicture.TestIsAnimation(filename))
		multipage = true;

	if (file != "")
	{
		if (multipage)
		{
			wxString wxfileName;
			int iFormat = 0;
			int option = 0;
			int quality = 0;

			iFormat = libPicture.TestImageFormat(file);
					   
			if (libPicture.SavePictureOption(iFormat, option, quality) == 1)
			{
				vector<int> listPage = SelectPage(window, filename);
				if (listPage.size() > 0)
				{
					for (int i = 0; i < listPage.size(); i++)
					{
						int numPage = listPage[i];
						CImageLoadingFormat * imageFormat = libPicture.LoadPicture(filename, false, numPage);
						if (imageFormat != nullptr)
						{
							wxFileName fileName(file);
							wxString extension = ".";
							extension.append(fileName.GetExt());

							wxString fileoutput = fileName.GetPath() + "\\" + fileName.GetName() + "_" + to_string(numPage) + extension;


							if (!libPicture.TestIsExifCompatible(filename))
								imageFormat->ApplyExifOrientation(imageFormat->GetOrientation());

							libPicture.SavePicture(fileoutput, imageFormat, option, quality);
						}

					}
				}
			}
		}
		else
			libPicture.SavePicture(filename, file);

		wxMessageBox("Save file complete", "Informations");
	}
}

void CSavePicture::SavePicture(wxWindow * window, CImageLoadingFormat * bitmap, const wxString &filename)
{
	CLibPicture libPicture;
	wxString file = "";
	if (filename != "")
	{
		file = SelectExternalFormat(window, filename);
	}

	if (file != "")
	{
		if (bitmap != nullptr)
		{
			libPicture.SavePicture(file, bitmap);
		}
		else
		{
			libPicture.SavePicture(filename, file);
		}
		wxMessageBox("Save file complete", "Informations");
	}

	
}
