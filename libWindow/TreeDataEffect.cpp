#include <header.h>
#include "TreeDataEffect.h"
#include "TreeElementValue.h"
using namespace Regards::Window;

CTreeDataEffect::CTreeDataEffect(void)
{
	index = -1;
	boolvalue = true;
	hasValue = false;
	position = nullptr;
}


CTreeDataEffect::~CTreeDataEffect(void)
{
	if (tabValue.size() > 0)
	{
		for (CTreeElementValue* i : tabValue)
		{
			delete i;
			i = nullptr;
		}

		tabValue.clear();
	}

	if (position != nullptr)
		delete position;
}


void CTreeDataEffect::SetIndex(const int& value)
{
	index = value;
}

bool CTreeDataEffect::HasValue()
{
	return hasValue;
}

int CTreeDataEffect::GetIndex()
{
	return index;
}

void CTreeDataEffect::SetInitValue(CTreeElementValue* position)
{
	this->position = position;
}

CTreeElementValue* CTreeDataEffect::GetInitValue()
{
	return position;
}

void CTreeDataEffect::SetValue(void* value, const int& typeValue)
{
	if (typeValue == 1)
	{
		vector<int> vecInt;
		auto v = static_cast<vector<int>*>(value);
		copy(v->begin(), v->end(), back_inserter(vecInt));

		for (auto i : vecInt)
		{
			tabValue.push_back(new CTreeElementValueInt(i));
		}
	}
	else if (typeValue == 2)
	{
		auto b = static_cast<bool*>(value);
		boolvalue = *b;
	}
	else if (typeValue == 3)
	{
		auto v = static_cast<vector<CMetadata>*>(value);
		copy(v->begin(), v->end(), back_inserter(tabMetadata));
		auto indexPos = static_cast<CTreeElementValueInt*>(this->position);
		index = indexPos->GetValue();
	}
	if (typeValue == 4)
	{
		vector<float> vecFloat;
		auto v = static_cast<vector<float>*>(value);
		copy(v->begin(), v->end(), back_inserter(vecFloat));

		for (float i : vecFloat)
		{
			tabValue.push_back(new CTreeElementValueFloat(i));
		}
	}

	hasValue = true;
}

vector<CTreeElementValue*>* CTreeDataEffect::GetVectorValue()
{
	return &tabValue;
}

vector<CMetadata> CTreeDataEffect::GetMetadataValue()
{
	return tabMetadata;
}


bool CTreeDataEffect::GetBoolValue()
{
	return boolvalue;
}
