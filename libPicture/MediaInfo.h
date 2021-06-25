/*
 *  Regards_libMediaInfo.h
 *  Regards.libMediaInfo
 *
 *  Created by figuinha jacques on 18/08/2015.
 *  Copyright (c) 2015 figuinha jacques. All rights reserved.
 *
 */

#pragma once
#include <Metadata.h>

class CMediaInfo
{
public:
	static vector<CMetadata> ReadMetadata(const wxString& filename);
	static int GetVideoRotation(const wxString& filename);
private:
	static vector<CMetadata> SplitByLine(const wstring& value);
	static std::wstring delUnnecessary(std::wstring& str);
};
