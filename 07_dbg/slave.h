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
    , tgt_skt("socket") {
        tgt_skt.register_transport_dbg(this, &Slave::transport_dbg_cb);
        for (int i = 0; i < 2048; i++) {mem[i] = i;}
    }

    unsigned int transport_dbg_cb(tlm_generic_payload &trans) {
        tlm_command cmd = trans.get_command();
        uint64_t    addr = trans.get_address();
        uint32_t    len = trans.get_data_length();
        uint8_t*    data_ptr = trans.get_data_ptr();

        unsigned int byte_num = (len < size - addr) ? len : size - addr;

        if (cmd == TLM_WRITE_COMMAND) {
            memcpy(&mem[addr], data_ptr, byte_num);
        } else if (cmd == TLM_READ_COMMAND) {
            memcpy(data_ptr, &mem[addr], byte_num);
        }

        return byte_num;
    }

    private:
    const static uint32_t size = 2048;
    uint8_t mem[2048];
};