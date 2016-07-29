#pragma once

#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;


namespace XASM
{

class CSourceCodeLine
{
private:
		string m_text;
		size_t m_row;

public:
		CSourceCodeLine(string& line, size_t row)
			: m_text(line)
			, m_row(row)
		{};
		~CSourceCodeLine(){};

		string text() { return m_text; };
		size_t row() { return m_row; };
};

namespace detail
{

class HolderImp
{
public:
    HolderImp(const string& file_name) 
            : m_file_name(file_name)
    {};
    ~HolderImp(void) {};
    using iterator = vector<shared_ptr<CSourceCodeLine>>::iterator;

public:

    void append(shared_ptr<CSourceCodeLine>& line_ptr);
    void append(const string& line, size_t row);

    iterator begin();
    iterator end();

    size_t row_count();
    shared_ptr<CSourceCodeLine> at(size_t row);
    shared_ptr<CSourceCodeLine> operator [](size_t row);

    string file_name() const;

private:
    vector<shared_ptr<CSourceCodeLine>> m_code;
    string m_file_name;
};

}

class CSourceCodeHolder
{
public:
		CSourceCodeHolder(const string& file_name = "") { 
			m_holder = std::make_shared<detail::HolderImp>(file_name); 
		};
		~CSourceCodeHolder(void) {};
		using iterator = detail::HolderImp::iterator;

public:
		void append(shared_ptr<CSourceCodeLine>& line_ptr);
		void append(const string& line, size_t row);

		iterator begin();
		iterator end();

		size_t row_count();
		shared_ptr<CSourceCodeLine> at(size_t row);
		shared_ptr<CSourceCodeLine> operator [](size_t row);

		string file_name() const;

private:
		shared_ptr<detail::HolderImp> m_holder;
};

}


