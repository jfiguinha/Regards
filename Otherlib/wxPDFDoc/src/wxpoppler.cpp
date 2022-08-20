/***************************************************************
 * Name:      wxpoppler.cpp
 * Purpose:   wxWidgets wrapper around libpoppler for PDF file
 *            viewing.
 * Author:    Daniel Nell (daniel.nell@nellresearch.de)
 * Created:   2008-07-08
 * Copyright: Daniel Nell (www.nellresearch.de)
 * License:
 **************************************************************/
#include <header.h>
#include <wx/filename.h>
#include <wx/filesys.h>
#include <wx/wxpoppler.h>
#include <ConvertUtility.h>
#include <FileUtility.h>
#include <fstream>
#ifdef wxUSE_PDF
#include <wx/wfstream.h>
#   include <wx/mstream.h>
#   include <wx/pdfdoc.h>
#endif // wxUSE_PDF

/////////////////////////////////////////////////////////////////////////////
//  Class wxPoppler
//


// Initialize static members.
//
int wxPoppler::m_nGlobalDpi= 300;


// Standard constructor.
//
wxPoppler::wxPoppler()
{
    // Initialize normal members;
    m_pPdfPage= nullptr;
    m_pPdfDocument= nullptr;
    m_nDpi= m_nGlobalDpi;

}

// Virtual destructor.
//
wxPoppler::~wxPoppler()
{
    
    // If exists delete the PDF page.
    if( m_pPdfPage != nullptr)
    {
        delete m_pPdfPage;
    }

    // If exists delete the PDF document.
    if( m_pPdfDocument  != nullptr)
    {
       delete m_pPdfDocument;
    }

}



// Opens a PDF stream from file or memory depending on the parameterization.
//
bool wxPoppler::Open(const wxString& strFileName )
{
    wxFileInputStream input(strFileName);
    input.Read(memOut);
    wxStreamBuffer* buffer = memOut.GetOutputStreamBuffer();

    m_pPdfDocument = poppler::document::load_from_raw_data((char *)buffer->GetBufferStart(), buffer->GetBufferSize());

    if( !m_pPdfDocument )
    {
        wxLogError( wxT( "wxPoppler::Open -- GError: %s" ),  "");
        // Signal error.
        return false;
    }
    return true;
}

// Opens a PDF stream from file or memory depending on the parameterization.
bool wxPoppler::Open( char *pcData, const int nLength)
{
    if( pcData && nLength )
    {
        m_pPdfDocument = poppler::document::load_from_raw_data(pcData, nLength);
    }

    if( !m_pPdfDocument )
    {
        wxLogError( wxT( "wxPoppler::Open -- GError: %s" ),  "");
        // Signal error.
        return false;
    }
    return true;    
}

// Select the given page number to render by a consecutive call to RenderPage().
//
bool wxPoppler::SelectPage( int nIndex )
{
	if (m_pPdfDocument != nullptr)
	{
		poppler::page * pPdfPageTemp = m_pPdfDocument->create_page(nIndex);

		if (!pPdfPageTemp)
		{
			return false;
		}
		m_pPdfPage = pPdfPageTemp;

		return true;
	}
	return false;
    
}

// Render the (previously selected) PDF page and store the result in m_oImage.
//
bool wxPoppler::RenderPage()
{
    if( !m_pPdfPage )
    {
        // Signal error.
        wxLogError( wxString( wxT( "wxPoppler::RenderPage -- " ) )
                      + _( "PDF page is not valid." ) );
        return false;
    }
    
    poppler::page_renderer popplerRenderer;

    //double dblWidth, dblHeight;
    //double dblDpiBy72= (double)m_nDpi / 72.0;

    popplerRenderer.set_render_hint(poppler::page_renderer::text_antialiasing);
    poppler::image myimage = popplerRenderer.render_page(m_pPdfPage, m_nDpi, m_nDpi);
		int w = myimage.width();
		int h = myimage.height();
    // Create an wxImage object an set resolution information.
    m_oImage= wxImage( w, h );
    m_oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONUNIT, wxIMAGE_RESOLUTION_INCHES );
    m_oImage.SetOption( wxIMAGE_OPTION_RESOLUTION, m_nDpi );
    m_oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONX, m_nDpi );
    m_oImage.SetOption( wxIMAGE_OPTION_RESOLUTIONY, m_nDpi );

;
    if (myimage.format() == poppler::image::format_rgb24)
    {
        int pointer = 0;
        char * data = myimage.data();
        unsigned char *imgRGB = m_oImage.GetData();    // destination RGB buffer
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                imgRGB[pointer + 2] = *data++;  // R
                imgRGB[pointer + 1] = *data++;   // G
                imgRGB[pointer + 0] = *data++;   // B
                pointer += 3;
            }
        }
        //Mat(myimage.height(), myimage.width(), CV_8UC3, myimage.data()).copyTo(cvimg);
    }
    else if (myimage.format() == poppler::image::format_argb32) {
        m_oImage.InitAlpha();
        int pointer = 0;
        char * data = myimage.data();
        unsigned char *imgRGB = m_oImage.GetData();    // destination RGB buffer
        unsigned char *imgAlpha = m_oImage.GetAlpha(); // destination alpha buffer
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                imgRGB[pointer + 2] = *data++;  // R
                imgRGB[pointer + 1] = *data++;   // G
                imgRGB[pointer + 0] = *data++;   // B
                pointer += 3;
                if (imgAlpha)
                    *imgAlpha++ = *data++;
            }
        }
    }
    else {
        cerr << "PDF format no good\n";
    }

    return true;
}



