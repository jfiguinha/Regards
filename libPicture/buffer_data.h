#pragma once

typedef struct buffer_data {
	uint8_t *buf = nullptr;
	int size;
	uint8_t *ptr = nullptr;
	size_t room; ///< size left in the buffer

	void FreeData()
	{
		if (buf != nullptr)
			free(buf);

		buf = nullptr;
		ptr = nullptr;
	}
} buffer_data;