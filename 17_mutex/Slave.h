#include "generic_lib.h"
#include "tlm_utils/simple_target_socket.h"

#define MEM_SIZE 10

class Slave : public sc_module
{
public:
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt_1;
private:
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
    /* data */
    uint32_t mem[MEM_SIZE];
    sc_mutex m_mutex;
public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name nm);
    ~Slave();
};

Slave::Slave(sc_module_name nm) : sc_module(nm)
, tgt_skt("tgt_skt")
, tgt_skt_1("tgt_skt_1")
{
    for (int i = 0; i < MEM_SIZE; i++) mem[i] = 0;
    tgt_skt.register_b_transport(this, &Slave::b_transport);
    tgt_skt_1.register_b_transport(this, &Slave::b_transport);
}

Slave::~Slave()
{
}

void Slave::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    int idx = trans.get_data_length();
    m_mutex.lock();
    cout << sc_time_stamp() << ": receive master[" << idx << "] transaction." << endl;
    wait(1, SC_NS);
    cout << sc_time_stamp() << ": master[" << idx << "] transaction done." << endl;
    m_mutex.unlock();
}