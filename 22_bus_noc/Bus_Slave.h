#include "generic_lib.h"

class Bus_Slave : public sc_module
{
public:
    /* interface */
    sc_in<bool> valid;
    sc_in<uint32_t> addr;
    sc_in<uint32_t> data;
private:
    /* data */
    uint32_t m_addr;
public:
    SC_HAS_PROCESS(Bus_Slave);
    Bus_Slave(sc_module_name nm, uint32_t address);
    ~Bus_Slave();
    void main_method();
};

Bus_Slave::Bus_Slave(sc_module_name nm, uint32_t address)
: sc_module(nm)
, m_addr(address)
{
    SC_METHOD(main_method);
    sensitive << valid.pos();
    dont_initialize();
}

void Bus_Slave::main_method()
{
    uint32_t in_addr = addr.read();
    if (in_addr == m_addr) {
        uint32_t in_data = data.read();
        cout << name() << "@" << sc_time_stamp() << ": bus receive 0x" << hex << in_data << endl;
    }
}

Bus_Slave::~Bus_Slave()
{
}
