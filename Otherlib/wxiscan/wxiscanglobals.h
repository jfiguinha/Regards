00001 /***************************************************************
00002  * Name:      wxiscanglobals.h
00003  * Purpose:   Definition of some global functions for image
00004  *            manipulation etc.
00005  * Author:    Daniel Nell (daniel.nell@nellresearch.de)
00006  * Created:   2010-11-21
00007  * Copyright: Daniel Nell (www.nellresearch.de)
00008  * License:   GPL
00009  **************************************************************/
00010 
00011 #ifndef WXISCANGLOBALS_H_INCLUDED
00012 #define WXISCANGLOBALS_H_INCLUDED
00013 
00014 /////////////////////////////////////////////////////////////////////////////
00015 //  Global image manipulation functions
00016 //
00017 /// \brief Convert an image to monochrome (black and white).
00018 ///
00019 /// \param oSrcImage        the image to transformed
00020 /// \param nMonoThreshold   value for deciding if a point is black or white
00021 ///
00022 /// NOTE:
00023 ///       The returned object is the transformed image. The original
00024 ///       image is left intact.
00025 ///
00026 wxImage Image2Mono( const wxImage &oSrcImage, int nMonoThreshold= 128 );
00027 
00028 /// \brief Convert an image to greyscale.
00029 ///
00030 /// \param oSrcImage        the image to transformed
00031 ///
00032 /// NOTE:
00033 ///       The returned object is the transformed image. The original
00034 ///       image is left intact.
00035 ///
00036 inline wxImage Image2Grey( const wxImage &oSrcImage )
00037 {
00038     return oSrcImage.ConvertToGreyscale();
00039 }
00040 
00041 /// \brief Crop an image to the given size.
00042 ///
00043 /// \param oSrcImage        the image to transformed
00044 /// \param x                x-coordinate in pixels of the upper left corner of the cropping region.
00045 /// \param y                y-coordinate in pixels of the upper left corner of the cropping region.
00046 /// \param w                Width of the cropping region in pixels.
00047 /// \param h                Height of the cropping region in pixels.
00048 ///
00049 /// NOTE:
00050 ///       The returned object is the transformed image.
00051 ///
00052 inline wxImage CropImage( wxImage oImage, int x, int y, int w, int h )
00053 {
00054     oImage.Resize( wxSize( w + x, h + y ), wxPoint( 0, 0 ), 255, 255, 255 );
00055     return oImage.GetSubImage( wxRect( x, y, w, h ) );
00056 }
00057 
00058 /// \brief Modify brightness, contrast, gamma value, red, green
00059 ///        and blue channel of an image.
00060 ///
00061 /// \param oSrcImage        the image to be transformed
00062 /// \param nBrightness      adjustment value for brightness
00063 /// \param nContrast        adjustment value for constrast
00064 /// \param nGamma           adjustment value for gamma value * 100
00065 /// \param nRed             adjustment value for red value
00066 /// \param nGreen           adjustment value for green value
00067 /// \param nBlue            adjustment value for blue value
00068 ///
00069 /// NOTE:
00070 ///       The returned object is the transformed image. The original
00071 ///       image is left intact.
00072 ///
00073 wxImage EnhenceColours( const wxImage &oSrcImage,
00074                         int nBrightness= 0, int nContrast= 0, int nGamma= 100,
00075                         int nRed= 0, int nGreen= 0, int nBlue= 0 );
00076 
00077 /// \brief Fine rotate an image by its centre.
00078 ///
00079 /// \param oSrcImage        the image to be transformed
00080 /// \param nAngleRadian     the angle in radian
00081 ///
00082 /// NOTE:
00083 ///       The returned object is the transformed image. The original
00084 ///       image is left intact.
00085 ///
00086 inline wxImage RotateImage( const wxImage &oSrcImage, double nAngleRadian )
00087 {
00088     return oSrcImage.Rotate( nAngleRadian, wxPoint( oSrcImage.GetWidth() / 2, oSrcImage.GetHeight() / 2 ), true, NULL );
00089 }
00090 
00091 /// \brief Resize an image to the given size.
00092 ///
00093 /// \param oSrcImage        the image to be transformed
00094 /// \param nDestWidth       the desired width of the destination image
00095 /// \param nDestHeight      the desired height of the destination image
00096 ///
00097 /// NOTE:
00098 ///       The returned object is the transformed image. The original
00099 ///       image is left intact.
00100 ///
00101 inline wxImage ResizeImage( const wxImage &oSrcImage, int nDestWidth, int nDestHeight )
00102 {
00103     return oSrcImage.Scale( nDestWidth, nDestHeight, wxIMAGE_QUALITY_HIGH );
00104 }
00105 
00106 /////////////////////////////////////////////////////////////////////////////
00107 //  Global clipboard functions
00108 //
00109 /// \brief Copy the given text to the sytem clipboard.
00110 ///
00111 /// \param strText          the text to copy to the clipboard
00112 ///
00113 bool CopyText2Clipboard( const wxString &strText );
00114 
00115 /// \brief Copy the given image to the sytem clipboard.
00116 ///
00117 /// \param oImage           the image to copy to the clipboard
00118 ///
00119 bool CopyImage2Clipboard( const wxImage &oImage );
00120 
00121 /////////////////////////////////////////////////////////////////////////////
00122 //  Global multiple usage functions
00123 //
00124 /// \brief Load the content of a text file into memory.
00125 ///
00126 /// \param strFilename      the name of the text file
00127 ///
00128 wxString GetTextFromFile( const wxString &strFilename );
00129 
00130 #endif // WXISCANGLOBALS_H_INCLUDED

