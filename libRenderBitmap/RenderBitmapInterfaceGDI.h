// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include "RenderBitmapInterface.h"
#include <vector>
using namespace std;
class CRegardsBitmap;



namespace Regards
{
	namespace Render
	{
		class CBitmapManager
		{
		public:

			CBitmapManager(){};
			~CBitmapManager(){};

			wxString filename;
			CRegardsBitmap * texture;
		};
		
		class CRenderBitmapInterfaceGDI : public CRenderBitmapInterface
		{
		public:
			CRenderBitmapInterfaceGDI();
			virtual ~CRenderBitmapInterfaceGDI();
			void CreateScreenRender(wxDC * deviceContext, const CRgbaquad &color);
			void CreateScreenTexture(const int &width, const int &height);
			bool TestIsTextureExist(const wxString &entry);
			int AddTextureBitmap(CRegardsBitmap * bitmap, const wxString &entry);
			void AddAlphaBlendingBitmap(const wxString &textureName, const int &left, const int &top);
			void AddBitmap(const wxString &textureName, const int &left, const int &top);
			void AddBitmapToScale(const wxString &textureName, const int &widthScale, const int &heightScale, const int &left, const int &top);
			void AddBitmapToScalePosition(const wxString &textureName, const int &widthScale, const int &heightScale, const wxRect &rc, const int &left, const int &top);
			void RenderToScreen(wxDC * deviceContext);
			void Transition(const wxString &textureName, const int &step);
			void PhotoFiltre(const CRgbaquad &color, const int &intensity);
			void RGBFiltre(const int &red, const int &green, const int &blue);
			void RotateFree(const int &angle);
			void Swirl(const float &radius, const float &angle, const float &bitmapWidth, const float &bitmapHeight);
			void BrightnessAndContrast(const int &lightness, const int &contrast);
			void Posterize(const float &level, const float &gamma);
			void Solarize(const long &threshold);
			void Clouds(const CRgbaquad &color1, const CRgbaquad &color2, const float &amplitude, const float &frequence, const int &octave);
			int GetType(){ return RENDERGDI; };
			void RedEye(float &strength, const wxRect& rSelectionBox);
			int GetTextureID(const wxString &textureName);

		private:
		
			CRegardsBitmap * FindBitmap(const wxString &name);
			CRegardsBitmap * ScaleToScreen(CRegardsBitmap * in);
			float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight);


			wxDC * deviceContext = nullptr;
			int width;
			int height;
			vector<CBitmapManager *> listTexture;
		};
	}
}