#include <header.h>
#include <FileUtility.h>
#include <ImageLoadingFormat.h>
#include <LibResource.h>
#include <header.h>
#include <libPicture.h>
#include <wx/busyinfo.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>

#include "SavePicture.h"
#include "SelectPage.h"

#ifdef __APPLE__
#include <wx/sysopt.h>
#endif

using namespace Regards::Picture;

namespace {
    wxString GetPictureFilter() {
        const std::vector<wxString> listExtension =
            CLibResource::GetSavePictureExtension();

        const std::vector<wxString> listFormat = CLibResource::GetSavePictureFormat();

        wxString filter;

        const size_t count = std::min(listExtension.size(), listFormat.size());

        for (size_t i = 0; i < count; ++i) {
            if (!filter.empty()) filter += "|";

            filter += listFormat[i];
            filter += " (*.";
            filter += listExtension[i].StartsWith(".") ? listExtension[i].Mid(1)
                : listExtension[i];
            filter += ")|*.";
            filter += listExtension[i].StartsWith(".") ? listExtension[i].Mid(1)
                : listExtension[i];
        }

        return filter;
    }

    wxString BuildOutputFilename(const wxString& filename,
        const wxString& extension) {
        wxFileName outputName(filename);

        wxString cleanExtension = extension;

        if (cleanExtension.StartsWith(".")) cleanExtension = cleanExtension.Mid(1);

        outputName.SetExt(cleanExtension);

        return outputName.GetFullPath();
    }
}  // namespace

CSavePicture::CSavePicture() = default;

CSavePicture::~CSavePicture() = default;

wxString CSavePicture::SelectExternalFormat(wxWindow* window,
    const wxString& filename) {
    if (filename.empty()) return {};

#ifdef __APPLE__
    wxSystemOptions::SetOption(wxOSX_FILEDIALOG_ALWAYS_SHOW_TYPES, 1);
#endif

    const std::vector<wxString> listExtension =
        CLibResource::GetSavePictureExtension();

    if (listExtension.empty()) return {};

    wxFileName inputFilename(filename);

    wxFileDialog saveFileDialog(window, inputFilename.GetName(), wxEmptyString,
        inputFilename.GetName(), GetPictureFilter(),
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    const wxString documentPath = CFileUtility::GetDocumentFolderPath();

    if (!documentPath.empty()) saveFileDialog.SetDirectory(documentPath);

    if (saveFileDialog.ShowModal() != wxID_OK) return {};

    const int filterIndex = saveFileDialog.GetFilterIndex();

    if (filterIndex < 0 ||
        static_cast<size_t>(filterIndex) >= listExtension.size()) {
        return {};
    }

    const wxString extension = listExtension[filterIndex];

    return BuildOutputFilename(saveFileDialog.GetPath(), extension);
}

std::vector<int> CSavePicture::SelectPage(wxWindow* window,
    const wxString& filename) {
    std::vector<int> listPage;

    if (filename.empty()) return listPage;

    CSelectFileDlg* selectFile = new CSelectFileDlg(window, wxID_ANY, filename,
        _("Select Page To Extract"));

    if (selectFile->ShowModal() == wxID_OK) {
        listPage = selectFile->GetSelectItem();
    }

    selectFile->Destroy();

    return listPage;
}

wxString CSavePicture::LoadPicture() {
    const wxString openPicture =
        CLibResource::LoadStringFromResource(L"LBLOPENPICTUREFILE", 1);

    wxFileDialog openFileDialog(nullptr, openPicture, wxEmptyString,
        wxEmptyString, GetPictureFilter(),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    const wxString documentPath = CFileUtility::GetDocumentFolderPath();

    if (!documentPath.empty()) openFileDialog.SetDirectory(documentPath);

    if (openFileDialog.ShowModal() != wxID_OK) return {};

    return openFileDialog.GetPath();
}

wxArrayString CSavePicture::LoadMultiplePicture() {
    wxArrayString listFile;

    const wxString openPicture =
        CLibResource::LoadStringFromResource(L"LBLOPENPICTUREFILE", 1);

    wxFileDialog openFileDialog(nullptr, openPicture, wxEmptyString,
        wxEmptyString, GetPictureFilter(),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

    const wxString documentPath = CFileUtility::GetDocumentFolderPath();

    if (!documentPath.empty()) openFileDialog.SetDirectory(documentPath);

    if (openFileDialog.ShowModal() != wxID_OK) return listFile;

    openFileDialog.GetPaths(listFile);

    return listFile;
}

void CSavePicture::ExportPicture(wxWindow* window, const wxString& filename) {
    if (filename.empty()) return;

    CLibPicture libPicture;

    const wxString file = SelectExternalFormat(window, filename);

    if (file.empty()) return;

    const bool multipage = libPicture.TestIsAnimation(filename);

    if (!multipage) {
        libPicture.SavePicture(filename, file);

        const wxString savecompleted =
            CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);

        const wxString infos =
            CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);

        wxMessageBox(savecompleted, infos, wxOK | wxICON_INFORMATION, window);

        return;
    }

    int iFormat = libPicture.TestImageFormat(file);

    int option = 0;
    int quality = 0;

    if (libPicture.SavePictureOption(iFormat, option, quality) != 1) {
        return;
    }

    const std::vector<int> listPage = SelectPage(window, filename);

    if (listPage.empty()) return;

    const wxString libelle =
        CLibResource::LoadStringFromResource(L"LBLBUSYINFO", 1);

    wxBusyInfo wait(libelle, window);

    const wxFileName outputFilename(file);

    const wxString extension = outputFilename.GetExt();

    for (const int numPage : listPage) {
        CImageLoadingFormat* imageFormat =
            libPicture.LoadPicture(filename, false, numPage);

        if (imageFormat == nullptr) continue;

        wxFileName pageFilename(
            outputFilename.GetPath(),
            outputFilename.GetName() + "_" + wxString::Format("%d", numPage),
            extension);

        const wxString fileOutput = pageFilename.GetFullPath();

        libPicture.SavePicture(fileOutput, imageFormat, option, quality);

        delete imageFormat;
    }

    const wxString savecompleted =
        CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);

    const wxString infos =
        CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);

    wxMessageBox(savecompleted, infos, wxOK | wxICON_INFORMATION, window);
}

void CSavePicture::SavePicture(wxWindow* window, CImageLoadingFormat* bitmap,
    const wxString& filename) {
    if (filename.empty()) return;

    CLibPicture libPicture;

    const wxString file = SelectExternalFormat(window, filename);

    if (file.empty()) return;

    if (bitmap != nullptr) {
        libPicture.SavePicture(file, bitmap);
    }
    else {
        libPicture.SavePicture(filename, file);
    }

    const wxString savecompleted =
        CLibResource::LoadStringFromResource("LBLSAVEFILECOMPLETED", 1);

    const wxString infos =
        CLibResource::LoadStringFromResource("LBLINFORMATIONS", 1);

    wxMessageBox(savecompleted, infos, wxOK | wxICON_INFORMATION, window);
}