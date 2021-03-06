#include <list>
#include <iostream>
#include "LxCommon.h"
#include "LxContextBuf.h"

#ifndef __LEXI_DOCUMENT_H
#define __LEXI_DOCUMENT_H

class LxComposeAlgom;
class Paragraph;
typedef std::list<Paragraph*>::iterator contex_pgh_iter;

class Document
{
public:
	Document();
	virtual ~Document();
	void clear();
	contex_pgh_iter begin() { return paragraph_list.begin(); }
	contex_pgh_iter end() { return paragraph_list.end(); }
	size_t get_offset_inner(size_t index_global, size_t pgh_index);
	Paragraph* get_pgh(int index);
	size_t pgh_size() { return paragraph_list.size(); }
	size_t size();
public:
	void store_stream(FILE* file);
	void build_from_stream(FILE* file);
	void add_paragraph(Paragraph* paragraph);
	void insert_paragraph(Paragraph* paragraph,int index);
	void insert_paragraph(Paragraph* paragraph, contex_pgh_iter pos);
	void remove_paragraph(int index);
	void remove_paragraph(contex_pgh_iter pgh_it);
	//to delete paragraphs in [index_b,index_e];
	void remove_paragraphs(size_t index_b, size_t index_e);
public:
	void insert(size_t pos, TCHAR* cs, size_t len);		//insert�в��������з�
private:
	std::list<Paragraph*> paragraph_list;
};

class Paragraph
{
public:
	Paragraph() : compose_algom_type_(ComposeAlgoType::COMPOSE_ALGO_SIMPLE) {}
	virtual ~Paragraph() {}
	inline void SetComposeAlgom(ComposeAlgoType compose_algom_type)
	{
		this->compose_algom_type_ = compose_algom_type;
	}
	LxComposeAlgom* GetComposeAlgom();
public:
	inline void store_stream(FILE* file)
	{
		store_stream_int(file, compose_algom_type_);
		context.store_stream(file);
	}
	inline void build_from_stream(FILE* file)
	{
		this->SetComposeAlgom((ComposeAlgoType)read_stream_int(file));
		context.build_from_stream(file);
	}
public:
	const TCHAR* get_context_ptr() const { return context.get_context_ptr(); }
	inline TCHAR Get(int index) { return context.Get(index); }
	inline size_t size() { return context.size(); }
	inline bool empty() { return context.empty(); }
	inline void Insert(size_t position, TCHAR ch)
	{
		context.Insert(position,ch);
	}
	inline void Insert(size_t position, const TCHAR* str, size_t count)
	{
		context.Insert(position,str,count);
	}
	inline void Delete(size_t position)
	{
		context.Delete(position);
	}
	inline void Delete(size_t section_begin, size_t section_end)
	{
		context.Delete(section_begin,section_end);
	}
private:
	//how to store the paragraph context string
	//RandomAccessStrBuf context;
	GeneralStrBuf context;
	ComposeAlgoType compose_algom_type_;
};

#endif