#include "generic_lib.h"
#include "Master.h"
#include "Slave.h"
#include "Signal.h"

class Top : public sc_module
{
private:
    /* data */
    Master master;
    Slave  slave;
    Signal<uint32_t> signal;
    sc_signal<uint32_t> signal1;
    sc_clock clk;
public:
    SC_HAS_PROCESS(Top);
    Top(sc_module_name nm);
    ~Top();
    void main_thread();
};

Top::Top(sc_module_name nm) : sc_module(nm)
, master("master")
, slave("slave")
, clk("clk", 10, SC_NS, 0.5)
{
    master.out(signal);
    slave.in(signal);
    master.clk(clk);
    slave.clk(clk);
    master.out1(signal1);
    slave.in1(signal1);
    SC_THREAD(main_thread);
}

Top::~Top()
{
}

void Top::main_thread()
{
    wait(100, SC_NS);
    sc_stop();
}