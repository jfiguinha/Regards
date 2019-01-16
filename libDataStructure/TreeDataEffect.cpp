#include "TreeDataEffect.h"

CTreeDataEffect::CTreeDataEffect(void)
{
	index = -1;
	boolvalue = true;
	hasValue = false;
	position = nullptr;
}


CTreeDataEffect::~CTreeDataEffect(void)
{
	if(tabValue.size() > 0)
	{
		for (CTreeElementValue * i : tabValue)
		{
			delete i;
			i = nullptr;
		}

		tabValue.clear();
	}

	if(position != nullptr)
		delete position;
}


void CTreeDataEffect::SetIndex(const int & value)
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

void CTreeDataEffect::SetInitValue(CTreeElementValue * position)
{
	this->position = position;
}

CTreeElementValue * CTreeDataEffect::GetInitValue()
{
	return position;
}

void CTreeDataEffect::SetValue(void * value,const int & typeValue)
{
	if(typeValue == 1)
	{
		vector<int> vecInt;
		std::vector<int> * v = (vector<int>*)value;
		copy(v->begin(), v->end(), back_inserter(vecInt));

        for (auto i : vecInt)
		{
			tabValue.push_back(new CTreeElementValueInt(i));
		}
        
	}
	else if(typeValue == 2)
	{
		bool * b = (bool *)value;
		boolvalue = *b;
	}
	else if(typeValue == 3)
	{
		std::vector<CMetadata> * v = (vector<CMetadata>*)value;
		copy(v->begin(), v->end(), back_inserter(tabMetadata));	
		CTreeElementValueInt * indexPos = (CTreeElementValueInt *)this->position;
		index = indexPos->GetValue();
	}
	if(typeValue == 4)
	{
		vector<float> vecFloat;
		std::vector<float> * v = (vector<float>*)value;
		copy(v->begin(), v->end(), back_inserter(vecFloat));

        for (float i : vecFloat)
		{
			tabValue.push_back(new CTreeElementValueFloat(i));
		}
        
	}

	hasValue = true;
}

vector<CTreeElementValue *> * CTreeDataEffect::GetVectorValue()
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




