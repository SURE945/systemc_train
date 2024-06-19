#include "generic_lib.h"
#include "interface.h"

class Memory : public sc_module, public liushuo_if<uint32_t>
{
private:
    /* data */
    uint32_t mem[0x1000];
public:
    SC_HAS_PROCESS(Memory);
    Memory(sc_module_name nm);
    ~Memory();
    virtual uint32_t b_read(uint32_t addr);
    virtual void b_write(uint32_t addr, uint32_t data);
};

Memory::Memory(sc_module_name nm) : sc_module(nm)
{
    for (int i = 0; i < 0x1000; i++) mem[i] = 0;
}

Memory::~Memory()
{
}

uint32_t Memory::b_read(uint32_t addr)
{
    cout << sc_time_stamp() << ": read from " << name() << " 0x" << hex << mem[addr] << " at 0x" << hex << addr << endl;
    return mem[addr];
}

void Memory::b_write(uint32_t addr, uint32_t data)
{
    cout << sc_time_stamp() << ": write to " << name() << " 0x" << hex << data << " at 0x" << hex << addr << endl;
    mem[addr] = data;
}
