#pragma once
#include "TreeData.h"
#include "Metadata.h"
#include "TreeElementValue.h"


class CTreeDataEffect : public CTreeData
{
public:
	CTreeDataEffect(void);
	~CTreeDataEffect(void);

	void SetIndex(const int & value);
	int GetIndex();

	void SetInitValue(CTreeElementValue * position);
	CTreeElementValue * GetInitValue();

	void SetValue(void * value,const int & typeValue);
	vector<CTreeElementValue *> * GetVectorValue();
	vector<CMetadata> GetMetadataValue();
	bool GetBoolValue();

	bool HasValue();

protected:

	int index;
	vector<CTreeElementValue *> tabValue;
	vector<CMetadata> tabMetadata;
	bool boolvalue;
	CTreeElementValue * position;
	bool hasValue;

};

