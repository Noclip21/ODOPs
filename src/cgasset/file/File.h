#pragma once


struct File
{
	static bool verbose;

	char* data;
	int size;

	inline File();
	inline File(const File&);
	inline File(const int);
	inline File(const void*,const int);
	inline File(const char* filepath);
	virtual ~File()
	{ delete[] data; data = nullptr; }

	inline File& operator = (const File&);
	inline operator bool() const;

	inline static bool write(const char*,const void*,const int);
	inline bool write(const char*);
};
