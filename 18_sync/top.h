#include "generic_lib.h"
#include "Master.h"
#include "Slave.h"

class Top : public sc_module
{
private:
    /* data */
    Master master0;
    Master master1;
    Master master2;
    Slave slave;
public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name nm);
    ~Top();
};

Top::Top(sc_module_name nm) : sc_module(nm)
, master0("master0", 0)
, master1("master1", 1)
, master2("master2", 2)
, slave("slave")
{
    master0.init_skt(slave.tgt_skt);
    master1.init_skt(slave.tgt_skt_1);
    master2.init_skt(slave.tgt_skt_2);
}

Top::~Top()
{
}
