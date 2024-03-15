#pragma once


namespace vole {

class StringConversion {
public:
	// FIXME no error checking
	static double toDbl(std::string str);
	static float toFlt(std::string str);
	static int toInt(std::string str);
	static unsigned int toUInt(std::string str);
	static std::vector<std::string> stringToList(std::string str, char split_character);
	static std::vector<int> stringToIntList(std::string str, char split_character);

};

}

