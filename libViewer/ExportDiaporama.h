#pragma once

class CFFmpegTranscoding;

namespace Regards::Viewer
{
	class CExportDiaporama
	{
	public:
		CExportDiaporama(wxWindow* parent);
		~CExportDiaporama() {};
		void OnExportDiaporama();
		void OnEndDecompressFile(int ret);
		void ExportVideo(const wxString& filename);

	private:
		wxString tempVideoFile;
		wxString tempAudioVideoFile;
		wxString fileOut;
		wxString filepathVideo;
		wxString fileOutVideo;
		wxString fileOutAudio;
		bool isAudio;
		bool needToRemux;
		CFFmpegTranscoding* ffmpegEncoder = nullptr;
		wxWindow* parent;
	};
}