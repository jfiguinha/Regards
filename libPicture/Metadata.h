//
//  Metadata.h
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 13/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once

#define BT601 1001
#define BT709 1002
#define BT2020 1003

class CMetadata
{
public:
	int depth;
	wxString key;
	wxString value;
};
