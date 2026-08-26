#include "header.h"
#include "Icone.h"
#include <ThumbnailData.h>
#include <ParamInit.h>
#include <LibResource.h>
#include <FileUtility.h>
#include "LoadingResource.h"
#include "WindowMain.h"
#include <config_id.h>
#ifdef WIN32
#endif
#include <RegardsConfigParam.h>
#include <libPicture.h>
#include <ThumbnailDataFace.h>
using namespace Regards::Picture;
using namespace Regards::Window;



wxImage CIcone::videoCadre;
wxImage CIcone::photoTemp;

void CIcone::ReinitPos()
{
	x = oldx;
	y = oldy;
}

void CIcone::InitPos()
{
	oldx = x;
	oldy = y;
}

void CIcone::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}

int CIcone::GetXPos() const
{
	return x;
}

int CIcone::GetYPos() const
{
	return y;
}

int CIcone::GetWidth()
{
	width = themeIcone.GetWidth() - (themeIcone.GetMarge() << 1);
	return width;
}

int CIcone::GetHeight()
{
	if (themeIcone.showOnlyThumbnail)
		height = themeIcone.GetHeight() - (themeIcone.GetMarge() << 1);
	else
		height = static_cast<int>(static_cast<float>(themeIcone.GetHeight()) * 0.75);

	return height;
}

void CIcone::SetWindowPos(int x, int y)
{
	double scale_factor = 1.0f;
	this->x = x / scale_factor;
	this->y = y / scale_factor;
}

wxRect CIcone::GetPos()
{
	double scale_factor = 1.0f;
	wxRect rc;
	rc.x = x;
	rc.width = themeIcone.GetWidth() / scale_factor;
	rc.y = y;
	rc.height = themeIcone.GetHeight() / scale_factor;
	return rc;
}

void CIcone::SetSizeIcone(const int& width, const int& height)
{
	if (width != themeIcone.GetRealWidth() || height != themeIcone.GetRealHeight())
	{
		themeIcone.SetWidth(width);
		themeIcone.SetHeight(height);
		scaleBackup.Destroy();
		photoDefault = false;
		Invalidate();
	}
}

void CIcone::SetTheme(CThemeIcone theme)
{
	themeIcone = theme;
	Invalidate();
}


void CIcone::SetSelected(bool value)
{
	isSelected = value;
	if (!value)
		state = INACTIFICONE;
	else
		state = SELECTEDICONE;
	Invalidate();
}

bool CIcone::GetSelected() const
{
	return isSelected;
}


wxImage CIcone::LoadImageResource(const wxString& resourceName)
{
	wxImage bitmap = CLoadingResource::LoadImageResource(resourceName);
	return bitmap;
}

CIcone::CIcone(CThumbnailData* data) : numElement(0), oldx(0), oldy(0)
{
	pThumbnailData = nullptr;
	showSelected = false;
	isChecked = false;
	isSelected = false;
	photoDefault = true;
	x = 0;
	y = 0;
	config = nullptr;
	width = 0;
	height = 0;
	showLoading = false;
	pictureLoad = false;
	posXThumbnail = 0;
	posYThumbnail = 0;
	showDeleted = false;
	state = INACTIFICONE;
	isChecked = false;
	config = CParamInit::getInstance();

	if (data != nullptr)
	{
		numElement = data->GetNumElement();
		pThumbnailData.reset(data);
	}
	else
	{
		numElement = 0;
		pThumbnailData = nullptr;
	}

	useBackgroundColor = false;
	thumbnailIconeCache = config->GetThumbnailIconeCache();
	Invalidate();
}

void CIcone::SetBackgroundColor(const wxColour& backgroundColor)
{
	this->backgroundColor = backgroundColor;
	useBackgroundColor = true;
	state = USEBACKGROUNDCOLOR;
	Invalidate();
}

void CIcone::StartLoadingPicture()
{
	showLoading = true;
}


void CIcone::StopLoadingPicture()
{
	showLoading = false;
}


