#pragma once
#include "TreeData.h"
#include <string>
using namespace std;

class CTreeDataEffect : public CTreeData
{
public:
	CTreeDataEffect(void);
	~CTreeDataEffect(void);

	void SetIndex(const int & value);
	int GetIndex();

	void SetPosition(const int &position);
	int GetPosition();
	
	void SetValue(const vector<int> &value);
	vector<int> GetValue();

protected:

	int index;
	vector<int> tabValue;
	int position;

};

