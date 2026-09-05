#pragma once
#include "PictureArray.h"

// Optionnel : s'assurer que les en-têtes FFmpeg sont inclus s'ils ne le sont pas via PictureArray.h
extern "C" {
#include <libavutil/frame.h>
}

class CDataAVFrame
{
public:
	CDataAVFrame()
	{}

	virtual ~CDataAVFrame()
	{
		// Libération automatique du compteur de référence de la matrice OpenCV
		matFrame.release();

		// Libération conforme à l'API FFmpeg 7.x (gère le nettoyage interne complet)
		if (dst != nullptr)
		{
			av_frame_free(&dst);
		}
	}

	float sample_aspect_ratio = 0.0;
	cv::Mat matFrame;
	int width;
	int height;
	bool isHardwareDecoding = false;
	float ratioVideo;
	AVFrame* dst = nullptr;
};