void CIcone::SetPictureLoading(const wxImage& imageLoading)
{
	pictureLoading = wxImage(imageLoading);
}

void CIcone::GenerateVideoIcone()
{
	if (!videoCadre.IsOk() || videoCadre.GetWidth() != themeIcone.GetWidth())
	{
		wxImage image = LoadImageResource(L"IDB_CADRE_VIDEO");
		videoCadre = image.ResampleBicubic(themeIcone.GetWidth(), image.GetHeight());
	}
}

int CIcone::OnClick(int x, int y, int posLargeur, int posHauteur)
{
	wxRect checkPos;
	int xPos = (x + posLargeur) - this->x;
	int yPos = (y + posHauteur) - this->y;
	checkPos.x = themeIcone.GetMarge();
	checkPos.width = checkPos.x + themeIcone.GetCheckboxWidth();
	checkPos.y = themeIcone.GetHeight() - themeIcone.GetCheckboxHeight();
	checkPos.height = checkPos.y + themeIcone.GetCheckboxHeight();
	if ((checkPos.x < xPos && xPos < checkPos.width) && (checkPos.y < yPos && yPos < checkPos.height))
	{
		isChecked = !isChecked;
		SetChecked(isChecked);
		return 1;
	}

	if (showDeleted && bitmapDelete.IsOk())
	{
		checkPos.x = themeIcone.GetWidth() - (bitmapDelete.GetWidth() + themeIcone.GetMarge());
		checkPos.width = checkPos.x + bitmapDelete.GetWidth();
		checkPos.y = themeIcone.GetHeight() - themeIcone.GetCheckboxHeight();
		checkPos.height = checkPos.y + themeIcone.GetCheckboxHeight();
		if ((checkPos.x < xPos && xPos < checkPos.width) && (checkPos.y < yPos && yPos < checkPos.height))
		{
			return 2;
		}
	}


	return 0;
}

