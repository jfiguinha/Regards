#pragma once
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

class CInsertCriteria
{
public:

	CInsertCriteria(){};
	~CInsertCriteria(){};

	CInsertCriteria& operator=(const CInsertCriteria& other) {
		value = other.value;
		type = other.type;
		id = other.id;
		isNew = other.isNew;
		return *this;
	}

	wxString value = "";
	int type = 0;
	int64_t id = 0;
	bool isNew = false;
};

class CListCriteriaPhoto
{
public:

	CListCriteriaPhoto(){};
	~CListCriteriaPhoto(){};
	
	wxString photoPath;
	int numPhotoId;
	int numCatalog;
	vector<CInsertCriteria *> listCriteria;
};
