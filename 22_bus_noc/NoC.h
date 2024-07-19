#include "generic_lib.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"

class NoC : public sc_module
{
public:
    tlm_utils::simple_initiator_socket<NoC, 32, tlm::tlm_base_protocol_types> init_skt_0;
    tlm_utils::simple_initiator_socket<NoC, 32, tlm::tlm_base_protocol_types> init_skt_1;
    tlm_utils::simple_target_socket<NoC, 32, tlm::tlm_base_protocol_types> tgt_skt;
private:
    /* data */
public:
    SC_HAS_PROCESS(NoC);
    NoC(sc_module_name nm);
    ~NoC();
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
};

NoC::NoC(sc_module_name nm)
: sc_module(nm)
, tgt_skt("tgt_skt")
, init_skt_0("init_skt_0")
, init_skt_1("init_skt_1")
{
    tgt_skt.register_b_transport(this, &NoC::b_transport);
}

void NoC::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    uint32_t addr = trans.get_address();
    if (addr == 0x0) {
        init_skt_0->b_transport(trans, delay);
    } else if (addr == 0x10) {
        init_skt_1->b_transport(trans, delay);
    } else {
        trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
    }
}

NoC::~NoC()
{
}