void CIcone::RenderPictureBitmap(wxDC* memDC, wxImage& bitmapScale, const int& type)
{
	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = themeIcone.GetWidth();
	rc.height = themeIcone.GetHeight();

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked)
			memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
		else
			memDC->GradientFillLinear(rc, themeIcone.colorBack, themeIcone.colorBack);
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
		if (isChecked)
			memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
		else
			memDC->GradientFillLinear(rc, themeIcone.colorTop, themeIcone.colorBottom);
		break;
	default:
		if (useBackgroundColor)
			CWindowMain::FillRect(memDC, rc, backgroundColor);
		else
			CWindowMain::FillRect(memDC, rc, themeIcone.colorBack);
		break;
	}


	if (pThumbnailData != nullptr)
	{
		CalculPosition(bitmapScale);
		if (bitmapScale.IsOk())
			memDC->DrawBitmap(bitmapScale, posXThumbnail, themeIcone.GetMarge());

		if (!themeIcone.showOnlyThumbnail)
		{
			wxString libelle;

			if (pThumbnailData->GetTypeElement() == TYPEPHOTO)
				libelle = CFileUtility::GetFileName(pThumbnailData->GetFilename());
			else if (pThumbnailData->GetTypeElement() == TYPEMULTIPAGE)
				libelle = pThumbnailData->GetLibelle();
			else
				libelle = pThumbnailData->GetFilename();

			if (libelle != "")
			{
				CThemeFont themeFont = themeIcone.font;
				wxSize sizeTexte;
				do
				{
					sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeFont);
					if (sizeTexte.x > (themeIcone.GetWidth() - (themeIcone.GetMarge() * 2)))
						themeFont.SetFontSize(themeFont.GetFontRealSize() - 1);
				} while (sizeTexte.x > (themeIcone.GetWidth() - (themeIcone.GetMarge() * 2)));

				int xPos = (themeIcone.GetWidth() - sizeTexte.x) / 2;
				//int yPos = bitmapScale.GetHeight() + themeIcone.GetMarge() + ((themeIcone.GetHeight() - (bitmapScale.GetHeight() + themeIcone.GetMarge())) - sizeTexte.y) / 2;
				int yPos = rc.height - (themeIcone.GetMarge() * 2 + sizeTexte.y);
				CWindowMain::DrawTexte(memDC, libelle, xPos, yPos, themeFont);
			}
		}

		if (showSelected)
		{
			if (!bitmapCheckOn.IsOk() || (bitmapCheckOn.GetHeight() != themeIcone.GetCheckboxHeight() || bitmapCheckOn.
				GetWidth() != themeIcone.GetCheckboxWidth()))
			{
				bitmapCheckOn = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_ON", themeIcone.GetCheckboxWidth(),
					themeIcone.GetCheckboxHeight());
			}

			if (!bitmapCheckOff.IsOk() || (bitmapCheckOff.GetHeight() != themeIcone.GetCheckboxHeight() ||
				bitmapCheckOff.GetWidth() != themeIcone.GetCheckboxWidth()))
			{
				bitmapCheckOff = CLibResource::CreatePictureFromSVG("IDB_CHECKBOX_OFF", themeIcone.GetCheckboxWidth(),
					themeIcone.GetCheckboxHeight());
			}

			switch (type)
			{
			case INACTIFICONE:
				if (isChecked && bitmapCheckOn.IsOk())
					memDC->DrawBitmap(bitmapCheckOn.ConvertToDisabled(), themeIcone.GetMarge(),
						themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
				else if (bitmapCheckOff.IsOk())
					memDC->DrawBitmap(bitmapCheckOff.ConvertToDisabled(), themeIcone.GetMarge(),
						themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());
				break;
			default:
				if (isChecked && bitmapCheckOn.IsOk())
					memDC->DrawBitmap(bitmapCheckOn, themeIcone.GetMarge(),
						themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
				else if (bitmapCheckOff.IsOk())
					memDC->DrawBitmap(bitmapCheckOff, themeIcone.GetMarge(),
						themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());
				break;
			}
		}

		if (showDeleted)
		{
			if (!bitmapDelete.IsOk() || (bitmapDelete.GetHeight() != themeIcone.GetCheckboxHeight() || bitmapDelete.
				GetWidth() != themeIcone.GetCheckboxWidth()))
			{
				bitmapDelete = CLibResource::CreatePictureFromSVG("IDB_DELETE", themeIcone.GetCheckboxWidth(),
					themeIcone.GetCheckboxHeight());
				bitmapDelete = bitmapDelete.ConvertToDisabled();
			}

			memDC->DrawBitmap(bitmapDelete, themeIcone.GetWidth() - (bitmapDelete.GetWidth() + themeIcone.GetMarge()),
				themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapDelete.GetHeight());
		}
	}

	if (showLoading && pictureLoading.IsOk())
	{
		if (!transparent.IsOk() ||
			transparent.GetWidth() != bitmapScale.GetWidth() ||
			transparent.GetHeight() != bitmapScale.GetHeight())
		{
			const int width = bitmapScale.GetWidth();
			const int height = bitmapScale.GetHeight();

			transparent = wxImage(width, height);
			transparent.InitAlpha();

			unsigned char* rgb = transparent.GetData();
			unsigned char* alpha = transparent.GetAlpha();

			const size_t pixelCount =
				static_cast<size_t>(width) * static_cast<size_t>(height);

			// Blanc
			std::memset(rgb, 255, pixelCount * 3);

			// Alpha 50 %
			std::memset(alpha, 128, pixelCount);
		}

		if (transparent.IsOk())
			memDC->DrawBitmap(transparent, posXThumbnail, themeIcone.GetMarge());


		//wxImage picture = pictureLoading.Scale(bitmapScale.GetWidth() / 2, bitmapScale.GetHeight() /2);

		int xLocal = posXThumbnail + (bitmapScale.GetWidth() - pictureLoading.GetWidth()) / 2;
		int yLocal = themeIcone.GetMarge() + (bitmapScale.GetHeight() - pictureLoading.GetHeight()) / 2;
		//if(pictureLoading.HasMask())

		if (pictureLoading.IsOk())
			memDC->DrawBitmap(pictureLoading.ConvertToGreyscale(), xLocal, yLocal, true);
	}
}

