#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "apb_phase.h"
#include "string"

class Master : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , m_addr(0)
    , m_data(0)
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
        if (phase == end_setup) {
            time_log("END_SETUP", trans.get_address()); m_addr = 0;
            m_slv_end_setup_evt.notify();
        }
        m_peq.notify(trans, phase, delay);
        return tlm::TLM_ACCEPTED;
    }

    private:
    void end_of_elaboration() {
        sc_trace(my_trace_file, m_addr, "ADDR");
        sc_trace(my_trace_file, m_data, "DATA");
    }

    void thread_main() {
        write32(0x11223344, 0x100);
        write32(0xffffffff, 0x200);
        write32(0x88776655, 0x300);
        wait(30, SC_NS);
        sc_stop();
    }

    void peq_cb(tlm::tlm_generic_payload &trans, const tlm::tlm_phase &phase) {
        tlm::tlm_phase phase_out;
        sc_time t_delay;
        if (phase == end_setup) {
            phase_out = begin_enable;
            time_log("BEGIN_ENABLE", trans.get_address());
            uint32_t* data_ptr_32 = reinterpret_cast<uint32_t*>(trans.get_data_ptr());
            m_data = *data_ptr_32;
            init_skt->nb_transport_fw(trans, phase_out, clk_period);
        } else if (phase == end_enable) {
            time_log("END_ENABLE", trans.get_address()); m_data = 0;
            delete trans.get_data_ptr();
            delete &trans;
            m_slv_end_enable_evt.notify();
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
        tlm::tlm_phase t_phase = begin_setup;
        sc_time t_delay = clk_period;
        time_log("BEGIN_SETUP", addr); m_addr = addr;
        init_skt->nb_transport_fw(*trans, t_phase, t_delay);
        wait(m_slv_end_setup_evt);
        wait(m_slv_end_enable_evt);
    }

    void time_log(std::string str, uint32_t addr) {
        cout << sc_time_stamp() << ":\tINIT:\t" << str << "\t0x" << hex << addr << endl;
    }

    private:
    sc_time clk_period;
    sc_event m_slv_end_setup_evt;
    sc_event m_slv_end_enable_evt;
    tlm_utils::peq_with_cb_and_phase<Master> m_peq;
    uint32_t m_addr, m_data;
    sc_trace_file* my_trace_file;
};