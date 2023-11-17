#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "axi_phase.h"
#include "string"

class Master : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , m_req(0)
    , m_resp(0)
    , init_skt("socket")
    , m_peq("peq", this, &Master::peq_cb)
    , clk_period(5, SC_NS) {
        init_skt.register_nb_transport_bw(this, &Master::nb_transport_bw);
        SC_THREAD(thread_main);
    }

    void register_trace_file(sc_trace_file* trace_file_ptr) {
        my_trace_file = trace_file_ptr;
    }

    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &delay) {
        m_peq.notify(trans, phase, delay);
        return tlm::TLM_ACCEPTED;
    }

    private:
    void end_of_elaboration() {
        sc_trace(my_trace_file, m_req, "REQ");
        sc_trace(my_trace_file, m_resp, "RESP");
    }

    void thread_main() {
        write32(0x11223344, 0x100);
    }

    void peq_cb(tlm::tlm_generic_payload &trans, const tlm::tlm_phase &phase) {
        tlm::tlm_phase phase_out;
        sc_time t_delay;
        if (phase == tlm::END_REQ) {
            m_slv_end_req_evt.notify();
        }  else if (phase == tlm::BEGIN_RESP) {
            time_log("BEGIN_RESP", trans.get_address()); m_resp = 1;
            phase_out = tlm::END_RESP;
            init_skt->nb_transport_fw(trans, phase_out, clk_period);
        } else if (phase == tlm::END_RESP) {
            time_log("END_RESP", trans.get_address()); m_resp = 0;
            delete trans.get_data_ptr();
            delete &trans;
        }
    }

    void write32(uint32_t addr, uint32_t data) {
        tlm::tlm_generic_payload* trans = new tlm::tlm_generic_payload;
        uint32_t* data_ptr_32 = new uint32_t;
        *(data_ptr_32) = data;
        uint8_t* data_ptr_8 = reinterpret_cast<uint8_t*>(data_ptr_32);
        trans->set_address(addr);
        trans->set_command(tlm::TLM_WRITE_COMMAND);
        trans->set_data_ptr(data_ptr_8);
        trans->set_data_length(4);
        trans->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        tlm::tlm_phase t_phase = tlm::BEGIN_REQ;
        sc_time t_delay = SC_ZERO_TIME;
        time_log("BEGIN_REQ", addr); m_req = 1;
        init_skt->nb_transport_fw(*trans, t_phase, t_delay);
        wait(m_slv_end_req_evt);
        time_log("END_REQ", addr); m_req = 0;
    }

    void time_log(std::string str, uint32_t addr) {
        cout << sc_time_stamp() << ":\tINIT:\t" << str << "\t0x" << hex << addr << endl;
    }

    private:
    sc_time clk_period;
    sc_event m_slv_end_req_evt;
    tlm_utils::peq_with_cb_and_phase<Master> m_peq;
    bool m_req, m_resp;
    sc_trace_file* my_trace_file;
};