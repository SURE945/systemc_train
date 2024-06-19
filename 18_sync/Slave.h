#include "generic_lib.h"
#include "tlm_utils/simple_target_socket.h"

#define MEM_SIZE 10

class Slave : public sc_module
{
public:
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt_1;
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt_2;
private:
    void b_transport(tlm_generic_payload &trans, sc_time &delay);
    /* data */
    uint32_t mem[MEM_SIZE];
    sc_mutex m_mutex;
    sc_semaphore* m_sema[3];
    // sync module: a flag array, an event array and a event
    sc_event arbit;
    sc_event start[3];
    bool request[3];

public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name nm);
    ~Slave();
    void method_arbitor();
    int arbit_strategy();
};

Slave::Slave(sc_module_name nm) : sc_module(nm)
, tgt_skt("tgt_skt")
, tgt_skt_1("tgt_skt_1")
, tgt_skt_2("tgt_skt_2")
{
    for (int i = 0; i < 3; i++) m_sema[i] = new sc_semaphore(0);
    for (int i = 0; i < 3; i++) request[i] = 0;
    for (int i = 0; i < MEM_SIZE; i++) mem[i] = 0;
    tgt_skt.register_b_transport(this, &Slave::b_transport);
    tgt_skt_1.register_b_transport(this, &Slave::b_transport);
    tgt_skt_2.register_b_transport(this, &Slave::b_transport);
    SC_METHOD(method_arbitor);
    sensitive << arbit;
}

Slave::~Slave()
{
    for (int i = 0; i < 3; i++) {
        delete m_sema[i];
        m_sema[i] = NULL;
    }
}

void Slave::b_transport(tlm_generic_payload &trans, sc_time &delay)
{
    int idx = trans.get_data_length();
    request[idx] = 1;
    arbit.notify();
    wait(start[idx]);
    cout << sc_time_stamp() << ": receive master[" << idx << "] transaction." << endl;
    wait(1, SC_NS);
    cout << sc_time_stamp() << ": master[" << idx << "] transaction done." << endl;
    arbit.notify();
}

void Slave::method_arbitor()
{
    int idx = arbit_strategy();
    if (idx != -1) {
        request[idx] = 0;
        start[idx].notify();
    }
}

int Slave::arbit_strategy()
{
    int res = -1;
    for (int i = 2; i >= 0; i--) {
        if (request[i]) {
            res = i;
            break;
        }
    }
    return res;
}