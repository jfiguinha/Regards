#pragma once

class CImageLoadingFormat;
class CEffectParameter;

class IFiltreUpdate
{
public:
	virtual void UpdateFiltre(CEffectParameter * effectParameter) = 0;
	virtual CEffectParameter * GetParameter() = 0;
	
};