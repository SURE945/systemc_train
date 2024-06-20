#include "generic_lib.h"
#include "Interface.h"

class Slave : public sc_module
{
public:
    sc_port<Interface_rd<uint32_t>> in;
    sc_in<uint32_t> in1;
    sc_in_clk clk;
private:
    /* data */
public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name nm);
    ~Slave();
    void main_thread();
};

Slave::Slave(sc_module_name nm) : sc_module(nm)
{
    SC_THREAD(main_thread)
}

Slave::~Slave()
{
}

void Slave::main_thread()
{
    int res = 0;
    int res1 = 0;
    while (true)
    {
        wait(clk.posedge_event());
        res = in->read();
        res1 = in1.read();
        cout << sc_time_stamp() << ": " << name() << " custom read " << res << endl;
        cout << sc_time_stamp() << ": " << name() << " sc read " << res1 << endl;
    }
}