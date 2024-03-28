#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Producer : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Producer, 32, tlm::tlm_base_protocol_types> init_skt;
    sc_in<bool> full;

    public:
    SC_HAS_PROCESS(Producer);
    Producer(sc_module_name name)
    : sc_module(name)
    , init_skt("socket")
    , clk_period(5, SC_NS) {
        SC_THREAD(thread_main);
        sensitive << full.neg();
    }

    private:
    void thread_main() {
        for (int i = 0; i < 10; i++) {
            if (!full.read()) {
                write32(0x0, i + 1, m_qk.get_local_time());
            } else {
                wait();
            }
        }
    }

    void write32(uint32_t addr, uint32_t data, sc_time delay) {
        tlm_generic_payload trans;
        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        uint8_t* dataptr = reinterpret_cast<uint8_t*>(&data);
        trans.set_data_ptr(dataptr);
        wait(clk_period);
        init_skt->b_transport(trans, delay);
        if (trans.get_response_status() != TLM_OK_RESPONSE) {
            cout << "write32 addr = 0x" << hex << addr << ", data = 0x" << hex << data << " failed" << endl;
            sc_stop();
        }
        cout << name() << "@[" << m_qk.get_current_time() << "][" << m_qk.get_local_time() << "]: write 0x" << hex << data << endl;

    }

    private:
    sc_time clk_period;
    tlm_utils::tlm_quantumkeeper m_qk;
};