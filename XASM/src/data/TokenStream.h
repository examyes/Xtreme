#pragma once

#include <memory>
#include <vector>
#include <string>
using std::shared_ptr;
using std::vector;
using std::string;

#include "PublicDefine.h"

namespace XASM
{

namespace detail
{

class TokenStreamImp
{
public:
    using iterator = vector<shared_ptr<SToken>>::const_iterator;

private:
    vector<shared_ptr<SToken>> m_tokens;

public:
    void push_back(shared_ptr<SToken>& token_ptr);
    iterator begin() const;
    iterator end() const;
};

}

class CTokenStream
{
public:
    using iterator = detail::TokenStreamImp::iterator;

private:
    shared_ptr<detail::TokenStreamImp> m_imp;
    iterator m_curr;

public:
    CTokenStream(){
        m_imp = std::make_shared<detail::TokenStreamImp>();
        m_curr = m_imp->begin();
    };

    void push_back(SToken& token);

    iterator begin() const;
    iterator end() const;

    void reset();
    shared_ptr<SToken> next_token();
    shared_ptr<SToken> peek_next_token();
};

}
