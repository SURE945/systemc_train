#include "generic_lib.h"
#include "Master.h"
#include "Bus_Slave.h"
#include "NoC.h"
#include "Noc_Slave.h"

class Top : sc_module
{
private:
    /* data */
    Master master;
    NoC noc;
    Noc_Slave noc_slave_0;
    Noc_Slave noc_slave_1;
    Bus_Slave bus_slave_0;
    Bus_Slave bus_slave_1;
    /* bus */
    sc_signal<bool>    bus_valid;
    sc_signal<uint32_t> bus_data;
    sc_signal<uint32_t> bus_addr;
public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name nm);
    ~Top();
};

Top::Top(sc_module_name nm)
: sc_module(nm)
, master("master")
, noc("noc")
, noc_slave_0("noc_slave_0")
, noc_slave_1("noc_slave_1")
, bus_slave_0("bus_slave_0", 0x0)
, bus_slave_1("bus_slave_1", 0x10)
{
    /* NoC */
    master.skt(noc.tgt_skt);
    noc.init_skt_0(noc_slave_0.skt);
    noc.init_skt_1(noc_slave_1.skt);
    /* Bus */
    master.valid(bus_valid); bus_slave_0.valid(bus_valid); bus_slave_1.valid(bus_valid);
    master.data(bus_data); bus_slave_0.data(bus_data); bus_slave_1.data(bus_data);
    master.addr(bus_addr); bus_slave_0.addr(bus_addr); bus_slave_1.addr(bus_addr);
}

Top::~Top()
{
}
