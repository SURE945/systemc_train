#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "axi_phase.h"

class Master : public sc_module
{
    public:
    // tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , m_num(0)
    /*, init_skt("socket")*/ {
        SC_THREAD(thread_main);
    }

    void register_trace_file(sc_trace_file* trace_file_ptr) {
        my_trace_file = trace_file_ptr;
    }

    private:
    void end_of_elaboration() {
        sc_trace(my_trace_file, m_num, "num");
    }

    void thread_main() {
        for (int i = 0; i < 5; i++) {
            wait(10, SC_NS);
            m_num++;
        }
    }

    private:
    uint32_t m_num;
    sc_trace_file* my_trace_file;
};