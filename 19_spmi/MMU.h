#include "generic_lib.h"
#include "interface.h"

class MMU : public sc_module, public liushuo_if<uint32_t>
{
public:
    sc_port<liushuo_if<uint32_t>, 3> init;
private:
    /* data */
public:
    SC_HAS_PROCESS(MMU);
    MMU(sc_module_name nm);
    ~MMU();
    virtual uint32_t b_read(uint32_t addr);
    virtual void b_write(uint32_t addr, uint32_t data);
};

MMU::MMU(sc_module_name nm) : sc_module(nm)
{
}

MMU::~MMU()
{
}

uint32_t MMU::b_read(uint32_t addr)
{
    if ((addr >= 0) && (addr < 0x1000)) {
        return init[0]->b_read(addr);
    } else if ((addr >= 1000) && (addr < 0x2000)) {
        return init[1]->b_read(addr - 0x1000);
    } else if ((addr >= 2000) && (addr < 0x3000)) {
        return init[2]->b_read(addr - 0x2000);
    }
    return 0;
}

void MMU::b_write(uint32_t addr, uint32_t data)
{
    if ((addr >= 0) && (addr < 0x1000)) {
        init[0]->b_write(addr, data);
    } else if ((addr >= 1000) && (addr < 0x2000)) {
        init[1]->b_write(addr - 0x1000, data);
    } else if ((addr >= 2000) && (addr < 0x3000)) {
        init[2]->b_write(addr - 0x2000, data);
    }
}