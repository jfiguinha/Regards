//
//  FilterData.hpp
//  Regards.libFiltreInterface
//
//  Created by figuinha jacques on 07/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
#include <FilterWindowParam.h>
#include <unordered_map>
#include <memory>
#include <functional>

class CFiltreEffet;
class CEffectParameter;


class CFiltreData
{
public:
	CFiltreData() {};
	~CFiltreData() {};

	static void CreateFilterList();
	static std::unique_ptr<CFilterWindowParam> CreateEffectPointer(const int& numFilter);
	static int GetTypeEffect(const int& numFilter);
	static bool SupportMouseClick(const int& numFilter);
	static void SetCursor(const int& numFilter);
	static bool SupportMouseSelection(const int& numFilter);
	static bool NeedPreview(const int& numFilter);
	static bool IsOpenCLCompatible(const int& numFilter);
	static int TypeApplyFilter(const int& numFiltre);
	static CDraw* GetDrawingPt(const int& numFilter);
	static int RenderEffect(const int& numEffect, CFiltreEffet* filtreEffet, CEffectParameter* effectParameter,
		const bool& preview = false);
	static wxString GetFilterLabel(const int& numFilter);
	static CEffectParameter* GetDefaultEffectParameter(const int& numFilter);
	static CEffectParameter* GetEffectParameter(const int& numFilter);

private:
	// Retourne le filtre correspondant à numFilter, ou nullptr si absent.
	static CFilterWindowParam* GetFilter(const int& numFilter);

	using FilterFactory = std::function<std::unique_ptr<CFilterWindowParam>()>;

	// Table de fabrication : associe un identifiant de filtre à sa factory.
	static const std::unordered_map<int, FilterFactory>& GetFactoryMap();

	static std::unordered_map<int, std::unique_ptr<CFilterWindowParam>> filterList;
};