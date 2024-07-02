#include "generic_lib.h"
#include "Master.h"
#include "Slave.h"

class Top : public sc_module
{
private:
    /* data */
    Master master;
    Slave slave;
public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name nm);
    ~Top();
};

Top::Top(sc_module_name nm) : sc_module(nm)
, master("master")
, slave("slave")
{
    master.init_skt.bind(slave.tgt_skt);
}

Top::~Top()
{
}