void CIcone::SetShowDelete(bool value)
{
	showDeleted = value;
}

void CIcone::RenderVideoBitmap(wxDC* memDC, wxImage& bitmapScale, const int& type)
{
	GenerateVideoIcone();

	wxImage bitmapImageActif;
	wxImage bitmapImageCadreActif = videoCadre.ConvertToDisabled();
	//int xBitmapPos = x + (themeIcone.GetWidth() - GetWidth()) / 2;
	//int bottomPos = 0;

	wxRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = themeIcone.GetWidth();
	rc.height = themeIcone.GetHeight();

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked)
			memDC->GradientFillLinear(rc, themeIcone.colorSelectTop, themeIcone.colorSelectBottom);
		else
			CWindowMain::FillRect(memDC, rc, themeIcone.colorBack);
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
		memDC->GradientFillLinear(rc, themeIcone.colorTop, themeIcone.colorBottom);
		break;
	default:;
	}

	switch (type)
	{
	case INACTIFICONE:
		if (isChecked && bitmapImageCadreActif.IsOk())
			memDC->DrawBitmap(bitmapImageCadreActif, 0, 0);
		else if (videoCadre.IsOk())
			memDC->DrawBitmap(videoCadre, 0, 0);
		break;
	case SELECTEDICONE:
	case ACTIFICONE:
		if (bitmapImageCadreActif.IsOk())
			memDC->DrawBitmap(bitmapImageCadreActif, 0, 0);
		break;
	default:;
	}

	//Size Thumbnail Max
	int heightThumbnailMax = (themeIcone.GetHeight() - 40) - (videoCadre.GetHeight() * 2);
	if (bitmapScale.GetHeight() > heightThumbnailMax)
		bitmapImageActif = bitmapScale.ResampleBicubic(bitmapScale.GetWidth(), heightThumbnailMax);
	else
		bitmapImageActif = bitmapScale;

	if (pThumbnailData != nullptr)
	{
		int y_pos;
		CalculPosition(bitmapScale);

		if (!themeIcone.showOnlyThumbnail)
		{
			wxString libelle = pThumbnailData->GetTimePositionLibelle();
			//wxSize sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeIcone.font);
			y_pos = videoCadre.GetHeight();
		}
		else
		{
			y_pos = videoCadre.GetHeight() + ((themeIcone.GetHeight() - (videoCadre.GetHeight() *
				2)) - bitmapImageActif.GetHeight()) / 2;
		}

		switch (type)
		{
		case INACTIFICONE:
			if (isChecked)
			{
				if (bitmapImageActif.IsOk())
					memDC->DrawBitmap(bitmapImageActif, posXThumbnail, y_pos);

				if (bitmapImageCadreActif.IsOk())
					memDC->DrawBitmap(bitmapImageCadreActif, 0,
						themeIcone.GetHeight() - videoCadre.GetHeight());
			}
			else
			{
				memDC->DrawBitmap(bitmapImageActif.ConvertToGreyscale(), posXThumbnail, y_pos);
				memDC->DrawBitmap(videoCadre, 0, themeIcone.GetHeight() - videoCadre.GetHeight());
			}
			break;
		case SELECTEDICONE:
		case ACTIFICONE:
			memDC->DrawBitmap(bitmapImageActif, posXThumbnail, y_pos);
			memDC->DrawBitmap(bitmapImageCadreActif, 0, themeIcone.GetHeight() - videoCadre.GetHeight());
			break;
		default:;
		}

		if (!themeIcone.showOnlyThumbnail)
		{
			const wxString libelle = pThumbnailData->GetTimePositionLibelle();
			if (libelle != L"")
			{
				CThemeFont themeFont = themeIcone.font;
				wxSize sizeTexte;
				do
				{
					sizeTexte = CWindowMain::GetSizeTexte(memDC, libelle, themeFont);
					if (sizeTexte.x > themeIcone.GetWidth())
						themeFont.SetFontSize(themeFont.GetFontSize() - 1);
				} while (sizeTexte.x > themeIcone.GetWidth());

				int xPos = (themeIcone.GetWidth() - sizeTexte.x) / 2;
				int y = (themeIcone.GetHeight() - (videoCadre.GetHeight() * 2 + bitmapImageActif.GetHeight()
					+ sizeTexte.y)) / 2;
				CWindowMain::DrawTexte(memDC, libelle, xPos, y_pos + y + bitmapImageActif.GetHeight(), themeFont);
			}
		}

		if (showSelected)
		{
			if (isChecked)
				memDC->DrawBitmap(bitmapCheckOn, themeIcone.GetMarge(),
					themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOn.GetHeight());
			else
				memDC->DrawBitmap(bitmapCheckOff, themeIcone.GetMarge(),
					themeIcone.GetHeight() - themeIcone.GetMarge() - bitmapCheckOff.GetHeight());
		}
	}
}

