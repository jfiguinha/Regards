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
#include <qpdf/QPDFWriter.hh>

#include <LibResource.h>
#include <ConvertUtility.h>

#include <wx/busyinfo.h>

#include <algorithm>
#include <cstdio>
#include <string>
#include <unordered_set>
#include <vector>

using namespace Regards::Picture;

namespace
{
    constexpr int DEFAULT_PDF_DPI = 300;

    std::unordered_set<int> MakePageSet(const std::vector<int>& pages)
    {
        std::unordered_set<int> result;
        result.reserve(pages.size());

        for (const int page : pages)
        {
            if (page >= 0)
                result.insert(page);
        }

        return result;
    }

    bool SaveImageToFile(wxImage& image,
        const wxString& filename,
        int option,
        int quality)
    {
        if (!image.IsOk())
            return false;

        if (option == 0)
        {
            image.SetOption("wxIMAGE_OPTION_QUALITY",
                std::clamp(quality, 0, 100));

            return image.SaveFile(filename, wxBITMAP_TYPE_JPEG);
        }

        image.SetOption("wxIMAGE_OPTION_TIFF_COMPRESSION", 5);
        return image.SaveFile(filename, wxBITMAP_TYPE_TIFF);
    }

    bool ReplaceFile(const wxString& temporaryFile,
        const wxString& destinationFile)
    {
        if (temporaryFile.empty() || destinationFile.empty())
            return false;

        if (!wxFileExists(temporaryFile))
            return false;

        // Remove the destination only after the new file has been
        // successfully generated.
        if (wxFileExists(destinationFile) &&
            !wxRemoveFile(destinationFile))
        {
            return false;
        }

        if (!wxCopyFile(temporaryFile, destinationFile, true))
            return false;

        wxRemoveFile(temporaryFile);
        return true;
    }

    int GetImageDpi(const wxImage& image)
    {
        if (image.HasOption(wxIMAGE_OPTION_RESOLUTION))
        {
            const int dpi =
                image.GetOptionInt(wxIMAGE_OPTION_RESOLUTION);

            if (dpi > 0)
                return dpi;
        }

        if (image.HasOption(wxIMAGE_OPTION_RESOLUTIONX) &&
            image.HasOption(wxIMAGE_OPTION_RESOLUTIONY))
        {
            const int dpiX =
                image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONX);
            const int dpiY =
                image.GetOptionInt(wxIMAGE_OPTION_RESOLUTIONY);

            if (dpiX > 0 && dpiX == dpiY)
                return dpiX;
        }

        return DEFAULT_PDF_DPI;
    }

    void FitImageToPage(wxPdfDocument& pdf,
        const wxImage& image,
        double& x,
        double& y,
        double& width,
        double& height)
    {
        const double pageWidth = pdf.GetPageWidth();
        const double pageHeight = pdf.GetPageHeight();

        if (!image.IsOk() ||
            image.GetWidth() <= 0 ||
            image.GetHeight() <= 0)
        {
            x = y = width = height = 0.0;
            return;
        }

        const double imageWidth =
            static_cast<double>(image.GetWidth());

        const double imageHeight =
            static_cast<double>(image.GetHeight());

        const double scaleX = pageWidth / imageWidth;
        const double scaleY = pageHeight / imageHeight;

        const double scale = std::min(scaleX, scaleY);

        width = imageWidth * scale;
        height = imageHeight * scale;

        x = (pageWidth - width) * 0.5;
        y = (pageHeight - height) * 0.5;
    }
}

CRegardsPDF::CRegardsPDF(const wxString& filename)
    : poppler(new wxPoppler()),
    isValid(false)
{
    if (!poppler)
        return;

    if (!poppler->Open(filename))
        return;

    // Keep the PDF object valid independently of later page operations.
    isValid = poppler->SetDpi(300);
}

CRegardsPDF::~CRegardsPDF()
{
    delete poppler;
    poppler = nullptr;
}

bool CRegardsPDF::IsOk()
{
    return poppler != nullptr && isValid;
}

