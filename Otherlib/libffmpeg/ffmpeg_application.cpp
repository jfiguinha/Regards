#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/progdlg.h>
#include <stdexcept>
#include "ffmpeg_application.h"
#include <ConvertUtility.h>
#include <wx/process.h>
#include <FileUtility.h> 
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avassert.h>

}



void CFFmpegApp::ExecuteFFmpeg()
{
	//arrayOfCstrings = new char*[arrayOfStrings.size()];

    wxString commande = "";
	for (int i = 0; i < arrayOfStrings.size(); i++)
    {
        cout << "ExecuteFFmpeg " << to_string(i) << " " << arrayOfStrings[i] <<endl;
		commande.append(arrayOfStrings[i]);
        commande.append(" ");
    }
    

	try
	{
        int x = 0;
        
        cout << "Commande : " << commande << endl;
     
        int pid = wxExecute(commande,  wxEXEC_SHOW_CONSOLE | wxEXEC_SYNC);


	}
	catch (int e)
	{
		ret = e;
	}
	catch (...)
	{
	}


}


CFFmpegApp::CFFmpegApp(const bool showProgressWindow)
{
    this->showProgressWindow = showProgressWindow;
    
    wxString workingFolder = CFileUtility::GetProgramFolderPath();

#ifdef WIN32
    arrayOfStrings.push_back(workingFolder + "\\ffmpeg.exe");
#else
    arrayOfStrings.push_back(workingFolder + "/ffmpeg");
#endif
};

int CFFmpegApp::ExecuteFFmpegAddRotateInfo(const wxString& inputVideoFile, const wxString& outputFile,
                                           const int& rotate)
{
	//ffmpeg -i 20200509_132328.mp4 -map_metadata 0 -metadata:s:v rotate=-90 -codec copy output.mp4
	
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile);
	arrayOfStrings.push_back("-map_metadata");
	arrayOfStrings.push_back("0");
	arrayOfStrings.push_back("-metadata:s:v");
	arrayOfStrings.push_back("rotate=" + std::to_string(rotate));
	arrayOfStrings.push_back("-codec");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();
	return 0;
}

int CFFmpegApp::CropAudio(const wxString& inputAudioFile, const wxString& timeVideo, const wxString& extension,
                          const wxString& outputFile)
{
	
	arrayOfStrings.push_back("-stream_loop");
	arrayOfStrings.push_back("-1");
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile);
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back(extension);
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo);
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegApp(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                 const wxString& timeVideo, const wxString& outputFile)
{
	
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo);
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile);
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back("00:00:00");
	arrayOfStrings.push_back("-t");
	arrayOfStrings.push_back(timeVideo);
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile);
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("0:v:0");
	arrayOfStrings.push_back("-map");
	arrayOfStrings.push_back("1:a:0");
	arrayOfStrings.push_back("-y");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();

	return 0;
}

int CFFmpegApp::ExecuteFFmpegCutVideo(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                      const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 


	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile);
	arrayOfStrings.push_back("-ss");
	arrayOfStrings.push_back(timeVideoIn);
	arrayOfStrings.push_back("-to");
	arrayOfStrings.push_back(timeVideoOut);
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractVideo(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                          const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile);
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn);
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut);
	}
	arrayOfStrings.push_back("-c:v");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-an");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegExtractAudio(const wxString& inputVideoFile, const wxString& timeVideoIn,
                                          const wxString& timeVideoOut, const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	
	arrayOfStrings.push_back("-i");
    cout << "Input Video File : " << inputVideoFile << endl;
	arrayOfStrings.push_back(inputVideoFile);
	if (timeVideoIn != "00:00:00" || timeVideoOut != "00:00:00")
	{
		arrayOfStrings.push_back("-ss");
		arrayOfStrings.push_back(timeVideoIn);
		arrayOfStrings.push_back("-to");
		arrayOfStrings.push_back(timeVideoOut);
	}
	arrayOfStrings.push_back("-c:a");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back("-vn");
	arrayOfStrings.push_back(outputFile);
	ExecuteFFmpeg();

	return 0;
}


int CFFmpegApp::ExecuteFFmpegMuxVideoAudio(const wxString& inputVideoFile, const wxString& inputAudioFile,
                                           const wxString& outputFile)
{
	//ffmpeg -i movie.mp4 -ss 00:00:03 -t 00:00:08 -async 1 -c copy cut.mp4 

	
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputVideoFile);
	arrayOfStrings.push_back("-i");
	arrayOfStrings.push_back(inputAudioFile);
	arrayOfStrings.push_back("-c");
	arrayOfStrings.push_back("copy");
	arrayOfStrings.push_back(outputFile);
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
