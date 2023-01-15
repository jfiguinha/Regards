//
//  FiltreEffectInterface.h
//  Regards.libViewer
//
//  Created by figuinha jacques on 12/04/2016.
//  Copyright © 2016 figuinha jacques. All rights reserved.
//

#pragma once
namespace Regards::Window
{
	class CTreeElementValue;
}

using namespace Regards::Window;

class IFiltreEffectInterface
{
public:
	virtual ~IFiltreEffectInterface()
	{
	};
	virtual void AddTreeInfos(const wxString& exifKey, CTreeElementValue* position, void* value,
	                          const int& typeValue = 1, const int& type = 1) = 0;
};
