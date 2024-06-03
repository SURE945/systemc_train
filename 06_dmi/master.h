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
        m_dmi.init();
        init_skt.register_invalidate_direct_mem_ptr(this, &Master::invalidate_direct_mem_ptr_cb);
        SC_THREAD(thread_main);
    }

    void invalidate_direct_mem_ptr_cb(sc_dt::uint64 start_addr, sc_dt::uint64 end_addr) {
        m_dmi.init();
    }

    void thread_main() {
        uint32_t data_in = 0x44332211;
        write32(0x0, data_in);
        uint8_t data_out = 0;
        read8(0x0, data_out);
        read8(0x1, data_out);
        read8(0x2, data_out);
        read8(0x3, data_out);
    }

    void read32(uint64_t addr, uint32_t &val) {
        if (m_dmi.is_none_allowed()) {
            tlm_generic_payload trans;
            trans.set_address(addr);
            init_skt->get_direct_mem_ptr(trans, m_dmi);
        }
        memcpy(&val, m_dmi.get_dmi_ptr() + addr, 4);
        cout << "0x" << hex << (uint32_t)(val) << endl;
    }

    void read8(uint64_t addr, uint8_t &val) {
        if (m_dmi.is_none_allowed()) {
            tlm_generic_payload trans;
            trans.set_address(addr);
            init_skt->get_direct_mem_ptr(trans, m_dmi);
        }
        memcpy(&val, m_dmi.get_dmi_ptr() + addr, 1);
        cout << "0x" << hex << (uint32_t)(val) << endl;
    }

    void write32(uint64_t addr, uint32_t &val) {
        if (m_dmi.is_none_allowed()) {
            tlm_generic_payload trans;
            trans.set_address(addr);
            init_skt->get_direct_mem_ptr(trans, m_dmi);
        }
        memcpy(m_dmi.get_dmi_ptr() + addr, &val, 4);
    }

    private:
    tlm_dmi m_dmi;
};