#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <string>
#include <vector>
using namespace std;

class CPhotos
{
public:
	CPhotos();
	~CPhotos();

	void SetId(const int &numId);
	int GetId();

	void SetFolderId(const int &numId);
	int GetFolderId();

	void SetPath(const wxString &path);
	wxString GetPath();

	void SetIsCriteriaInsert(const int &criteriaInsert);
	int GetIsCriteriaInsert();

private:
	int numId;
	int numFolderId;
	wxString path;
	int criteriaInsert;
};


typedef std::vector<CPhotos> PhotosVector;
