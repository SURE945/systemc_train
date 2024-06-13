#include "generic_lib.h"
#include "interface.h"
#include "payload.h"

class Master : public sc_module
{
public:
    sc_port<liushuo_if<payload>> init;
private:
    /* data */
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm);
    ~Master();
    void main_thread();
    void write_string(uint32_t addr, const char* c_str, uint32_t len);
    void read_string(uint32_t addr, uint32_t len);
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
    write_string(0x0, "hello tlm", 9);
    wait(10, SC_NS);
    read_string(0x0, 9);
}

void Master::write_string(uint32_t addr, const char* c_str, uint32_t len)
{
    payload trans;
    trans.set_addr(addr);
    trans.set_len(len);
    trans.set_cmd(liushuo_cmd::WRITE);
    char* str = const_cast<char *>(c_str);
    trans.set_dataptr(str);
    int res = init->b_transport(&trans);
}

void Master::read_string(uint32_t addr, uint32_t len)
{
    payload trans;
    trans.set_addr(addr);
    trans.set_len(len);
    trans.set_cmd(liushuo_cmd::READ);
    int res = init->b_transport(&trans);
    char* ptr = trans.get_dataptr();
}