#ifndef __TLM_MASTER__
#define __TLM_MASTER__

#include "systemc"
#include "tlm"
#include "payload.h"
#include "interface.h"

using namespace std;
using namespace tlm;
using namespace sc_dt;
using namespace sc_core;

class master : public sc_module
{
public:
    sc_port<liushuo_if<payload>> interface;
private:
    /* data */
public:
    SC_HAS_PROCESS(master);
    master(/* args */sc_module_name nm);
    ~master();
private:
    void main_thread();
    void write_string(uint32_t addr, const char* c_str, uint32_t len);
    void read_string(uint32_t addr, uint32_t len);
};

master::master(/* args */sc_module_name nm)
: sc_module(nm)
{
    SC_THREAD(main_thread);
}

master::~master()
{
}

void master::main_thread()
{
    write_string(0x0, "hello tlm", 9);
    wait(10, SC_NS);
    read_string(0x0, 9);
}

void master::write_string(uint32_t addr, const char* c_str, uint32_t len)
{
    payload trans;
    trans.set_addr(addr);
    trans.set_len(len);
    char* str = const_cast<char *>(c_str);
    trans.set_dataptr(str);
    int res = interface->b_write(&trans);
}

void master::read_string(uint32_t addr, uint32_t len)
{
    payload trans;
    trans.set_addr(addr);
    trans.set_len(len);
    int res = interface->b_read(&trans);
    char* ptr = trans.get_dataptr();
}

#endif