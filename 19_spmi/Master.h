#include "generic_lib.h"
#include "interface.h"

class Master : public sc_module
{
public:
    sc_port<liushuo_if<uint32_t>> init;
private:
    /* data */
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm);
    ~Master();
    void main_thread();
};

Master::Master(sc_module_name nm) : sc_module(nm)
{
    SC_THREAD(main_thread)
}

Master::~Master()
{
}

void Master::main_thread()
{
    init->b_write(0x0000, 0x11111111);
    wait(10, SC_NS);
    init->b_write(0x1000, 0x22222222);
    wait(10, SC_NS);
    init->b_write(0x2000, 0x33333333);
    wait(10, SC_NS);
    init->b_read(0x0);
    wait(10, SC_NS);
    init->b_read(0x1000);
    wait(10, SC_NS);
    init->b_read(0x2000);
}