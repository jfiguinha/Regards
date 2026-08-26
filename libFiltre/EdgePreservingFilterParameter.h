#pragma once
#include "EffectParameter.h"

class CEdgePreservingFilterParameter : public CEffectParameter
{
public:
	CEdgePreservingFilterParameter()
	{
		flags = 1; //RECURS_FILTER 
		sigmaX = 60;
		sigmaP = 40;
	};

	int flags;
	int sigmaX;
	int sigmaP;
};

