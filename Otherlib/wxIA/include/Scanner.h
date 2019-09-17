#pragma once

class Scanner {
public:
	enum class State { IDLE = 0, OPEN, SET_OPTIONS, START, GET_PARAMETERS, READ };
	enum class ScanMode { DEFAULT = 0, COLOR, GRAY, LINEART };
	enum class ScanType { SINGLE = 0, ADF_FRONT, ADF_BACK, ADF_BOTH };

	struct Device {
		std::string name;
		std::string label;
	};

	struct Params {
		std::string device;
		std::string filename;
		double dpi;
		ScanMode scan_mode;
		int depth;
		ScanType type;
		int page_width;
		int page_height;
	};
};