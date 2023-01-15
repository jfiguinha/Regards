// InsertResource.cpp : définit le point d'entrée pour l'application console.
//
#include "header.h"
#define REGARDS_RESOURCE

float clamp(float val, float minval, float maxval)
{
	return std::clamp(val, minval, maxval);
}

#ifdef REGARDS_RESOURCE


#include "SqlInit.h"
#include <SqlResource.h>
#include <wx/textfile.h>
using namespace Regards::Sqlite;


wxString to_string(const float& value)
{
	wxString chaine;
	chaine << value;
	return chaine;
}


float value[256];

void WriteToFile(const wxString& filename, const wstring& text)
{
	wxTextFile file(filename);
	file.Create();

	file.AddLine(text);

	file.Write();
	file.Close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	CSqlInit::InitializeSQLServerDatabase();
	CSqlResource sqlResource;
	wxString path = "C:\\developpement\\git\\Regards\\Regards\\Resources\\vector\\";
	wxInitAllImageHandlers();
	wxString icone = sqlResource.GetVector("IDB_FLIPHORZ");
	WriteToFile(path + "IDB_FLIPHORZ.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_FLIPVERT");
	WriteToFile(path + "IDB_FLIPVERT.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PICTUREDETAILS");
	WriteToFile(path + "IDB_PICTUREDETAILS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_BOULESLIDER");
	WriteToFile(path + "IDB_BOULESLIDER.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_VOLUME_UP_VIDEO");
	WriteToFile(path + "IDB_VOLUME_UP_VIDEO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_VOLUME_DOWN_VIDEO");
	WriteToFile(path + "IDB_VOLUME_DOWN_VIDEO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PHOTO");
	WriteToFile(path + "IDB_PHOTO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_OK");
	WriteToFile(path + "IDB_OK.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CANCEL");
	WriteToFile(path + "IDB_CANCEL.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_DELETE");
	WriteToFile(path + "IDB_DELETE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CROP");
	WriteToFile(path + "IDB_CROP.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_EMAIL");
	WriteToFile(path + "IDB_EMAIL.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_EXIT");
	WriteToFile(path + "IDB_EXIT.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_FOLDER");
	WriteToFile(path + "IDB_FOLDER.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_FOLDER_REFRESH");
	WriteToFile(path + "IDB_FOLDER_REFRESH.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_INFOS");
	WriteToFile(path + "IDB_INFOS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_MULTIPLESELECT");
	WriteToFile(path + "IDB_MULTIPLESELECT.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_NEWPNG");
	WriteToFile(path + "IDB_NEWPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PHOTOVIEWERPNG");
	WriteToFile(path + "IDB_PHOTOVIEWERPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PICTURETOOLBAR");
	WriteToFile(path + "IDB_PICTURETOOLBAR.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PICTURE");
	WriteToFile(path + "IDB_PICTURE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PRINTERPNG");
	WriteToFile(path + "IDB_PRINTERPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ROTATION270");
	WriteToFile(path + "IDB_ROTATION270.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ROTATION90");
	WriteToFile(path + "IDB_ROTATION90.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_SAVE");
	WriteToFile(path + "IDB_SAVE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_SHRINK");
	WriteToFile(path + "IDB_SHRINK.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CHECKBOX_ON");
	WriteToFile(path + "IDB_CHECKBOX_ON.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CHECKBOX_OFF");
	WriteToFile(path + "IDB_CHECKBOX_OFF.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PLAY");
	WriteToFile(path + "IDB_PLAY.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_STOP");
	WriteToFile(path + "IDB_STOP.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PAUSE");
	WriteToFile(path + "IDB_PAUSE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_THUMBNAILPNG");
	WriteToFile(path + "IDB_THUMBNAILPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ZOOMMOINS");
	WriteToFile(path + "IDB_ZOOMMOINS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ZOOMPLUS");
	WriteToFile(path + "IDB_ZOOMPLUS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PLAY_VIDEO");
	WriteToFile(path + "IDB_PLAY_VIDEO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PAUSE_VIDEO");
	WriteToFile(path + "IDB_PAUSE_VIDEO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_VOLUME_SPEAKER_VIDEO");
	WriteToFile(path + "IDB_VOLUME_SPEAKER_VIDEO.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_SCREENPNG");
	WriteToFile(path + "IDB_SCREENPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PLUS");
	WriteToFile(path + "IDB_PLUS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_MINUS");
	WriteToFile(path + "IDB_MINUS.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_BUTTON_CLICK");
	WriteToFile(path + "IDB_BUTTON_CLICK.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_BUTTON_CLICK_VERTICAL");
	WriteToFile(path + "IDB_BUTTON_CLICK_VERTICAL.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_LINK_ASSOC");
	WriteToFile(path + "IDB_LINK_ASSOC.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_EXPORT");
	WriteToFile(path + "IDB_EXPORT.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_SEARCH");
	WriteToFile(path + "IDB_SEARCH.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CALENDAR");
	WriteToFile(path + "IDB_CALENDAR.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_MAPLOCATION");
	WriteToFile(path + "IDB_MAPLOCATION.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PHOTOTEMP");
	WriteToFile(path + "IDB_PHOTOTEMP.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_CIRCLE");
	WriteToFile(path + "IDB_CIRCLE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ARROWRPNG");
	WriteToFile(path + "IDB_ARROWRPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ARROWLPNG");
	WriteToFile(path + "IDB_ARROWLPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ARROWTRACKRPNG");
	WriteToFile(path + "IDB_ARROWTRACKRPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_ARROWTRACKLPNG");
	WriteToFile(path + "IDB_ARROWTRACKLPNG.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_EDIT_LABEL");
	WriteToFile(path + "IDB_EDIT_LABEL.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PEOPLE_FACE");
	WriteToFile(path + "IDB_PEOPLE_FACE.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_PHOTOINDEX");
	WriteToFile(path + "IDB_PHOTOINDEX.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_SCANNER");
	WriteToFile(path + "IDB_SCANNER.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_EXTRACT");
	WriteToFile(path + "IDB_EXTRACT.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_STAREMPTY");
	WriteToFile(path + "IDB_STAREMPTY.svg", icone.ToStdWstring());
	icone = sqlResource.GetVector("IDB_STARYELLOW");
	WriteToFile(path + "IDB_STARYELLOW.svg", icone.ToStdWstring());

	//sqlResource.InsertVector(L"IDB_EXTRACT", L"C:\\developpement\\git\\Regards\\Resource\\res\\vector\\extract.svg");
	//sqlResource.InsertVector(L"IDB_OPEN", L"C:\\developpement\\git\\Regards\\Resource\\res\\vector\\open.svg");
	//sqlResource.InsertVector(L"IDB_STARYELLOW", L"C:\\developpement\\git\\svg\\star_yellow.svg");
	/*
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_BLUR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_BLUR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_BOXBLUR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_BOXBLUR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_EDGE", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_EDGE.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_EMBOSS", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_EMBOSS.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FASTGAUSSIANBLUR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FASTGAUSSIANBLUR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_GAUSSIANBLUR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_GAUSSIANBLUR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_MOTIONBLUR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_MOTIONBLUR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_SHARPEN", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_SHARPEN.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_SHARPENSTRONG", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_SHARPENSTRONG.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_SOFTEN", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_SOFTEN.cl");
	*/
	/*
	List des shaders
	//IDR_GLSL_SHADER_PHOTOFILTRE
	//IDR_GLSL_SHADER_RGB
	IDR_GLSL_SHADER_VIDEO
	IDR_GLSL_ALPHA_SHADER

	IDR_OPENCL_BM3D
	IDR_OPENCL_TEXTURE
	IDR_OPENCL_CONVERTTOY
	IDR_OPENCL_RAW
	IDR_OPENCL_CONVOLUTION

	IDR_OPENCL_COLORCONVERSION
	IDR_OPENCL_FFMPEG
	IDR_OPENCL_FFMPEGDXVA2YUV420
	IDR_OPENCL_FFMPEGYUV420
	IDR_OPENCL_FUSION
	IDR_BILATERALFILTER_RAW
	IDR_OPENCL_SHARPENMASKING
	IDR_OPENCL_COLOR
	IDR_OPENCL_INTERPOLATION
	IDR_OPENCL_CONVOLUTION
	IDR_OPENCL_MOSAIC
	IDR_OPENCL_DILATEERODE
	IDR_OPENCL_MEDIAN
	IDR_OPENCL_NOISE
	IDR_OPENCL_FLIP
	IDR_OPENCL_SWIRL
	IDR_OPENCL_ROTATION
	*/

	//sqlResource.GetText("")
	/*
	wstring text_shader = L"";
	text_shader = sqlResource.GetText("IDR_GLSL_SHADER_VIDEO");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_GLSL_SHADER_VIDEO.gl", text_shader);
	text_shader = sqlResource.GetText("IDR_GLSL_ALPHA_SHADER");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_GLSL_ALPHA_SHADER.gl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_BM3D");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_BM3D.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_CONVERTTOY");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_CONVERTTOY.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_RAW");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_RAW.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_FFMPEG");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_FFMPEG.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_FFMPEGDXVA2YUV420");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_FFMPEGDXVA2YUV420.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_FFMPEGYUV420");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_FFMPEGYUV420.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_FUSION");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_FUSION.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_BILATERALFILTER_RAW");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_BILATERALFILTER_RAW.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_SHARPENMASKING");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_SHARPENMASKING.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_COLOR");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_COLOR.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_INTERPOLATION");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_INTERPOLATION.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_CONVOLUTION");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_CONVOLUTION.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_MOSAIC");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_MOSAIC.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_DILATEERODE");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_DILATEERODE.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_MEDIAN");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_MEDIAN.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_NOISE");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_NOISE.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_FLIP");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_FLIP.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_SWIRL");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_SWIRL.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_ROTATION");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_ROTATION.cl", text_shader);
	text_shader = sqlResource.GetText("IDR_OPENCL_BITMAPCONVERSION");
	WriteToFile("c:\\developpement\\git\\Regards\\Resource\\res\\IDR_OPENCL_BITMAPCONVERSION.cl", text_shader);
	*/

	/*
	sqlResource.InsertOpenGLShader(L"IDR_GLSL_SHADER_VIDEO", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opengl\\IDR_GLSL_SHADER_VIDEO.gl");
	sqlResource.InsertOpenGLShader(L"IDR_GLSL_ALPHA_SHADER", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opengl\\IDR_GLSL_ALPHA_SHADER.gl");

	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_BM3D", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_BM3D.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_CONVERTTOY", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_CONVERTTOY.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_RAW", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_RAW.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_FFMPEG", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_FFMPEG.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_FFMPEGDXVA2YUV420", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_FFMPEGDXVA2YUV420.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_FFMPEGYUV420", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_FFMPEGYUV420.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_FUSION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_FUSION.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_BILATERALFILTER_RAW", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_BILATERALFILTER_RAW.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_SHARPENMASKING", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_SHARPENMASKING.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_COLOR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_COLOR.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_INTERPOLATION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_INTERPOLATION.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_CONVOLUTION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_CONVOLUTION.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_MOSAIC", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_MOSAIC.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_DILATEERODE", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_DILATEERODE.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_MEDIAN", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_MEDIAN.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_NOISE", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_NOISE.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_FLIP", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_FLIP.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_SWIRL", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_SWIRL.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_ROTATION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_ROTATION.cl");
	sqlResource.InsertOpenCLFloat(L"IDR_OPENCL_BITMAPCONVERSION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_float\\IDR_OPENCL_BITMAPCONVERSION.cl");


	/*
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_BM3D", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_BM3D.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_CONVERTTOY", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_CONVERTTOY.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_RAW", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_RAW.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FFMPEG", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FFMPEG.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FFMPEGDXVA2YUV420", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FFMPEGDXVA2YUV420.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FFMPEGYUV420", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FFMPEGYUV420.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FUSION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FUSION.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_BILATERALFILTER_RAW", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_BILATERALFILTER_RAW.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_SHARPENMASKING", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_SHARPENMASKING.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_COLOR", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_COLOR.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_INTERPOLATION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_INTERPOLATION.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_CONVOLUTION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_CONVOLUTION.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_MOSAIC", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_MOSAIC.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_DILATEERODE", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_DILATEERODE.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_MEDIAN", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_MEDIAN.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_NOISE", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_NOISE.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_FLIP", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_FLIP.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_SWIRL", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_SWIRL.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_ROTATION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_ROTATION.cl");
	sqlResource.InsertOpenCLUchar(L"IDR_OPENCL_BITMAPCONVERSION", L"OPENCL_CODE", L"C:\\developpement\\git\\Regards\\resource\\res\\shader\\opencl_uchar\\IDR_OPENCL_BITMAPCONVERSION.cl");
	*/

	//sqlResource.InsertText(L"IDR_OPENCL_BM3D", L"GLSL_SHADER", L"C:\\developpement\\Regards\\resource\\res\\text\\bm3d.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\text\\interpolation.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_BITMAPCONVERSION", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\opencl\\bitmap_conversion.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_BBOXINTERPOLATION", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\opencl\\interpolation_bbox.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION_UCHAR", L"GLSL_SHADER", L"C:\\developpement\\git\\Regards\\resource\\res\\opencl\\interpolation_short.cl");
	//sqlResource.InsertText(L"IDR_GLSL_SHADER_VIDEO", L"GLSL_SHADER", L"C:\\developpement\\Regards\\resource\\res\\opengl\\video_shader.cg");

	//sqlResource.InsertBitmap(L"IDB_FILTRE_BM3D", L"PNG", L"c:\\Regards\\resource\\res\\bitmap\\cameraman.png");
	//sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"H:\\developpement\\Regards\\resource\\res\\opencl\\noise.cl");
	//sqlResource.InsertVector(L"IDB_PHOTOINDEX", L"H:\\developpement\\Regards\\resource\\res\\vector\\index.svg");
	/*
	sqlResource.InsertLibelle(L"LBLeffectDenoisingSigmaP",L"Effect.Denoising.Sigma P",1);
	sqlResource.InsertLibelle(L"LBLeffectDenoisingFSize",L"Effect.Denoising.FSize",1);
	sqlResource.InsertLibelle(L"LBLeffectDenoisingSigmaX",L"Effect.Denoising.Sigma X",1);
	sqlResource.InsertLibelle(L"LBLeffectSharpenLevel",L"Effect.Sharpen.Level",1);
	sqlResource.InsertLibelle(L"LBLtoneConstrastLevel",L"Tone.Constrast.Level",1);
	sqlResource.InsertLibelle(L"LBLtoneBrightnessLevel",L"Tone.Brightness.Level",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostRed",L"Color Boost.Red",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostGreen",L"Color Boost.Green",1);
	sqlResource.InsertLibelle(L"LBLcolorBoostBlue",L"Color Boost.Blue",1);
	sqlResource.InsertLibelle(L"LBLenableEffect",L"Effect.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableBlurEffect",L"Effect.Denoising.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableSharpenEffect",L"Effect.Sharpen.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableColorEffect",L"Color Boost.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableBandCEffect",L"Tone.Enable",1);
	sqlResource.InsertLibelle(L"LBLenableGrayScale",L"Effect.Gray Scale",1);
	sqlResource.InsertLibelle(L"LBLshowFPS",L"Effect.Show FPS",1);
	*/
	//sqlResource.InsertLibelle(L"REGARDSVERSION", L"2.0.0.6", 1);
	/*
	sqlResource.InsertLibelle(L"LBLUserMultiOther",L"Effect.User Multiplier.Other",1);
	sqlResource.InsertLibelle(L"LBLUserFlip",L"Effect.User_flip",1);
	sqlResource.InsertLibelle(L"LBLUserInterpolation",L"Effect.Interpolation Quality",1);
	sqlResource.InsertLibelle(L"LBLUserBlack",L"Effect.Black Level",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelRed",L"Effect.Black.Channel Red",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelGreen",L"Effect.Black.Channel Green",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelBlue",L"Effect.Black.Channel Blue",1);
	sqlResource.InsertLibelle(L"LBLUserBlackChannelOther",L"Effect.Black.Channel Other",1);
	sqlResource.InsertLibelle(L"LBLUserSaturation",L"Effect.Saturation",1);
	sqlResource.InsertLibelle(L"LBLmed_passes",L"Effect.Median Filter Passes",1);
	sqlResource.InsertLibelle(L"LBLnoautobright",L"Effect.No auto bright",1);
	sqlResource.InsertLibelle(L"LBLauto_bright_thr",L"Effect.Auto brighness increase",1);
	sqlResource.InsertLibelle(L"LBLadjust_maximum_thr",L"Effect.Correcting channel overflow",1);
	sqlResource.InsertLibelle(L"LBLfuji_rotate",L"Effect.Use Fuji Rotate",1);
	sqlResource.InsertLibelle(L"LBLgreen_matching",L"Effect.Green channels disbalance",1);
	sqlResource.InsertLibelle(L"LBLdcb_iterations",L"Effect.Number of DCB correction passes",1);
	sqlResource.InsertLibelle(L"LBLdcb_enhance_fl",L"Effect.DCB interpolation with enhance interpolated colors",1);
	sqlResource.InsertLibelle(L"LBLfbdd_noiserd",L"Effect.Controls FBDD noise reduction",1);
	sqlResource.InsertLibelle(L"LBLeeci_refine",L"Effect.Use EECI refine for VCD interpolation",1);
	sqlResource.InsertLibelle(L"LBLes_med_passes",L"Effect.Number of edge-sensitive median filter passes after VCD+AHD demosaic",1);
	sqlResource.InsertLibelle(L"LBLca_correc",L"Effect.Chromatic aberrations suppression",1);
	sqlResource.InsertLibelle(L"LBLcared",L"Effect.Chromatic.Red",1);
	sqlResource.InsertLibelle(L"LBLcablue",L"Effect.Chromatic.Blue",1);
	sqlResource.InsertLibelle(L"LBLcfaline",L"Effect.Use Line noise reduction",1);
	sqlResource.InsertLibelle(L"LBLlinenoise",L"Effect.Line noise reduction",1);
	sqlResource.InsertLibelle(L"LBLcfa_clean",L"Effect.Use Reduce impulse noise and Gaussian high frequency",1);
	sqlResource.InsertLibelle(L"LBLlclean",L"Effect.Reduce impulse noise.Luminance",1);
	sqlResource.InsertLibelle(L"LBLcclean",L"Effect.Reduce impulse noise.Color",1);
	sqlResource.InsertLibelle(L"LBLcfa_green",L"Effect.Use Reduces maze artifacts Green",1);
	sqlResource.InsertLibelle(L"LBLgreen_thresh",L"Effect.Reduces maze artifacts Green",1);
	sqlResource.InsertLibelle(L"LBLexp_correc",L"Effect.Use Exposure correction before demosaic",1);
	sqlResource.InsertLibelle(L"LBLexp_shift",L"Effect.Exposure correction.Shift in linear scale",1);
	sqlResource.InsertLibelle(L"LBLexp_preser",L"Effect.Exposure correction.Preserve highlights",1);
	sqlResource.InsertLibelle(L"LBLwf_debanding",L"Effect.Use banding suppression",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdRed",L"Effect.Banding suppression.Red",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdGreen",L"Effect.Banding suppression.Green",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdBlue",L"Effect.Banding suppression.Blue",1);
	sqlResource.InsertLibelle(L"LBLwf_deband_tresholdOther",L"Effect.Banding suppression.Other",1);
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGYUV420",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegyuv420.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGDXVA2YUV420",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegdxva2.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEGDXVA2",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpegdxva2yuv420.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	/*
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	sqlResource.InsertText(L"IDR_BILATERALFILTER_RAW",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\bilateralfilter.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\interpolation.cl");
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	/*
		sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\sharpenMasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION_RGB", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation_rgb.cl");
	*/

	//sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	/*
	sqlResource.InsertText(L"IDR_BILATERALFILTER_RAW",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\bilateralfilter.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\sharpenmasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\swirl.cl");
	*/
	//sqlResource.InsertText(L"IDR_OPENCL_CXIMAGE",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\cximage.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\interpolation.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_REGARDBITMAP",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\regardsbitmap.cl");
	//sqlResource.InsertBitmap(L"IDB_BLACKROOM", L"PNG", L"d:\\resource\\res\\bitmap\\1475171356_raw.png");
	//sqlResource.InsertText(L"IDR_OPENCL_WXWIDGET",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\wxwidget.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_FFMPEG",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\ffmpeg.cl");
	//sqlResource.InsertText(L"IDR_GLSL_ALPHA_SHADER",L"GLSL_SHADER",L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\opengl\\alpha_shader.cg");
	//sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\color_conversion.cl");
	//sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"C:\\developpement\\vs2013wx3.1\\Regards\\resource\\res\\openCL\\matrix_convolution.cl");
	//sqlResource.InsertLibelle(L"LBLTEST", L"Test.Test", 1);

	CSqlInit::KillSqlEngine();
	return 0;

	sqlResource.InsertVector(L"IDB_CALENDAR", L"d:\\resource\\res\\vector\\calendar.svg");
	sqlResource.InsertVector(L"IDB_MAPLOCATION", L"d:\\resource\\res\\vector\\map_location.svg");
	sqlResource.InsertVector(L"IDB_PHOTOTEMP", L"d:\\resource\\res\\vector\\photo_temp.svg");
	sqlResource.InsertVector(L"IDB_LINK_ASSOC", L"d:\\resource\\res\\vector\\link.svg");
	sqlResource.InsertVector(L"IDB_ARROWRPNG", L"d:\\resource\\res\\vector\\next.svg");
	sqlResource.InsertVector(L"IDB_ARROWLPNG", L"d:\\resource\\res\\vector\\back.svg");
	sqlResource.InsertVector(L"IDB_ARROWTRACKRPNG", L"d:\\resource\\res\\vector\\trackend.svg");
	sqlResource.InsertVector(L"IDB_ARROWTRACKLPNG", L"d:\\resource\\res\\vector\\trackback.svg");
	sqlResource.InsertVector(L"IDB_BUTTON_CLICK", L"d:\\resource\\res\\vector\\butonclick.svg");
	sqlResource.InsertVector(L"IDB_BUTTON_CLICK_VERTICAL", L"d:\\resource\\res\\vector\\butonclick_vertical.svg");
	//sqlResource.InsertVector(L"IDB_DONATE", L"d:\\resource\\res\\vector\\donate.svg");
	sqlResource.InsertVector(L"IDB_FLIPHORZ", L"d:\\resource\\res\\vector\\flip_horizontal.svg");
	sqlResource.InsertVector(L"IDB_FLIPVERT", L"d:\\resource\\res\\vector\\flip_vertical.svg");
	sqlResource.InsertVector(L"IDB_PICTUREDETAILS", L"d:\\resource\\res\\vector\\pictures_details.svg");
	//sqlResource.InsertVector(L"IDB_EYEIRIS", L"d:\\resource\\res\\vector\\eye.svg");
	sqlResource.InsertVector(L"IDB_BOULESLIDER", L"d:\\resource\\res\\vector\\bouleSlider.svg");
	//sqlResource.InsertVector(L"IDB_CADRE_VIDEO", L"d:\\resource\\res\\vector\\cadre_video.svg");
	sqlResource.InsertVector(L"IDB_VOLUME_UP_VIDEO", L"d:\\resource\\res\\vector\\volumeup.svg");
	sqlResource.InsertVector(L"IDB_VOLUME_DOWN_VIDEO", L"d:\\resource\\res\\vector\\volumedown.svg");
	sqlResource.InsertVector(L"IDB_PHOTO", L"d:\\resource\\res\\vector\\photo.svg");
	//sqlResource.InsertVector(L"IDB_LINK_ASSOC", L"d:\\resource\\res\\vector\\link_assoc.svg");
	//sqlResource.InsertVector(L"IDB_REDEYE", L"d:\\resource\\res\\vector\\redeye.svg");


	sqlResource.InsertVector(L"IDB_OK", L"d:\\resource\\res\\vector\\ok.svg");
	sqlResource.InsertVector(L"IDB_CANCEL", L"d:\\resource\\res\\vector\\cancel.svg");
	sqlResource.InsertVector(L"IDB_DELETE", L"d:\\resource\\res\\vector\\delete.svg");
	sqlResource.InsertVector(L"IDB_CROP", L"d:\\resource\\res\\vector\\crop.svg");

	sqlResource.InsertVector(L"IDB_EMAIL", L"d:\\resource\\res\\vector\\email.svg");
	sqlResource.InsertVector(L"IDB_EXIT", L"d:\\resource\\res\\vector\\exit.svg");

	sqlResource.InsertVector(L"IDB_FOLDER", L"d:\\resource\\res\\vector\\folder.svg");
	sqlResource.InsertVector(L"IDB_FOLDER_REFRESH", L"d:\\resource\\res\\vector\\folder_refresh.svg");
	sqlResource.InsertVector(L"IDB_INFOS", L"d:\\resource\\res\\vector\\infos.svg");
	sqlResource.InsertVector(L"IDB_MULTIPLESELECT", L"d:\\resource\\res\\vector\\copy.svg");
	sqlResource.InsertVector(L"IDB_NEWPNG", L"d:\\resource\\res\\vector\\new.svg");
	sqlResource.InsertVector(L"IDB_PHOTOVIEWERPNG", L"d:\\resource\\res\\vector\\viewer.svg");

	sqlResource.InsertVector(L"IDB_PICTURETOOLBAR", L"d:\\resource\\res\\vector\\picture_toolbar.svg");
	sqlResource.InsertVector(L"IDB_PICTURE", L"d:\\resource\\res\\vector\\pictures.svg");
	sqlResource.InsertVector(L"IDB_PRINTERPNG", L"d:\\resource\\res\\vector\\Printer_Button.svg");
	sqlResource.InsertVector(L"IDB_ROTATION270", L"d:\\resource\\res\\vector\\rotation270.svg");
	sqlResource.InsertVector(L"IDB_ROTATION90", L"d:\\resource\\res\\vector\\rotation90.svg");
	sqlResource.InsertVector(L"IDB_SAVE", L"d:\\resource\\res\\vector\\save.svg");
	sqlResource.InsertVector(L"IDB_SHRINK", L"d:\\resource\\res\\vector\\shrink.svg");

	//sqlResource.InsertVector(L"IDB_CIRCLE_NEXT", L"d:\\resource\\res\\vector\\circle_next_arrow.svg");
	//sqlResource.InsertVector(L"IDB_CIRCLE_PREVIOUS", L"d:\\resource\\res\\vector\\circle_previous_arrow.svg");
	sqlResource.InsertVector(L"IDB_CHECKBOX_ON", L"d:\\resource\\res\\vector\\checkbox_on.svg");
	sqlResource.InsertVector(L"IDB_CHECKBOX_OFF", L"d:\\resource\\res\\vector\\checkbox_off.svg");


	sqlResource.InsertVector(L"IDB_PLAY", L"d:\\resource\\res\\vector\\play.svg");
	sqlResource.InsertVector(L"IDB_STOP", L"d:\\resource\\res\\vector\\stop.svg");
	sqlResource.InsertVector(L"IDB_PAUSE", L"d:\\resource\\res\\vector\\pause.svg");
	sqlResource.InsertVector(L"IDB_THUMBNAILPNG", L"d:\\resource\\res\\vector\\thumbnail.svg");
	sqlResource.InsertVector(L"IDB_ZOOMMOINS", L"d:\\resource\\res\\vector\\zoommoins.svg");
	sqlResource.InsertVector(L"IDB_ZOOMPLUS", L"d:\\resource\\res\\vector\\zoomplus.svg");


	sqlResource.InsertVector(L"IDB_PLAY_VIDEO", L"d:\\resource\\res\\vector\\play_video.svg");
	sqlResource.InsertVector(L"IDB_PAUSE_VIDEO", L"d:\\resource\\res\\vector\\pause_video.svg");

	sqlResource.InsertVector(L"IDB_VOLUME_SPEAKER_VIDEO", L"d:\\resource\\res\\vector\\speaker.svg");
	sqlResource.InsertVector(L"IDB_SCREENPNG", L"d:\\resource\\res\\vector\\monitor.svg");

	sqlResource.InsertVector(L"IDB_PLUS", L"d:\\resource\\res\\vector\\plus.svg");
	sqlResource.InsertVector(L"IDB_MINUS", L"d:\\resource\\res\\vector\\minus.svg");

	/*
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP1", L"PNG", L"d:\\resource\\res\\bitmap\\loading1.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP2", L"PNG", L"d:\\resource\\res\\bitmap\\loading2.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP3", L"PNG", L"d:\\resource\\res\\bitmap\\loading3.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP4", L"PNG", L"d:\\resource\\res\\bitmap\\loading4.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP5", L"PNG", L"d:\\resource\\res\\bitmap\\loading5.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP6", L"PNG", L"d:\\resource\\res\\bitmap\\loading6.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP7", L"PNG", L"d:\\resource\\res\\bitmap\\loading7.png");
	sqlResource.InsertBitmap(L"IDB_LOADING_STEP8", L"PNG", L"d:\\resource\\res\\bitmap\\loading8.png");

	sqlResource.InsertBitmap(L"IDB_ARROWRPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowright.png");
	sqlResource.InsertBitmap(L"IDB_ARROWLPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowleft.png");
	sqlResource.InsertBitmap(L"IDB_ARROWTRACKRPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowtrackright.png");
	sqlResource.InsertBitmap(L"IDB_ARROWTRACKLPNG", L"PNG", L"d:\\resource\\res\\bitmap\\arrowtrackleft.png");
	sqlResource.InsertBitmap(L"IDB_BUTTON_CLICK", L"PNG", L"d:\\resource\\res\\bitmap\\butonclick.png");
	sqlResource.InsertBitmap(L"IDB_BUTTON_CLICK_VERTICAL", L"PNG", L"d:\\resource\\res\\bitmap\\butonclick_vertical.png");
	sqlResource.InsertBitmap(L"IDB_OK", L"PNG", L"d:\\resource\\res\\bitmap\\ok.png");
	sqlResource.InsertBitmap(L"IDB_CANCEL", L"PNG", L"d:\\resource\\res\\bitmap\\cancel.png");
	sqlResource.InsertBitmap(L"IDB_DELETE", L"PNG", L"d:\\resource\\res\\bitmap\\delete.png");
	sqlResource.InsertBitmap(L"IDB_CROP", L"PNG", L"d:\\resource\\res\\bitmap\\crop.png");
	sqlResource.InsertBitmap(L"IDB_DONATE", L"PNG", L"d:\\resource\\res\\bitmap\\donate.png");
	sqlResource.InsertBitmap(L"IDB_EMAIL", L"PNG", L"d:\\resource\\res\\bitmap\\email.png");
	sqlResource.InsertBitmap(L"IDB_EXIT", L"PNG", L"d:\\resource\\res\\bitmap\\exit.png");
	sqlResource.InsertBitmap(L"IDB_FLIPHORZ", L"PNG", L"d:\\resource\\res\\bitmap\\flip_horizontal.png");
	sqlResource.InsertBitmap(L"IDB_FLIPVERT", L"PNG", L"d:\\resource\\res\\bitmap\\flip_vertical.png");
	sqlResource.InsertBitmap(L"IDB_FOLDER", L"PNG", L"d:\\resource\\res\\bitmap\\folder.png");
	sqlResource.InsertBitmap(L"IDB_FOLDER_REFRESH", L"PNG", L"d:\\resource\\res\\bitmap\\folder_refresh.png");
	sqlResource.InsertBitmap(L"IDB_INFOS", L"PNG", L"d:\\resource\\res\\bitmap\\infos.png");
	sqlResource.InsertBitmap(L"IDB_MULTIPLESELECT", L"PNG", L"d:\\resource\\res\\bitmap\\copy.png");
	sqlResource.InsertBitmap(L"IDB_NEWPNG", L"PNG", L"d:\\resource\\res\\bitmap\\new.png");
	sqlResource.InsertBitmap(L"IDB_PHOTOVIEWERPNG", L"PNG", L"d:\\resource\\res\\bitmap\\viewer.png");
	sqlResource.InsertBitmap(L"IDB_PICTUREDETAILS", L"PNG", L"d:\\resource\\res\\bitmap\\pictures_details.png");
	sqlResource.InsertBitmap(L"IDB_PICTURETOOLBAR", L"PNG", L"d:\\resource\\res\\bitmap\\picture_toolbar.png");
	sqlResource.InsertBitmap(L"IDB_PICTURE", L"PNG", L"d:\\resource\\res\\bitmap\\pictures.png");
	sqlResource.InsertBitmap(L"IDB_PRINTERPNG", L"PNG", L"d:\\resource\\res\\bitmap\\Printer_Button.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION270", L"PNG", L"d:\\resource\\res\\bitmap\\rotation270.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION90", L"PNG", L"d:\\resource\\res\\bitmap\\rotation90.png");
	sqlResource.InsertBitmap(L"IDB_SAVE", L"PNG", L"d:\\resource\\res\\bitmap\\save.png");
	sqlResource.InsertBitmap(L"IDB_SHRINK", L"PNG", L"d:\\resource\\res\\bitmap\\shrink.png");

	sqlResource.InsertBitmap(L"IDB_CIRCLE_NEXT", L"PNG", L"d:\\resource\\res\\bitmap\\circle_next_arrow.png");
	sqlResource.InsertBitmap(L"IDB_CIRCLE_PREVIOUS", L"PNG", L"d:\\resource\\res\\bitmap\\circle_previous_arrow.png");
	sqlResource.InsertBitmap(L"IDB_CHECKBOX_ON", L"PNG", L"d:\\resource\\res\\bitmap\\checkbox_on.png");
	sqlResource.InsertBitmap(L"IDB_CHECKBOX_OFF", L"PNG", L"d:\\resource\\res\\bitmap\\checkbox_off.png");
	sqlResource.InsertBitmap(L"IDB_EYEIRIS", L"PNG", L"d:\\resource\\res\\bitmap\\eye.png");
	sqlResource.InsertBitmap(L"IDB_BOULESLIDER", L"PNG", L"d:\\resource\\res\\bitmap\\bouleSlider.png");

	sqlResource.InsertBitmap(L"IDB_PLAY", L"PNG", L"d:\\resource\\res\\bitmap\\play.png");
	sqlResource.InsertBitmap(L"IDB_STOP", L"PNG", L"d:\\resource\\res\\bitmap\\stop.png");
	sqlResource.InsertBitmap(L"IDB_PAUSE", L"PNG", L"d:\\resource\\res\\bitmap\\pause.png");
	sqlResource.InsertBitmap(L"IDB_THUMBNAILPNG", L"PNG", L"d:\\resource\\res\\bitmap\\thumbnail.png");
	sqlResource.InsertBitmap(L"IDB_ZOOMMOINS", L"PNG", L"d:\\resource\\res\\bitmap\\zoommoins.png");
	sqlResource.InsertBitmap(L"IDB_ZOOMPLUS", L"PNG", L"d:\\resource\\res\\bitmap\\zoomplus.png");

	sqlResource.InsertBitmap(L"IDB_CADRE_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\cadre_video.png");
	sqlResource.InsertBitmap(L"IDB_PLAY_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\play_video.png");
	sqlResource.InsertBitmap(L"IDB_PAUSE_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\pause_video.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_UP_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\volumeup.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_DOWN_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\volumedown.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_SPEAKER_VIDEO", L"PNG", L"d:\\resource\\res\\bitmap\\speaker.png");
	sqlResource.InsertBitmap(L"IDB_SCREENPNG", L"PNG", L"d:\\resource\\res\\bitmap\\monitor.png");
	sqlResource.InsertBitmap(L"IDB_LINK_ASSOC", L"PNG", L"d:\\resource\\res\\bitmap\\link_assoc.png");
	sqlResource.InsertBitmap(L"IDB_REDEYE", L"PNG", L"d:\\resource\\res\\bitmap\\redeye.png");
	sqlResource.InsertBitmap(L"IDB_PLUS", L"PNG", L"d:\\resource\\res\\bitmap\\plus.png");
	sqlResource.InsertBitmap(L"IDB_MINUS", L"PNG", L"d:\\resource\\res\\bitmap\\minus.png");
	sqlResource.InsertBitmap(L"IDB_PHOTO", L"PNG", L"d:\\resource\\res\\bitmap\\photo.png");

	//CSqlResource sqlResource;
	//wstring libelle = sqlResource.GetText(1);
	/*
	sqlResource.InsertBitmap(L"IDB_PHOTO", L"PNG", L"d:\\resource\\res\\bitmap\\photo.png");
	sqlResource.InsertBitmap(L"IDB_REPORTVIEWPNG",L"PNG",L"d:\\resource\\res\\bitmap\\details.png");
	sqlResource.InsertBitmap(L"IDB_LARGEICONPNG",L"PNG",L"d:\\resource\\res\\bitmap\\large.png");
	sqlResource.InsertBitmap(L"IDB_LISTVIEWBUTTON",L"PNG",L"d:\\resource\\res\\bitmap\\list.png");
	sqlResource.InsertBitmap(L"IDB_SMALLICONPNG",L"PNG",L"d:\\resource\\res\\bitmap\\small.png");
	sqlResource.InsertBitmap(L"IDB_PHOTOEDITORPNG",L"PNG",L"d:\\resource\\res\\bitmap\\editor.png");
	sqlResource.InsertBitmap(L"IDB_FOLDERUPPNG" ,L"PNG",L"d:\\resource\\res\\bitmap\\folderup.png");
	sqlResource.InsertBitmap(L"IDB_GLASSPNG",L"PNG",L"d:\\resource\\res\\bitmap\\1398712562_search_32.png");
	sqlResource.InsertBitmap(L"IDB_EDITORPNG" ,L"PNG",L"d:\\resource\\res\\bitmap\\eidtor 2.png");
	sqlResource.InsertBitmap(L"IDB_CATALOG",L"PNG",L"d:\\resource\\res\\bitmap\\catalog.png");
	sqlResource.InsertBitmap(L"IDB_PREVIEW",L"PNG",L"d:\\resource\\res\\bitmap\\preview.png");
	sqlResource.InsertBitmap(L"IDB_ROTATION90",L"PNG",L"d:\\resource\\res\\bitmap\\rotation90.png");
	sqlResource.InsertBitmap(L"IDB_ZOOM_SELECTION",L"PNG",L"d:\\resource\\res\\bitmap\\zoom_selection.png");
	sqlResource.InsertBitmap(L"IDB_SELECTION" ,L"PNG",L"d:\\resource\\res\\bitmap\\selection.png");
	sqlResource.InsertBitmap(L"IDB_MOVE_PICTURE",L"PNG",L"d:\\resource\\res\\bitmap\\move_picture.png");
	sqlResource.InsertBitmap(L"IDB_PEN" ,L"PNG",L"d:\\resource\\res\\bitmap\\pen.png");
	sqlResource.InsertBitmap(L"IDB_SYSTEM_FOLDER" ,L"PNG",L"d:\\resource\\res\\bitmap\\system_folder.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_UP" ,L"PNG",L"d:\\resource\\res\\bitmap\\volumeup.png");
	sqlResource.InsertBitmap(L"IDB_VOLUME_DOWN" ,L"PNG",L"d:\\resource\\res\\bitmap\\volumedown.png");
	sqlResource.InsertBitmap(L"IDB_EFFECT" ,L"PNG",L"d:\\resource\\res\\bitmap\\effect.png");
	sqlResource.InsertBitmap(L"IDB_TOOLS",L"PNG",L"d:\\resource\\res\\bitmap\\tools.png");
	sqlResource.InsertBitmap(L"IDB_COPY",L"PNG",L"d:\\resource\\res\\bitmap\\copy.png");
	sqlResource.InsertBitmap(L"IDB_TITLEREGARDS",L"PNG",L"d:\\resource\\res\\bitmap\\title.png");*/
	//return 0;

	/*
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BELL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_Bell.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BILINEAR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiLinear.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BSPLINE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BSpline.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_CATMULL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_CatMull.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\matrix_convolution.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MEDIAN_SHADER",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\median_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_TRIANGLE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiTriangle.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NIVEAUDEGRIS",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\niveaudegris.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\color_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_EDGE_DETECTION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\edge_detection.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SEPIA",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\sepia_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOIRETBLANC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\noiretblanc_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NEGATIF",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\negative_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_DILATE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\dilate_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ERODE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\erode_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\mosaic_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPV",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\flipvert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPH",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\fliphorz_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT90",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rotation90_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT270",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rotation270_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTFREE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\freerotation_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INTERPOLATION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_interpolation.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBICYUV",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_YUV.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_VIDEO_EFFECT",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_video_effect.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_GAUSSIAN_BLUR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\gaussian_blur.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SHARPEN_MASKING",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\sharpen_masking.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RENDER_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_BitmapRender.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADD_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmap_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScale_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDBITMAPTOSCALE_POSITION",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScalePosition_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ADDALPHABLENDING_BITMAP",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddAlphaBlendingBitmapShader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_TRANSITION	",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\AddBitmapToScale_shaderTransition.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INIT",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\init_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SOLARIZE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\solarize.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RGB",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\rgb.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BRIGHTANDCONTRAST",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\brightandcontrast.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_CLOUDS",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\clouds.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SWIRL",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\swirl.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PHOTOFILTRE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\photofiltre.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_POSTERIZE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\posterize.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOTIONBLUR",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\motionBlur.cg");
		//Version 4
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\pixel_shader_BiCubic_v4.cg");
	sqlResource.InsertText(L"IDR_GLSL_VERTEXSHADER_COORDONATE",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\vertex_shader_coordonate.cg");
	sqlResource.InsertText(L"IDR_GLSL_COMPUTE_TEST",L"GLSL_SHADER",L"d:\\resource\\res\\opengl\\compute_test.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPV_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\flipvert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_FLIPH_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\fliphorz_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT90_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation90_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROT270_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation270_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NIVEAUDEGRIS_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\niveaudegris.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOIRETBLANC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\noiretblanc_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\matrix_convolution.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MATRIX_CONVOLUTION5X5_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\matrix_convolution5x5.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_COPY_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\copy_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MEDIAN_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\median_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_INTERPOLATION_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\pixel_shader_BiCubic.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_BICUBIC_INTERPOLATION_ZONE_SHADER_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\pixel_shader_BiCubic_zone.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SEPIA_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\sepia.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NEGATIF_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\negatif.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_RGB_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rgb.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_LIGHTANDCONTRAST_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\lightnessandcontrast.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PHOTOFILTRE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\photo_filtre.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOSAIC_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\mosaic_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ERODE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\erode_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_DILATE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\dilate_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NOISE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\noise_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTATEFREE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation_free_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_INSERT_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\insert_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_MOTIONBLUR_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\motionblur.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_PERLINNOISE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\perlinnoise_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SWIRL_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\swirl_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_ROTATEFREEPREVIEW_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\rotation_free_preview_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_POSTERIZE_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\posterize_shader.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_NV12TORGB32_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\nv12torgb32.cg");
	sqlResource.InsertText(L"IDR_GLSL_SHADER_SHARPENMASKING_V4",L"GLSL_SHADER",L"d:\\resource\\res\\openglv4\\sharpenMasking.cg");

	/////////////////////////////////////////////////////////////////////////////
	//
	// OPENCL
	//
	/*
	sqlResource.InsertText(L"IDR_OPENCL_COLOR",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\color.cl");
	sqlResource.InsertText(L"IDR_OPENCL_ROTATION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\rotation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FLIP",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\flip.cl");
	sqlResource.InsertText(L"IDR_OPENCL_FUSION",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\fusion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MOSAIC",L"GLSL_SHADER",L"d:\\resource\\res\\openCL\\mosaic.cl");
	sqlResource.InsertText(L"IDR_OPENCL_CONVOLUTION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\matrix_convolution.cl");
	sqlResource.InsertText(L"IDR_OPENCL_DILATEERODE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\dilate_erode.cl");
	sqlResource.InsertText(L"IDR_OPENCL_NOISE", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\noise.cl");
	sqlResource.InsertText(L"IDR_OPENCL_MEDIAN", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\median.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SWIRL", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\swirl.cl");
	sqlResource.InsertText(L"IDR_OPENCL_COLORCONVERSION", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\color_conversion.cl");
	sqlResource.InsertText(L"IDR_OPENCL_SHARPENMASKING", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\sharpenMasking.cl");
	sqlResource.InsertText(L"IDR_OPENCL_INTERPOLATION_RGB", L"GLSL_SHADER", L"d:\\resource\\res\\openCL\\interpolation_rgb.cl");
	//Email
	sqlResource.InsertText(L"EMAIL", L"EMAIL_EML", L"d:\\resource\\email.eml");

	/////////////////////////////////////////////////////////////////////////////
	//
	// String Table
	//
	sqlResource.InsertLibelle(L"REGARDSVERSION", L"2.0.0.6", 1);
	sqlResource.InsertLibelle(L"ADRESSEWEBVERSION",L"/jacques.figuinha/version.txt", 1);
	sqlResource.InsertLibelle(L"SITEWEB", L"http://perso.numericable.fr/jacques.figuinha/regards/", 1);
	sqlResource.InsertLibelle(L"SERVEURWEB", L"perso.numericable.fr", 1);
	sqlResource.InsertLibelle(L"DONATE", L"http://perso.numericable.fr/jacques.figuinha/regards/donate.htm", 1);
	sqlResource.InsertLibelle(L"LBLINFOS", L"Infos", 1);
	sqlResource.InsertLibelle(L"LBLHISTORY", L"History", 1);
	sqlResource.InsertLibelle(L"LBLMAPS", L"Maps", 1);
	sqlResource.InsertLibelle(L"LBLEFFECT", L"Effect", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTPARAMETER",L"Effect Parameter", 1);
	sqlResource.InsertLibelle(L"LBLSAVE", L"Save", 1);
	sqlResource.InsertLibelle(L"LBLROTATE90", L"Rotate 90", 1);
	sqlResource.InsertLibelle(L"LBLCOLOREFFECT", L"Color Effect", 1);
	sqlResource.InsertLibelle(L"LBLEMAIL", L"Email", 1);
	sqlResource.InsertLibelle(L"LBLROTATE270", L"Rotate 270", 1);
	sqlResource.InsertLibelle(L"LBLCONVOLUTIONEFFECT",L"Convolution Effect", 1);
	sqlResource.InsertLibelle(L"LBLPRINT", L"Print", 1);
	sqlResource.InsertLibelle(L"LBLFLIPV", L"Flip vertical", 1);
	sqlResource.InsertLibelle(L"LBLSPECIALEFFECT",L"Special Effect", 1);
	sqlResource.InsertLibelle(L"LBLCROP", L"Crop", 1);
	sqlResource.InsertLibelle(L"LBLDOWNLOADNEWVERSION",L"Do you want to download new version ?", 1);
	sqlResource.InsertLibelle(L"LBLSHRINK", L"Shrink Picture", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTRADIUS", L"Effect.Radius", 1);
	sqlResource.InsertLibelle(L"LBLZOOMON", L"Zoom On", 1);
	sqlResource.InsertLibelle(L"LBLZOOMOFF", L"Zoom Off", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTSIGMA", L"Effect.Sigma", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTANGLE", L"Effect.Angle", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSFREQUENCY", L"Clouds.Frequency", 1);
	sqlResource.InsertLibelle(L"LBLfilterSoften", L"Soften", 1);
	sqlResource.InsertLibelle(L"LBLfilterBlur", L"Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterSolarize", L"Solarize", 1);
	sqlResource.InsertLibelle(L"LBLfilterGaussian", L"Gaussian Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterMedian", L"Median", 1);
	sqlResource.InsertLibelle(L"LBLfilterMotion", L"Motion Blur", 1);
	sqlResource.InsertLibelle(L"LBLfilterRotate", L"Rotate Free", 1);
	sqlResource.InsertLibelle(L"LBLfilterLight", L"Brigthness and Contrast", 1);
	sqlResource.InsertLibelle(L"LBLfilterPhoto", L"Photo Filtre", 1);
	sqlResource.InsertLibelle(L"LBLfilterPosterisation", L"Posterisation", 1);
	sqlResource.InsertLibelle(L"LBLfilterColor", L"Color Balance", 1);
	sqlResource.InsertLibelle(L"LBLfilterSwirl", L"Swirl", 1);
	sqlResource.InsertLibelle(L"LBLfilterClouds", L"Clouds", 1);
	sqlResource.InsertLibelle(L"LBLfilterErode", L"Erode", 1);
	sqlResource.InsertLibelle(L"LBLfilterDilate", L"Dilate", 1);
	sqlResource.InsertLibelle(L"LBLfilterSharpen", L"Sharpen", 1);
	sqlResource.InsertLibelle(L"LBLfilterSharpenStrong",L"Sharpen Strong", 1);
	sqlResource.InsertLibelle(L"LBLfilterNoise", L"Noise", 1);
	sqlResource.InsertLibelle(L"LBLfilterMosaic", L"Mosaic", 1);
	sqlResource.InsertLibelle(L"LBLfilterEmboss", L"Emboss", 1);
	sqlResource.InsertLibelle(L"LBLfilterGrey", L"Grey Level", 1);
	sqlResource.InsertLibelle(L"LBLfilterSepia", L"Sepia", 1);
	sqlResource.InsertLibelle(L"LBLfilterBlack", L"Black and White", 1);
	sqlResource.InsertLibelle(L"LBLfilterEdge", L"Edge", 1);
	sqlResource.InsertLibelle(L"LBLfilterNegatif", L"Negatif", 1);
	sqlResource.InsertLibelle(L"LBLfilterLensFlare", L"Lens Flare", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSAMPLITUDE", L"Clouds.Amplitude", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTRED", L"Clouds.Color Front.Red", 1);
	sqlResource.InsertLibelle(L"LBLFLIPH", L"Flip horizontal", 1);
	sqlResource.InsertLibelle(L"LBLOPENFOLDER", L"Open Folder", 1);
	sqlResource.InsertLibelle(L"LBLASSOCIATE", L"Associate", 1);
	sqlResource.InsertLibelle(L"LBLDONATE", L"Donate", 1);
	sqlResource.InsertLibelle(L"LBLQUIT", L"Quit", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTGREEN", L"Clouds.Color Front.Green", 1);
	sqlResource.InsertLibelle(L"LBLOPEN", L"Open", 1);
	sqlResource.InsertLibelle(L"LBLFULLSCREEN", L"Fullscreen", 1);
	sqlResource.InsertLibelle(L"LBLSETTINGS", L"Settings", 1);
	sqlResource.InsertLibelle(L"LBLINFORMATIONS", L"Informations", 1);
	sqlResource.InsertLibelle(L"LBLPREVIEW", L"Preview", 1);
	sqlResource.InsertLibelle(L"LBLVIDEOTHUMBNAIL", L"Video Thumbnail", 1);
	sqlResource.InsertLibelle(L"LBLTHUMBNAIL", L"Thumbnail", 1);
	sqlResource.InsertLibelle(L"LBLCRITERIA", L"Criteria", 1);
	sqlResource.InsertLibelle(L"LBLSELECTFOLDER", L"Select Folder", 1);
	sqlResource.InsertLibelle(L"LBLLISTPICTURE", L"List Picture", 1);
	sqlResource.InsertLibelle(L"LBLPREVIEWPICTURE", L"Preview Picture", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORFRONTBLUE", L"Clouds.Color Front.Blue", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKRED", L"Clouds.Color Back.Red", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKGREEN", L"Clouds.Color Back.Green", 1);
	sqlResource.InsertLibelle(L"LBLCLOUDSCOLORBACKBLUE", L"Clouds.Color Back.Blue", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORRED", L"Effect.Color.Red", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORGREEN", L"Effect.Color.Green", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCOLORBLUE", L"Effect.Color.Blue", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTLEVEL", L"Effect.Level", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTGAMMA", L"Effect.Gamma", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTINTENSITY", L"Effect.Intensity", 1);
	sqlResource.InsertLibelle(L"LBLSELECT", L"Select", 1);
	sqlResource.InsertLibelle(L"LBLDELETE", L"Delete files", 1);
	sqlResource.InsertLibelle(L"LBLCOPY", L"Copy files", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTMEDIANLEVEL", L"Effect.Median.Level", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTCONTRAST", L"Effect.Constrast", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTBRIGHTNESS", L"Effect.Brightness", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTTHRESHOLD", L"Effect.Threshold", 1);
	sqlResource.InsertLibelle(L"LBLROTATIONANGLE", L"Rotation.Angle", 1);
	sqlResource.InsertLibelle(L"LBLOK", L"Ok", 1);
	sqlResource.InsertLibelle(L"LBLCANCEL", L"Cancel", 1);
	sqlResource.InsertLibelle(L"LBLEFFECTSHARPENLEVEL", L"Effect.Sharpen.Level", 1);
	sqlResource.InsertLibelle(L"LBLFIRST", L"First", 1);
	sqlResource.InsertLibelle(L"LBLPREVIOUS", L"Previous", 1);
	sqlResource.InsertLibelle(L"LBLPLAY", L"Play", 1);
	sqlResource.InsertLibelle(L"LBLSTOP", L"Stop", 1);
	sqlResource.InsertLibelle(L"LBLNEXT", L"Next", 1);
	sqlResource.InsertLibelle(L"LBLEND", L"End", 1);
	sqlResource.InsertLibelle(L"LBLREFRESH", L"Refresh Folder", 1);
	sqlResource.InsertLibelle(L"LBLPHOTOVIEWER", L"Photo Viewer", 1);
	sqlResource.InsertLibelle(L"LBLLAST", L"Last", 1);
	sqlResource.InsertLibelle(L"LBLNOTGEO", L"Not Geolocalized", 1);
	sqlResource.InsertLibelle(L"LBLALLPHOTO", L"All photos", 1);
	sqlResource.InsertLibelle(L"LBLMONTHNAME", L"January,February,March,Avril,May,June,July,August,September,October,November,December", 1);
	sqlResource.InsertLibelle(L"LBLDAYNAME", L"Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday", 1);
	sqlResource.InsertLibelle(L"LBLTONECONTRASTLEVEL", L"Tone.Constrast.Level", 1);
	sqlResource.InsertLibelle(L"LBLTONEEXPOSURELEVEL", L"Tone.Exposure.Level", 1);
	sqlResource.InsertLibelle(L"LBLTONEBRIGHTNESSLEVEL", L"Tone.Brightness.Level", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTRED", L"Color Boost.Red", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTGREEN", L"Color Boost.Green", 1);
	sqlResource.InsertLibelle(L"LBLCOLORBOOSTBLUE", L"Color Boost.Blue", 1);
	sqlResource.InsertLibelle(L"LBLPICTUREDETAILS", L"Picture Toolbar", 1);
	sqlResource.InsertLibelle(L"LBLFOLDER", L"Show Folder", 1);
	sqlResource.InsertLibelle(L"LBLfilterRedEye", L"Red Eye", 1);
	sqlResource.InsertLibelle(L"LBLHISTOGRAMEFFECT", L"Histogram Effect", 1);
	sqlResource.InsertLibelle(L"LBLHistogramNormalize", L"Histogram Normalize", 1);
	sqlResource.InsertLibelle(L"LBLHistogramEqualize", L"Histogram Equalize", 1);
	sqlResource.InsertLibelle(L"LBLHistogramLog", L"Histogram Log", 1);
	sqlResource.InsertLibelle(L"LBLDeleteText", L"Do you want to delete this files ?", 1);
	sqlResource.InsertLibelle(L"LBLDeleteCaption", L"Confirmation", 1);
	sqlResource.InsertLibelle(L"LBLDeleteMessage", L"Delete Processing", 1);
	sqlResource.InsertLibelle(L"LBLDeleteFinalMessage", L"Delete Files is finished.", 1);
	sqlResource.InsertLibelle(L"LBLOpenCLDevice", L"No OpenCL device compatible !", 1);
	sqlResource.InsertLibelle(L"LBLCopyText", L"Select Save Folder", 1);
	sqlResource.InsertLibelle(L"LBLCopyMessage", L"Copy Processing", 1);
	sqlResource.InsertLibelle(L"LBLCopyFinalMessage", L"Copy Files is finished.", 1);
	sqlResource.InsertLibelle(L"LBLClose", L"Close", 1);
	sqlResource.InsertLibelle(L"LBLGPSADRESS", L"http://www.geoplugin.net", 1);
	*/
	CSqlInit::KillSqlEngine();
	return 0;
}

#else

//#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex>
#include <vector>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

int main()
{
	//Give the names of the images to be registered
	const char* imRef_name = "d:\\2.jpg";
	const char* imNxt_name = "d:\\3.jpg";
	int hessianThresh = 100, ransacThresh = 3;;
	Mat mask, H12;
	// Read images
	Mat img1 = imread(imRef_name, cv::IMREAD_GRAYSCALE);
	Mat img2 = imread(imNxt_name, cv::IMREAD_GRAYSCALE);
	Mat img2Out;	// Registered image2 wrt image1

	// Check to see if images exist
	if (img1.empty() || img2.empty())
	{
		printf("Can’t read one of the images\n");
		exit(0);
	}


	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	Ptr<SIFT> descriptor = SIFT::create();
	descriptor->detect(img1, keypoints1);

	Ptr<SURF> detector = SURF::create(hessianThresh);
	detector->compute(img1, keypoints1, descriptors1);


	descriptor->detect(img2, keypoints2);
	detector->compute(img2, keypoints2, descriptors2);

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors1, descriptors2, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	//-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
	//-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
	//-- small)
	//-- PS.- radiusMatch can also be used here.
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors1.rows; i++)
	{
		if (matches[i].distance > 0.4 && matches[i].distance < 0.6)
		{
			good_matches.push_back(matches[i]);
		}
	}

	//-- Draw only "good" matches
	Mat img_matches;
	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Show detected matches
	imshow("Good Matches", img_matches);

	/*
	for (int i = 0; i < (int)good_matches.size(); i++)
	{
		printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i, good_matches[i].queryIdx, good_matches[i].trainIdx);
	}
	*/

	// Extract indices of matched points
	vector<int> queryIdxs(good_matches.size()), trainIdxs(good_matches.size());
	for (size_t i = 0; i < good_matches.size(); i++)
	{
		queryIdxs[i] = good_matches[i].queryIdx;
		trainIdxs[i] = good_matches[i].trainIdx;
	}

	// Extract matched points from indices
	vector<Point2f> points1; KeyPoint::convert(keypoints1, points1, queryIdxs);
	vector<Point2f> points2; KeyPoint::convert(keypoints2, points2, trainIdxs);

	int nbPoint = 0;
	for (int i = 0; i < points1.size(); i++)
	{
		if (abs(points1[i].y - points2[i].y) < 10)
			nbPoint++;
	}

	/*

	// Use RANSAC to find the homography
	printf("\nComputing homography ... ");
	H12 = findHomography(Mat(points2), Mat(points1), RANSAC, ransacThresh);

	// Warp the second image according to the homography
	warpPerspective(img2, img2Out, H12, cv::Size(img2.cols, img2.rows), INTER_LINEAR);

	// Write result to file
	imwrite("d:\\im2reg.png", img2Out);

	*/
	waitKey(0);

	/*
	// detecting keypoints
	printf("Finding keypoints ... ");
	//SURF ImgSurf(hessianThresh);
	Ptr<SURF> extractor = SURF::create(hessianThresh);
	vector<KeyPoint> keypoints1, keypoints2;
	//ImgSurf(img1, mask, keypoints1);
	//ImgSurf(img2, mask, keypoints2);
	// computing descriptors
	//SurfDescriptorExtractor extractor;
	Mat descriptors1, descriptors2;
	//extractor(img1, mask, keypoints1, descriptors1, TRUE);
	//extractor(img2, mask, keypoints2, descriptors2, TRUE);
	extractor->detectAndCompute(img1, mask, keypoints1, descriptors1, true);
	extractor->detectAndCompute(img2, mask, keypoints2, descriptors2, true);

	// Match the points
	printf("\nMatching keypoints ... ");
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match(descriptors1, descriptors2, matches);

	// Extract indices of matched points
	vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
	for (size_t i = 0; i < matches.size(); i++)
	{
		queryIdxs[i] = matches[i].queryIdx;
		trainIdxs[i] = matches[i].trainIdx;
	}

	// Extract matched points from indices
	vector<Point2f> points1; KeyPoint::convert(keypoints1, points1, queryIdxs);
	vector<Point2f> points2; KeyPoint::convert(keypoints2, points2, trainIdxs);

	// Use RANSAC to find the homography
	printf("\nComputing homography ... ");
	H12 = findHomography(Mat(points2), Mat(points1), RANSAC, ransacThresh);

	// Warp the second image according to the homography
	warpPerspective(img2, img2Out, H12, cv::Size(img2.cols, img2.rows), INTER_LINEAR);

	// Write result to file
	imwrite("im2reg.png", img2Out);
	printf("\nDone!!!.... ");
	*/
	return 0;
}

#endif