wxImage CRegardsPDF::GetPicture(const int& numPicture)
{
    if (!IsOk() || numPicture < 0)
        return {};

    if (!poppler->SelectPage(numPicture))
        return {};

    if (!poppler->RenderPage())
        return {};

    wxImage image = poppler->GetImage();

    return image;
}

int CRegardsPDF::GetNbFrame(const wxString& filename, bool& error)
{
    error = true;

    wxPoppler localPoppler;

    if (!localPoppler.Open(filename))
        return 0;

    const int pageCount = localPoppler.GetPageCount();

    error = pageCount < 0;

    return error ? 0 : pageCount;
}

void CRegardsPDF::SavePictureToPdf(const wxString& fileName,
    CImageLoadingFormat* bitmap,
    const int& option,
    const int& quality)
{
    if (fileName.empty() || bitmap == nullptr)
        return;

    wxImage image = bitmap->GetwxImage();

    if (!image.IsOk())
        return;

    const wxString temporaryFile =
        CFileUtility::GetTempFile(
            option == 0 ? "temporary.jpg" : "temporary.tiff");

    if (temporaryFile.empty())
        return;

    if (!SaveImageToFile(image, temporaryFile, option, quality))
        return;

    SaveToPDF(&image, fileName, temporaryFile, option, quality);

    wxRemoveFile(temporaryFile);
}

void CRegardsPDF::SavePicture(const wxString& fileName,
    CImageLoadingFormat* bitmap,
    const int& option,
    const int& quality)
{
    if (fileName.empty() || bitmap == nullptr)
        return;

    wxImage image = bitmap->GetwxImage();

    if (!image.IsOk())
        return;

    wxLogNull logNo;

    if (wxFileExists(fileName))
    {
        if (!wxRemoveFile(fileName))
            return;
    }

    SaveImageToFile(image, fileName, option, quality);
}

int CRegardsPDF::SavePictureOption(int& option, int& quality)
{
    int returnValue = 0;

    PDFOption pdfOption(nullptr);

    if (pdfOption.ShowModal() != wxID_OK)
        return returnValue;

    if (!pdfOption.IsOk())
        return returnValue;

    option = pdfOption.CompressionOption();

    switch (option)
    {
    case 0:
    {
        CompressionOption jpegOption(nullptr);

        if (jpegOption.ShowModal() != wxID_OK)
            return returnValue;

        if (!jpegOption.IsOk())
            return returnValue;

        quality = jpegOption.CompressionLevel();
        break;
    }

    case 1:
    {
        TiffOption tiffOption(nullptr);

        if (tiffOption.ShowModal() != wxID_OK)
            return returnValue;

        if (!tiffOption.IsOk())
            return returnValue;

        quality = tiffOption.CompressionOption();
        break;
    }

    default:
        break;
    }

    returnValue = 1;
    return returnValue;
}

bool CRegardsPDF::SaveToPDF(wxImage* poImage,
    const wxString& pdfFile,
    int option,
    int quality)
{
    if (poImage == nullptr || !poImage->IsOk() || pdfFile.empty())
        return false;

    const wxString temporaryFile =
        CFileUtility::GetTempFile(
            option == 0 ? "scanner.jpg" : "scanner.tif");

    if (temporaryFile.empty())
        return false;

    if (!SaveImageToFile(*poImage, temporaryFile, option, quality))
        return false;

    const bool result =
        SaveToPDF(poImage,
            pdfFile,
            temporaryFile,
            option,
            quality);

    wxRemoveFile(temporaryFile);

    return result;
}

bool CRegardsPDF::SaveToPDF(wxImage* poImage,
    const wxString& fileName,
    const wxString& pictureName,
    int option,
    int quality)
{
    if (poImage == nullptr ||
        !poImage->IsOk() ||
        fileName.empty() ||
        pictureName.empty())
    {
        return false;
    }

    if (!wxFileExists(pictureName))
        return false;

    wxLogNull logNo;

    const wxPrintOrientation orientation =
        (poImage->GetHeight() > poImage->GetWidth())
        ? wxPORTRAIT
        : wxLANDSCAPE;

    wxPdfDocument pdf;
    pdf.AddPage(orientation);

    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    FitImageToPage(pdf,
        *poImage,
        x,
        y,
        width,
        height);

    if (width <= 0.0 || height <= 0.0)
        return false;

    const wxString imageType =
        option == 0 ? wxT("image/jpeg") : wxT("image/tiff");

    pdf.Image(pictureName,
        x,
        y,
        width,
        height,
        imageType);

    pdf.Close();

    // wxPdfDocument writes directly to the destination. Keep the
    // existing public API and report success when the destination exists.
    pdf.SaveAsFile(fileName);

    return wxFileExists(fileName);
}

