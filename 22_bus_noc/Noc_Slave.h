#include "generic_lib.h"
#include "tlm_utils/simple_target_socket.h"

class Noc_Slave : public sc_module
{
public:
    /* interface */
    tlm_utils::simple_target_socket<Noc_Slave, 32, tlm::tlm_base_protocol_types> skt;
private:
    /* data */
public:
    SC_HAS_PROCESS(Noc_Slave);
    Noc_Slave(sc_module_name nm);
    ~Noc_Slave();
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
};

Noc_Slave::Noc_Slave(sc_module_name nm) : sc_module(nm), skt("skt")
{
    skt.register_b_transport(this, &Noc_Slave::b_transport);
}

void Noc_Slave::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    uint8_t* u8_data = trans.get_data_ptr();
    uint32_t u32_data = 0;
    memcpy(&u32_data, u8_data, 4);
    cout << name() << "@" << sc_time_stamp() << ": noc receive 0x" << hex << u32_data << endl;
    trans.set_response_status(TLM_OK_RESPONSE);
}

Noc_Slave::~Noc_Slave()
{
}
