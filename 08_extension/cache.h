#include "systemc"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "extension.h"

using namespace std;
using namespace sc_dt;
using namespace sc_core;
using namespace tlm;

class Cache : public sc_module
{
    public:
    tlm_utils::simple_target_socket<Cache, 32, tlm::tlm_base_protocol_types> tgt_skt;
    tlm_utils::simple_initiator_socket<Cache, 32, tlm::tlm_base_protocol_types> init_skt;
    public:
    SC_HAS_PROCESS(Cache);
    Cache(sc_module_name name)
    : sc_module(name)
    , tgt_skt("socket")
    , clk_period(5, SC_NS) {
        tgt_skt.register_b_transport(this, &Cache::b_transport);
    }

    void b_transport(tlm_generic_payload &trans, sc_time &delay) {
        if (trans.get_command() == TLM_WRITE_COMMAND) {
            delay += clk_period;
            //**get cache extension*********
            Cache_extension* cache_extension;
            trans.get_extension(cache_extension);
            //******************************
            if (!cache_extension->cacheable) {
                int len = trans.get_data_length();
                uint8_t data[len];
                memcpy(data, trans.get_data_ptr(), len);
                uint32_t* dataptr32 = reinterpret_cast<uint32_t*>(data);
                cout << this->name() << ": receive data 0x" << hex << *(dataptr32) << endl;
                trans.set_response_status(TLM_OK_RESPONSE);
            } else {
                this->init_skt->b_transport(trans, delay);
            }
        }
    }

    private:
    sc_time clk_period;
};