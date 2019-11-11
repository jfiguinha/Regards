#include <header.h>
#include "SavePicture.h"
#include <RegardsBitmap.h>
#include <libPicture.h>
#include <ConvertUtility.h>
#ifdef __APPLE__
#include <SaveFileFormat.h>
#include <SaveFromCFunction.h>
#include <Picture_id.h>
#endif

CSavePicture::CSavePicture()
{
}


CSavePicture::~CSavePicture()
{
}


void CSavePicture::SavePicture(wxWindow * window, const wxString &filename)
{
	CLibPicture libPicture;

	if (filename != "")
	{
		wxString szFilter = "";

#ifdef __APPLE__


		SaveFileFormat saveFormat(nullptr);
		saveFormat.ShowModal();
		if (!saveFormat.IsOk())
			return;

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
		}

		wxString file = saveFileProgrammaticVersion(CConvertUtility::ConvertToStdString(extension));
		libPicture.SavePicture(file, filename);

#else
		szFilter = "Files PDF(*.PDF) | *.pdf|Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files BPG(*.BPG)|*.bpg|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files MNG(*.MNG)|*.mng|Files WEBP (*.WEBP)|*.webp|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files ICO (*.ICO)|*.ico|Files CUR (*.CUR)|*.cur|Files ANI (*.ANI)|*.ani";


		wxFileDialog saveFileDialog(window, filename, "", "",
			szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog.ShowModal() == wxID_CANCEL)
			return;


		wxString file = saveFileDialog.GetPath();
		libPicture.SavePicture(filename, file);

#endif
	}
}

void CSavePicture::SavePicture(wxWindow * window, CImageLoadingFormat * bitmap, const wxString &filename)
{
	CLibPicture libPicture;
	//CRegardsBitmap * bitmap = GetBitmap(true);
	if (bitmap != nullptr)
	{
		if (filename != "")
		{
			wxString szFilter = "";

#ifdef __APPLE__


			SaveFileFormat saveFormat(nullptr);
			saveFormat.ShowModal();
			if (!saveFormat.IsOk())
				return;

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
			}

			wxString file = saveFileProgrammaticVersion(CConvertUtility::ConvertToStdString(extension));
			libPicture.SavePicture(file, bitmap);

#else
            
            std::vector<wxString> v = {".pdf",".pnm",".bmp",".bpg",".pcx",".jpg",".tif",".gif",".png",".tga",".jp2",".jpc",".ppm",".mng",".webp",".iff",".xpm",".ico",".cur",".ani",".jxr",".exr",".j2k"};
			szFilter = "Files PDF(*.PDF) | *.pdf|Files PNM (*.PNM)|*.pnm|Files BMP(*.BMP)|*.bmp|Files BPG(*.BPG)|*.bpg|Files PCX(*.PCX)|*.pcx|Files JPEG(*.JPG)|*.jpg|Files TIFF(*.TIF)|*.tif|Files GIF(*.GIF)|*.gif| Files PNG(*.PNG)|*.png|Files TGA(*.TGA)|*.tga|Files JPEG2000(*.JP2)|*.jp2|Files JPC(*.JPC)|*.jpc|Files PPM(*.PPM)|*.ppm|Files MNG(*.MNG)|*.mng|Files WEBP (*.WEBP)|*.webp|Files IFF (*.IFF)|*.iff|Files XPM (*.XPM)|*.xpm|Files ICO (*.ICO)|*.ico|Files CUR (*.CUR)|*.cur|Files ANI (*.ANI)|*.ani|Files JXR (*.JXR)|*.jxr|Files EXR (*.EXR)|*.exr|Files J2K (*.J2K)|*.j2k";


			wxFileDialog saveFileDialog(window, filename, "", "",
				szFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

			if (saveFileDialog.ShowModal() == wxID_CANCEL)
				return;

            int filterIndex = saveFileDialog.GetFilterIndex();
			wxString file = saveFileDialog.GetPath();
            wxString ext = v.at(filterIndex);
            
			libPicture.SavePicture(file, bitmap);

#endif
		}
	}
}