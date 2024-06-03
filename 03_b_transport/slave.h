#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

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
    , clk_period(5, SC_NS) {
        tgt_skt.register_b_transport(this, &Slave::b_transport);
    }

    void b_transport(tlm_generic_payload &trans, sc_time &delay) {
        if (trans.get_command() == TLM_WRITE_COMMAND) {
            delay += clk_period;
            int len = trans.get_data_length();
            uint8_t data[len];
            memcpy(data, trans.get_data_ptr(), len);
            cout << "target: "; for (int i = 0; i < len; i++) {cout << data[i];} cout << endl;
            trans.set_response_status(TLM_OK_RESPONSE);
        }
    }

    private:
    sc_time clk_period;
};