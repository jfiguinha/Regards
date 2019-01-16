#pragma once
#include <vector>
#include <string>
using namespace std;


class CMetadata;
class wxString;

class CRawMetadata
{
public:
	CRawMetadata(){};
	~CRawMetadata(){};
    static vector<CMetadata> GetMetadata(const string & fileName);

};
