#pragma once

class CImageLoadingFormat;
class CEffectParameter;

class IFiltreUpdate
{
public:
    virtual ~IFiltreUpdate(){};
	virtual void UpdateFiltre(CEffectParameter * effectParameter) = 0;
	virtual CEffectParameter * GetParameter() = 0;
	
};