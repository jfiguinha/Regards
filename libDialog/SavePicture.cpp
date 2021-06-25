#include <header.h>
#include "SavePicture.h"
#include <libPicture.h>
#ifdef __APPLE__
#include <wx/sysopt.h>
//#include <SaveFileFormat.h>
//#include <SaveFromCFunction.h>
//#include <Picture_id.h>
//#include <ConvertUtility.h>
#endif
#include <wx/filename.h>
#include "SelectPage.h"
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include <wx/busyinfo.h>
using namespace Regards::Picture;

CSavePicture::CSavePicture()
{
}


CSavePicture::~CSavePicture()
{
}

wxString CSavePicture::SelectExternalFormat(wxWindow* window, const wxString& filename)
{
	wxString file = "";
	if (filename != "")
	{
		wxString szFilter = "";


#ifdef __APPLE__
		wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
#endif

		wxString _filename = CLibResource::LoadStringFromResource(L"LBLFILESNAME", 1);

		std::vector<wxString> v = {
			".pdf", ".pnm", ".bmp", ".bpg", ".pcx", ".jpg", ".tif", ".gif", ".png", ".tga", ".jp2", ".jpc", ".ppm",
			".mng", ".webp", ".iff", ".xpm", ".jxr", ".exr", ".j2k", ".pfm", ".avif", ".heic"
		};
		szFilter = filename + " PDF(*.PDF)|*.pdf|" + filename + " PNM (*.PNM)|*.pnm|" + filename + "  BMP(*.BMP)|*.bmp|"
			+ filename + "  BPG(*.BPG)|*.bpg|" + filename + "  PCX(*.PCX)|*.pcx|" + filename + "  JPEG(*.JPG)|*.jpg|" +
			filename + "  TIFF(*.TIF)|*.tif|" + filename + "  GIF(*.GIF)|*.gif|" + filename + "  PNG(*.PNG)|*.png|" +
			filename + "  TGA(*.TGA)|*.tga|" + filename + "  JPEG2000(*.JP2)|*.jp2|" + filename + "  JPC(*.JPC)|*.jpc|"
			+ filename + "  PPM(*.PPM)|*.ppm|" + filename + "  MNG(*.MNG)|*.mng|" + filename + "  WEBP (*.WEBP)|*.webp|"
			+ filename + "  IFF (*.IFF)|*.iff|" + filename + "  XPM (*.XPM)|*.xpm|" + filename + "  JXR (*.JXR)|*.jxr|"
			+ filename + "  EXR (*.EXR)|*.exr|" + filename + "  J2K (*.J2K)|*.j2k|" + filename + "  PFM (*.PFM)|*.pfm|"
			+ filename + "  AVIF (*.avif)|*.avif|" + filename + "  HEIC (*.heic)|*.heic";

		wxFileName bmpFilename(filename);
		wxFileDialog saveFileDialog(nullptr, filename, "", bmpFilename.GetName(),
		                            szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return "";

		//int filterIndex = saveFileDialog.GetFilterIndex();
		file = saveFileDialog.GetPath();

	}
	return file;
}


vector<int> CSavePicture::SelectPage(wxWindow* window, const wxString& filename)
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


void CSavePicture::ExportPicture(wxWindow* window, const wxString& filename)
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
					wxBusyInfo wait("Please wait, working...");

					for (int i = 0; i < listPage.size(); i++)
					{
						int numPage = listPage[i];
						CImageLoadingFormat* imageFormat = libPicture.LoadPicture(filename, false, numPage);
						if (imageFormat != nullptr)
						{
							wxFileName fileName(file);
							wxString extension = ".";
							extension.append(fileName.GetExt());

							wxString fileoutput = fileName.GetPath() + "\\" + fileName.GetName() + "_" +
								to_string(numPage) + extension;


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

		wxString savecompleted = CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(savecompleted, infos);
	}
}

void CSavePicture::SavePicture(wxWindow* window, CImageLoadingFormat* bitmap, const wxString& filename)
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
		wxString savecompleted = CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);
		wxString infos = CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);
		wxMessageBox(savecompleted, infos);
	}
}
