#ifndef __TLM_MEMORY__
#define __TLM_MEMORY__

#include "systemc"
#include "tlm"
#include "interface.h"
#include "payload.h"

using namespace std;
using namespace tlm;
using namespace sc_dt;
using namespace sc_core;

#define MEM_SIZE 1024

class memory : public sc_module, public liushuo_if<payload>
{
private:
    /* data */
    uint8_t mem[MEM_SIZE];
public:
    SC_HAS_PROCESS(memory);
    memory(/* args */sc_module_name nm);
    ~memory();
    void reset();
    transfer_status b_write(payload* payload);
    transfer_status b_read(payload* payload);
};

memory::memory(/* args */sc_module_name nm)
: sc_module(nm)
{
    reset();
}

memory::~memory()
{
}

void memory::reset()
{
    for (int i = 0; i < MEM_SIZE; i++) mem[i] = 0;
}

transfer_status memory::b_write(payload* payload)
{
    cout << sc_time_stamp() << ": call b_write" << endl;
    return OK;
}

transfer_status memory::b_read(payload* payload)
{
    cout << sc_time_stamp() << ": call b_read" << endl;
    return OK;
}

#endif