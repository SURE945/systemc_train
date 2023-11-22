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
        tgt_skt.register_get_direct_mem_ptr(this, &Slave::get_direct_mem_ptr_cb);
        for (int i = 0; i < 2048; i++) {mem[0] = 0;}
    }

    bool get_direct_mem_ptr_cb(tlm_generic_payload &trans, tlm_dmi &dmi) {
        bool ans = true;
        dmi.set_dmi_ptr(&mem[0]);
        dmi.set_start_address(0x0);
        dmi.set_end_address(2048);
        dmi.allow_read_write();
        return ans;
    }

    private:
    uint8_t mem[2048];
};