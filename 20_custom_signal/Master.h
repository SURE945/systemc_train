#include "generic_lib.h"
#include "Interface.h"

class Master : public sc_module
{
public:
    sc_port<Interface_wr<uint32_t>> out;
    sc_out<uint32_t> out1;
    sc_in_clk clk;
private:
    /* data */
    uint32_t _idx = 0;
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm);
    ~Master();
    void main_thread();
};

Master::Master(sc_module_name nm) : sc_module(nm)
{
    SC_THREAD(main_thread);
}

Master::~Master()
{
}

void Master::main_thread()
{
    while (true)
    {
        wait(clk.posedge_event());
        out->write(_idx);
        out1.write(_idx);
        cout << sc_time_stamp() << ": " << name() << "  custom write " << _idx << endl;
        cout << sc_time_stamp() << ": " << name() << "  sc write " << _idx << endl;
        _idx++;
    }
}