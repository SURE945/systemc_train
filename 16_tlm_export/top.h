#include "generic_lib.h"
#include "Slave.h"
#include "Master.h"

class Top : public sc_module
{
private:
    /* data */
    Master master;
    Slave slave;
public:
    SC_HAS_PROCESS(Top);
    Top(/* args */sc_module_name nm);
    ~Top();
};

Top::Top(/* args */sc_module_name nm)
: sc_module(nm)
, master("master")
, slave("slave")
{
    master.init(slave.tgt);
}

Top::~Top()
{
}