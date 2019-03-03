#include "EffectParameter.h"
#pragma once

class CKuwaharaEffectParameter : public CEffectParameter
{
public:
	CKuwaharaEffectParameter()
	{
		fSize = 11;
	};
	~CKuwaharaEffectParameter(){};

	int fSize;
};

