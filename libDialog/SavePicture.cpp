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

const wxString GetPictureFilter()
{
	vector<wxString> listExtension = CLibResource::GetSavePictureExtension();
	vector<wxString> listFormat = CLibResource::GetSavePictureFormat();

	int i = 0;
	/*
	wxString listPictureFilter = "";
	for (wxString format : listFormat)
	{
		if (i < listFormat.size() - 1)
			listPictureFilter.append(format + "(*" + format + ")|" + listExtension.at(i++) + "|");
		else
			listPictureFilter.append(format + "(*" + format + ")|" + listExtension.at(i++));
	}
	return listPictureFilter;
	
	*/
	return "PDF(*.PDF)|*.pdf|BMP(*.BMP)|*.bmp|BPG(*.BPG)|*.bpg|JPEG(*.JPG)|*.jpg|TIFF(*.TIF)|*.tif|GIF(*.GIF)|*.gif|PNG(*.PNG)|*.png|TGA(*.TGA)|*.tga|JPEG2000(*.JP2)|*.jp2|PPM(*.PPM)|*.ppm|WEBP (*.WEBP)|*.webp|PCX (*.PCX)|*.pcx|XPM (*.XPM)|*.xpm|JXR (*.JXR)|*.jxr|EXR (*.EXR)|*.exr|J2K (*.J2K)|*.j2k|PFM (*.PFM)|*.pfm|AVIF (*.avif)|*.avif|HEIC (*.heic)|*.heic|NFO (*.nfo)|*.nfo";
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
		vector<wxString> listExtension = CLibResource::GetSavePictureExtension();
		/*
		std::vector<wxString> v = {
			".pdf", ".bmp", ".bpg", ".jpg", ".tif", ".gif", ".png", ".tga", ".jp2", ".ppm",
			".webp", ".pcx", ".xpm", ".jxr", ".exr", ".j2k", ".pfm", ".avif", ".heic", ".nfo"
		};
		*/

		wxFileName bmpFilename(filename);
		wxFileDialog saveFileDialog(nullptr, bmpFilename.GetName(), "", bmpFilename.GetName(),
		                            GetPictureFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return "";

		int filterIndex = saveFileDialog.GetFilterIndex();
		file = saveFileDialog.GetPath();
		wxFileName outputName(file);
		wxString extension = listExtension[filterIndex];
#ifdef WIN32
		file = outputName.GetPath() + "\\" + outputName.GetName() + extension;
#else
        file = outputName.GetPath() + "/" + outputName.GetName() + extension;
#endif
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

wxString CSavePicture::LoadPicture()
{
	wxString openPicture = CLibResource::LoadStringFromResource(L"LBLOPENPICTUREFILE", 1);

	wxFileDialog openFileDialog(nullptr, openPicture, "", "",
	                            GetPictureFilter(), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return ""; // the user changed idea..


	//int filterIndex = openFileDialog.GetFilterIndex();
	return openFileDialog.GetPath();
}


wxArrayString CSavePicture::LoadMultiplePicture()
{
	wxArrayString listFile;

	wxString openPicture = CLibResource::LoadStringFromResource(L"LBLOPENPICTUREFILE", 1);

	wxFileDialog openFileDialog(nullptr, openPicture, "", "",
	                            GetPictureFilter(), wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return listFile; // the user changed idea..


	//int filterIndex = openFileDialog.GetFilterIndex();
	openFileDialog.GetPaths(listFile);

	//int filterIndex = openFileDialog.GetFilterIndex();
	return listFile;
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
					wxString libelle = CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);
					wxBusyInfo wait(libelle);

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
