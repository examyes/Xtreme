#pragma once

#include <list>
#include <string>
#include <memory>
using std::list;
using std::string;
using std::shared_ptr;

#include <liter/utils/Singleton.h>
#include "PublicDefine.h"


namespace XASM
{

class CLabelTable : public Singleton<CLabelTable>
{
    friend class Singleton<CLabelTable>;
private:
    list<shared_ptr<SLabelNode>> m_table;

public:
    shared_ptr<SLabelNode> get_label_by_ident(string& val_ident, int func_index);
    int add(string& val_ident, int target_index, int func_index);

private:
    CLabelTable(){};
    ~CLabelTable(){};
};

}


