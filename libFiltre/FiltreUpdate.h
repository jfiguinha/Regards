#pragma once

class IFiltreUpdate
{
public:
	virtual void UpdateFiltre(CEffectParameter * effectParameter) = 0;
	virtual CEffectParameter * GetParameter() = 0;
};