#pragma once
#include <TreeControl.h>
#include <RegardsBitmap.h>
#include <BitmapWndViewer.h>
#include "EffectParameter.h"
using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CFiltreEffect : public CTreeControl
		{
		public:

			CFiltreEffect(CBitmapWndViewer * bitmapViewer, CThemeTree * theme, CTreeElementControlInterface * interfaceControl);
			~CFiltreEffect(void);
			virtual void Init(CEffectParameter * effectParameter, CRegardsBitmap * source, const int &filtre);
			virtual void SlidePosChange(CTreeElement * treeElement, const int &position, const int &value, const wxString &key);

            void UpdateScreenRatio();

		protected:

			void MouseOver(wxDC * deviceContext, CPositionElement * element, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void ClickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);
			void UnclickOnElement(CPositionElement * element, wxWindow * window, const int &x, const int &y, const int& posLargeur, const int &posHauteur);

			void UpdateElement();
			void UpdateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void CreateElement();
			void CreateChildTree(tree<CTreeData *>::sibling_iterator &parent);
			void AddTreeInfos(const wxString &exifKey, const int &position, const vector<int> & value, const int &index, tree<CTreeData *>::iterator &top, tree<CTreeData *>::iterator &child);

			void RotateFree();
			void RotateFreeChangeParam(const int &valueData, const wxString &key);

			void Solarisation();
			void SolarisationChangeParam(const int &valueData, const wxString &key);

			void BrightAndContrast();
			void BrightAndContrastChangeParam(const int &valueData, const wxString &key);

			void PhotoFiltre();
			void PhotoFiltreChangeParam(const int &valueData, const wxString &key);

			void Posterisation();
			void PosterisationChangeParam(const int &valueData, const wxString &key);

			void RGBFiltre();
			void RGBFiltreChangeParam(const int &valueData, const wxString &key);

			void Swirl();
			void SwirlChangeParam(const int &valueData, const wxString &key);

			void Clouds();
			void CloudsChangeParam(const int &valueData, const wxString &key);

			void MotionBlur();
			void MotionBlurChangeParam(const int &valueData, const wxString &key);

			void LensFlare();
			void LensFlareChangeParam(const int &valueData, const wxString &key);

			void Crop();

			int filtre = 0;

			int yPos;
			int cxMax;
			int cyMax;
			int xMaxPos;
			int xMaxPosValue;
			int xMinPos;
			int widthPosition = 0;

			CEffectParameter * effectParameter;

			wxString libelleEffectRadius = L"";
			wxString libelleEffectSigma = L"";
			wxString libelleEffectAngle = L"";
			wxString libelleEffectColorRed = L"";
			wxString libelleEffectColorGreen = L"";
			wxString libelleEffectColorBlue = L"";
			wxString libelleEffectLevel = L"";
			wxString libelleEffectGamma = L"";
			wxString libelleEffectIntensity = L"";
			wxString libelleEffectContrast = L"";
			wxString libelleEffectLightness = L"";
			wxString libelleEffectThreshold = L"";
			wxString libelleRotationAngle = L"";
			wxString libelleCloudsFrequency = L"";
			wxString libelleCloudsAmplitude = L"";
			wxString libelleCloudsColorFrontRed = L"";
			wxString libelleCloudsColorFrontGreen = L"";
			wxString libelleCloudsColorFrontBlue = L"";
			wxString libelleCloudsColorBackRed = L"";
			wxString libelleCloudsColorBackGreen = L"";
			wxString libelleCloudsColorBackBlue = L"";
			wxString libelleColor = L"";
			CRegardsBitmap * source = nullptr;
			CBitmapWndViewer * bitmapViewer = nullptr;
		};
	}
}