#ifdef wxUSE_PDF
/////////////////////////////////////////////////////////////////////////////
//  Class wxPDFHandler
//
IMPLEMENT_DYNAMIC_CLASS( wxPDFHandler, wxImageHandler )

// Loads an image from a stream, putting the resulting data into image.
//
bool wxPDFHandler::LoadFile( wxImage* poImage, wxInputStream& oIStream, bool bVerbose, int nIndex )
{
    // Check if there is a valid PDF file.
    if( !CanRead( oIStream ) )
    {
        if( bVerbose )
        {
            wxLogError( wxString( wxT( "wxPDFHandler::LoadFile -- " ) )
                          + _( "PDF: This is not a PDF file." ) );
        }
        return false;
    }

    // Write the input stream to a memory buffer (by using a wxMemoryOutputStream object).
    wxFileOffset oOffset= oIStream.TellI();
    oIStream.SeekI( 0, wxFromEnd );
    wxFileOffset nSize= oIStream.TellI();
    oIStream.SeekI( 0 );
    char *pcData= new char[nSize];
    wxMemoryOutputStream oOStream( pcData, nSize );
    oIStream.Read( oOStream );
    oIStream.SeekI( oOffset );

    int bRetC= true;
    int nPage= ( nIndex >= 0 ) ? nIndex : 0;
    wxPoppler oPoppler;

    // Render an image of the selected page.
    if(    oPoppler.Open( (char *)oOStream.GetOutputStreamBuffer()->GetBufferStart(),
                          oOStream.GetOutputStreamBuffer()->GetBufferSize() )
        && oPoppler.SelectPage( nPage )
        && oPoppler.RenderPage()
      )
    {
        (*poImage)= oPoppler.GetImage();
        m_nPageCount= oPoppler.GetPageCount();
    }
    else
    {
        if( bVerbose )
        {
            wxLogError( wxString( wxT( "wxPDFHandler::LoadFile -- " ) )
                          + _( "Cannot render temporary PDF file." ) );
        }
        bRetC= false;
    }
    delete[] pcData;
    return bRetC;
}

// Saves a given image in the output stream.
//
bool wxPDFHandler::SaveFile( wxImage* poImage, wxOutputStream& oOStream, bool bVerbose )
{
    if( poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONUNIT ) )
    {
        int nResolutionUnit= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONUNIT );
        int nResolution= 0;

        // Get image resolution-
        if(  poImage->HasOption( wxIMAGE_OPTION_RESOLUTION ) )
        {
            nResolution= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTION );
        }
        else if(  poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONX ) && (  poImage->HasOption( wxIMAGE_OPTION_RESOLUTIONY ) ) )
        {
            int nResolutionX= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONX );
            int nResolutionY= poImage->GetOptionInt( wxIMAGE_OPTION_RESOLUTIONY );

            if( nResolutionX == nResolutionY )
            {
                nResolution= nResolutionX;
            }
        }
        if( nResolution )
        {
            // Save image in a temporary file.
            wxString strTempFileName= wxFileName::CreateTempFileName( wxEmptyString );

            poImage->SaveFile( strTempFileName, wxT( "image/jpeg" ) );

            // Create a PDF document, add a page, and put the image on it.
            wxPdfDocument oPdfDocument;

            oPdfDocument.AddPage( ( poImage->GetHeight() > poImage->GetWidth() ) ? wxPORTRAIT : wxLANDSCAPE );

            oPdfDocument.SetImageScale( (double)nResolution * ( nResolutionUnit == wxIMAGE_RESOLUTION_CM ? 2.54 : 1.0 ) / 72.0 );
            oPdfDocument.Image( strTempFileName, 0 , 0, 0, 0, wxT( "image/jpeg" ) );

            // Remove temporary file.
            ::wxRemoveFile( strTempFileName );

            // Send the PDF to the output stream.
            wxMemoryInputStream oMemoryInputStream( oPdfDocument.CloseAndGetBuffer() );

            oOStream.Write( oMemoryInputStream );

            // Signal success.
            return true;
        }
    }
    // File saving not possible or not implemented.
    return false;
}

// Check if the file contains a PDF document.
//
bool wxPDFHandler::DoCanRead( wxInputStream& oIStream )
{
    unsigned char aucHeader[4];

    if ( !oIStream.Read( aucHeader, WXSIZEOF( aucHeader ) ) )
    {
        return false;
    }
    return memcmp( aucHeader, "%PDF", WXSIZEOF( aucHeader ) ) == 0;
}
#endif // wxUSE_PDF


