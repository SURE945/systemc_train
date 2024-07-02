#include "generic_lib.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "Sub_slave.h"

class Slave : public sc_module
{
public:
    /* interfaces */
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
private:
    /* internal interfaces */
    tlm_utils::simple_initiator_socket<Slave, 32, tlm_base_protocol_types> init_skt;
private:
    /* data */
    Sub_slave sub_slave;
public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name nm);
    ~Slave();
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
};

Slave::Slave(sc_module_name nm) : sc_module(nm)
, tgt_skt("tgt_skt")
, init_skt("init_skt")
, sub_slave("sub_slave")
{
    tgt_skt.register_b_transport(this, &Slave::b_transport);
    init_skt.bind(sub_slave.tgt_skt);
}

Slave::~Slave()
{
}

void Slave::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    cout << name() << ": receive and forward trans" << endl;
    init_skt->b_transport(trans, delay);
    cout << name() << ": forward trans done" << endl;
}
