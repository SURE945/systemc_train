#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_cb_and_phase.h"
#include "apb_phase.h"
#include "string"

class Slave : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Slave, 32, tlm::tlm_base_protocol_types> tgt_skt;
    public:
    SC_HAS_PROCESS(Slave);
    Slave(sc_module_name name)
    : sc_module(name)
    , m_num(0)
    , tgt_skt("socket")
    , m_peq("peq", this, &Slave::peq_cb)
    , clk_period(5, SC_NS) {
        tgt_skt.register_nb_transport_fw(this, &Slave::nb_transport_fw);
    }

    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &delay) {
        m_peq.notify(trans, phase, delay);
        return tlm::TLM_ACCEPTED;
    }

    private:
    void peq_cb(tlm::tlm_generic_payload &trans, const tlm::tlm_phase &phase) {
        tlm::tlm_phase phase_out;
        sc_time t_delay;
        if (phase == begin_setup) {
            phase_out = end_setup;
            t_delay = SC_ZERO_TIME;
            tgt_skt->nb_transport_bw(trans, phase_out, t_delay);
        }  else if (phase == begin_enable) {
            phase_out = end_enable;
            t_delay = SC_ZERO_TIME;
            tgt_skt->nb_transport_bw(trans, phase_out, t_delay);
        }
    }

    void time_log(std::string str, uint32_t addr) {
        cout << sc_time_stamp() << ":\tTGT:\t" << str << "\t0x" << hex << addr << endl;
    }

    private:
    sc_time clk_period;
    tlm_utils::peq_with_cb_and_phase<Slave> m_peq;
    uint32_t m_num;
};