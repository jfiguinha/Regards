00001 /***************************************************************
00002  * Name:      wxiscanglobals.cpp
00003  * Purpose:   Definition of some global functions for image
00004  *            manipulation etc.
00005  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00006  * Created:   2010-11-21
00007  * Copyright: Daniel Nell (www.nellresearch.de)
00008  * License:   GPL
00009  **************************************************************/
00010 
00011 // Include precompiled headers.
00012 #include "wx_pch.h"
00013 
00014 // wxWidgets headers
00015 #include <wx/clipbrd.h>
00016 #include <wx/file.h>
00017 
00018 // Private headers
00019 #include "wxiscanglobals.h"
00020 
00021 
00022 /////////////////////////////////////////////////////////////////////////////
00023 // Global image manipulation functions
00024 //
00025 // Convert an image to monochrome (black and white).
00026 //
00027 wxImage Image2Mono( const wxImage &oSrcImage, int nMonoThreshold )
00028 {
00029     wxImage oImage= oSrcImage.ConvertToGreyscale();
00030 
00031     int nCount= oImage.GetWidth() * oImage.GetHeight() * 3;
00032     unsigned char *pImageDataSrc= oImage.GetData();
00033     unsigned char *pImageDataDest= (unsigned char *)malloc( nCount * sizeof( unsigned char ) );
00034 
00035     if( !pImageDataDest )
00036     {
00037         wxLogError( wxString( wxT( "Image2Mono - " ) )
00038                       + _( "Cannot allocate memory for destination image." ) );
00039         return oImage;
00040     }
00041 
00042     // ...
00043     unsigned char *pIndex= pImageDataDest;
00044 
00045     for( int i= 0; i < nCount; i += 3 )
00046     {
00047         unsigned char nValue= ( (*pImageDataSrc) < nMonoThreshold ) ? 0 : 255;
00048 
00049         // Red.
00050         (*pIndex)= nValue;
00051         pImageDataSrc++;
00052         pIndex++;
00053 
00054         // Green.
00055         (*pIndex)= nValue;
00056         pImageDataSrc++;
00057         pIndex++;
00058 
00059         // Blue.
00060         (*pIndex)= nValue;
00061         pImageDataSrc++;
00062         pIndex++;
00063     }
00064 
00065     oImage.SetData( pImageDataDest );
00066     return oImage;
00067 }
00068 
00069 /// \brief Calculate the value of x^y.
00070 ///
00071 /// \param  x           the base (as a floating point value)
00072 /// \param  y           the exponent (as a floating point value)
00073 ///
00074 /// NOTE:
00075 ///        At the moment this function is internally called by
00076 ///        EnhenceColours() only.
00077 ///
00078 inline static double x_exp_y( double x, double y )
00079 {
00080     return exp( y * log( x ) );
00081 }
00082 
00083 /// \brief Limit the given integer value to the intervall [0..255] (8 bit).
00084 ///
00085 /// \param  x           the integer value to limit
00086 ///
00087 /// NOTE:
00088 ///        At the moment this function is internally called by
00089 ///        EnhenceColours() only.
00090 ///
00091 inline static unsigned char Limit255( int x )
00092 {
00093     return ( x > 255 ) ? 255 : ( ( x < 0 ) ? 0 : x );
00094 }
00095 
00096 // Modify brightness, contrast, gamma value, red, green
00097 // and blue channel of an image (all at the same time).
00098 //
00099 wxImage EnhenceColours( const wxImage &oSrcImage,
00100                         int nBrightness, int nContrast, int nGamma,
00101                         int nRed, int nGreen, int nBlue )
00102 {
00103     wxImage oImage= oSrcImage;
00104 
00105     // ...
00106     int nCount= oImage.GetWidth() * oImage.GetHeight() * 3;
00107     unsigned char *pImageDataSrc= oImage.GetData();
00108     unsigned char *pImageDataDest= (unsigned char *)malloc( nCount * sizeof( unsigned char ) );
00109 
00110     if( !pImageDataDest )
00111     {
00112         wxLogError( wxString( wxT( "EnhenceColours - " ) )
00113                       + _( "Cannot allocate memory for destination image." ) );
00114         return oImage;
00115     }
00116 
00117     // ...
00118     int nIncRed=   nRed   + nBrightness;
00119     int nIncGreen= nGreen + nBrightness;
00120     int nIncBlue=  nBlue  + nBrightness;
00121     double nContrastFactor= exp( (double)nContrast / 75.0 );
00122     double nInvGamma= 100.0 / (double)nGamma;
00123 
00124     // Recalculate the image as a whole. That is calculate brightness,
00125     // contrast, gamma and color correction all at once.
00126     //
00127     // NOTE: We use floating point (double) calculation for not to cut off
00128     //       values as early as 8 bit integer calculation would do (for
00129     //       numeric stability). The disadvantage is that the transformation
00130     //       of one image of about 8 Mega pixels (e. g. an A4 page) even on
00131     //       a 1.6 GHertz machine lasts for more than 5s.
00132     //
00133     unsigned char *pIndex= pImageDataDest;
00134     for( int i= 0; i < nCount; i += 3 )
00135     {
00136         // Red.
00137         (*pIndex)= Limit255( (int)( x_exp_y( ( (double)( (*pImageDataSrc) + nIncRed   ) * nContrastFactor ) / 255.0,
00138                                              nInvGamma ) * 255.0 ) );
00139         pImageDataSrc++;
00140         pIndex++;
00141 
00142         // Green.
00143         (*pIndex)= Limit255( (int)( x_exp_y( ( (double)( (*pImageDataSrc) + nIncGreen ) * nContrastFactor ) / 255.0,
00144                                                nInvGamma ) * 255.0 ) );
00145         pImageDataSrc++;
00146         pIndex++;
00147 
00148         // Blue.
00149         (*pIndex)= Limit255( (int)( x_exp_y( ( (double)( (*pImageDataSrc) + nIncBlue  ) * nContrastFactor ) / 255.0,
00150                                                nInvGamma ) * 255.0 ) );
00151         pImageDataSrc++;
00152         pIndex++;
00153     }
00154 
00155     oImage.SetData( pImageDataDest );
00156     return oImage;
00157 }
00158 
00159 
00160 /////////////////////////////////////////////////////////////////////////////
00161 //  Global clipboard functions
00162 //
00163 // Copy the given text to the sytem clipboard.
00164 //
00165 bool CopyText2Clipboard( const wxString &strText )
00166 {
00167     // Create a text data object for the clipboard.
00168     wxTextDataObject *poTextData=new wxTextDataObject;
00169 
00170     if( !poTextData )
00171     {
00172         wxLogError( wxString( wxT( "wxIScanFrame::CopyText2Clipboard -- " ) )
00173                       + _( "Failed to create clipboard text data object." ) );
00174         return false;
00175     }
00176 
00177     // Copy the text to the text data object for the clipboard.
00178     poTextData->SetText( strText );
00179 
00180     // Open the clipboard and copy the text to the clipboard.
00181     bool bRetC= wxTheClipboard->Open() && wxTheClipboard->SetData( poTextData );
00182 
00183     // Close the clipboard (and release it).
00184     wxTheClipboard->Close();
00185 
00186     // Log errors.
00187     if( !bRetC )
00188     {
00189         wxLogError( wxString( wxT( "wxIScanFrame::CopyText2Clipboard -- " ) )
00190                       + _( "Failed to copy text to clipboard." ) );
00191     }
00192 
00193     // Return the error state.
00194     return bRetC;
00195 }
00196 
00197 // Copy the given image to the sytem clipboard.
00198 //
00199 bool CopyImage2Clipboard( const wxImage &oImage )
00200 {
00201     // Create a image data object for the clipboard.
00202     wxBitmapDataObject *poBitmapData=new wxBitmapDataObject;
00203 
00204     if( !poBitmapData )
00205     {
00206         wxLogError( wxString( wxT( "wxIScanFrame::CopyImage2Clipboard -- " ) )
00207                       + _( "Failed to create clipboard image data object." ) );
00208         return false;
00209     }
00210 
00211     // Copy the image to the image data object for the clipboard.
00212     poBitmapData->SetBitmap( oImage );
00213 
00214     // Open the clipboard and copy the image to the clipboard.
00215     bool bRetC= wxTheClipboard->Open() && wxTheClipboard->SetData( poBitmapData );
00216 
00217     // Close the clipboard (and release it).
00218     wxTheClipboard->Close();
00219 
00220     // Log errors.
00221     if( !bRetC )
00222     {
00223         wxLogError( wxString( wxT( "wxIScanFrame::CopyImage2Clipboard -- " ) )
00224                       + _( "Failed to copy image to clipboard." ) );
00225     }
00226 
00227     // Return the error state.
00228     return bRetC;
00229 }
00230 
00231 
00232 /////////////////////////////////////////////////////////////////////////////
00233 //  Global multiple usage functions
00234 //
00235 // Load the content of a text file into memory.
00236 //
00237 wxString GetTextFromFile( const wxString &strFilename )
00238 {
00239     // Open a textfile, ...
00240     wxFile oFile( strFilename );
00241 
00242     if( !oFile.IsOpened() )
00243     {
00244         // Return an empty string on failure.
00245         return wxEmptyString;
00246     }
00247 
00248     // ... create a temporary buffer of file size, ...
00249     wxFileOffset nLength= oFile.Length();
00250     char *pcBuffer= new char[nLength];
00251 
00252     if( !pcBuffer )
00253     {
00254         // Return an empty string on failure.
00255         return wxEmptyString;
00256     }
00257 
00258     // ... read the file's content into the buffer, ...
00259     oFile.Read( pcBuffer, nLength );
00260 
00261     // ... convert it to a wxString object (from UTF8), ...
00262     wxString strText= wxString::FromUTF8( pcBuffer, nLength );
00263 
00264     // ... delete the (temporary) buffer,  ...
00265     delete[] pcBuffer;
00266 
00267     // ... and return the text.
00268     return strText;
00269 }

