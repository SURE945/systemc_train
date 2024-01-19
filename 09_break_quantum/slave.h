#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "quantum_control.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Slave : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
    public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name name)
    : sc_module(name)
    , tgt_skt("socket")
    , m_qc("quantum_control") {
        tgt_skt.register_b_transport(this, &Slave::b_transport);
        SC_METHOD(method_main)
        sensitive << event_test;
        dont_initialize();
    }

    void b_transport(tlm_generic_payload &trans, sc_time &time) {
        sc_time notify_latency = sc_time(10, SC_MS);
        if (time == sc_time(100, SC_MS)) {
            event_test.notify(time + notify_latency);
            sc_time local_quantum = tlm_global_quantum::instance().compute_local_quantum();
            if (notify_latency < local_quantum) {
                m_qc.m_need_break = true;
                m_qc.m_dynamic_quantum = notify_latency;
            }
        }
    }

    private:
    void method_main() {
        cout << this->name() << ": event notify @ " << sc_time_stamp() << endl;
    }

    private:
    sc_event event_test;
    quantum_control m_qc;

};