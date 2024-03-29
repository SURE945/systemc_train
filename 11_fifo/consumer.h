#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Consumer : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Consumer, 32, tlm::tlm_base_protocol_types> init_skt;
    sc_in<bool> not_empty;

    public:
    SC_HAS_PROCESS(Consumer);
    Consumer(sc_module_name name)
    : sc_module(name)
    , init_skt("socket")
    , clk_period(5, SC_NS) {
        SC_THREAD(thread_main);
        sensitive << not_empty.pos();
    }

    void thread_main() {
        int i = 0;
        while (i < 10) {
            wait(0, SC_NS);
            if (not_empty.read()) {
                read32(0x0, m_qk.get_local_time());
                i++;
            } else {
                wait();
            }
        }
    }

    void read32(uint32_t addr, sc_time delay) {
        tlm_generic_payload trans;
        trans.set_command(TLM_READ_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        wait(clk_period);
        init_skt->b_transport(trans, delay);
        if (trans.get_response_status() != TLM_OK_RESPONSE) {
            cout << "read addr = 0x" << hex << addr << " failed" << endl;
            sc_stop();
        }
        uint8_t* dataptr = trans.get_data_ptr();
        uint32_t data = 0;
        memcpy(&data, dataptr, 4);
        cout << name() << "@[" << m_qk.get_current_time() << "][" << m_qk.get_local_time() << "]: read 0x" << hex << data << endl;
    }

    private:
    sc_time clk_period;
    tlm_utils::tlm_quantumkeeper m_qk;
};