int CRegardsPDF::SavePicture(const wxString& fileName,
    CImageLoadingFormat* bitmap)
{
    if (fileName.empty() || bitmap == nullptr)
        return 0;

    int option = 0;
    int quality = 0;

    if (SavePictureOption(option, quality) != 1)
        return 0;

    SavePictureToPdf(fileName, bitmap, option, quality);

    return wxFileExists(fileName) ? 1 : 0;
}

void CRegardsPDF::AddPdfPage(wxPdfDocument* oPdfDocument,
    CImageLoadingFormat* imageFormat,
    int option,
    int quality,
    int numpage)
{
    if (oPdfDocument == nullptr ||
        imageFormat == nullptr ||
        numpage < 0)
    {
        return;
    }

    wxString file =
        "temporary" + to_string(numpage);

    file += option == 0 ? ".jpg" : ".tiff";
    file = CFileUtility::GetTempFile(file);

    if (file.empty())
        return;

    SavePicture(file, imageFormat, option, quality);

    if (!wxFileExists(file))
        return;

    wxImage image;

    if (!image.LoadFile(file) || !image.IsOk())
    {
        wxRemoveFile(file);
        return;
    }

    const wxPrintOrientation orientation =
        (image.GetHeight() > image.GetWidth())
        ? wxPORTRAIT
        : wxLANDSCAPE;

    oPdfDocument->AddPage(orientation);

    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    FitImageToPage(*oPdfDocument,
        image,
        x,
        y,
        width,
        height);

    if (width > 0.0 && height > 0.0)
    {
        const wxString imageType =
            option == 0 ? wxT("image/jpeg") : wxT("image/tiff");

        oPdfDocument->Image(file,
            x,
            y,
            width,
            height,
            imageType);
    }

    wxRemoveFile(file);
}

wxString CRegardsPDF::ExtractPage(const wxString& filename,
    const vector<int>& listPage)
{
    if (filename.empty() ||
        !wxFileExists(filename) ||
        listPage.empty())
    {
        return {};
    }

    const wxString file =
        CFileUtility::GetTempFile("extract.pdf");

    if (file.empty())
        return {};

    try
    {
        QPDF inpdf;
        inpdf.processFile(
            CConvertUtility::ConvertToStdString(filename).c_str());

        const std::vector<QPDFObjectHandle>& pages =
            inpdf.getAllPages();

        const std::unordered_set<int> pagesToExtract =
            MakePageSet(listPage);

        QPDF outpdf;
        outpdf.emptyPDF();

        for (size_t pageNumber = 0;
            pageNumber < pages.size();
            ++pageNumber)
        {
            if (pagesToExtract.find(
                static_cast<int>(pageNumber)) ==
                pagesToExtract.end())
            {
                continue;
            }

            outpdf.addPage(pages[pageNumber], false);
        }

        QPDFWriter writer(
            outpdf,
            CConvertUtility::ConvertToStdString(file).c_str());

        writer.setCompressStreams(true);
        writer.setRecompressFlate(true);
        writer.write();

        return wxFileExists(file) ? file : wxString();
    }
    catch (...)
    {
        wxRemoveFile(file);
        return {};
    }
}