void CIcone::RenderBitmap(wxDC* memdc, wxImage& bitmapScale, const int& type)
{
	if (memdc == nullptr || pThumbnailData == nullptr)
		return;

	if (pThumbnailData->GetTypeElement() == TYPEVIDEO)
		return RenderVideoBitmap(memdc, bitmapScale, type);

	return RenderPictureBitmap(memdc, bitmapScale, type);
}

int CIcone::GetBitmapWidth()
{
	return GetWidth();
}

int CIcone::GetBitmapHeight()
{
	return GetHeight();
}

void CIcone::Invalidate() noexcept
{
	redraw = true;
}

CThumbnailData* CIcone::GetPtData()
{
	return pThumbnailData.get();
}

void CIcone::CalculPosition(const wxImage& render)
{
	int localx = (themeIcone.GetWidth() - GetWidth()) / 2;
	int localy = themeIcone.GetMarge();
	//float newRatio = 1.0f;
	int left;
	int top;

	if (GetWidth() > render.GetWidth())
		left = ((GetWidth() - render.GetWidth()) / 2);
	else
	{
		left = 0;
	}

	if (GetHeight() > render.GetHeight())
	{
		top = ((GetHeight() - render.GetHeight()) / 2);
	}
	else
	{
		top = 0;
	}

	posXThumbnail = localx + left;
	posYThumbnail = localy + top;
}


wxBitmap CIcone::GetCopyIcone() const
{
	return wxBitmap(localmemBitmap_backup);
}

void CIcone::RefreshIcone()
{
	photoDefault = false;
	Invalidate();
}

