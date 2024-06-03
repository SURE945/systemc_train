#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"
#include "string"
#include "extension.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Master : public sc_module
{
    private:
    static const bool CACHEABLE     = 0;
    static const bool NON_CACHEABLE = 1;

    public:
    tlm_utils::simple_initiator_socket<Master, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Master);
    Master(sc_module_name name)
    : sc_module(name)
    , init_skt("socket")
    , clk_period(5, SC_NS) {
        SC_THREAD(thread_main);
    }

    void thread_main() {
        m_qk.reset();
        write32(0x0, 0x11223344, CACHEABLE, m_qk.get_current_time());
        write32(0x0, 0x11223344, NON_CACHEABLE, m_qk.get_current_time());
    }

    private:
    void write32(uint32_t addr, uint32_t data, bool cache, sc_time delay) {
        tlm_generic_payload trans;
        trans.set_command(TLM_WRITE_COMMAND);
        trans.set_address(addr);
        trans.set_data_length(4);
        trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
        uint8_t* dataptr = reinterpret_cast<uint8_t*>(&data);
        trans.set_data_ptr(dataptr);
        //**add cache extension*********
        Cache_extension* cache_extension = new Cache_extension;
        cache_extension->set_cache(cache);
        trans.set_extension(cache_extension);
        //******************************
        init_skt->b_transport(trans, delay);
        if (trans.get_response_status() != TLM_OK_RESPONSE) {
            cout << "write32 addr = 0x" << hex << addr << ", data = 0x" << hex << data << " failed" << endl;
        }
        m_qk.set(delay);
        if(m_qk.need_sync()) m_qk.sync();
        m_qk.inc(sc_time(1, SC_NS));
        if(m_qk.need_sync()) m_qk.sync();
    }

    private:
    sc_time clk_period;
    //************declaration********
    tlm_utils::tlm_quantumkeeper m_qk;
    //************declaration********
};