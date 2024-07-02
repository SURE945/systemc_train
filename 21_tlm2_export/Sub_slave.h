#include "generic_lib.h"
#include "tlm_utils/simple_target_socket.h"

class Sub_slave : public sc_module
{
public:
    /* interface */
    tlm_utils::simple_target_socket<Sub_slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
private:
    /* data */
public:
    SC_HAS_PROCESS(Sub_slave);
    Sub_slave(sc_module_name nm);
    ~Sub_slave();
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
};

Sub_slave::Sub_slave(sc_module_name nm) : sc_module(nm), tgt_skt("tgt_skt")
{
    tgt_skt.register_b_transport(this, &Sub_slave::b_transport);
}

Sub_slave::~Sub_slave()
{
}

void Sub_slave::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    int len = trans.get_data_length();
    uint32_t data;
    memcpy(&data, trans.get_data_ptr(), len);
    uint32_t addr = trans.get_address();
    cout << name() << ": receive data = 0x" << hex << data << ", addr = 0x" << hex << addr << endl;
}