#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Master : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , init_skt("socket") {
        SC_THREAD(thread_main);
    }

    void thread_main() {
        wait(10, SC_NS);
        tlm_generic_payload* trans = new tlm_generic_payload;
        trans->set_address(0x0);
        uint8_t* data_ptr = new uint8_t[128];
        trans->set_data_ptr(data_ptr);
        trans->set_command(TLM_READ_COMMAND);
        trans->set_data_length(128);

        uint32_t byte_num = init_skt->transport_dbg(*trans);

        for (int i = 0; i < byte_num; i++) {
            cout << "mem[" << dec << i << "] = 0x" << hex << (int)(*(data_ptr + i)) << endl;
        }

        delete [] data_ptr;
        delete trans;
    }
};