void CIcone::GetBitmapIcone(
	int& returnValue,
	const bool& flipHorizontal,
	const bool& flipVertical,
	const bool& forceRedraw)
{
	returnValue = 0;

	if (forceRedraw)
		Invalidate();

	// -------------------------------------------------------------------------
	// Impossible de construire l'icône sans ThumbnailData.
	// -------------------------------------------------------------------------
	if (pThumbnailData == nullptr)
	{
		returnValue = 1;
		return;
	}

	// -------------------------------------------------------------------------
	// Récupération de l'image.
	// IMPORTANT : on conserve exactement la sémantique actuelle de
	// photoDefault avec GetImage().
	// -------------------------------------------------------------------------
	wxImage image(20, 20);

	cv::Mat mat = pThumbnailData->GetImage(photoDefault);
	image = CLibPicture::ConvertRegardsBitmapToWXImage(mat);

	if (!photoDefault)
	{
		if (image.IsOk())
			Invalidate();

		returnValue = 0;
	}
	else
	{
		returnValue = 1;
	}

	// -------------------------------------------------------------------------
	// Si aucune image n'est disponible, utiliser l'image par défaut.
	// -------------------------------------------------------------------------
	if (!image.IsOk())
	{
		if (pThumbnailData->IsVideo() ||
			pThumbnailData->IsAnimation())
		{
			image = application_context.GetWxDefaultVideoThumbnail();
		}
		else
		{
			image = application_context.GetWxDefaultPictureThumbnail();
		}

		photoDefault = true;
		returnValue = 1;
	}

	// -------------------------------------------------------------------------
	// Vérification du cache du bitmap final.
	// -------------------------------------------------------------------------
	const bool bitmapSizeChanged =
		localmemBitmap_backup.GetWidth() != themeIcone.GetWidth() ||
		localmemBitmap_backup.GetHeight() != themeIcone.GetHeight();

	if (!redraw && !bitmapSizeChanged)
		return;

	wxBitmap localmemBitmap(
		themeIcone.GetWidth(),
		themeIcone.GetHeight());

	wxMemoryDC memDC;
	memDC.SelectObject(localmemBitmap);

	try
	{
		

		// ---------------------------------------------------------------------
		// Calcul des dimensions d'affichage.
		// ---------------------------------------------------------------------
		int tailleAffichageBitmapWidth = 0;
		int tailleAffichageBitmapHeight = 0;
		float ratio = 0.0f;

		GetBitmapDimension(
			image.GetWidth(),
			image.GetHeight(),
			tailleAffichageBitmapWidth,
			tailleAffichageBitmapHeight,
			ratio);

		// ---------------------------------------------------------------------
		// Le cache de l'image redimensionnée peut être réutilisé uniquement
		// si ses dimensions correspondent.
		// ---------------------------------------------------------------------
		const bool rebuildScale =
			!scaleBackup.IsOk() ||
			!photoDefault ||
			scaleBackup.GetWidth() != tailleAffichageBitmapWidth ||
			scaleBackup.GetHeight() != tailleAffichageBitmapHeight;

		if (rebuildScale)
		{
			// -----------------------------------------------------------------
			// On conserve la logique originale.
			// -----------------------------------------------------------------
			if (pThumbnailData != nullptr)
			{
				if (!image.IsOk())
				{
					image = wxImage(
						themeIcone.GetWidth(),
						themeIcone.GetHeight());
				}
				else
				{
					photoDefault = true;
				}

				// -------------------------------------------------------------
				// IMPORTANT :
				// Mirror() retourne une nouvelle wxImage.
				// Pas de std::move.
				// -------------------------------------------------------------
				if (flipHorizontal)
					image = image.Mirror(true);

				if (flipVertical)
					image = image.Mirror(false);
			}

			// -----------------------------------------------------------------
			// Redimensionnement.
			// -----------------------------------------------------------------
			wxImage scale;

			if (image.IsOk())
			{
				if (config->GetThumbnailQuality() == 0)
				{
					scale = image.Scale(
						tailleAffichageBitmapWidth,
						tailleAffichageBitmapHeight);
				}
				else if (photoDefault)
				{
					scale = image.ResampleBicubic(
						tailleAffichageBitmapWidth,
						tailleAffichageBitmapHeight);
				}
				else
				{
					// ---------------------------------------------------------
					// Image temporaire.
					// ---------------------------------------------------------
					if (!photoTemp.IsOk() ||
						photoTemp.GetWidth() != tailleAffichageBitmapWidth ||
						photoTemp.GetHeight() != tailleAffichageBitmapHeight)
					{
						const wxColor colorToReplace(0, 0, 0);
						const wxColor colorActifReplacement(255, 255, 255);

						photoTemp = CLibResource::CreatePictureFromSVG(
							"IDB_PHOTOTEMP",
							tailleAffichageBitmapWidth,
							tailleAffichageBitmapHeight);

						if (photoTemp.IsOk())
						{
							photoTemp.Replace(
								colorToReplace.Red(),
								colorToReplace.Green(),
								colorToReplace.Blue(),
								colorActifReplacement.Red(),
								colorActifReplacement.Green(),
								colorActifReplacement.Blue());
						}
					}

					scale = photoTemp;
				}
			}

			// -----------------------------------------------------------------
			// IMPORTANT :
			// On fait volontairement une copie.
			//
			// scaleBackup est le cache permanent.
			// Ne pas utiliser std::move ici.
			// -----------------------------------------------------------------
			scaleBackup = scale;
			scale.Destroy();
		}

		// ---------------------------------------------------------------------
		// Rendu à partir du cache.
		// ---------------------------------------------------------------------
		RenderBitmap(
			&memDC,
			scaleBackup,
			state);

		memDC.SelectObject(wxNullBitmap);

		// ---------------------------------------------------------------------
		// Mise à jour du bitmap final.
		// ---------------------------------------------------------------------
		localmemBitmap_backup = localmemBitmap;

		redraw = false;
	}
	catch (...)
	{
		// Toujours désélectionner le bitmap de la wxMemoryDC.
		memDC.SelectObject(wxNullBitmap);
	}

}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int CIcone::RenderIcone(wxDC* dc, const int& posLargeur, const int& posHauteur, const bool& flipHorizontal,
	const bool& flipVertical, const bool& forceRedraw)
{
	int returnValue = 0;
	if (!visible)
		return 0;


	try
	{
		if (pThumbnailData != nullptr)
		{
			if (pThumbnailData->GetNbFrame() > 1 && (state == ACTIFICONE || state == SELECTEDICONE))
			{
				int numFrame = pThumbnailData->GetNumFrame();
				pThumbnailData->SetNumFrame(numFrame + 1);
			}
		}

		GetBitmapIcone(returnValue, flipHorizontal, flipVertical, forceRedraw);
		dc->DrawBitmap(localmemBitmap_backup, x + posLargeur, y + posHauteur);
	}
	catch (...)
	{
		printf("toto");
	}

	return returnValue;
}

