#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class InterConnect : public sc_module
{
    public:
    tlm_utils::simple_initiator_socket<InterConnect, 32, tlm::tlm_base_protocol_types> init_skt;
    tlm_utils::simple_target_socket<InterConnect, 32, tlm::tlm_base_protocol_types> tgt_skt;
    public:
    SC_HAS_PROCESS(InterConnect);
    InterConnect(sc_module_name name)
    : sc_module(name)
    , init_skt("init_skt")
    , tgt_skt("tgt_skt") {
        init_skt.register_invalidate_direct_mem_ptr(this, &InterConnect::invalidate_direct_mem_ptr_cb);
        tgt_skt.register_get_direct_mem_ptr(this, &InterConnect::get_direct_mem_ptr_cb);
    }

    void invalidate_direct_mem_ptr_cb(sc_dt::uint64 start_addr, sc_dt::uint64 end_addr) {
        tgt_skt->invalidate_direct_mem_ptr(start_addr, end_addr);
    }

    bool get_direct_mem_ptr_cb(tlm_generic_payload &trans, tlm_dmi& dmi) {
        bool ans = false;
        ans = init_skt->get_direct_mem_ptr(trans, dmi);
        return ans;
    }
};