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
            void RenderEffect(const int &numEffect, CEffectParameter * effectParameter);
            void RotateFree(const int &angle);
			int GetType(){ return RENDERGDI; };
			
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