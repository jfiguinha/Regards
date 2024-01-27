#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"
#include <ConvertUtility.h>


extern "C" {
#include "ffmpeg.h"
#include <libavcodec/avcodec.h>
#include <libavutil/avassert.h>
#include "cmdutils.h"
}




void CFFmpegApp::ExitFunction(int x)
{
	throw x;
}

int CFFmpegApp::ProgressBarFunction(int x, void* progressWnd)
{
	if (progressWnd != nullptr)
	{
		auto dialog = static_cast<wxProgressDialog*>(progressWnd);
		if (false == dialog->Update(x, "Video Progress ..."))
			return 1;
	}

	return 0;
}

void CFFmpegApp::ExecuteFFmpeg()
{
	arrayOfCstrings = new char*[arrayOfStrings.size()];

	for (int i = 0; i < arrayOfStrings.size(); ++i)
		arrayOfCstrings[i] = (char*)arrayOfStrings[i].c_str();


	wxProgressDialog* dialog = nullptr;

	if (showProgressWindow)
	{
		dialog = new wxProgressDialog("FFmpeg Process", "Checking...", 100, nullptr,
		                              wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);
	}

	try
	{
		void (*foo)(int);
		foo = &ExitFunction;

		int (*progress)(int, void*);
		progress = &ProgressBarFunction;

		ret = ExecuteFFMpegProgram(arrayOfStrings.size(), arrayOfCstrings, foo, progress, dialog);
	}
	catch (int e)
	{
		ret = e;
	}
	catch (...)
	{
	}

	if (dialog != nullptr)
		delete dialog;

	Cleanup(ret);
}

int CFFmpegApp::TestFFmpeg(const wxString& commandline)
{
	//ffmpeg -hwaccel_output_format cuda -i F:\music_video\test.mp4 -frames:v 1 -q:v 2 test.png
	/*
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-hwaccel_output_format");
	arrayOfStrings.push_back("cuda");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back("F:\\music_video\\test.mp4");
	arrayOfStrings.push_back("-frames:v");
	arrayOfStrings.push_back("1");
	arrayOfStrings.push_back("-q:v");
	arrayOfStrings.push_back("2");
	arrayOfStrings.push_back("d:\\test.png");
	*/
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("cuda");
	arrayOfStrings.push_back("d:\\video\\20200509_132206.mp4");
	arrayOfStrings.push_back("F:\\music_video\\20200509_132206.yuv");
	//TestHWDecode(arrayOfStrings);

	return 0;
}

int CFFmpegApp::ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile,
                                           const int& rotate)
{
	//ffmpeg -i 20200509_132328.mp4 -map_metadata 0 -metadata:s:v rotate=-90 -codec copy output.mp4
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile.ToStdString());
	arrayOfStrings.push_back("-map_metadata");
	arrayOfStrings.push_back("0");
	arrayOfStrings.push_back("-metadata:s:v");
	arrayOfStrings.push_back("rotate=" + std::to_string(rotate));
	arrayOfStrings.push_back("-codec");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile.ToStdString());
	ExecuteFFmpeg();
	return 0;
}

void CFFmpegApp::Cleanup(int ret)
{
	ffmpeg_cleanup(ret);
	delete[] arrayOfCstrings;
}


int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
                          const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-stream_loop");
	arrayOfStrings.push_back("-1");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputAudioFile));
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(extension));
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideo));
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                 const wxString& timeVideo, const wxString& outputFile)
{
	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideo));
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputVideoFile));
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideo));
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputAudioFile));
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("0:v:0");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("1:a:0");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                      const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputVideoFile));
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoIn));
	arrayOfStrings.push_back("-to");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoOut));
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                          const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputVideoFile));
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoIn));
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoOut));
	}
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-an");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                          const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputVideoFile));
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoIn));
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(timeVideoOut));
	}
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                           const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	arrayOfStrings.push_back("ffmpeg");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputVideoFile));
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(inputAudioFile));
	arrayOfStrings.push_back("-c");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(CConvertUtility::ConvertToStdString(outputFile));
	ExecuteFFmpeg();

	return 0;
}

std::vector<wxString> CFFmpegApp::GetHardwareList()
{
	std::vector<wxString> list;
	enum AVHWDeviceType type = AV_HWDEVICE_TYPE_NONE;
	while ((type = av_hwdevice_iterate_types(type)) != AV_HWDEVICE_TYPE_NONE)
	{
		wxString encoder = av_hwdevice_get_type_name(type);
		if(encoder == "cuda")
			list.push_back("nvenc");
		else if(encoder == "qsv")
			list.push_back("qsv");
		else if (encoder == "amf")
			list.push_back("amf");
	}

	return list;
}

std::vector<std::string> CFFmpegApp::GetListOfCodec()
{
	std::vector<std::string> list;
//	print_codecs(1);
	bool encoder = true;

	const AVCodecDescriptor** codecs;
	unsigned i, nb_codecs = get_codecs_sorted(&codecs);

	printf("%s:\n"
		" V..... = Video\n"
		" A..... = Audio\n"
		" S..... = Subtitle\n"
		" .F.... = Frame-level multithreading\n"
		" ..S... = Slice-level multithreading\n"
		" ...X.. = Codec is experimental\n"
		" ....B. = Supports draw_horiz_band\n"
		" .....D = Supports direct rendering method 1\n"
		" ------\n",
		encoder ? "Encoders" : "Decoders");
	for (i = 0; i < nb_codecs; i++)
	{
		const AVCodecDescriptor* desc = codecs[i];
		const AVCodec* codec;
		void* iter = NULL;

		while ((codec = next_codec_for_id(desc->id, &iter, encoder)))
		{
			printf(" %c", get_media_type_char(desc->type));
			printf((codec->capabilities & AV_CODEC_CAP_FRAME_THREADS) ? "F" : ".");
			printf((codec->capabilities & AV_CODEC_CAP_SLICE_THREADS) ? "S" : ".");
			printf((codec->capabilities & AV_CODEC_CAP_EXPERIMENTAL) ? "X" : ".");
			printf((codec->capabilities & AV_CODEC_CAP_DRAW_HORIZ_BAND) ? "B" : ".");
			printf((codec->capabilities & AV_CODEC_CAP_DR1) ? "D" : ".");

			printf(" %-20s %s", codec->name, codec->long_name ? codec->long_name : "");
			if (strcmp(codec->name, desc->name))
				printf(" (codec %s)", desc->name);

			printf("\n");
		}
	}
	av_free(codecs);

	return list;
}