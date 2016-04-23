#include "TreeDataEffect.h"


CTreeDataEffect::CTreeDataEffect(void)
{
	index = -1;
}


CTreeDataEffect::~CTreeDataEffect(void)
{
}


void CTreeDataEffect::SetIndex(const int & value)
{
	index = value;
}

int CTreeDataEffect::GetIndex()
{
	return index;
}

void CTreeDataEffect::SetPosition(const int &position)
{
	this->position = position;
}

int CTreeDataEffect::GetPosition()
{
	return position;
}

void CTreeDataEffect::SetValue(const vector<int> &value)
{
	tabValue = value;
}

vector<int> CTreeDataEffect::GetValue()
{
	return tabValue;
}




