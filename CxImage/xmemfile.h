#if !defined(__xmemfile_h)
#define __xmemfile_h

#include "xfile.h"

//////////////////////////////////////////////////////////
class DLL_EXP CxMemFile : public CxFile
{
public:
	CxMemFile(uint8_t* pBuffer = nullptr, uint32_t size = 0);
	~CxMemFile() override;

	bool Open();
	uint8_t* GetBuffer(bool bDetachBuffer = true);

	bool Close() override;
	size_t Read(void* buffer, size_t size, size_t count) override;
	size_t Write(const void* buffer, size_t size, size_t count) override;
	bool Seek(int32_t offset, int32_t origin) override;
	int32_t Tell() override;
	int32_t Size() override;
	bool Flush() override;
	bool Eof() override;
	int32_t Error() override;
	bool PutC(uint8_t c) override;
	int32_t GetC() override;
	char* GetS(char* string, int32_t n) override;
	int32_t Scanf(const char* format, void* output) override;

protected:
	bool Alloc(uint32_t nBytes);
	void Free();

	uint8_t* m_pBuffer;
	uint32_t m_Size;
	bool m_bFreeOnClose;
	int32_t m_Position; //current position
	int32_t m_Edge; //buffer size
	bool m_bEOF;
};

#endif
