#include "generic_lib.h"
#include "tlm_utils/simple_initiator_socket.h"

class Master : public sc_module
{
public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
private:
    void write32(uint32_t addr, uint32_t data);
    uint32_t read32(uint32_t addr);
    void main_thread();
    /* data */
    int m_idx;
public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name nm, int idx);
    ~Master();
};

void Master::write32(uint32_t addr, uint32_t data)
{
    tlm_generic_payload trans;
    trans.set_address(addr);
    trans.set_command(TLM_WRITE_COMMAND);
    unsigned char data_ptr[4];
    memcpy(data_ptr, &data, 4);
    trans.set_data_ptr(data_ptr);
    trans.set_data_length(m_idx);
    sc_time delay = SC_ZERO_TIME;
    init_skt->b_transport(trans, delay);
}

uint32_t Master::read32(uint32_t addr)
{
    tlm_generic_payload trans;
    trans.set_address(addr);
    trans.set_command(TLM_READ_COMMAND);
    trans.set_data_length(m_idx);
    sc_time delay = SC_ZERO_TIME;
    init_skt->b_transport(trans, delay);
    unsigned char* data_ptr = trans.get_data_ptr();
    uint32_t data = 0;
    memcpy(&data, data_ptr, 4);
    return data;
}

Master::Master(sc_module_name nm, int idx) : sc_module(nm)
, m_idx(idx)
, init_skt("init_skt")
{
    SC_THREAD(main_thread);
}

Master::~Master()
{
}

void Master::main_thread()
{
    wait(10, SC_NS);
    write32(0x0, 0x100);
}