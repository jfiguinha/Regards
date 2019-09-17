/***************************************************************
 * Name:      wxpoppler.h
 * Purpose:   wxWidgets wrapper around libpoppler for PDF file
 *            viewing.
 * Author:    Daniel Nell (daniel.nell@nellresearch.de)
 * Created:   2008-07-08
 * Copyright: Daniel Nell (www.nellresearch.de)
 * License:
 **************************************************************/

#ifndef WXPOPPLER_H
#define WXPOPPLER_H
#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-page-renderer.h>
#include <wx/image.h>

/////////////////////////////////////////////////////////////////////////////
//  Class wxPoppler
//
/// \brief A class to handle PDF files using libpoppler.
///
/// ...
class wxPoppler
{
  public:
    /// \brief Standard constructor.
    wxPoppler();

    /// \brief Virtual destructor.
    virtual ~wxPoppler();

  public:
    /// \brief Opens a PDF file.
    ///
    /// \param strFileName          The name of the PDF file.
    ///
    /// \return                     Returns true on success and false otherwise.
    virtual bool Open( const wxString& strFileName );
    
    /// \brief Opens a PDF file in memory.
    ///
    /// \param pcData               A pointer to the buffer containing the PDF document.
    /// \param nLength              The length (in Byte) of the PDF document.
    ///
    /// \return                     Returns true on success and false otherwise.
    virtual bool Open( char *pcData, const int nLength );


    /// \brief Get the number of pages in the PDF document.
    virtual int  GetPageCount()
    {
        if(m_pPdfDocument != nullptr)
        return m_pPdfDocument->pages();
    }

    /// \brief Select the given page number to render by a consecutive call to RenderPage().
    ///
    /// \param nIndex               Zero based page number.
    ///
    /// \return                     Returns true on success and false otherwise.
    virtual bool SelectPage( int nIndex );

    /// \brief Render the (previously selected) PDF page and store the result in m_oImage.
    virtual bool RenderPage();                        ;

    /// \brief Set the dpi parameter to the given value. Returns the old dpi value.
    ///
    /// \param nDpi                 The new dpi value.
    ///
    /// \return                     Returns the old dpi value.
    virtual int SetDpi( int nDpi )
    {
        int nOldDpi= m_nDpi;

        m_nDpi= nDpi;
        return nOldDpi;
    }

    /// \brief Get the rendered PDF page as wxImage object.
    virtual wxImage GetImage()
    {
        return m_oImage;
    }


  public:
    static int m_nGlobalDpi;                              ///< Holds the standart image resolution in dots per inch for presetting m_nDpi.

  protected:
    wxImage m_oImage;                                     ///< Image object rendered by RenderPage().
    int m_nDpi;                                           ///< Holds the image resolution in dots per inch.
    poppler::page * m_pPdfPage;                              ///< A pointer to the current PDF page.
    poppler::document * m_pPdfDocument;                      ///< A pointer to the current PDF document.
};


#if wxUSE_PDF
/////////////////////////////////////////////////////////////////////////////
//  Enum wxBitmapTypePdf
//
/// \brief ...
///
/// ...
enum wxBitmapTypePdf
{
    wxBITMAP_TYPE_PDF = wxBITMAP_TYPE_ANY + 1047
};


/////////////////////////////////////////////////////////////////////////////
//  Class wxPDFHandler
//
/// \brief Image handler to handle PDF files in a way e. g. the BMP or JPEG
///        file handler does it.
///
/// ...
///
/// NOTE:
///
/// Only opening of PDF files is implemented, yet. But it is possible to
/// implement saving, too, using wxPdfDocument.
///
class wxPDFHandler : public wxImageHandler
{
  public:
    /// Standard constructor.
    wxPDFHandler()
      : m_nPageCount( 0 )
    {
        m_name= wxT( "wxPDFHandler" );
        m_extension= wxT( "pdf" );
        m_type= (wxBitmapType)wxBITMAP_TYPE_PDF;
        m_mime= wxT( "application/pdf" );
    }


#if wxUSE_STREAMS
  public:
    /// \brief Loads an image from a stream, putting the resulting data
    ///        into image. If the image file contains more than one image,
    ///        nIndex indicates which image to read from the stream.
    ///
    /// \param poImage      A pointer to the image object which
    ///                     is to be affected by this operation.
    /// \param oIStream     Opened input stream for reading image data.
    /// \param bVerbose     If set to true, errors reported by the image
    ///                     handler will produce wxLogMessages.
    /// \param nIndex       The index of the image in the file (starting
    ///                     from zero).
    virtual bool LoadFile( wxImage* poImage, wxInputStream& oIStream, bool bVerbose= true, int nIndex= 0 );

    /// \brief Saves a given image in the output stream.
    ///
    /// \param poImage      A pointer to the image object which
    ///                     is to be affected by this operation.
    /// \param oOStream     Opened output stream for writing the data.
    virtual bool SaveFile(wxImage* poImage, wxOutputStream& oOStream, bool bVerbose= true );

#  if wxCHECK_VERSION( 2, 9, 0 )
#  else  // wxCHECK_VERSION( 2, 9, 0 )
    /// \brief If the image file contains more than one image, this
    ///        function will return the number of available images.
    ///
    /// \param oIStream     Opened input stream for reading image data
    ///                     (ignored).
    virtual int GetImageCount( wxInputStream& WXUNUSED( oIStream ) ){ return m_nPageCount; }
#  endif // wxCHECK_VERSION( 2, 9, 0 )


  protected:
    /// Check if the file contains a PDF document.
    ///
    /// \param oIStream     Opened input stream for reading image data.
    virtual bool DoCanRead( wxInputStream& oIStream );

#  if wxCHECK_VERSION( 2, 9, 0 )
    /// \brief If the image file contains more than one image, this
    ///        function will return the number of available images.
    ///
    /// \param oIStream     Opened input stream for reading image data
    ///                     (ignored).
    virtual int DoGetImageCount( wxInputStream& WXUNUSED( oIStream ) ){ return m_nPageCount; }
#  else  // wxCHECK_VERSION( 2, 9, 0 )
#  endif // wxCHECK_VERSION( 2, 9, 0 )
#endif // wxUSE_STREAMS

  private:
    int m_nPageCount;   ///< Number of pages contained in the PDF file.

  private:
    DECLARE_DYNAMIC_CLASS( wxPDFHandler )
};
#endif // wxUSE_PDF

#endif // WXPOPPLER_H


