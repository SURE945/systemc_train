#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "quantum_control.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

sc_time quantum_control::m_dynamic_quantum;
bool quantum_control::m_need_break;

class Master : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , init_skt("socket")
    , clk_period(1, SC_MS)
    , m_qc("quantum_control") {
        SC_THREAD(thread_main);
        m_qc.m_dynamic_quantum = SC_ZERO_TIME;
        m_qc.m_need_break = false;
        m_qc.quantum_keeper.set_global_quantum(sc_time(250, SC_MS));
        m_qc.quantum_keeper.reset();
    }

    private:
    void thread_main() {
        wait(50, SC_MS);
        for (int idx = 0; idx < 5; idx++) {
            if (m_qc.quantum_keeper.get_global_quantum() != sc_time(250, SC_MS)) {
                m_qc.quantum_keeper.set_global_quantum(sc_time(250, SC_MS));
                m_qc.quantum_keeper.reset();
            }
            if (m_qc.m_need_break) {
                m_qc.quantum_keeper.set_global_quantum(m_qc.m_dynamic_quantum);
                m_qc.quantum_keeper.reset();
                m_qc.m_need_break = false;
            }
            sc_time const quantum = tlm::tlm_global_quantum::instance().compute_local_quantum();
            cout << "quantum = " << quantum << endl;
            int len = quantum / clk_period;
            for (int i = 0; i < len; i++) {
                sc_time local_quantum_time = m_qc.quantum_keeper.get_local_time();
                bool const side_effects = do_instruction(local_quantum_time);
                m_qc.quantum_keeper.inc(clk_period);
                if (side_effects) {break;}
            }
            m_qc.quantum_keeper.sync();
        }
    }

    bool do_instruction(sc_time& local_quantum_time) {
        int side_effects = false;
        sc_time const local_time(sc_time_stamp() + local_quantum_time);
        tlm_generic_payload trans;
        init_skt->b_transport(trans, local_quantum_time);
        m_qc.quantum_keeper.set(local_quantum_time);
        if (m_qc.quantum_keeper.need_sync() || m_qc.m_need_break) {
            side_effects = true;
        }
		std::cout << "LOCAL TIME = " << local_time << "\t";
        std::cout << "SYSTEMC TIME = " << sc_time_stamp() << std::endl;
        return side_effects;
    }

    private:
    sc_time clk_period;
    //************declaration********
    quantum_control m_qc;
    //************declaration********
};