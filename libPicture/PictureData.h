#pragma once

class CRegardsBitmap;

class CPictureData
{
public:
	CPictureData(){
		width = 0;
		height = 0;
		size = 0;
	};
	~CPictureData();

	static CPictureData * LoadPictureToJpeg(const wxString &filename, bool &pictureOK, const int &resizeWidth = 0, const int &resizeHeight = 0);
	static float CalculPictureRatio(const int &pictureWidth, const int &pictureHeight, const int &widthOutput, const int &heightOutput);

	int GetWidth();
	void SetWidth(const int &width);
	int GetHeight();
	void SetHeight(const int &height);
	wxString GetFilename();
	void SetFilename(const wxString &filename);
	void SetData(uint8_t * & extdata, const int &size);
	void CopyData(CRegardsBitmap * & bitmap);
	uint8_t * GetData();
	int GetSize();
	//std::vector<char> CopyData();
private:


	int width;
	int height;
	wxString filename;
	uint8_t * data = nullptr;
	int	size;

};


