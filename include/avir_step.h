#pragma once


class CAvirStep
{
public:
	CAvirStep() {};
	~CAvirStep() {
		if (!picture.empty())
			picture.release();
	};

	virtual int GetType() { return 0; };

	int width = 0;
	int height = 0;
	UMat picture;
};



class CAvirFilterParam
{
public:

	CAvirFilterParam() {};
	~CAvirFilterParam() {
		for (CAvirStep* step : stepH)
		{
			delete step;
		}

		for (CAvirStep* step : stepV)
		{
			delete step;
		}

		if (!srcV.empty())
			srcV.release();

		if (!dest.empty())
			dest.release();
	};

	vector<CAvirStep*> stepH;
	vector<CAvirStep*> stepV;
	float gm;
	double PkOut;
	double TrMul;

	int width;
	int height;
	int widthOut;
	int heightOut;

	cv::UMat srcV;
	cv::UMat dest;
};

class CAvirStepDoFilter : public CAvirStep
{
public:
	CAvirStepDoFilter() {};
	~CAvirStepDoFilter() {	
		if (f != nullptr)
		{
			delete[] f;
			f = nullptr;
		}
	};

	int GetType() { return 1; };

	int type = 1;
	float * f = nullptr;
	int flen = 0;
	int step = 0;
};


class CAvirStepResize : public CAvirStep
{
public:
	CAvirStepResize() {};
	~CAvirStepResize() 
	{
		if (PositionTab != nullptr)
		{
			delete[] PositionTab;
			PositionTab = nullptr;
		}
		if (ftp != nullptr)
		{
			delete[] ftp;
			ftp = nullptr;
		}
	};

	int GetType() { return 2; };
	int* PositionTab = nullptr;
	int posTabSize = 0;
	float* ftp = nullptr;
	int ftpTabSize = 0;
	int IntFltLen = 0;
};

class CAvirStepResize2 : public CAvirStep
{
public:
	CAvirStepResize2() {};
	~CAvirStepResize2()
	{
		if (PositionTab != nullptr)
		{
			delete[] PositionTab;
			PositionTab = nullptr;
		}
		if (ftp != nullptr)
		{
			delete[] ftp;
			ftp = nullptr;
		}
	};

	int GetType() { return 3; };
	int* PositionTab = nullptr;
	int posTabSize = 0;
	float* ftp = nullptr;
	int ftpTabSize = 0;
	int IntFltLen = 0;
};


class CAvirStepUpSample : public CAvirStep
{
public:
	CAvirStepUpSample() {};
	~CAvirStepUpSample()
	{
	}

	int GetType() { return 4; };
	int widthSrc = 0;
	int start = 0;
	int outLen = 0;
	int ResampleFactor = 0;
	int OutPrefix = 0;
	int OutLen = 0;
	int OutSuffix = 0;
};