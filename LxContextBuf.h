#include <list>
#include <vector>

#ifndef __LEXI_CONTEXT_BUF_H
#define __LEXI_CONTEXT_BUF_H
#define TRIVAL_STR_BUF_CAPACITY		50

#include "LxCommon.h"

class ContextBuf
{
public:
	ContextBuf();
	virtual ~ContextBuf();
public:
	virtual void store_stream(FILE* file) =0;
};

class TrivalStrBuf;

class RandomAccessStrBuf
{
public:
	RandomAccessStrBuf();
	~RandomAccessStrBuf();
public:
	void store_stream(FILE* file);
public:
	void Insert(size_t position, TCHAR ch);
	void Insert(size_t position, TCHAR* str, size_t count);
	void Delete(size_t section_begin, size_t section_end);
private:
	typedef std::list<TrivalStrBuf*>::iterator buf_it;
	std::list<TrivalStrBuf*> str_buffer_list;
};

class GeneralStrBuf
{
public:
	GeneralStrBuf() {}
	~GeneralStrBuf() {}
public:
	inline void store_stream(FILE* file)
	{
		store_stream_int(file, str_buf.size());
		if (!str_buf.empty())
			fwrite(&str_buf[0], 1, sizeof(TCHAR)*str_buf.size(), file);
	}
	inline void build_from_stream(FILE* file)
	{
		int str_size = read_stream_int(file);
		if (str_size == 0)
			return;
		str_buf.resize(str_size);
		fread(&str_buf[0], 1, sizeof(TCHAR)*str_size, file);
	}
public:
	const TCHAR* get_context_ptr() const { return &str_buf[0]; }
	inline TCHAR Get(int index) { return str_buf[index]; }
	inline size_t size() { return str_buf.size(); }
	inline bool empty() { return str_buf.empty(); }
	inline void Insert(size_t position, TCHAR ch)
	{
		str_buf.insert(str_buf.begin()+position,ch);
	}
	inline void Insert(size_t position, const TCHAR* str, size_t count)
	{
		str_buf.insert(str_buf.begin()+position,str,str+count);
	}
	inline void Delete(size_t position)
	{
		str_buf.erase(str_buf.begin()+position);
	}
	inline void Delete(size_t section_begin, size_t section_end)
	{
		str_buf.erase(str_buf.begin()+section_begin,str_buf.begin()+section_end+1);
	}
private:
	std::vector<TCHAR> str_buf;
};

class TrivalStrBuf
{
public:
	TrivalStrBuf();
	~TrivalStrBuf();
public:
	void store_stream(FILE* file);
public:
	inline size_t capacity() { return TRIVAL_STR_BUF_CAPACITY; }
	inline size_t remaining_capacity() { return TRIVAL_STR_BUF_CAPACITY-size_; }
	inline size_t size() { return size_; }
public:
	size_t Insert(size_t position, TCHAR ch);
	size_t Insert(size_t position, TCHAR* str, size_t count);
	size_t Delete(size_t section_begin, size_t section_end);
private:
	TCHAR str_buf[TRIVAL_STR_BUF_CAPACITY];
	size_t size_;
};

#endif