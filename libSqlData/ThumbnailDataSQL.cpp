#include <header.h>
#include "ThumbnailDataSQL.h"
#include "SqlThumbnail.h"
#include "SqlThumbnailVideo.h"
#include <libPicture.h>
using namespace Regards::Sqlite;
#include <wx/filename.h>
#include <RegardsConfigParam.h>
#include <ParamInit.h>
#include <OpenCVVideoPlayer.h>
#include <VideoPlayer.h>
using namespace Regards::Video;
using namespace Regards::Picture;

CThumbnailDataSQL::CThumbnailDataSQL(const wxString& filename, const bool& testValidity, const bool& generateVideoPlayer)
	: CThumbnailData(filename)
{
	frameOut = GetDefaultPicture();

	this->generateVideoPlayer = generateVideoPlayer;

	// FIX [sérieux #6] : testValidity était ignoré. Il est maintenant utilisé
	// pour conditionner la vérification de l'existence du thumbnail en base.
	if (testValidity && !TestBitmap())
	{
		// Thumbnail absent ou invalide : on force la régénération.
		this->generateVideoPlayer = true;
	}

	CLibPicture libPicture;
	if (libPicture.TestIsVideo(filename) || libPicture.TestIsPDF(filename) || libPicture.TestIsAnimation(filename))
	{
		isVideo = true;
	}

	if (isVideo)
	{
		if (libPicture.TestIsVideoValid(filename))
		{
			nbFrame = 20;
			this->generateVideoPlayer = true;
			// FIX [critique #1] : videoCaptureCV = nullptr retiré (redondant,
			// unique_ptr est nullptr par défaut, et le commentaire mort qui suivait
			// laissait croire que le garde-fou plus bas avait de l'intérêt).
		}
		else
		{
			isAnimation = true;
			isVideo = false;

			CSqlThumbnailVideo sqlThumbnailVideo;
			nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
		}
	}

	CRegardsConfigParam* regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		useOpenCV = regardsParam->GetThumbnailOpenCV();
	}
}

CThumbnailDataSQL::~CThumbnailDataSQL(void)
{
	// FIX [critique #2] : plus de delete manuel — unique_ptr s'en charge.
	// Le destructeur se contente de libérer les cv::Mat.
	frameOut.release();

	// FIX [mineur #9] : cvImg supprimé du header, plus besoin de le release ici.
}

int CThumbnailDataSQL::GetNbFrame()
{
	if (nbFrame > 0)
		return nbFrame;
	return 1;
}

void CThumbnailDataSQL::SetMouseOn()
{
	mouseOn = true;
}

void CThumbnailDataSQL::SetMouseOut()
{
	mouseOn = false;
	oldVideoFrame = videoFramePos;
}

bool CThumbnailDataSQL::TestBitmap()
{
	CSqlThumbnail sqlThumbnail;
	wxFileName file(filename);
	wxULongLong sizeFile = file.GetSize();
	return sqlThumbnail.TestThumbnail(filename, sizeFile.ToString());
}

// Retourne la frame souhaitée.
// FIX [critique #2] : chaque chemin de retour effectue un clone() pour que
// l'appelant possède sa propre copie du buffer et ne subisse pas les effets
// de bord des appels ultérieurs à GetImage.
cv::Mat CThumbnailDataSQL::GetImage(bool& isDefault)
{
	CSqlThumbnailVideo sqlThumbnailVideo;
	CSqlThumbnail sqlThumbnail;

	if (isAnimation && nbFrame == 0)
	{
		nbFrame = sqlThumbnailVideo.GetNbThumbnail(filename);
	}

	if (numFrame >= nbFrame)
		numFrame = 0;

	// Retour rapide du cache si on n'est pas en survol et que la frame est valide.
	if (!defaultPicture)
	{
		if (isVideo && generateVideoPlayer && !mouseOn && !frameOut.empty())
		{
			// FIX [critique #2] : clone pour isoler le buffer de l'appelant.
			return frameOut.clone();
		}
	}

	if (!isVideo && numFrame == 0 && nbFrame == 0)
	{
		frameOut = sqlThumbnail.GetThumbnail(filename, isDefault);
		if (isDefault)
			frameOut = GetDefaultPicture();
	}
	else if (isVideo && generateVideoPlayer)
	{
		isDefault = false;

		// FIX [critique #1] : création du lecteur vidéo si nécessaire.
		// Si le constructeur lève une exception, videoCaptureCV reste nullptr
		// et le bloc de fallback ci-dessous renverra frameOut sans planter.
		if (videoCaptureCV == nullptr)
		{
			try
			{
				if (useOpenCV)
					videoCaptureCV = std::make_unique<COpenCVVideoPlayer>(filename);
				else
					videoCaptureCV = std::make_unique<CVideoPlayer>(filename);
			}
			catch (...)
			{
				// Échec d'initialisation : on tombera dans le fallback frameOut.
			}
		}

		// FIX [critique #1] : le garde-fou qui était mort est désormais utile :
		// si la création a échoué, on renvoie la dernière frame connue.
		if (videoCaptureCV == nullptr)
		{
			isDefault = frameOut.empty();
			return frameOut.clone();
		}

		if (numFrame < nbFrame)
		{
			bool grabbed = false;

			// FIX [mineur #10] : isVideo est garanti vrai dans ce bloc,
			// la vérification redondante est supprimée.
			if (mouseOn && videoCaptureCV != nullptr)
			{
				bool invertRotation = !useOpenCV ? false : true;

				frameOut = videoCaptureCV->GetVideoFrame(true, invertRotation);
				if (frameOut.empty())
				{
					videoFramePos = 0;
					videoCaptureCV->SeekToBegin();
					frameOut = videoCaptureCV->GetVideoFrame(true, invertRotation);
					grabbed = true;
				}
				else
				{
					videoFramePos++;
					grabbed = true;
				}
			}

			if (!grabbed)
			{
				frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
			}
		}
	}
	else if (isVideo && !generateVideoPlayer)
	{
		// FIX [critique #3] : suppression du double appel SQL inutile.
		// Si numFrame est absent, on essaie directement la frame 0.
		// Dans tous les cas, le fallback final sur GetDefaultPicture() suffit.
		frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
		if (isDefault && numFrame != 0)
		{
			frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0, isDefault);
		}
	}
	else if (isAnimation)
	{
		// FIX [critique #3] : même correction que ci-dessus.
		frameOut = sqlThumbnailVideo.GetThumbnail(filename, numFrame, isDefault);
		if (isDefault && numFrame != 0)
		{
			frameOut = sqlThumbnailVideo.GetThumbnail(filename, 0, isDefault);
		}
	}

	if (frameOut.empty())
		isDefault = true;

	defaultPicture = isDefault;
	if (isDefault)
	{
		frameOut = GetDefaultPicture();
	}

	// FIX [critique #2] : clone systématique avant retour.
	return frameOut.clone();
}