void CRegardsPDF::AddPage(const wxString& fileToAdd,
    const wxString& filename,
    const vector<int>& listPage,
    int oldAnimationPosition)
{
    if (fileToAdd.empty() ||
        !wxFileExists(fileToAdd) ||
        listPage.empty())
    {
        return;
    }

    const wxString file =
        CFileUtility::GetTempFile("add.pdf");

    if (file.empty())
        return;

#ifndef DEMO
    wxBusyInfo wait(
        CLibResource::LoadStringFromResource(
            L"LBLBUSYINFO", 1));
#endif

    try
    {
        const bool fileIn =
            !filename.empty() && wxFileExists(filename);

        QPDF oldpdf;
        if (fileIn)
        {
            oldpdf.processFile(
                CConvertUtility::ConvertToStdString(filename).c_str());
        }

        QPDF inpdf;
        inpdf.processFile(
            CConvertUtility::ConvertToStdString(fileToAdd).c_str());

        QPDF outpdf;
        outpdf.emptyPDF();

        if (fileIn)
        {
            const std::vector<QPDFObjectHandle>& oldpages =
                oldpdf.getAllPages();

            const size_t oldPageCount = oldpages.size();

            size_t insertPosition = 0;

            if (oldAnimationPosition >= 0)
            {
                insertPosition = std::min(
                    static_cast<size_t>(oldAnimationPosition) + 1,
                    oldPageCount);
            }

            for (size_t i = 0; i < insertPosition; ++i)
            {
                outpdf.addPage(oldpages[i], false);
            }
        }

        const std::vector<QPDFObjectHandle>& pages =
            inpdf.getAllPages();

        const std::unordered_set<int> pagesToAdd =
            MakePageSet(listPage);

        for (size_t pageNumber = 0;
            pageNumber < pages.size();
            ++pageNumber)
        {
            if (pagesToAdd.find(
                static_cast<int>(pageNumber)) ==
                pagesToAdd.end())
            {
                continue;
            }

            outpdf.addPage(pages[pageNumber], false);
        }

        if (fileIn)
        {
            const std::vector<QPDFObjectHandle>& oldpages =
                oldpdf.getAllPages();

            const size_t oldPageCount = oldpages.size();

            size_t insertPosition = 0;

            if (oldAnimationPosition >= 0)
            {
                insertPosition = std::min(
                    static_cast<size_t>(oldAnimationPosition) + 1,
                    oldPageCount);
            }

            for (size_t i = insertPosition;
                i < oldPageCount;
                ++i)
            {
                outpdf.addPage(oldpages[i], false);
            }
        }

        QPDFWriter writer(
            outpdf,
            CConvertUtility::ConvertToStdString(file).c_str());

        writer.setCompressStreams(true);
        writer.setRecompressFlate(true);
        writer.write();

#ifndef DEMO
        if (!wxFileExists(file))
            return;

        if (filename.empty())
            return;

        ReplaceFile(file, filename);
#endif
    }
    catch (...)
    {
        wxRemoveFile(file);
    }
}

void CRegardsPDF::RemovePage(const wxString& filename,
    const vector<int>& listPage)
{
    if (filename.empty() ||
        !wxFileExists(filename) ||
        listPage.empty())
    {
        return;
    }

    const wxString file =
        CFileUtility::GetTempFile("delete.pdf");

    if (file.empty())
        return;

#ifndef DEMO
    wxBusyInfo wait(
        CLibResource::LoadStringFromResource(
            L"LBLBUSYINFO", 1));
#endif

    try
    {
        QPDF inpdf;
        inpdf.processFile(
            CConvertUtility::ConvertToStdString(filename).c_str());

        const std::vector<QPDFObjectHandle>& pages =
            inpdf.getAllPages();

        const std::unordered_set<int> pagesToRemove =
            MakePageSet(listPage);

        QPDF outpdf;
        outpdf.emptyPDF();

        for (size_t pageNumber = 0;
            pageNumber < pages.size();
            ++pageNumber)
        {
            if (pagesToRemove.find(
                static_cast<int>(pageNumber)) !=
                pagesToRemove.end())
            {
                continue;
            }

            outpdf.addPage(pages[pageNumber], false);
        }

        QPDFWriter writer(
            outpdf,
            CConvertUtility::ConvertToStdString(file).c_str());

        writer.setCompressStreams(true);
        writer.setRecompressFlate(true);
        writer.write();

#ifndef DEMO
        if (!wxFileExists(file))
            return;

        ReplaceFile(file, filename);
#endif
    }
    catch (...)
    {
        wxRemoveFile(file);
    }
}