bool CIcone::GetVisibility()
{
	return visible;
}
void CIcone::SetVisibility(const bool& visible)
{
	this->visible = visible;
}

void CIcone::SetNumFrame(int numFrame)
{
	if (pThumbnailData != nullptr)
		pThumbnailData->SetNumFrame(numFrame);
}


void CIcone::SetActive(bool value)
{
	if (state != SELECTEDICONE)
	{
		if (value)
			state = ACTIFICONE;
		else
			state = INACTIFICONE;
	}

	if (pThumbnailData != nullptr)
	{
		if (value)
			pThumbnailData->SetMouseOn();
		else
			pThumbnailData->SetMouseOut();
	}

	Invalidate();
}


bool CIcone::IsChecked() const
{
	return isChecked;
}

void CIcone::SetChecked(bool value)
{
	isChecked = value;
	Invalidate();
}

float CIcone::CalculRatio(
	const int& width,
	const int& height,
	const int& maxWidth,
	const int& maxHeight)
{
	float ratio;

	if (width > height)
	{
		ratio = static_cast<float>(maxWidth) / width;

		if (height * ratio > maxHeight)
			ratio = static_cast<float>(maxHeight) / height;
	}
	else
	{
		ratio = static_cast<float>(maxHeight) / height;

		if (width * ratio > maxWidth)
			ratio = static_cast<float>(maxWidth) / width;
	}

	return std::min(ratio, 1.0f);
}

void CIcone::GetBitmapDimension(const int& width, const int& height, int& tailleAffichageBitmapWidth,
	int& tailleAffichageBitmapHeight, float& newRatio)
{
	newRatio = CalculRatio(width, height, GetWidth(), GetHeight());

	tailleAffichageBitmapWidth = static_cast<int>(static_cast<float>(width) * newRatio);
	tailleAffichageBitmapHeight = static_cast<int>(static_cast<float>(height) * newRatio);
}