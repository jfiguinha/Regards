#pragma once

class CInsertCriteria
{
public:
	CInsertCriteria()
	{
		value = "";
		type = 0;
		id = 0;
		isNew = false;
	};

	~CInsertCriteria()
	{
	};

	CInsertCriteria& operator=(const CInsertCriteria& other)
	{
		value = other.value;
		type = other.type;
		id = other.id;
		isNew = other.isNew;
		return *this;
	}

	wxString value;
	int type;
	int64_t id;
	bool isNew;
};

class CListCriteriaPhoto
{
public:
	CListCriteriaPhoto(): numPhotoId(0), numCatalog(0)
	{
	}
	;

	~CListCriteriaPhoto()
	{
	};

	wxString photoPath;
	int numPhotoId;
	int numCatalog;
	vector<CInsertCriteria*> listCriteria;
};
