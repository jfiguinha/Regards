#if !defined(__xiofile_h)
#define __xiofile_h

#include "xfile.h"
//#include <char.h>

class DLL_EXP CxIOFile : public CxFile
{
public:
	CxIOFile(FILE* fp = nullptr)
	{
		m_fp = fp;
		m_bCloseFile = fp == nullptr;
	}

	~CxIOFile() override
	{
		Close();
	}

	//////////////////////////////////////////////////////////
	bool Open(const char* filename, const char* mode)
	{
		if (m_fp) return false; // Can't re-open without closing first

		m_fp = fopen(filename, mode);
		if (!m_fp) return false;

		m_bCloseFile = true;

		return true;
	}

	//////////////////////////////////////////////////////////
	bool Close() override
	{
		int32_t iErr = 0;
		if ((m_fp) && (m_bCloseFile))
		{
			iErr = fclose(m_fp);
			m_fp = nullptr;
		}
		return iErr == 0;
	}

	//////////////////////////////////////////////////////////
	size_t Read(void* buffer, size_t size, size_t count) override
	{
		if (!m_fp) return 0;
		return fread(buffer, size, count, m_fp);
	}

	//////////////////////////////////////////////////////////
	size_t Write(const void* buffer, size_t size, size_t count) override
	{
		if (!m_fp) return 0;
		return fwrite(buffer, size, count, m_fp);
	}

	//////////////////////////////////////////////////////////
	bool Seek(int32_t offset, int32_t origin) override
	{
		if (!m_fp) return false;
		return fseek(m_fp, offset, origin) == 0;
	}

	//////////////////////////////////////////////////////////
	int32_t Tell() override
	{
		if (!m_fp) return 0;
		return ftell(m_fp);
	}

	//////////////////////////////////////////////////////////
	int32_t Size() override
	{
		if (!m_fp) return -1;
		int32_t pos, size;
		pos = ftell(m_fp);
		fseek(m_fp, 0, SEEK_END);
		size = ftell(m_fp);
		fseek(m_fp, pos,SEEK_SET);
		return size;
	}

	//////////////////////////////////////////////////////////
	bool Flush() override
	{
		if (!m_fp) return false;
		return fflush(m_fp) == 0;
	}

	//////////////////////////////////////////////////////////
	bool Eof() override
	{
		if (!m_fp) return true;
		return feof(m_fp) != 0;
	}

	//////////////////////////////////////////////////////////
	int32_t Error() override
	{
		if (!m_fp) return -1;
		return ferror(m_fp);
	}

	//////////////////////////////////////////////////////////
	bool PutC(uint8_t c) override
	{
		if (!m_fp) return false;
		return fputc(c, m_fp) == c;
	}

	//////////////////////////////////////////////////////////
	int32_t GetC() override
	{
		if (!m_fp) return EOF;
		return getc(m_fp);
	}

	//////////////////////////////////////////////////////////
	char* GetS(char* string, int32_t n) override
	{
		if (!m_fp) return nullptr;
		return fgets(string, n, m_fp);
	}

	//////////////////////////////////////////////////////////
	int32_t Scanf(const char* format, void* output) override
	{
		if (!m_fp) return EOF;
		return fscanf(m_fp, format, output);
	}

	//////////////////////////////////////////////////////////
protected:
	FILE* m_fp;
	bool m_bCloseFile;
};

#endif
